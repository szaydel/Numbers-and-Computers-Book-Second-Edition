from APFP import *

def main():
    APFP.dprec = 100
   
    x = APFP("0.25")
    r = APFP("3.8")
    one = APFP("1")

    for i in xrange(8):
        x = r*x*(one-x)
        print x

    for i in xrange(100000):
        x = r*x*(one-x)

    print
    print

    for i in xrange(8):
        x = r*x*(one-x)
        print x
    
    print


main()

