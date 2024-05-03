#  Find the factors leading to 1 0 0 0 sets

def main():
    b0 = 233
    b1 = 239
    b2 = 241
    b3 = 251

    #b0 = 2   # 105
    #b1 = 3   #  70
    #b2 = 5   # 126
    #b3 = 7   # 120

    b = b0*b1*b2*b3

    i = 0

    while (i < b):
        n = (i % b0, i % b1, i % b2, i % b3)
        if (n == (1,0,0,0)):
            print "%3d -> (1,0,0,0)" % i
        elif (n == (0,1,0,0)):
            print "%3d -> (0,1,0,0)" % i
        elif (n == (0,0,1,0)):
            print "%3d -> (0,0,1,0)" % i
        elif (n == (0,0,0,1)):
            print "%3d -> (0,0,0,1)" % i
        i += 1


main()

