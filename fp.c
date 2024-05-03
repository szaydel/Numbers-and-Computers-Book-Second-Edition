#include <stdio.h>
#include <stdint.h>
#include <math.h>

double ibm_to_ieee_64(uint32_t n) {
    int s, e, m;
    double ans;

    s = (n & (1<<31)) != 0;
    e = (n >> 24) & 0x7F;
    m = n & 0xFFFFFF;

    ans = m * pow(16,-6) * pow(16,e-64);
    return (s == 1) ? -ans : ans;
}


int main() {
    uint32_t ibm1, ibm2;
    double ieee1, ieee2;

    printf("min IBM = %g\n", ibm_to_ieee_64(0x00100000));
    printf("max IBM = %g\n", ibm_to_ieee_64(0x7FFFFFFF));
}


