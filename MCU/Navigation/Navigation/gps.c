// GPS U-blox NEO M6 library for Texas Instruments TIVA C
// written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
// last update: 23/07/2019

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/fpu.h"
#include "utils/uartstdio.h"

#include "gps.h"

//===================================================
// DEFINES

#define R   6371000.0  // [m] Earth mean radius

//===================================================
// VARIABLES

volatile char data[26][12];                             // string block with GGA/VTG data
volatile char time[8];                                  // GPS time data
volatile double latitude = 0, longitude = 0;            // [deg] GPS position coordinates
volatile double altitude = 0;                           // [m]   GPS altitude data
volatile double lat0 = 0, lon0 = 0;                     // [deg] last GPS position coordinates
volatile double alt0 = 0;                               // [m]   last GPS altitude data
volatile double groundspeed = 0;                        // [m/s] ground speed
volatile double distance = 0;                           // [m]   distance between two gps points
volatile double bearing = 0;                            // [rad] direction angle from North
volatile double vN = 0, vE = 0, vD = 0;                 // [m/s] geographic speeds
volatile double dN = 0, dE = 0, dD = 0;                 // [m/s] geographic displacement

//===================================================
// FUNCTIONS

// initial setup
void InitGPS(){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); //blink green LED on
    // wait for valid coordinate data
    while(abs(lat0)>90 || abs(lon0)>180 || !lat0 || !lon0){
        lat0 = Latitude(data);
        lon0 = Longitude(data);
        alt0 = Altitude(data);
        SysCtlDelay(SysCtlClockGet() / (10 * 3)); //delay ~1e-10 msec
    }
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); //blink green LED off
    UpdateDataGPS();
}

// get new data
void UpdateDataGPS(){
    latitude = Latitude(data);
    longitude = Longitude(data);
    altitude = Altitude(data);
    groundspeed = GroundSpeed(data);
    // latitude must have values between -90 and 90 deg
    // longitude must have values between -180 and 180 deg
    if((abs(latitude)>1e-5 && abs(latitude)<=90) && (abs(longitude)>1e-5 && abs(longitude)<=180)){
        // if there was no change in coordinate data, there is no displacement
        if(lat0==latitude && lon0==longitude){
            distance = 0;
            bearing = 0;
            dN = 0;
            dE = 0;
            dD = 0;

            vN = 0;
            vE = 0;
            vD = 0;
        // if there is displacement, it shouldn't be ridiculous big
        // this piece of code ensures that only short displacements are considered at each step
        }else if(abs(lat0-latitude)<1e-2 && abs(lat0-latitude)<1e-2){
            distance = HaversineFormula(lat0,lon0,latitude,longitude);
            bearing = Bearing(lat0,lon0,latitude,longitude)*M_PI/180;
            lat0 = latitude;
            lon0 = longitude;

            // if distance is calculated, it can be decomposed into three components:
            // north, east and down
            if(distance){
                double c = cos(bearing);
                double s = sin(bearing);

                dN = distance*c;
                dE = distance*s;
                dD = altitude-alt0;
                alt0 = altitude;

                vN = groundspeed*c;
                vE = groundspeed*s;
                vD = groundspeed*dD/distance;

            }else{
                distance = 0;
                bearing = 0;
                dN = 0;
                dE = 0;
                dD = 0;

                vN = 0;
                vE = 0;
                vD = 0;
            }
        }
    }
}

// write UTC time string inside t
void Time(char t[8], char data[][12]){
    t[0]=data[10][0];
    t[1]=data[10][1];
    t[2]=':';
    t[3]=data[10][2];
    t[4]=data[10][3];
    t[5]=':';
    t[6]=data[10][4];
    t[7]=data[10][5];
}

// returns double value for latitude
double Latitude(char data[][12]){
    char deg_str[2];
    deg_str[0] = data[11][0];
    deg_str[1] = data[11][1];

    char min_str[6];
    min_str[0] = data[11][2];
    min_str[1] = data[11][3];
    min_str[2] = data[11][4];
    min_str[3] = data[11][5];
    min_str[4] = data[11][6];
    min_str[5] = data[11][7];

    double deg = atoi(deg_str);
    double min = atof(min_str);
    double l = deg + min*0.01666667;
    if(data[12][0]=='S'){
        l=-l;
    }
    return l;
}

// returns double value for longitude
double Longitude(char data[][12]){
    char deg_str[3];
    deg_str[0] = data[13][0];
    deg_str[1] = data[13][1];
    deg_str[2] = data[13][2];

    char min_str[6];
    min_str[0] = data[13][3];
    min_str[1] = data[13][4];
    min_str[2] = data[13][5];
    min_str[3] = data[13][6];
    min_str[4] = data[13][7];
    min_str[5] = data[13][8];

    double deg = atoi(deg_str);
    double min = atof(min_str);
    double l = deg + min*0.01666667;
    if(data[14][0]=='W'){
        l=-l;
    }
    return l;
}

// returns double value for altitude
double Altitude(char data[][12]){
    return atof(data[18]);
}

// returns double value for ground speed in m/s
double GroundSpeed(char data[][12]){
    return atof(data[7])/3.6;
}

// returns double value for Bearing in deg
double Bearing(double lat1,double lon1,double lat2,double lon2){
    lat1 *= M_PI/180.0;
    lon1 *= M_PI/180.0;
    lat2 *= M_PI/180.0;
    lon2 *= M_PI/180.0;
    double dlon = lon2-lon1;
    double x = sin(dlon)*cos(lat2);
    double y = cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(dlon);
    double B = atan2(x,y)*180.0/M_PI;
    B = fmod(B+360,360);
    return B;
}

// haversine function
double hav(double t){
    return 0.5 - cos(t)*0.5;
}

// returns double value for distance between two coordinates in meters
double HaversineFormula(double lat1, double lon1, double lat2, double lon2){
    lat1 *= M_PI/180.0;
    lon1 *= M_PI/180.0;
    lat2 *= M_PI/180.0;
    lon2 *= M_PI/180.0;
    double dlat = lat2-lat1;
    double dlon = lon2-lon1;
    double h = hav(dlat) + cos(lat1)*cos(lat2)*hav(dlon);
    double d = 2*R*asin(sqrt(h));
    return d;
}
