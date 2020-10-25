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
        try:
            line = line.decode("utf-8") #ser.readline returns a binary, convert to string
            output_file.write(line)
            print count,line,
            count+=1;
            if(count==600):
                break
        except:
            0
            
output_file.close()


dados = np.loadtxt("sensor.dat",unpack=True,delimiter=',')

dN = dados[0]
dE = dados[1]
dD = dados[2]
vN = dados[3]
vE = dados[4]
vD = dados[5]
aN = dados[6]
aE = dados[7]
aD = dados[8]

dN_var = np.var(dN)
dE_var = np.var(dE)
dD_var = np.var(dD)
vN_var = np.var(vN)
vE_var = np.var(vE)
vD_var = np.var(vD)
aN_var = np.var(aN)
aE_var = np.var(aE)
aD_var = np.var(aD)


print 'd: ', dN_var,dE_var,dD_var
print 'v: ', vN_var,vE_var,vD_var
print 'a: ', aN_var,aE_var,aD_var
