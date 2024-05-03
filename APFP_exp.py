from APFP import *
import time
import sys


def exp0():
    fact = APFP("1")
    lasts = APFP("1")
    s = APFP("0")
    k = APFP("0")
    i = 0
    
    while (s != lasts):
        lasts = s
        s += (k + APFP(1)) / fact
        k += APFP(1)
        fact *= k
        i += 1

    return [APFP("0.5") * s, i]


def exp1():
    fact = APFP("1")
    lasts = APFP("1")
    s = APFP("0")
    k = APFP("0")
    i = 0

    while (s != lasts):
        lasts = s
        s += (k + APFP(1)) / fact
        k += APFP(1)
        m = APFP(2)*k + APFP(1)
        fact *= m - APFP(1)
        fact *= m
        i += 1

    return [APFP(2)*s, i]


def exp2():
    fact = APFP("1")
    lasts = APFP("1")
    s = APFP("0")
    k = APFP("0")
    i = 0

    while (s != lasts):
        lasts = s
        t = APFP(3)*k
        s += (t*t + APFP(1)) / fact
        k += APFP(1)
        m = APFP(3)*k
        fact *= m - APFP(2)
        fact *= m - APFP(1)
        fact *= m
        i += 1

    return [s, i]


def exp3():
    fact = APFP("1")
    lasts = APFP("1")
    s = APFP("0")
    k = APFP("1")
    i = 0
    
    while (s != lasts):
        lasts = s
        t = k*k
        t = t*t*t*k
        s += t / (APFP("877") * fact)
        k += APFP(1)
        fact *= k
        i += 1

    return [s, i]


def calc(prec):
    APFP.dprec = prec

    s=time.time(); e0,i0 = exp0(); t0=time.time()-s
    s=time.time(); e1,i1 = exp1(); t1=time.time()-s
    s=time.time(); e2,i2 = exp2(); t2=time.time()-s
    s=time.time(); e3,i3 = exp3(); t3=time.time()-s
    s=time.time(); v = APFP("1"); _ = v.exp(); terms = v.terms; te = time.time()-s

    return [[e0,i0,t0],
            [e1,i1,t1],
            [e2,i2,t2],
            [e3,i3,t3],
            [_, terms, te]]
    

def main():
    for i in range(8,4500,100):
        t = calc(i)
        print ("%4d" % i),
        print ("  %4d  %0.5f" % (t[4][1],t[4][2])),
        print ("  %4d  %0.5f" % (t[0][1],t[0][2])),
        print ("  %4d  %0.5f" % (t[1][1],t[1][2])),
        print ("  %4d  %0.5f" % (t[2][1],t[2][2])),
        print ("  %4d  %0.5f" % (t[3][1],t[3][2]))
    print


if (__name__ == "__main__"):
    main()


