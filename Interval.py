#
#  file:  Interval.py
#
#  Implements a simple interval arithmetic class in Python.
#
#  RTK, 06-Nov-2014
#  Last update:  07-Nov-2014
#
###############################################################

from math import exp
from ctypes import cdll
from ctypes.util import find_library

class Interval:
    libc = cdll.LoadLibrary(find_library("m"))
    FE_TOWARDZERO = 0xc00
    FE_DOWNWARD = 0x400
    FE_UPWARD = 0x800
    FE_TONEAREST = 0

    def __init__(self, a,b=None):
        if (b == None):
            self.a = a 
            self.b = a
        else:
            self.a = a
            self.b = b

    def __RoundDown(self):
        self.libc.fesetround(self.FE_DOWNWARD)

    def __RoundUp(self):
        self.libc.fesetround(self.FE_UPWARD)

    def __RoundNearest(self):
        self.libc.fesetround(self.FE_TONEAREST)

    def __add__(self, y):
        self.__RoundDown()
        a = self.a + y.a
        self.__RoundUp()
        b = self.b + y.b
        self.__RoundNearest()
        return Interval(a,b)

    def __sub__(self, y):
        self.__RoundDown()
        a = self.a - y.b 
        self.__RoundUp()
        b = self.b - y.a
        self.__RoundNearest()
        return Interval(a,b)

    #def __mul__(self, y):
    #    self.__RoundDown()
    #    a = self.a * y.a
    #    t = self.a * y.b
    #    if (t < a):
    #        a = t
    #    t = self.b * y.a
    #    if (t < a):
    #        a = t
    #    t = self.b * y.b
    #    if (t < a):
    #        a = t

    #    self.__RoundUp()
    #    b = self.a * y.a
    #    t = self.a * y.b
    #    if (t > b):
    #        b = t
    #    t = self.b * y.a
    #    if (t > b):
    #        b = t
    #    t = self.b * y.b
    #    if (t > b):
    #        b = t

    #    self.__RoundNearest()
    #    return Interval(a,b)

    def __mul__(self, y):
        self.__RoundDown()
        a = min([self.a*y.a,self.a*y.b,self.b*y.a,self.b*y.b])
        self.__RoundUp()
        b = max([self.a*y.a,self.a*y.b,self.b*y.a,self.b*y.b])
        self.__RoundNearest()
        return Interval(a,b)

    def recip(self):
        if (self.a < 0.0) and (self.b > 0.0):
            return Interval(-exp(1000.0), exp(1000.0))
        self.__RoundDown()
        a = 1.0 / self.b
        self.__RoundUp()
        b = 1.0 / self.a
        self.__RoundNearest()
        return Interval(a,b)

    def __div__(self, y):
        return self.__mul__(y.recip())

    def __pow__(self, n):
        if (n%2) == 1:
            self.__RoundDown()
            a = self.a**n
            self.__RoundUp()
            b = self.b**n
        elif (self.a >= 0):
            self.__RoundDown()
            a = self.a**n
            self.__RoundUp()
            b = self.b**n
        elif (self.b < 0):
            self.__RoundDown()
            a = self.b**n
            self.__RoundUp()
            b = self.a**n
        else:
            a = 0.0
            self.__RoundUp()
            b = self.a**n
            t = self.b**n
            if (t > b):
                b = t
        self.__RoundNearest()
        return Interval(a,b)

    def __float__(self):
        return 0.5*(self.a + self.b) 

    def __neg__(self):
        return Interval(-self.b, -self.a)

    def __abs__(self):
        pass 

    def __str__(self):
        return "[%0.16f, %0.16f]" % (self.a, self.b)

    def __repr__(self):
        return self.__str__()


