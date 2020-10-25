#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 27/05/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from bearing import Bearing
from haversineformula import HaversineFormula

lat,lon,distance,dE,dN = np.loadtxt("gps_distance0.csv",unpack='True')

x = [0]
y = [0]
N = [0]
E = [0]

xc = [0]
yc = [0]

k=0

for i in range(1,len(lat)):

    E.append(E[-1] + dE[i])
    N.append(N[-1] + dN[i])

    B = Bearing(lat[i-1],lon[i-1],lat[i],lon[i])*np.pi/180.0
    d = HaversineFormula(lat[i-1],lon[i-1],lat[i],lon[i])
    print d,distance[i]
    if(d):
        xc.append(xc[-1] + d*np.sin(B))
        yc.append(yc[-1] + d*np.cos(B))
        k+=1;

plt.figure()
plt.subplot(1,2,1)
plt.grid()
plt.plot(E,N,'o-')
plt.plot(xc,yc,'ro-')
plt.plot(0,0,'Xk')

plt.subplot(1,2,2)
plt.grid()
plt.plot(lon,lat,'o-')
plt.plot(lon[0],lat[0],'Xk')

plt.show()
