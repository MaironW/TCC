#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 27/03/2020
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt
from numpy.random import randn
from numpy.linalg import inv
# from matplotlib2tikz import save as tikz_save

# plot colors
c1 = (0,0.4,1)  
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)

# Measurement Covariance (R)
#dNvar  = 1.7       # ± [m]
#dEvar  = 1.7       # ± [m]
#vNvar = 0.5        # ± [m/s]
#vEvar = 0.5        # ± [m/s]

dNvar  = 2.5       # ± [m]
dEvar  = 2.5       # ± [m]
vNvar = 0.1        # ± [m/s]
vEvar = 0.1        # ± [m/s]

# Process Noise Covariance (Q)
aNvar = 0.32
aEvar = 0.32

#dNvar  = 2.75       # ± [m]
#dEvar  = 2.75       # ± [m]
#vNvar = 1.22        # ± [m/s]
#vEvar = 1.22        # ± [m/s]


n = 8 # number of cycles

dt = 0.04 # [s] time step
t = np.arange(0,n*2*np.pi,dt)
npoints = len(t) 

# choose between (1) cicle (2) eight (3) slow eight
path = 3      

# size of the trajectory 
pathsize = 15   # [m]

if(path==1):
    # position
    x = pathsize*np.cos(0.25*t) # circle
    y = pathsize*np.sin(0.25*t) # circle
    # velocity
    vx = -0.25*pathsize*np.sin(0.25*t) # circle
    vy = 0.25*pathsize*np.cos(0.25*t)  # circle
    # accelation
    ax = -0.125*pathsize*np.cos(0.25*t)   # circle
    ay = -0.125*pathsize*np.sin(0.25*t)   # circle
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
elif(path==3):
    # position
    x = pathsize*np.cos(0.125*t)    # eight
    y = pathsize*np.sin(0.25*t)    # eight
    # velocity
    vx = -0.125*pathsize*np.sin(0.125*t)  # eight
    vy = 0.25*pathsize*np.cos(0.25*t) # eight
    # accelation
    ax = -0.015625*pathsize*np.cos(0.125*t)   # eight
    ay = -0.0625*pathsize*np.sin(0.25*t) #eight 
    
        

# apply noise if desired
dNnoise = dNvar
dEnoise = dEvar
vNnoise = vNvar
vEnoise = vEvar
aNnoise = aNvar
aEnoise = aEvar

# dNnoise = 0
# dEnoise = 0
# vNnoise = 0
# vEnoise = 0
# aNnoise = 0
# aEnoise = 0


dN_gps = x + dNnoise * randn(*x.shape)
dE_gps = y + dEnoise * randn(*y.shape)

vN_gps = vx + vNnoise * randn(*vx.shape)
vE_gps = vy + vEnoise * randn(*vy.shape)

aN = ax + aNnoise * randn(*ax.shape)   
aE = ay + aEnoise * randn(*ay.shape)


# simulate GPS delay
gps_dt = 1 # [s]
waiting_time = int(gps_dt/dt)

x_new = np.zeros(npoints)
y_new = np.zeros(npoints)
vx_new = np.zeros(npoints)
vy_new = np.zeros(npoints)
for i in range(0,npoints,waiting_time):
    x_new[i:i+waiting_time]=dN_gps[i]
    y_new[i:i+waiting_time]=dE_gps[i]
    vx_new[i:i+waiting_time]=vN_gps[i]
    vy_new[i:i+waiting_time]=vE_gps[i]
dN_gps = np.copy(x_new)
dE_gps = np.copy(y_new)
vN_gps = np.copy(vx_new)
vE_gps = np.copy(vy_new)


# simulate invalid GPS data for a while
# START = int(len(t)*0.2)
# STOP = int(len(t)*0.28)

# dN_gps[START:STOP] = dN_gps[START]
# dE_gps[START:STOP] = dE_gps[START]

# vN_gps[START:STOP] = vN_gps[START]
# vE_gps[START:STOP] = vE_gps[START]

# ========= KALMAN FILTER

# dynamic matrix
A = np.array([
    [1, dt],
    [0, 1]
    ])

# control matrix
B = np.array([0.5*dt**2, dt])

# measurement matrix
H = np.array([
    [1, 0],
    [0, 1]
    ])

def KalmanFilter(R_d,R_v,Q_a,d,v,a):
    # measurement covariance matrix
    R = np.array([[R_d*R_d,0],
                  [0,R_v*R_v]])

    # process noise covariance matrix
    #Q = np.array([[Q_a*Q_a,0],
                  #[0,Q_a*Q_a]])
                  
    # discrete noise
    Q = Q_a*Q_a*np.array([[0.25*dt**4,0.5*dt**3],
                          [0.5*dt**3,dt**2]])
    
    # continuous noise
    #Q = Q_a*Q_a*np.array([[0.05*dt**5,0.125*dt**4],
                        #[0.125*dt**4,dt**3/3.0]])
    
    # create an observation vector of noisy GPS signals
    Z = np.array([d,v]).T

    # control signal
    u = a

    # matrix dimensions
    nx = Q.shape[0]
    ny = R.shape[0]
    nt = Z.shape[0]

    # allocate identity matrix for re-use
    I = np.eye(nx)

    # initial estimative
    x_init = np.array([d[0], v[0]]).T
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
            x_pred[i] = np.dot(A,x_est[i-1]) + B.T*u[i]
            P_pred[i] = np.dot(np.dot(A,P_est[i-1]),A.T) + Q

        # estimation stage
        y = Z[i] - np.dot(H,x_pred[i])
        

        # Kalman gain
        S = np.dot(np.dot(H,P_pred[i]),H.T)+R
        K[i] = np.dot(np.dot(P_pred[i],H.T), inv(S))

        # prediction
        x_est[i] = x_pred[i] + np.dot(K[i],y)
        #P_est[i] = np.dot((I - np.dot(K[i],H)),P_pred[i])
        P_est[i] = np.dot(np.dot(I-K[i],P_pred[i]),(I-K[i]).T) + np.dot(np.dot(K[i],R),K[i].T)

    return x_est

# ESTIMATIVAS
XN = KalmanFilter(dNvar,vNvar,aNvar,dN_gps,vN_gps,aN)
XE = KalmanFilter(dEvar,vEvar,aEvar,dE_gps,vE_gps,aE)

# ERROS ABSOLUTOS
# posicao
errodNgps = abs(dN_gps-x)
errodEgps = abs(dE_gps-y)
errodXgps = (errodNgps**2 + errodEgps**2)**0.5 # modulo do erro 
errodNest = abs(XN[:,0]-x)
errodEest = abs(XE[:,0]-y)
errodXest = (errodNest**2 + errodEest**2)**0.5 # modulo do erro
# velocidade 
errovNgps = abs(vN_gps-vx)
errovEgps = abs(vE_gps-vy)
errovXgps = (errovNgps**2 + errovEgps**2)**0.5 # modulo do erro 
errovNest = abs(XN[:,1]-vx)
errovEest = abs(XE[:,1]-vy)
errovXest = (errovNest**2 + errovEest**2)**0.5 # modulo do erro

# # ERROS RELATIVOS
# reldNgps = abs(dN_gps-x)/abs(x+1e-10)
# reldEgps = abs(dE_gps-y)/abs(y+1e-10)
# reldXgps = (reldNgps**2 + reldEgps**2)**0.5 # modulo do rel 
# reldNest = abs(XN[:,0]-x)/abs(x+1e-10)
# reldEest = abs(XE[:,0]-y)/abs(y+1e-10)
# reldXest = (reldNest**2 + reldEest**2)**0.5 # modulo do rel
# # velocidade 
# relvNgps = abs(vN_gps-vx)/abs(vx+1e-10)
# relvEgps = abs(vE_gps-vy)/abs(vy+1e-10)
# relvXgps = (relvNgps**2 + relvEgps**2)**0.5 # modulo do rel 
# relvNest = abs(XN[:,1]-vx)/abs(vx+1e-10)
# relvEest = abs(XE[:,1]-vy)/abs(vy+1e-10)
# relvXest = (relvNest**2 + relvEest**2)**0.5 # modulo do rel


tlim = 0.0 # [s]
ilim = int(tlim/dt)

        
# Plota os graficos

plt.figure("Linear")
plt.subplot(3,1,1)
plt.plot(t,dN_gps,color=c1)
plt.plot(t,dE_gps,color=c2)
plt.plot(t,XN[:,0],'--',color=c1)
plt.plot(t,XE[:,0],'--',color=c2)
plt.grid()
plt.axvline(x=tlim,color='k')


plt.subplot(3,1,2)
plt.plot(t,vN_gps,color=c1)
plt.plot(t,vE_gps,color=c2)
plt.plot(t,XN[:,1],'--',color=c1)
plt.plot(t,XE[:,1],'--',color=c2)
plt.grid()
plt.axvline(x=tlim,color='k')

plt.subplot(3,1,3)
plt.plot(t,aN,color=c1)
plt.plot(t,aE,color=c2)
plt.grid()
plt.axvline(x=tlim,color='k')

#tikz_save("noiselinear.tex")
#plt.show()

plt.figure("Erro")
plt.subplot(2,1,1)
plt.grid()
# plt.plot(t,errodNgps,color=c1,alpha=0.7)
# plt.plot(t,errodEgps,color=c2,alpha=0.7)
# plt.plot(t,errodNest,color=c1)
# plt.plot(t,errodEest,color=c2)
plt.plot(t,errodXgps,'k',alpha=0.7)
plt.plot(t,errodXest,'k')
plt.axvline(x=tlim,color='k')

print "Erro de posição:"
# print "dN:", np.mean(abs(dN_gps-x)), "dE:", np.mean(abs(dE_gps-y))
# print "dNest:", np.mean(abs(XN[:,0]-x)), "dEest:", np.mean(abs(XE[:,0]-y))
# print "dXgps:", np.mean(errodXgps)
# print "dXest:", np.mean(errodXest)

ierro = 0
testerro = errodXgps-errodXest
for i in range(len(testerro)):
    if(testerro[i]>=0):
       ierro+=1
dterro = ierro*dt
print dterro
print ierro/float(npoints)


plt.subplot(2,1,2)
plt.grid()
# plt.plot(t,errovNgps,color=c1,alpha=0.7)
# plt.plot(t,errovEgps,color=c2,alpha=0.7)
# plt.plot(t,errovNest,color=c1)
# plt.plot(t,errovEest,color=c2)
plt.plot(t,errovXgps,'k',alpha=0.7)
plt.plot(t,errovXest,'k')
plt.axvline(x=tlim,color='k')

print "Erro de velocidade:"
# print "vN:", np.mean(abs(vN_gps-vx)), "vE:", np.mean(abs(vE_gps-vy))
# print "vNest:", np.mean(abs(XN[:,1]-vx)), "vEest:", np.mean(abs(XE[:,1]-vy))
# print "vXgps:", np.mean(errovXgps)
# print "vXest:", np.mean(errovXest)

ierro = 0
testerro = errovXgps-errovXest
for i in range(len(testerro)):
    if(testerro[i]>=0):
       ierro+=1
dterro = ierro*dt
print dterro
print ierro/float(npoints)


#tikz_save("noiseerror.tex")
#plt.show()

plt.figure("Trajetoria")

plt.plot(y,x,color='black')
plt.plot(dE_gps,dN_gps,'o-',color=c1,alpha=0.7)
plt.plot(y[-1],x[-1],'+k')
plt.plot(XE[:,0],XN[:,0],'-',color=c2)
plt.plot(XE[0,0],XN[0,0],'x',color=c2)
plt.plot(XE[-1,0],XN[-1,0],'x',color=c2)
plt.grid()
plt.axis('equal')
plt.plot(y[ilim],x[ilim],'o',color='orange')
#tikz_save("noisepath.tex")

plt.show()

#loganimfile = "kfsimblackoutanim.dat"
#np.savetxt(loganimfile,np.array([dN_gps,dE_gps,XN[:,0],XE[:,0],x,y]).T,fmt="%.6f",delimiter=',')
