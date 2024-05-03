#include <stdio.h>
#include <math.h>

int main() {
    double x0,x1,x2,x3;
    double r = 3.8;
    int i;

    x0 = x1 = x2 = x3 = 0.25;

    for(i=0; i < 100000; i++) {
        x0 = r*x0*(1.0 - x0);
        x1 = r*x1 - r*x1*x1;
        x2 = x2*(r - r*x2);
        x3 = r*x3 - r*pow(x3,2);
    }

    printf("     x0          x1          x2          x3\n");

    for(i=0; i < 8; i++) {
        x0 = r*x0*(1.0 - x0);
        x1 = r*x1 - r*x1*x1;
        x2 = x2*(r - r*x2);
        x3 = r*x3 - r*pow(x3,2);
        printf("%0.8f  %0.8f  %0.8f  %0.8f\n", x0,x1,x2,x3);
    }
}

