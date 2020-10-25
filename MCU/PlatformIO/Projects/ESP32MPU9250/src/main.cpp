#include "Arduino.h"
#include "MPU9250.h"

MPU9250 IMU(i2c0, 0x68);
int status;

void setup()
{
  // serial to display data
  Serial.begin(115200);
  // Serial.begin(57600);
  while (!Serial)
  {
  }
  // start communication with IMU
  status = IMU.begin();
  if (status < 0)
  {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while (1)
    {
    }
  }
  // setting the accelerometer full scale range to +/-2G
  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G);
  // setting the gyroscope full scale range to +/-250 deg/s
  IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS);
  // setting DLPF bandwidth to 41 Hz
  // IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
  // setting SRD to 19 for a 50 Hz update rate
  // IMU.setSrd(19);
  // IMU.calibrateMag();
  IMU.calibrateMagExternal();
}

#define RAD_TO_DEG 57.295779513082320876798154814105

void loop()
{
  // read the sensor
  IMU.readSensor();

  Serial.println(String(IMU._hx,3)+','+String(IMU._hy,3)+','+String(IMU._hz,3));
  
  delay(20);
}

// ----------------------------------------------
// #include <Arduino.h>
// #include <BluetoothSerial.h>

// #include <MPU9250.h>
// #include <MadgwickAHRS.h>
// #include <Quaternions.h>

// #define RAD_TO_DEG 57.295779513082320876798154814105
// #define PRECISION 6

// #define LED_BUILTIN 2

// BluetoothSerial ESP_BT; //Object for Bluetooth

// String accelmessage = "";
// String gyromessage = "";
// String magmessage = "";
// String message = "";

// float start = 0.0;

// MPU9250 IMU(i2c0, 0x68);
// int status;


// void setup()
// {
//   // serial to display data
//   Serial.begin(115200);
//   while (!Serial){}

//   pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output

//   // start communication with IMU
//   status = IMU.begin();
//   if (status < 0){
//     Serial.println("IMU initialization unsuccessful");
//     Serial.println("Check IMU wiring or try cycling power");
//     Serial.print("Status: ");
//     Serial.println(status);
//     while (1){}
//   }else{ Serial.println("IMU initialization successful!"); }

//   // setting the accelerometer full scale range to +/-8G
//   IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G);
//   // setting the gyroscope full scale range to +/-500 deg/s
//   IMU.setGyroRange(MPU9250::GYRO_RANGE_2000DPS);
//   // setting DLPF bandwidth to 41 Hz
//   // IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
//   // calibrate accelerometer
//   IMU.calibrateAccel2();
//   // calibrate magnetometer with external bias and scale data
//   // you can change its values on MPU9250.h
//   IMU.calibrateMagExternal();


//   ESP_BT.begin("ESP32_Mairon"); //Name of your Bluetooth Signal
//   Serial.println("Bluetooth Device is Ready to Pair");
// }

// void loop()
// {

//   // read the sensor
//   IMU.readSensor();

//   delay(20);

//   accelmessage = String(IMU.getAccelY_mss(),PRECISION) + ',' + String(IMU.getAccelX_mss(),PRECISION) + ',' + String(-IMU.getAccelZ_mss(),PRECISION);
//   gyromessage = String(IMU.getGyroY_rads(),PRECISION) + ',' + String(IMU.getGyroX_rads(),PRECISION) + ',' + String(-IMU.getGyroZ_rads(),PRECISION);
//   magmessage = String(IMU.getMagY_uT(),PRECISION) + ',' + String(IMU.getMagX_uT(),PRECISION) + ',' + String(-IMU.getMagZ_uT(),PRECISION);
//   // magmessage = String(IMU.getMagX_uT(),PRECISION) + ',' + String(IMU.getMagY_uT(),PRECISION) + ',' + String(IMU.getMagZ_uT(),PRECISION);
  
//   MadgwickTimeStep((millis() - start)/1000); // calculate time step

//   // MadgwickAHRSupdateIMU(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
//   //                       IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss());
  
//   MadgwickAHRSupdate(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
//                      IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss(),
//                      IMU.getMagY_uT(), IMU.getMagX_uT(), -IMU.getMagZ_uT());
  

//   start = millis(); // update time start

//   GravityVector();
//   LinearAcceleration(IMU.getAccelY_mss(),IMU.getAccelX_mss(),IMU.getAccelZ_mss());
//   EulerAngles();
//   NEDAcceleration();
                                                                                                                                
//   // message = String(q0,PRECISION) + ',' + String(q1,PRECISION) + ',' + String(q2,PRECISION) + ',' + String(q3,PRECISION); 
//   // message = String(roll,PRECISION)+','+String(pitch,PRECISION)+','+String(yaw,PRECISION);
//   message = String(roll,PRECISION)+','+String(pitch,PRECISION)+','+String(yaw,PRECISION);

//   ESP_BT.print(message);
//   Serial.println(message);

// }
