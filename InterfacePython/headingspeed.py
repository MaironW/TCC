import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure("Heading and Velocimeter")
ax1 = fig.add_subplot(1,3,1,aspect='equal',projection='polar')
ax2 = fig.add_subplot(1,3,2,aspect='equal',projection = 
'polar')
ax3 = fig.add_subplot(1,3,3,aspect='equal',projection='polar')


ax1.set_rmax(1)
ax1.set_rticks([1])
ax1.set_yticklabels([])
ax1.set_theta_zero_location("N")
ax1.set_title("Compass")


ax2.set_rmax(1)
ax2.set_rticks([1])
ax2.set_yticklabels([])
ax2.set_thetamin(0)
ax2.set_thetamax(180)
ax2.set_thetagrids([0,30,60,90,120,150,180],labels=[60,50,40,30,20,10,0])
ax2.set_title("Absolute Velocimeter")

ax3.set_rmax(1)
ax3.set_rticks([1])
ax3.set_yticklabels([])
ax3.set_theta_zero_location("N")
ax3.set_title("Accel Direction")

line1, = ax1.plot([],[],'m')
line2, = ax2.plot([],[],'m')
line3, = ax3.plot([],[],'m')

def init():
    line1.set_data([],[])
    line2.set_data([],[])
    line3.set_data([],[])
    return line1,line2,line3,

def animate(i):
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split(',')


        yaw = (float(l[5]))*np.pi/180.0
        speed = (float(l[2]))*3.6
        speed = (1-speed/60.0) * np.pi
    
        aN = float(l[3])
        aE = float(l[4])
        ahead = np.arctan2(aN,aE)
    
        line1.set_data([yaw,yaw],[0,1])
        line2.set_data([speed,speed],[0,1])
        line3.set_data([ahead,ahead],[0,1])

        return line1,line2,line3,
    except:
        return line1,line2,line3,

anim = animation.FuncAnimation(fig, animate,init_func=init, frames=10, interval=10)
plt.show()


plt.show()

