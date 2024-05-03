#include <stdio.h>

int main() {
    unsigned char z = 0xB5;

    z = z | 0x08;
    printf("%x\n", z);
    z |= 0x40;
    printf("%x\n", z);

    return 0;
}

