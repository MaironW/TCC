##
## Script listens to serial port and writes contents into a file
##
## requires pySerial to be installed
import serial
from serial.tools import list_ports
import os.path

baud_rate = 115200
#baud_rate = 57600
write_to_file_path = "MPU.dat"
write_to_file_2 = "rawdata.dat"

ports = list(list_ports.comports())
print([port.name for port in ports])

# try:
serial_port = '/dev/ttyUSB0'
ser = serial.Serial(serial_port, baud_rate)
print('/dev/ttyUSB0')
# except serial.serialutil.SerialException:
    # serial_port = '/dev/ttyACM1'
    # ser = serial.Serial(serial_port, baud_rate)
    # print('/dev/ttyACM1')
# except serial.serialutil.SerialException:
    # serial_port = '/dev/ttyACM0'
    # ser = serial.Serial(serial_port, baud_rate)
    # print('/dev/ttyACM0')
# except:
    # 0
    

#fileindex = 0
#write_to_file_change = "dat"+str(fileindex)+".dat"
#while(os.path.exists(write_to_file_change)):
    #write_to_file_change = "dat"+str(fileindex)+".dat"
    #fileindex=fileindex+1
#print write_to_file_change
#output_file = open(write_to_file_change, "w")
while True:
        #output_file = open(write_to_file_path, "w")
        line = ser.readline()
        try:
            line = line.decode("utf-8") #ser.readline returns a binary, convert to string           
            #output_file.write(line)
            print(line,end='') # python3
            #print line, # python2
        except:
            0

