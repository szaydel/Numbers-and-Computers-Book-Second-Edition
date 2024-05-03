#
#  file:  DBLNS.py
#
#  An implementation of the 2D LNS DBNS number system.
#
#  RTK, 20-Jun-2016
#  Last update:  21-Jun-2016
#
###############################################################

from bisect import bisect_left

###############################################################
#  Build the table once on import
#
p = []

for i in range(-600,1023+1):
    for j in range(-600,646+1):
        p.append(((2.0**i)*(3.0**j),i,j))
p.sort()

gValues = []
gBexp = []
gTexp = []

for i in p:
    v,b,t = i
    gValues.append(v)
    gBexp.append(b)
    gTexp.append(t)
del p,v,b,t,i


###############################################################
#  DBLNS
#
class DBLNS:
    """DBLNS numbers"""

    #----------------------------------------------------------
    #  Value
    #
    def Value(self):
        """Value of the number"""

        try:
            v = ((-1.0)**self.s)*(2.0**self.b)*(3.0**self.t)
        except:
            v = float('inf') if (self.s==0) else float('-inf')
        return v

    
    #----------------------------------------------------------
    #  __str__
    #
    def __str__(self):
        """String form"""

        return "%0.6f" % self.Value()


    #----------------------------------------------------------
    #  __repr__
    #
    def __repr__(self):
        """Representation"""

        return self.__str__()


    #----------------------------------------------------------
    #  Closest
    #
    def Closest(self, x):
        """Find the closest to x"""

        pos = bisect_left(gValues, x)

        if (pos == 0) or (pos == len(gValues)):
            return pos

        if (gValues[pos]-x) < (x-gValues[pos-1]):
           return pos
        else:
           return pos-1


    #----------------------------------------------------------
    #  __mul__
    #
    def __mul__(self, y):
        """Multiplication"""

        z = DBLNS(0)
        z.s = (self.s + y.s) % 2
        z.b = self.b + y.b
        z.t = self.t + y.t
        return z


    #----------------------------------------------------------
    #  __div__
    #
    def __div__(self, y):
        """Division"""

        z = DBLNS(0)
        z.s = (self.s + y.s) % 2
        z.b = self.b - y.b
        z.t = self.t - y.t
        return z


    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, x):
        """Represent x"""

        #  Use the pre-generated list of representable values to find
        #  the closest approximation to x.
        idx = self.Closest(abs(x))

        self.b = gBexp[idx]
        self.t = gTexp[idx]
        self.s = 0 if (x >= 0.0) else 1

#  end DBLNS.py

