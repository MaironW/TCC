# LIVE 3 AXIS VALUE PLOTTER
# written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
# last update: 17/09/2019

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
from matplotlib2tikz import save as tikz_save
from numpy.random import randn

c1 = (0,0.4,1)
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)


X = []
Y = []
Z = []

while (len(X)<=100):
    try:
        linha = open('MPU.dat','r').read()
        l = linha.split('\t')
        print l

        X.append(float(l[0]))
        Y.append(float(l[1]))
        Z.append(float(l[2]))

    except:
        a = 1

SF = 4096.0 # scale factor

Xscl = np.array(X)/SF
Yscl = np.array(Y)/SF
Zscl = np.array(Z)/SF

Xbias = np.mean(Xscl)
Ybias = np.mean(Yscl)
Zbias = np.mean(Zscl) - 1

print Xbias,Ybias,Zbias

Xtrue = Xscl - Xbias
Ytrue = Yscl - Ybias
Ztrue = Zscl - Zbias

Xsin = Xtrue + np.array(X) * randn(*Xtrue.shape)
Ysin = Ytrue + np.array(Y) * randn(*Ytrue.shape)
Zsin = Ztrue + np.array(Z) * randn(*Ztrue.shape)

for i in range(30):
    Xsin[i] = Xtrue[i]
    Ysin[i] = Ytrue[i]
    Zsin[i] = Ztrue[i]
    Xsin[len(X)-1-i] = Xtrue[len(X)-1-i]
    Ysin[len(X)-1-i] = Ytrue[len(X)-1-i]
    Zsin[len(X)-1-i] = Ztrue[len(X)-1-i]

plt.figure()
plt.plot(X,label='$X$',color=c1)
plt.plot(Y,label='$Y$',color=c2)
plt.plot(Z,label='$Z$',color=c3)
plt.ylabel('Raw Gyro Data (LSB)')
plt.xlabel('Samples')
plt.legend()
plt.grid()
# tikz_save("g_raw.tex")
plt.show()

plt.figure()
plt.plot(Xscl,label='$X$',color=c1)
plt.plot(Yscl,label='$Y$',color=c2)
plt.plot(Zscl,label='$Z$',color=c3)
plt.ylabel('Scaled Gyro Data (g)')
plt.xlabel('Samples')
plt.legend()
plt.grid()
# tikz_save("g_scl.tex")
plt.show()

plt.figure()
plt.plot(Xtrue,label='$X$',color=c1)
plt.plot(Ytrue,label='$Y$',color=c2)
plt.plot(Ztrue,label='$Z$',color=c3)
plt.ylabel('Scaled Gyro Data (g)')
plt.xlabel('Samples')
plt.legend()
plt.grid()
# tikz_save("g_true.tex")
plt.show()

plt.figure()
plt.plot(Xsin,label='$X$',color=c1)
plt.plot(Ysin,label='$Y$',color=c2)
plt.plot(Zsin,label='$Z$',color=c3)
plt.ylabel('Scaled Gyro Data (g)')
plt.xlabel('Samples')
plt.legend()
plt.grid()
tikz_save("g_true.tex")
plt.show()





