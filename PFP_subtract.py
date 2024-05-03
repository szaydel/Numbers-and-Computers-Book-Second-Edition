import numpy as np
from APFP import *


def main():
    """Subtracting nearly equal numbers"""

    APFP.dprec = 60
    N = 10000

    for e in [1]: #range(17):
        scale = 10.0**e
        A = 3.0 + np.random.random(N)
        M = (np.random.random(N)+1)/scale
        B = A.copy() + M
        M = abs(M)
        #print "A[100]=%0.16f, B[100]=%0.16f, M[100]=%0.16f" % (A[100],B[100],M[100])
        D = abs(A-B)
        S = APFP(0)

        for i in xrange(D.shape[0]):
            a = APFP("%0.16f" % A[i])
            b = APFP("%0.16f" % B[i])
            d = abs(a-b)
            delta = abs(d - APFP("%0.16f" % D[i]))
            S = S + delta
            print "%0.16f  %s  %0.16f  %s" % (D[i],d, M[i], delta)

        #print ("%2d" % e), S/APFP(D.shape[0])


if __name__ == "__main__":
    main()

