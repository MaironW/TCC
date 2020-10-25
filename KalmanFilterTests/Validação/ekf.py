#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 13/02/2020
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from numpy.random import randn
from numpy.linalg import inv

# Measurement Covariance (R)
R_Xvar  = 1.7694       # ± [m]
R_Yvar  = 1.0274       # ± [m]
R_vXvar = 0.0532       # ± [m/s]
R_vYvar = 0.0318       # ± [m/s]

# Process Noise Covariance (Q)
R_aNvar = 0.003      # ± [m/s^2]
R_aEvar = 0.003      # ± [m/s^2]

n = 1 # number of cycles

dt = 0.02 # [s] time step
t = np.arange(0,n*2*np.pi,dt)
npoints = len(t) 

# choose between (1) cicle or (2) eight
path = 2

# size of the trajectory 
pathsize = 10   # [m]

if(path==1):
    # position
    x = pathsize*np.cos(t) # circle
    y = pathsize*np.sin(t) # circle
    # velocity
    vx = -pathsize*np.sin(t) # circle
    vy = pathsize*np.cos(t)  # circle
    # accelation
    ax = -pathsize*np.cos(t)   # circle
    ay = -pathsize*np.sin(t)   # circle
elif(path==2):
    # position
    x = pathsize*np.cos(t)    # eight
    y = pathsize*np.sin(2*t)    # eight
    # velocity
    vx = -pathsize*np.sin(t)  # eight
    vy = 2*pathsize*np.cos(2*t) # eight
    # accelation
    ax = -pathsize*np.cos(t)   # eight
    ay = -4*pathsize*np.sin(2*t) #eight    
    
        

# apply noise if desired
dXnoise = R_Xvar
dYnoise = R_Yvar
vXnoise = R_vXvar
vYnoise = R_vYvar
aXnoise = R_aNvar*1000
aYnoise = R_aEvar*1000

dXnoise = 0
dYnoise = 0
vXnoise = 0
vYnoise = 0
aXnoise = 0
aYnoise = 0


x_gps = x + dXnoise * randn(*x.shape)
y_gps = y + dYnoise * randn(*y.shape)

vx_gps = vx + vXnoise * randn(*vx.shape)
vy_gps = vy + vYnoise * randn(*vy.shape)

ax = ax + aXnoise * randn(*ax.shape)   
ay = ay + aYnoise * randn(*ay.shape)


# simulate GPS delay
gps_dt = 0.1 # [s]
waiting_time = int(gps_dt/dt)
x_new = np.zeros(npoints)
y_new = np.zeros(npoints)
vx_new = np.zeros(npoints)
vy_new = np.zeros(npoints)
for i in range(0,npoints,waiting_time):
    x_new[i:i+waiting_time]=x_gps[i]
    y_new[i:i+waiting_time]=y_gps[i]
    vx_new[i:i+waiting_time]=vx_gps[i]
    vy_new[i:i+waiting_time]=vy_gps[i]
x_gps = np.copy(x_new)
y_gps = np.copy(y_new)
vx_gps = np.copy(vx_new)
vy_gps = np.copy(vy_new)


# simulate invalid GPS data for a while
START = 0*waiting_time #[s]
STOP = 1*waiting_time # [s]

x_gps[START:STOP] = x_gps[START]
y_gps[START:STOP] = y_gps[START]

vx_gps[START:STOP] = vx_gps[START]
vy_gps[START:STOP] = vy_gps[START]

# ====== EXTENDED KALMAN FILTER

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
    [0,0,1]])           # accel update

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

        # Kalman gain
        S = np.dot(np.dot(H,P_pred[i]),H.T)+R
        K[i] = np.dot(np.dot(P_pred[i],H.T), inv(S))

        # prediction
        x_est[i] = x_pred[i] + np.dot(K[i],y)
        P_est[i] = np.dot((I - np.dot(K[i],H)),P_pred[i])

    return x_est

XN = KalmanFilter(R_Xvar,R_vXvar,R_aNvar,x_gps,vx_gps,ax)
XE = KalmanFilter(R_Yvar,R_vYvar,R_aEvar,y_gps,vy_gps,ay)

plt.figure("Linear")
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

plt.figure("Erro")
plt.subplot(3,1,1)
plt.plot(abs(x_gps-x),color='C0',alpha=0.7)
plt.plot(abs(y_gps-y),color='C1',alpha=0.7)
plt.plot(abs(XN[:,0]-x),color='C0')
plt.plot(abs(XE[:,0]-y),color='C1')
plt.grid()

plt.subplot(3,1,2)
plt.plot(abs(x_gps-vx),color='C0',alpha=0.7)
plt.plot(abs(y_gps-vy),color='C1',alpha=0.7)
plt.plot(abs(XN[:,1]-vx),color='C0')
plt.plot(abs(XE[:,1]-vy),color='C1')
plt.grid()

plt.subplot(3,1,3)
plt.plot(abs(XN[:,2]-ax),color='C0')
plt.plot(abs(XE[:,2]-ay),color='C1')
plt.grid()

plt.figure("Trajetoria")
plt.plot(y_gps,x_gps,'o-',color='C1',alpha=0.7)
plt.plot(y,x)
plt.plot(y[-1],x[-1],'+k')
plt.plot(XE[:,0],XN[:,0],'--',color='black')
plt.plot(XE[0,0],XN[0,0],'x',color='b')
plt.plot(XE[-1,0],XN[-1,0],'x',color='r')
plt.grid()
plt.axis('equal')

plt.show()

plt.figure()
plt.plot(y_gps,x_gps,'o-',color='C1',alpha=0.7)
plt.plot(y,x)
plt.plot(XE[:,0],XN[:,0],'--',color='black')
plt.plot(XE[0,0],XN[0,0],'x',color='b')
plt.plot(XE[-1,0],XN[-1,0],'x',color='r')
plt.grid()
plt.axis('equal')

plt.show()
