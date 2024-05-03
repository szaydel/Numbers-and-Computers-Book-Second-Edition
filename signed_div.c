#include <stdio.h>

signed char signed_div2(signed char n, 
                        signed char m,
                        signed char *r) {
    unsigned char i, sign=0, rsign=0;

    if ((n < 0) && (m > 0)) {
        sign = rsign = 1;
        n = -n;
    }
    if ((n > 0) && (m < 0)) {
        sign = 1;
        m = -m;
    }
    if ((n < 0) && (m < 0)) {
        rsign = 1;
        n = -n;
        m = -m;
    }

    *r = 0;

    for(i=0; i<8; i++) {
        *r = (*r << 1) + ((n & 0x80) != 0);
        n <<= 1;

        if ((*r-m) >= 0) {
            n |= 1;
            *r -= m;
        } 
    }

    if (sign) n = -n;
    if (rsign) *r = -*r;

    return n;
}

int main() {
    signed char n=123, m=4;
    signed char q,r;

    n=123; m=4;
    q = signed_div2(n,m, &r);
    printf("n=%d, m=%d, q=%d, r=%d\n", n,m,q,r);
    n=-123; m=4;
    q = signed_div2(n,m, &r);
    printf("n=%d, m=%d, q=%d, r=%d\n", n,m,q,r);
    n=123; m=-4;
    q = signed_div2(n,m, &r);
    printf("n=%d, m=%d, q=%d, r=%d\n", n,m,q,r);
    n=-123; m=-4;
    q = signed_div2(n,m, &r);
    printf("n=%d, m=%d, q=%d, r=%d\n", n,m,q,r);
}

