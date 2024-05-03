#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <mpfr.h>

void pp(mpfr_t x0, mpfr_t x1, mpfr_t x2) {
    printf("       --------------------\n");
    printf("   x0: ");
    mpfr_out_str(stdout, 10, 0, x0, MPFR_RNDN);
    printf("\n");
    printf("       --------------------\n");
    printf("   x1: ");
    mpfr_out_str(stdout, 10, 0, x1, MPFR_RNDN);
    printf("\n");
    printf("       --------------------\n");
    printf("   x2: ");
    mpfr_out_str(stdout, 10, 0, x2, MPFR_RNDN);
    printf("\n\n");
}

int main (int argc, char *argv[]) {
    unsigned int i, p, n;
    mpfr_t x0,x1,x2,u,v;
    mpfr_t r, one;

    p = (unsigned int)atoi(argv[1]);
    n = (unsigned int)atoi(argv[2]);

    mpfr_init2(x0, p);
    mpfr_init2(x1, p);
    mpfr_init2(x2, p);
    mpfr_init2(r,  p);
    mpfr_init2(one,p);
    mpfr_init2(u,  p);
    mpfr_init2(v,  p);

    mpfr_set_d(one,1, MPFR_RNDN);
    mpfr_set_d(r, 3.8, MPFR_RNDN);
    mpfr_set_d(x0, 0.25, MPFR_RNDN);
    mpfr_set_d(x1, 0.25, MPFR_RNDN);
    mpfr_set_d(x2, 0.25, MPFR_RNDN);

    //  Initial values
    printf("Initial values:\n");

    for(i=0; i < 8; i++) {
        mpfr_sub(u, one, x0, MPFR_RNDN);  // x0 = r*x0*(1.0 - x0)
        mpfr_mul(x0, x0, u, MPFR_RNDN);
        mpfr_mul(x0, r, x0, MPFR_RNDN);

        mpfr_mul(u, r, x1, MPFR_RNDN);    // x1 = r*x1 - r*x1*x1
        mpfr_mul(v, r, x1, MPFR_RNDN);
        mpfr_mul(v, v, x1, MPFR_RNDN);
        mpfr_sub(x1, u, v, MPFR_RNDN);

        mpfr_mul(u, r, x2, MPFR_RNDN);    // x2 = x2*(r - r*x2)
        mpfr_sub(v, r, u, MPFR_RNDN);
        mpfr_mul(x2, x2, v, MPFR_RNDN);
    }
    pp(x0,x1,x2);

    //  Iteration
    for(i=0; i < n-16; i++) {
        mpfr_sub(u, one, x0, MPFR_RNDN);  // x0 = r*x0*(1.0 - x0)
        mpfr_mul(x0, x0, u, MPFR_RNDN);
        mpfr_mul(x0, r, x0, MPFR_RNDN);

        mpfr_mul(u, r, x1, MPFR_RNDN);    // x1 = r*x1 - r*x1*x1
        mpfr_mul(v, r, x1, MPFR_RNDN);
        mpfr_mul(v, v, x1, MPFR_RNDN);
        mpfr_sub(x1, u, v, MPFR_RNDN);

        mpfr_mul(u, r, x2, MPFR_RNDN);    // x2 = x2*(r - r*x2)
        mpfr_sub(v, r, u, MPFR_RNDN);
        mpfr_mul(x2, x2, v, MPFR_RNDN);
    }

    //  Final values
    printf("\n\nFinal values:\n");

    for(i=0; i < 8; i++) {
        mpfr_sub(u, one, x0, MPFR_RNDN);  // x0 = r*x0*(1.0 - x0)
        mpfr_mul(x0, x0, u, MPFR_RNDN);
        mpfr_mul(x0, r, x0, MPFR_RNDN);

        mpfr_mul(u, r, x1, MPFR_RNDN);    // x1 = r*x1 - r*x1*x1
        mpfr_mul(v, r, x1, MPFR_RNDN);
        mpfr_mul(v, v, x1, MPFR_RNDN);
        mpfr_sub(x1, u, v, MPFR_RNDN);

        mpfr_mul(u, r, x2, MPFR_RNDN);    // x2 = x2*(r - r*x2)
        mpfr_sub(v, r, u, MPFR_RNDN);
        mpfr_mul(x2, x2, v, MPFR_RNDN);
    }
    pp(x0,x1,x2);

    return 0;
}

