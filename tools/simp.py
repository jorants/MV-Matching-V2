import random
import math
import subprocess
from subprocess import PIPE
import networkx as nx


def docmd(cmd,input):
    sp = subprocess.Popen(cmd,stdin = PIPE,stdout = PIPE)    
    return sp.communicate(input = input)[0]



def remove(G,i,k):
    G = G.split("\n")
    l = G[0]
    e = G[1:-2]
    e = e[:i]+e[i+k:]
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

    out = docmd(["./main","-","greedy"],G).strip()
    if out == "CODE":
        raise Exception("LOOP")
    if out.split("\n")[2] == "0":
        raise ValueError("Something wrong")
    return float(out.split("\n")[0]),int(out.split("\n")[1]),out.split("\n")[2] != "0"




def simplefy(fn,k=1):
    data = open(fn).read()
    err = errors(data)
    print err
    if err == False:
        return
    i = 0
    while i < len(data.split("\n"))-3:
        tmp = remove(data,i,k)
        if err == errors(tmp):
            print "Removed:",i
            data = tmp
            fp = open(fn,"w")
            fp.write(data)
            fp.close()
        else:
            print "kept:",i
            i+=k

def simplefy_bin(fn):
    data = open(fn).read()
    err = errors(data)
    print err
    if err == False:
        return
    head = data.split("\n")[0]
    edges = data.split("\n")[1:-2]
    def edges_to_graph(edges):
        return head+"\n"+"\n".join(edges)+"\nENDL\n"
    
    parts = [(0,len(edges))]
             
    def get_parts(egd,parts):
        es = []
        for p in parts:
            es+= egd[p[0]:p[1]]
        return es

    
    try:
        while True:
            current = parts[0]
            if current[1] - current[0] <2:
                break
            after = parts[1:]
            G =  edges_to_graph(get_parts(edges,after))
            if errors(G) == err:
                #we can miss this part
                print "Removed:",current
                parts = after
            else:
                print "kept:",current
                #we should keep it, but split it into two
                half = (current[0]+current[1])/2
                parts = after
                if half>current[0]:
                    parts+= [(current[0],half)]
                if half<current[1]:
                    parts+= [(half,current[1])]
    except KeyboardInterrupt:
        pass
    fp = open(fn,"w")
    fp.write(edges_to_graph(get_parts(edges,after)))
    fp.close()
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
        print "Found!",errors(data)
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
        find_error(10000,10/10000.)
    else:
        if len(sys.argv)>2:
            k = int(sys.argv[2])
        else:
            k=1
        while k>0:
            simplefy(sys.argv[1],k)
            k/=3
            print "------",k,"-------"
        simplefy_nums(sys.argv[1])    
