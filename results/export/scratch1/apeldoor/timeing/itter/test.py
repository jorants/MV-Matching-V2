from time import time
import db
import math
import networkx as nx
import subprocess
from subprocess import PIPE
import sys



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
    lastline = db.Vline.select().order_by(db.Vline.vval.desc()).get()
    vmax = lastline.vval
    estep = int(math.log(db.Point.select().where(db.Point.V == vmax).count()-1,2)+0.5)
    
    def scale_v(vmax,estep):
        vmax *= 2
        db.add_vline(vmax,estep)
        return vmax
        
    def sharpen_v(vmax,estep):
        lines = list(db.Vline.select().order_by(db.Vline.vval))

        for i,x in enumerate(lines[:-1]):
            newv = (x.vval +lines[i+1].vval) / 2
            if x.vval == newv:
                continue #line exists
            db.add_vline(newv,estep)

            

    def sharpen_e(vmax,estep):
        lines = db.Vline.select().order_by(db.Vline.vval)
        for l in lines:
            v = l.vval
            points = list(db.Point.select().where(db.Point.V == v).order_by(db.Point.E))
            for i,x in enumerate(points[:-1]):
                newe = (x.E +points[i+1].E) / 2
                if newe == x.E:
                    continue #point exists
                P = db.Point(V=v,E=newe)
                P.save()


    i = 0
    while True:
        #yield all new points
        for P in db.get_undone_points():
            yield P
            P.done = True
            P.save()
        
        if i%3 == 0:
            vmax = scale_v(vmax,estep)
        elif i%3 == 1:
            sharpen_v(vmax,estep)
        elif i%3 == 2:
            sharpen_e(vmax,estep)
            estep+=1
        i+=1
        


def test(test_functions,point,G = None):
    V,E = point.V,point.E
    print V,E
    if G == None:
        emax = V*(V-1)/2
        if emax == 0:
            return 0
        p = E/float(emax)
        G = nx.fast_gnp_random_graph(V,p)
        
    E = G.number_of_edges()
    form = toform(G)
    for f in test_functions:
        testtype = db.to_test_type(f.__name__)
        time,runtime = f(G)
        r = db.Result(point = point,time = time,runtime=runtime,testType=testtype)
        r.save()




it = graph_space_iter()
for p in it:
    test([lemon,libmv],p)


