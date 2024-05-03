#include <stdio.h>
#include <math.h>
#include <string.h>

const char *ppx(unsigned int x) {
    static char b[36];
    unsigned int z, i=0;
    b[0] = '\0';
    for (z = (1<<31); z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
        if ((i == 0) || (i == 8))
            strcat(b, " ");
        i++;
    }
    return b;
}

typedef union {
    float f;
    int d;
} sp_t;

int fp_compare(float a, float b) {
    sp_t x,y;

    x.f=a;
    y.f=b;

    if ((x.d == (int)0x80000000) &&
        (y.d == 0)) return 0;
    if ((y.d == (int)0x80000000) &&
        (x.d == 0)) return 0;
    
    if (x.d == y.d) return 0;
    if (x.d < y.d) return -1;
    return 1;
}

int fp_eq(float a, float b) {
    return fp_compare(a,b) == 0;
}

int fp_lt(float a, float b) {
    return fp_compare(a,b) == -1;
}

int fp_gt(float a, float b) {
    return fp_compare(a,b) == 1;
}

int main() {
    sp_t x;
    float a,b;
    unsigned int t;

    x.f = 0.5;
    printf("%1.9f = %s = %d\n", x.f, ppx(x.d), x.d);
    x.d++;
    printf("%1.9f = %s = %d\n", x.f, ppx(x.d), x.d);
    x.d -= 2;
    printf("%1.9f = %s = %d\n", x.f, ppx(x.d), x.d);
    x.f = -x.f;
    printf("%1.9f = %s = %d\n", x.f, ppx(x.d), x.d);

    printf("\n");

    x.f = 0.0;
    printf("%1.9f = %s = %d\n", x.f, ppx(x.d), x.d);
    x.d--; 
    printf("%1.9f = %s = %d\n", x.f, ppx(x.d), x.d);

    printf("\n\n");

    a = 3.13;  b = 3.13;
    printf(" 3.13 = 3.13 : %d %d\n", fp_eq(a,b), a==b);

    a = 3.1;   b = 3.13;
    printf(" 3.10 < 3.13 : %d %d\n", fp_lt(a,b), a<b);

    a = 3.2;   b = 3.13;
    printf(" 3.20 > 3.13 : %d %d\n", fp_gt(a,b), a>b);

    a = -1;  b = 1;
    printf("-1.00 < 1.00 : %d %d\n", fp_lt(a,b), a<b);

    x.d = (int)0x80000000;
    a = x.f;
    b = 0.0;
    printf("-0: %1.9f = %s = %d\n", x.f, ppx(x.d), x.d);
    printf("-0 == 0: %d %d\n", fp_eq(a,b), a==b);
    printf("0 == -0: %d %d\n", fp_eq(b,a), b==a);

    a = exp(100);
    b = -exp(100);
    printf("+inf = +inf: %d %d\n", fp_eq(a,a), a==a);
    printf("+inf > -inf: %d %d\n", fp_lt(a,b), a>b);
    printf("-inf = -inf: %d %d\n", fp_gt(b,b), b==b);
}

