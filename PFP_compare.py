#
#  file:  PFP_compare.py
#
#  Comparison of FP numbers
#
#  RTK, 12-Sep-2016
#  Last update:  12-Sep-2016
#
###############################################################

from APFP import *
import numpy as np

eps32 = np.finfo(np.float32).eps
eps64 = np.finfo(np.float64).eps


def main():
    """Risks of comparing FP numbers"""

    ex = [int(i) for i in list(np.linspace(-200,200,401))]

    for e in ex:
        a = float("0.1e"+("%d" % e)) + float("0.2e"+("%d" % e))

        if (a == float("0.3e"+("%s" % e))):
            print "%d  1" % e
        else:
            print "%d  0" % e


if __name__ == "__main__":
    main()

