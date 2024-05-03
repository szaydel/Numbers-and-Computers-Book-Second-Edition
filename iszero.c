#include <stdio.h>

unsigned char isZero(unsigned char x) {
    unsigned char ans;

    return ~(((x & (1<<7)) >> 7) |
             ((x & (1<<6)) >> 6) |
             ((x & (1<<5)) >> 5) |
             ((x & (1<<4)) >> 4) |
             ((x & (1<<3)) >> 3) |
             ((x & (1<<2)) >> 2) |
             ((x & (1<<1)) >> 1) |
              (x & 1)) & 1;
}

unsigned char isEqual(unsigned char a,
                      unsigned char b) {
    return isZero(a^b);
}


int main() {

    printf("%d\n", isZero(11));
    printf("%d\n", isZero(128));
    printf("%d\n", isZero(1));
    printf("%d\n\n", isZero(0));

    printf("%d\n", isEqual(123,33));
    printf("%d\n", isEqual(0,33));
    printf("%d\n", isEqual(33,33));

    return 0;
}

