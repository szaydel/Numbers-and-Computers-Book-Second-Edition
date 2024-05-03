#include <stdio.h>

unsigned char sqr(unsigned char n) {
    unsigned char c=0, p=1;

    while (n >= p) {
        n -= p;
        p += 2;
        c++;
        //printf("n=%d, p=%d, c=%d\n", n,p,c);
    }

    return c;
}


int main() {
    unsigned char n=64;

    printf("n=%d, sqrt(n)=%d\n", n, sqr(n));
    printf("%d\n", n);
}

