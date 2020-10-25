# LIVE GPS PLOTTER
# Compara posicao em coordenadas geograficas com conversao em metros
# written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
# last update: 28/12/2019

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax = fig.add_subplot(2,1,1)
ax2 = fig.add_subplot(2,1,2)
# ax.set_aspect('equal', adjustable='box')

Xdeg = np.zeros(50)
Xm = np.zeros(50)

def shift(a):
    for i in range(len(a)-1):
        a[i]=a[i+1]

def animate(i):
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split(',')
        print l
        if(len(l)>=2):
            shift(Xdeg)
            shift(Xm)
            Xdeg[-1] = float(l[0])
            Xm[-1] = float(l[3])

	    ax.clear()
	    ax2.clear()
            #ax.grid()
            #ax2.grid()
            ax.plot(Xdeg,color='C0')
            ax2.plot(Xm,color='C1')
            
        return 0
    except:
        return 0

# anim = animation.FuncAnimation(fig, animate, frames=360, interval=1, blit=True, save_count=10)
anim = animation.FuncAnimation(fig, animate)
plt.show()


