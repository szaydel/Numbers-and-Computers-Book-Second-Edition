#include <string.h>
#include <stdio.h>

const char *pp(int x) {
    static char b[17];
    b[0] = '\0';

    unsigned int z;
    for (z = (1<<15); z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

unsigned char bcd2bin8(unsigned short b) {
    unsigned char n;

    n = (b & 0x0f);
    b >>= 4;
    n += 10*(b & 0x0f);
    b >>= 4;
    n += 100*(b & 0x0f);

    return n;
}

int main() {
    unsigned short b;

    b = 0x123;
    printf("%s -> %d\n", pp(b), bcd2bin8(b));
}

