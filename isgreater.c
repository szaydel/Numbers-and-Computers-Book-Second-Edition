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

unsigned char isGreater(unsigned char a,
                        unsigned char b) {
    unsigned char x,y;

    x = ~a & b;
    y = a & ~b;

    //printf("x = %d\n", x);
    x = x | (x >> 1);
    //printf("x = %d\n", x);
    x = x | (x >> 2);
    //printf("x = %d\n", x);
    x = x | (x >> 4);
    //printf("x = %d\n", x);

    return ~isZero(~x & y) & 1;
}

int main() {

    printf("%d\n", isGreater(11,33));
    printf("%d\n", isGreater(110,33));
    //printf("%d\n", isGreater(1,33));
    //printf("%d\n", isGreater(0,33));
    //printf("%d\n", isGreater(33,33));

    return 0;
}

