#include <stdio.h>

unsigned char parity(unsigned char x) {
    x = (x >> 4) ^ x;
    x = (x >> 2) ^ x;
    x = (x >> 1) ^ x;

    return x & 1;
}

int main() {
    unsigned char x = 0x5d;

    printf("%d\n", parity(x));
    return 0;
}

