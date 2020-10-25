# LIVE 3 AXIS VALUE PLOTTER
# written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
# last update: 17/09/2019

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
# ax.set_aspect('equal', adjustable='box')

X = np.zeros(100)
Y = np.zeros(100)
Z = np.zeros(100)
# X = []
# Y = []
# Z = []

def shift(a):
    for i in range(len(a)-1):
        a[i]=a[i+1]

def animate(i):
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split('\t')
        print l

        if(len(l)==4):
            shift(X)
            shift(Y)
            shift(Z)
            X[-1] = float(l[0])
            Y[-1] = float(l[1])
            Z[-1] = float(l[2])
            # X.append(float(l[0]))
            # Y.append(float(l[1]))
            # Z.append(float(l[2]))

            ax.clear()
            plt.grid()
            ax.plot(X,label='$X$')
            ax.plot(Y,label='$Y$')
            ax.plot(Z,label='$Z$')

            plt.ylabel('Scaled Data')
            plt.xlabel('Samples')
            plt.legend()
        return 0
    except:
        return 0

anim = animation.FuncAnimation(fig, animate, frames=10, interval=1)

plt.show()


