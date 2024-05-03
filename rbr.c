//
//  file:  rbr.c
//
//  Simple RBR routines
//
//  RTK, 03-Jul-2016
//  Last update:  05-Jul-2016
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>

typedef uint64_t rbr_t;

//  Conversion routines
rbr_t to_rbr(int32_t n) {
    rbr_t ans = 0;
    uint8_t i, s=0;

    if (n < 0) {
        s = 1;
        n = abs(n);
    }

    for(i=0; i < 32; i++) {
        ans += ((n & 1) ? 3 : 1) * (1llu << (2*i));
        n >>= 1;
    }

    return (s) ? ~ans : ans;
}


int8_t rbr_digit(uint8_t b) {
    switch (b) {
        case 0: return -1;
        case 1: return  0;
        case 2: return  0;
        case 3: return  1;
    }
}

int32_t to_int(rbr_t n) {
    int64_t ans = 0;
    uint8_t i;

    for(i=0; i < 32; i++) {
        ans += rbr_digit(n & 3) * (1 << i);
        n >>= 2;
    }

    return (int32_t)ans;
}


//  Addition
int8_t rbr_enc(int8_t s) {
    switch (s) {
        case -1: return 0;
        case  0: return 1;
        case  1: return 3;
    }
} 

int8_t rbr_add_partial(rbr_t s, rbr_t t) {
    int8_t z = rbr_digit(s & 3) + rbr_digit(t & 3);
    return rbr_enc(z);
}

int8_t rbr_add_st(rbr_t x, rbr_t y, int8_t *s, int8_t *t) {
    int8_t dx = rbr_digit(x & 3);
    int8_t dy = rbr_digit(y & 3);
    if ((dx == -1) && (dy == -1)) { *t = -1; *s =  0; }
    if ((dx == -1) && (dy ==  0)) { *t = -1; *s =  1; }
    if ((dx ==  0) && (dy == -1)) { *t =  0; *s = -1; }
    if ((dx == -1) && (dy ==  1)) { *t =  0; *s =  0; }
    if ((dx ==  1) && (dy == -1)) { *t =  0; *s =  0; }
    if ((dx ==  0) && (dy ==  0)) { *t =  0; *s =  0; }
    if ((dx ==  0) && (dy ==  1)) { *t =  0; *s =  1; }
    if ((dx ==  1) && (dy ==  0)) { *t =  1; *s = -1; }
    if ((dx ==  1) && (dy ==  1)) { *t =  1; *s =  0; }
}

rbr_t rbr_add(rbr_t x, rbr_t y) {
    rbr_t ans=0, s=0, t=0;
    int8_t ds,dt;
    uint8_t i;

    // Go bit by bit setting w and t
    for(i=0; i < 32; i++) {
        rbr_add_st(x, y, &ds, &dt);
        s += ds * (1llu << (i*2));
        t += dt * (1llu << (i*2));
        x >>= 2;
        y >>= 2;
    }

    //  Shift t up
    t <<= 2;

    //  Answer is add s and t
    for(i=0; i < 32; i++) {
        ans += rbr_add_partial(s,t) * (1llu << (2*i));
        s >>= 2;
        t >>= 2;
    }

    return ans;
}


//  Subtraction
rbr_t rbr_sub(rbr_t x, rbr_t y) {
    return rbr_add(x, ~y);
}


//  Multiplication
rbr_t rbr_mul(rbr_t x, rbr_t y) {
    return (rbr_t)0;
}


int main(int argc, char *argv[]) {
    rbr_t x,y,z;

    x = to_rbr(1011219);
    printf("%u => x = %llx => %u\n", 1011219, (uint64_t)x, to_int(x));

    x = to_rbr(-1011219);
    printf("%d => x = %llx => %d\n", -1011219, (uint64_t)x, to_int(x));

    x = to_rbr(123);
    printf("%u => x = %llx => %u\n", 123, (uint64_t)x, to_int(x));

    x = to_rbr(-123);
    printf("%d => x = %llx => %d\n", -123, (uint64_t)x, to_int(x));

    x = to_rbr(123); y = to_rbr(35);
    z = rbr_add(x,y);
    printf("%d + %d = %d\n", to_int(x), to_int(y), to_int(z));

    x = to_rbr(-123); y = to_rbr(35);
    z = rbr_add(x,y);
    printf("%d + %d = %d\n", to_int(x), to_int(y), to_int(z));

    x = to_rbr(12345678); y = to_rbr(-12345678);
    z = rbr_add(x,y);
    printf("%d + %d = %d\n", to_int(x), to_int(y), to_int(z));

    x = to_rbr(12345678); y = to_rbr(12345678);
    z = rbr_add(x,y);
    printf("%d + %d = %d\n", to_int(x), to_int(y), to_int(z));

    x = to_rbr(12345678); y = to_rbr(12345678);
    z = rbr_sub(x,y);
    printf("%d - %d = %d\n", to_int(x), to_int(y), to_int(z));

    x = to_rbr(123); y = to_rbr(35);
    z = rbr_sub(x,y);
    printf("%d - %d = %d\n", to_int(x), to_int(y), to_int(z));

    x = to_rbr(-123); y = to_rbr(35);
    z = rbr_sub(x,y);
    printf("%d - %d = %d\n", to_int(x), to_int(y), to_int(z));

    x = to_rbr(12345678); y = to_rbr(-12345678);
    z = rbr_sub(x,y);
    printf("%d - %d = %d\n", to_int(x), to_int(y), to_int(z));

    y = to_rbr(123); x = to_rbr(35);
    z = rbr_sub(x,y);
    printf("%d - %d = %d\n", to_int(x), to_int(y), to_int(z));

    return 0;
}

