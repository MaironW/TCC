import numpy as np
import matplotlib.pyplot as plt

fig1 = plt.figure()
ax1 = fig1.add_subplot(111,aspect='equal')

trajeto = np.loadtxt("trajeto3.dat",delimiter=',',unpack='true')

dN = trajeto[0]
dE = trajeto[1]

ax1.plot(dE,dN,'.')


POS1 = 168
POS2 = 225
n = POS2-POS1
ax1.plot(dE[POS1],dN[POS1],'k.')
ax1.plot(dE[POS2],dN[POS2],'r.')

Nintp = np.linspace(dN[POS1],dN[POS2],n)
Eintp = np.linspace(dE[POS1],dE[POS2],n)

ax1.plot(Eintp,Nintp)

Nvar = sum((dN[POS1:POS2] - Nintp)**2)/float(n-1)
Evar = sum((dE[POS1:POS2] - Eintp)**2)/float(n-1)

print Nvar,Evar

ax1.grid()
plt.show()
