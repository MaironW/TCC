import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure(num='Linear Kalman Filter Written in C++')
axd = fig.add_subplot(3,1,1)
axv = fig.add_subplot(3,1,2)
axa = fig.add_subplot(3,1,3)

dados = np.loadtxt("sensor.dat",unpack=True,delimiter=',')

a = dados[0]
d = dados[1]
v = dados[2]

x_est = np.loadtxt("filtered.dat",unpack='True',delimiter=',')

# ==========================

axd.plot(d)
axd.plot(x_est[0])

axv.plot(v)
axv.plot(x_est[1])

axa.plot(a)

axd.grid()
axv.grid()
axa.grid()
plt.show()
