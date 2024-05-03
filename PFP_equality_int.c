//
//  file:  PFP_equality_int.c
//
//  Use the integer representation to define equality with a tolerance
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>

#if 0
#define FTYPE double
#define ITYPE uint64_t
#endif
#if 1
#define FTYPE float
#define ITYPE uint32_t
#endif

uint8_t equal_ulps(FTYPE a, FTYPE b, ITYPE ulps) {
    ITYPE A = *(ITYPE*)&a;
    ITYPE B = *(ITYPE*)&b;

    if ((a == 0.0) && (b == 0.0)) return 1;
    if (isnan(a) && isnan(b)) return 1;
    return abs(A-B) < ulps;
}


int main() {
    FTYPE a,b;
    ITYPE A,B,i;
    uint32_t S=0, UMAX=0, UMIN=80000000;
    uint32_t p0=0,f0=0,p1=0,f1=0, M=50000000;

    srand(time(NULL));

    for(i=0; i < M; i++) {
        a = (FTYPE)(1001.0*(double)rand()/(double)RAND_MAX);
        b = 2.1*a;
        b = b - 2.0*a;
        b = 10.0*b;
        A = *(ITYPE*)&a;
        B = *(ITYPE*)&b;
        S += abs(A-B);
        if (abs(A-B) > UMAX) UMAX = abs(A-B);
        if (abs(A-B) < UMIN) UMIN = abs(A-B);
        if (equal_ulps(a,b,30)) p0++; else f0++;
        if (a==b) p1++; else f1++;
    }

    printf("p0=%8d, f0=%8d, passed=%0.8f\n", p0, f0, (double)p0/(double)M);
    printf("p1=%8d, f1=%8d, passed=%0.8f\n", p1, f1, (double)p1/(double)M);
    printf("mean difference in ulps = %0.8f, [%u, %u]\n", (double)S / (double)M, UMIN, UMAX);

    return 0;
}

