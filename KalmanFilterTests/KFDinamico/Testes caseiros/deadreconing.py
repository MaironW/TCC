import numpy as np
import matplotlib.pyplot as plt

fig1 = plt.figure("Dead Reconing")
ax1 = fig1.add_subplot(121,aspect='equal')
ax2 = fig1.add_subplot(122,aspect='equal')

trajeto = np.loadtxt("sid3.dat",unpack='True',delimiter=',')

dN = trajeto[0]
dE = trajeto[1]

vN = trajeto[2]
vE = trajeto[3]

aN = trajeto[4]
aE = trajeto[5]

aXlin = trajeto[6]
aYlin = trajeto[7]

yaw = trajeto[10]*np.pi/180.0

n = len(aN)

amod = (aN*aN+aE*aE)**0.5

MULTI = 5

ax1.plot(dE,dN,'.k')
ax2.plot(dE,dN,'.k')
ax1.grid()
ax2.grid()


for i in range(n):
    
    ax1.plot([dE[i],dE[i]+MULTI*aE[i]],[dN[i],dN[i]+MULTI*aN[i]],'b')

    
    ax2.plot([dE[i],dE[i]+MULTI*amod[i]*np.cos(yaw[i])],[dN[i],dN[i]+MULTI*amod[i]*np.sin(yaw[i])],'b')


plt.show()
