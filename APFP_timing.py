from APFP import *

import numpy as np
import matplotlib.pylab as plt
import time


def main():
    print
    print "Time as function of precision:"
    print
    print "prec        sin              cos             exp"

    sterms = []
    cterms = []
    eterms = []
    prec = []

    for i in range(8,2500,100):
        APFP.dprec = i
        x = APFP(1.123456789)
        s=time.time(); y = x.sin(); st = time.time()-s
        s=time.time(); y = x.cos(); ct = time.time()-s
        s=time.time(); y = x.exp(); et = time.time()-s
        print "%4d  %0.12f  %0.12f  %0.12f" % (i, st, ct, et)
        sterms.append(st)
        cterms.append(ct)
        eterms.append(et)
        prec.append(i)

    print

    plt.plot(prec, eterms, "-r")
    plt.plot(prec, cterms, "-b")
    plt.plot(prec, sterms, "-g")
    plt.xlabel("Decimal Precision")
    plt.ylabel("Time (s)")
    plt.text(2250,2,"exp", fontsize=12)
    plt.text(1950,2.5,"cos, sin", fontsize=12)
    plt.savefig("APFP_timing.eps", format="eps", dpi=300)
    plt.show()

main()

