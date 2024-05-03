#
#  file:  PFP_compare_plot.py
#
#  Plot comparison of (0.1+0.2)e^N = 0.3e^N for positive N
#
#  RTK, 14-Sep-2016
#  Last update:  14-Sep-2016
#
###############################################################

import numpy as np
import matplotlib.pylab as plt

def main():
    plt.figure(figsize=(4.69, 1.00))
    d = np.loadtxt("PFP_compare.txt")
    plt.plot(d[200:,0],d[200:,1])
    plt.tight_layout(pad=0 ,w_pad=0, h_pad=0)
    plt.savefig("PFP_compare.eps", dpi=1000, format='eps')
    plt.show()


if __name__ == "__main__":
    main()

