import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax = fig.add_subplot(1,1,1)

#ax.set_aspect('equal')

X_est = []
X_gps = []
Y_est = []
Y_gps = []

def animate(i):
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split(',')
        print l
        if(len(l)>=3):
            X_est.append(float(l[0]))
            X_gps.append(float(l[1]))
            Y_est.append(float(l[2]))
            Y_gps.append(float(l[3]))
            
	    ax.clear()
            ax.grid()
            ax.plot([0],[0],'+k')
            ax.plot(X_est,Y_est,color='C0')
            ax.plot(X_gps,Y_gps,color='C1')
            ax.plot(X_est[-1],Y_est[-1],'o',color='C0')
            ax.plot(X_gps[-1],Y_gps[-1],'o',color='C1')
            
            
        return 0
    except:
        return 0

# anim = animation.FuncAnimation(fig, animate, frames=360, interval=1, blit=True, save_count=10)
anim = animation.FuncAnimation(fig, animate, frames=10, interval=10)
plt.show()


