#include <Arduino.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

//===================================================
// DEFINES

#define LED_BUILTIN 2
#define R   6371000.0  // [m] Earth mean radius
#define RXD2 16
#define TXD2 17

#define DEGTORAD 0.01745329252
#define RADTODEG 57.29577951

#define PRECISION 6

void UpdateInfo();

//===================================================
// VARIABLES

TaskHandle_t Task0;

double lat0=0.0f, lon0=0.0f, alt0=0.0f; // [deg] old GPS position coordinates
double distance=0.0f;                   // [m]   distance between two gps points
double bearing=0.0f;                    // [rad] direction angle from North
double vN=0.0f, vE=0.0f, vD=0.0f;       // [m/s] geographic speeds
double dN=0.0f, dE=0.0f, dD=0.0f;       // [m/s] geographic displacement

String dmessage = "";
String vmessage  = "";
String coordmessage = "";

// The TinyGPS++ object
TinyGPSPlus gps;  

HardwareSerial gps_serial(2);

void codeForTask0( void * parameter )
{
  for (;;) {
    while(gps_serial.available()>0){
      byte gpsData = gps_serial.read();
      if(gps.encode(gpsData)){
        if(gps.location.isUpdated()){
          UpdateInfo();
        }
      }
    // Serial.write(gpsData); //output serial data
    }
  }
}

void setup() {
  Serial.begin(115200);
  gps_serial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
  
  Serial.println("ESP32 GPS");
  Serial.println("Calculate geographic displacement and speed");
  Serial.println();

  while(!lat0 or !lon0 or !alt0){
    while (gps_serial.available() > 0){
      byte gpsData = gps_serial.read();
      if(gps.encode(gpsData)){
        if(gps.location.isUpdated()){
          lat0 = gps.location.lat();
          lon0 = gps.location.lng();
          alt0 = gps.altitude.meters();
          Serial.println("waiting gps...");
        }
      }
    }
  }

  xTaskCreatePinnedToCore(
    codeForTask0,
    "GPStask",
    8192,
    NULL,
    1,
    &Task0,
    1);

}

void loop(){
  delay(1000);
}

// returns double value for Bearing in radians
double Bearing(double lat1,double lon1,double lat2,double lon2){
    lat1 *= DEGTORAD;
    lon1 *= DEGTORAD;
    lat2 *= DEGTORAD;
    lon2 *= DEGTORAD;
    double dlon = lon2-lon1;
    double x = sin(dlon)*cos(lat2);
    double y = cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(dlon);
    double B = atan2(x,y);
    B = fmod(B+2*PI,2*PI);
    return B;
}

// haversine function
double hav(double t){
    return 0.5 - cos(t)*0.5;
}

// returns double value for distance between two coordinates in meters
double HaversineFormula(double lat1, double lon1, double lat2, double lon2){
    lat1 *= DEGTORAD;
    lon1 *= DEGTORAD;
    lat2 *= DEGTORAD;
    lon2 *= DEGTORAD;
    double dlat = lat2-lat1;
    double dlon = lon2-lon1;
    double h = hav(dlat) + cos(lat1)*cos(lat2)*hav(dlon);
    double d = 2*R*asin(sqrt(h));
    return d;
}

// convert displacement in three NED coordinates
void toNED(double distance, double bearing){
  if(distance){
    double c = cos(bearing);
    double s = sin(bearing);

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

void UpdateInfo(){
  if(gps.location.lat()!=0.0 && gps.location.lng()!=0.0){
    distance = gps.distanceBetween(lat0,lon0, gps.location.lat(), gps.location.lng());

    bearing = Bearing(lat0, lon0, gps.location.lat(), gps.location.lng());
//      bearing = gps.course_to(lat0, lon0, gps.location.lat(), gps.location.lng());
    toNED(distance,bearing);
    lat0 = gps.location.lat();
    lon0 = gps.location.lng();
    alt0 = gps.altitude.meters();  

    dmessage = String(dN,PRECISION)+','+String(dE,PRECISION)+','+String(dD,PRECISION);
    vmessage = String(vN,PRECISION)+','+String(vE,PRECISION)+','+String(vD,PRECISION);
    coordmessage = String(gps.location.lat(),PRECISION)+','+String(gps.location.lng(),PRECISION)+','+String(gps.altitude.meters(),PRECISION);
    Serial.println(coordmessage);
  } 
}
