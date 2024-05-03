from decimal import *

def main():
    setcontext(Context(prec=16))

    x = Decimal("0.25")
    r = Decimal("3.8")
    one = Decimal("1")

    for i in xrange(8):
        a = one - x
        b = x * a
        x = r * b
        print x

    for i in xrange(10000000):
        a = one - x
        b = x * a
        x = r * b

    print

    for i in xrange(8):
        a = one - x
        b = x * a
        x = r * b
        print x

main()

