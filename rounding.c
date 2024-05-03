#include <stdio.h>
#include <fenv.h>
#include <math.h>
#include <string.h>

// compile: gcc rounding.c -o rounding -lm -frounding-math

const char *pp(unsigned int x) {
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
    unsigned int d;
} fp_t;

int main() {
    fp_t fp;

    printf("\nFE_TONEAREST:\n");
    fesetround(FE_TONEAREST);
    fp.f = exp(1.1);
    printf("   exp(1.1) = (%s) %0.8f\n", pp(fp.d), fp.f);
    fp.f = -exp(1.1);
    printf("  -exp(1.1) = (%s) %0.8f\n\n", pp(fp.d), fp.f);

    printf("FE_UPWARD:\n");
    fesetround(FE_UPWARD);
    fp.f = exp(1.1);
    printf("   exp(1.1) = (%s) %0.8f\n", pp(fp.d), fp.f);
    fp.f = -exp(1.1);
    printf("  -exp(1.1) = (%s) %0.8f\n\n", pp(fp.d), fp.f);

    printf("FE_DOWNWARD:\n");
    fesetround(FE_DOWNWARD);
    fp.f = exp(1.1);
    printf("   exp(1.1) = (%s) %0.8f\n", pp(fp.d), fp.f);
    fp.f = -exp(1.1);
    printf("  -exp(1.1) = (%s) %0.8f\n\n", pp(fp.d), fp.f);

    printf("FE_TOWARDZERO:\n");
    fesetround(FE_TOWARDZERO);
    fp.f = exp(1.1);
    printf("   exp(1.1) = (%s) %0.8f\n", pp(fp.d), fp.f);
    fp.f = -exp(1.1);
    printf("  -exp(1.1) = (%s) %0.8f\n\n", pp(fp.d), fp.f);
}

