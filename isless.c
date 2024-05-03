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

unsigned char isGreater(unsigned char a,
                        unsigned char b) {
    unsigned char x,y;

    x = ~a & b;
    y = a & ~b;

    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);

    return ~isZero(~x & y) & 1;
}

unsigned char isLess(unsigned char x,
                     unsigned char y) {
    return (!isEqual(x,y)) && (!isGreater(x,y));
}

//  Other ops:
unsigned char isNotEqual(unsigned char x, 
                         unsigned char y) {
    return !isEqual(x,y);
}

unsigned char isLessOrEqual(unsigned char x,
                            unsigned char y) {
    return isEqual(x,y) || isLess(x,y);
}

unsigned char isGreaterOrEqual(unsigned char x,
                               unsigned char y) {
    return isEqual(x,y) || isGreater(x,y);
}


int main() {

    printf("%d\n", isLess(123,33));
    printf("%d\n", isLess(0,33));
    printf("%d\n", isLess(33,33));
    printf("%d\n", isLess(3,33));

    return 0;
}

