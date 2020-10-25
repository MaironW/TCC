from bluetooth import *
 
# search for bluetooth devices
devices = discover_devices(lookup_names=True)
print(type(devices))
print("Devices found: %s" % len(devices))
 
for item in devices:
	print(item)

	# if ESP is found, connect
	if(item[1]=="ESP32_Mairon"):
		macAddr = item[0]
		BTsocket=BluetoothSocket(RFCOMM)
		BTsocket.connect((macAddr, 1))

		# send messages to ESP
		mensagem = 'run'
		while(mensagem!='quit'):			
			mensagem = raw_input('Envie sua mensagem: ')	
			if(mensagem):
				BTsocket.send(mensagem+'\n')
						
		# read messages from ESP
		#while(1):
		#	retorno = BTsocket.recv(1024)
		#	if(retorno):
		#		print retorno

		BTsocket.close()		
		break
