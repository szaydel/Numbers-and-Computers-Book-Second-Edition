#include <stdio.h>

unsigned char rotr(unsigned char x, int n) {
    return (x >> n) | (x << 8 - n);
}

unsigned char rotl(unsigned char x, int n) {
    return (x << n) | (x >> 8 - n);
}

int main() {
    unsigned char x = 0xAB;

    printf("%X\n", x);
    printf("%X\n", rotr(x,1));
    printf("%X\n", rotl(x,1));

    return 0;
}

