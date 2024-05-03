#
#  file:  DBNS_test.py
#
#  Run some tests on the DBNS class.
#

from DBNS import *

import sys
import time

def main():
    """Run some tests on DBNS"""

    sss = time.time()

    if (len(sys.argv) == 1):
        print
        print "DBNS_test repr | add | mult | all"
        print
        print "Any combination of arguments allowed:"
        print
        print "    repr - can we represent the numbers?"
        print "    add  - check addition accuracy"
        print "    mult - check multiplication accuracy"
        print "    all  - run all tests"
        print
        return

    print
    print "Running DBNS tests (%s)" % time.ctime()
    print

    #  Representation check
    if ("repr" in sys.argv) or ("all" in sys.argv):
        start = time.time()
        print "*** Checking representation, [0,30000):"
        for i in range(30000):
            if (DBNS(i).Value() != i):
                print "    %d != %d" % (DBNS(i), i)
        print
        print "    time = %0.3f" % (time.time()-start)
        print

    #  Addition check
    if ("add" in sys.argv) or ("all" in sys.argv):
        start = time.time()
        print "*** Checking all additions: [0,10000)"
        n = range(0,10000)
        for i in n:
            for j in n:
                try:
                    c = DBNS(i) + DBNS(j)
                except Exception as e:
                    print "    %d + %d = error" % (i,j)
                    raise e
                if (c.Value() != (i+j)):
                    print "    %d + %d = %d, not %d" % (i, j, c.Value(), i+j)
        print
        print "    time = %0.3f" % (time.time()-start)
        print

    #  Multiplication
    if ("mult" in sys.argv) or ("all" in sys.argv):
        start = time.time()
        print "*** Checking all multiplications: [0,10000)"
        n = range(1,10000)
        for i in n:
            for j in n:
                try:
                    c = DBNS(i) * DBNS(j)
                except Exception as e:
                    print "    %d * %d = error" % (i,j)
                    raise e
                if (c.Value() != (i*j)):
                    print "    %d * %d = %d, not %d" % (i, j, c.Value(), i+j)
        print
        print "    time = %0.3f" % (time.time()-start)
        print

    print "Complete.  Total time = %0.3f" % (time.time()-sss)
    print


if (__name__ == "__main__"):
    main()

