#include <stdio.h>

int main() {
    unsigned char *p;
    unsigned short n = 256;

    p = (unsigned char *)&n;
    printf("address of first byte  = %p\n", &p[0]);
    printf("address of second byte = %p\n", &p[1]);

    return 0;
}

