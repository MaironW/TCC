import numpy as np
import matplotlib.pyplot as plt

fig1 = plt.figure("Meters")
ax1 = fig1.add_subplot(111,aspect='equal')

#fig2 = plt.figure("Coordinates")
#ax2 = fig2.add_subplot(111,aspect='equal')

trajeto = np.loadtxt("trajeto3.dat",unpack='True',delimiter=',')

dN = trajeto[0]
dE = trajeto[1]

dN_est = trajeto[6]
dE_est = trajeto[7]

lat = trajeto[8]
lon = trajeto[9]

ax1.plot(dE,dN,'.')
# ax1.plot(dE_est,dN_est)
ax1.grid()

#ax2.plot(lon,lat)
#ax2.grid()

plt.show()
