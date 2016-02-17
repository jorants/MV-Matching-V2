import itertools
import networkx as nx
import random
import subprocess
from subprocess import PIPE
import sys
from time import time

def random_bi(n1,n2,p):
    g = nx.Graph()
    for i in range(n1+n2):
        g.add_node(i)
    for i in range(n1):
        for j in range(n2):
            if random.random()<p:
                g.add_edge(i,n1+j)
    return g


def random_g(n,p):
    return nx.fast_gnp_random_graph(n,p)


"""
creates the union of two graphs K_n,n.
Then stitches those together on one side
"""
def hell(n):
    g = nx.Graph()
    s1 = range(n)
    s2 = range(n,2*n)
    s3 = range(2*n,3*n)
    s4 = range(3*n,4*n)
    g.add_nodes_from(s1+s2+s3+s4)
    g.add_edges_from(zip(s1,s2))
    g.add_edges_from(itertools.product(s1,s3))
    g.add_edges_from(itertools.product(s2,s4))
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
        G = toform(G).strip()
    try:
        t0 = time()
        out = docmd(["./match","-s","-m"],G).strip()
        t1 = time()
        td = t1-t0
        count = int(out.split("\n")[-1].split(":")[1].strip())
        t = float(out.split("\n")[3].split(":")[1].strip())
    except Exception as e:
        import simp
        simp.simp_data(G)
        sys.exit()
    return t,count,td/10.


def lemon(G,inform = False):
    if not inform:
        G = toform(G)

    t0 = time()
    out = docmd(["./match","-s","-l"],G).strip()
    t1 = time()
    td = t1-t0
        
    count = int(out.split("\n")[-1].split(":")[1].strip())
    t = float(out.split("\n")[3].split(":")[1].strip())
    return t,count,td/10.


def lemon_greedy(G,inform = False):
    if not inform:
        G = toform(G)

    t0 = time()
    out = docmd(["./match","-s","-l","-a"],G).strip()
    t1 = time()
    td = t1-t0
        
    count = int(out.split("\n")[-1].split(":")[1].strip())
    t = float(out.split("\n")[3].split(":")[1].strip())
    return t,count,td/10.


def libmv_greedy(G,inform = False):
    if not inform:
        G = toform(G)
        
    out = docmd(["./match","-L","-s","-m"],G).strip()
    count = int(out.split("\n")[-1].split(":")[1].strip())
    t = float(out.split("\n")[3].split(":")[1].strip())
    return t,count


def mvjoran(G,inform = False):
    if not inform:
        G = to_form_new(G)

    out = docmd(["./main","-"],G).strip()
    try:
        return float(out.split("\n")[0]),int(out.split("\n")[1])    
    except Exception as e:
        import simp
        simp.simp_data(G)
        sys.exit()



def mvjoran_greedy(G,inform = False):
    if not inform:
        G = to_form_new(G)
        
    t0 = time()
    out = docmd(["./main","-","greedy"],G)
    t1 = time()
    td = t1-t0


    try:
        return float(out.split("\n")[0]),int(out.split("\n")[1]),td/10.
    except Exception as e:
        print e
        fp = open("error.bu","w")
        fp.write(G)
        fp.close()
        sys.exit()
        
def plot_bi(x,p = lambda i: 10./i):
    lem = []
    mv = []
    mvg = []
    jor = []
    jorg = []
    for i in x:
        print i
        G = random_bi(i,i,p(i))
        res = [lemon(G),libmv(G),libmv_greedy(G),mvjoran(G),mvjoran_greedy(G)]
        lem.append(res[0][0])
        mv.append(res[1][0])
        mvg.append(res[2][0])
        jor.append(res[3][0])
        jorg.append(res[4][0])
        counts =  map(lambda x: x[1],res)
        print counts
        if len(set(counts))>1:
            print counts
    
    from matplotlib import pyplot as plt
    plt.plot(x,lem,color="y")
    plt.plot(x,jor,color="b")
    plt.plot(x,mv,color="r")
    plt.plot(x,jorg,color="g")
    plt.plot(x,mvg,color="k")
    plt.show()



X = range(10,3000,100)
#plot(X,lambda i: 0.1)
#plot_bi(X)


def plot(x,p = lambda i: 10./i):
    lem = []
    mv = []
    mvg = []
    jor = []
    jorg = []
    for i in x:
        print i
        G = random_g(i,p(i))
        res = [lemon(G),libmv(G),libmv_greedy(G),mvjoran(G),mvjoran_greedy(G)]
        lem.append(res[0][0])
        mv.append(res[1][0])
        mvg.append(res[2][0])
        jor.append(res[3][0])
        jorg.append(res[4][0])
        counts =  map(lambda x: x[1],res)
        print counts
        if len(set(counts))>1:
            print counts
    
    from matplotlib import pyplot as plt
    plt.plot(x,lem,color="y")
    plt.plot(x,jor,color="b")
    plt.plot(x,mv,color="r")
    plt.plot(x,jorg,color="g")
    plt.plot(x,mvg,color="k")
    plt.show()


def plot_two(x,p = lambda i: 10./i):
    lem = []
    jorg = []
    lemT = []
    jorgT = []

    for i in x:
        print i
        G = random_g(i,p(i))
        res = [lemon(G),mvjoran_greedy(G)]
        lem.append(res[0][0])
        jorg.append(res[1][0])
        lemT.append(res[0][2])
        jorgT.append(res[1][2])
        print res[1]
        counts =  map(lambda x: x[1],res)
        print counts
        if len(set(counts))>1:
            print counts
    
    from matplotlib import pyplot as plt
    fig, ax1 = plt.subplots()
    
    ax1.plot(x,lem,color="y")
    ax1.plot(x,jorg,color="g")
    ax1.plot(x,lemT,color="r")
    ax1.plot(x,jorgT,color="b")
    plt.savefig("plot.png")


def plot_hell(x):
    lem = []
    jorg = []
    lemT = []
    jorgT = []

    for i in x:
        print i
        G = hell(i)
        res = [lemon_greedy(G),mvjoran_greedy(G)]
        lem.append(res[0][0])
        jorg.append(res[1][0])
        lemT.append(res[0][2])
        jorgT.append(res[1][2])
        print res[1]
        counts =  map(lambda x: x[1],res)
        print counts
        if len(set(counts))>1:
            print counts
    
    from matplotlib import pyplot as plt
    fig, ax1 = plt.subplots()
    
    ax1.plot(x,lem,color="y")
    ax1.plot(x,jorg,color="g")
    
    ax1.plot(x,lemT,color="r")
    ax1.plot(x,jorgT,color="b")
    plt.savefig("plot.png")

if __name__ == "__main__":
    X = range(10,1000,10)
    plot_two(X,lambda i: 0.1)
