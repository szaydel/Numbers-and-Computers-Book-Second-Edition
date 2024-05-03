//
//  file:  PFP_loop.c
//
//  Unpredictability of floating-point comparison in a loop
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
    double start, end, inc, d, s=0.0;
    int i=0,j=0,n,m;

    srand(time(NULL));

    n = 100;
    m = 10000000;

    for(j=0; j<m; j++) {
        start = 100001.0*((double)rand()/(double)RAND_MAX);
        end = start + 1001.0*((double)rand()/(double)RAND_MAX);
        inc = (end - start) / n;
        i = 0;

        //  Wrong way
        for(d=start; d < end; d += inc)
            i++;
        s += i-n;

        //  Right way
        //for(j=0; j < n; j++) {
        //    d = start + j*inc;
        //    ...
        //}
    }

    printf("fraction with extra loop = %0.9f\n", s/(double)m);

    //d = start;
    //for(j=0; j<n; j++)
    //    d = i*inc;
    //printf("d = %0.16f\n", d);

    return 0;
}

