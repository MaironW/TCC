#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 22/01/2020
# Mairon de Souza Wolniewicz

# realiza filtro de kalman em 2 dimensões de uma trajetória real
# recebe como entrada um arquivo no padrao
# [dN dE vN vE aN aE]

import numpy as np
import matplotlib.pyplot as plt
from KalmanFilter import LinearKalmanFilter1D
from KalmanFilter import ExtendedKalmanFilter1D

# --------------------------------------------
# configura graficos
figN = plt.figure("North")
figE = plt.figure("East")
fig2D = plt.figure("2D")
#figErro = plt.figure("Erro")
axdN = figN.add_subplot(311)
axvN = figN.add_subplot(312)
axaN = figN.add_subplot(313)
axdE = figE.add_subplot(311)
axvE = figE.add_subplot(312)
axaE = figE.add_subplot(313)
ax2D = fig2D.add_subplot(111,aspect='equal')
#axErroN = figErro.add_subplot(211)
#axErroE = figErro.add_subplot(212)

# --------------------------------------------
# le dados

trajeto = np.loadtxt("sid3.dat",unpack='True',delimiter=',')

dN = trajeto[0]
dE = trajeto[1]
vN = trajeto[2]
vE = trajeto[3]
aN = trajeto[4]
aE = trajeto[5]
dN_est = trajeto[6]
dE_est = trajeto[7]
vN_est = trajeto[8]
vE_est = trajeto[9]

dt = 0.2        # [s]

interval = np.arange(0,len(dN),1)*dt

#gps_dt = 0.4 # [s]
#waiting_time = int(gps_dt/dt)

#simulate invalid GPS data for a while
#START =20*waiting_time #[s]
#STOP = 25*waiting_time # [s]

#dN[START:STOP] = dN[START]
#dE[START:STOP] = dE[START]

#vN[START:STOP] = vN[START]
#vE[START:STOP] = vE[START]

# --------------------------------------------
# FILTRO DE KALMAN


# NORTE

Rd_N = 1.7694
Rv_N = 0.532
Qa_N = 1.0

#Rd_N = 2.5
#Rv_N = 0.1
#Qa_N = (2.27e-6)**0.5

N_est = LinearKalmanFilter1D(dN,vN,aN,Rd_N,Rv_N,Qa_N,dt)
dN_estL = N_est[:,0]
vN_estL = N_est[:,1]

N_est = ExtendedKalmanFilter1D(dN,vN,aN,Rd_N,Rv_N,Qa_N,dt)
dN_estE = N_est[:,0]
vN_estE = N_est[:,1]
aN_estE = N_est[:,2]

# LESTE

Rd_E = 1.7274
Rv_E = 0.318
Qa_E = 1.0

#Rd_E = 2.5
#Rv_E = 0.1
#Qa_E = (2.70e-6)**0.5

E_est = LinearKalmanFilter1D(dE,vE,aE,Rd_E,Rv_E,Qa_E,dt)
dE_estL = E_est[:,0]
vE_estL = E_est[:,1]

E_est = ExtendedKalmanFilter1D(dE,vE,aE,Rd_E,Rv_E,Qa_E,dt)
dE_estE = E_est[:,0]
vE_estE = E_est[:,1]
aE_estE = E_est[:,2]


# --------------------------------------------
# plota graficos

# Linear
axdN.plot(interval,dN,'k')
axdN.plot(interval,dN_estL)
#axdN.plot(dN_est)
#axdN.plot(dN_estE)

axvN.plot(interval,vN,'k')
axvN.plot(interval,vN_estL)
#axvN.plot(vN_est)
#axvN.plot(vN_estE)

axaN.plot(interval,aN,'k')
#axaN.plot(aNclean)
#axaN.plot(aN_estE,color='C1')

axdE.plot(interval,dE,'k')
axdE.plot(interval,dE_estL)
#axdE.plot(dE_est)
#axdE.plot(dE_estE)

axvE.plot(interval,vE,'k')
axvE.plot(interval,vE_estL)
#axvE.plot(vE_est)
#axvE.plot(vE_estE)

axaE.plot(interval,aE,'k')
#axaE.plot(aEclean)
#axaE.plot(aE_estE,color='C1')


axdN.grid()
axvN.grid()
axaN.grid()
axdE.grid()
axvE.grid()
axaE.grid()

# 2D

FINAL = -1

ax2D.plot(dE[0:FINAL],dN[0:FINAL],'k.-')
ax2D.plot(dE_estL[0:FINAL],dN_estL[0:FINAL],color='C0')
#ax2D.plot(dE_est[0:FINAL],dN_est[0:FINAL],color='C1')
#ax2D.plot(dE_estE,dN_estE,color='C1')
ax2D.grid()

# Erro
#axErroN.plot(abs(dN-dN_estL))
#axErroE.plot(abs(dE-dE_estL))
#axErroN.grid()
#axErroE.grid()

plt.show()
