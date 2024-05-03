//
//  file:  mpfr_exp.c
//
//  Calculate values for comparing performance with mpmath.
//
//  RTK, 19-Jul-2016
//  Last update:  19-Jul-2016
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <gmp.h>
#include <mpfr.h>

double getTime() {
    struct timeval tv; 
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}


int main(int argc, char *argv[]) {
    unsigned int i, p, n;
    mpfr_t s, t;
    double sss, eee; 
    
    // get limits
    p = (unsigned int)atoi(argv[1]);
    n = (unsigned int)atoi(argv[2]);

    // calls to exp(0.5) for a specific precision
    sss = getTime();

    mpfr_init2(t, p);
    mpfr_set_d(t, 0.5, MPFR_RNDN);
    mpfr_init2(s, p);

    for(i=0; i < n; i++) {
        mpfr_exp(s, t, MPFR_RNDN);
    }

    //printf("\n");
    //mpfr_out_str(stdout, 10, 0, s, MPFR_RNDN);
    //printf("\n");

    mpfr_clear(s);
    mpfr_clear(t);

    eee = getTime();

    printf("\nruntime = %0.16f\n\n", eee-sss);

    return 0;
}

#if 0
  mpfr_init2 (t, 200);
  mpfr_set_d (t, 1.0, MPFR_RNDD);
  mpfr_init2 (s, 200);
  mpfr_set_d (s, 1.0, MPFR_RNDD);
  mpfr_init2 (u, 200);
  for (i = 1; i <= 100; i++)
    {
      mpfr_mul_ui (t, t, i, MPFR_RNDU);
      mpfr_set_d (u, 1.0, MPFR_RNDD);
      mpfr_div (u, u, t, MPFR_RNDD);
      mpfr_add (s, s, u, MPFR_RNDD);
    }
  printf ("Sum is ");
  mpfr_out_str (stdout, 10, 0, s, MPFR_RNDD);
  putchar ('\n');
#endif

