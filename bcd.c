//
//  file:  bcd.c
//
//  Misc BCD routines
//
//  RTK, 07-Aug-2016
//  Last update:  01-Sep-2016
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <inttypes.h>

//  Positive and negative nibbles, EBCDIC
#if 1
#define PNIB 0xF0
#define NNIB 0xD0
#endif

//  Positive and negative nibbles, ASCII
#if 0
#define PNIB 0x30
#define NNIB 0x70
#endif


//  Convert zoned number to binary
//int zoned2bin(uint64_t z) {
//    int32_t b=0;
//    int8_t i,s=1;
//
//    for (i=0; i<8; i++) {
//        b += (z & 0xF) * (int32_t)pow(10,i);
//        if ((z & 0xF0) == NNIB) s=-1;
//        z >>= 8;
//    }
//    return s*b;
//}

int zoned2bin(uint64_t z) {
    int32_t b=0,p=1;
    int8_t i,s=1;

    for (i=0; i<8; i++) {
        b += (z & 0xF) * p;
        p *= 10;
        if ((z & 0xF0) == NNIB) s=-1;
        z >>= 8;
    }
    return s*b;
}

//  Convert binary to zoned
//uint64_t bin2zoned(int b) {
//    uint8_t i,s = (b<0) ? 1:0;
//    uint64_t d,z=0;
//    
//    b = abs(b);
//    for(i=7; i>0; i--) {
//        d = b / (uint64_t)pow(10,i);
//        b -= d * (uint64_t)pow(10,i);
//        z += (PNIB+d) << (uint64_t)(i*8);
//    }
//    z += (s) ? (NNIB+b) : (PNIB+b);
//    return z;
//}

uint64_t bin2zoned(int b) {
    uint8_t i,s = (b<0) ? 1:0;
    uint64_t d,z=0,p=10000000;
    
    b = abs(b);
    for(i=7; i>0; i--) {
        d = b / p;
        b -= d * p;
        p /= 10;
        z += (PNIB+d) << (uint64_t)(i*8);
    }
    z += (s) ? (NNIB+b) : (PNIB+b);
    return z;
}

int main() {
    uint64_t z;
    int b;

    b = 1234;
    z = bin2zoned(b);
    printf("%9d -> %llx\n", b, z);

    b = 9;
    z = bin2zoned(b);
    printf("%9d -> %llx\n", b, z);

    b = 87654321;
    z = bin2zoned(b);
    printf("%9d -> %llx\n", b, z);

    b = -1234;
    z = bin2zoned(b);
    printf("%9d -> %llx\n", b, z);

    b = -9;
    z = bin2zoned(b);
    printf("%9d -> %llx\n", b, z);

    b = -87654321;
    z = bin2zoned(b);
    printf("%9d -> %llx\n", b, z);

    printf("\n\n");

    b = 87654321;
    z = bin2zoned(b);
    printf("%d -> %llx -> %d\n", b, z, zoned2bin(z));

    b = -87654321;
    z = bin2zoned(b);
    printf("%d -> %llx -> %d\n", b, z, zoned2bin(z));

    b = 8;
    z = bin2zoned(b);
    printf("%d -> %llx -> %d\n", b, z, zoned2bin(z));

    b = -8;
    z = bin2zoned(b);
    printf("%d -> %llx -> %d\n", b, z, zoned2bin(z));

    b = 8675309;
    z = bin2zoned(b);
    printf("%d -> %llx -> %d\n", b, z, zoned2bin(z));

    b = -8675309;
    z = bin2zoned(b);
    printf("%d -> %llx -> %d\n", b, z, zoned2bin(z));

    return 0;
}

