import sys

if (not sys.argv[1]):
    print("Provide degree ./make-test.py 15")
    exit(1)

degree = 0

try:
    degree = int(sys.argv[1])
except:
    print("Can't convert to Int: '%s'" % sys.argv[1])
    exit(1)

times = 2**degree
filename = "./bin/%s" % degree
print("Create: %s" % filename)

with open(filename, "w") as file:
    file.write("%d " % times)
    file.write("%d " % times)
    file.write("1 " * times)
    file.write("1 " * times)