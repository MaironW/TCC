# estimate temperature of a liquid with fluctuations in its true value

import numpy as np
import matplotlib.pyplot as plt

xn = []     # estimative array
yn = [49.95, 49.967, 50.1, 50.106, 49.992, 49.819, 49.933, 50.007, 50.023, 49.99] # measurements
n = np.linspace(1.0,10.0,10)
realvalue = [49.979, 50.025, 50, 50.003, 49.994, 50.002, 49.999, 50.006, 49.998, 49.991]

r1 = 0.1**2           # measurement uncertainty (standard deviation)
q = 0.0001            # process noise variance

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

