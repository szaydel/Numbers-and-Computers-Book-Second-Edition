#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

int main() {
    float a,b,t=1e-4;
    int p0,f0,p1,f1,i,m;
    uint32_t u = 0x00800000;

    srand(time(NULL));

    m = 10000;
    p0 = f0 = p1 = f1 = 0;

    for(i=0; i<m; i++) {
        a = (float)(1001.0*(double)rand()/(double)RAND_MAX);
        b = 2.1*a;
        b = b - 2.0*a;
        b = 10.0*b;
        if (fabsf(a-b) < t) p0++; else f0++;
        if (a == b) p1++; else f1++;
    }

    printf("p0=%5d, f0=%5d\n", p0, f0);
    printf("p1=%5d, f1=%5d\n", p1, f1);

    return 0;
}

