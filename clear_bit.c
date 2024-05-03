#include <stdio.h>

int main() {
    unsigned char z = 0xBD;

    z = z & (~0x08);
    printf("%x\n", z);

    return 0;
}

