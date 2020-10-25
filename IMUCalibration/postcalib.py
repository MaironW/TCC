import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
from matplotlib2tikz import save as tikz_save
from numpy.random import randn

c1 = (0,0.4,1)
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)

arquivo = np.loadtxt("rawdata.dat",unpack='true',delimiter=',')

ax_raw = arquivo[1]
ay_raw = arquivo[0]
az_raw = -arquivo[2]
gx_raw = arquivo[4]
gy_raw = arquivo[3]
gz_raw = -arquivo[5]

ax_scl = ax_raw / 16384
ay_scl = ay_raw / 16384
az_scl = az_raw / 16384
gx_scl = gx_raw / 131
gy_scl = gy_raw / 131
gz_scl = gz_raw / 131

axbias = np.mean(ax_scl[50:150])  
aybias = np.mean(ay_scl[50:150])
azbias = np.mean(az_scl[50:150]) - 1
gxbias = np.mean(gx_scl[50:150])
gybias = np.mean(gy_scl[50:150])
gzbias = np.mean(gz_scl[50:150])
    
ax = ax_scl - axbias
ay = ay_scl - aybias
az = az_scl - azbias
gx = gx_scl - gxbias
gy = gy_scl - gybias
gz = gz_scl - gzbias


MIN = 700
MAX = 950

# ACCELEROMETER

plt.figure()
plt.grid()
plt.plot(ax_raw[MIN:MAX],label='$X$',color=c1)
plt.plot(ay_raw[MIN:MAX],label='$Y$',color=c2)
plt.plot(az_raw[MIN:MAX],label='$Z$',color=c3)
plt.ylabel('Raw Accel Data (LSB)')
plt.xlabel('Samples')
plt.legend()
tikz_save("a_raw.tex")
plt.show()

plt.figure()
plt.grid()
plt.plot(ax_scl[MIN:MAX],label='$X$',color=c1)
plt.plot(ay_scl[MIN:MAX],label='$Y$',color=c2)
plt.plot(az_scl[MIN:MAX],label='$Z$',color=c3)
plt.ylabel('Scaled Accel Data (g)')
plt.xlabel('Samples')
plt.legend()
tikz_save("a_scl.tex")
plt.show()

plt.figure()
plt.grid()
plt.plot(ax[MIN:MAX],label='$X$',color=c1)
plt.plot(ay[MIN:MAX],label='$Y$',color=c2)
plt.plot(az[MIN:MAX],label='$Z$',color=c3)
plt.ylabel('Corrected Accel Data (g)')
plt.xlabel('Samples')
plt.legend()
tikz_save("a_true.tex")
plt.show()

# GYROSCOPE

plt.figure()
plt.grid()
plt.plot(gx_raw[MIN:MAX],label='$X$',color=c1)
plt.plot(gy_raw[MIN:MAX],label='$X$',color=c2)
plt.plot(gz_raw[MIN:MAX],label='$X$',color=c3)
plt.ylabel('Raw Gyro Data (LSB)')
plt.xlabel('Samples')
plt.legend()
tikz_save("g_raw.tex")
plt.show()

plt.figure()
plt.grid()
plt.plot(gx_scl[MIN:MAX],label='$X$',color=c1)
plt.plot(gy_scl[MIN:MAX],label='$X$',color=c2)
plt.plot(gz_scl[MIN:MAX],label='$X$',color=c3)
plt.ylabel('Scaled Gyro Data (°/s)')
plt.xlabel('Samples')
plt.legend()
tikz_save("g_scl.tex")
plt.show()

plt.figure()
plt.grid()
plt.plot(gx[MIN:MAX],label='$X$',color=c1)
plt.plot(gy[MIN:MAX],label='$X$',color=c2)
plt.plot(gz[MIN:MAX],label='$X$',color=c3)
plt.ylabel('Corrected Gyro Data (°/s)')
plt.xlabel('Samples')
plt.legend()
tikz_save("g_true.tex")
plt.show()



plt.show()
