#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 28/12/2019
# Mairon de Souza Wolniewicz

# realiza filtro de kalman em 1 dimensao em tempo real
# recebe como entrada um arquivo no padrao
# [a d v]

import numpy as np
from numpy.linalg import inv
import matplotlib.pyplot as plt
from matplotlib import animation

d = []
v = []
a = []

fig = plt.figure()
axd = fig.add_subplot(3,1,1)
axv = fig.add_subplot(3,1,2)
axa = fig.add_subplot(3,1,3)

dt = 0.021

# Measurement Covariance (R)
Rd = 2.5       # ± [m] GPS datasheet p.6
Rv = 0.1       # ± [m/s] GPS datasheet p.6

# Process Noise Covariance (Q)
Qa = 0.006     # ± [m/s^2] IMU calib.

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
    [Rd*Rd,0],
    [0,Rv*Rv]])

# process noise covariance matrix
Q = np.array([
    [Qa*Qa,0],
    [0,Qa*Qa]])

# matrix dimensions
nx = Q.shape[0]
ny = R.shape[0]

I = np.eye(nx)

#inital estimative
x_pred = np.array([0,0])
P_pred = P

x_est = np.copy(x_pred)
P_est = np.copy(P_pred)

K = np.zeros([nx,ny])

X_EST = []
V_EST = []

def animate(i):
    global x_pred, P_pred, x_est, P_est, X_EST, V_EST
    # try:
    linha = open('MPU.dat','r').read()
    l = linha.split(',')

    a.append(float(l[0]))
    d.append(float(l[1]))
    v.append(float(l[2]))

    Z = np.array([d[-1],v[-1]])
    u = a[-1]

    if i>0:
        x_pred = np.dot(A,x_est) + B.T*u
        P_pred = np.dot(np.dot(A,P_est),A.T) + Q

    y = Z - x_pred

    # Kalman gain
    S = np.dot(np.dot(H,P_pred),H.T)+R
    K = np.dot(np.dot(P_pred,H.T), inv(S))

    # prediction
    x_est = x_pred + np.dot(K,y)
    P_est = np.dot((I - np.dot(K,H)),P_pred)

    X_EST.append(x_est[0])
    V_EST.append(x_est[1])

    axd.clear()
    axv.clear()
    axa.clear()

    axd.grid()
    axv.grid()
    axa.grid()

    axd.plot(d)
    axd.plot(X_EST)
    axv.plot(v)
    axv.plot(V_EST)
    axa.plot(a)


    return 0
    # except:
    #     return 0


# ==========================

anim = animation.FuncAnimation(fig, animate, frames=10, interval=10)


plt.show()









