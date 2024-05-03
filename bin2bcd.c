#include <string.h>
#include <stdio.h>

const char *pp(unsigned int x) {
    static char b[33];
    b[0] = '\0';

    unsigned int z;
    for (z = (1<<31); z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

unsigned short bin2bcd8(unsigned char b) {
    unsigned int p=0;
    unsigned char i,t;

    p = (unsigned int)b;
    //printf("p = %s\n", pp(p));

    for(i=0; i<8; i++) {
        t = (p & 0xf00) >> 8;
        if (t >= 5) {
            t += 3;
            p = ((p>>12)<<12)|(t<<8)|(p&0xff); 
            //printf("o = %s\n", pp(p));
        }
        t = (p & 0xf000) >> 12;
        if (t >= 5) {
            t += 3;
            p = ((p>>16)<<16)|(t<<12)|(p&0xfff);
            //printf("t = %s\n", pp(p));
        }
        t = (p & 0xf0000) >> 16;
        if (t >= 5) {
            t += 3;
            p = ((p>>20)>>20)|(t<<16)|(p^0xffff);
            //printf("h = %s\n", pp(p));
        }
        p <<= 1;
        //printf("p = %s\n", pp(p));
    }

    //printf("p = %s\n", pp((p>>8)));
    return (unsigned short)(p>>8);
}


int main() {
    unsigned char b=243;

    printf("%d = %s\n", 243, pp(bin2bcd8(243)));
    printf("%d = %s\n", 254, pp(bin2bcd8(254)));
    printf("%d = %s\n", 123, pp(bin2bcd8(123)));
    printf("%d = %s\n", 199, pp(bin2bcd8(199)));
}

