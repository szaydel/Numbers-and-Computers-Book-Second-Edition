#include <stdio.h>
#include <math.h>
#include <float.h>
#include <inttypes.h>

int main() {
    float a,b,c,d;
    char sa[] = "0.1e+00";
    char sb[] = "0.2e+00";
    char sc[] = "0.3e+00";
    int i;

    for(i=-36; i < 37; i++) {
        sprintf(&sa[4],"%+02d", i);
        sprintf(&sb[4],"%+02d", i);
        sprintf(&sc[4],"%+02d", i);
        sscanf(sa,"%f",&a);
        sscanf(sb,"%f",&b);
        sscanf(sc,"%f",&c);

        if ((a+b) == c) {
            printf("%d  1\n", i);
        } else {
            printf("%d  0\n", i);
        }
    }

    return 0;
}

