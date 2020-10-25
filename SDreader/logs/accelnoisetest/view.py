import matplotlib.pyplot as plt
import numpy as np

x,y,z = np.loadtxt("log0007.dat",unpack='true',delimiter=',')

plt.grid()
plt.plot(x)
plt.plot(y)
plt.plot(z)
plt.show()
