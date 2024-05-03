#include <stdio.h>
#include <mpfr.h>
#include <mpfi.h>
#include <mpfi_io.h>

int main() {
    mpfi_t x, one, t, y;

    mpfr_set_default_prec(64);
    mpfi_init_set_str(x, "[0, 1]", 10);
    mpfi_init_set_str(one, "[1,1]", 10);    

    mpfi_init(t);
    mpfi_sub(t, one, x);
    mpfi_init(y);
    mpfi_mul(y, t, x);

    printf("y = ");
    mpfi_out_str(stdout, 10, 0, y);
    printf("\n");

    mpfi_clear(y);
    mpfi_clear(t);
    mpfi_clear(one);

    mpfi_init(t);
    mpfi_mul(t, x, x);
    mpfi_init(y);
    mpfi_sub(y, x, t);
 
    printf("y = ");
    mpfi_out_str(stdout, 10, 0, y);
    printf("\n");
}

