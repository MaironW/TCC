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
TaskHandle_t TaskGPS;         // task for the GPS

BluetoothSerial ESP_BT;       // bt serial port
HardwareSerial gps_serial(2); // gps serial port
MPU9250 IMU(i2c0, 0x68);      // imu sensor object
TinyGPSPlus gps;              // TinyGPS++ object

Time IMUTime;
Time GPSTime;
// Time KalmanTime;
// Time MadgwickTime;
Time SDwriteTime;
double sdtime = 0;

float acclN_kf = 0.0, acclE_kf = 0.0, acclD_kf = 0.0;
uint32_t imucounter = 0;

// String sdmessage = "";          // serial output string
String btmessage = "";          // serial output string

Eigen::Vector2d x_est_N,x_est_E;

bool MadgwickIsConverged = false;
bool SDisReady = false;
bool GPSisValid = false;

// Output Variables
String sgps = "";     //  "lat,lon"
String sq = "";       //  "q0,q1,q2,q3"
String sdist = "";    //  "dN,dE"
String svel = "";     //  "vN,vE"
String saccl = "";    //  "aN,aE"
String sacclsum = ""; //  "accelsumN,accelsumE"
String syaw = "";     //  "yaw"
String sd_est = "";   //  "x_est_N(0),x_est_E(0)"
String sv_est = "";   //  "x_est_N(1),x_est_E(1)"

// ----------------------------------------
// MAIN FUNCTIONS

// core 0 code. Writes into SD card
void codeForGPSTask(void * parameter){
  for(;;){
    if(GPSisValid){
      while(gps_serial.available()>0){
        byte gpsData = gps_serial.read();
        if(gps.encode(gpsData)){
          if(gps.location.isUpdated()&&gps.NMEAType==1){
            GPSUpdateInfo();
            digitalWrite(LED_BUILTIN, LOW);
            sgps = String(gps.location.lat(),PRECISION)+','+String(gps.location.lng(),PRECISION);
            // Serial.println("\nGPS IS VALID\n");
          }
        }
        // Serial.write(gpsData); //output serial data
      }
    }else{
        GPSisValid = CheckGPSData();
    }
    vTaskDelay(5 / portTICK_PERIOD_MS);
  }
}

// core 0 code. Writes into SD card
void codeForTask0(void * parameter){
  int core0index = 0;
  double accelsumN = 0.0;
  double accelsumE = 0.0;
  for(;;){
    core0index++;
    accelsumN += acclN;
    accelsumE += acclE;

    sdtime += SDwriteTime.Update();
    
    if(sdtime>0.04){
      accelsumN /= core0index;
      accelsumE /= core0index;
      sacclsum = String(accelsumN,PRECISION)+','+String(accelsumE,PRECISION);

      digitalWrite(LED_BUILTIN, HIGH);
      logSDCard(sdist+','+svel+','+sacclsum+','+syaw+','+sgps+','+sd_est+','+sv_est+','+String(sdtime,4)+"\r\n");
      
      // attitude log
      // logSDCard(sacclsum+','+sq+','+String(sdtime,4)+"\r\n");

      // Serial.println(sdtime,PRECISION);
      sdtime=0;

      accelsumN = 0;
      accelsumE = 0;
      core0index = 0;

    }
    //  vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// the setup function runs once when you press reset or power the board
void setup(){
  
  // Serial.begin(57600); // setup serial monitor
  Serial.begin(115200); // setup serial monitor
  Serial.println("\n---MAIRON NAVSYS---\n");
  pinMode(LED_BUILTIN, OUTPUT); // configure LED as output
  digitalWrite(LED_BUILTIN, HIGH);

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

  IMU.setAccelRange(MPU9250::ACCEL_RANGE_2G); // accelerometer full scale range to +/-2G
  IMU.setGyroRange(MPU9250::GYRO_RANGE_250DPS); // gyroscope full scale range to +/-250 deg/s
  
  // setting DLPF bandwidth to 41 Hz
  IMU.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_5HZ);
  
  // setting data output rate to 100Hz = 1000/(1+srd)
  // IMU.setSrd(9);

  // IMU.calibrateAccel();
  IMU.calibrateAccel2(); // calibrate accelerometer
  IMU.calibrateMagExternal(); // calibrate magnetometer with external bias and scale data (values on MPU9250.h)

  // GPS setup
  gps_serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("GPS connection successful!");
  Serial.println();
  xTaskCreatePinnedToCore(codeForGPSTask,"GPSTask",8192,NULL,2,&TaskGPS,1); // setup GPS task

  // Madgwick pre alignment
  Serial.println("Waiting Madgwick to converge...");

  float start_heading = 1000;
  float delta_heading = 1000;
  
  while(MadgwickIsConverged == false){

    IMU.readSensor(); // read MPU9250 sensor
    delay(2);
    IMUTime.Update();
    
    // INSERT KALMAN FILTER HERE
    // !!!!!!!!!!!!!!!!!!!!!!!!!

    if(delta_heading<1e-4){
      MadgwickIsConverged = true;

      // DeclinationAlign(); // apply declination angle to mag measurements. Comment if using IMU mode only.
      
      Serial.println("Madgwick converged well!");
      digitalWrite(LED_BUILTIN, LOW);
    }else{
      digitalWrite(LED_BUILTIN, HIGH);
      MadgwickTimeUpdate(IMUTime.dt());
      
      // calculate attitude without magnetometer
      MadgwickAHRSupdateIMU(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
                        IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss());
      // calculate attitude with magnetometer
      // MadgwickAHRSupdate(IMU.getGyroY_rads(), IMU.getGyroX_rads(), -IMU.getGyroZ_rads(), 
                        // IMU.getAccelY_mss(), IMU.getAccelX_mss(), -IMU.getAccelZ_mss(),
                        // IMU.getMagY_uT(), IMU.getMagX_uT(), -IMU.getMagZ_uT());
            
      GravityVector(); // define a 3D vector for gravity
      LinearAcceleration(IMU.getAccelY_mss(),IMU.getAccelX_mss(),-IMU.getAccelZ_mss()); // remove gravity
      EulerAngles(); // calculate attitude in Euler angles form
      NEDAcceleration(); // calculate linear acceleration in NED axis

      delta_heading = abs(start_heading - yaw);
      start_heading = yaw;

      sgps = String(gps.location.lat(),PRECISION)+','+String(gps.location.lng(),PRECISION);
      sq = String(q0,PRECISION)+','+String(q1,PRECISION)+','+String(q2,PRECISION)+','+String(q3,PRECISION);

      Serial.println(sgps+','+sq); 
      ESP_BT.print(sgps+','+sq);
    }
  }

  KalmanFilterInit(KFP);
  Serial.println("Kalman Filter values initialized");

  // Setup SD Card reader
  if(InitSDCard()){  // if SD is properly initialized, activate core 0 task
    xTaskCreatePinnedToCore(codeForTask0,"SDTask",8192,NULL,0,&Task0,0); // setup core 0
    Serial.println("ESP32 Core 0 is active");
  }
  
}

// loop for core 1. Runs IMU, Madgwick, Kalman Filter and sends messages through serial/bluetooth
void loop(){
  // if(GPSisValid){
  //     while(gps_serial.available()>0){
  //       byte gpsData = gps_serial.read();
  //       if(gps.encode(gpsData)){
  //         if(gps.location.isUpdated()&&gps.NMEAType==1){
  //           GPSUpdateInfo();
  //           digitalWrite(LED_BUILTIN, LOW);
  //           // Serial.println("\nGPS IS VALID\n");
  //         }
  //       }
  //       // Serial.write(gpsData); //output serial data
  //     }
  //   }else{
  //       GPSisValid = CheckGPSData();
  //   }
    

  IMU.readSensor(); // read MPU9250 sensor
  // delay(2);
  IMUTime.Update();

  // INSERT KALMAN FILTER HERE
  // !!!!!!!!!!!!!!!!!!!!!!!!!

  MadgwickTimeUpdate(IMUTime.dt());

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

  // runs Kalman filter in one dimension a time
  x_est_N = Linear1DKalmanFilter(dN, vN, acclN, IMUTime.dt(), 'N'); // North Kalman Filter
  x_est_E = Linear1DKalmanFilter(dE, vE, acclE, IMUTime.dt(), 'E'); // East Kalman Filter

  // updates strings
  sq = String(q0,PRECISION)+','+String(q1,PRECISION)+','+String(q2,PRECISION)+','+String(q3,PRECISION);
  sdist = String(dN,PRECISION)+','+String(dE,PRECISION); 
  svel = String(vN,PRECISION)+','+String(vE,PRECISION);
  // saccl = String(acclN,PRECISION)+','+String(acclE,PRECISION);
  syaw = String(yaw,PRECISION);
  sd_est = String(x_est_N(0),PRECISION)+','+String(x_est_E(0),PRECISION);
  sv_est = String(x_est_N(1),PRECISION)+','+String(x_est_E(1),PRECISION);

  // updates output message for real time view
  btmessage = sgps+','+sq;

  // updates output for linear real time view
  // btmessage = sdist+','+'0'+','+svel+','+'0'+','+saccl+','+String(acclD,PRECISION)+','+sq+','+sgps;

  // output
  // Serial.println(sdist+','+svel+','+saccl+','+syaw+','+sgps+','+sx_est+','+String(IMUTime.dt(),4));
  // Serial.println(btmessage);
  ESP_BT.print(btmessage);  

  // Serial.println("N1: "+String(IMUTime.dt(),PRECISION));

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

// check for the first GPS valid data
bool CheckGPSData(){
  // Serial.println("waiting gps...");
  if(!lat0 or !lon0 or !alt0){
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
    return false;
  }else{
    return true;
  }
}