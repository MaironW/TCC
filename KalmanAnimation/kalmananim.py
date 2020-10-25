import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

# plot colors
c1 = (0,0.4,1)
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)

fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.set_aspect('equal')
ax.grid()

fileid = "rect"
# fileid = "circularA"
# fileid = "circularB"

#fileid = "kfsimclean"
#fileid = "kfsimblackout"
#fileid = "kfsimnoise"
dados = np.loadtxt(fileid+"anim.dat",unpack=True,delimiter=',')

dN = dados[0]
dE = dados[1]
dNest = dados[2]
dEest = dados[3]
dNest_esp = dados[4]
dEest_esp = dados[5]
dt = dados[6]

plt.xlabel('$E$ (m)')
plt.ylabel('$N$ (m)')

lineX, = ax.plot([],[],'.',color=c1)
# lineXest, = ax.plot([],[],color=c2)
lineXestesp, = ax.plot([],[],color=c2)
lineespdot, = ax.plot([],[],'+',color=c2)
# lineestdot, = ax.plot([],[],'x',color=c2)

# rect
plt.xlim(-3.2036429,22.8185269)
plt.ylim(-13.0085245750817,11.7030137416706)

# circularA
# plt.xlim(-6.94299347337826,17.6198257844466)
# plt.ylim(-18.3550512143577,3.9188535015127)

# circularB
# plt.xlim(-3.84605675,18.18449575)
# plt.ylim(-10.1165967,6.5829367)

def animate(i):
   lineX.set_xdata(dE[0:i+1])
   lineX.set_ydata(dN[0:i+1])
   
   # lineXest.set_xdata(dEest[0:i+1])
   # lineXest.set_ydata(dNest[0:i+1])
   
   lineXestesp.set_xdata(dEest_esp[0:i+1])
   lineXestesp.set_ydata(dNest_esp[0:i+1])
   
   lineespdot.set_xdata(dEest_esp[i])
   lineespdot.set_ydata(dNest_esp[i])
   
   # lineestdot.set_xdata(dEest[i])
   # lineestdot.set_ydata(dNest[i])
   
   anim.event_source.interval = dt[i]*1000
   
   return lineX,lineXestesp,lineespdot
   # return lineX,lineXest,lineXestesp,lineestdot,lineespdot,

    
anim = animation.FuncAnimation(fig, animate, frames=len(dN))
#anim.save(fileid+"anim.mp4",writer="ffmpeg",fps=30)
plt.show()
