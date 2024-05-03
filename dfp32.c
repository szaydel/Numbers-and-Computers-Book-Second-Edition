#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned char h2d(unsigned char s) {
    if (s >= 'A') {
        return 10 + (s - 'A');
    } else {
        return (s - '0');
    }
}

unsigned char hex2dec(unsigned char *s) {
    return 16*h2d(s[0]) + h2d(s[1]);
}


int dfp_parse_exp(int cf, int bxcf, int *d) {
    static int digits[] = {0,1,2,3,4,5,6,7,
                           0,1,2,3,4,5,6,7,
                           0,1,2,3,4,5,6,7,
                           8,9,8,9,8,9};
    static int bits[] = {0,0,0,0,0,0,0,0,
                         1,1,1,1,1,1,1,1,
                         2,2,2,2,2,2,2,2,
                         0,0,1,1,2,2};

    *d = digits[cf];
    return ((bits[cf] << 6) + bxcf) - 101;
}

int dfp_parse_bits(int a, int b, int c, int d) {
    return 8*a + 4*b + 2*c + d;
}

void dfp_parse_declet(int dec, int *d0, int *d1, int *d2) {
    int p,q,r,s,t,u,v,w,x,y;

    y = (dec & 0x1) != 0;
    x = (dec & 0x2) != 0;
    w = (dec & 0x4) != 0;
    v = (dec & 0x8) != 0;
    u = (dec & 0x10) != 0;
    t = (dec & 0x20) != 0;
    s = (dec & 0x40) != 0;
    r = (dec & 0x80) != 0;
    q = (dec & 0x100) != 0;
    p = (dec & 0x200) != 0;

    if (v==0) {
        *d0 = dfp_parse_bits(0,p,q,r);
        *d1 = dfp_parse_bits(0,s,t,u);
        *d2 = dfp_parse_bits(0,w,x,y);
    }
    if ((v==1) && (x==0) && (w==0)) {
        *d0 = dfp_parse_bits(0,p,q,r);
        *d1 = dfp_parse_bits(0,s,t,u);
        *d2 = dfp_parse_bits(1,0,0,y);
    }
    if ((v==1) && (x==0) && (w==1)) {
        *d0 = dfp_parse_bits(0,p,q,r);
        *d1 = dfp_parse_bits(1,0,0,u);
        *d2 = dfp_parse_bits(0,s,t,y);
    }
    if ((v==1) && (x==1) && (w==0)) {
        *d0 = dfp_parse_bits(1,0,0,r);
        *d1 = dfp_parse_bits(0,s,t,u);
        *d2 = dfp_parse_bits(0,p,q,y);
    }
    if ((v==1) && (x==1) && (w==1) && (s==0) && (t==0)) {
        *d0 = dfp_parse_bits(1,0,0,r);
        *d1 = dfp_parse_bits(0,p,q,u);
        *d2 = dfp_parse_bits(1,0,0,y);
    }
    if ((v==1) && (x==1) && (w==1) && (s==1) && (t==0)) {
        *d0 = dfp_parse_bits(0,p,q,r);
        *d1 = dfp_parse_bits(1,0,0,u);
        *d2 = dfp_parse_bits(1,0,0,y);
    }
    if ((v==1) && (x==1) && (w==1) && (s==1) && (t==1)) {
        *d0 = dfp_parse_bits(1,0,0,r);
        *d1 = dfp_parse_bits(1,0,0,u);
        *d2 = dfp_parse_bits(1,0,0,y);
    }

    return;
}

void dfp_parse32(unsigned char *b, unsigned char *s) {
    int sign, cf, bxcf, d0,d1,d2, dec, exponent, i=0;
    
    s[0] = '-';
    sign = (b[0] & 0x80) != 0;
    cf = (b[0] >> 2) & 0x1f;
    bxcf = ((b[0] & 0x3) << 4) + ((b[1] >> 4) & 0xf);

    if (cf == 0x1e) {
        strcpy(&s[sign],"inf");
        return;
    }
    if (cf == 0x1f) {
        strcpy(s,"NaN");
        return;
    }

    exponent = dfp_parse_exp(cf, bxcf, &d0);
    
    i += sign;
    s[i++] = d0 + '0';

    dec = ((b[1] & 0xf) << 6) + ((b[2] & 0xfc) >> 2);
    dfp_parse_declet(dec, &d0, &d1, &d2);
    s[i++] = d0 + '0';
    s[i++] = d1 + '0';
    s[i++] = d2 + '0';
    dec = ((b[2] & 0x3) << 8) + b[3];
    dfp_parse_declet(dec, &d0, &d1, &d2);
    s[i++] = d0 + '0';
    s[i++] = d1 + '0';
    s[i++] = d2 + '0';

    s[i++] = 'E';
    sprintf(&s[i], "%d", exponent);
}

int main(int argc, char *argv[]) {
    unsigned char s[100];
    unsigned char b[100];
    int i;

    //  Parse command line bytes
    for(i=1; i < 5; i++) {
        b[i-1] = hex2dec(argv[i]);
    }

    //  Generate ASCII output
    s[0] = '\0';
    dfp_parse32(b,s);
    printf(" --> %s\n", s);
}

