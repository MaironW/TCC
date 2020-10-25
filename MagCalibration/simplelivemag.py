import serial # requires pySerial to be installed
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
from mpl_toolkits.mplot3d import Axes3D
from matplotlib2tikz import save as tikz_save

baud_rate = 115200

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
#lineyz, = ax.plot([],[],'.-',markersize=MARK,color=c3)

try:
    serial_port = '/dev/ttyUSB0'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyUSB0"
except serial.serialutil.SerialException:
    serial_port = '/dev/ttyUSB1'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyUSB1"
    
X = []
Y = []
Z = []

def init():
    linexy.set_data([],[])
    linexz.set_data([],[])
    #lineyz.set_data([],[])
    return linexy, linexz #, lineyz,

def animate(i):
    try:
        linha = ser.readline()
        linha = linha.decode("utf-8")
        l = linha.split(',')
        print linha,
        if(len(l)==3):
            X.append(float(l[0]))
            Y.append(float(l[1]))
            #Z.append(float(l[2]))

            linexy.set_data(X,Y)
            #linexz.set_data(X,Z)
            #lineyz.set_data(Y,Z)

        return linexy,linexz #,lineyz,
    except ValueError:
        return linexy,linexz #,lineyz,
    
anim = animation.FuncAnimation(fig, animate, init_func=init,frames=360,interval=1)
plt.grid()
plt.show()

ax2 = plt.axes(xlim=(-PLOTSIZE,PLOTSIZE), ylim=(-PLOTSIZE,PLOTSIZE))
ax2.set_aspect('equal', adjustable='box')
ax2.grid()
ax2.plot(X,Y,'.',color=c1)

if(GENERATETIKZ):
    tikz_save("mag_turnon.tex")

plt.show()
