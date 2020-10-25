    #!/usr/bin/env python
# -*- coding: utf-8 -*-

# 09/06/2020
# Mairon de Souza Wolniewicz

# le o trajeto armazenado no cartão de memória
# apresenta apenas os dados de posição do GPS e plota a orientação linear
# recebe como entrada um arquivo log####.dat no padrao
# [dN dE q0 q1 q2 q3 dt]

import glob
import numpy as np
import matplotlib.pyplot as plt
from KalmanFilter import LinearKalmanFilter1D
# from matplotlib2tikz import save as tikz_save
import tikzplotlib

# tempos desejados para marcacao
# tmark = [16.6,18.7,21.3] # roll 4
tmark = [14.26,22.22,22.63,24.21] # pitch 11
# tmark = [5,16,22,30,40]
tlegend = ["a","b","c","d","e"]

# plot colors
c1 = (0,0.4,1)
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)

# --------------------------------------------
# lista pastas
print("Available directories")
folderlist = glob.glob("logs/*/")
folderlist.sort()
for i in range(len(folderlist)):
    print(folderlist[i]+'\t'+str(i))
    
folderindex = eval(input("Select directory: "))
currentfolder = folderlist[folderindex]

print(currentfolder+"*.dat")

# lista arquivos
print("Available path data")
filelist = glob.glob(currentfolder+"*.dat")
filelist.sort()
for i in range(len(filelist)):
    print(filelist[i]+'\t'+str(i))
    
logindex = eval(input("Select log: "))
currentlog = filelist[logindex]
print(currentlog)

# --------------------------------------------
# le dados

trajeto = np.loadtxt(currentlog,unpack='True',delimiter=',')

dN = trajeto[0]             # [m]
dE = trajeto[1]             # [m]
q0 = trajeto[2]             
q1 = trajeto[3]             
q2 = trajeto[4]             
q3 = trajeto[5]              
dt = trajeto[6]             # [s]

# ajuste de origem para dados recortados
dN0 = dN[0]
dE0 = dE[0]
for i in range(len(dN)):
    dN[i] = dN[i] - dN0
    dE[i] = dE[i] - dE0

# calcula o tempo que se passou
t = np.zeros(len(dN))
for i in range(1,len(t)):
    t[i] = t[i-1] + dt[i]

    #print i,t[i]

# ============= TOMAR CUIDADO COM ESSE VALOR
# fator de escala para SD nao sincronizado
alpha = 38/30.0 # fator de escala pro yaw
alpha=1
t = alpha*t
# =============
    
# calcula atitude em angulos de Euler
yaw = np.arctan2(2*(q1*q2 + q0*q3), 1 - 2*(q2*q2 + q3*q3)); # [rad]
decl = yaw*0.5

q1 = -q1
q2 = -q2
q3 = -q3

qDecl0 = q0*np.cos(decl)-q3*np.sin(decl)
qDecl1 = q1*np.cos(decl)+q2*np.sin(decl)
qDecl2 = -q1*np.sin(decl)+q2*np.cos(decl)
qDecl3 = q0*np.sin(decl)+q3*np.cos(decl)

q0 = qDecl0
q1 = qDecl1
q2 = qDecl2
q3 = qDecl3

yaw = np.rad2deg(yaw) # [deg]
yaw = (yaw+360)%360
for i in range(len(yaw)):
    if(yaw[i]>359):
        yaw[i]=0
pitch = np.rad2deg(np.arcsin(2*(q0*q2 - q1*q3)));                       # [deg]
roll = np.rad2deg(np.arctan2(2*q2*q3 + 2*q0*q1, 1 - 2*(q1*q1+q2*q2)));  # [deg]

# --------------------------------------------
# plota graficos

# Linear
figLin = plt.figure("Linear")
# axd = figLin.add_subplot(211)
# axatd = figLin.add_subplot(212)
axatd = figLin.add_subplot()

# axd.grid()
# axd.plot(t,dN,color=c1)
# axd.plot(t,dE,color=c2)

# plt.yticks(np.arange(0,361,step=90)) # yaw
# plt.yticks(np.arange(-181,181,step=90)) # pitch/roll
# plt.xticks(tmark)

axatd.grid()
for i in range(len(tmark)):
    axatd.annotate(tlegend[i],(tmark[i]+1,5))
    # axatd.plot([tmark[i],tmark[i]],[0,360],'k') # yaw
    # axatd.plot([tmark[i],tmark[i]],[min(roll),max(roll)],'k') # roll
    axatd.plot([tmark[i],tmark[i]],[min(pitch),max(pitch)],'k') # pitch
    
    index = (np.abs(t-tmark[i])).argmin()
    # axatd.plot(t[index],roll[index],'.',color=c1)
    axatd.plot(t[index],pitch[index],'.',color=c2)
    # axatd.plot(t[index],yaw[index],'.',color=c3)
    # print(roll[index])
    print(pitch[index])
    # print(yaw[index])

# axatd.plot(t,roll,color=c1)
axatd.plot(t,pitch,color=c2)
# axatd.plot(t,yaw,color=c3)

# tikzplotlib.save(currentlog+"_linear.tex")

plt.show()

# generate file with angle data for animation
# roll pitch yaw dt
# loganimfile = currentlog[-11:-4]+"anim.dat"
# np.savetxt(loganimfile,np.array([roll,pitch,yaw,dt*alpha]).T,fmt="%.6f",delimiter=',')




