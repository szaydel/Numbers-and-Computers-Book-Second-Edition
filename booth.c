#include <stdio.h>
#include <string.h>

const char *pp(int x) {
    static char b[33];
    b[0] = '\0';

    unsigned int z;
    for (z = (1<<31); z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}

signed short multb8(signed char m,
                    signed char r) {
    signed int A, S, P;
    unsigned char i;

    //printf("Initial:\n");
    //A = m << 17;  //printf("A = %s\n", pp(A));
    //S = (-m) << 17;  //printf("S = %s\n", pp(S));
    //P = (r & 0xff) << 1;  //printf("P = %s\n", pp(P));
    //printf("\n");
    A = m << 9;  //printf("A = %s\n", pp(A));
    S = (-m) << 9;  //printf("S = %s\n", pp(S));
    P = (r & 0xff) << 1;  //printf("P = %s\n", pp(P));

    for(i=0; i < 8; i++) {
        switch (P & 3) {
            case 1:    // 01
                P += A;
                break;
            case 2:    // 10
                P += S;
                break;
            default:   // 11 or 00
                break;
        }
        //printf("P = %s\n", pp(P));
        P >>= 1;
        //printf("    new P = %s\n", pp(P));
    }

    return P>>1;
}

signed short multb4(signed char m,
                    signed char r) {
    signed short A, S, P;
    unsigned char i;

    //printf("Initial:\n");
    A = m << 5;  //printf("A = %s\n", pp(A));
    S = (-m) << 5;  //printf("S = %s\n", pp(S));
    P = (r & 0x0f) << 1;  //printf("P = %s\n", pp(P));
    //printf("\n");

    for(i=0; i < 4; i++) {
        switch (P & 3) {
            case 1:    // 01
                P += A;
                break;
            case 2:    // 10
                P += S;
                break;
            default:   // 11 or 00
                break;
        }
        //printf("P = %s\n", pp(P));
        P >>= 1;
        //printf("    new P = %s\n", pp(P));
    }

    return P>>1;
}

int main() {

    //printf("%d x %d = %d\n", 3,-4, multb4(3,-4));
    //printf("%d x %d = %d\n",-3, 4, multb4(-3,4));
    //printf("%d x %d = %d\n", 2, 4, multb4(2,4));
    //printf("%d x %d = %d\n",-2,-4, multb4(-2,-4));
    //printf("%d x %d = %d\n", 0, 1, multb4(0,1));
    //printf("%d x %d = %d\n", -1, 0, multb4(-1,0));
    //printf("%d x %d = %d\n", -1, 1, multb4(-1,1));
    //printf("%d x %d = %d\n", 1, -1, multb4(1,-1));
    //printf("%d x %d = %d\n",-1, -1, multb4(-1,-1));

    //printf("\n");

    printf("%d x %d = %d\n", 3,-4, multb8(3,-4));
    printf("%d x %d = %d\n",-3, 4, multb8(-3,4));
    printf("%d x %d = %d\n", 2, 4, multb8(2,4));
    printf("%d x %d = %d\n",-2,-4, multb8(-2,-4));
    printf("%d x %d = %d\n", 0, 1, multb8(0,1));
    printf("%d x %d = %d\n", -1, 0, multb8(-1,0));
    printf("%d x %d = %d\n", -1, 1, multb8(-1,1));
    printf("%d x %d = %d\n", 1, -1, multb8(1,-1));
    printf("%d x %d = %d\n",-1, -1, multb8(-1,-1));
    printf("%d x %d = %d\n",-15,15, multb8(-15,15));
    printf("%d x %d = %d\n",-25,25, multb8(-25,25));
    printf("%d x %d = %d\n",-126,126, multb8(-126,126));
    printf("%d x %d = %d\n",126,126, multb8(126,126));
    printf("%d x %d = %d\n",126,-126, multb8(126,-126));
    printf("%d x %d = %d\n",-126,-126, multb8(-126,-126));
}


