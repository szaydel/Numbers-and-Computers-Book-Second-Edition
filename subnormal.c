#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

double getTime() {
    struct timeval tv; 

    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1000000;
}

const char *pp(unsigned int x) {
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
    fp_t x;
    float n;
    int i;
    double ss, ee;

    x.f = 1.0;
    for(i=0; i < 151; i++) {
        printf("x = (%s) %0.8g\n", pp(x.d), x.f);
        x.f /= 2.0;
    }

    //x.f = pow(2,-126);
    //printf("%s\n", pp(x.d));
    //ss = getTime();
    //for(i=1; i < 10000001; i++)
    //    n = x.f * ((float)i/(float)i);
    //ee = getTime();
    //printf("10,000,000 iterations = %0.10f s\n", ee-ss);

    //printf("\n");

    //x.f /= 2;
    //printf("%s\n", pp(x.d));
    //ss = getTime();
    //for(i=1; i < 10000001; i++)
    //    n = x.f * ((float)i/(float)i);
    //ee = getTime();
    //printf("10,000,000 iterations = %0.10f s\n", ee-ss);
}

