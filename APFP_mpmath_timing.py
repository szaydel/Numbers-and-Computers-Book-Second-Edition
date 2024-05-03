from APFP import *
from mpmath import mp

import numpy as np
import matplotlib.pylab as plt
import time


def main():
    print
    print "Time as function of precision:"
    print
    print "                sin               exp"
    print "prec        APFP    mpmath    APFP   mpmath"

    for i in [100,1000,2000,3000,4000,5000]:
        APFP.dprec = i
        mp.dps = i
        s=time.time(); y = APFP("1").sin(); st0 = time.time()-s
        s=time.time(); y = APFP("0.5").exp(); et0 = time.time()-s
        s=time.time(); y = mp.sin("1"); st1 = time.time()-s
        s=time.time(); y = mp.exp("0.5"); et1 = time.time()-s
        print "%4d  %0.6f  %0.6f  %0.6f  %0.6f" % (i, st0, st1, et0, et1)

main()

