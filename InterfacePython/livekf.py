# written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
# last update: 07/01/2020

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
axd = fig.add_subplot(1,1,1)
#axv = fig.add_subplot(3,1,2)
#axa = fig.add_subplot(3,1,3)

d_est = np.zeros(50)
d = np.zeros(50)
v_est = np.zeros(50)
v = np.zeros(50)
a = np.zeros(50)


def shift(a):
    for i in range(len(a)-1):
        a[i]=a[i+1]

def animate(i):
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split(',')
        print l
        if(len(l)>=3):
            shift(d_est)
            shift(d)
            #shift(v_est)
            #shift(v)
            #shift(a)
            d_est[-1] = float(l[0])
            d[-1] = float(l[1])
            #v_est[-1] = float(l[2])
            #v[-1] = float(l[3])
            #a[-1] = float(l[4])

	    axd.clear()
	    #axv.clear()
	    #axa.clear()
            plt.grid()
            axd.plot(d_est)
            axd.plot(d)
            #axv.plot(v_est)
            #axv.plot(v)
            #axa.plot(a)
            
        return 0
    except:
        return 0

# anim = animation.FuncAnimation(fig, animate, frames=360, interval=1, blit=True, save_count=10)
anim = animation.FuncAnimation(fig, animate, frames=10, interval=10)
plt.show()


