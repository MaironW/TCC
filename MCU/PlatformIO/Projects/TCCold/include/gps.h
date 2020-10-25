#ifndef GPS_H
#define GPS_H

// ----------------------------------------
// DEFINES

#define DEGTORAD    0.01745329252
#define RADTODEG    57.29577951
#define EARTH_RAD   6371000.0  // [m] EaEARTH_rADradius

// ----------------------------------------
// DECLARATIONS

extern volatile float lat0, lon0, alt0;  // [deg] old GPS position coordinates
extern volatile float distance;          // [m]   distance between two gps points
extern volatile float bearing;           // [rad] direction angle from North
extern volatile float vN, vE, vD;        // [m/s] geographic speeds
extern volatile float dN, dE, dD;        // [m/s] geographic displacement

// ----------------------------------------
// FUNCTIONS

// returns float value for Bearing in radians
float Bearing(float lat1,float lon1,float lat2,float lon2);

// haversine function
float hav(float t);

// returns float value for distance between two coordinates in meters
float HaversineFormula(float lat1, float lon1, float lat2, float lon2);

// convert displacement in three NED coordinates
void toNED(float distance, float bearing);

// calculates distance and bearing from GPS
void GPSUpdateInfo(); 

// wait for the first GPS valid data
void WaitGPSData();

# endif