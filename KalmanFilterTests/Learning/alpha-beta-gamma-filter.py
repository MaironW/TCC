# one dimensional alpha-beta-gamma filter for estimate airplane range

import numpy as np
import matplotlib.pyplot as plt

xn = []     # estimative array
xp = []     # prediction array
yn = [30160,30365,30890,31050,31785,32215,33130,34510,36010,37265] # measurements
n = np.linspace(1.0,10.0,10)
realvalue = np.ones(len(n))*1010

alpha = 0.5
beta = 0.4
gamma = 0.1
dt = 5             # [s]        timestep

# iteration zero
x0 = 30000         # [m]        range initial guess
v0 = 50            # [m/s]      velocity initial guess
a0 = 0             # [m/s^2]    acceleration initial guess
# prediction
x1 = x0 + dt * v0 + a0 * dt**2 *0.5
v1 = v0 + a0 * dt
a1 = a0
xp.append(x1)

# filter loop
for i in range(len(n)):
    x0 = x1
    v0 = v1
    a0 = a1
    # state update
    x1 = x0 + alpha*(yn[i]-x0)
    v1 = v0 + beta*(yn[i]-x0)/dt
    a1 = a0 + gamma*(yn[i]-x0)/(0.5*dt**2)
    xn.append(x1)

    # prediction
    x0 = x1
    v0 = v1
    a0 = a1
    x1 = x0 + dt * v0 + a0 * dt**2 *0.5
    v1 = v0 + a0 * dt
    a1 = a0
    xp.append(x1)

print xn
print xp

plt.grid()
# plt.plot(n,realvalue,label='Real Value')
plt.plot(n,yn,label='Measurements')
plt.plot(n,xn,label='Estimatives')
plt.plot(n,xp[0:-1],label='Prediction')
plt.legend()
plt.show()

