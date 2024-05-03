#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define MASK_SIGN 0x80000000
#define DEG2RAD 0.0174532925
#define PI 3.141592653589793
#define EXP_ITER_MAX 50
#define EXP_TOLERANCE 0.000001

typedef int32_t fix_t;
typedef int64_t fixx_t;
int32_t M=0,N=31;
fix_t PIh, PId, d2r, r2d;
fix_t f90,f180,f270,f360;
fix_t st1,st2,st3,st4;
fix_t p0,p1,p2,p3,p4,p5;

//  sine lookup table, [0,90] degrees
fix_t sin_tbl[91];

void Q_init(int32_t m) {
    int i;

    M = ((m >= 0) && (m < 32)) ? m : 0;
    N = 32 - (m+1);

    for(i=0; i<91; i++) {
        sin_tbl[i] = Q_to_fixed(sin(i*DEG2RAD));
    }

    PId = Q_to_fixed(2*PI);
    PIh = Q_to_fixed(PI/2);
    d2r = Q_to_fixed(DEG2RAD);
    r2d = Q_to_fixed(1.0/DEG2RAD);

    f90 = Q_to_fixed(90*DEG2RAD);
    f180= Q_to_fixed(180*DEG2RAD);
    f270= Q_to_fixed(270*DEG2RAD);
    f360= Q_to_fixed(360*DEG2RAD);

    st1 = Q_to_fixed(1.0/6);
    st2 = Q_to_fixed(1.0/120);
    st3 = Q_to_fixed(1.0/5040);
    st4 = Q_to_fixed(1.0/362880);

    //  5th degree poly fit to sine
    p0 = Q_to_fixed(1.17708643e-05);
    p1 = Q_to_fixed(9.99655930e-01);
    p2 = Q_to_fixed(2.19822760e-03);
    p3 = Q_to_fixed(-1.72068031e-01);
    p4 = Q_to_fixed(5.87493727e-03);
    p5 = Q_to_fixed(5.79939713e-03);
}

fix_t Q_to_fixed(double z) {
    return (fix_t)floor(pow(2.0,N)*z + 0.5);
}

double Q_to_double(fix_t z) {
    return (double)z * pow(2.0,-N);
}

//
// no overflow detection versions, fastest
//
fix_t Q_add(fix_t x, fix_t y) { return x+y; }
fix_t Q_sub(fix_t x, fix_t y) { return x-y; }

fix_t Q_mul(fix_t x, fix_t y) {
    fixx_t ans = (fixx_t)x * (fixx_t)y;

    ans += 1<<(N-1);
    return (fix_t)(ans >> N);
}

fix_t Q_div(fix_t x, fix_t y) {
    fixx_t xx,yy;

    xx = (fixx_t)x << N;
    xx += (fixx_t)y / 2;
    
    return xx / (fixx_t)y;
}

//
//  overflow detection
//
fix_t Q_add_over(fix_t x, fix_t y) {
    uint32_t ux = x;
    uint32_t uy = y;
    uint32_t s = ux + uy;
    
    if ((ux & MASK_SIGN) == (uy & MASK_SIGN)) {
        if ((s & MASK_SIGN) != (ux & MASK_SIGN)) {
            printf("Q_add_over: overflow!\n");
            return 0;
        }
    }

    return x+y;
}

fix_t Q_sub_over(fix_t x, fix_t y) {
    uint32_t ux = x;
    uint32_t uy = y;
    uint32_t s = ux - uy;

    if ((ux & MASK_SIGN) != (uy & MASK_SIGN)) {
        if ((s & MASK_SIGN) != (ux & MASK_SIGN)) {
            printf("Q_sub_over: overflow!\n");
            return 0;
        }
    }

    return x-y;
}

fix_t Q_mul_over(fix_t x, fix_t y) {
    fixx_t xx,yy,tt;
    uint32_t ux = x;
    uint32_t uy = y;

    xx = (fixx_t)x;
    yy = (fixx_t)y;
    tt = xx * yy;
    tt += 1<<(N-1);
    tt >>= N;

    //  overflow - sign of answer != sign x if sign x = sign y
    if ((ux & MASK_SIGN) == (uy & MASK_SIGN)) {
        if (((fix_t)tt & MASK_SIGN) != (ux & MASK_SIGN)) {
            printf("Q_mul_over: overflow!\n");
            return 0;
        }
    }

    return (fix_t)tt;
}

void sin_val_sign(int32_t *angle, int32_t *sign) {

    if ((*angle >=0) && (*angle <= f90)) {
        // quadrant I
        *sign = 1;
    } else {
        if ((*angle > f90) && (*angle <= f180)) {
            // quadrant II
            *sign = 1;
            *angle = f180-*angle;
        } else {
            if ((*angle > f180) && (*angle <= f270)) {
                // quadrant III
                *sign = -1;
                *angle = *angle-f180;
            } else {
                // quadrant IV
                *sign = -1;
                *angle = f360-*angle;
            }
        }
    }
}

//
//  Trig using table
//      argument [0,2pi] radians
//
fix_t Q_sin_table(fix_t x) {
    int32_t sign;
    fix_t angle;

    if (x < 0) x += PId;
    sin_val_sign(&x, &sign);

    angle = Q_mul(x,r2d);
    angle += 1 << (N-1);
    return sign*sin_tbl[angle >> N];
}

fix_t Q_cos_table(fix_t x) {
    return Q_sin_table(Q_sub(PIh,x));
}

fix_t Q_tan_table(fix_t x) {
    return Q_div(Q_sin_table(x), Q_cos_table(x));
}

//
//  Trig using Taylor series
//
fix_t Q_sin_taylor(fix_t x) {
    fix_t x3,x5,x7,x9;
    fix_t ans;
    int32_t sign;

    if (x < 0) x += PId;
    sin_val_sign(&x, &sign);

    x3 = Q_mul(Q_mul(x,x),x);
    x5 = Q_mul(Q_mul(x3,x),x);
    x7 = Q_mul(Q_mul(x5,x),x);
    x9 = Q_mul(Q_mul(x7,x),x);

    ans = x;
    if (x3 != 0) ans -= Q_mul(st1,x3);
    if (x5 != 0) ans += Q_mul(st2,x5);
    if (x7 != 0) ans += Q_mul(st3,x7);
    if (x9 != 0) ans += Q_mul(st4,x9);

    return sign*ans;
}

fix_t Q_cos_taylor(fix_t x) {
    return Q_sin_taylor(Q_sub(PIh,x));
}

fix_t Q_tan_taylor(fix_t x) {
    return Q_div(Q_sin_taylor(x), Q_cos_taylor(x));
}

//
//  Trig using 5th degree polynomial fit
//
fix_t Q_sin_poly(fix_t x) {
    fix_t x2,x3,x4,x5;
    fix_t ans;
    int32_t sign;

    if (x < 0) x += PId;
    sin_val_sign(&x, &sign);

    x2 = Q_mul(x,x);
    x3 = Q_mul(x2,x);
    x4 = Q_mul(x3,x);
    x5 = Q_mul(x4,x);

    ans = p0 + Q_mul(p1,x) + Q_mul(p2,x2) + Q_mul(p3,x3) + 
          Q_mul(p4,x4) + Q_mul(p5,x5);
    return sign*ans;
}

fix_t Q_cos_poly(fix_t x) {
    return Q_sin_poly(Q_sub(PIh,x));
}

fix_t Q_tan_poly(fix_t x) {
    return Q_div(Q_sin_poly(x), Q_cos_poly(x));
}


//
//  Other functions: sqrt, log, exp
//
fix_t Q_sqrt(fix_t n) {
    fix_t a;
    int32_t k;

    if (n <= 0) return -(1<<N);
    if (n == (1<<N)) return (1<<N);

    a = n >> 1;

    for(k=0; k < 10; k++)
        a = Q_mul(1 << (N-1),Q_add(Q_div(n,a),a));

    return a;
}

fix_t Q_exp(fix_t x) {
    int32_t i;
    fix_t e, c, t, tol;

    tol = Q_to_fixed(EXP_TOLERANCE);
    i = 0;

    c = 1 << N;
    e = 1 << N;
    t = x;
    
    do { 
        e = Q_add(e,t);
        c = Q_add(c, 1<<N);
        t = Q_mul(t, Q_div(x,c));
        i++;
    } while ((abs(t) > tol) && (i < EXP_ITER_MAX));

    return e;
}

fix_t Q_log(fix_t x) {
    fix_t a;
    int32_t k;

    if (x <= 0) return -(1<<N);
    if (x == (1<<N)) return 0;

    a = x >> 1;

    for(k=0; k < 7; k++) {
        a = Q_add(a, Q_mul(x,Q_exp(-a)));
        a = Q_sub(a,1<<N);
    }

    return a;
}


int main() {
    int i;
    fix_t a,b,c;
    double pi = 3.14159265358979;
    double e = exp(1), f;

    //Q_init(2);  // [-4, 3.9999999...]
    //printf("Q2.29:\n");
    //printf("    %0.20f is %d\n", pi, (a=Q_to_fixed(pi)));
    //printf("    going back gives %0.20f\n", Q_to_double(a));

    //Q_init(20);  // [-1048576, 1048575.9995117188]
    //printf("Q20.11:\n");
    //printf("    %0.20f is %d\n", pi, (a=Q_to_fixed(pi)));
    //printf("    going back gives %0.20f\n", Q_to_double(a));

    //Q_init(2);
    //a = Q_to_fixed(pi);
    //b = Q_to_fixed(pi);
    //c = Q_add(a,b);
    //printf("Q2.29: pi+pi = %d (%0.10f)\n", c, Q_to_double(c));
    //c = Q_add_over(a,b);
    //printf("Q2.29: pi+pi = %d (%0.10f)\n", c, Q_to_double(c));
    //a = Q_to_fixed(-pi);
    //c = Q_sub(a,b);
    //printf("Q2.29: -pi-pi= %d (%0.10f)\n", c, Q_to_double(c));
    //c = Q_sub_over(a,b);
    //printf("Q2.29: -pi-pi= %d (%0.10f)\n", c, Q_to_double(c));

    //Q_init(20);
    //a = Q_to_fixed(pi);
    //b = Q_to_fixed(pi);
    //c = Q_add(a,b);
    //printf("Q20.11: pi+pi = %d (%0.10f)\n", c, Q_to_double(c));
    //c = Q_to_fixed(e);
    //printf("Q20.11: pi-pi = %d (%0.10f)\n", Q_sub(a,b), Q_to_double(Q_sub(a,b)));
    //printf("Q20.11: pi/pi = %d (%0.10f)\n", Q_div(a,b), Q_to_double(Q_div(a,b)));
    //printf("Q20.11: pi/e  = %d (%0.10f)\n", Q_div(a,c), Q_to_double(Q_div(a,c)));

    //Q_init(2);
    //a = Q_to_fixed(pi);
    //c = Q_to_fixed(e);
    //printf("Q2.29: pi/e  = %d (%0.10f)\n", Q_div(a,c), Q_to_double(Q_div(a,c)));
    //printf("Q2.29: e/pi  = %d (%0.10f)\n", Q_div(c,a), Q_to_double(Q_div(c,a)));
    //b = Q_to_fixed(-pi);
    //a = Q_to_fixed(-e);
    //printf("Q2.29: -pi/e = %d (%0.10f)\n", Q_div(b,c), Q_to_double(Q_div(b,c)));
    //printf("Q2.29: -pi/-e= %d (%0.10f)\n", Q_div(b,a), Q_to_double(Q_div(b,a)));

    //Q_init(4);
    //a = Q_to_fixed(pi);
    //b = Q_to_fixed(e);
    //printf("Q4.27: pi*e  = %d (%0.10f)\n", Q_mul(a,b), Q_to_double(Q_mul(a,b)));

    //Q_init(3);
    //a = Q_to_fixed(pi);
    //b = Q_to_fixed(e);
    //c = Q_mul(a,b);
    //printf("Q3.28: pi*e  = %d (%0.10f)\n", c, Q_to_double(c));
    //c = Q_mul_over(a,b);
    //printf("Q3.28: pi*e  = %d (%0.10f)\n", c, Q_to_double(c));

    //printf("\n");

    //printf("pi = %0.14f\n", pi);
    //Q_init(2);
    //a = Q_to_fixed(pi);
    //printf("Q2.29 : pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);
    //Q_init(3);
    //a = Q_to_fixed(pi);
    //printf("Q3.28 : pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);
    //Q_init(4);
    //a = Q_to_fixed(pi);
    //printf("Q4.27 : pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);
    //Q_init(5);
    //a = Q_to_fixed(pi);
    //printf("Q5.26 : pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);
    //Q_init(10);
    //a = Q_to_fixed(pi);
    //printf("Q10.21: pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);
    //Q_init(15);
    //a = Q_to_fixed(pi);
    //printf("Q15.16: pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);
    //Q_init(20);
    //a = Q_to_fixed(pi);
    //printf("Q20.11: pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);
    //Q_init(25);
    //a = Q_to_fixed(pi);
    //printf("Q25.6 : pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);
    //Q_init(30);
    //a = Q_to_fixed(pi);
    //printf("Q30.1 : pi = %0.14f, delta = %0.14f\n", Q_to_double(a), Q_to_double(a)-pi);

    //printf("\n");

    //Q_init(2);
    //a = Q_to_fixed(pi-3);
    //b = Q_to_fixed(pi-3);
    //
    //for(i=0; i<9; i++)
    //    b = Q_mul(b,a,1);

    //printf("Q2.29: (pi-3)^10 = %0.14f (bias corrected)\n", Q_to_double(b));

    //Q_init(2);
    //a = Q_to_fixed(pi-3);
    //b = Q_to_fixed(pi-3);
    //
    //for(i=0; i<9; i++)
    //    b = Q_mul(b,a,0);

    //printf("Q2.29: (pi-3)^10 = %0.14f (not corrected)\n", Q_to_double(b));
    //printf("     : (pi-3)^10 = %0.14f\n", pow((pi-3),10));

    //printf("\n");

    //Q_init(10);

    //for(i=0; i<360; i++) {
    //    a = Q_to_fixed(i*DEG2RAD);
    //    printf("Table: sin(%0.4f) = %0.4f (%0.4f)\n", Q_to_double(a), Q_to_double(Q_sin_table(a)),
    //        sin(i*DEG2RAD));
    //}

    //printf("\n");

    //for(i=0; i<360; i++) {
    //    a = Q_to_fixed(i*DEG2RAD);
    //    printf("Table: cos(%0.4f) = %0.4f (%0.4f)\n", Q_to_double(a), Q_to_double(Q_cos_table(a)),
    //        cos(i*DEG2RAD));
    //}

    //printf("\n");

    //a = Q_to_fixed(45*DEG2RAD);
    //printf("Table: tan(%0.4f) = %0.4f (%0.4f)\n", 45*DEG2RAD, Q_to_double(Q_tan_table(a)), tan(45*DEG2RAD));

    //printf("\n");

    //Q_init(7);

    //for(i=0; i<360; i++) {
    //    a = Q_to_fixed(i*DEG2RAD);
    //    printf("Taylor: sin(%0.4f) = %0.4f (%0.4f)\n", Q_to_double(a), Q_to_double(Q_sin_taylor(a)),
    //        sin(i*DEG2RAD));
    //}

    //printf("\n");

    //for(i=0; i<360; i++) {
    //    a = Q_to_fixed(i*DEG2RAD);
    //    printf("Taylor: cos(%0.4f) = %0.4f (%0.4f)\n", Q_to_double(a), Q_to_double(Q_cos_taylor(a)),
    //        cos(i*DEG2RAD));
    //}

    //Q_init(7);

    //for(i=0; i<360; i++) {
    //    a = Q_to_fixed(i*DEG2RAD);
    //    printf("Poly: sin(%0.4f) = %0.4f (%0.4f)\n", Q_to_double(a), Q_to_double(Q_sin_poly(a)),
    //        sin(i*DEG2RAD));
    //}

    //printf("\n");

    //for(i=0; i<360; i++) {
    //    a = Q_to_fixed(i*DEG2RAD);
    //    printf("Poly: cos(%0.4f) = %0.4f (%0.4f)\n", Q_to_double(a), Q_to_double(Q_cos_poly(a)),
    //        cos(i*DEG2RAD));
    //}

    //printf("\n");

    //Q_init(10);

    //srand(time(NULL));

    //for(i=0; i<100; i++) {
    //    f = 1000.0*((double)rand()/RAND_MAX);
    //    a = Q_to_fixed(f);
    //    printf("%0.8f %0.8f %0.8f\n", f, sqrt(f), Q_to_double(Q_sqrt(a)));
    //}

    //Q_init(7);

    //for(i=0; i<360; i++) {
    //    a = Q_to_fixed(i*DEG2RAD);
    //    printf("%0.8f %0.8f %0.8f %0.8f\n", sin(i*DEG2RAD), Q_to_double(Q_sin_table(a)),
    //        Q_to_double(Q_sin_taylor(a)), Q_to_double(Q_sin_poly(a)));
    //}

    //Q_init(7);

    //for(i=0; i<360; i++) {
    //    a = Q_to_fixed(i*DEG2RAD);
    //    printf("%0.8f %0.8f %0.8f %0.8f\n", cos(i*DEG2RAD), Q_to_double(Q_cos_table(a)),
    //        Q_to_double(Q_cos_taylor(a)), Q_to_double(Q_cos_poly(a)));
    //}

    //Q_init(7);

    //a = Q_to_fixed(2.0);
    //printf("exp(%0.8f) = %0.8f (%0.8f)\n", 2.0, Q_to_double(Q_exp(a)), exp(2.0));
    //a = Q_to_fixed(4.0);
    //printf("exp(%0.8f) = %0.8f (%0.8f)\n", 4.0, Q_to_double(Q_exp(a)), exp(4.0));
    //a = Q_to_fixed(-4.0);
    //printf("exp(%0.8f) = %0.8f (%0.8f)\n", -4.0, Q_to_double(Q_exp(a)), exp(-4.0));
    //a = Q_to_fixed(-5.0);
    //printf("exp(%0.8f) = %0.8f (%0.8f)\n", -5.0, Q_to_double(Q_exp(a)), exp(-5.0));

    //Q_init(10);

    //srand(time(NULL));

    //for(i=0; i<100; i++) {
    //    f = -4.0 + 8.0*((double)rand()/RAND_MAX);
    //    a = Q_to_fixed(f);
    //    printf("%0.8f %0.8f %0.8f\n", f, exp(f), Q_to_double(Q_exp(a)));
    //}

    //printf("\n");

    //a = Q_to_fixed(2.0);
    //printf("log(%0.8f) = %0.8f (%0.8f)\n", 2.0, Q_to_double(Q_log(a)), log(2.0));
    //a = Q_to_fixed(4.0);
    //printf("log(%0.8f) = %0.8f (%0.8f)\n", 4.0, Q_to_double(Q_log(a)), log(4.0));

    //Q_init(7);
    //srand(time(NULL));

    //for(i=0; i<100; i++) {
    //    f = 2*3.14159265*((double)rand()/RAND_MAX);
    //    a = Q_to_fixed(f);
    //    printf("%0.8f %0.8f %0.8f %0.8f %0.8f\n", f, sin(f), Q_to_double(Q_sin_table(a)),
    //        Q_to_double(Q_sin_taylor(a)), Q_to_double(Q_sin_poly(a)));
    //}

    //Q_init(10);

    //srand(time(NULL));

    //for(i=0; i<100; i++) {
    //    f = 0.5 + 9.5*((double)rand()/RAND_MAX);
    //    a = Q_to_fixed(f);
    //    printf("%0.8f %0.8f %0.8f\n", f, log(f), Q_to_double(Q_log(a)));
    //}

    Q_init(10);

    srand(time(NULL));

    for(i=0; i<100; i++) {
        f = -4.0 + 8.0*((double)rand()/RAND_MAX);
        a = Q_to_fixed(f);
        printf("%0.8f %0.8f\n", f, Q_to_double(a));
    }
}

