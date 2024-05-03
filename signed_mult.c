#include <stdio.h>

signed short signed_mult2(signed char n, signed char m) {
    unsigned char i, s = 0;
    signed short ans = 0;

    if ((n > 0) && (m < 0)) {
        s = 1;
        m = -m;
    }
    if ((n < 0) && (m > 0)) {
        s = 1;
        n = -n;
    }
    if ((n < 0) && (m < 0)) {
        n = -n;
        m = -m;
    }

    for(i=0; i < 8; i++) {
        if (m & 1) ans += n << i;
        m >>= 1;
    }

    if (s) ans = -ans;
    return ans;
}

int main() {

    printf("%d x %d = %d\n", 15, 11, signed_mult2(15,11));
    printf("%d x %d = %d\n", 15, -11, signed_mult2(15,-11));
    printf("%d x %d = %d\n", -15, 11, signed_mult2(-15,11));
    printf("%d x %d = %d\n", -15, -11, signed_mult2(-15,-11));
}

