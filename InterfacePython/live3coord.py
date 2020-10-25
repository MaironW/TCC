# LIVE 3 to 4 AXIS VALUE PLOTTER
# written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
# last update: 26/12/2019

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
# ax.set_aspect('equal', adjustable='box')

X = np.zeros(50)
Y = np.zeros(50)
Z = np.zeros(50)
W = np.zeros(50)

def shift(a):
    for i in range(len(a)-1):
        a[i]=a[i+1]

def animate(i):
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split(',')
        #print l
        if(len(l)>=3):
            shift(X)
            shift(Y)
            shift(Z)
            X[-1] = float(l[0])
            Y[-1] = float(l[1])
            Z[-1] = float(l[2])

            ax.clear()
            plt.grid()
            ax.plot(X)
            ax.plot(Y)
            ax.plot(Z)

            #if(len(l)>3):
                #shift(W)
                #W[-1] = float(l[3])
                #ax.plot(W)

        return 0
    except:
        return 0

# anim = animation.FuncAnimation(fig, animate, frames=360, interval=1, blit=True, save_count=10)
anim = animation.FuncAnimation(fig, animate)
plt.show()


