import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure("Dead Reconing Walking")
ax1 = fig.add_subplot(1,1,1,aspect='equal')

dN = [0]
dE = [0]
vN = 0
vE = 0


def animate(i):
    global vN
    global vE
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split(',')
        print l
        if(len(l)>=3):
            aN = float(l[0][0:4])
            aE = float(l[1][0:4])
            dt = float(l[5])

            vN = vN + aN*dt
            vE = vE + aE*dt

            dN.append(dN[-1]+vN*dt+aN*dt*dt*0.5)
            dE.append(dE[-1]+vE*dt+aE*dt*dt*0.5)

            ax1.clear()
            ax1.grid()
            ax1.plot([0],[0],'+k')
            ax1.plot(dN,dE,color='C0')

        return 0
    except:
        return 0

anim = animation.FuncAnimation(fig, animate, frames=10, interval=10)
plt.show()