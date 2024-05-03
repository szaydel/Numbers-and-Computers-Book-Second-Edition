#include <stdio.h>

int main() {
    unsigned short x, y, z;
    
    x = 0xEE;
    y = 0x35;
    z = x + y;

    printf("%x\n", z);
}
 
