import db
from db import *
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import numpy as np
import scipy.optimize as optimization
import sys
import itertools
import math
def get_vmax():
    return Vline.select().order_by(Vline.vval.desc()).get().vval

def plot_points(X,Y,Z):
    plt.scatter(X,Y)
    plt.show()



def lsq_last(X,Y,Z):
    X = map(math.sqrt,X)
    # Fit a 3rd order, 2d polynomial
    m = polyfit2d(np.array(X),np.array(Y),np.array(Z),order = 5)
    return m

def polyfit2d(x, y, z, order=3):
    ncols = (order + 1)**2
    G = np.zeros((x.size, ncols))
    ij = itertools.product(range(order+1), range(order+1))
    for k, (i,j) in enumerate(ij):
        G[:,k] = x**i * y**j
    m, _, _, _ = np.linalg.lstsq(G, z)
    return m

def polyval2d(x, y, m):
    order = int(np.sqrt(len(m))) - 1
    ij = itertools.product(range(order+1), range(order+1))
    z = np.zeros_like(x)
    for a, (i,j) in zip(m, ij):
        z += a * x**(0.5*i) * y**j
    return z

def plot_lsq_3d(X,Y,Z,m,plot_type = "surface",ax = None,color = "blue"):
    nx, ny = 100, 100
    xx, yy = np.meshgrid(np.linspace(min(X), max(X), nx), 
                         np.linspace(min(Y), max(Y), ny))
    zz = polyval2d(xx, yy, m)


    if ax == None:
        fig = plt.figure(figsize=plt.figaspect(0.5))
        ax = fig.add_subplot(1, 1, 1, projection='3d')

    if plot_type == "contour":
        CS = plt.contour(xx,yy,zz,15,linewidths=0.5,color='k')
    else:
        surf = ax.plot_surface(xx, yy, zz,linewidth=0,color=color)

    return ax
    
    

def plot_3d(X,Y,Z,plot_type = "surface",ax = None,color = "blue"):
    from scipy.interpolate import griddata
    if ax == None:
        fig = plt.figure(figsize=plt.figaspect(0.5))
        ax = fig.add_subplot(1, 1, 1, projection='3d')

    if plot_type == "scatter":
        ax.scatter(X,Y,Z)
        return
    x = np.array(X)
    y = np.array(Y)
    z = np.array(Z)
    xi = np.linspace(x.min(),x.max(),100)
    yi = np.linspace(y.min(),y.max(),100)
    # VERY IMPORTANT, to tell matplotlib how is your data organized
    zi = griddata((x, y), z, (xi[None,:], yi[:,None]), method='linear')
    xig, yig = np.meshgrid(xi, yi)


    if plot_type == "contour":
        CS = plt.contour(xi,yi,zi,15,linewidths=0.5,color='k')
    else:
        surf = ax.plot_surface(xig, yig, zi,linewidth=0,color=color)#,cmap=cm.coolwarm)

    return ax


def plot_2d_func(X,Y,Z,fE = lambda V: 0.4*(V*(V-1)/2),vmin = 0 ,vmax = None,color="b"):
    if vmax == None:
        vmax = get_vmax()
    V= range(vmin,vmax)
    E = [max(0,min(fE(v),v*(v-1)/2)) for v in V]
    XY = zip(X,Y)
    plotx,ploty = [],[]
    for v,e in zip(V,E):
        if v in X:
            xy = filter(lambda i: i[0] == v,XY) #all pairs with v
            bestElower = 0
            bestEup = float("inf")
            for ve in xy:
                if ve[1] >= e and ve[1] < bestEup:
                    bestEup = ve[1]
                elif ve[1] <= e and ve[1] > bestElower:
                    bestElower = ve[1]
            if bestEup == bestElower: #exact solution
                i = XY.index((v,bestEup))
                z = Z[i]
            else:
                i = XY.index((v,bestEup))
                zup = Z[i]
                i = XY.index((v,bestElower))
                zlow = Z[i]
                z = (zup*(bestEup - e)+zlow*(e - bestElower)) / (bestEup-bestElower)
            plotx.append(v)
            ploty.append(z)
            
    plt.plot(plotx,ploty,color=color)


def get_XYZ(type,runtime = False):
    points = Point.select().order_by(Point.V).order_by(Point.E)
    X,Y,Z = [],[],[]
    for P in points:
        V,E = P.V,P.E
        zzz = db.avr_time(P,type)
        if zzz != None:
            X.append(V)
            Y.append(E)
            if runtime:
                Z.append(zzz[1])
            else:
                Z.append(zzz[0])
    return X,Y,Z
    

Xlem,Ylem,Zlem = get_XYZ("lemon")
Xmv,Ymv,Zmv = get_XYZ("libmv")

plot_points(Xmv,Ymv,Zmv)
ax = plot_3d(Xmv,Ymv,Zmv,color="red")
plot_3d(Xlem,Ylem,Zlem,ax=ax,color="yellow")
plt.show()


Xlem,Ylem,Zlem = get_XYZ("lemon",runtime=True)
Xmv,Ymv,Zmv = get_XYZ("libmv",runtime=True)
ax = plot_3d(Xmv,Ymv,Zmv,color="red")
plot_3d(Xlem,Ylem,Zlem,ax=ax,color="yellow")
plt.show()


#print "Done fetching"
#plot_2d_func(Xlem,Ylem,Zlem,lambda v: 10*v,color="yellow")
#plot_2d_func(Xmv,Ymv,Zmv,lambda v: 10*v,color="red")
#plt.show()
#Xlem,Ylem,Zlem = get_XYZ("lemon",runtime=True)
#Xmv,Ymv,Zmv = get_XYZ("libmv",runtime=True) 
#print "Done fetching"
#plot_2d_func(Xlem,Ylem,Zlem,lambda v: 10*v,color="yellow")
#plot_2d_func(Xmv,Ymv,Zmv,lambda v: 10*v,color="red")
#plt.show()

#mlem = lsq_last(Xlem,Ylem,Zlem)
#ax = plot_lsq_3d(Xlem,Ylem,Zlem,mlem,color = "yellow")
