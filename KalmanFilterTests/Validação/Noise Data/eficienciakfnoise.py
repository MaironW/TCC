import numpy as np
import matplotlib.pyplot as plt 

dNgps = np.loadtxt("posNgps.dat",unpack=True)
dEgps = np.loadtxt("posEgps.dat",unpack=True)
dNest = np.loadtxt("posNest.dat",unpack=True)
dEest = np.loadtxt("posEest.dat",unpack=True)
vNgps = np.loadtxt("velNgps.dat",unpack=True)
vEgps = np.loadtxt("velEgps.dat",unpack=True)
vNest = np.loadtxt("velNest.dat",unpack=True)
vEest = np.loadtxt("velEest.dat",unpack=True)

dXgps = (dNgps[1]**2 + dEgps[1]**2)**0.5
dXest = (dNest[1]**2 + dEest[1]**2)**0.5
vXgps = (vNgps[1]**2 + vEgps[1]**2)**0.5
vXest = (vNest[1]**2 + vEest[1]**2)**0.5

dt = 0.04
npoints = len(dNgps[0])

ierro = 0
testerro = dXgps-dXest
for i in range(len(testerro)):
    if(testerro[i]>=0):
       ierro+=1
dterro = ierro*dt
print "posicao"
print dterro
print ierro/float(npoints)

ierro = 0
testerro = vXgps-vXest
for i in range(len(testerro)):
    if(testerro[i]>=0):
       ierro+=1
dterro = ierro*dt
print "velocidade"
print dterro
print ierro/float(npoints)

plt.figure()
plt.grid()
plt.plot(dNgps[0],dXgps,'k',alpha=0.5)
plt.plot(dNest[0],dXest,'k')

plt.figure()
plt.grid()
plt.plot(vNgps[0],vXgps,'k',alpha=0.5)
plt.plot(vNest[0],vXest,'k')

plt.show()