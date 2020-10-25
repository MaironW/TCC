// UFSC - Aerospace Engineering
// TCC - Navigation Systems
// Written by: Mairon de Souza Wolniewicz - 16250094
// last update: 10/09/2019

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "inc/hw_i2c.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include "driverlib/fpu.h"

#include "uartout.h"
#include "timer.h"
#include "MPU9250.h"
#include "gps.h"

//===================================================
// MAIN



int main(void){

    // Set the clocking to run directly from the external crystal/oscillator.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

    // setup onboard led pins as output
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // setup MPU9250 before UART interruptions
    InitMPU9250();
    UpdateDataMPU9250();        // get new data from MPU9250

    // setup UART console and GPS communication
    UARTsetup(115200);

    // setup float point unity
    FPULazyStackingEnable();
    FPUEnable();

    while(1){

        UpdateDataMPU9250();    // get new data from MPU9250

        int i;
        for(i=0;i<5;i++) MadgwickAHRS();         // calculate quaternions

        EulerAngles();          // calculate Euler Angles
//        GravityVector();        // calculate gravity direction
//        LinearAcceleration();   // calculate linear acceleration
//        NEDAcceleration();      // calculate NED acceleration

        char buffer[100];//
//                sprintf(buffer,"%d\t%d\t%d\t\n",acclXraw,acclYraw,acclZraw);
//                sprintf(buffer,"%d\t%d\t%d\t\n",gyroXraw,gyroYraw,gyroZraw);

                sprintf(buffer,"%8.3f\t%8.3f\t%8.3f\t\n",acclXscl,acclYscl,acclZscl);
//                sprintf(buffer,"%8f\t%8f\t%8f\t\n",gyroXscl,gyroYscl,gyroZscl);
//                sprintf(buffer,"M,%8.2f,%8.2f,%8.2f\n",magXscl,magYscl,magZscl);

//        sprintf(buffer,"%8.2f\t%8.2f\t%8.2f\t%8.2f\t\n",q0,q1,q2,q3);
//        sprintf(buffer,"%8.2f\t%8.2f\t%8.2f\t\n",q1,q2,q3);
//        sprintf(buffer,"%8.2f\t%8.2f\t%8.2f\t\n",roll,pitch,yaw);
//        sprintf(buffer,"%8.2f\t%8.2f\t%8.2f\t\n",gravX,gravY,gravZ);
//        sprintf(buffer,"%8.2f\t%8.2f\t%8.2f\t\n",acclXlin,acclYlin,acclZlin);
//        sprintf(buffer,"%8.3f\t%8.3f\t%8.3f\t\n",acclN,acclE,acclD);
//        sprintf(buffer,"%8.6f\t%8.6f\t%8.1f\t\n",latitude,longitude,altitude);
//        sprintf(buffer,"%8.2f\t%8.2f\t%8.2f\t\n",dN,dE,dD);
//        sprintf(buffer,"%8.2f\t%8.2f\t%8.2f\t\n",vN,vE,vD);
//        sprintf(buffer,"%8.6f\t%8.6f\t%8.3f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.3f\t\n",latitude,longitude,altitude,dN,dE,dD,vN,vE,vD);
//        sprintf(buffer,"%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6f\t\n",dN,dE,dD,vN,vE,vD,acclN,acclE,acclD,dt);
//        sprintf(buffer,"%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6.3f\t%6f\t%6f\t\n",dN,dE,dD,vN,vE,vD,acclN,acclE,acclD,latitude,longitude);
        UARTprintf("%s",buffer);
    }
}

