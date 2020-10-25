import numpy as np
import serial

baud_rate = 115200

write_to_file = "sensor.dat"

try:
    serial_port = '/dev/ttyUSB0'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyUSB0"
except serial.serialutil.SerialException:
    serial_port = '/dev/ttyACM1'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyACM1"
except serial.serialutil.SerialException:
    serial_port = '/dev/ttyACM0'
    ser = serial.Serial(serial_port, baud_rate)
    print "/dev/ttyACM0"
    
count = 0;
output_file = open(write_to_file, "w")
while True:
        line = ser.readline()
        l = line.split(',')
        if(len(l)==3):
            try:
                line = line.decode("utf-8") #ser.readline returns a binary, convert to string
                output_file.write(line)
                print count,line,
                count+=1;
                if(count==1000):
                    break
            except:
                0
            
output_file.close()


dados = np.loadtxt("sensor.dat",unpack=True,delimiter=',')

aN = dados[0]
aE = dados[1]
aD = dados[2]

aN_var = np.std(aN)
aE_var = np.std(aE)
aD_var = np.std(aD)

print 'a: ', aN_var,aE_var,aD_var
