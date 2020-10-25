#include <Arduino.h>
#include <BluetoothSerial.h>
#include <HardwareSerial.h>
#include <MPU9250.h>
#include <MadgwickAHRS.h>
#include <Quaternions.h>
#include <TinyGPS++.h>
#include <gps.h>

// ----------------------------------------
// DEFINES

#define PRECISION   6   // serial output precision
#define LED_BUILTIN 2   // led register
#define RXD2        16  // RX2 register
#define TXD2        17  // TX2 register


// ----------------------------------------
// DECLARATIONS

TaskHandle_t Task0;           // task for core 0
BluetoothSerial ESP_BT;       // bt serial port
HardwareSerial gps_serial(2); // gps serial port
MPU9250 IMU(i2c0, 0x68);      // imu sensor object
TinyGPSPlus gps;              // TinyGPS++ object

String imumessage = "";       // serial imu output string
String gpsmessage = "";       // serial gps output string

float start = 0.0;            // [ms] time reference
float _dt = 0.0;               // [s] time step


float N=0.0, E=0.0, D=0.0;
float VN=0.0, VE=0.0, VD=0.0;

float conv_time = 100;

int conv_count=0;

// ----------------------------------------
// MAIN FUNCTIONS

// core 0 code. Runs GPS related stuff
void codeForTask0(void * parameter){
  for(;;){
    while(gps_serial.available()>0){
      byte gpsData = gps_serial.read();
      if(gps.encode(gpsData)){
        if(gps.location.isUpdated()){
          GPSUpdateInfo();
        }
      }
    // Serial.write(gpsData); //output serial data
    }
  }
}

// the setup function runs once when you press reset or power the board
void setup(){
  Serial.begin(115200); // setup serial monitor
  Serial.println("\n---MAIRON NAVSYS---\n");
  pinMode (LED_BUILTIN, OUTPUT); // configure LED as output

  xTaskCreatePinnedToCore(codeForTask0,"GPSTask",8192,NULL,1,&Task0,1); // setup core 0
  Serial.println("ESP32 Core 0 is active");

  ESP_BT.begin("ESP32_Mairon"); // setup Bluetooth Signal
  Serial.println("Bluetooth device is ready to pair");

  // MPU9250 setup
  if(IMU.begin()<0){
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    while (1){}
  }else{
    Serial.println("IMU initialization successful!");
  }

  IMU.setAccelRange(MPU9250::ACCEL_RANGE_8G); // accelerometer full scale range to +/-8G
  IMU.setGyroRange(MPU9250::GYRO_RANGE_2000DPS); // gyroscope full scale range to +/-500 deg/s
  IMU.calibrateAccel2(); // calibrate accelerometer
  IMU.calibrateMagExternal(); // calibrate magnetometer with external bias and scale data (values on MPU9250.h)

  // GPS setup
  gps_serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("GPS connection successful!");
  // GPSWaitData();

}

// loop for core 1. Runs IMU and Madgwick filter
void loop(){
  IMU.readSensor(); // read MPU9250 sensor
  delay(20);

  _dt = (millis() - start)/1000;

  MadgwickTimeStep(_dt); // write time step inside Madwick algorithm
  
  // calculate attitude without magnetometer
  // MadgwickAHRSupdateIMU(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
                        // IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss());
  
  // calculate attitude with magnetometer
  MadgwickAHRSupdate(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
                     IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss(),
                     IMU.getMagY_uT(), IMU.getMagX_uT(), -IMU.getMagZ_uT());

  start = millis(); // update time start

  // if(quat_conv==false){
  //   VerifyConvergence();
  //   conv_count++;
  //   conv_time = millis()/1000.0;
  // }

  // define a 3D vector for gravity
  GravityVector();

  // remove gravity from accel measurements
  LinearAcceleration(IMU.getAccelY_mss(),IMU.getAccelX_mss(),IMU.getAccelZ_mss());
  
  // calculate attitude in Euler angles form
  EulerAngles();

  // calculate linear acceleration in NED axis
  NEDAcceleration();

  imumessage = String(q0,PRECISION)+','+String(q1,PRECISION)+','+String(q2,PRECISION)+','+String(q3,PRECISION);

  // ESP_BT.print(imumessage+','+gpsmessage);
  Serial.println(imumessage);//+','+String(conv_time,PRECISION)+','+String(conv_count));
}

// ----------------------------------------
// GPS FUNCTIONS

// returns float value for Bearing in radians
float Bearing(float lat1,float lon1,float lat2,float lon2){
    lat1 *= DEGTORAD;
    lon1 *= DEGTORAD;
    lat2 *= DEGTORAD;
    lon2 *= DEGTORAD;
    float dlon = lon2-lon1;
    float x = sin(dlon)*cos(lat2);
    float y = cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(dlon);
    float B = atan2(x,y);
    B = fmod(B+2*PI,2*PI);
    return B;
}

// haversine function
float hav(float t){
    return 0.5 - cos(t)*0.5;
}

// returns float value for distance between two coordinates in meters
float HaversineFormula(float lat1, float lon1, float lat2, float lon2){
    lat1 *= DEGTORAD;
    lon1 *= DEGTORAD;
    lat2 *= DEGTORAD;
    lon2 *= DEGTORAD;
    float dlat = lat2-lat1;
    float dlon = lon2-lon1;
    float h = hav(dlat) + cos(lat1)*cos(lat2)*hav(dlon);
    float d = 2*R*asin(sqrt(h));
    return d;
}

// convert displacement in three NED coordinates
void toNED(float distance, float bearing){
  if(distance){
    float c = cos(bearing);
    float s = sin(bearing);

    dN = distance*c;
    dE = distance*s;
    dD = gps.altitude.meters()-alt0;
    
    vN = gps.speed.mps()*c;
    vE = gps.speed.mps()*s;
    vD = gps.speed.mps()*dD/distance;  
  }else{
    dN = 0;
    dE = 0;
    dD = 0;

    vN = 0;
    vE = 0;
    vD = 0; 
  }  
}

// calculates distance and bearing from GPS
void GPSUpdateInfo(){
  if(gps.location.lat()!=0.0 && gps.location.lng()!=0.0){
    distance = gps.distanceBetween(lat0,lon0, gps.location.lat(), gps.location.lng());

    bearing = Bearing(lat0, lon0, gps.location.lat(), gps.location.lng());
    // bearing = gps.course_to(lat0, lon0, gps.location.lat(), gps.location.lng());
    toNED(distance,bearing);
    lat0 = gps.location.lat();
    lon0 = gps.location.lng();
    alt0 = gps.altitude.meters();  

    // dmessage = String(dN,PRECISION)+','+String(dE,PRECISION)+','+String(dD,PRECISION);
    // vmessage = String(vN,PRECISION)+','+String(vE,PRECISION)+','+String(vD,PRECISION);
    gpsmessage = String(dN,PRECISION)+','+String(dE,PRECISION)+','+String(dD,PRECISION)+','+
                 String(vN,PRECISION)+','+String(vE,PRECISION)+','+String(vD,PRECISION);
  } 
}

// waits the GPS to receive data
void GPSWaitData(){
  while(!lat0 or !lon0 or !alt0){
    while (gps_serial.available() > 0){
      byte gpsData = gps_serial.read();
      if(gps.encode(gpsData)){
        if(gps.location.isUpdated()){
          lat0 = gps.location.lat();
          lon0 = gps.location.lng();
          alt0 = gps.altitude.meters();
          // Serial.println("waiting gps...");
        }
      }
    }
  }
  gpsmessage = "GPS data is ready!";
  // Serial.println("GPS data is ready!");
  // Serial.println();
}
