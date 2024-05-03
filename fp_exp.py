def fact(n):
    if (n == 1):
        return 1.0
    return n * fact(n-1)


def exponential(x, tol=1e-25):
    ans = 0.0
    term= 1.0
    p = 1.0

    while (abs(term) > tol):
        ans += term
        term = x**p / fact(p)
        p += 1

    return ans

def fp_exp():
    from math import exp

    for x in range(25):
        a = 1.0 + x
        print "%3d  %0.16e  %0.16e" % (int(a), exponential(a), exp(a))

    print

    for x in range(25):
        a = 1.0 + x
        print "%3d  %0.16e  %0.16e" % (int(-a), exponential(-a), exp(-a))

    print

    for x in range(25):
        a = 1.0 + x
        print "%3d  %0.16e  %0.16e" % (int(-a), 1.0/exponential(a), exp(-a))


