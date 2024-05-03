#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define N 10001

#if 1
#define FTYPE double
#define FMT "%0.16f"
#endif

#if 0
#define FTYPE float
#define FMT "%0.8f"
#endif

FTYPE A[N];

int main() {
    FTYPE sum, c, y, t;
    int i;
    FILE *f;

    srand(time(NULL));
    for(i=0; i<N; i++)
        A[i] = 1001.0*((FTYPE)rand()/(FTYPE)RAND_MAX);

    //  Stupid way
    sum = 0.0;
    for(i=0; i<N; i++)
        sum += A[i];
    printf("simple sum = " FMT "\n", sum);

    //  Kahan way
    sum = c = 0.0;
    for(i=0; i<N; i++) {
        y = A[i] - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }
    printf("Kahan sum  = " FMT "\n", sum);

    //  Store the array to get the high-precision sum
    f = fopen("PFP_summation.raw", "w");
    fwrite((void*)A, N*sizeof(FTYPE), 1, f);
    fclose(f);

    return 0;
}

// function KahanSum(input)
//     var sum = 0.0
//     var c = 0.0                  // A running compensation for lost low-order bits.
//     for i = 1 to input.length do
//         var y = input[i] - c         // So far, so good: c is zero.
//         var t = sum + y              // Alas, sum is big, y small, so low-order digits of y are lost.
//         c = (t - sum) - y        // (t - sum) cancels the high-order part of y; subtracting y recovers negative (low part of y)
//         sum = t                  // Algebraically, c should always be zero. Beware overly-aggressive optimizing compilers!
//     next i                       // Next time around, the lost low part will be added to y in a fresh attempt.
//     return sum

