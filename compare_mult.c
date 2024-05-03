#include <stdlib.h>
#include <stdio.h>
#include <string.h>

double getTime() {
    struct timeval tv; 

    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}

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

const char *pp(int x) {
    static char b[33];
    b[0] = '\0';

    unsigned int z;
    for (z = (1<<31); z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

signed short multb8(signed char m,
                    signed char r) {
    signed int A, S, P;
    unsigned char i;

    A = m << 17;
    S = (-m) << 17;
    P = (r & 0xff) << 1;

    for(i=0; i < 8; i++) {
        switch (P & 3) {
            case 1:    // 01
                P += A;
                break;
            case 2:    // 10
                P += S;
                break;
            default:   // 11 or 00
                break;
        }
        P >>= 1;
    }

    return P>>9;
}

int main() {
    double sss;
    int i;
    signed char j;
    signed short z;

    sss = getTime();
    for(j=-126; j<127; j++)
        for(i=0; i < 100000; i++)
            z = multb8(j,j);
    printf("multb8       = %f\n", getTime()-sss);

    sss = getTime();
    for(j=-126; j<127; j++)
        for(i=0; i < 100000; i++)
        z = signed_mult2(j,j);
    printf("signed_mult2 = %f\n", getTime()-sss);
}


