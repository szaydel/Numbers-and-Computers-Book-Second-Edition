#
#  Table of RSD digits
#

import sys

def pp(x,i,digits):
    """Print the representations"""

    print "%2d" % i,":",

    if (digits == 2):
        for d1,d0 in x:
            print "[%d,%d] " % (d1,d0),
        print
    else:
        for d2,d1,d0 in x:
            print "[%d,%d,%d] " % (d2,d1,d0),
        print


def representations(n, digits):
    """All the ways n can be represented in two digits"""
    
    ans = []
    
    if (digits == 2):
        for i in range(-9,10):
            for j in range(-9,10):
                if (10*i+j) == n:
                    ans.append((i,j))
    else:
        for i in range(-9,10):
            for j in range(-9,10):
                for k in range(-9,10):
                    if (100*i+10*j+k) == n:
                        ans.append((i,j,k))

    return ans


def main():
    """0..99 table using two radix-10 maximally redundant representation"""
    
    if (len(sys.argv) == 1):
        print
        print "rsd_table <n>"
        print
        print "  <n> - number digits 2|3"
        print
        return

    digits = int(sys.argv[1])

    for i in range(100):
        pp(representations(i,digits),i,digits)


main()

