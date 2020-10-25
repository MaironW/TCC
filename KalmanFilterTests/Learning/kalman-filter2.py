# estimate temperature of a heating liquid

import numpy as np
import matplotlib.pyplot as plt

xn = []     # estimative array
yn = [50.45, 50.967, 51.6, 52.106, 52.492, 52.819, 53.433, 54.007, 54.523, 54.99] # measurements
n = np.linspace(1.0,10.0,10)
realvalue = [50.479, 51.025, 51.5, 52.003, 52.494, 53.002, 53.499, 54.006, 54.498, 54.991]

r1 = 0.1**2       # measurement uncertainty (standard deviation)
q = 0.15          # process noise variance

# iteration zero
x0 = 10           # [degC]  initial guess
p0 = 100**2       #         human estimation error (standard deviation)

# prediction
x1 = x0
p1 = p0 + q

# filter loop
for i in range(len(n)):
    K = p1/float(p1+r1)             # Kalman Gain

    x1 = x0 + K*(yn[i]-x0)          # state update
    p1 = (1-K)*p0                   # uncertainty update

    x0 = x1                         # state prediction
    p0 = p1 + q                     # uncertainty prediction

    xn.append(x1)

plt.grid()
plt.plot(n,realvalue,label='Real Value')
plt.plot(n,yn,label='Measurements')
plt.plot(n,xn,label='Estimatives')
plt.legend()
plt.show()

