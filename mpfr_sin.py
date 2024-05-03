from mpmath import mp
import time
import sys

def main():
    p = int(sys.argv[1])
    n = int(sys.argv[2])

    sss = time.time()

    mp.prec = p
    t = mp.mpf(1.0)

    for i in xrange(n):
        s = mp.sin(t)

    eee = time.time()

    print "\nruntime = %0.16f\n" % (eee-sss)


main()

