import numpy as np
from APFP import *

def main():
    """Kahan summation example"""

    M = 200
    APFP.dprec = 100
    kmean = APFP(0)
    smean = APFP(0)

    for j in xrange(M):
        N = 10001
        A = list(np.random.random(N)*1001.0)

        #  Stupid way
        sum = 0.0
        for i in xrange(N):
            sum = sum + A[i]
        #print "simple sum = %0.16f" % sum
        ssum = sum

        #  Kahan way
        sum = c = 0.0
        for i in xrange(N):
            y = A[i] - c
            t = sum + y
            c = (t - sum) - y
            sum = t
        #print "Kahan sum  = %0.16f" % sum
        ksum = sum

        #  Numpy - seems to be using Kahan summation
        #print "Numpy sum  = %0.16f" % np.array(A).sum()

        #  APFP
        sum = APFP(0)
        for i in xrange(N):
            #sum = sum + APFP(float(A[i]))
            sum = sum + APFP("%0.16f" % A[i])
        #print "APFP sum   =", sum

        #  Differences
        #smean = smean + abs(sum - APFP(float(ssum)))
        #kmean = kmean + abs(sum - APFP(float(ksum)))
        smean = smean + abs(sum - APFP("%0.16f" % ssum))
        kmean = kmean + abs(sum - APFP("%0.16f" % ksum))

    #  Mean deviation
    print "mean deviation for the simple sum =", smean/APFP(M)
    print "mean deviation for the Kahan  sum =", kmean/APFP(M)


main()

