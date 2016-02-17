
def toform_my(g):
    res = "%i\n" % (g.number_of_nodes())
    for e in g.edges():
        res+= "%i %i\n" % e
    return res+"END\n"


def toform_dirac(G):
    res = "p edge %i %i\n" % (G.number_of_nodes(),G.number_of_edges())
    for e1,e2 in G.edges_iter():
        res += "e %i %i 1\n" % (e1+1,e2+1)
    return res

