from time import time

import math
import networkx as nx
import subprocess
from subprocess import PIPE
import sys
import random
import itertools



def docmd(cmd,input):
    sp = subprocess.Popen(cmd,stdin = PIPE,stdout = PIPE)    
    return sp.communicate(input = input)[0]

def toform(G):
    
    res = "p edge %i %i\n" % (G.number_of_nodes(),G.number_of_edges())
    for e1,e2 in G.edges_iter():

        res += "e %i %i 1\n" % (e1+1,e2+1)
    return res


def to_form_new(g):
    res = "%i %i\n" % (g.number_of_nodes(),g.number_of_edges())
    for e in g.edges():
        res+= "%i %i\n" % e
    return res+"END\n"



def lemon(G,inform = False):
    if not inform:
        G = toform(G)

    t0 = time()
    out = docmd(["./match","-s","-l","-a"],G).strip()
    t1 = time()
    td = t1-t0
    t = float(out.split("\n")[3].split(":")[1].strip())
    return t,td


def libmv(G,inform = False):
    if not inform:
        G = to_form_new(G)
        
    t0 = time()
    out = docmd(["./main","-","greedy"],G)
    t1 = time()
    td = t1-t0


    try:
        return float(out.split("\n")[0]),td
    except Exception as e:
        print e
        fp = open("error.bu","w")
        fp.write(G)
        fp.close()
        sys.exit()


all_tests = [lemon,libmv]

    
def graph_space_iter():
    i = 20
    n = 0
    while n<1000000:
        G = nx.barabasi_albert_graph(i,10)
        G = nx.convert_node_labels_to_integers(G)
        n = G.number_of_edges()
        i*=2
        yield G.number_of_nodes(),G
    
def test(test_functions,V,G):
    print V,G.number_of_edges()
    for f in test_functions:
        time,runtime = f(G)
        fp = open(f.__name__+".res","a")
        fp.write("%i,%f,%f\n" % (V,time,runtime))


it = graph_space_iter()
for VG in it:
    test([lemon,libmv],VG[0],VG[1])


