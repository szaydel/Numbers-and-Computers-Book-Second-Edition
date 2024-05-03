#
#  file:  PFP_division.py
#
#  Floating-point division deviations
#
#  RTK, 12-Sep-2016
#  Last update:  12-Sep-2016
#
###############################################################

from math import exp, log
from APFP import *

def main():
    """Run the tests"""

    APFP.dprec = 30

    M = ["15","156","1506","15006","150006","1500006","15000006","150000006","1500000006"]
    N = ["3","35","355","3555","35555","3555555","35555555","355555555","3555555555"]

    for i in M:
        for j in N:
            a = float(i)/float(j)
            b = exp(log(float(i))-log(float(j)))
            c = str((APFP(i)/APFP(j)))
            da = str(abs(APFP(a)-APFP(c)))
            db = str(abs(APFP(b)-APFP(c)))
            print "%s  %s  %0.18f  %0.18f  %s  %s  %s" % (i,j,a,b,c,da,db)

    print
    APFP.dprec = 60
    M = ["1.6","15.6","150.6","1500.6","15000.6","150000.6","1500000.6","15000000.6","150000000.6"]
    N = ["0.36","3.56","35.06","350.06","3500.06","35000.06","350000.06","3500000.06","35000000.06"]

    for ii in M:
        for jj in N:
            i = ii + "00000000000000000000"
            j = jj + "00000000000000000000"
            a = float(i)/float(j)
            b = exp(log(float(i))-log(float(j)))
            c = str((APFP(i)/APFP(j)))
            da = str(abs(APFP(a)-APFP(c)))
            db = str(abs(APFP(b)-APFP(c)))
            print "%s  %s  %0.18f  %0.18f  %s  %s  %s" % (i,j,a,b,c,da,db)

    print
    APFP.dprec = 120
    M = ["15","156","1506","15006","150006","1500006","15000006","150000006","1500000006"]
    N = ["3","35","355","3555","35555","3555555","35555555","355555555","3555555555"]

    for ii in M:
        for jj in N:
            i = ii + "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            j = jj + "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
            a = float(i)/float(j)
            b = exp(log(float(i))-log(float(j)))
            c = str((APFP(i)/APFP(j)))
            da = str(abs(APFP(a)-APFP(c)))
            db = str(abs(APFP(b)-APFP(c)))
            print "%s  %s  %0.18f  %0.18f  %s  %s  %s" % (i,j,a,b,c,da,db)


if (__name__ == "__main__"):
    main()

