#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 29/04/2019
# Mairon de Souza Wolniewicz

import serial
from haversineformula import HaversineFormula
from bearing import Bearing
import numpy as np

# write_to_file_path1 = "gps_distance_bearing.csv"
write_to_file_path2 = "gps_distance.csv"

baud_rate = 115200

first = 1

groundspeed = 0;
distance = 0
bearing = 0
lat1 = 0
lon1 = 0
lat2 = 0
lon2 = 0

vN = 0
vE = 0
vD = 0

dN = 0
dE = 0
dD = 0

try:
    serial_port = '/dev/ttyACM0'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyACM0"
except serial.serialutil.SerialException:
    serial_port = '/dev/ttyACM1'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyACM1"

# output_file1 = open(write_to_file_path1, "w")
output_file2 = open(write_to_file_path2, "w")
while True:
    try:
        line = ser.readline()
        line = line.decode("utf-8") #ser.readline returns a binary, convert to string
        # print (line,len(line))
        if(line[1:3]=="M:"):
            l = line.split('\t')
            if(first==1):
                lat1 = float(l[1])
                lon1 = float(l[2])
                lat2 = lat1
                lon2 = lon1
                first = 0

            lat2 = float(l[1])
            lon2 = float(l[2])

            erro = ((lat1-lat2)**2 + (lon1-lon2)**2)**0.5

            altitude = float(l[3])
            groundspeed = float(l[4])

            distance = HaversineFormula(lat1,lon1,lat2,lon2)
            bearing = Bearing(lat1,lon1,lat2,lon2)

            lat1 = lat2
            lon1 = lon2

            if(distance and abs(erro)<1e-2):
                # -------------------------------------
                B = bearing*np.pi/180
                c = np.sin(B)
                s = np.cos(B)

                vN = groundspeed*c
                vE = groundspeed*s
                vD = -groundspeed*altitude/distance

                dN += distance*c
                dE += distance*s
                dD = -altitude
                # -------------------------------------

            # print "\033[2J\033[Hlat\t\tlon\t\talt\td\tB\terro\t\tvg\tvN\tvE\tvD\tdN\tdE\tdD"
            print "%f\t%f\t%.2f\t%.2f\t%.2f\t%f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f" %(lat2,lon2,altitude,distance,bearing,erro,groundspeed,vN,vE,vD,dN,dE,dD)
            # output_file1.write("%f\t%f\t%f\n" %(distance,bearing,groundspeed))
            output_file2.write("%f\t%f\n" %(dN,dE))
    except ValueError:
        a=1
    except IndexError:
        a=1
