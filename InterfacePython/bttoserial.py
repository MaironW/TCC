# reads bluetooth SPP data and writes it on a virtual serial port
# written by Mairon S. Wolniewicz
# last update: 24/04/2020

# run in bash:
#
#   git clone https://github.com/freemed/tty0tty
#   tar xf tty0tty-1.2.tgz
#   cd tty0tty-1.2/module

# write inside tty0tty.c
#
#   #include <linux/version.h>
#   #if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#   #include <linux/sched/signal.h>
#   #endif

# run in bash:
#
#   sudo modprobe tty0tty
#       modprobe: FATAL: Module tty0tty not found in directory /lib/modules/<DIRECTORY>    
#   make (cant have spaces in directory name to work)
#   sudo cp tty0tty.ko /lib/modules/<DIRECTORY>/kernel/drivers/misc/
#   sudo depmod
#   sudo modprobe tty0tty
#   sudo chmod 666 /dev/tnt*

# AFTER PROPPER INSTALLATION
# you must run NAVSYS in Bluetooth mode to open /dev/tnt0 port
# then run this script with ESP32 plugged in

import os, pty, serial
from bluetooth import *

baud_rate = 115200
#baud_rate = 57600

serial_port = '/dev/tnt1'
ser = serial.Serial(serial_port, baud_rate)
    
#search for bluetooth devices
devices = discover_devices(lookup_names=True)
print(type(devices))
print("Devices found: %s" % len(devices))
 
for item in devices:
	print(item)
	
	#if ESP is found, connect
	
	if(item[1]=='ESP32_Mairon'):
		ser.write(('ESP 32 found\n'.encode()))
		
		macAddr = item[0]
		BTsocket=BluetoothSocket(RFCOMM)
		BTsocket.connect((macAddr, 1))
		
		ser.write(('ESP 32 connected\n'.encode()))
		
		#read messages from ESP
		while(1):
			retorno = BTsocket.recv(1152)
			if(retorno):
				# print(retorno)
				ser.write(retorno + b'\r\n')
		BTsocket.close()
		break
