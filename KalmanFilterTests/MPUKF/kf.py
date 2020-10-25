#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Simple Kalman filter to remove rotor noise 
# on accelerometer and gyro measurements
# 20/05/2020
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from numpy.random import randn
from numpy.linalg import inv
from matplotlib2tikz import save as tikz_save

# plot colors
c1 = (0,0.4,1)  
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)

# load measurement file
ax,ay,az,gx,gy,gz = np.loadtxt("accelteste.dat",unpack='true',delimiter=',')

# Measurement Covariance (R)
avar = np.std(ax[0:-1])
gvar = np.std(gx[0:-1])
print avar
print gvar
avar  = 0.32          # ± [m/s2]
#gvar  = 0.036          # ± [rad/s]

#accelation
#ax = np.zeros(1000)
#for i in range(500,650):
    #ax[i] = ax[i]+3
    #gx[i] = gx[i]+0.5
#for i in range(500,750):
    #ax[i] = ax[i]-(i-750)/100.0
#for i in range(750,850):
    #ax[i] = ax[i]+2
#ax[250:500]=np.ones(250)
        
# apply noise
#axnoise = axvar
#axn = ax #+ axnoise * randn(*ax.shape)

# ========= KALMAN FILTER


def KalmanFilter(Ra,Rg,ax,ay,az,gx,gy,gz):
    # measurement covariance matrix
    R = np.array([[Ra*Ra,0,0,0,0,0],
                  [0,Ra*Ra,0,0,0,0],
                  [0,0,Ra*Ra,0,0,0],
                  [0,0,0,Rg*Rg,0,0],
                  [0,0,0,0,Rg*Rg,0],
                  [0,0,0,0,0,Rg*Rg]
                  ])
    
    # process noise covariance matrix
    qa = 0.01
    qg = 0.001
    Q = np.array([[qa**2,0,0,0,0,0],
                [0,qa**2,0,0,0,0],
                [0,0,qa**2,0,0,0],
                [0,0,0,qg**2,0,0],
                [0,0,0,0,qg**2,0],
                [0,0,0,0,0,qg**2]])

    
    # create an observation vector of noisy GPS signals
    Z = np.array([ax,ay,az,gx,gy,gz]).T

    # matrix dimensions
    nx = R.shape[0]
    ny = R.shape[0]
    nt = Z.shape[0]
    
    # allocate identity matrix for re-use
    I = np.eye(nx)

    # allocate result matrices
    x = np.zeros((nt, nx))      # prediction of state vector
    P = np.zeros((nt, nx, nx))  # prediction error covariance matrix
    K = np.zeros((nt, nx, ny))  # Kalman Gain
    
    # set initial prediction
    x[0] = np.array([ax[0],ay[0],az[0],gx[0],gy[0],gz[0]])
    P[0] = 0.1 * np.eye(nx)

    # for each time-step...
    for i in range(nt):
        
        # prediction stage
        if i > 0:            
            x[i] = x[i-1]
            P[i] = P[i-1] + Q

        # estimation stage
        y = Z[i] - x[i]

        # Kalman gain
        S = P[i]+R
        K[i] = np.dot(P[i], inv(S))

        # prediction
        x[i] = x[i] + np.dot(K[i],y)
        P[i] = np.dot((I - K[i]),P[i])
        #P_est[i] = np.dot(np.dot(I-K[i],P_pred[i]),(I-K[i]).T) + np.dot(np.dot(K[i],R),K[i].T)
        
    return x

X = KalmanFilter(avar,gvar,ax,ay,az,gx,gy,gz)
axest = X[:,0]
ayest = X[:,1]
azest = X[:,2]
gxest = X[:,3]
gyest = X[:,4]
gzest = X[:,5]

#axest = KalmanFilter(avar,ax)
#ayest = KalmanFilter(avar,ay)
#azest = KalmanFilter(avar,az)
        
#gxest = KalmanFilter(gvar,gx)
#gyest = KalmanFilter(gvar,gy)
#gzest = KalmanFilter(gvar,gz)


for i in range(len(axest)):        
    print "%.6f,%.6f" %(axest[i],ayest[i])        
        
# Plota os graficos

plt.figure("Accel")
plt.subplot(3,1,1)
plt.plot(ax,color=c1)
plt.plot(axest,color=c2)
plt.grid()
plt.subplot(3,1,2)
plt.plot(ay,color=c1)
plt.plot(ayest,color=c2)
plt.grid()
plt.subplot(3,1,3)
plt.plot(az,color=c1)
plt.plot(azest,color=c2)
plt.grid()

plt.figure("Gyro")
plt.subplot(3,1,1)
plt.plot(gx,color=c1)
plt.plot(gxest,color=c2)
plt.grid()
plt.subplot(3,1,2)
plt.plot(gy,color=c1)
plt.plot(gyest,color=c2)
plt.grid()
plt.subplot(3,1,3)
plt.plot(gz,color=c1)
plt.plot(gzest,color=c2)
plt.grid()

#tikz_save("noiselinear.tex")
#plt.show()

#plt.figure("Erro")
#plt.plot(abs(Xx[:,0]-ax),color=c1)
#plt.plot(abs(Xx2[:,0]-ax),color=c2)
#plt.grid() 

#tikz_save("noiseerror.tex")
#plt.show()

plt.show()
