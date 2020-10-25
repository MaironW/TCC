#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 07/08/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from numpy.random import randn
from numpy.linalg import inv

R_aNvar = 0.1      # ± [m/s^2] IMU calib.
R_aEvar = 0.1      # ± [m/s^2] IMU calib.

R_gNvar = 0.1      # ± [m/s^2] IMU calib.
R_gEvar = 0.1      # ± [m/s^2] IMU calib.

Q_aNvar = 0.05      # ± [m/s^2] IMU calib.
Q_aEvar = 0.05      # ± [m/s^2] IMU calib.

t = np.linspace(0,2*np.pi,100)
dt = t[1] - t[0]

# accel
axclean = -2*np.cos(t)   # circle
ayclean = -2*np.sin(t)   # circle
ax = axclean + R_aNvar * randn(*axclean.shape)   # circle with noise
ay = ayclean + R_aEvar * randn(*ayclean.shape)  # circle with noise

gN = ax-1
gE = ay

# dynamic matrix
A = np.array([
    [1,-1],
    [0,1]])

# measurement matrix
H = np.array([
    [1,0],
    [0,1]])

def KalmanFilter(R_a,R_g,Q_a,a,g):
    # measurement covariance matrix
    R = np.array([[R_a*R_a,0],
                  [0,R_g*R_g]])

    # process noise covariance matrix
    Q = np.array([[Q_a*Q_a,0],
                  [0,Q_a*Q_a]])

    # create an observation vector of noisy GPS signals
    Z = np.array([a,g]).T

    # matrix dimensions
    nx = Q.shape[0]
    ny = R.shape[0]
    nt = Z.shape[0]

    # allocate identity matrix for re-use
    I = np.eye(nx)

    # initial estimative
    x_init = np.array([a[0],g[0]])
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
            x_pred[i] = np.dot(A,x_est[i-1])
            P_pred[i] = np.dot(np.dot(A,P_est[i-1]),A.T) + Q

        # estimation stage
        y = Z[i] - np.dot(H,x_pred[i])

        # Kalman gain
        S = np.dot(np.dot(H,P_pred[i]),H.T)+R
        K[i] = np.dot(np.dot(P_pred[i],H.T), inv(S))

        # prediction
        x_est[i] = x_pred[i] + np.dot(K[i],y)
        P_est[i] = np.dot((I - np.dot(K[i],H)),P_pred[i])

    return x_est

XN = KalmanFilter(R_aNvar,R_gNvar,Q_aNvar,ax,gN)
XE = KalmanFilter(R_aEvar,R_gEvar,Q_aEvar,ay,gE)

plt.figure()
plt.subplot(2,1,1)
plt.plot(axclean,'k')
plt.plot(ax,'.',color='C0')
plt.plot(XN[:,0],color='C0')
plt.plot(ayclean,'k')
plt.plot(ay,'.',color='C1')
plt.plot(XE[:,0],color='C1')
plt.grid()

plt.subplot(2,1,2)
plt.plot(gN,'.',color='C0')
plt.plot(XN[:,1],color='C0')
plt.plot(gE,'.',color='C1')
plt.plot(XE[:,1],color='C1')
plt.grid()

#plt.figure()
#plt.plot(y_gps,x_gps,'o-',color='C1',alpha=0.7)
#plt.plot(y,x)
#plt.plot(XE[:,0],XN[:,0],'--',color='black')
#plt.plot(XE[0,0],XN[0,0],'x',color='b')
#plt.plot(XE[-1,0],XN[-1,0],'x',color='r')
#plt.grid()
#plt.axis('equal')

plt.show()
