from APFP import *

import numpy as np
import matplotlib.pylab as plt


def main():
    print
    print "Iterations as function of precision:"
    print
    print "prec   sqrt"

    siters = []
    prec = []

    for i in xrange(8,30000,500):
        APFP.dprec = i
        x = APFP(1.123456789)
        y = x.sqrt()
        st = x.iters
        print "%4d  %4d" % (i, st)
        siters.append(st)
        prec.append(i)

    print

    plt.plot(prec, siters, "-r")
    plt.xlabel("Decimal Precision")
    plt.ylabel("Number of Iterations")
    axes = plt.gca()
    axes.set_ylim([0,18])
    plt.text(25000,16,"sqrt", fontsize=12)
    plt.savefig("APFP_iters.eps", format="eps", dpi=300)
    plt.show()

main()

