#include <Arduino.h>
#include <BluetoothSerial.h>
#include <HardwareSerial.h>
#include <MPU9250.h>
#include <MadgwickAHRS.h>

#include <TinyGPS++.h>
#include <gps.h>

#include <Eigen30.h>

#include <runtime.h>
#include <KalmanFilter.h>
#include <Quaternions.h>
#include <SDCard.h>

// ----------------------------------------
// DEFINES

#define PRECISION   6   // serial output precision
#define LED_BUILTIN 2   // led register
#define RXD2        16  // RX2 register
#define TXD2        17  // TX2 register

#define KFP         0.012 // [s] Kalman Filter period

// ----------------------------------------
// DECLARATIONS

TaskHandle_t Task0;           // task for core 0
BluetoothSerial ESP_BT;       // bt serial port
HardwareSerial gps_serial(2); // gps serial port
MPU9250 IMU(i2c0, 0x68);      // imu sensor object
TinyGPSPlus gps;              // TinyGPS++ object

Time IMUTime;
Time GPSTime;
Time KalmanTime;
Time MadgwickTime;

float KalmanPeriod = 0.0;

float acclN_kf = 0.0, acclE_kf = 0.0, acclD_kf = 0.0;
uint32_t imucounter = 0;

String sdmessage = "";          // serial output string
String btmessage = "";          // serial output string

Eigen::Vector2d x_est_N,x_est_E;

bool MadgwickIsConverged = false;

// ----------------------------------------
// MAIN FUNCTIONS

// core 0 code. Runs GPS related stuff and Kalman Filter
void codeForTask0(void * parameter){
  for(;;){
    while(gps_serial.available()>0){
      byte gpsData = gps_serial.read();
      if(gps.encode(gpsData)){
        if(gps.location.isUpdated()&&gps.NMEAType==1){
          GPSUpdateInfo();
        }
      }
      // Serial.write(gpsData); //output serial data
    }

    btmessage = String(gps.location.lat(),PRECISION)+','+String(gps.location.lng(),PRECISION)+','+
                       String(q0,PRECISION)+','+String(q1,PRECISION)+','+String(q2,PRECISION)+','+String(q3,PRECISION); 

    if(MadgwickIsConverged==true){
      KalmanPeriod+=KalmanTime.Update();

      // acclN_kf+=acclN;
      // acclE_kf+=acclE;
      // acclD_kf+=acclD;
      // imucounter++;

      if(KalmanPeriod>0.01){

        // acclN_kf/=imucounter;
        // acclE_kf/=imucounter;
        // acclD_kf/=imucounter;

        // x_est_N = Linear1DKalmanFilter(dN, vN, acclN_kf, 'N'); // North Kalman Filter
        // x_est_E = Linear1DKalmanFilter(dE, vE, acclE_kf, 'E'); // East Kalman Filter
        x_est_N = Linear1DKalmanFilter(dN, vN, acclN, KalmanPeriod, 'N'); // North Kalman Filter
        x_est_E = Linear1DKalmanFilter(dE, vE, acclE, KalmanPeriod, 'E'); // East Kalman Filter

        KalmanPeriod = 0.0;

        sdmessage = String(dN,PRECISION)+','+String(dE,PRECISION)+','+
                String(vN,PRECISION)+','+String(vE,PRECISION)+','+
                String(acclN,PRECISION)+','+String(acclE,PRECISION)+','+
                String(yaw,PRECISION)+','+
                String(gps.location.lat(),PRECISION)+','+String(gps.location.lng(),PRECISION)+','+
                String(x_est_N(0),PRECISION)+','+String(x_est_E(0),PRECISION);

        logSDCard(sdmessage+"\r\n");


        // acclN_kf = 0.0;
        // acclE_kf = 0.0;
        // acclD_kf = 0.0;
        // imucounter = 0;

      }
  

    }
  }
}

// the setup function runs once when you press reset or power the board
void setup(){
  
  Serial.begin(57600); // setup serial monitor
  // Serial.begin(115200); // setup serial monitor
  Serial.println("\n---MAIRON NAVSYS---\n");
  pinMode(LED_BUILTIN, OUTPUT); // configure LED as output
  digitalWrite(LED_BUILTIN, HIGH);

  xTaskCreatePinnedToCore(codeForTask0,"GPSTask",8192,NULL,1,&Task0,1); // setup core 0
  Serial.println("ESP32 Core 0 is active");

  ESP_BT.begin("ESP32_Mairon"); // setup Bluetooth Signal
  Serial.println("Bluetooth device is ready to pair");

  InitSDCard(); // Setup SD Card reader

  // MPU9250 setup
  if(IMU.begin()<0){
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    while (1){}
  }else{
    Serial.println("IMU initialization successful!");
  }

  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G); // accelerometer full scale range to +/-2G
  IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS); // gyroscope full scale range to +/-250 deg/s
  
  // setting DLPF bandwidth to 41 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_41HZ);
  
  // setting data output rate to 100Hz = 1000/(1+srd)
  // IMU.setSrd(9);

  // IMU.calibrateAccel();
  IMU.calibrateAccel2(); // calibrate accelerometer
  IMU.calibrateMagExternal(); // calibrate magnetometer with external bias and scale data (values on MPU9250.h)

  // GPS setup
  gps_serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("GPS connection successful!");
  // WaitGPSData(); // comment this line if you dont need propper gps signal
  Serial.println("GPS data is ready!");
  Serial.println();

  // Madgwick pre alignment
  Serial.println("Waiting Madgwick to converge...");

  float start_heading = 1000;
  float delta_heading = 1000;
  
  while(MadgwickIsConverged == false){

    IMU.readSensor(); // read MPU9250 sensor
    delay(2);
    IMUTime.Update();

    if(delta_heading<1e-4){
      MadgwickIsConverged = true;

      DeclinationAlign(); // apply declination angle to mag measurements. Comment if using IMU mode only.
      
      Serial.println("Madgwick converged well!");
      digitalWrite(LED_BUILTIN, LOW);
    }else{
      MadgwickTimeUpdate(MadgwickTime.Update());
      
      // calculate attitude without magnetometer
      // MadgwickAHRSupdateIMU(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
                        // IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss());
      // calculate attitude with magnetometer
      MadgwickAHRSupdate(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
                        IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss(),
                        IMU.getMagY_uT(), IMU.getMagX_uT(), -IMU.getMagZ_uT());
            
      GravityVector(); // define a 3D vector for gravity
      LinearAcceleration(IMU.getAccelY_mss(),IMU.getAccelX_mss(),-IMU.getAccelZ_mss()); // remove gravity
      EulerAngles(); // calculate attitude in Euler angles form
      NEDAcceleration(); // calculate linear acceleration in NED axis

      delta_heading = abs(start_heading - yaw);
      start_heading = yaw;

      Serial.println(String(gps.location.lat(),PRECISION)+','+String(gps.location.lng(),PRECISION)+','+
                    String(q0,PRECISION)+','+String(q1,PRECISION)+','+String(q2,PRECISION)+','+String(q3,PRECISION)); 
      ESP_BT.print(String(gps.location.lat(),PRECISION)+','+String(gps.location.lng(),PRECISION)+','+
                    String(q0,PRECISION)+','+String(q1,PRECISION)+','+String(q2,PRECISION)+','+String(q3,PRECISION));
    }
  }

  KalmanFilterInit(KFP);
}

// loop for core 1. Runs IMU and Madgwick filter
void loop(){
  IMU.readSensor(); // read MPU9250 sensor
  // delay(2);

  IMUTime.Update();

  MadgwickTimeUpdate(MadgwickTime.Update());

  // calculate attitude without magnetometer
  MadgwickAHRSupdateIMU(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
                        IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss());

  // calculate attitude with magnetometer
  // MadgwickAHRSupdate(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
  //                 IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss(),
  //                 IMU.getMagY_uT(), IMU.getMagX_uT(), -IMU.getMagZ_uT());

    
  // define a 3D vector for gravity
  GravityVector();

  // remove gravity from accel measurements
  LinearAcceleration(IMU.getAccelY_mss(),IMU.getAccelX_mss(),-IMU.getAccelZ_mss());
  
  // calculate attitude in Euler angles form
  EulerAngles();

  // calculate linear acceleration in NED axis
  NEDAcceleration();

  // Serial.println(String(IMUTime.dt(),PRECISION));
  Serial.println(btmessage);
  // ESP_BT.print(btmessage);

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
    float d = 2*EARTH_RAD*asin(sqrt(h));
    return d;
}

// convert displacement in three NED coordinates
void toNED(float distance, float bearing){
  if(distance){
    float c = cos(bearing);
    float s = sin(bearing);

    dN = dN + distance*c;
    dE = dE + distance*s;
    dD = gps.altitude.meters()-alt0;
    
    vN = gps.speed.mps()*c;
    vE = gps.speed.mps()*s;
    vD = gps.speed.mps()*dD/distance;  
  }else{
    dN = dN + 0;
    dE = dE + 0;
    dD = 0;

    vN = 0;
    vE = 0;
    vD = 0; 
  }  
}

// calculates distance and bearing from GPS
void GPSUpdateInfo(){
  if(gps.location.lat()!=0.0 && gps.location.lng()!=0.0){
    // distance = gps.distanceBetween(lat0,lon0, gps.location.lat(), gps.location.lng());
    distance = HaversineFormula(lat0,lon0, gps.location.lat(), gps.location.lng());
    if(distance<1000){
      bearing = Bearing(lat0, lon0, gps.location.lat(), gps.location.lng());
      // bearing = gps.course_to(lat0, lon0, gps.location.lat(), gps.location.lng());
      toNED(distance,bearing);
      lat0 = gps.location.lat();
      lon0 = gps.location.lng();
      alt0 = gps.altitude.meters();  

    }
  } 
}

// wait for the first GPS valid data
void WaitGPSData(){
  Serial.println("waiting gps...");
  while(!lat0 or !lon0 or !alt0){
    while (gps_serial.available() > 0){
      byte gpsData = gps_serial.read();
      if(gps.encode(gpsData)){
        if(gps.location.isUpdated()){
          lat0 = gps.location.lat();
          lon0 = gps.location.lng();
          alt0 = gps.altitude.meters();
        }
      }
    }
  }
}