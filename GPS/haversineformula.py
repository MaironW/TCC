#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 29/04/2019
# Mairon de Souza Wolniewicz

from numpy import cos
from numpy import sin
from numpy import arcsin as asin
from numpy import pi

R = 6371000.0 # [m]

def hav(t):
    return 0.5 - cos(t)*0.5

# returns the distance between two points given its coordinates in vetor form [lat,lon]
def HaversineFormula(lat1,lon1,lat2,lon2):
    lat1 = lat1*pi/180.0
    lon1 = lon1*pi/180.0
    lat2 = lat2*pi/180.0
    lon2 = lon2*pi/180.0
    dlat = lat2-lat1
    dlon = lon2-lon1
    h = hav(dlat) + cos(lat1)*cos(lat2)*hav(dlon)
    d = 2*R*asin(h**0.5)
    return d

# latA = -0.457881868*180/np.pi
# lonA = -0.457881868*180/np.pi
# latB = -0.85318476*180/np.pi
# lonB = -0.853184402*180/np.pi
#
# print hav(latB-latA)
# print hav(lonB-lonA)
#
# print HaversineFormula(latA,lonA,latB,lonB)
