#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 19/09/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation

fig = plt.figure()
ax = fig.add_subplot(1,1,1)

aN = 0.0
aE = 0.0
aD = 0.0
dN = 0.0
dE = 0.0
dD = 0.0
vN = 0.0
vE = 0.0
vD = 0.0

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
Q_aNvar = 0.006     # ± [m/s^2] IMU calib.
Q_aEvar = 0.012     # ± [m/s^2] IMU calib.
Q_aDvar = 0.003     # ± [m/s^2] IMU calib.

dt = 0.58

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

#================== KALMAN FILTER INIT

# estimative uncertainty matrix
P = np.array([
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
X = np.zeros([2])

# measurement matrix
Z = np.zeros([2])
Y = np.zeros([2])

# control signal
u = [0]

# iteration zero
X0 = np.ones([2])*0

# ===================================

def LoopKalmanFilter(noise):
    global X0
    global P
    global u

    aN = noise

    # prediction
    X1 = np.dot(A,X0) + np.dot(B,u)
    P1 = np.dot(np.dot(A,P),np.transpose(A)) + Q

    # Kalman Gain
    S = P1 + R
    K = np.dot(P1,np.linalg.inv(S))

    # Measurements
    Z[0] = dN
    Z[1] = vN
    u = np.array([aN])

    Y = Z - X1

    # state update
    X1 = X1 + np.dot(K,Y)

    #uncertainty update
    P1 = np.dot(I-K,P1)

    # state prediction
    X0 = np.copy(X1)
    P = np.copy(P1)
    return X1

X = []
Y = []
# def init():
#     line.set_data([],[])
#     return line,

timetinit = time.time()

def animate(i):
    try:
        linha = ser.readline()
        linha = linha.decode("utf-8")
        imu = linha.split('\t')
        aN = float(imu[0])
        XN = LoopKalmanFilter(aN)
        # X.append(0)
        # Y.append(aN*10)
        Y = XN[0]
        print aN,'\t',XN[0]
        # line.set_data(X,Y)
        ax.clear()
        ax.grid()
        ax.plot(Y,0,'o')
        # return line,
    except:
        a = 0
        # return line,

# XN = KalmanFilterNorth()
# XE = KalmanFilterEast()
# XD = KalmanFilterDown()

# anim = animation.FuncAnimation(fig, animate, init_func=init,
                            #    frames=360, interval=1, blit=True)

anim = animation.FuncAnimation(fig,animate,interval=1)

plt.grid()
plt.show()
#
# plt.figure()
# plt.subplot(3,1,1)
# plt.plot(dN,color='C0')
# plt.plot(dE,color='C1')
# # plt.plot(dD,'g')
# plt.plot(XN[:,0],'--',color='C0')
# plt.plot(XE[:,0],'--',color='C1')
# plt.grid()
#
# plt.subplot(3,1,2)
# plt.plot(vN,color='C0')
# plt.plot(vE,color='C1')
# # plt.plot(vD)
# plt.plot(XN[:,1],'--',color='C0')
# plt.plot(XE[:,1],'--',color='C1')
# plt.grid()
#
# plt.subplot(3,1,3)
# plt.plot(aN)
# plt.plot(aE)
# # plt.plot(aD)
# plt.grid()
#
# plt.figure()
# plt.plot(dE,dN,'-o',color='C1')
# plt.plot(XE[:,0],XN[:,0],'--',color='black')
# plt.plot(XE[-1,0],XN[-1,0],'o',color='black')
# plt.grid()
#
#
# plt.show()


