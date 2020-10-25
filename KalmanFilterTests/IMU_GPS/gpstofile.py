#!/usr/bin/env python
# -*- coding: utf-8 -*-

# 25/07/2019
# Mairon de Souza Wolniewicz

import serial


path = "gpsdata.dat"
output_file = open(path,"w")

baud_rate = 115200

try:
    serial_port = '/dev/ttyACM0'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyACM0"
except serial.serialutil.SerialException:
    serial_port = '/dev/ttyACM1'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyACM1"

while(1):
    linha = ser.readline()
    # linha = linha.decode("utf-8")
    # print linha
    l = linha.split('\t')

    if(abs(l[0]>10) and abs(l[1]>10)):
        print linha
        output_file.write(linha)


