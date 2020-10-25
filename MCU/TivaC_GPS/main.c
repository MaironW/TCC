// GPS U-blox NEO M6 on Texas Instrument Tiva C
// written by: Mairon de Souza Wolniewicz
// last update: 14/06/2019

// displays latitude, longitude, velocity and displacement in NED reference

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

#include "uartout.h"
#include "gps.h"

int main(void)
{
    // setup float point unity
    FPULazyStackingEnable();
    FPUEnable();

    char buffer[100];   // buffer to print on screen

    // [deg] last GPS position update
    double lat0 = 0;
    double lon0 = 0;

    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // Set UART to be as fast as it can
    UARTsetup(115200);

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3); //blink green LED on
    // wait for valid coordinate data
    while(abs(lat0)>90 || abs(lon0)>180 || !lat0 || !lon0){
        lat0 = Latitude(data);
        lon0 = Longitude(data);
        SysCtlDelay(SysCtlClockGet() / (10 * 3)); //delay ~1e-10 msec
    }
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0); //blink green LED off

    while(1){
        Time(time,data);
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
                vN = 0;
                vE = 0;
                vD = 0;

                dN = 0;
                dE = 0;
                dD = 0;
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

                    vN = groundspeed*c;
                    vE = groundspeed*s;
                    vD = -groundspeed*altitude/distance;

                    dN = distance*c;
                    dE = distance*s;
                    dD = -altitude;
                }else{
                    distance = 0;
                    bearing = 0;
                    vN = 0;
                    vE = 0;
                    vD = 0;

                    dN = 0;
                    dE = 0;
                    dD = 0;
                }
            }
        sprintf(buffer,"M:\t%15f\t%15f\t%12f\t%12f\t%12f\t%12f\n",latitude,longitude,bearing,dE,dN,-altitude);
//        sprintf(buffer,"M:\t%12f\t%12f\t%12f\t%12f\t%12f\t%12f\n",latitude,longitude,altitude,groundspeed,distance,bearing);
        UART0Send((uint8_t *)buffer,88);
        }
    }
}
