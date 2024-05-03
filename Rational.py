#
#  file:  Rational.py
#
#  Implements a simple rational number class in Python.
#
#  RTK, 13-Aug-2014
#  Last update:  15-Oct-2014
#
###############################################################

from types import *

class Rational:

    def __init__(self, n,d=None):
        if (d == None):
            t = float(n).as_integer_ratio()
            n = t[0]
            d = t[1]
        g = self.__gcd(n,d)
        self.n = n/g
        self.d = d/g
        if (self.d < 0):
            self.n *= -1
            self.d = abs(self.d)

    def __gcd(self, a,b):
        while b:
            a,b = b, a % b
        return abs(a)

    #def __gcd(self, a,b):
    #    if (a == 0):
    #        return b
    #    return self.__gcd( b % a, a)

    def __add__(self, b):
        t = type(b)
        if (t == InstanceType):
            n = self.n*b.d + self.d*b.n
            d = self.d*b.d
        elif (t == IntType) or (t == LongType):
            n = self.n + self.d*b
            d = self.d
        return Rational(n,d)

    def __radd__(self, b):
        return self.__add__(b)

    def __sub__(self, b):
        t = type(b)
        if (t == InstanceType):
            n = self.n*b.d - self.d*b.n
            d = self.d*b.d
        elif (t == IntType) or (t == LongType):
            n = self.n - self.d*b
            d = self.d
        return Rational(n,d)

    def __rsub__(self, b):
        t = type(b)
        if (t == IntType) or (t == LongType):
            n = b*self.d - self.n
            d = self.d
            return Rational(n,d)

    def __mul__(self, b):
        t = type(b)
        if (t == InstanceType):
            n = self.n * b.n
            d = self.d * b.d
        elif (t == IntType) or (t == LongType):
            n = self.n * b
            d = self.d
        return Rational(n,d)

    def __rmul__(self, b):
        return self.__mul__(b)

    def __div__(self, b):
        t = type(b)
        if (t == InstanceType):
            n = self.n * b.d
            d = self.d * b.n
        elif (t == IntType) or (t == LongType):
            n = self.n
            d = self.d * b
        return Rational(n,d)

    def __rdiv__(self, b):
        if (type(b) == IntType):
            n = b * self.d 
            d = self.n
            return Rational(n,d)

    def __str__(self):
        if (self.n == 0):
            return "0"
        elif (self.d == 1):
            return "%d" % self.n
        else:
            return "%d/%d" % (self.n, self.d)

    def __repr__(self):
        return self.__str__()

    def __float__(self):
        return float(self.n) / float(self.d)

    def __int__(self):
        return int(self.__float__())

    def __long__(self):
        return long(self.__float__())

    def __neg__(self):
        return Rational(-self.n, self.d)

    def __abs__(self):
        return Rational(abs(self.n), self.d)


