# LIVE MAGNETIC FIELD VISUALIZER
# written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
# last update: 05/05/2019

# Script listens to serial port, plot magnetometer data in real time and write contents into a file

# Useful to calibrate a magnetometer with Hard Iron and Soft Iron corrections
# Ploting a 2D graph with eccentric groups of data means the need for a hard iron correction
# Centered, but not almost spherical groups of data means the need for a soft iron correction
# Both corrections are applied in 'mpu9250.c' function calibrateMag (TIVA) 
# or in 'mpu9250.cpp' function calibrateMagExternal (ESP32)
# Uncomment it before the main loop and follow the steps

import serial # requires pySerial to be installed
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib2tikz import save as tikz_save
from bluetooth import *

PLOTSIZE = 40

MARK = 4 # size of plot mark

GENERATETIKZ = 0 # set to 1 if you want to generate tikz code

# plot colors
c1 = (0,0.4,1)  
c2 = (0.8,0,0)
c3 = (0.2,0.8,0)

fig = plt.figure()
ax = plt.axes(xlim=(-PLOTSIZE,PLOTSIZE), ylim=(-PLOTSIZE,PLOTSIZE))
ax.set_aspect('equal', adjustable='box')
linexy, = ax.plot([],[],'.-',markersize=MARK,color=c1)
linexz, = ax.plot([],[],'.-',markersize=MARK,color=c2)
lineyz, = ax.plot([],[],'.-',markersize=MARK,color=c3)

X = []
Y = []
Z = []
Xscl = []
Yscl = []
Zscl = []

# search for bluetooth devices
devices = discover_devices(lookup_names=True)
print(type(devices))
print("Devices found: %s" % len(devices))
for item in devices:
    print item
    if(item[1]=="ESP32_Mairon"):
        macAddr = item[0]
        BTsocket=BluetoothSocket(RFCOMM)
        BTsocket.connect((macAddr,1))
        print "found"

def init():
    linexy.set_data([],[])
    linexz.set_data([],[])
    lineyz.set_data([],[])
    return linexy, linexz, lineyz,

templine = ""

def animate(i):
        global templine
        global BTsocket
    #try:
        linha = BTsocket.recv(24)
    
        linha = linha.decode("utf-8")
        truelinha = templine+linha
        print "TEMPLINE: ",templine
        print "LINHA: ",linha
        print "TRUELINHA: ",
        print truelinha
        
        nlineindex = linha.find('\n')
        templine = linha[nlineindex:24]

        l = truelinha[0:nlineindex].split(',')
        print "L: ",l
        if(len(l)>=3):
            X.append(float(l[0]))
            Y.append(float(l[1]))
            Z.append(float(l[2]))

            linexy.set_data(X,Y)
            linexz.set_data(X,Z)
            lineyz.set_data(Y,Z)

        return linexy,linexz,lineyz,
    #except:
        #return linexy,linexz,lineyz,

# if you click on the plot, it will close and the calibration will start
def onClick(event):
    anim.event_source.stop()
    plt.close()

# calibration function
# input: raw magnetometer data collected
def Calibration(X,Y,Z):
    print "\nCalibrating magnetometer...\n"

    X = np.array(X)
    Y = np.array(Y)
    Z = np.array(Z)

    mag_offset = np.array([0.0,0.0,0.0])
    mag_scale = np.array([1.0,1.0,1.0])

    minimum = [32768.0,32768.0,32768.0]
    maximum = [-32768.0,-32768.0,-32768.0]

    minimum[0] = min(min(X), minimum[0])
    minimum[1] = min(min(Y), minimum[1])
    minimum[2] = min(min(Z), minimum[2])

    maximum[0] = max(max(X), maximum[0])
    maximum[1] = max(max(Y), maximum[1])
    maximum[2] = max(max(Z), maximum[2])

    print "Min: ", minimum
    print "Max: ", maximum

    # hard iron correction
    # get the average bias in all three directions
    mag_offset[0] = (maximum[0] + minimum[0])*0.5
    mag_offset[1] = (maximum[1] + minimum[1])*0.5
    mag_offset[2] = (maximum[2] + minimum[2])*0.5

    # soft iron correction
    # get average max chord lenght in all three axis
    mag_scale[0] = (maximum[0] - minimum[0])*0.5
    mag_scale[1] = (maximum[1] - minimum[1])*0.5
    mag_scale[2] = (maximum[2] - minimum[2])*0.5

    # average the three max chord values
    mag_scale = np.mean(mag_scale)/mag_scale

    print "Offset: ", mag_offset
    print "Scale: ", mag_scale

    Xhard = X - mag_offset[0]
    Yhard = Y - mag_offset[1]
    Zhard = Z - mag_offset[2]

    Xsoft = X * mag_scale[0]
    Ysoft = Y * mag_scale[1]
    Zsoft = Z * mag_scale[2]

    Xscl = (np.array(X) - mag_offset[0])*mag_scale[0]
    Yscl = (np.array(Y) - mag_offset[1])*mag_scale[1]
    Zscl = (np.array(Z) - mag_offset[2])*mag_scale[2]

    np.savetxt('mag_calibration.dat',[mag_offset,mag_scale],header='offset\nscale\n[X,Y,Z] ')

    # plot raw data
    print 'RAW DATA'
    # ax1 = plt.subplot(141)
    ax1 = plt.subplot()
    ax1.set_xlim([-PLOTSIZE,PLOTSIZE])
    ax1.set_ylim([-PLOTSIZE,PLOTSIZE])
    ax1.set_aspect('equal', adjustable='box')
    ax1.plot(X,Y,'.',color=c1,markersize=MARK,label='XY')
    ax1.plot(X,Z,'.',color=c2,markersize=MARK,label='XZ')
    ax1.plot(Y,Z,'.',color=c3,markersize=MARK,label='YZ')
    plt.xlabel("$\mu$T")
    plt.ylabel("$\mu$T")
    plt.grid()
    plt.legend()
    plt.title('RAW DATA')
    if(GENERATETIKZ):
        tikz_save("mag_raw.tex")
    plt.show()

    # plot raw data with hard iron correction
    print 'HARD IRON'
    # ax2 = plt.subplot(142)
    ax2 = plt.subplot()
    ax2.set_xlim([-PLOTSIZE,PLOTSIZE])
    ax2.set_ylim([-PLOTSIZE,PLOTSIZE])
    # ax2.set_yticklabels([])
    ax2.set_aspect('equal', adjustable='box')
    ax2.plot(Xhard,Yhard,'.',color=c1,markersize=MARK,label='XY')
    ax2.plot(Xhard,Zhard,'.',color=c2,markersize=MARK,label='XZ')
    ax2.plot(Yhard,Zhard,'.',color=c3,markersize=MARK,label='YZ')
    plt.xlabel("$\mu$T")
    plt.ylabel("$\mu$T")
    plt.grid()
    plt.title('HARD IRON')
    plt.legend()
    if(GENERATETIKZ):
        tikz_save("mag_hard.tex")
    plt.show()

    # plot raw data with soft iron correction
    print 'SOFT IRON'
    # ax3 = plt.subplot(143)
    ax3 = plt.subplot()
    ax3.set_xlim([-PLOTSIZE,PLOTSIZE])
    ax3.set_ylim([-PLOTSIZE,PLOTSIZE])
    # ax3.set_yticklabels([])
    ax3.set_aspect('equal', adjustable='box')
    ax3.plot(Xsoft,Ysoft,'.',color=c1,markersize=MARK,label='XY')
    ax3.plot(Xsoft,Zsoft,'.',color=c2,markersize=MARK,label='XZ')
    ax3.plot(Ysoft,Zsoft,'.',color=c3,markersize=MARK,label='YZ')
    plt.xlabel("$\mu$T")
    plt.ylabel("$\mu$T")
    plt.grid()
    plt.legend()
    if(GENERATETIKZ):
        tikz_save("mag_soft.tex")
    plt.title('SOFT IRON')
    plt.show()

    # plot raw data with full correction
    print 'HARD+SOFT IRON'
    # ax4 = plt.subplot(144)
    ax4 = plt.subplot()
    ax4.set_xlim([-PLOTSIZE,PLOTSIZE])
    ax4.set_ylim([-PLOTSIZE,PLOTSIZE])
    # ax4.set_yticklabels([])
    ax4.set_aspect('equal', adjustable='box')
    ax4.plot(Xscl,Yscl,'.',color=c1,markersize=MARK,label='XY')
    ax4.plot(Xscl,Zscl,'.',color=c2,markersize=MARK,label='XZ')
    ax4.plot(Yscl,Zscl,'.',color=c3,markersize=MARK,label='YZ')
    plt.xlabel("$\mu$T")
    plt.ylabel("$\mu$T")
    plt.grid()
    plt.legend()
    if(GENERATETIKZ):
        tikz_save("mag_scl.tex")
    plt.title('HARD+SOFT IRON')
    plt.show()

    # plot scaled data in 3D
    fig3D = plt.figure()
    ax3D = fig3D.gca(projection='3d')
    ax3D.set_aspect('equal')
    ax3D.plot(Xscl,Yscl,Zscl,'.')
    plt.grid()

    plt.show()
    
    print "\nCalibration finished.\n\n"
    print 'OFFSET',mag_offset
    print 'SCALE',mag_scale

anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=360, interval=1, blit=True,save_count=10)

fig.canvas.mpl_connect('button_press_event', onClick)

plt.grid()
plt.show()

print "\nData extraction finished."

Calibration(X,Y,Z)

# plt.show()






