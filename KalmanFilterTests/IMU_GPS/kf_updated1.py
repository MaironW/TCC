#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 07/08/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from numpy.random import randn

imu = np.loadtxt("imudata_gpssquare.dat",unpack=True)
gps = np.loadtxt("gpsdata_gpssquare.dat",unpack=True)

aN = imu[0]
aE = imu[1]
aD = imu[2]
dN = gps[3]
dE = gps[4]
dD = gps[5]
vN = gps[6]
vE = gps[7]
vD = gps[8]

# Measurement Covariance (R)
# R_Xvar  = 2.5       # ± [m] GPS datasheet p.6
R_Xvar  = 0.1       # ± [m] GPS datasheet p.6
# R_Yvar  = 2.5       # ± [m] GPS datasheet p.6
R_Yvar  = 0.1       # ± [m] GPS datasheet p.6
# R_Zvar  = 2.5        # ± [m] GPS calculated real-measure
# R_Zvar  = 52        # ± [m] GPS calculated real-measure
R_vXvar = 0.1       # ± [m/s] GPS datasheet p.6
R_vYvar = 0.1       # ± [m/s] GPS datasheet p.6
R_vZvar = 0.1       # ± [m/s] GPS datasheet p.6

# Process Noise Covariance (Q)
Q_aNvar = 0.006     # ± [m/s^2] IMU calib.
Q_aEvar = 0.012     # ± [m/s^2] IMU calib.
Q_aDvar = 0.003     # ± [m/s^2] IMU calib.

t = np.linspace(0,2*np.pi,100)
dt = t[1] - t[0]

# position
x = 2*np.cos(t)
y = 2*np.sin(t)
x_gps = x + R_Xvar * randn(*x.shape)
y_gps = y + R_Yvar * randn(*y.shape)

# velocity
vx = -2*np.sin(t)
vy = 2*np.cos(t)
vx_gps = vx + R_vXvar * randn(*vx.shape)
vy_gps = vy + R_vYvar * randn(*vy.shape)

# accel
ax = -2*np.cos(t)
ay = -2*np.sin(t)


def KalmanFilter(R_d,R_v,Q_a,d,v,a):
    # dynamic matrix
    A = np.array([
        [1, dt, 0.5*(dt**2)],
        [0, 1, dt],
        [0, 0, 1],
        ])

    # measurement matrix
    H = np.array([
        [1, 0, 0]
        ])

    # estimative uncertainty matrix
    P0 = np.array([
        [1,0],
        [0,1]])

    # measurement covariance matrix
    R = np.diag(np.array([R_d**2]))

    # process noise covariance matrix
    Q1 = np.array([(dt**3)/6.0, 0.5*(dt**2), dt])
    Q1 = np.expand_dims(Q1, 1)
    Q = np.dot(Q1,np.transpose(Q1))

    # create an observation vector of noisy GPS signals
    observations = np.array([d,v]).T

    # matrix dimensions
    nx = Q.shape[0]
    ny = R.shape[0]
    nt = observations.shape[0]

    # allocate identity matrix for re-use
    Inx = np.eye(nx)

    # initial estimative
    x_init = np.array([d[0], v[0], a[0]])
    P_init = 0.01 * np.eye(len(x_init))  # small initial prediction error

    # allocate result matrices
    x_pred = np.zeros((nt, nx))      # prediction of state vector
    P_pred = np.zeros((nt, nx, nx))  # prediction error covariance matrix
    x_est = np.zeros((nt, nx))       # estimation of state vector
    P_est = np.zeros((nt, nx, nx))   # estimation error covariance matrix
    K = np.zeros((nt, nx, ny))       # Kalman Gain

    # set initial prediction
    x_pred[0] = x_init
    P_pred[0] = P_init

    # for each time-step...
    for i in range(nt):

        # prediction stage
        if i > 0:
            x_pred[i] = np.dot(A,x_est[i-1])
            P_pred[i] = np.dot(np.dot(A,P_est[i-1]),A.T) + Q

        # estimation stage
        y_obs = observations[i]

        K[i] = np.dot(np.dot(P_pred[i],H.T),        np.linalg.inv(np.dot(np.dot(H,P_pred[i]),H.T)+R))

        x_est[i] = x_pred[i] + np.dot(K[i],(y_obs[0] -     np.dot(H,x_pred[i])))
        P_est[i] = np.dot((Inx - np.dot(K[i],H)),P_pred[i])
    return x_est

XN = KalmanFilter(R_Xvar,R_vXvar,Q_aNvar,x_gps,vx_gps,ax)
XE = KalmanFilter(R_Yvar,R_vYvar,Q_aEvar,y_gps,vy_gps,ay)

plt.figure()
plt.subplot(3,1,1)
plt.plot(x_gps,color='C0')
plt.plot(y_gps,color='C1')
plt.plot(XN[:,0],'--',color='C0')
plt.plot(XE[:,0],'--',color='C1')
plt.grid()

plt.subplot(3,1,2)
plt.plot(vx_gps,color='C0')
plt.plot(vy_gps,color='C1')
plt.plot(XN[:,1],'--',color='C0')
plt.plot(XE[:,1],'--',color='C1')
plt.grid()

plt.subplot(3,1,3)
plt.plot(ax)
plt.plot(ay)
plt.grid()

plt.figure()
plt.plot(x,y)
plt.plot(x_gps,y_gps,'o-',color='C1')
plt.plot(XE[:,0],XN[:,0],'--',color='black')
plt.plot(XE[-1,0],XN[-1,0],'o',color='black')
plt.grid()


plt.show()


