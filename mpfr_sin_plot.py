import numpy as np
import matplotlib.pylab as plt
import sys
import os

def main():
    mpfr = np.loadtxt("mpfr_sin_timing.txt")
    mp   = np.loadtxt("mpfr_sin_py_timing.txt")
    prec = mp[:,0]

    plt.plot(prec, mp[:,1], "-r")
    plt.plot(prec, mpfr[:,1], "-b")

    plt.xlabel("Precision (bits)")
    plt.ylabel("Time (s)")
    plt.text(14940,25.3, "mpmath", fontsize=12)
    plt.text(13363,5.99, "MPFR", fontsize=12)
    plt.savefig("mpfr_sin_plot.eps", format="eps", dpi=300)
    plt.show()


main()

