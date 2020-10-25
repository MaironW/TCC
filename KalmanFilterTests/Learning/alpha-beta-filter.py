# one dimensional alpha-beta filter for estimate airplane range

import numpy as np
import matplotlib.pyplot as plt

xn = []     # estimative array
xp = []     # prediction array
yn = [30110,30265,30740,30750,31135,31015,31180,31610,31960,31865] # measurements
n = np.linspace(1.0,10.0,10)
realvalue = np.ones(len(n))*1010

alpha = 0.2
beta = 0.1
dt = 5             # [s] timestep

# iteration zero
x0 = 30000         # [m]    range initial guess
v0 = 40            # [m/s]  velocity initial guess
# prediction
x1 = x0 + dt * v0
v1 = v0
xp.append(x1)

# filter loop
for i in range(len(n)):
    x0 = x1
    v0 = v1
    # state update
    x1 = x0 + alpha*(yn[i]-x0)
    v1 = v0 + beta*(yn[i]-x0)/dt
    xn.append(x1)

    # prediction
    x0 = x1
    v0 = v1
    x1 = x0 + dt * v0
    v1 = v0
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

