#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 25/07/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt

l = np.loadtxt("gpsdata.dat",unpack=True)

lat = l[0]
lon = l[1]
alt = l[2]
dN = l[3]
dE = l[4]
dD = l[5]
vN = l[6]
vE = l[7]
vD = l[8]

plt.plot(dN)
plt.plot(dE)
plt.plot(-dD)
plt.grid()
plt.show()