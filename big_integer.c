#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// input
signed int *bigint_input(char *s) {
    int i, j=0, n, sign=0;
    signed int *b;

    if (s[0] == '-') {
        sign = 1;
        j++;
    }
    
    n = (int)ceil((strlen(s)-sign) / 4.0);
    b = (signed int *)malloc((n+1)*sizeof(signed int));
    b[0] = (sign) ? -n : n;

    for(i=1; i <= n; i++) {
        if ((i==1) && (((strlen(s)-sign)%4) != 0)) {
            b[i] = 0;
            switch ((strlen(s)-sign)%4) {
                case 3:
                    b[i] += (s[j++]-'0') * 100;
                case 2:
                    b[i] += (s[j++]-'0') * 10;
                case 1:
                    b[i] += (s[j++]-'0');
            }
        } else {
            b[i]  = (s[j++]-'0') * 1000;
            b[i] += (s[j++]-'0') * 100;
            b[i] += (s[j++]-'0') * 10;
            b[i] += (s[j++]-'0');
        }
    }

    return b;
}

// output
void bigint_output(signed int *b, char*s) {
    int i, j=0;
    char t[5];

    if (b[0] < 0)
        s[j++] = '-';

    for(i=1; i <= abs(b[0]); i++) {
        sprintf(t, "%04d", b[i]);
        if (!((t[0] == '0') && (i == 1))) s[j++] = t[0];
        if (!((t[1] == '0') && (i == 1))) s[j++] = t[1];
        if (!((t[2] == '0') && (i == 1))) s[j++] = t[2];
        s[j++] = t[3];
    }

    s[j] = '\0';
}
 
//  negate
signed int *bigint_negate(signed int *a) {
    a[0] = -a[0];
    return a;
}


//  unsigned compare
int bigint_ucompare(signed int *a, signed int *b) {
    signed int i;

    if (abs(a[0]) < abs(b[0])) return -1;
    if (abs(a[0]) > abs(b[0])) return 1;

    for(i=1; i <= abs(a[0]); i++) {
        if (a[i] > b[i]) return 1;
        if (b[i] > a[i]) return -1;
    }

    return 0;
}


//  low-level add
signed int *bigint_uadd(signed int *a, signed int *b) {
    signed int *c, *x, *y, *t;
    int i, m, n, yy, carry=0;

    if (abs(a[0]) > abs(b[0])) {
        n = abs(a[0]);  m = abs(b[0]);
        x = a;          y = b;
    } else {
        n = abs(b[0]);  m = abs(a[0]);
        x = b;          y = a;
    }

    c = (signed int *)malloc((n+1)*sizeof(signed int));
    c[0] = n;

    for(i=n; i>0; i--) {
        yy = (i>(n-m)) ? y[i-(n-m)] : 0;
        c[i] = x[i] + yy + carry;
        if (c[i] > 9999) {
            carry = 1;
            c[i] -= 10000;
        } else
            carry = 0;
    }

    if (carry) {
        t = (signed int *)malloc((n+2)*sizeof(signed int));
        for(i=1; i<=n; i++)  t[i+1] = c[i];
        t[0] = n+1;
        t[1] = 1;
        free(c);
        c = t;
    }
    return c;
}


//  low-level sub
signed int *bigint_usub(signed int *a, signed int *b) {
    signed int *c, *t;
    int i, m, n, y, borrow=0, zero=0;

    n = abs(a[0]);
    m = abs(b[0]);
    c = (signed int *)malloc((n+1)*sizeof(signed int));
    c[0] = n;

    for(i=n; i>0; i--) {
        y = ((i-(n-m)) > 0) ? b[i-(n-m)] : 0;
        c[i] = a[i] - y - borrow;
        if (c[i] < 0) {
            borrow = 1;
            c[i] += 10000;
        } else
            borrow = 0;
        zero += c[i];
    }

    //  leading zero?
    if ((c[1] == 0) && (zero != 0)) {
        t = (signed int *)malloc(n*sizeof(signed int));
        for(i=2; i<=n; i++)  t[i-1] = c[i];
        t[0] = n-1;
        free(c);
        c = t;
    }

    //  exactly zero?
    if (zero == 0) {
        free(c);
        c = (signed int *)malloc(2*sizeof(signed int));
        c[0] = 1;
        c[1] = 0;
    }

    return c;
}


//  addition
signed int *bigint_add(signed int *a, signed int *b) {

    //  both positive
    if ((a[0]>0) && (b[0]>0))
        return bigint_uadd(a,b);
    
    //  both negative
    if ((a[0]<0) && (b[0]<0))
        return bigint_negate(bigint_uadd(a,b));

    //  a positive, b negative
    if ((a[0]>0) && (b[0]<0)) {
        if (bigint_ucompare(a,b) == -1)
            // |a|<|b|
            return bigint_negate(bigint_usub(b,a));
        else
            // |a|>=|b|
            return bigint_usub(a,b);
    }

    //  a negative, b positive
    if ((a[0]<0) && (b[0]>0)) {
        if (bigint_ucompare(a,b) == 1)
            // |a|>|b|
            return bigint_negate(bigint_usub(a,b));
        else
            // |a|<=|b|
            return bigint_usub(b,a);
    }
}

//  subtract
signed int *bigint_sub(signed int *a, signed int *b) {

    //  both positive
    if ((a[0]>0) && (b[0]>0)) {
        if (bigint_ucompare(a,b) == -1)
            // |a|<|b|
            return bigint_negate(bigint_usub(b,a));
        else
            return bigint_usub(a,b);
    }

    //  both negative
    if ((a[0]<0) && (b[0]<0)) {
        if (bigint_ucompare(a,b) == 1)
            return bigint_negate(bigint_usub(a,b));
        else
            return bigint_usub(b,a);
    }

    //  a positive, b negative
    if ((a[0]>0) && (b[0]<0))
        return bigint_uadd(a,b);

    //  a negative, b positive
    if ((a[0]<0) && (b[0]>0))
        return bigint_negate(bigint_uadd(a,b));
}


//  signed compare
int bigint_compare(signed int *a, signed int *b) {
    signed int i;

    //  Opposite signs
    if ((a[0] > 0) && (b[0] < 0)) return  1;   // a>0, b<0
    if ((a[0] < 0) && (b[0] > 0)) return -1;  // a<0, b>0

    //  Same sign, check by length
    if ((abs(a[0]) > abs(b[0])) && (a[0] > 0)) return  1; // a>0, b>0, a longer than b
    if ((abs(b[0]) > abs(a[0])) && (a[0] > 0)) return -1; // a>0, b>0, b longer than a
    if ((abs(a[0]) > abs(b[0])) && (a[0] < 0)) return -1; // a<0, b<0, a longer than b
    if ((abs(b[0]) > abs(a[0])) && (a[0] < 0)) return  1; // a<0, b<0, b longer than a

    //  Same sign and length, check position by position
    if (a[0] > 0) {
        for(i=1; i <= abs(a[0]); i++) {
            if (a[i] > b[i]) return 1;
            if (b[i] > a[i]) return -1;
        }
    } else {
        for(i=1; i <= abs(a[0]); i++) {
            if (a[i] > b[i]) return -1;
            if (b[i] > a[i]) return  1;
        }
    }

    //  If we get here, the numbers are equal
    return 0;
}


// copy bigint
signed int *bigint_copy(signed int *a) {
    signed int *b;

    b = (signed int *)malloc((abs(a[0])+1)*sizeof(signed int));
    memcpy((void *)b, (void *)a, (abs(a[0])+1)*sizeof(signed int));
    return b;
}
 

// multiply by single digit and shift answer
signed int *bigint_umultd(signed int *a, signed int d, unsigned int s) {
    signed int *p, carry=0, n;
    int i;

    n = abs(a[0]) + s + 2;
    p = (signed int *)malloc(n*sizeof(signed int));
    memset((void *)p, 0, n*sizeof(signed int));
    p[0] = n-1;

    for(i=abs(a[0]); i>=1; i--) {
        p[i+1] = a[i] * d + carry;
        if (p[i+1] > 9999) {
            carry = p[i+1] / 10000;
            p[i+1] = p[i+1] % 10000;
        } else
            carry = 0;
    }

    if (carry) p[1] = carry;
    return p;
}


//  unsigned multiply
signed int *bigint_umult(signed int *a, signed int *b) {
    int i, n;
    signed int *m, *x, *y;
    signed int *p=(signed int *)NULL, *q=(signed int *)NULL;

    if (abs(a[0]) > abs(b[0])) {
        x = a;  y = b;
    } else {
        x = b;  y = a;
    }

    m = (signed int *)malloc(2*sizeof(signed int));
    m[0] = 1;  m[1] = 0;
    
    for(i=abs(y[0]); i>=1; i--) {
        if (p != NULL) free(p);
        if (q != NULL) free(q);
        p = bigint_umultd(x, y[i], abs(y[0])-i);
        q = bigint_uadd(m, p);
        if (m != NULL) free(m);
        m = bigint_copy(q);
    }
    
    if (p != NULL) free(p);
    if (q != NULL) free(q);
    return m;
}


//  signed multiply
signed int *bigint_mult(signed int *a, signed int *b) {

    if (((a[0]>0) && (b[0]>0)) ||
        ((a[0]<0) && (b[0]<0)))
        return bigint_umult(a,b);

    return bigint_negate(bigint_umult(a,b));
}


//  iszero
int bigint_iszero(signed int *a) {
    int i;

    for(i=1; i<=abs(a[0]); i++) {
        if (a[i]) return 0;
    }
    return 1;
}


//  simple divide
signed int *bigint_udiv(signed int *a, signed int *b) {
    signed int *q, *t, *c, *x=NULL, *y=NULL;

    q = (signed int *)malloc(2*sizeof(signed int));
    q[0] = 1;
    q[1] = 0;

    c = (signed int *)malloc(2*sizeof(signed int));
    c[0] = 1;
    c[1] = 1;

    t = bigint_copy(a);

    while (1) {
        if (x != NULL) free(x);
        if (y != NULL) free(y);
        x = bigint_usub(t,b);
        y = bigint_uadd(q,c);
        free(t);
        free(q);
        t = bigint_copy(x);
        q = bigint_copy(y);

        if (bigint_ucompare(t,b) < 1)
            break;
    }

    free(x);
    free(y);
    free(t);
    free(c);
    return q;
}


//  signed divide
signed int *bigint_divide(signed int *a, signed int *b) {
    int sa, sb;
    signed int *c;

    if ((bigint_iszero(b)) ||
        (bigint_ucompare(a,b) == -1)) {
        c = (signed int *)malloc(2*sizeof(signed int));
        c[0] = 1;
        c[1] = 0;
        return c;
    }

    sa = (a[0] < 0) ? -1 : 1;
    sb = (b[0] < 0) ? -1 : 1;
    a[0] = abs(a[0]);
    b[0] = abs(b[0]);

    if (((sa<0) && (sb<0)) ||
        ((sa>0) && (sb>0))) {
        c = bigint_udiv(a,b);
    } else {
        c = bigint_negate(bigint_udiv(a,b));
    }

    a[0] = sa*a[0];
    b[0] = sa*b[0];

    return c;
}


int main() {
    signed int *b;
    char *s = "123456789012345678901";
    char *w = "-123456789012345678901";
    char t[32];

    //b = bigint_input(s);
    //bigint_output(b,t);
    //printf("%s\n", t);
    //free(b);

    //b = bigint_input(w);
    //bigint_output(b,t);
    //printf("%s\n", t);
    //free(b);

    //signed int A[] = {-4, 123,234,456,567};
    //signed int B[] = { 4, 123,234,457,568};
    //printf("%d\n", bigint_compare(A,B));
    //printf("%d\n", bigint_compare(B,A));
    //printf("\n\n");
    //bigint_output(A,t);
    //printf("A = %s\n", t);
    //bigint_negate(A);
    //bigint_output(A,t);
    //printf("A = %s\n", t);
    //bigint_output(B,t);
    //printf("B = %s\n", t);
    //bigint_negate(B);
    //bigint_output(B,t);
    //printf("B = %s\n", t);

    signed int A[] = {3,1,4471,8279};
    signed int B[] = {2,4,8979};
    signed int *C;

    C = bigint_divide(A,B);
    bigint_output(C,t);
    printf("C = %s\n", t);

    //C = bigint_sub(A,B);
    //bigint_output(C,t);
    //printf("C = %s\n", t);

    //C = bigint_mult(A,B);
    //bigint_output(C,t);
    //printf("C = %s\n", t);

    //signed int D[] = {-12,0,0,0,0,0,0,0,0,2,0,0,0};
    //printf("bigint_iszero(D) = %d\n", bigint_iszero(D));
}

