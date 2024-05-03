//
//  Integer-only cube root
//

#include <stdio.h>

unsigned int cbroot(unsigned int n) {
    unsigned int c=0, p=1;

    while (n >= p) {
        n -= p;
        c++;
        p = 3*c*c+3*c+1;
    }

    return c;
}


int main() {
    printf("cube root of %10d = %4d\n", 1860867, cbroot(1860867));
    printf("cube root of %10d = %4d\n", 1, cbroot(1));
    printf("cube root of %10d = %4d\n", 29, cbroot(29));
    printf("cube root of %10d = %4d\n", 46268279, cbroot(46268279));
    printf("cube root of %10d = %4d\n", 2141700569, cbroot(2141700569));

    return 0;
}

