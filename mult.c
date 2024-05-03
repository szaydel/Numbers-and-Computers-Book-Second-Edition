#include <stdlib.h>
#include <stdio.h>

double getTime() {
    struct timeval tv; 

    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}

unsigned short mult2(unsigned char n, unsigned char m) {
    unsigned char i;
    unsigned short ans = 0;

    for(i=0; i < 8; i++) {
        if (m & 1) {
            ans += n << i;
        }
        m >>= 1;
    }

    return ans;
}


unsigned short mult1(unsigned char n, unsigned char m) {
    unsigned char i;
    unsigned short ans = 0;
    
    if (n < m) {
        for(i=0; i < n; i++)
            ans += m;
    } else {
        for(i=0; i < m; i++)
            ans += n;
    }

    return ans;
}


unsigned int mult2s(unsigned short n, unsigned short m) {
    unsigned short i;
    unsigned int ans = 0;

    for(i=0; i < 16; i++) {
        if (m & 1) {
            ans += n << i;
        }
        m >>= 1;
    }

    return ans;
}

unsigned int mult1s(unsigned short n, unsigned short m) {
    unsigned short i;
    unsigned int ans = 0;
    
    if (n < m) {
        for(i=0; i < n; i++)
            ans += m;
    } else {
        for(i=0; i < m; i++)
            ans += n;
    }

    return ans;
}

int main() {
    unsigned char n=14, m=20;
    unsigned short a=65530;
    unsigned short b=65005;
    unsigned int z;
    unsigned short p;
    int i;
    double sss;
    
    sss = getTime();
    for(i=0; i < 10000000; i++)
        p = mult1(n, m);
    printf("mult1(%d,%d) = %d (%f)\n", n,m,p, getTime()-sss);

    sss = getTime();
    for(i=0; i < 10000000; i++)
        p = mult2(n, m);
    printf("mult2(%d,%d) = %d (%f)\n", n,m,p, getTime()-sss);
    printf("m = %d, n = %d\n\n", m, n);

    sss = getTime();
    for(i=0; i < 100000; i++)
        z = mult1s(a, b);
    printf("mult1s(%d,%d) = %d (%f)\n", a,b,z, getTime()-sss);

    sss = getTime();
    for(i=0; i < 100000; i++)
        z = mult2s(a, b);
    printf("mult2s(%d,%d) = %d (%f)\n", a,b,z, getTime()-sss);
}

