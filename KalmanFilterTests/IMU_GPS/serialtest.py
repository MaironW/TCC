##
## Script listens to serial port and writes contents into a file
##
## requires pySerial to be installed
import serial

baud_rate = 115200
#baud_rate = 9600
write_to_file_path = "MPU.dat"
#write_to_file_2 = "sensor.dat"


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


#output_file = open(write_to_file_2, "w")
while True:
        output_file = open(write_to_file_path, "w")
        line = ser.readline()
        try:
            line = line.decode("utf-8") #ser.readline returns a binary, convert to string
            output_file.write(line)
            print line,
        except:
            0
    
