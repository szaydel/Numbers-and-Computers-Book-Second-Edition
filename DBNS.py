#
#  file:  DBNS.py
#
#  A faster implementation of DBNS.
#
#  RTK, 10-Jun-2016
#  Last update:  16-Jun-2016
#
###############################################################

from math import log, ceil
import Image, ImageDraw


###############################################################
#  DBNS
#
class DBNS:
    """A base-2,3 double-number system class"""

    #----------------------------------------------------------
    #  Picture
    #
    def Picture(self, fname):
        """Output a graphical representation of the number"""

        s = self.__str__(delimit=False, spaces=False).split("\n")
        a = len(s[0])
        b = len(s)
        img = Image.new(mode="RGB",size=(self.gridSize*a+self.gridOffset, self.gridSize*b+self.gridOffset))
        draw = ImageDraw.Draw(img)

        for i in range(b):
            y = self.gridSize*i + self.gridOffset
            for j in range(a):
                x = self.gridSize*j+self.gridOffset
                c = self.zeroColor if (s[i][j]=="0") else self.oneColor
                draw.rectangle([x,y,x+(self.gridSize-self.gridOffset),y+(self.gridSize-self.gridOffset)], 
                                outline=c, fill=c)

        img.save(fname)


    #----------------------------------------------------------
    #  __str__
    #
    def __str__(self, delimit=True, spaces=True):
        """Pretty print"""

        m = ""

        for i in range(self.a+1):
            n = "|" if (delimit) else ""
            for j in range(self.b+1):
                if ((i,j) in self.n):
                    n += "1 " if (spaces) else "1"
                else:
                    n += "0 " if (spaces) else "0"
            if (spaces):
                n = n[0:-1]
            n += "|" if (delimit) else ""
            m += n + "\n"

        return m[0:-1]


    #----------------------------------------------------------
    #  __repr__
    #
    def __repr__(self):
        """Pretty print"""

        return self.__str__()


    #----------------------------------------------------------
    #  __pow__ - Exercise 4
    #
    def __pow__(self, y):
        """Exponent"""

        z = DBNS(1)

        for i in xrange(y.Value()):
            z *= self

        z.Ready()
        z.Limits()

        return z


    #----------------------------------------------------------
    #  Fact - Exercise 5
    #
    def Fact(self):
        """Compute the factorial"""

        z = DBNS(1)

        for i in xrange(self.Value()):
            z *= DBNS(i+1)

        z.Ready()
        z.Limits()

        return z


    #----------------------------------------------------------
    #  __mul__
    #
    def __mul__(self, y):
        """Multiply"""

        z = DBNS()

        for i,j in self.n:
            for a,b in y.n:
                elem = (i+a,j+b)
                if (elem not in z.n):
                    z.n.add(elem)
                else:
                    done = False
                    while (not done):
                        z.n.remove(elem)
                        elem = (elem[0]+1,elem[1])
                        if (elem not in z.n):
                            z.n.add(elem)
                            done = True
                            
        z.Ready()
        z.Limits()

        return z


    #----------------------------------------------------------
    #  __add__
    #
    def __add__(self, y):
        """Add a value"""

        z = DBNS()                          #  output object

        z.n = self.n.union(y.n)             #  addition is the union
        b = self.n.intersection(y.n)        #  collisions in the intersection

        while (b != set()):                 #  while there are collisions
            i,j = b.pop()                   #  get the collision
            z.n.remove((i,j))               #  remove from the answer
            if ((i+1,j) in z.n):            #  if 2*(i,j) -> (i+1,j) in the answer
                b.add((i+1,j))              #  it is a new collision
            else:
                z.n.add((i+1,j))            #  otherwise, add the new element

        z.Ready()                           #  get ready for addition
        z.Limits()                          #  set the maximum 2 and 3 exponents

        return z                            #  return the new object representing the sum


    #----------------------------------------------------------
    #  AdjacentColumn
    #
    #def AdjacentColumn(self):
    #    """Fix adjacent elements in the same column"""

    #    modified = True
    #    ever = False

    #    while (modified):
    #        modified = False

    #        b = self.n.copy()
    #        while (b != set()):
    #            i,j = b.pop()
    #            if ((i+1,j) in self.n):
    #                modified = True
    #                ever = True
    #                if ((i,j+1) in self.n):
    #                    self.n.remove((i,j+1))
    #                    if ((i,j+1) in b):
    #                        b.remove((i,j+1))
    #                    self.n.add((i+1,j+1))
    #                    b.add((i+1,j+1))
    #                else:
    #                    self.n.add((i,j+1))
    #                if ((i,j) in self.n):
    #                    self.n.remove((i,j))
    #                if ((i+1,j) in self.n):
    #                    self.n.remove((i+1,j))
    #                if ((i+1,j) in b):
    #                    b.remove((i+1,j))

    #    return ever


    #----------------------------------------------------------
    #  AdjacentRow
    #
    #def AdjacentRow(self):
    #    """Fix adjacent elements in the same row"""

    #    modified = True
    #    ever = False

    #    while (modified):
    #        modified = False

    #        b = self.n.copy()
    #        while (b != set()):
    #            i,j = b.pop()
    #            if ((i,j+1) in self.n):
    #                modified = True
    #                ever = True
    #                if ((i+2,j) in self.n):
    #                    self.n.remove((i+2,j))
    #                    if ((i+2,j) in b):
    #                        b.remove((i+2,j))
    #                    self.n.add((i+3,j))
    #                    b.add((i+3,j))
    #                else:
    #                    self.n.add((i+2,j))
    #                if ((i,j) in self.n):
    #                    self.n.remove((i,j))
    #                if ((i,j+1) in self.n):
    #                    self.n.remove((i,j+1))
    #                if ((i,j+1) in b):
    #                    b.remove((i,j+1))

    #    return ever

    #----------------------------------------------------------
    #  AdjacentColumn
    #
    def AdjacentColumn(self):
        """Fix adjacent elements in the same column"""

        modified = True
        ever = False

        while (modified):
            modified = False

            b = self.n.copy()
            while (b != set()):
                i,j = b.pop()
                if ((i+1,j) in self.n):
                    modified = True
                    ever = True
                    self.n.remove((i,j))
                    self.n.remove((i+1,j))
                    elem = (i,j+1)
                    if (elem not in self.n):
                        self.n.add(elem)
                        b = self.n.copy()
                    else:
                        done = False
                        while (not done):
                            self.n.remove(elem)
                            elem = (elem[0]+1,elem[1])
                            if (elem not in self.n):
                                self.n.add(elem)
                                b = self.n.copy()
                                done = True

        return ever


    #----------------------------------------------------------
    #  AdjacentRow
    #
    def AdjacentRow(self):
        """Fix adjacent elements in the same row"""

        modified = True
        ever = False

        while (modified):
            modified = False

            b = self.n.copy()
            while (b != set()):
                i,j = b.pop()
                if ((i,j+1) in self.n):
                    modified = True
                    ever = True
                    self.n.remove((i,j))
                    self.n.remove((i,j+1))
                    elem = (i+2,j)
                    if (elem not in self.n):
                        self.n.add(elem)
                        b = self.n.copy()
                    else:
                        done = False
                        while (not done):
                            self.n.remove(elem)
                            elem = (elem[0]+1,elem[1])
                            if (elem not in self.n):
                                self.n.add(elem)
                                b = self.n.copy()
                                done = True

        return ever


    #----------------------------------------------------------
    #  Ready
    #
    def Ready(self):
        """Get this number ready for addition"""

        rows = cols = True

        while (rows or cols):
            cols = self.AdjacentColumn()
            rows = self.AdjacentRow()


    #----------------------------------------------------------
    #  Value
    #
    def Value(self):
        """Return the decimal value of the number"""

        m = 0

        for i,j in self.n:
            m += (2**i)*(3**j) 

        return m


    #----------------------------------------------------------
    #  Largest2Integer
    #
    def Largest2Integer(self, x):
        """Return the largest 2-integer (and exponents) <= x"""

        m = [1,0,0]

        a = int(ceil(log(x)/log(2))) + 1
        b = int(ceil(log(x)/log(3))) + 1

        for i in range(a):
            for j in range(b):
                n = [(2**i)*(3**j),i,j]
                if (n[0] > m[0]) and (n[0] <= x):
                    m = n

        return m


    #----------------------------------------------------------
    #  Greedy
    #
    def Greedy(self, x):
        """Fill in the matrix using the greedy algorithm"""

        while (x > 0):
            w,i,j = self.Largest2Integer(x)
            self.n.add((i,j))
            x -= w


    #----------------------------------------------------------
    #  Limits
    #
    def Limits(self):
        """Find the largest exponent for each base"""

        self.a = 0
        self.b = 0

        for i,j in self.n:
            if (i > self.a):
                self.a = i
            if (j > self.b):
                self.b = j


    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, x=0):
        """Create a representation of x"""

        #  Represent the number
        self.n = set()  #  empty set is zero

        if (x > 0):
            self.Greedy(x)    #  fill in with the greedy algorithm
            self.Ready()      #  fix any adjacent cells
            self.Limits()     #  find the largest exponents

        #  For image output
        #self.oneColor = "#ff0000"
        #self.zeroColor= "#333333"
        self.oneColor = "#cccccc"
        self.zeroColor= "#666666"
        self.gridSize = 30
        self.gridOffset = 6


# DBNS.py

