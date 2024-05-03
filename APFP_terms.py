from APFP import *

import numpy as np
import matplotlib.pylab as plt


def main():
    print
    print "Term use as function of precision:"
    print
    print "prec   sin   cos   exp"

    sterms = []
    cterms = []
    eterms = []
    prec = []

    for i in range(8,1000,10):
        APFP.dprec = i
        x = APFP(1.123456789)
        y = x.sin()
        st = x.terms
        y = x.cos()
        ct = x.terms
        y = x.exp()
        et = x.terms
        print "%4d  %4d  %4d  %4d" % (i, st, ct, et)
        sterms.append(st)
        cterms.append(ct)
        eterms.append(et)
        prec.append(i)

    print

    plt.plot(prec, eterms, "-r")
    plt.plot(prec, cterms, "-b")
    plt.plot(prec, sterms, "-g")
    plt.xlabel("Decimal Precision")
    plt.ylabel("Number of Terms")
    plt.text(800,350,"exp", fontsize=12)
    plt.text(800,150,"cos, sin", fontsize=12)
    plt.savefig("APFP_terms.eps", format="eps", dpi=300)
    plt.show()

main()

