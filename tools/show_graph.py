import random
import math
import subprocess
from subprocess import PIPE
import networkx as nx
import sys

def docmd(cmd,input):
    sp = subprocess.Popen(cmd,stdin = PIPE,stdout = PIPE)    
    return sp.communicate(input = input)[0]

def to_dot(G):
    res = "graph g{\n"
    for line in G.strip().split("\n")[1:-1]:
        n1,n2 = line.split(" ")[0],line.split(" ")[1]
        res+= " %s -- %s;\n" % (n1,n2)
    return res+ "}"

data = open(sys.argv[1]).read()
dot =  to_dot(data)
png = docmd(["dot","-Tpng"],dot)
docmd(["display"],png)
