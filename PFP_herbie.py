import random
from APFP import *
from math import sqrt

def f(x):
    return x*(sqrt(x-1) - sqrt(x))

def g(x):
    return (-x) / (sqrt(x) + sqrt(x-1))


def main():
    """Test Herbie's claims"""

    APFP.dprec = 100

    M = 10000
    SA = APFP(0)
    SB = APFP(0)

    for i in xrange(M):
        x = 1001.0*random.random() + 1.0
        a = f(x)
        b = g(x)

        X0 = (APFP(x) - APFP(1)).sqrt()
        X1 = APFP(x).sqrt()
        B = APFP(x)*(X0 - X1)

        DA = abs(B - APFP("%0.18f" % a))
        DB = abs(B - APFP("%0.18f" % b))

        SA += DA
        SB += DB

        #print "DA: %s" % DA
        #print "DB: %s" % DB
        #print

    print
    print "Running %d tests:" % M
    print
    print "    mean deviation using f() = %s" % (SA / APFP(M))
    print "    mean deviation using g() = %s" % (SB / APFP(M))
    print


if (__name__ == "__main__"):
    main()

