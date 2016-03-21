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



def todata(line):
    line = line.split(",")
    return int(line[0]),float(line[1]),float(line[2])

datalem = map(todata, open("lemon.res").read().strip().split("\n"))
datamv = map(todata, open("libmv.res").read().strip().split("\n"))

V = [x[0] for x in datalem]
rlem = [x[1] for x in datalem]
rtlem = [x[2] for x in datalem]
rmv = [x[1] for x in datamv]
rtmv = [x[2] for x in datamv]


plt.scatter(V,rlem,color = "yellow")
plt.scatter(V,rtlem,color = "blue")
plt.scatter(V,rmv,color = "green")
plt.scatter(V,rtmv,color = "red")
plt.show()
