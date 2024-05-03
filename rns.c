//
//  file:  rns.c
//
//  RNS number system
//
//  RTK, 28-Jun-2016
//  Last update:  02-Jul-2016
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

//  RNS type
//typedef utint64_t rns_t;
typedef uint32_t rns_t;

//  RNS base values - uint32
#if 1
#define B0 233
#define B1 239
#define B2 241
#define B3 251

#define A0 3021585941u
#define A1 1099363434u
#define A2 1663315003u
#define A3  952860257u
#endif

#define M0 0xFF
#define M1 0xFF00
#define M2 0xFF0000
#define M3 0xFF000000

#define S0 0
#define S1 8
#define S2 16
#define S3 24

//  Example bases and conversion parameters
#if 0
#define B0 2
#define B1 3
#define B2 5
#define B3 7
#define A0 105
#define A1 70
#define A2 126
#define A3 120
#endif

//  uint64
//#if 0
//#define B0 65479
//#define B1 65497
//#define B2 65519
//#define B3 65521
//
//#define A0 36466
//#define A1 22997
//#define A2 38753
//#define A3 32793
//
//#define M0  0xFFFF
//#define M1  0xFFFF0000
//#define M2  0xFFFF00000000
//#define M3  0xFFFF000000000000
//
//#define S0  0
//#define S1  16
//#define S2  32
//#define S3  48
//#endif


//  Maximum
uint32_t rns_maxint(void) {
    return (uint32_t)B0*(uint32_t)B1*(uint32_t)B2*(uint32_t)B3 - 1;
}


//  Pretty print
void pp(rns_t n, uint8_t mode) {
    uint16_t n0,n1,n2,n3;
    n0 = (n & M0) >> S0;
    n1 = (n & M1) >> S1;
    n2 = (n & M2) >> S2;
    n3 = (n & M3) >> S3;
    if (mode) {
        printf("%d %d %d %d", n0,n1,n2,n3);
    } else {
        printf("%02x %02x %02x %02x", n0,n1,n2,n3);
    }
}


//  Convert to RNS
rns_t to_rns(uint32_t n) {
    rns_t ans = 0;

    ans |= (n % B0) << S0;
    ans |= (n % B1) << S1;
    ans |= (n % B2) << S2;
    ans |= (n % B3) << S3;

    return ans;
}


//  Access and set individual base residues
void rns_set(rns_t *num, uint16_t val, uint8_t base) {
    switch (base) {
        case 0: *num |= (uint32_t)val << S0; break;
        case 1: *num |= (uint32_t)val << S1; break;
        case 2: *num |= (uint32_t)val << S2; break;
        case 3: *num |= (uint32_t)val << S3; break;
        default: break;
    }
}

uint16_t rns_get(rns_t num, uint8_t base) {
    uint16_t ans = 0;
    switch (base) {
        case 0: ans = (uint16_t)((num & M0) >> S0); break;
        case 1: ans = (uint16_t)((num & M1) >> S1); break;
        case 2: ans = (uint16_t)((num & M2) >> S2); break;
        case 3: ans = (uint16_t)((num & M3) >> S3); break;
        default: break;
    }
    return ans;
}

//  Addition
rns_t rns_add(rns_t x, rns_t y) {
    rns_t ans = 0;
    rns_set(&ans, (rns_get(x,0)+rns_get(y,0)) % B0, 0);
    rns_set(&ans, (rns_get(x,1)+rns_get(y,1)) % B1, 1);
    rns_set(&ans, (rns_get(x,2)+rns_get(y,2)) % B2, 2);
    rns_set(&ans, (rns_get(x,3)+rns_get(y,3)) % B3, 3);
    return ans;
}

//  Subtraction (unsigned)
rns_t rns_inverse(rns_t y) {
    rns_t ans = 0;
    rns_set(&ans, B0 - rns_get(y,0), 0);
    rns_set(&ans, B1 - rns_get(y,1), 1);
    rns_set(&ans, B2 - rns_get(y,2), 2);
    rns_set(&ans, B3 - rns_get(y,3), 3);
    return ans;
}

rns_t rns_sub(rns_t x, rns_t y) {
    rns_t ans = 0, iy = rns_inverse(y);
    rns_set(&ans, (rns_get(x,0)+rns_get(iy,0)) % B0, 0);
    rns_set(&ans, (rns_get(x,1)+rns_get(iy,1)) % B1, 1);
    rns_set(&ans, (rns_get(x,2)+rns_get(iy,2)) % B2, 2);
    rns_set(&ans, (rns_get(x,3)+rns_get(iy,3)) % B3, 3);
    return ans;
}

//  Multiplication
rns_t rns_mul(rns_t x, rns_t y) {
    rns_t ans = 0;
    rns_set(&ans, (rns_get(x,0)*rns_get(y,0)) % B0, 0);
    rns_set(&ans, (rns_get(x,1)*rns_get(y,1)) % B1, 1);
    rns_set(&ans, (rns_get(x,2)*rns_get(y,2)) % B2, 2);
    rns_set(&ans, (rns_get(x,3)*rns_get(y,3)) % B3, 3);
    return ans;
}

//  Convert from RNS
uint32_t to_int(rns_t n) {
    uint64_t ans;
    ans = (uint64_t)A0*rns_get(n,0) + 
          (uint64_t)A1*rns_get(n,1) + 
          (uint64_t)A2*rns_get(n,2) + 
          (uint64_t)A3*rns_get(n,3);
    return (uint32_t)(ans % (rns_maxint()+1));
}


int main(int argc, char *argv[]) {
    rns_t x,y;

    printf("1234567890 = "); pp(to_rns(1234567890),0); printf("\n");
    printf("        10 = "); pp(to_rns(10),0); printf("\n");
    printf("        13 = "); pp(to_rns(13),0); printf("\n\n");
    printf("       105 = "); pp(to_rns(105),0); printf("\n");
    printf("        70 = "); pp(to_rns(70),0); printf("\n");
    printf("       126 = "); pp(to_rns(126),0); printf("\n");
    printf("       120 = "); pp(to_rns(120),0); printf("\n\n");

    printf("3021585941 = "); pp(to_rns(3021585941u),1); printf("\n");
    printf("1099363434 = "); pp(to_rns(1099363434u),1); printf("\n");
    printf("1663315003 = "); pp(to_rns(1663315003u),1); printf("\n");
    printf(" 952860257 = "); pp(to_rns(952860257u),1); printf("\n\n");
    printf("max = %u\n\n", rns_maxint());

    x = rns_add(to_rns(4), to_rns(5));
    pp(to_rns(4),1); printf(" + "); pp(to_rns(5),1); printf(" = "); pp(x,1); printf("\n");

    x = rns_add(to_rns(104), to_rns(95));
    pp(to_rns(104),1); printf(" + "); pp(to_rns(95),1); printf(" = "); pp(x,1); printf("\n");

    printf("\n");

    x = rns_mul(to_rns(4), to_rns(5));
    pp(to_rns(4),1); printf(" * "); pp(to_rns(5),1); printf(" = "); pp(x,1); printf("\n");

    x = rns_mul(to_rns(14), to_rns(13));
    pp(to_rns(14),1); printf(" * "); pp(to_rns(13),1); printf(" = "); pp(x,1); printf("\n");

    printf("x is %u\n", to_int(x));
    printf("13 == %u\n", to_int(to_rns(13)));
    printf("1234567890 == %u\n", to_int(to_rns(1234567890u)));
    printf("3368562316 == %u\n", to_int(to_rns(3368562316u)));
    printf("0 == %u\n", to_int(to_rns(0)));

    printf("\n");

    pp(rns_inverse(to_rns(95)),1); printf("\n");
    x = rns_sub(to_rns(134), to_rns(95));
    pp(to_rns(134),1); printf(" - "); pp(to_rns(95),1); printf(" = "); pp(x,1); printf(" (%d)\n", to_int(x));

    printf("\n");

    pp(rns_inverse(to_rns(199)),1); printf("\n");
    x = rns_sub(to_rns(209), to_rns(199));
    pp(to_rns(209),1); printf(" - "); pp(to_rns(199),1); printf(" = "); pp(x,1); printf(" (%d)\n", to_int(x));

    return 0;
}


