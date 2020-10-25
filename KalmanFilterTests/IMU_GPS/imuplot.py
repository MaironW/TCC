#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 25/07/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt

l = np.loadtxt("imudata.dat",unpack=True)

aN = l[0]
aE = l[1]
aD = l[2]

print len(aN)

plt.plot(aN)
plt.plot(aE)
plt.plot(aD)
plt.grid()
plt.show()