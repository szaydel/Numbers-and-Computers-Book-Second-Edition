#include <stdio.h>

unsigned char div2(unsigned char n, 
                   unsigned char m,
                   unsigned char *r) {
    unsigned char i;

    *r = 0;

    for(i=0; i<8; i++) {
        *r = (*r << 1) + ((n & 0x80) != 0);
        n <<= 1;

        if ((*r-m) >= 0) {
            n |= 1;
            *r -= m;
        } 
        //printf("%d   %d   %d   %d\n", i,*r,n,m);
    }

    return n;
}


unsigned char div1(unsigned char n, 
                   unsigned char m,
                   unsigned char *r) {
    unsigned char q=0;

    *r = n;

    while (*r > m) {
        q++;
        *r -= m;
    }

    return q;
}


int main() {
    unsigned char n=123, m=4;
    unsigned char q,r;

    q = div1(n,m, &r);
    printf("quotient = %d, remainder = %d\n", q,r);

    q = div2(n,m, &r);
    printf("quotient = %d, remainder = %d\n", q,r);
    printf("n = %d\n", n);
}

