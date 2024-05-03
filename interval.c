#include <fenv.h>
#include <stdio.h>
#include <math.h>

//  Compile with:
//      gcc interval.c -o interval -lm -frounding-math

typedef struct {
    double a;
    double b;
} interval_t;

char *pp(interval_t x) {
    static char s[100];

    sprintf(s, "[%0.16f, %0.16f]", x.a, x.b);
    return s;
}

interval_t interval(double a, double b) {
    interval_t ans;

    ans.a = a;
    ans.b = b;
    return ans;
}

interval_t int_recip(interval_t x) {
    interval_t inv;

    if ((x.a < 0.0) && (x.b > 0.0)) {
        inv.a = -exp(1000.0);
        inv.b =  exp(1000.0);
    } else {
        fesetround(FE_DOWNWARD);
        inv.a = 1.0 / x.b;
        fesetround(FE_UPWARD);
        inv.b = 1.0 / x.a;
        fesetround(FE_TONEAREST);
    }

    return inv;
}

interval_t int_add(interval_t x, interval_t y) {
    interval_t sum;

    fesetround(FE_DOWNWARD);
    sum.a = x.a + y.a;
    fesetround(FE_UPWARD);
    sum.b = x.b + y.b;
    fesetround(FE_TONEAREST);
    return sum;
}

interval_t int_sub(interval_t x, interval_t y) {
    interval_t diff;

    fesetround(FE_DOWNWARD);
    diff.a = x.a - y.b;
    fesetround(FE_UPWARD);
    diff.b = x.b - y.a;
    fesetround(FE_TONEAREST);
    return diff;
}

interval_t int_mul(interval_t x, interval_t y) {
    interval_t prod;
    double t;

    fesetround(FE_DOWNWARD);
    prod.a = x.a * y.a;
    t = x.a * y.b;
    if (t < prod.a) prod.a = t;
    t = x.b * y.a;
    if (t < prod.a) prod.a = t;
    t = x.b * y.b;
    if (t < prod.a) prod.a = t;

    fesetround(FE_UPWARD);
    prod.b = x.a * y.a;
    t = x.a * y.b;
    if (t > prod.b) prod.b = t;
    t = x.b * y.a;
    if (t > prod.b) prod.b = t;
    t = x.b * y.b;
    if (t > prod.b) prod.b = t;

    fesetround(FE_TONEAREST);
    return prod;
}

interval_t int_div(interval_t x, interval_t y) {
    
    return int_mul(x,int_recip(y));
}

interval_t int_pow(interval_t x, int n) {
    interval_t ans;

    if ((n%2) == 1) {
        fesetround(FE_DOWNWARD);
        ans.a = pow(x.a,n);
        fesetround(FE_UPWARD);
        ans.b = pow(x.b,n);
    } else {
        if (x.a >= 0) {
            fesetround(FE_DOWNWARD);
            ans.a = pow(x.a,n);
            fesetround(FE_UPWARD);
            ans.b = pow(x.b,n);
        } else {
            if (x.b < 0) {
                fesetround(FE_DOWNWARD);
                ans.a = pow(x.b,n);
                fesetround(FE_UPWARD);
                ans.b = pow(x.a,n);
            } else {
                ans.a = 0.0;
                fesetround(FE_UPWARD);
                ans.b = pow(x.a,n);
                if (pow(x.b,n) > ans.b) ans.b = pow(x.b,n);
            }
        }
    }

    fesetround(FE_TONEAREST);
    return ans;
}

interval_t int_neg(interval_t x) {
    interval_t ans;

    fesetround(FE_DOWNWARD);
    ans.a = -x.b;
    fesetround(FE_UPWARD);
    ans.b = -x.a;
    fesetround(FE_TONEAREST);
    return ans;
}

interval_t int_abs(interval_t x) {
    interval_t ans;

    if (x.b * x.a >= 0) {
        fesetround(FE_DOWNWARD);
        ans.a = (fabs(x.b) < fabs(x.a)) ? fabs(x.b) : fabs(x.a);
        fesetround(FE_UPWARD);
        ans.b = (fabs(x.b) < fabs(x.a)) ? fabs(x.a) : fabs(x.b);
    } else {
        ans.a = 0.0;
        fesetround(FE_UPWARD);
        ans.b = (fabs(x.b) < fabs(x.a)) ? fabs(x.a) : fabs(x.b);
    }
    fesetround(FE_TONEAREST);
    return ans;
}

unsigned char int_eq(interval_t x, interval_t y) {
    return (x.a==y.a) && (x.b==y.b);
}

unsigned char int_le(interval_t x, interval_t y) {
    return (x.a <= y.a) && (x.b <= y.b);
}

double int_dist(interval_t x, interval_t y) {
    double a,b;
    a = fabs(x.a - y.a);
    b = fabs(x.b - y.b);
    return (a>b) ? a : b;
}

double fact(int n) {
    return (n==1) ? 1.0 : n*fact(n-1);
}

double int_sin_lo(double x, int n) {
    int i, m = (x < 0) ? 2*n : 2*n+1;
    double ans=0.0;

    for(i=1; i <= m; i++) {
        ans += pow(-1,i-1) * pow(x,2*i-1) / fact(2*i-1);
    }

    return ans;
}

double int_sin_hi(double x, int n) {
    int i, m = (x < 0) ? 2*n : 2*n+1;
    double ans=0.0;

    for(i=1; i <= m+1; i++) {
        ans += pow(-1,i-1) * pow(x,2*i-1) / fact(2*i-1);
    }

    return ans;
}

interval_t int_sin(interval_t x) {
    int n=8;
    interval_t ans;

    fesetround(FE_DOWNWARD);
    ans.a = int_sin_lo(x.a, n);
    fesetround(FE_UPWARD);
    ans.b = int_sin_hi(x.b, n);
    fesetround(FE_TONEAREST);
    return ans;
}

double int_cos_lo(double x, int n) {
    int i, m = (x < 0) ? 2*n : 2*n+1;
    double ans=0.0;

    for(i=1; i <= m+1; i++) {
        ans += pow(-1,i) * pow(x,2*i) / fact(2*i);
    }

    return 1.0 + ans;
}

double int_cos_hi(double x, int n) {
    int i, m = (x < 0) ? 2*n : 2*n+1;
    double ans=0.0;

    for(i=1; i <= m; i++) {
        ans += pow(-1,i) * pow(x,2*i) / fact(2*i);
    }

    return 1.0+ans;
}

interval_t int_cos(interval_t x) {
    int n=8;
    interval_t ans;

    fesetround(FE_DOWNWARD);
    ans.a = int_cos_lo(x.b, n);
    fesetround(FE_UPWARD);
    ans.b = int_cos_hi(x.a, n);
    fesetround(FE_TONEAREST);
    return ans;
}

int main() {
    double a=12.23, sa=0.11;
    double b=11.224, sb=0.007;
    double y,sy;
    interval_t ia,ib,iy;
    interval_t ix;

    //ia = interval(a-sa, a+sa);
    //ib = interval(b-sb, b+sb);
    //iy = int_add(ia,ib);
    //y = a+b;
    //sy = sqrt(sa*sa + sb*sb);
    //printf("Error propagation, a+b: [%0.16g, %0.16g]\n", y-sy, y+sy);
    //printf("Intervals, a+b        : %s\n", pp(iy));

    //printf("\n");

    //ia = interval(a-sa, a+sa);
    //ib = interval(b-sb, b+sb);
    //iy = int_sub(ia,ib);
    //y = a-b;
    //sy = sqrt(sa*sa + sb*sb);
    //printf("Error propagation, a-b: [%0.16g, %0.16g]\n", y-sy, y+sy);
    //printf("Intervals, a-b        : %s\n", pp(iy));

    //ia = int_recip(interval(-1,1));
    //printf("%s\n", pp(ia));

    //double x = 3.1415;
    //double sx = 0.1;
    //interval_t ix = interval(x-sx,x+sx);
    //interval_t iy;
    //double y = 3*x+4;
    //double sy = sqrt(9*sx*sx);

    //printf("Intervals:\n");
    //iy = int_add(int_mul(interval(3,3),ix),interval(4,4));
    //printf("iy = %s\n", pp(iy));
    //printf("\nError propagation:\n");
    //printf("iy = [%0.16g, %0.16g]\n", y-sy, y+sy);
    

    //interval_t x = {-exp(1), exp(1)+0.1};
    //interval_t y = {1.5, 3.1};

    //printf("%s\n", pp(x));
    //printf("%s\n", pp(y));
    //printf("%s\n", pp(int_add(x,y)));
    //printf("%s\n", pp(int_sub(x,y)));
    //printf("%s\n", pp(int_mul(x,y)));
    //printf("%s\n", pp(int_div(x,y)));

    //printf("\n");

    //ix = interval(-1.5, 2.7);
    //iy = int_mul(ix,ix);
    //ix = int_mul(iy,ix);
    //printf("%s\n", pp(ix));
    //ix = interval(-2.7182818284590451, -2.6182818284590454);
    //ia = int_mul(ix,ix);
    //iy = int_mul(ix,ia);
    //printf("interval    = %s\n", pp(ix));
    //printf("interval**3 = %s\n", pp(int_pow(ix,3)));
    //printf("times 3     = %s\n", pp(iy));
    //printf("interval**4 = %s\n", pp(int_pow(ix,4)));
    //printf("times 4     = %s\n", pp(int_mul(ix,iy)));
    //printf("\n\n");
    //ix = interval(2.7182818284590451, 2.8182818284590454);
    //ia = int_mul(ix,ix);
    //iy = int_mul(ix,ia);
    //printf("interval    = %s\n", pp(ix));
    //printf("interval**3 = %s\n", pp(int_pow(ix,3)));
    //printf("times 3     = %s\n", pp(iy));
    //printf("interval**4 = %s\n", pp(int_pow(ix,4)));
    //printf("times 4     = %s\n", pp(int_mul(ix,iy)));
    //printf("\n\n");
    //ix = interval(-2.7182818284590451, 2.8182818284590454);
    //ia = int_mul(ix,ix);
    //iy = int_mul(ix,ia);
    //printf("interval    = %s\n", pp(ix));
    //printf("interval**3 = %s\n", pp(int_pow(ix,3)));
    //printf("times 3     = %s\n", pp(iy));
    //printf("interval**4 = %s\n", pp(int_pow(ix,4)));
    //printf("times 4     = %s\n", pp(int_mul(ix,iy)));

    //ix = interval(-2.7182818284590451, 2.8182818284590454);
    //iy = interval(2.5182818284590451, 2.9182818284590454);
    //printf("int x   = %s\n", pp(ix));
    //printf("int y   = %s\n", pp(iy));
    //printf("int_neg = %s\n", pp(int_neg(ix))); 
    //printf("int_abs = %s\n", pp(int_abs(ix)));
    //printf("int_eq  = %d\n", int_eq(ix,iy));
    //printf("int_le  = %d\n", int_le(ix,iy));
    //printf("int_dist= %0.16f\n", int_dist(ix,iy));

    ix = interval(0.7852981625, 0.7854981625);
    printf("ix                = %s\n", pp(ix));
    printf("sin(ix)           = %s\n", pp(int_sin(ix)));
    printf("sin(0.7852981625) = %0.16f\n", sin(0.7852981625));
    printf("sin(0.7854981625) = %0.16f\n", sin(0.7854981625));
    printf("cos(ix)           = %s\n", pp(int_cos(ix)));
}

