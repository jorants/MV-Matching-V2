import random
import math
import subprocess
from subprocess import PIPE
import networkx as nx


def docmd(cmd,input):
    sp = subprocess.Popen(cmd,stdin = PIPE,stdout = PIPE)    
    return sp.communicate(input = input)[0]



def remove(G,i):
    G = G.split("\n")
    l = G[0]
    e = G[1:-2]
    e = e[:i]+e[i+1:]
    return l +"\n"+ "\n".join(e) + "\nEND\n"


def errors(G):
    try:
        mvjoran(G,True)
        return False
    except Exception as e:
        return str(e)



def mvjoran(G,inform = False):
    if not inform:
        G = to_form_new(G)

    out = docmd(["./main","-"],G).strip()

    if out.split("\n")[2] == "0":
        raise ValueError("Something wrong")
    return float(out.split("\n")[0]),int(out.split("\n")[1]),out.split("\n")[2] != "0"




def simplefy(fn):
    data = open(fn).read()
    err = errors(data)
    print err
    if err == False:
        return
    i = 0
    while i < len(data.split("\n"))-3:
        tmp = remove(data,i)
        if err == errors(tmp):
            print "Removed:",i
            data = tmp
            fp = open(fn,"w")
            fp.write(data)
            fp.close()
        else:
            i+=1

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

def to_form_new(g):
    res = "%i\n" % (g.number_of_nodes())
    for e in g.edges():
        res+= "%i %i\n" % e
    return res+"END\n"


def find_error(n,p):
    i = 0
    while True:
        print i
        i+=1
        g = random_g(n,p)
        simp(g)
        
def simp(g):
    data = to_form_new(g)
    simp_data(data)
    
def simp_data(data):
    if errors(data):
        print "Found!"
        fp = open("errorfile","w")
        fp.write(data)
        fp.close()
        fp = open("errorfile.bu","w")
        fp.write(data)
        fp.close()

        simplefy("errorfile")
        simplefy("errorfile")
        simplefy("errorfile")
        simplefy("errorfile")
        simplefy("errorfile")
        simplefy("errorfile")
        simplefy("errorfile")
        simplefy("errorfile")
        simplefy_nums("errorfile")
        


def simplefy_nums(fn):
    data = open(fn).read()
    err = errors(data)    
    print err
    i = 0
    lines = data.split("\n")[1:-2]
    lines = map(lambda x: map(int,x.split(" ")),lines)
    numbers = list(set(reduce(lambda x,y: x+y,lines,[])))
    numbers.sort()
    newlines = "%i\n" % (len(numbers) +2)
    for l in lines:
        newlines += ("%i %i\n" % (numbers.index(l[0]),numbers.index(l[1])))
    newlines+= "END\n"

    if errors(newlines):
        fp = open(fn,"w")
        fp.write(newlines)
        fp.close()



if __name__ == "__main__":
    import sys
    
    if len(sys.argv)<2:
        find_error(1000,10./500.)
    else:
        simplefy(sys.argv[1])
        simplefy(sys.argv[1])
        simplefy(sys.argv[1])
        simplefy(sys.argv[1])
        simplefy(sys.argv[1])
        simplefy(sys.argv[1])
        simplefy(sys.argv[1])
        simplefy(sys.argv[1])
        simplefy_nums(sys.argv[1])

    
