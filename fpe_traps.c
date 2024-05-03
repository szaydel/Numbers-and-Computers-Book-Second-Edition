#include <stdio.h>
#include <signal.h>
#include <fenv.h>

int main() {
    float c;

    feclearexcept(FE_ALL_EXCEPT);
    c = 0.0/0.0;
    if (fetestexcept(FE_INVALID) != 0)
        printf("FE_INVALID happened\n");
    
    feclearexcept(FE_ALL_EXCEPT);
    c = 2.0/3.0;
    if (fetestexcept(FE_INEXACT) != 0)
        printf("FE_INEXACT happened\n");

    feclearexcept(FE_ALL_EXCEPT);
    c = 1.17549435e-38 / 3.0;
    if (fetestexcept(FE_UNDERFLOW) != 0)
        printf("FE_UNDERFLOW happened\n");

    feclearexcept(FE_ALL_EXCEPT);
    c = 3.40282347e+38 * 3.40282347e+38;
    if (fetestexcept(FE_OVERFLOW) != 0)
        printf("FE_OVERFLOW happend\n");

    feclearexcept(FE_ALL_EXCEPT);
    c = 1.0 / 0.0;
    if (fetestexcept(FE_DIVBYZERO) != 0)
        printf("FE_DIVBYZERO happened\n");
}

