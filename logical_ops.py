def pp(z):
    print "{0:08b}".format(z & 0xff)

def main():
    z = 189 & 222;  pp(z)
    z = 189 | 222;  pp(z)
    z = 189 ^ 222;  pp(z)
    z = z ^ 222;    pp(z)
    z = ~z;         pp(z)

main()

