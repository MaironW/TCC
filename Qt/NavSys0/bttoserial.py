# reads bluetooth SPP data and writes it on a virtual serial port
# written by Mairon S. Wolniewicz
# last update: 17/03/2020

import os, pty, serial
from bluetooth import *

baud_rate = 115200

serial_port = '/dev/tnt1'
ser = serial.Serial(serial_port, baud_rate)
    
#search for bluetooth devices
devices = discover_devices(lookup_names=True)
print(type(devices))
print("Devices found: %s" % len(devices))
 
for item in devices:
	print(item)
	
	#if ESP is found, connect
	
	if(item[1]=="ESP32_Mairon"):
		ser.write("ESP 32 found\n")
		
		macAddr = item[0]
		BTsocket=BluetoothSocket(RFCOMM)
		BTsocket.connect((macAddr, 1))
		
		ser.write("ESP 32 connected\n")
		
		#read messages from ESP
		while(1):
			retorno = BTsocket.recv(150)
			if(retorno):
				print retorno
				ser.write(retorno)
				ser.write("\n")
		BTsocket.close()
		break
