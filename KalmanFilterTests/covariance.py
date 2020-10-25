#!/usr/bin/env python
# -*- coding: utf-8 -*-

# returns the error variance of n samples when called for gps and accel
# reads dN,dE,dD,vN,vE,vD,aN,aE,aD
# 21/09/2019
# Mairon de Souza Wolniewicz

import numpy as np
import matplotlib.pyplot as plt

def Covariance(mode=0):
    print 'Calculating covariance'
    gpsdN = []
    gpsdE = []
    gpsdD = []
    gpsvN = []
    gpsvE = []
    gpsvD = []
    imuaN = []
    imuaE = []
    imuaD = []

    # Measurement Covariance (R)
    R_Xvar  = 2.5       # ± [m] GPS datasheet p.6
    R_Yvar  = 2.5       # ± [m] GPS datasheet p.6
    R_Zvar  = 2.5       # ± [m] GPS calculated real-measure
    R_vXvar = 0.1       # ± [m/s] GPS datasheet p.6
    R_vYvar = 0.1       # ± [m/s] GPS datasheet p.6
    R_vZvar = 0.1       # ± [m/s] GPS datasheet p.6

    # Process Noise Covariance (Q)
    Q_aNvar = 0.006     # ± [m/s^2] IMU calib.
    Q_aEvar = 0.012     # ± [m/s^2] IMU calib.
    Q_aDvar = 0.003     # ± [m/s^2] IMU calib.


    print 'Getting sensor data...'
    for i in range(5000):
        try:
            linha = open('MPU.dat','r').read()
            l = linha.split('\t')
            if(len(l)==12):
                gpsdN.append(float(l[0]))
                gpsdE.append(float(l[1]))
                gpsdD.append(float(l[2]))
                gpsvN.append(float(l[3]))
                gpsvE.append(float(l[4]))
                gpsvD.append(float(l[5]))
                imuaN.append(float(l[6]))
                imuaE.append(float(l[7]))
                imuaD.append(float(l[8]))
        except:
            a = 0

    R_Xvar = np.std(gpsdN)
    R_Yvar = np.std(gpsdE)
    R_Zvar = np.std(gpsdD)
    R_vXvar = np.std(gpsvN)
    R_vYvar = np.std(gpsvE)
    R_vZvar = np.std(gpsvD)
    Q_aNvar = np.std(imuaN)
    Q_aEvar = np.std(imuaE)
    Q_aDvar = np.std(imuaD)

    print R_Xvar,'\t',R_Yvar,'\t',R_Zvar,'\t',R_vXvar,'\t',R_vYvar,'\t',R_vZvar,'\t',Q_aNvar,'\t',Q_aEvar,'\t',Q_aDvar

    if(mode==0):
        return R_Xvar,R_Yvar,R_Zvar,R_vXvar,R_vYvar,R_vZvar,Q_aNvar,Q_aEvar,Q_aDvar
    if(mode==1):
        return R_Xvar,R_Yvar,R_Zvar,R_vXvar,R_vYvar,R_vZvar
    if(mode==2):
        return Q_aNvar,Q_aEvar,Q_aDvar
