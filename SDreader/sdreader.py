    #!/usr/bin/env python
# -*- coding: utf-8 -*-

# 08/06/2020
# Mairon de Souza Wolniewicz

# le o trajeto armazenado no cartão de memória
# realiza filtro de kalman linear em 2 dimensões de uma trajetória real
# recebe como entrada um arquivo log####.dat no padrao
# [dN dE vN vE aN aE heading lat lon dNest dEest dt]

import glob
import numpy as np
import matplotlib.pyplot as plt
from KalmanFilter import LinearKalmanFilter1D
from matplotlib2tikz import save as tikz_save

# plot colors
c1 = (0,0.4,1)
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)

# --------------------------------------------
# lista pastas
print "Available directories"
folderlist = glob.glob("logs/*/")
folderlist.sort()
for i in range(len(folderlist)):
    print folderlist[i]+'\t'+str(i)
    
folderindex = input("Select directory: ")
currentfolder = folderlist[folderindex]

print currentfolder+"*.dat"

# lista arquivos
print "Available path data"
filelist = glob.glob(currentfolder+"*.dat")
filelist.sort()
for i in range(len(filelist)):
    print filelist[i]+'\t'+str(i)
    
logindex = input("Select log: ")
currentlog = filelist[logindex]
print currentlog

# --------------------------------------------
# le dados

trajeto = np.loadtxt(currentlog,unpack='True',delimiter=',')

dN = trajeto[0]             # [m]
dE = trajeto[1]             # [m]
vN = trajeto[2]             # [m/s]
vE = trajeto[3]             # [m/s]
aN = trajeto[4]             # [m/s2]
aE = trajeto[5]             # [m/s2]
heading = -np.deg2rad(trajeto[6]) # [rad]
lat = trajeto[7]            # [deg]
lon = trajeto[8]            # [deg]
dNest_esp = trajeto[9]      # [m]
dEest_esp = trajeto[10]     # [m]
dt = trajeto[11]            # [s]

# ajuste de origem para dados recortados
dN0 = dN[0]
dE0 = dE[0]
vN0 = vN[0]
vE0 = vE[0]
dNestesp0 = dNest_esp[0]
dEestesp0 = dEest_esp[0]
for i in range(len(dN)):
    dN[i] = dN[i] - dN0
    dE[i] = dE[i] - dE0
    vN[i] = vN[i] - vN0
    vE[i] = vE[i] - vE0
    dNest_esp[i] = dNest_esp[i] - dNestesp0
    dEest_esp[i] = dEest_esp[i] - dEestesp0


# calcula o tempo que se passou
t = np.zeros(len(dN))
t[0] = dt[0]
for i in range(1,len(t)):
    t[i] = t[i-1] + dt[i]
    print i,t[i]
    
#for i in range(len(t)):
    #print "%.6f,%.6f"%(lon[i],lat[i])

# modulo da aceleração a cada passo
amod = (aN*aN+aE*aE)*0.5
ahead = np.arctan2(aN,aE)

# --------------------------------------------
# FILTRO DE KALMAN

Rd = 2.5    # [m]
Rv = 0.1    # [m]
Qa = 0.32  # [m/s2]

# NORTE

N_est = LinearKalmanFilter1D(dN,vN,aN,Rd,Rv,Qa,dt)
dN_est = N_est[:,0]
vN_est = N_est[:,1]

# LESTE

E_est = LinearKalmanFilter1D(dE,vE,aE,Rd,Rv,Qa,dt)
dE_est = E_est[:,0]
vE_est = E_est[:,1]

# --------------------------------------------
# plota graficos

# Linear
figLin = plt.figure("Linear")
axd = figLin.add_subplot(311)
axv = figLin.add_subplot(312)
axa = figLin.add_subplot(313)

axd.grid()
axd.plot(t,dN,':',color=c1)
axd.plot(t,dE,':',color=c2)
axd.plot(t,dN_est,color=c1)
axd.plot(t,dE_est,color=c2)
axd.plot(t,dNest_esp,'--',color=c1,alpha=0.7)
axd.plot(t,dEest_esp,'--',color=c2,alpha=0.7)

axv.grid()
axv.plot(t,vN,':',color=c1)
axv.plot(t,vE,':',color=c2)
axv.plot(t,vN_est,color=c1)
axv.plot(t,vE_est,color=c2)

axa.grid()
axa.plot(t,aN,':',color=c1)
axa.plot(t,aE,':',color=c2)

#tikz_save(currentlog+"_linear.tex")

# 2D
fig2D = plt.figure("2D")
ax2D = fig2D.add_subplot(111,aspect='equal')
ax2D.grid()
ax2D.plot(dE,dN,'o-',color=c1,alpha=0.7)
ax2D.plot(dE_est,dN_est,color=c2)
ax2D.plot(dEest_esp,dNest_esp,'k')

#tikz_save(currentlog+"_2d.tex")

#acceleration heading
dens = 1
alen = (aN*aN+aE*aE)**0.5
alen = alen[0:len(dE):dens]
#ax2D.plot([dE_est[0:len(dE):dens],dE_est[0:len(dE):dens]+alen*np.sin(ahead[0:len(dE):dens])],[dN_est[0:len(dE):dens],dN_est[0:len(dE):dens]+alen*np.cos(ahead[0:len(dE):dens])],color=c3)

#true heading
alen = np.mean(alen)
for i in range(1,len(dE)):
    if(dE[i-1]!=dE[i]):
        ax2D.plot([dE[i],dE[i]+alen*np.sin(heading[i])],[dN[i],dN[i]+alen*np.cos(heading[i])],color=c3)


# Geo
figGeo = plt.figure("Real Coordinates")
axGeo = figGeo.add_subplot(111,aspect='equal')
axGeo.grid()
axGeo.plot(lon,lat,'k.-')

# Heading
#figHead = plt.figure("Heading")
#axh = figHead.add_subplot(111)
#axh.plot(t,-heading*180/np.pi)
#axh.grid()


#alpha = np.zeros(len(dE))
#delta = np.zeros(len(dE))
#deltaesp = np.zeros(len(dE))
#for i in range(1,len(dE)):
    #if(dE[i]-dE[i-1]==0):
        #alpha[i] = alpha[i-1]
    #else:
        #alpha[i] = dE[i]-dE[i-1]
    #delta[i] = dE[i]-dE_est[i]
    #deltaesp[i] = dE[i]-dEest_esp[i]
    
#Prediction
#figPredict = plt.figure("Prediction Analysis")
#axdPredict = figPredict.add_subplot(211)
#axvPredict = figPredict.add_subplot(212)
#axdPredict.grid()
#axvPredict.grid()
#axdPredict.plot(t,alpha*delta,color=c2)
#axdPredict.plot(t,alpha*deltaesp,'--',color=c2)

plt.show()

# generate file with kalman filter data for animation
# dN dE dN_est dE_est dNest_esp dEest_esp
#loganimfile = currentlog[-11:-4]+"anim.dat"
#np.savetxt(loganimfile,np.array([dN,dE,dN_est,dE_est,dNest_esp,dEest_esp,dt]).T,fmt="%.6f",delimiter=',')


