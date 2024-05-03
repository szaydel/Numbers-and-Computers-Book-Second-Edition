import numpy as np
import os
import sys
from math import sqrt
from APFP import *


def main():
    """Test subtraction using sqrts"""

    APFP.dprec = 40

    #A = np.fromfile("PFP_squares_a.raw", dtype="float64")
    #B = np.fromfile("PFP_squares_b.raw", dtype="float64")

    A = 1e10*np.random.random(100)
    B = 1e10*np.random.random(100)

    D0 = A - B
    D1 = (np.sqrt(A) + np.sqrt(B))*(np.sqrt(A) - np.sqrt(B))

    for i in xrange(A.shape[0]):
        a = APFP(float(A[i]))
        b = APFP(float(B[i]))
        d = a - b
        dD0 = abs(d - APFP(float(D0[i])))
        dD1 = abs(d - APFP(float(D1[i])))
        print "%010.16f %010.16f %40s %40s" % (abs(D0[i]), abs(D1[i]), str(dD0), str(dD1))


if (__name__ == "__main__"):
    main()

