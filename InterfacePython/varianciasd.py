import numpy as np
import matplotlib.pyplot as plt

dados = np.loadtxt("gpsvar1.dat",unpack=True,delimiter=',')

dN = dados[0]
dE = dados[1]
vN = dados[2]
vE = dados[3]
aN = dados[4]
aE = dados[5]
#aNs = dados[6]
#aEs = dados[7]

dN_var = np.std(dN)
dE_var = np.std(dE)
vN_var = np.std(vN)
vE_var = np.std(vE)
aN_var = np.std(aN)
aE_var = np.std(aE)
#aNs_var = np.std(aNs)
#aEs_var = np.std(aEs)

print "%.2f,%.2f" %(dN_var,dE_var)
print "%.2f,%.2f" %(vN_var,vE_var)
print "%.2f,%.2f" %(aN_var,aE_var)
#print "%.2f,%.2f" %(aNs_var,aEs_var)

plt.figure("Displacement")
plt.grid()
plt.plot(dN)
plt.plot(dE)

plt.figure("Velocity")
plt.grid()
plt.plot(vN)
plt.plot(vE)

plt.figure("Acceleration N")
plt.grid()
plt.plot(aN,color='C0',alpha=0.5)
#plt.plot(aNs,':',color='C0')

plt.figure("Acceleration E")
plt.grid()
plt.plot(aE,color='C1',alpha=0.5)
#plt.plot(aEs,':',color='C1')

plt.figure("2D")
plt.grid()
plt.plot(dE,dN,'o:')

plt.show()
