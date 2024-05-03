#include <stdio.h>
#include <gmp.h>

int main() {
    mpz_t A;

    mpz_init(A);

    mpz_set_str(A, "1234567890123456789", 0);
    mpz_out_str(NULL, 10, A);
    printf("\n");
    mpz_out_str(NULL, 2, A);
    printf("\n");

    mpz_clear(A);
}

