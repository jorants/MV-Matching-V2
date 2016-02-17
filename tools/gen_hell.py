import sys
import itertools

n = int(sys.argv[1])
s1 = range(0,n)
s2 = range(n,2*n)
s3 = range(2*n,3*n)
s4 = range(3*n,4*n)

fp = open("hell_"+sys.argv[1]+".my","w")
fp.write("%i\n" % (4*n))
for n1,n2 in (zip(s1,s2)):
    fp.write("%i %i\n"%(n1,n2))

for n1,n2 in (itertools.product(s1,s3)):
    fp.write("%i %i\n"%(n1,n2))

for n1,n2 in (itertools.product(s2,s4)):
    fp.write("%i %i\n"%(n1,n2))

fp.write("ENDL\n")
fp.close()



fp = open("hell_"+sys.argv[1]+".dirac","w")
fp.write("p edge %i %i\n" % (4*n,2*n*n+n))
for n1,n2 in (zip(s1,s2)):
    fp.write("e %i %i 1\n"%(n1+1,n2+1))

for n1,n2 in (itertools.product(s1,s3)):
    fp.write("e %i %i 1\n"%(n1+1,n2+1))

for n1,n2 in (itertools.product(s2,s4)):
    fp.write("e %i %i 1\n"%(n1+1,n2+1))

fp.close()
