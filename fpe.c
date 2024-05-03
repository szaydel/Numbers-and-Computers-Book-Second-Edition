#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fenv.h>

void fpe_handler(int sig) {
    
    //  Ignore non-floating-point exceptions
    if (sig != SIGFPE) return;

    printf("Floating point exception!\n");
    exit(1);
}

int main() {
    int i;
    float a=1, b=0, c;
    double d;

    //  Enable exception trapping example
    //feenableexcept(FE_INVALID  | FE_DIVBYZERO |
    //               FE_OVERFLOW | FE_UNDERFLOW);
    //signal(SIGFPE,  fpe_handler);
    //
    //c = a/b;

    //  Test for exceptions
    feclearexcept(FE_ALL_EXCEPT);
    c = 0.0/0.0;
    i = fetestexcept(FE_INVALID);
    if (i != 0)
        printf("FE_INVALID happened!\n");
    
    feclearexcept(FE_ALL_EXCEPT);
    c = 2.0/3.0;
    i = fetestexcept(FE_INEXACT);
    if (i != 0)
        printf("FE_INEXACT happened!\n");

    feclearexcept(FE_ALL_EXCEPT);
    c = 1.17549435e-38 / 3.0;
    i = fetestexcept(FE_UNDERFLOW);
    if (i != 0)
        printf("FE_UNDERFLOW happened!\n");

    feclearexcept(FE_ALL_EXCEPT);
    c = 3.40282347e+38 * 3.40282347e+38;
    feraiseexcept(FE_OVERFLOW);
    i = fetestexcept(FE_OVERFLOW);
    if (i != 0)
        printf("FE_OVERFLOW happend!\n");
}

