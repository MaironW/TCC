# estimate the height of a building with an imprecise altimeter

import numpy as np
import matplotlib.pyplot as plt

xn = []     # estimative array
yn = [48.54, 47.11, 55.01, 55.15, 49.89, 40.85, 46.72, 50.05, 51.27, 49.95] # measurements
n = np.linspace(1.0,10.0,10)
realvalue = np.ones(len(n))*50

r1 = 25           #     altimeter measurement uncertainty (variance)

# iteration zero
x0 = 60           # [m] initial guess
p0 = 225          #     human estimation error (variance)

# prediction
x1 = x0
p1 = p0

# filter loop
for i in range(len(n)):
    K = p1/float(p1+r1)             # Kalman Gain

    x1 = x0 + K*(yn[i]-x0)          # state update
    p1 = (1-K)*p0                   # uncertainty update

    x0 = x1                         # state prediction
    p0 = p1                         # uncertainty prediction

    xn.append(x1)

plt.grid()
plt.plot(n,realvalue,label='Real Value')
plt.plot(n,yn,label='Measurements')
plt.plot(n,xn,label='Estimatives')
plt.legend()
plt.show()

