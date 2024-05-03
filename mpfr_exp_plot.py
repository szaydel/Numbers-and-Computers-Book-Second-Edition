import numpy as np
import matplotlib.pylab as plt
import sys
import os

def main():
    mpfr = np.loadtxt("mpfr_exp_timing.txt")
    mp   = np.loadtxt("mpfr_exp_py_timing.txt")
    prec = mp[:,0]

    plt.plot(prec, mp[:,1], "-r")
    plt.plot(prec, mpfr[:,1], "-b")
    plt.show()


main()

