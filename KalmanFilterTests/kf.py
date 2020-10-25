#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 19/09/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
from numpy.linalg import inv

from covariance import Covariance

class Direction:
    def __init__(self,R_d,R_v,Q_a):
        self.Q = np.array([[Q_a*Q_a,0],
                           [0,Q_a*Q_a]])
        self.R = np.array([[R_d*R_d,0],
                           [0,R_v*R_v]])
        self.x = np.zeros(2)
        self.P = 10.0 * np.eye(2)

    def KalmanFilter(self,d,v,a):
        # observation matrix
        Z = np.array([d,v])

        # control signal
        u = a

        # prediction
        self.x = np.dot(A,self.x.T) + B*u
        self.P = np.dot(np.dot(A,self.P),A.T) + self.Q

        # Kalman gain
        S = np.dot(np.dot(H,self.P),H.T) + self.R
        K = np.dot(np.dot(self.P,H.T), inv(S))

        y = Z - self.x
        # estimative
        self.x = self.x + np.dot(K,y)
        self.P = np.dot((I - np.dot(K,H)), self.P)

        return self.x

fig = plt.figure()
ax = fig.add_subplot(1,2,1)
ax2 = fig.add_subplot(1,2,2)

aN = 0.0
aE = 0.0
aD = 0.0
dN = 0.0
dE = 0.0
dD = 0.0
vN = 0.0
vE = 0.0
vD = 0.0
dt = 0.001
lat = 0.0
lon = 0.0

X_est = []
Y_est = []
Z_est = []
X_gps = []
Y_gps = []
Z_gps = []
lat_gps = []
lon_gps = []

# Measurement Covariance (R)
R_Xvar  = 2.5       # ± [m] GPS datasheet p.6
R_Yvar  = 2.5       # ± [m] GPS datasheet p.6
R_Zvar  = 2.5       # ± [m] GPS calculated real-measure
R_vXvar = 0.1       # ± [m/s] GPS datasheet p.6
R_vYvar = 0.1       # ± [m/s] GPS datasheet p.6
R_vZvar = 0.1       # ± [m/s] GPS datasheet p.6
# R_Xvar  = 5.0       # ± [m]
# R_Yvar  = 5.0       # ± [m]
# R_Zvar  = 5.0       # ± [m]
# R_vXvar = 5.0       # ± [m/s]
# R_vYvar = 5.0       # ± [m/s]
# R_vZvar = 5.0       # ± [m/s]

# Process Noise Covariance (Q)
Q_aNvar = 0.1     # ± [m/s^2] IMU calib.
Q_aEvar = 0.1     # ± [m/s^2] IMU calib.
Q_aDvar = 0.1     # ± [m/s^2] IMU calib.
# Q_aNvar,Q_aEvar,Q_aDvar = Covariance(2);

# identity matrix
I = np.array([
    [1,0],
    [0,1]])

# dynamic matrix
A = np.array([
    [1,dt],   # position update
    [0,1]])   # velocity update

# control matrix
B = np.array([0.5*dt**2, dt])

# measurement matrix
H = np.array([
    [1,0],
    [0,1]])


#================== INIT

North = Direction(R_Xvar, R_vXvar, Q_aNvar)
East = Direction(R_Yvar, R_vYvar, Q_aEvar)
Down = Direction(R_Zvar, R_vZvar, Q_aDvar)


# ================= ANIMATION

def animate(i):
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split('\t')
        if(len(l)==12):
            dN = float(l[0])
            dE = float(l[1])
            dD = float(l[2])
            vN = float(l[3])
            vE = float(l[4])
            vD = float(l[5])
            aN = float(l[6])
            aE = float(l[7])
            aD = float(l[8])
            lat = float(l[9])
            lon = float(l[10])

            Xkf = North.KalmanFilter(dN,vN,aN)
            Ykf = East.KalmanFilter(dE,vE,aE)
            Zkf = Down.KalmanFilter(dD,vD,aD)


            X_est.append(Xkf[0])
            Y_est.append(Ykf[0])
            Z_est.append(Zkf[0])

            X_gps.append(dN)
            Y_gps.append(dE)
            Z_gps.append(dD)

            lat_gps.append(lat)
            lon_gps.append(lon)

            print X_est[-1],'\t',Y_est[-1],'\t',Z_est[-1]

            ax.clear()
            ax.grid()
            ax.plot(Y_est,X_est)
            ax.plot(Y_est[-1],X_est[-1],'o',color='C0')
            ax.plot(Y_gps,X_gps,'--')
            ax.plot(Y_gps[-1],X_gps[-1],'o',color='C1')

            ax2.clear()
            ax2.grid()
            ax2.plot(lon_gps,lat_gps,color='C1')
            ax2.plot(lon_gps[-1],lat_gps[-1],'o',color='C1')

        return 0
    except:
        return 0

anim = animation.FuncAnimation(fig,animate,interval=1)

plt.show()


