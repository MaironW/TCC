import matplotlib.pyplot as plt
import numpy as np

yost = np.loadtxt("yost.dat",unpack='True',delimiter=',')
mpu = np.loadtxt("sensor.dat",unpack='True',delimiter=',')

x_yost = yost[2]
y_yost = -yost[0]
z_yost = -yost[1]

x_mpu = mpu[0]
y_mpu = mpu[1]
z_mpu = mpu[2]

fig = plt.figure('Yost vs MPU9250 - Linear Acceleration')
ax_yost = fig.add_subplot(2,1,1)
ax_yost.plot(x_yost)
ax_yost.plot(y_yost)
#ax_yost.plot(z_yost)
ax_yost.grid()
ax_mpu = fig.add_subplot(2,1,2)
ax_mpu.plot(x_mpu)
ax_mpu.plot(y_mpu)
#ax_mpu.plot(z_mpu)
ax_mpu.grid()
plt.show()

