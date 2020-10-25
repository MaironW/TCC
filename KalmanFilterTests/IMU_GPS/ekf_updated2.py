#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 05/01/2020
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from numpy.random import randn
from numpy.linalg import inv

# Measurement Covariance (R)
R_Xvar  = 1.7694       # ± [m] GPS datasheet p.6
R_Yvar  = 1.0274       # ± [m] GPS datasheet p.6
R_vXvar = 0.0532       # ± [m/s] GPS datasheet p.6
R_vYvar = 0.0318       # ± [m/s] GPS datasheet p.6
R_aNvar = 0.003      # ± [m/s^2] IMU calib.
R_aEvar = 0.003      # ± [m/s^2] IMU calib.

t = np.linspace(0,6*np.pi,300)
dt = t[1] - t[0]

# position
x = 2*np.cos(t) # circle
y = 2*np.sin(t) # circle
# x = 2*np.cos(t)    # eight
# y = np.sin(2*t)    # eight

x_gps = x + R_Xvar * randn(*x.shape)
y_gps = y + R_Yvar * randn(*y.shape)
# x_gps = x_gps*0
# y_gps = y_gps*0

# velocity
vx = -2*np.sin(t) # circle
vy = 2*np.cos(t)  # circle
# vx = -2*np.sin(t)  # eight
# vy = 2*np.cos(2*t) # eight

vx_gps = vx + R_vXvar * randn(*vx.shape)
vy_gps = vy + R_vYvar * randn(*vy.shape)
# vx_gps = vx_gps*0
# vy_gps = vy_gps*0

# accel
ax = -2*np.cos(t)   # circle
ay = -2*np.sin(t)   # circle
ax = ax + R_aNvar * randn(*ax.shape)   # circle with noise
ay = ay + R_aEvar * randn(*ay.shape)  # circle with noise
# ax = -2*np.cos(t)   # eight
# ay = -4*np.sin(2*t) #eight

# dynamic matrix
A = np.array([
    [1,dt,dt*dt*0.5],   # position update
    [0,1,dt],           # velocity update
    [0,0,1]])           # accel update

# control matrix
# B = np.array([0.5*dt**2, dt])

# measurement matrix
H = np.array([
    [1,dt,dt*dt*0.5],   # position update
    [0,1,dt],           # velocity update
    [0,0,0]])           # accel update

def KalmanFilter(R_d,R_v,R_a,d,v,a):
    # measurement covariance matrix
    R = np.array([[R_d*R_d,0,0],
                  [0,R_v*R_v,0],
                  [0,0,R_a*R_a]])

    # process noise covariance matrix
    # Q = np.array([[Q_a*Q_a,0],
    #               [0,Q_a*Q_a]])

    # create an observation vector of noisy GPS signals
    Z = np.array([d,v,a]).T

    # control signal
    # u = a

    # matrix dimensions
    nx = R.shape[0]
    ny = R.shape[0]
    nt = Z.shape[0]

    # allocate identity matrix for re-use
    I = np.eye(nx)

    # initial estimative
    x_init = np.array([d[0], v[0], a[0]])
    P_init = 0.1 * np.eye(len(x_init))  # small initial prediction error

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
            x_pred[i] = np.dot(A,x_est[i-1]) #+ B.T*u[i]
            P_pred[i] = np.dot(np.dot(A,P_est[i-1]),A.T) #+ Q

        # estimation stage
        y = Z[i] - np.dot(H,x_pred[i])

        # S = P_pred[i] + R
        # K[i] = np.dot(P_pred[i], np.linalg.inv(S))

        # Kalman gain
        S = np.dot(np.dot(H,P_pred[i]),H.T)+R
        K[i] = np.dot(np.dot(P_pred[i],H.T), inv(S))

        # prediction
        x_est[i] = x_pred[i] + np.dot(K[i],y)
        P_est[i] = np.dot((I - np.dot(K[i],H)),P_pred[i])

    return x_est

XN = KalmanFilter(R_Xvar,R_vXvar,R_aNvar,x_gps,vx_gps,ax)
XE = KalmanFilter(R_Yvar,R_vYvar,R_aEvar,y_gps,vy_gps,ay)

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
plt.plot(ax,color='C0')
plt.plot(ay,color='C1')
plt.plot(XN[:,2],'--',color='C0')
plt.plot(XE[:,2],'--',color='C1')
plt.grid()

plt.figure()
plt.plot(y_gps,x_gps,'o-',color='C1',alpha=0.7)
plt.plot(y,x)
plt.plot(XE[:,0],XN[:,0],'--',color='black')
plt.plot(XE[0,0],XN[0,0],'x',color='b')
plt.plot(XE[-1,0],XN[-1,0],'x',color='r')
plt.grid()
plt.axis('equal')

plt.show()
