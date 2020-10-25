#include "Arduino.h"
#include "MPU9250.h"
#include <BluetoothSerial.h>

// ----------------------------------------
// DECLARATIONS

#define LED_BUILTIN 2         // led register

BluetoothSerial ESP_BT;       // bt serial port
MPU9250 IMU(i2c0, 0x68);      // imu sensor object
int status;

String message = "";          // serial output message

void setup()
{
  Serial.println("\n---MAG CALIBRATION---\n");
  pinMode(LED_BUILTIN, OUTPUT); // configure LED as output
  digitalWrite(LED_BUILTIN, HIGH);

  ESP_BT.begin("ESP32_Mairon"); // setup Bluetooth Signal
  Serial.println("Bluetooth device is ready to pair");

  // serial to display data
  // Serial.begin(115200);
  Serial.begin(57600);
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
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
  // setting SRD to 19 for a 50 Hz update rate
  // IMU.setSrd(19);
  IMU.calibrateMagExternal();

  delay(20);
  digitalWrite(LED_BUILTIN, LOW);
}

#define RAD_TO_DEG 57.295779513082320876798154814105

void loop()
{
  // read the sensor
  IMU.readSensor();

  message = String(IMU._hx)+','+String(IMU._hy)+','+String(IMU._hz);
  Serial.println(message);
  ESP_BT.print(message+"\r\n");
  
  delay(10);
}