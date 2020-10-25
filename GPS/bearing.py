#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 29/04/2019
# Mairon de Souza Wolniewicz

from numpy import sin
from numpy import cos
from numpy import arctan2 as atan2
from numpy import pi

#import matplotlib.pyplot as plt

# returns the bearing between two coordinates in degrees
def Bearing(lat1,lon1,lat2,lon2):
    lat1 = lat1*pi/180.0
    lon1 = lon1*pi/180.0
    lat2 = lat2*pi/180.0
    lon2 = lon2*pi/180.0
    dlon = lon2-lon1
    x = sin(dlon)*cos(lat2)
    y = cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(dlon)
    B = atan2(x,y) * 180.0/pi
    return (B+360)%360

#latA = 41.466138
#lonA = 15.547839
#latB = 41.467216
#lonB = 15.547025

#B = 360 - Bearing(latA,lonA,latB,lonB)

#d = 137.72647343701317
#y = d*cos(B*pi/180.0)
#x = d*sin(B*pi/180.0)

##plt.plot([lonA,lonB],[latA,latB],'-o')
#plt.plot([lonA*10000-155480,lonB*10000-155480],[latA*10000-414660,latB*10000-414660],'-o')

#plt.plot([0,x],[0,y],'-o')
#plt.grid()
#plt.show()
