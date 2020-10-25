// GPS processing library for Texas Instruments TIVA C
// GPS data is preprocessed by an Arduino Nano and sent here through UART
// written by: Mairon de Souza Wolniewicz at Universidade Federal de Santa Catarina
// last update: 10/09/2019

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

volatile char *data[10];                                // string block with GPS data
volatile double latitude = 0, longitude = 0;            // [deg] GPS position coordinates
volatile double altitude = 0;                           // [m]   GPS altitude data
volatile double dN = 0, dE = 0, dD = 0;                 // [m]   geographic displacement
volatile double vN = 0, vE = 0, vD = 0;                 // [m/s] geographic speeds

//===================================================
// FUNCTIONS

// get new GPS data
void UpdateDataGPS(){
    if(atof((char*)data[1])!=0 && atof((char*)data[2])!=0){
        latitude = atof((char*)data[1]);
            longitude = atof((char*)data[2]);
            altitude = atof((char*)data[3]);
            dN = dN + atof((char*)data[4]);
            dE = dE + atof((char*)data[5]);
            dD = dD + atof((char*)data[6]);
            vN = vN + atof((char*)data[7]);
            vE = vE + atof((char*)data[8]);
            vD = vD + atof((char*)data[9]);
    }

}
