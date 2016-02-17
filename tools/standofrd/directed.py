import networkx as nx
import forms
import sys


data = open(sys.argv[1]).read().split("\n")
data = list(filter(lambda x: "#" not in x,data))
data = map(lambda x: map(int,filter(lambda y: len(y)>0,x.strip().split("\t"))) ,data)
G = nx.Graph()
for e in data:
    if len(e)>1:
        e1,e2 = e[0],e[1]
    G.add_edge(e1,e2)

my = forms.toform_my(G)
open(sys.argv[1]+".my","w").write(my)
dirac = forms.toform_dirac(G)
open(sys.argv[1]+".dirac","w").write(dirac)
