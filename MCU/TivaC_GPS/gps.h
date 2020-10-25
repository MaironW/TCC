// GPS U-blox NEO M6 library for Texas Instruments TIVA C
// written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
// last update: 04/06/2019


#ifndef GPS_H_
#define GPS_H_

//===================================================
// VARIABLES

extern char data[26][12];                               // string block with GGA data
char time[8];                                           // GPS time data
extern volatile double latitude,longitude,altitude;      // [deg] GPS position coordinates
extern volatile double groundspeed;                      // [m/s] ground speed
extern volatile double distance;                         // [m]   distance between two gps points
extern volatile double bearing;                          // [rad] direction angle from North
extern volatile double vN, vE, vD;                       // [m/s] geographic speeds
extern volatile double dN, dE, dD;                       // [m/s] geographic displacement

//===================================================
// HEADERS

// write UTC time string inside t
void Time(char t[8], char data[][12]);

// returns double value for latitude
double Latitude(char data[][12]);

// returns double value for longitude
double Longitude(char data[][12]);

// returns double value for altitude
double Altitude(char data[][12]);

// returns double value for ground speed in m/s
double GroundSpeed(char data[][12]);

// returns double value for Bearing in rad
double Bearing(double lat1, double lat2, double lon1, double lon2);

// returns double value for distance between two coordinates in meters
double HaversineFormula(double lat1, double lat2, double lon1, double lon2);

#endif
