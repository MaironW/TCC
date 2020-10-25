import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

# plot colors
c1 = (0,0.4,1)
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.grid()

fileid = "11pitch"
fileid = "4yawcut"
fileid = "0004yaw"
dados = np.loadtxt(fileid+"anim.dat",unpack=True,delimiter=',')

roll = dados[0]
pitch = dados[1]
yaw = dados[2]
dt = dados[3]
t = np.zeros(len(dt))
for i in range(1,len(dt)):
   t[i]=t[i-1]+dt[i]

lineroll, = ax.plot([],[],color=c1)
linepitch, = ax.plot([],[],color=c2)
lineyaw, = ax.plot([],[],color=c3)
linevert, = ax.plot([],[],color='k')

# circularA
plt.xlim(-2,t[-1]+2)
plt.ylim(-10+min(yaw),max(yaw)+10)

def animate(i):
   # lineroll.set_xdata(t[0:i+1])
   # lineroll.set_ydata(roll[0:i+1])
   
   # linepitch.set_xdata(t[0:i+1])
   # linepitch.set_ydata(pitch[0:i+1])
   
   lineyaw.set_xdata(t[0:i+1])
   lineyaw.set_ydata(yaw[0:i+1])
   
   linevert.set_xdata([t[i],t[i]])
   linevert.set_ydata([min(yaw),max(yaw)])
   
   anim.event_source.interval = dt[i]*1000

   return lineroll,linepitch,lineyaw,linevert,

    
anim = animation.FuncAnimation(fig, animate, frames=len(dt))
#anim.save(fileid+"anim.mp4",writer="ffmpeg",fps=30)
plt.show()
