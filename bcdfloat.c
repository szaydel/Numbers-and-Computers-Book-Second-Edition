//
//  file:  bcdfloat.c
//
//  Conversion routines for BCD float 32 <--> IEEE double
//
//  RTK, 09-Sep-2016
//  Last update:  09-Sep-2016
//
///////////////////////////////////////////////////////////////

#include <stdio.h>
#include <inttypes.h>
#include <math.h>


double bcdfloat32_to_double(uint32_t bcd) {
    uint32_t sig = bcd >> 8;
    uint8_t i, sexp = bcd & 0xFF;
    double ans=0, exp;

    //  significand
    for(i=6; i > 0; i--) {
        ans += (sig & 0xF) * pow(10,-i);
        sig >>= 4;
    }

    //  exponent
    exp = (sexp & 0x40) ? (sexp & 0x3F) : -(sexp & 0x3F);
    ans *= pow(10,exp);

    //  sign
    return (sexp & 0x80) ? -ans : ans;
}


uint32_t double_to_bcdfloat32(double d) {
    char s[20];
    uint32_t ans=0, nsgn=1, sgn=0, sexp=0;
    int32_t exp;

    sprintf(s, "%+0.5e", d);
    
    if (s[0] == '-') sgn=1;
    if (s[9] == '-') nsgn=0;
    exp = 10*(s[10]-'0') + (s[11]-'0');
    exp = (s[9]=='-') ? -exp : exp;
    exp++;
    sexp = (abs(exp) & 0x3F) + (sgn*0x80) + (nsgn*0x40);

    ans += (s[1]-'0') << 7*4;
    ans += (s[3]-'0') << 6*4;
    ans += (s[4]-'0') << 5*4;
    ans += (s[5]-'0') << 4*4;
    ans += (s[6]-'0') << 3*4;
    ans += (s[7]-'0') << 2*4;
    ans += sexp;

    return ans;
}


int main() {
    uint32_t x,y;

    x = 0x12378943;
    printf("%x -> %0.6g\n", x, bcdfloat32_to_double(x));
    x = 0x123789C3;
    printf("%x -> %0.6g\n", x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(0.1);
    printf("%0.6g -> %x -> %0.6g\n", 0.1, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(0.0);
    printf("%0.6g -> %x -> %0.6g\n", 0.0, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(123.789);
    printf("%0.6g -> %x -> %0.6g\n", 123.789, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(-123.789);
    printf("%0.6g -> %x -> %0.6g\n", -123.789, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(-0.00123456);
    printf("%0.6g -> %x -> %0.6g\n", -0.00123456, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(1.0);
    printf("%0.6g -> %x -> %0.6g\n", 1.0, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(42.0);
    printf("%0.6g -> %x -> %0.6g\n", 42.0, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(-3.14159e-62);
    printf("%0.6g -> %x -> %0.6g\n", -3.14159e-62, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(-3.14159e+62);
    printf("%0.6g -> %x -> %0.6g\n", -3.14159e+62, x, bcdfloat32_to_double(x));

    x = double_to_bcdfloat32(-3.14159e+6);
    printf("%0.6g -> %x -> %0.6g\n", -3.14159e+6, x, bcdfloat32_to_double(x));

    return 0;
}

