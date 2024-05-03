#
#  Direct copy of the Python decimal library example to calculate pi
#

from APFP import *
    
APFP.dprec = 1000

def pi():
    three = APFP("3")
    lasts = APFP("0")
    t = APFP("3")
    s = APFP("3")
    n = APFP("1")
    na = APFP("0")
    d = APFP("0")
    da = APFP("24")
    eight = APFP("8")
    t32 = APFP("32")

    while s != lasts:
        lasts = s
        n, na = n+na, na + eight
        d, da = d+da, da + t32
        t = (t * n) / d
        s += t

    return s

print pi()

