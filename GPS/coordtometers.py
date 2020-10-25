#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 06/06/2019
# Mairon de Souza Wolniewicz

import serial
from haversineformula import HaversineFormula
from bearing import Bearing
import numpy as np
from matplotlib import animation
import matplotlib.pyplot as plt

fig = plt.figure()
ax = plt.axes(xlim=(-50.112, -50.110), ylim=(-27.508, -27.504))
line, = ax.plot([], [],'m-', lw=1)

write_to_file_path1 = "gps_distance.csv"

baud_rate = 115200

lat=0
lon=0
dN = 0
dE = 0
dD = 0

try:
    serial_port = '/dev/ttyUSB0'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyUSB0"
except serial.serialutil.SerialException:
    serial_port = '/dev/ttyACM0'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyACM0"


X = [0]
Y = [0]
def init():
    line.set_data([], [])
    return line,

def animate(i):
    try:
        linha = ser.readline()
        linha = linha.decode("utf-8") #ser.readline returns a binary, convert to string
        # print linha
        # if(linha[1:3]=="M:"):
        #     l = linha.split('\t')
        #     lat = float(l[1])
        #     lon = float(l[2])
        #     bearing = float(l[3])
        #     dE = float(l[4])
        #     dN = float(l[5])
            # distance = float(l[4])
        l = linha.split(',')
        if(len(l)==3):
            distance = 0
            bearing = 0
            dE = float(l[1])
            dN = float(l[0])
            lat = 0
            lon = 0
            print dE,dN

                # if(abs(lat)>1e-6 and abs(lon)>1e-6):
                    # output_file1.write("%f\t%f\t%f\t%f\t%f\n" %(lat,lon,distance,dE,dN))

                    # distance = float(l[5])
                    # bearing = float(l[6])
            #
            #         c = np.sin(bearing)
            #         s = np.cos(bearing)
            #
            #         dN = distance*c
            #         dE = distance*s
            #
            #         print lat,lon,distance,bearing,dN,dE
            #
            #X.append(dE+X[-1])
            #Y.append(dN+Y[-1])
            X.append(dE)
            Y.append(dN)
            #

            #         output_file1.write("%f\t%f\t%f\t%f\t%f\t%f\n" %(lat,lon,bearing,distance,dN,dE))
            #
            line.set_data(X,Y)
        return line,
    except ValueError:
        return line,

output_file1 = open(write_to_file_path1, "w")
# while True:
#     try:
#         line = ser.readline()
#         line = line.decode("utf-8") #ser.readline returns a binary, convert to string
#         # print (line,len(line))
#         if(line[1:3]=="M:"):
#             l = line.split('\t')
#             lat = float(l[1])
#             lon = float(l[2])
#
#             if(abs(lat)>1e-5 and abs(lon)>1e-6):
#
#                 distance = float(l[5])
#                 bearing = float(l[6])
#
#
#             #     if(distance and abs(erro)<1e-2):
#             #         # -------------------------------------
#                 c = np.sin(bearing)
#                 s = np.cos(bearing)
#             #
#             #         vN = groundspeed*c
#             #         vE = groundspeed*s
#             #         vD = -groundspeed*altitude/distance
#             #
#                 dN += distance*c
#                 dE += distance*s
#             #         dD = -altitude
#             #         # -------------------------------------
#
#                 print lat,lon,distance,bearing,dN,dE
#                 output_file1.write("%f\t%f\n" %(dN,dE))
#     except ValueError:
#         a=1
#     except IndexError:
#         a=1

anim = animation.FuncAnimation(fig, animate, init_func=init,
                               frames=360, interval=1, blit=True,save_count=10)

plt.grid()
plt.show()
