#include <stdio.h>
#include <math.h>
#include <string.h>

const char *pp(unsigned int x) {
    static char b[33];
    unsigned int z;
    b[0] = '\0';
    for (z = (1<<31); z > 0; z >>= 1)
        strcat(b, ((x & z) == z) ? "1" : "0");
    return b;
}


//
//  Fun with NaNs
//

typedef union {
    float f;
    unsigned int d;
} fp_t;

char nan_isnan(float nan) {
    fp_t x;
    x.f = nan;
    return (((x.d >> 23) & 0xFF) == 0xFF) && ((x.d & 0x7FFFFF) != 0);
}

float nan_set_payload(float nan, unsigned int payload) {
    fp_t x;
    x.f = nan;
    x.d |= (payload & 0x3FFFFF);
    return x.f;
}

unsigned int nan_get_payload(float nan) {
    fp_t x;
    x.f = nan;
    return (x.d & 0x3FFFFF);
}

int main() {
    fp_t x;
    float n = log(-1);
    unsigned int d;

    printf("log(-1) is NaN = %d\n", nan_isnan(log(-1.0)));
    printf("log(1) is NaN  = %d\n", nan_isnan(log(1.0)));

    x.f = n;
    printf("%d  original    = %s\n", nan_isnan(n),pp(x.d));
    n = nan_set_payload(n, 12345);
    x.f = n;
    printf("%d  set payload = %s\n", nan_isnan(n),pp(x.d));
    d = nan_get_payload(n);
    printf("payload is %d (%s)\n", d, pp(d));

    n = 123 * n;
    d = nan_get_payload(n);
    printf("payload is still %d (%s)\n", d, pp(d));
}

