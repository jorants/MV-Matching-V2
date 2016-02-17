import sys

data = open(sys.argv[1]).read().strip().split("\n")
V = int(data[0])
E = len(data)-1
data = data[1:]

def line(l):
    l = l.split(" ")
    l = map(int,l)
    l = map(lambda x: x+1,l)
    return "e %i %i 1\n" % (l[0],l[1])

data = map(line,data)

fp = open(sys.argv[2],"w")

fp.write("p edge %i %i\n" % (V,E))
for l in data:
    fp.write(l)
fp.close()
