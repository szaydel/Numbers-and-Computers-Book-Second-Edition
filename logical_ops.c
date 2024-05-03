#include <stdio.h>

void pp(unsigned char z) {
    printf("%3d (%02x)\n", z, z);
}

int main() {
    unsigned char z;

    z = 189 & 222;  pp(z);
    z = 189 | 222;  pp(z);
    z = 189 ^ 222;  pp(z);
    z = z ^ 222;    pp(z);
    z = ~z;         pp(z);

    return 0;
}

