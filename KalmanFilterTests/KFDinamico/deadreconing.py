import numpy as np
import matplotlib.pyplot as plt

fig1 = plt.figure("Meters")
ax1 = fig1.add_subplot(121,aspect='equal')
ax2 = fig1.add_subplot(122,aspect='equal')

trajeto = np.loadtxt("trajeto5.dat",unpack='True',delimiter=',')

dN = trajeto[0]
dE = trajeto[1]

vN = trajeto[2]
vE = trajeto[3]

aN = trajeto[4]
aE = trajeto[5]

aNclean = trajeto[6]
aEclean = trajeto[8]

yaw = trajeto[10]*np.pi/180.0

n = len(aN)

amod = (aN*aN+aE*aE)*0.5
amodclean = (aNclean*aNclean+aEclean*aEclean)*0.5
head = np.arctan2(aN,aE)
headclean = np.arctan2(aNclean,aEclean)

for i in range(n):
    ax1.plot([dE[i],dE[i]+5*amod[i]*np.cos(head[i])],[dN[i],dN[i]+5*amod[i]*np.sin(head[i])],'r')
    ax1.plot([dE[i],dE[i]+amodclean[i]*np.cos(headclean[i])],[dN[i],dN[i]+amodclean[i]*np.sin(headclean[i])],'g')
    ax2.plot([dE[i],dE[i]+5*amod[i]*np.cos(yaw[i])],[dN[i],dN[i]+5*amod[i]*np.sin(yaw[i])],'r')

ax1.plot(dE,dN,'.')
ax2.plot(dE,dN,'.')
ax1.grid()
ax2.grid()

plt.show()
