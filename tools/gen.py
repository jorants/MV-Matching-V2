

import sys
from run import to_form_new,toform
import networkx as nx
import random

def random_g(n,p):
    return nx.fast_gnp_random_graph(n,p)



V,p = int(sys.argv[1]),float(sys.argv[2])

G = random_g(V,p)
form1 = to_form_new(G)
form2 = toform(G)
open("g_%i.my" % V,"w").write(form1)
open("g_%i.dirac" % V,"w").write(form2)
