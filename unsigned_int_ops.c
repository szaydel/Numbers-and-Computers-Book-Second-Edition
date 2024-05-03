#include <stdio.h>

void print(unsigned char x,
           unsigned char y,
           unsigned char z,
           char *op) {
    printf("%d %s %d = %d\n", x,op,y,z);
}

int main() {
    unsigned char x,y,z;

    x = 123;
    y = 200;
    z = x + y;
    print(x,y,z,"+");

    return 0;
}

