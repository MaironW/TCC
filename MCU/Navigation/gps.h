// GPS U-blox NEO M6 library for Texas Instruments TIVA C
// written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
// last update: 10/09/2019

#ifndef GPS_H_
#define GPS_H_

//===================================================
// VARIABLES

extern volatile char *data[10];                          // string block with GPS data
extern volatile double latitude, longitude;              // [deg] GPS position coordinates
extern volatile double altitude;                         // [m]   GPS altitude data
extern volatile double dN, dE, dD;                       // [m]   geographic displacement
extern volatile double vN, vE, vD;                       // [m/s] geographic speeds

//===================================================
// HEADERS

// get new GPS data
void UpdateDataGPS();

#endif
