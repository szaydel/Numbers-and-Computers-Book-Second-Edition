#
#  file:  DBNS.py
#
#  A pedagogical implementation of a double-base number system.
#
#  RTK, 08-Jun-2016
#  Last update:  09-Jun-2016
#
###############################################################


###############################################################
#  DBNS
#
class DBNS:
    """A base-2,3 double-number system class"""


    #----------------------------------------------------------
    #  __str__
    #
    def __str__(self):
        """Pretty print"""

        m = ""

        for i in range(self.size):
            n = "|"
            for j in range(self.size):
                n += "%d " % self.n[i][j]
            n = n[0:-1] + "|"
            m += n + "\n"

        return m[0:-1]


    #----------------------------------------------------------
    #  __repr__
    #
    def __repr__(self):
        """Pretty print"""

        return self.__str__()


    #----------------------------------------------------------
    #  __add__
    #
    def __add__(self, y):
        """Add a value"""

        z = DBNS(size=self.size)

        for i in range(self.size):
            for j in range(self.size):
                z.n[i][j] = self.n[i][j] + y.n[i][j]

        z.Carry()
        z.Standardize()
        return z


    #----------------------------------------------------------
    #  Value
    #
    def Value(self):
        """Return the decimal value of the number"""

        m = 0
        for i in range(self.size):
            for j in range(self.size):
                m += self.n[i][j]*(2**i)*(3**j)
        return m


    #----------------------------------------------------------
    #  EmptyMatrix
    #
    def EmptyMatrix(self):
        """Return an empty matrix"""

        m = []

        for i in range(self.size):
            m.append([0]*self.size)

        return m


    #----------------------------------------------------------
    #  Largest2Integer
    #
    def Largest2Integer(self, x):
        """Return the largest 2-integer (and exponents) <= x"""

        m = [1,0,0]

        for i in range(self.size):
            for j in range(self.size):
                n = [(2**i)*(3**j),i,j]
                if (n[0] > m[0]) and (n[0] <= x):
                    m = n

        return m


    #----------------------------------------------------------
    #  Greedy
    #
    def Greedy(self, x):
        """Fill in the matrix using the greedy algorithm"""

        if (x > 0):
            w,i,j = self.Largest2Integer(x)
            self.n[i][j] = 1
            x -= w
            self.Greedy(x)
        else:
            return


    #----------------------------------------------------------
    #  FixConsecutiveRows
    #
    def FixConsecutiveRows(self):
        """
        Fix any instances of consecutive elements in the same
        row that are both 1
        """

        for i in range(self.size):
            for j in range(self.size):
                if (self.n[i][j]==1) and (self.n[i][j+1]==1):
                    self.n[i][j] = 0
                    self.n[i][j+1] = 0
                    if (i+2) >= self.size:
                        raise ValueError("Entry unavailable")
                    self.n[i+2][j] += 1
        return


    #----------------------------------------------------------
    #  FixConsecutiveColumns
    #
    def FixConsecutiveColumns(self):
        """
        Fix any instances of consecutive elements in the same
        column that are both 1
        """

        for i in range(self.size):
            for j in range(self.size):
                if (self.n[i][j]==1) and (self.n[i+1][j]==1):
                    self.n[i][j] = 0
                    self.n[i+1][j] = 0
                    if (j+1) >= self.size:
                        raise ValueError("Entry unavailable")
                    self.n[i][j+1] += 1
        return


    #----------------------------------------------------------
    #  Standardize
    #
    def Standardize(self):
        """Use row and column operations to minimize collisions during addition"""

        self.FixConsecutiveRows()
        self.Carry()
        self.FixConsecutiveColumns()
        self.Carry()


    #----------------------------------------------------------
    #  Carry
    #
    def Carry(self):
        """Process 'carries'"""

        for i in range(self.size):
            for j in range(self.size):
                if (self.n[i][j] > 1):
                    self.n[i][j] -= 2
                    if (i+1) >= self.size:
                        raise ValueError("Entry unavailable")
                    self.n[i+1][j] += 1


    #----------------------------------------------------------
    #  __init__
    #
    def __init__(self, x=0, size=5):
        """Create a representation of x"""

        #  Set up for the given size DBN
        self.p2 = []
        self.p3 = []
        self.size = size

        for i in range(size):
            self.p2.append(2**i)
            self.p3.append(3**i)

        self.maxInt = 0
        for i in range(size):
            for j in range(size):
                self.maxInt += self.p2[i]*self.p3[j]

        #  Check if the given number will fit
        if (x < 0) or (x > self.maxInt):
            raise ValueError("Number out of range")

        #  Represent the number
        self.n = self.EmptyMatrix() #  set up a matrix

        if (x > 0):
            self.Greedy(x)      #  fill in with the greedy algorithm
            self.Standardize()  #  set up for addition


# DBNS.py

