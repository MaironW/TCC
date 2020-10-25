#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 28/12/2019
# Mairon de Souza Wolniewicz

# realiza filtro de kalman em 1 dimensao
# recebe como entrada um arquivo no padrao
# [a d v]

import numpy as np
from numpy.linalg import inv
import matplotlib.pyplot as plt

fig = plt.figure(num='Linear Kalman Filter')
axd = fig.add_subplot(3,1,1)
axv = fig.add_subplot(3,1,2)
axa = fig.add_subplot(3,1,3)

dados = np.loadtxt("sensordouble.dat",unpack=True,delimiter=',')

d = dados[0]
d = d - np.mean(d)
v = dados[1]
a = dados[2]
aclean = dados[3]

#j = np.linspace(0,len(a)-1,len(a))
#for i in range(len(j)/2):
    #aclean[i+len(j)/2]=aclean[i]-(1e-3)*j[i]
    #d[len(j)-i-1]=d[len(j)-i-1]-(1e-3)*j[i]


#media_a = np.mean(a)
#a = a - media_a

dt = 0.2

# Measurement Covariance (R)
Rd = 1.7694       # ± [m] GPS datasheet p.6
Rv = 0.0532       # ± [m/s] GPS datasheet p.6

# Process Noise Covariance (Q)
Qa = 0.0030     # ± [m/s^2] IMU calib.

# dynamic matrix
A = np.array([
    [1, dt, dt*dt*0.5],
    [0, 1, dt],
    [0, 0, 1]
    ])

# measurement matrix
H = np.array([[1, 0, 0],
              [0, 1, 0],
              [0, 0, 1]])

#================== KALMAN FILTER INIT

# estimative uncertainty matrix
P = np.array([
    [Rd*Rd,0,0],
    [0,Rv*Rv,0],
    [0,0,Qa*Qa]])

# measurement covariance matrix
R = np.array([[Rd*Rd,0,0],
                [0,Rv*Rv,0],
                [0,0,Qa*Qa]])

# process noise covariance matrix
Q = np.array([[1,0,0],
              [0,1,0],
              [0,0,1]])*Qa*Qa

# measurements
Z = np.array([d,v,aclean]).T

# matrix dimensions
nx = Q.shape[0]
ny = R.shape[0]
nt = Z.shape[0]

I = np.eye(nx)

#inital estimative
x_init = np.array([d[0],v[0],aclean[0]])
P_init = P

x_pred = np.zeros((nt,nx))
P_pred = np.zeros((nt,nx,nx))

x_est = np.zeros((nt,nx))
P_est = np.zeros((nt,nx,nx))

K = np.zeros((nt,nx,ny))

x_pred[0] = x_init
P_pred[0] = P_init

for i in range(nt):
    if i>0:
        x_pred[i] = np.dot(A,x_est[i-1]) #+ B.T*u[i]
        P_pred[i] = np.dot(np.dot(A,P_est[i-1]),A.T) + Q

    y = Z[i] - np.dot(H,x_pred[i])

    # Kalman gain
    S = np.dot(np.dot(H,P_pred[i]),H.T) + R
    K[i] = np.dot(np.dot(P_pred[i],H.T), inv(S))

    # prediction
    x_est[i] = x_pred[i] + np.dot(K[i],y)
    P_est[i] = np.dot((I - np.dot(K[i],H)),P_pred[i])


# ==========================

axd.plot(d)
axd.plot(x_est[:,0])

axv.plot(v)
axv.plot(x_est[:,1])

axa.plot(a)
axa.plot(aclean)


axd.grid()
axv.grid()
axa.grid()
plt.show()









