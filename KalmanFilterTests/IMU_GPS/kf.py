#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 29/07/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt

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
R_Xvar  = 2.5       # ± [m] GPS datasheet p.6
# R_Xvar  = 0.1       # ± [m] GPS datasheet p.6
R_Yvar  = 2.5       # ± [m] GPS datasheet p.6
# R_Yvar  = 0.1       # ± [m] GPS datasheet p.6
R_Zvar  = 2.5        # ± [m] GPS calculated real-measure
# R_Zvar  = 52        # ± [m] GPS calculated real-measure
R_vXvar = 0.1       # ± [m/s] GPS datasheet p.6
R_vYvar = 0.1       # ± [m/s] GPS datasheet p.6
R_vZvar = 0.1       # ± [m/s] GPS datasheet p.6

# Process Noise Covariance (Q)
# Q_Nvar  = 0.003706  # ± [m] GPS calib.
# Q_Evar  = 0.002465  # ± [m] GPS calib.
# Q_Dvar  = 0.003450  # ± [m] GPS calib.
# Q_vNvar = 0.0       # ± [m/s] GPS calib.
# Q_vEvar = 0.000083  # ± [m/s] GPS calib.
# Q_vDvar = 0.000040  # ± [m/s] GPS calib.
Q_aNvar = 0.006     # ± [m/s^2] IMU calib.
Q_aEvar = 0.012     # ± [m/s^2] IMU calib.
Q_aDvar = 0.003     # ± [m/s^2] IMU calib.

dt = 0.58

def KalmanFilterNorth():
    # identity matrix
    I = np.array([
        [1,0],
        [0,1]])

    # dynamic matrix
    A = np.array([
        [1,dt],   # position update
        [0,1]])   # velocity update


    # control matrix
    B = np.array([
        [0.5*dt**2],
        [dt]])

    # measurement matrix
    H = np.array([
        [1,0],
        [0,1]])

    # estimative uncertainty matrix DEVE SER ALTERADA DEPOIS
    P0 = np.array([
        [1,0],
        [0,1]])

    # measurement covariance matrix
    R = np.array([
        [R_Xvar*R_Xvar,0],
        [0,R_vXvar*R_vXvar]])

    # process noise covariance matrix
    Q = np.array([
        [Q_aNvar*Q_aNvar,0],
        [0,Q_aNvar*Q_aNvar]])

    # estimative matrix
    X = np.zeros([len(aN),2])

    # uncertainty matrix
    P = np.zeros([len(aN),2])

    # measurement matrix
    Z = np.zeros([2])
    Y = np.zeros([2])

    # control signal
    u = [0]

    # iteration zero
    X0 = np.ones([2])*0

    # #filter loop
    for i in range(len(aN)):
        # prediction
        X1 = np.dot(A,X0) + np.dot(B,u)
        P1 = np.dot(np.dot(A,P0),np.transpose(A)) + Q

        # Kalman Gain
        S = P1 + R
        K = np.dot(P1,np.linalg.inv(S))

        # Measurements
        Z[0] = dN[i]
        Z[1] = vN[i]
        u[0] = aN[i]

        Y = Z - X1

        # state update
        X1 = X1 + np.dot(K,Y)

        #uncertainty update
        P1 = np.dot(I-K,P1)

        # state prediction
        X0 = np.copy(X1)
        P0 = np.copy(P1)
        X[i] = X1
        P[i,0] = P1[0,0]
        P[i,1] = P1[0,1]
    return X,P

def KalmanFilterEast():
    # identity matrix
    I = np.array([
        [1,0],
        [0,1]])

    # dynamic matrix
    A = np.array([
        [1,dt],   # position update
        [0,1]])   # velocity update


    # control matrix
    B = np.array([
        [0.5*dt**2],
        [dt]])

    # measurement matrix
    H = np.array([
        [1,0],
        [0,1]])

    # estimative uncertainty matrix DEVE SER ALTERADA DEPOIS
    P0 = np.array([
        [1,0],
        [0,1]])

    # measurement covariance matrix
    R = np.array([
        [R_Yvar*R_Yvar,0],
        [0,R_vYvar*R_vYvar]])

    # process noise covariance matrix
    Q = np.array([
        [Q_aEvar*Q_aEvar,0],
        [0,Q_aEvar*Q_aEvar]])

    # estimative matrix
    X = np.zeros([len(aE),2])

    # uncertainty matrix
    P = np.zeros([len(aE),2])

    # measurement matrix
    Z = np.zeros([2])
    Y = np.zeros([2])

    # control signal
    u = [0]

    # iteration zero
    X0 = np.ones([2])*0

    # #filter loop
    for i in range(len(aE)):
        # prediction
        X1 = np.dot(A,X0) + np.dot(B,u)
        P1 = np.dot(np.dot(A,P0),np.transpose(A)) + Q

        # Kalman Gain
        S = P1 + R
        K = np.dot(P1,np.linalg.inv(S))

        # Measurements
        Z[0] = dE[i]
        Z[1] = vE[i]
        u[0] = aE[i]

        Y = Z - X1

        # state update
        X1 = X1 + np.dot(K,Y)

        #uncertainty update
        P1 = np.dot(I-K,P1)

        # state prediction
        X0 = np.copy(X1)
        P0 = np.copy(P1)
        X[i] = X1
        P[i,0] = P1[0,0]
        P[i,1] = P1[0,1]
    return X,P

def KalmanFilterDown():
    # identity matrix
    I = np.array([
        [1,0],
        [0,1]])

    # dynamic matrix
    A = np.array([
        [1,dt],   # position update
        [0,1]])   # velocity update


    # control matrix
    B = np.array([
        [0.5*dt**2],
        [dt]])

    # measurement matrix
    H = np.array([
        [1,0],
        [0,1]])

    # estimative uncertainty matrix DEVE SER ALTERADA DEPOIS
    P0 = np.array([
        [1,0],
        [0,1]])

    # measurement covariance matrix
    R = np.array([
        [R_Zvar*R_Zvar,0],
        [0,R_vZvar*R_vZvar]])

    # process noise covariance matrix
    Q = np.array([
        [Q_aDvar*Q_aDvar,0],
        [0,Q_aDvar*Q_aDvar]])

    # estimative matrix
    X = np.zeros([len(aD),2])

    # uncertainty matrix
    P = np.zeros([len(aD),2])

    # measurement matrix
    Z = np.zeros([2])
    Y = np.zeros([2])

    # control signal
    u = [0]

    # iteration zero
    X0 = np.ones([2])*0

    # #filter loop
    for i in range(len(aD)):
        # prediction
        X1 = np.dot(A,X0) + np.dot(B,u)
        P1 = np.dot(np.dot(A,P0),np.transpose(A)) + Q

        # Kalman Gain
        S = P1 + R
        K = np.dot(P1,np.linalg.inv(S))

        # Measurements
        Z[0] = dD[i]
        Z[1] = vD[i]
        u[0] = aD[i]

        Y = Z - X1

        # state update
        X1 = X1 + np.dot(K,Y)

        #uncertainty update
        P1 = np.dot(I-K,P1)

        # state prediction
        X0 = np.copy(X1)
        P0 = np.copy(P1)
        X[i] = X1
        P[i,0] = P1[0,0]
        P[i,1] = P1[0,1]
    return X,P

XN,PN = KalmanFilterNorth()
XE,PE = KalmanFilterEast()
XD,PD = KalmanFilterDown()

plt.figure()
plt.subplot(3,1,1)
plt.plot(dN,color='C0')
plt.plot(dE,color='C1')
# plt.plot(dD,'g')
plt.plot(XN[:,0],'--',color='C0')
plt.plot(XE[:,0],'--',color='C1')
plt.grid()

plt.subplot(3,1,2)
plt.plot(vN,color='C0')
plt.plot(vE,color='C1')
# plt.plot(vD)
plt.plot(XN[:,1],'--',color='C0')
plt.plot(XE[:,1],'--',color='C1')
plt.grid()

plt.subplot(3,1,3)
plt.plot(aN)
plt.plot(aE)
# plt.plot(aD)
plt.grid()

plt.figure()
plt.plot(dE,dN,'-o',color='C1')
plt.plot(XE[:,0],XN[:,0],'--',color='black')
plt.plot(XE[-1,0],XN[-1,0],'o',color='black')
plt.grid()


plt.show()


