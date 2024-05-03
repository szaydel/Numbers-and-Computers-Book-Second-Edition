//
//  file:  lns.c
//
//  Logarithmic number system routines
//
//  RTK, 22-May-2016
//  Last update:  29-May-2016
//
//  For "Numbers and Computers", 2nd Ed.
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <inttypes.h>

#define K 8
#define L 22
#define LNS_ZERO 0x80000000
#define LNS_SIGN 0x40000000
#define LNS_MASK 0x3FFFFFFF

typedef uint32_t lns_t;

double lg2(double x) {
    return log(x) / log(2.0);
}


//  Representation
lns_t to_lns(double x) {
    double e = lg2(fabs(x));
    uint8_t s = (x >= 0.0) ? 0 : 1;
    lns_t ans;
    
    if (x == 0.0) return LNS_ZERO; 
    ans = (lns_t)(LNS_MASK & (uint32_t)floor(pow(2.0,L)*e + 0.5));
    if (s) { ans |= LNS_SIGN; }
    return ans;
}

double to_double(lns_t x) {
    uint32_t z = (uint32_t)(LNS_ZERO & x);
    uint32_t s = (uint32_t)(LNS_SIGN & x);
    int32_t p  = (uint32_t)(LNS_MASK & x);
    double m;

    p = (0x20000000 & p) ? 0xC0000000+p : p;
    if (z != 0) { return 0.0; }
    m = pow(2.0, (double)p * pow(2.0,-L));
    return (double)(s == 0) ? m : -m;
}


//  Multiplication and division
unsigned char lns_isneg(lns_t x) {
    return (LNS_SIGN & x) != 0;
}

unsigned char lns_iszero(lns_t x) {
    return (LNS_ZERO & x) != 0;
}

lns_t lns_abs(lns_t x) {
    if (lns_iszero(x))
        return x;
    return LNS_MASK & x;
}

lns_t lns_negate(lns_t x) {
    if (lns_iszero(x)) 
        return LNS_ZERO;
    if (lns_isneg(x)) 
        return LNS_MASK & x;
    return LNS_SIGN | x;
}

lns_t lns_multiply(lns_t a, lns_t b) {
    if (lns_iszero(a)) return LNS_ZERO;
    if (lns_iszero(b)) return LNS_ZERO;

    if (lns_isneg(a) == lns_isneg(b))
        return LNS_MASK & (lns_abs(a) + lns_abs(b));
    else
        return lns_negate(LNS_MASK & (lns_abs(a) + lns_abs(b)));
}

lns_t lns_divide(lns_t a, lns_t b) {
    if (lns_iszero(a)) return LNS_ZERO;
    if (lns_iszero(b)) return LNS_ZERO;

    if (lns_isneg(a) == lns_isneg(b))
        return LNS_MASK & (lns_abs(a) - lns_abs(b));
    else
        return lns_negate(LNS_MASK & (lns_abs(a) - lns_abs(b)));
}


//  Comparison operators
int32_t lns_exponent(lns_t x) {
    int32_t e = LNS_MASK & x;
    return (0x20000000 & e) ? 0xC0000000+e : e;
}

int8_t lns_compare(lns_t a, lns_t b) {
    int32_t ea = lns_exponent(a);
    int32_t eb = lns_exponent(b);

    if (lns_iszero(a) && lns_iszero(b))
        return 0;
    if (lns_iszero(a))
        return (lns_isneg(b)) ? 1 : -1; 
    if (lns_iszero(b))
        return (lns_isneg(a)) ? -1 : 1;

    if ((lns_isneg(a)==1) && (lns_isneg(b)==0))
        return -1;
    if ((lns_isneg(a)==0) && (lns_isneg(b)==1))
        return 1;

    if (lns_isneg(a) && lns_isneg(b))
        return (ea == eb) ? 0  
                          : (eb < ea) ? -1
                                      :  1;
    else
        return (ea == eb) ? 0  
                          : (ea < eb) ? -1
                                      :  1;
}

unsigned char lns_equal(lns_t a, lns_t b) {
    return (lns_compare(a,b) == 0);
}

unsigned char lns_less(lns_t a, lns_t b) {
    return (lns_compare(a,b) == -1);
}

unsigned char lns_greater(lns_t a, lns_t b) {
    return (lns_compare(a,b) == 1);
}


//  Addition and subtraction
double fminus(double ea, double eb) {
    return pow(2.0, ea + lg2(1.0 - pow(2.0, eb-ea)));
}

double fplus(double ea, double eb) {
    return pow(2.0, ea + lg2(1.0 + pow(2.0, eb-ea)));
}

lns_t lns_add_mag(lns_t a, lns_t b) {
    double ea = (double)(LNS_MASK & a) * pow(2.0,-L);
    double eb = (double)(LNS_MASK & b) * pow(2.0,-L);
    return to_lns(fplus(ea,eb));
}

lns_t lns_sub_mag(lns_t a, lns_t b) {
    double ea = (double)(LNS_MASK & a) * pow(2.0,-L);
    double eb = (double)(LNS_MASK & b) * pow(2.0,-L);
    return to_lns(fminus(ea,eb));
}

lns_t lns_add(lns_t a, lns_t b) {
    lns_t A = lns_abs(a);
    lns_t B = lns_abs(b);

    if (lns_iszero(a)) return b;
    if (lns_iszero(b)) return a;
    if (lns_equal(a,b))
        return lns_multiply(a, to_lns(2.0));

    if ((lns_isneg(a)==0) && (lns_isneg(b)==0)) {
        if (lns_greater(A,B))
            return lns_add_mag(A,B);
        else
            return lns_add_mag(B,A);
    }
    if ((lns_isneg(a)==1) && (lns_isneg(b)==1)) {
        if (lns_greater(A,B))
            return lns_negate(lns_add_mag(A,B));
        else
            return lns_negate(lns_add_mag(B,A));
    }
    if ((lns_isneg(a)==0) && (lns_isneg(b)==1)) {
        if (lns_greater(A,B))
            return lns_sub_mag(A,B);
        else
            return lns_negate(lns_sub_mag(B,A));
    }
    if ((lns_isneg(a)==1) && (lns_isneg(b)==0)) {
        if (lns_greater(A,B))
            return lns_negate(lns_sub_mag(A,B));
        else
            return lns_sub_mag(B,A);
    }
}

lns_t lns_sub(lns_t a, lns_t b) {
    lns_t A = lns_abs(a);
    lns_t B = lns_abs(b);

    if (lns_iszero(a)) return lns_negate(b);
    if (lns_iszero(b)) return a;
    if (lns_equal(a,b)) return LNS_ZERO;

    if ((lns_isneg(a)==0) && (lns_isneg(b)==0)) {
        if (lns_greater(A,B))
            return lns_sub_mag(A,B);
        else
            return lns_negate(lns_sub_mag(B,A));
    }
    if ((lns_isneg(a)==1) && (lns_isneg(b)==1)) {
        if (lns_greater(A,B))
            return lns_negate(lns_sub_mag(A,B));
        else
            return lns_sub_mag(B,A);
    }
    if ((lns_isneg(a)==0) && (lns_isneg(b)==1)) {
        if (lns_greater(A,B))
            return lns_add_mag(A,B);
        else
            return lns_add_mag(B,A);
    }
    if ((lns_isneg(a)==1) && (lns_isneg(b)==0)) {
        if (lns_greater(A,B))
            return lns_negate(lns_add_mag(A,B)); 
        else
            return lns_negate(lns_add_mag(B,A));
    }
}


int main(int argc, char *argv[]) {
    lns_t a,b,c;
    double x=1,y=0;
    char s[20];

    while (x != 0) {
        printf("x: "); scanf("%lf", &x);
        printf("y: "); scanf("%lf", &y);
        a = to_lns(x);
        b = to_lns(y);
        printf("a=%f, b=%f, a+b=%f, a-b:%f\n", x,y, to_double(lns_add(a,b)), to_double(lns_sub(a,b)));
    }

    //x = 10.43;
    //y =  1.237;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f, a+b=%f\n", x,y, to_double(lns_add(a,b)));

    //x = 10.43;
    //y =  1.237;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f, a-b=%f\n", x,y, to_double(lns_sub(a,b)));

    //y = 10.43;
    //x =  1.237;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f, a+b=%f\n", x,y, to_double(lns_add(a,b)));

    //y = 10.43;
    //x =  1.237;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f, a-b=%f\n", x,y, to_double(lns_sub(a,b)));

    //x = 0.0;
    //y = 0.0;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 0.0;
    //y = 0.011;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 0.0;
    //y = -0.011;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //y = 0.0;
    //x = 0.011;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //y = 0.0;
    //x = -0.011;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 34.011;
    //y = 34.011;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = -34.011;
    //y = 34.011;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 34.011;
    //y = -34.011;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 0.0411;
    //y = 0.0411;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = -0.0411;
    //y = 0.0411;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 0.0411;
    //y = -0.0411;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 0.0412;
    //y = 0.0411;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 0.0412;
    //y = 0.0413;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = -0.0412;
    //y = -0.0411;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = -0.0412;
    //y = -0.0413;
    //a = to_lns(x);
    //b = to_lns(y);
    //printf("a=%f, b=%f:  a==b:%d, a<b:%d, a>b:%d\n", x,y, lns_equal(a,b), lns_less(a,b), lns_greater(a,b));

    //x = 0.00312345;  a = to_lns(x);
    //y = 0.010000;    b = to_lns(y);
    //c = lns_divide(a,b);
    //printf("%0.8f (%x) / %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -0.00312345;  a = to_lns(x);
    //y = 0.010000;    b = to_lns(y);
    //c = lns_divide(a,b);
    //printf("%0.8f (%x) / %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 0.00312345;  a = to_lns(x);
    //y = -0.010000;    b = to_lns(y);
    //c = lns_divide(a,b);
    //printf("%0.8f (%x) / %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -0.00312345;  a = to_lns(x);
    //y = -0.010000;    b = to_lns(y);
    //c = lns_divide(a,b);
    //printf("%0.8f (%x) / %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 0.00312345;  a = to_lns(x);
    //y = 10000;    b = to_lns(y);
    //c = lns_divide(a,b);
    //printf("%0.8f (%x) / %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -0.00312345;  a = to_lns(x);
    //y = 10000;    b = to_lns(y);
    //c = lns_divide(a,b);
    //printf("%0.8f (%x) / %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 0.00312345;  a = to_lns(x);
    //y = -10000;    b = to_lns(y);
    //c = lns_divide(a,b);
    //printf("%0.8f (%x) / %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -0.00312345;  a = to_lns(x);
    //y = -10000;    b = to_lns(y);
    //c = lns_divide(a,b);
    //printf("%0.8f (%x) / %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 123.45;  a = to_lns(x);
    //y = 10.0;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.3f (%x) * %0.3f (%x) = %0.3f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -123.45;  a = to_lns(x);
    //y = 10.0;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.3f (%x) * %0.3f (%x) = %0.3f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 123.45;  a = to_lns(x);
    //y = -10.0;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.3f (%x) * %0.3f (%x) = %0.3f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -123.45;  a = to_lns(x);
    //y = -10.0;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.3f (%x) * %0.3f (%x) = %0.3f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 0.00312345;  a = to_lns(x);
    //y = 0.010000;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.8f (%x) * %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -0.00312345;  a = to_lns(x);
    //y = 0.010000;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.8f (%x) * %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 0.00312345;  a = to_lns(x);
    //y = -0.010000;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.8f (%x) * %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -0.00312345;  a = to_lns(x);
    //y = -0.010000;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.8f (%x) * %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 0.00312345;  a = to_lns(x);
    //y = 10000;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.8f (%x) * %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -0.00312345;  a = to_lns(x);
    //y = 10000;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.8f (%x) * %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = 0.00312345;  a = to_lns(x);
    //y = -10000;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.8f (%x) * %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //x = -0.00312345;  a = to_lns(x);
    //y = -10000;    b = to_lns(y);
    //c = lns_multiply(a,b);
    //printf("%0.8f (%x) * %0.8f (%x) = %0.8f (%x)\n", x,a, y,b, to_double(c),c);

    //a = to_lns(-1234.5);
    //b = lns_negate(a);
    //printf("%0.3f -> %0.3f\n", to_double(a), to_double(b));
    //a = to_lns(1234.5);
    //b = lns_negate(a);
    //printf("%0.3f -> %0.3f\n", to_double(a), to_double(b));
    
    //x = 3.14159265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = -3.14159265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = 314159265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = -314159265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = 314159.265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = -314159.265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = 0.0;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = 1.0;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = 1.00001;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = 0.00314159265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = -0.00314159265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.9f in LNS is %x which cast back is %0.9f\n", x, a, y);

    //x = 0.00000314159265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.14f in LNS is %x which cast back is %0.14f\n", x, a, y);

    //x = -0.00000314159265;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.14f in LNS is %x which cast back is %0.14f\n", x, a, y);

    //x = 3.14159265e-9;
    //a = to_lns(x);
    //y = to_double(a);
    //printf("%0.7g in LNS is %x which cast back is %0.7g\n", x, a, y);

    return 0;
}

