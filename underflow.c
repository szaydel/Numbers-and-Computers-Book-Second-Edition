#include <stdio.h>

int main() {
    unsigned char x, y, z;
    
    x = 0xEE;
    y = 0x35;

    z = x - y;
    printf("%X\n", z);

    z = y - x;
    printf("%X\n", z);

    z = 0 - 1;
    printf("%X\n", z);
}
 
