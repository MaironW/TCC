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
axdN = figN.add_subplot(311)
axvN = figN.add_subplot(312)
axaN = figN.add_subplot(313)
axdE = figE.add_subplot(311)
axvE = figE.add_subplot(312)
axaE = figE.add_subplot(313)
ax2D = fig2D.add_subplot(111,aspect='equal')

# --------------------------------------------
# le dados

trajeto = np.loadtxt("trajeto5.dat",unpack='True',delimiter=',')

dN = trajeto[0]
dE = trajeto[1]
vN = trajeto[2]
vE = trajeto[3]
aN = trajeto[4]*9.81
aE = trajeto[5]*9.81
aNclean = trajeto[6]*9.81
aEclean = trajeto[7]*9.81
roll = trajeto[8]
pitch = trajeto[9]
yaw = trajeto[10]

# --------------------------------------------
# distorcao opicional dos dados

START = 320
STOP = 330

dN[START:STOP] = dN[START]
dE[START:STOP] = dE[START]
vN[START:STOP] = vN[START]
vE[START:STOP] = vE[START]

# --------------------------------------------
# FILTRO DE KALMAN

dt = 0.2        # [s]

# NORTE

Rd_N = 0.1
Rv_N = 0.1
Qa_N = 0.1

N_est = LinearKalmanFilter1D(dN[0:START],vN[0:START],aN[0:START],Rd_N,Rv_N,Qa_N,dt)
dN_estL1 = N_est[:,0]
vN_estL1 = N_est[:,1]

N_est = ExtendedKalmanFilter1D(dN[0:START],vN[0:START],aN[0:START],Rd_N,Rv_N,Qa_N,dt)
dN_estE1 = N_est[:,0]
vN_estE1 = N_est[:,1]
aN_estE1 = N_est[:,2]

N_est = LinearKalmanFilter1D(dN[STOP:-1],vN[STOP:-1],aN[STOP:-1],Rd_N,Rv_N,Qa_N,dt)
dN_estL2 = N_est[:,0]
vN_estL2 = N_est[:,1]

N_est = ExtendedKalmanFilter1D(dN[STOP:-1],vN[STOP:-1],aN[STOP:-1],Rd_N,Rv_N,Qa_N,dt)
dN_estE2 = N_est[:,0]
vN_estE2 = N_est[:,1]
aN_estE2 = N_est[:,2]

Rd_N = 1.7694
Rv_N = 0.0532
Qa_N = 0.003

#Rd_N = 100
#Rv_N = 0.1
#Qa_N = 0.1

N_est = LinearKalmanFilter1D(dN[START:STOP],vN[START:STOP],aN[START:STOP],Rd_N,Rv_N,Qa_N,dt)
dN_estL3 = N_est[:,0]
vN_estL3 = N_est[:,1]

N_est = ExtendedKalmanFilter1D(dN[START:STOP],vN[START:STOP],aN[START:STOP],Rd_N,Rv_N,Qa_N,dt)
dN_estE3 = N_est[:,0]
vN_estE3 = N_est[:,1]
aN_estE3 = N_est[:,2]

# LESTE

Rd_E = 0.1
Rv_E = 0.1
Qa_E = 0.1

E_est = LinearKalmanFilter1D(dE[0:START],vE[0:START],aE[0:START],Rd_E,Rv_E,Qa_E,dt)
dE_estL1 = E_est[:,0]
vE_estL1 = E_est[:,1]

E_est = ExtendedKalmanFilter1D(dE[0:START],vE[0:START],aE[0:START],Rd_E,Rv_E,Qa_E,dt)
dE_estE1 = E_est[:,0]
vE_estE1 = E_est[:,1]
aE_estE1 = E_est[:,2]

E_est = LinearKalmanFilter1D(dE[STOP:-1],vE[STOP:-1],aE[STOP:-1],Rd_E,Rv_E,Qa_E,dt)
dE_estL2 = E_est[:,0]
vE_estL2 = E_est[:,1]

E_est = ExtendedKalmanFilter1D(dE[STOP:-1],vE[STOP:-1],aE[STOP:-1],Rd_E,Rv_E,Qa_E,dt)
dE_estE2 = E_est[:,0]
vE_estE2 = E_est[:,1]
aE_estE2 = E_est[:,2]

Rd_E = 1.0274
Rv_E = 0.0318
Qa_E = 0.003

#Rd_E = 100
#Rv_E = 0.1
#Qa_E = 0.1

E_est = LinearKalmanFilter1D(dE[START:STOP],vE[START:STOP],aE[START:STOP],Rd_E,Rv_E,Qa_E,dt)
dE_estL3 = E_est[:,0]
vE_estL3 = E_est[:,1]

E_est = ExtendedKalmanFilter1D(dE[START:STOP],vE[START:STOP],aE[START:STOP],Rd_E,Rv_E,Qa_E,dt)
dE_estE3 = E_est[:,0]
vE_estE3 = E_est[:,1]
aE_estE3 = E_est[:,2]

# N_est = ExtendedKalmanFilter1D(dN,vN,aN,Rd_N,Rv_N,Qa_N,dt)
# dN_estE = N_est[:,0]
# vN_estE = N_est[:,1]
# aN_estE = N_est[:,2]
#
# E_est = ExtendedKalmanFilter1D(dE,vE,aE,Rd_E,Rv_E,Qa_E,dt)
# dE_estE = E_est[:,0]
# vE_estE = E_est[:,1]
# aE_estE = E_est[:,2]

# --------------------------------------------
# plota graficos

# Linear
axdN.plot(dN,'k')
# axdN.plot(dN_estL)
# axdN.plot(dN_estE)

axvN.plot(vN,'k')
# axvN.plot(vN_estL)
# axvN.plot(vN_estE)

axaN.plot(aN,'k')
axaN.plot(aNclean)
# axaN.plot(aN_estE,color='C1')

axdE.plot(dE,'k')
# axdE.plot(dE_estL)
# axdE.plot(dE_estE)

axvE.plot(vE,'k')
# axvE.plot(vE_estL)
# axvE.plot(vE_estE)

axaE.plot(aE,'k')
axaE.plot(aEclean)
# axaE.plot(aE_estE,color='C1')


axdN.grid()
axvN.grid()
axaN.grid()
axdE.grid()
axvE.grid()
axaE.grid()

# 2D
ax2D.plot(dE,dN,'k.')
ax2D.plot(dE_estL1,dN_estL1,color='C0')
ax2D.plot(dE_estL2,dN_estL2,color='C0')
ax2D.plot(dE_estL3,dN_estL3,color='C0')
ax2D.plot(dE_estE1,dN_estE1,color='C1')
ax2D.plot(dE_estE2,dN_estE2,color='C1')
ax2D.plot(dE_estE3,dN_estE3,color='C1')
# ax2D.plot(dE_estE,dN_estE)
ax2D.grid()

plt.show()
