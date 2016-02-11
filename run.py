import networkx as nx
import random
import subprocess
from subprocess import PIPE
import sys
def random_bi(n1,n2,p):
    g = nx.Graph()
    for i in range(n1+n2):
        g.add_node(i)
    for i in range(n1):
        for j in range(n2):
            if random.random()<p:
                g.add_edge(i,n1+j)
    return g

def to_form_new(g):
    res = "%i\n" % (g.number_of_nodes())
    for e in g.edges():
        res+= "%i %i\n" % e
    return res+"END\n"



def docmd(cmd,input):
    sp = subprocess.Popen(cmd,stdin = PIPE,stdout = PIPE)    
    return sp.communicate(input = input)[0]


def toform(G):
    res = "p edge %i %i\n" % (G.number_of_nodes(),G.number_of_edges())
    for e1,e2 in G.edges_iter():
        res += "e %i %i 1\n" % (e1+1,e2+1)
    return res


def libmv(G,inform = False):
    if not inform:
        G = toform(G)
        
    out = docmd(["./match","-s","-m"],G)
    return float(out.split("\n")[3].split(":")[1].strip())


def lemon(G,inform = False):
    if not inform:
        G = toform(G)
        
    out = docmd(["./match","-s","-l"],G)
    return float(out.split("\n")[3].split(":")[1].strip())


def libmv_greedy(G,inform = False):
    if not inform:
        G = toform(G)
        
    out = docmd(["./match","-L","-s","-m"],G)
    return float(out.split("\n")[3].split(":")[1].strip())


def mvjoran(G,inform = False):
    if not inform:
        G = to_form_new(G)

    out = docmd(["./main"],G)
    return float(out)

def mvjoran_greedy(G,inform = False):
    if not inform:
        G = to_form_new(G)
    try:
        out = docmd(["./main","greedy"],G)
        return float(out)
    except Exception:
        open("graph","w").write(G)
        exit()

x = range(5,500,5)
lem = []
mv = []
mvg = []
jor = []
jorg = []
for i in x:
    print i
    G = random_bi(i,i,10./i)
    lem.append(lemon(G))
    mv.append(libmv(G))
    mvg.append(libmv_greedy(G))
    jor.append(mvjoran(G))
    jorg.append(mvjoran_greedy(G))
    
from matplotlib import pyplot as plt
plt.plot(x,lem,color="y")
plt.plot(x,jor,color="b")
plt.plot(x,mv,color="r")
plt.plot(x,jorg,color="g")
plt.plot(x,mvg,color="k")
    
plt.show()


