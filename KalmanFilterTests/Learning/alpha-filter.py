# one dimensional alpha filter for estimate the real weight of a gold bar

import numpy as np
import matplotlib.pyplot as plt

xn = []     # estimative array
yn = [1030,989,1017,1009,1013,979,1008,1042,1012,1011] # measurements
n = np.linspace(1.0,10.0,10)
realvalue = np.ones(len(n))*1010

# iteration zero
x0 = 1000         # initial guess
x1 = x0           # prediction (static model)

# filter loop
for i in range(len(n)):
    alpha = 1/float(i+1)            # gain update
    x1 = x0 + alpha*(yn[i]-x0)      # state update
    x0 = x1                         # prediction
    xn.append(x1)

plt.grid()
plt.plot(n,realvalue,label='Real Value')
plt.plot(n,yn,label='Measurements')
plt.plot(n,xn,label='Estimatives')
plt.legend()
plt.show()

