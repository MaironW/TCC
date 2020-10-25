from bluetooth import *

write_to_file_path = "MPU.dat"
 
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

		#send messages to ESP
		#mensagem = 'run'
		#while(mensagem!='quit'):			
			#mensagem = raw_input('Envie sua mensagem: ')	
			#if(mensagem):
				#BTsocket.send(mensagem+'\n')
						
		#read messages from ESP
		print("found")
		while(1):
			#output_file = open(write_to_file_path, "w")
			retorno = BTsocket.recv(1152)
			if(retorno):
				print(retorno)
			line = retorno.decode("utf-8")
			#output_file.write(line)

		BTsocket.close()		
		break
