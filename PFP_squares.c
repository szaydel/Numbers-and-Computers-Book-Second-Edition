#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define N 100
#define FTYPE double
#define FMT "%0.16f"

FTYPE A[N], B[N];

FTYPE rnd(FTYPE n) {
    return n*((FTYPE)rand()/(FTYPE)RAND_MAX);
}

int main() {
    FTYPE x,y,a,b,d0,d1;
    int i;
    FILE *f;

    srand(time(NULL));

    for(i=0; i<N; i++) {
        a = rnd(100001.0);
        b = rnd(100001.0);
        A[i] = a;
        B[i] = b;
        x = sqrt(a);
        y = sqrt(b);
        d0 = a - b;
        d1 = (x+y)*(x-y);
        printf(FMT " " FMT " " FMT " " FMT "\n", a, b, d0, d1);
    }

    f = fopen("PFP_squares_a.raw", "w");
    fwrite((void*)A, N*sizeof(FTYPE), 1, f);
    fclose(f);
    f = fopen("PFP_squares_b.raw", "w");
    fwrite((void*)B, N*sizeof(FTYPE), 1, f);
    fclose(f);

    return 0;
}

