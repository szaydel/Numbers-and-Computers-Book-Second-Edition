//
//  file:  PFP_equality.c
//
//  Functions testing for FP equality with varying strictness.
//
//  RTK, 14-Sep-2016
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#if 0
#define FTYPE double
#define ITYPE uint64_t
#define FMT "%0.18f"
#endif
#if 1
#define FTYPE float
#define ITYPE uint32_t
#define FMT "%0.10f"
#endif

//  Strictest test for equality
uint8_t equal_strict(FTYPE a, FTYPE b) {
    ITYPE A = *(ITYPE *)&a;
    ITYPE B = *(ITYPE *)&b;
    
    if ((a == 0.0) && (b == 0.0)) return 1;
    if (isnan(a) && isnan(b)) return 1;
    return (A == B);
}


int main() {
    FTYPE a,b;

    printf(FMT " == " FMT " -> %d\n", 0.1, 0.1, equal_strict(0.1, 0.1));
    printf(FMT " == " FMT " -> %d\n", 3.141592, 3.14159, equal_strict(3.14159, 3.14159));
    printf(FMT " == " FMT " -> %d\n", 1.2345e19, 1.2345e19, equal_strict(1.2345e19,1.2345e19));
    printf(FMT " == " FMT " -> %d\n", 0.0, 0.0, equal_strict(0.0,0.0));
    printf(FMT " == " FMT " -> %d\n", 0.0, -0.0, equal_strict(0.0,-0.0));
    printf(FMT " == " FMT " -> %d\n", -0.0, 0.0, equal_strict(-0.0,0.0));
    printf(FMT " == " FMT " -> %d\n", -0.0, -0.0, equal_strict(-0.0,-0.0));
    printf(FMT " == " FMT " -> %d\n", 1.0/0.0, 1.0/0.0, equal_strict(1.0/0.0,1.0/0.0));
    printf(FMT " == " FMT " -> %d\n", -1.0/0.0, 1.0/0.0, equal_strict(-1.0/0.0,1.0/0.0));
    printf(FMT " == " FMT " -> %d\n", 1.0/0.0, -1.0/0.0, equal_strict(1.0/0.0,-1.0/0.0));
    printf(FMT " == " FMT " -> %d\n", -1.0/0.0, -1.0/0.0, equal_strict(-1.0/0.0,-1.0/0.0));
    printf(FMT " == " FMT " -> %d\n", log(-1.0), log(-1.0), equal_strict(log(-1.0),log(-1.0)));
    printf(FMT " == " FMT " -> %d\n", sqrt(-1.0), sqrt(-1.0), equal_strict(sqrt(-1.0),sqrt(-1.0)));

    return 0;
}

