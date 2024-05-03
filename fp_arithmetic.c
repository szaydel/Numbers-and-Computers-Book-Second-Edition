#include <stdio.h>
#include <math.h>
#include <string.h>

const char *ppx(unsigned int x) {
    static char b[36];
    unsigned int z, i=0;
    b[0] = '\0';
    for (z = (1<<31); z > 0; z >>= 1) {
        strcat(b, ((x & z) == z) ? "1" : "0");
        if ((i == 0) || (i == 8))
            strcat(b, " ");
        i++;
    }
    return b;
}

typedef union {
    float f;
    unsigned int d;
} fp_t;

int main() {
    fp_t x,y,z;

    x.f = 1.34;
    y.f = 8.76;
    printf("%1.9f (%s)\n", x.f, ppx(x.d));
    printf("%1.9f (%s)\n", y.f, ppx(y.d));
    z.f = x.f + y.f;
    printf("%1.9f (%s)\n", z.f, ppx(z.d));

    printf("\n");

    x.f = 3.141592;
    y.f = 2.1;
    z.f = x.f * y.f;
    printf("%1.9f (%s)\n", x.f, ppx(x.d));
    printf("%1.9f (%s)\n", y.f, ppx(y.d));
    printf("%1.9f (%s)\n", z.f, ppx(z.d));
}

