import numpy as np
import matplotlib.pylab as plt

def main():
    d = np.loadtxt("APFP_exp_results.txt")
    i = d[:,0]  #  precision
    ex = d[:,1] #  exp method iterations
    e0 = d[:,3]
    e1 = d[:,5]
    e2 = d[:,7]
    e3 = d[:,9]

    plt.plot(i,ex,"-k")
    plt.plot(i,e0,"-b")
    plt.plot(i,e1,"-g")
    plt.plot(i,e2,"-r")
    plt.plot(i,e3,"-c")

    plt.xlabel("Decimal Precision")
    plt.ylabel("Number of Terms")
    plt.text(3447,1183, "exp, $e_0$, $e_3$", fontsize=12)
    plt.text(3222,738, "$e_1$", fontsize=12)
    plt.text(3437,340, "$e_2$", fontsize=12)
    plt.savefig("APFP_exp_plot.eps", format="eps", dpi=300)
    plt.show()

main()

