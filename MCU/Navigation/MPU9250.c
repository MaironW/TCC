// MPU9250 library for Tiva C MCU
// written by: Mairon de Souza Wolniewicz
// last update: 17/06/2019

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

#include "i2c1.h"
#include "timer.h"

#include "MadgwickAHRS.h"
#include "MPU9250.h"

//===================================================
// DEFINES

#define gravity 9.80665

//===================================================
// VARIABLES

volatile int16_t acclXraw = 0, acclYraw = 0, acclZraw = 0;         // accelerometer raw data
volatile int16_t gyroXraw = 0, gyroYraw = 0, gyroZraw = 0;         // gyro raw data
volatile int16_t magXraw = 0, magYraw = 0, magZraw = 0;            // magnetometer raw data
volatile float acclXscl = 0.0, acclYscl = 0.0, acclZscl = 0.0;     // accelerometer data scaled to specific force in g
volatile float gyroXscl = 0.0, gyroYscl = 0.0, gyroZscl = 0.0;     // gyro data scaled to angular rate in °/s
volatile float magXscl = 0.0, magYscl = 0.0, magZscl = 0.0;        // magnetometer scaled data
volatile float gravX = 0.0, gravY = 0.0, gravZ = 0.0;              // gravity vector direction
volatile float acclXlin = 0.0, acclYlin = 0.0, acclZlin = 0.0;     // linear acceleration direction
volatile float acclN = 0.0, acclE = 0.0, acclD = 0.0;              // linear acceleration in NED reference

float acclBias[3] = {0,0,0};           // bias corrections for accelerometer axis
float gyroBias[3] = {0,0,0};           // bias corrections for gyroscope axis

uint8_t mag_aj[3] = {181,182,171};      // magnetometer adjustment values (calculated once, they won't change)

// when you get good values for local calibration, write them down here and do not calibrate the magnetometer
double mag_offset[3] = {-4.542e+03,2.231e+03,1.345e+03};  // last magnetometer hard iron offset values
double mag_scale[3] = {9.965564738292010949e-01,1.060850439882697893e+00,9.488524590163934791e-01}; // last magnetometer soft iron scale values

//double mag_offset[3] = {0,0,0};
//double mag_scale[3] = {1,1,1};


//===================================================
// FUNCTIONS

// get sensor values with composition of high and low bits
int16_t Read_Word_2c(uint8_t addr, uint8_t reg){
    int16_t val;
    val =  ReadI2C1(addr, reg);
    val = val << 8;
    val += ReadI2C1(addr, reg+1);

    if(val>=0x8000)
        val = -(65536 - val);

    return val;
}

// initial setup
void InitMPU9250(){
    //initialize Timer
    TimerBegin();

    //initialize I2C module 1
    InitI2C1();

    WriteI2C1(MPU9250_ADDRESS, MPU9250_CONFIG, 0x06);
    WriteI2C1(MPU9250_ADDRESS, MPU9250_ACCEL_CONFIG2,0x06);
    WriteI2C1(MPU9250_ADDRESS, MPU9250_GYRO_CONFIG,0x06);
    WriteI2C1(MPU9250_ADDRESS, MPU9250_ACCEL_CONFIG,ACCEL_SCALE_8_G);
    WriteI2C1(MPU9250_ADDRESS, MPU9250_GYRO_CONFIG,GYRO_SCALE_2000_DPS);

    InitMag();

    // quick calibrate accelerometer and gyroscope before start
    CalibrateMPU(gyroBias, acclBias, GYRO_FS_2000, ACCEL_FS_8);
}

// configure the magnetometer. It needs to happen at every iteration
void InitMag(){
    WriteI2C1(MPU9250_ADDRESS,MPU9250_USER_CTRL,0x00);    // disable master aux I2C mode
    WriteI2C1(MPU9250_ADDRESS,MPU9250_INT_PIN_CFG,0x02);  // enable I2C aux bypass mode
    WriteI2C1(MAG_ADDRESS,MAG_CNTL, 0 << 4 | 6);          // continuous magnetometer measurements in 16 bits at 100Hz sample rate
    int16_t haha = ReadI2C1(MAG_ADDRESS,MAG_CNTL);
    int i;   // wait the mag to stabilize it's registers
    for(i=0;i<100;i++);
}

// calculate bias and scale for gyro and accelerometer
// input: arrays for gyro and accelerometer bias and full scale factor
void CalibrateMPU(float *gyroBias, float *acclBias, uint16_t gyroscale, uint16_t accelscale){
    int i;

    // let the sensor stable when when the led goes red
    // wait for the yellow led to be green
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    SysCtlDelay(SysCtlClockGet() / 3);

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
    // reads 100 samples of accelerometer and gyro data and add it to their bias
    for(i=0;i<100;i++){
        acclBias[0] += Read_Word_2c(MPU9250_ADDRESS, ACCEL_XOUT_H);
        acclBias[1] += Read_Word_2c(MPU9250_ADDRESS, ACCEL_YOUT_H);
        acclBias[2] += Read_Word_2c(MPU9250_ADDRESS, ACCEL_ZOUT_H);

        gyroBias[0] += Read_Word_2c(MPU9250_ADDRESS, GYRO_XOUT_H);
        gyroBias[1] += Read_Word_2c(MPU9250_ADDRESS, GYRO_YOUT_H);
        gyroBias[2] += Read_Word_2c(MPU9250_ADDRESS, GYRO_ZOUT_H);
    }

    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_3, 0);

    // average the bias by the number of samples to get the mean deviation
    acclBias[0] /= 100;
    acclBias[1] /= 100;
    acclBias[2] /= 100;

    gyroBias[0] /= 100;
    gyroBias[1] /= 100;
    gyroBias[2] /= 100;

    if(acclBias[2] > 0L){
        acclBias[2] -= (int32_t) accelscale; // remove gravity from thye z-axis accel bias
    }else{
        acclBias[2] += (int32_t) accelscale;
    }

    // scale bias by the selected factor
    acclBias[0] /= (float)accelscale;
    acclBias[1] /= (float)accelscale;
    acclBias[2] /= (float)accelscale;

    gyroBias[0] /= (float)gyroscale;
    gyroBias[1] /= (float)gyroscale;
    gyroBias[2] /= (float)gyroscale;
}

// get new data
void UpdateDataMPU9250(){
    // get accelerometer raw data
    acclXraw = Read_Word_2c(MPU9250_ADDRESS, ACCEL_XOUT_H);
    acclYraw = Read_Word_2c(MPU9250_ADDRESS, ACCEL_YOUT_H);
    acclZraw = Read_Word_2c(MPU9250_ADDRESS, ACCEL_ZOUT_H);

    // get gyroscope raw data
    gyroXraw = Read_Word_2c(MPU9250_ADDRESS, GYRO_XOUT_H);
    gyroYraw = Read_Word_2c(MPU9250_ADDRESS, GYRO_YOUT_H);
    gyroZraw = Read_Word_2c(MPU9250_ADDRESS, GYRO_ZOUT_H);

    InitMag(); // initialize mag before read data

    // get magnetometer adjusted raw data
    magXraw = Read_Word_2c(MAG_ADDRESS,MAG_X_L)*((mag_aj[0]-128)/256. +1);
    magYraw = Read_Word_2c(MAG_ADDRESS,MAG_Y_L)*((mag_aj[1]-128)/256. +1);
    magZraw = Read_Word_2c(MAG_ADDRESS,MAG_Z_L)*((mag_aj[2]-128)/256. +1);
    ReadI2C1(MAG_ADDRESS,MAG_CNTL);
    ReadI2C1(MAG_ADDRESS,MAG_ST2);

    // scale accelerometer data to g
    acclXscl = (float)acclXraw/ACCEL_FS_8 - acclBias[0];
    acclYscl = (float)acclYraw/ACCEL_FS_8 - acclBias[1];
    acclZscl = (float)acclZraw/ACCEL_FS_8 - acclBias[2];

    // scale gyro data, correct bias and convert it from deg/s to rad/s
    gyroXscl = ((float)gyroXraw/GYRO_FS_2000 - gyroBias[0]);// *0.0174533;
    gyroYscl = ((float)gyroYraw/GYRO_FS_2000 - gyroBias[1]);// *0.0174533;
    gyroZscl = ((float)gyroZraw/GYRO_FS_2000 - gyroBias[2]);// *0.0174533;

    // scale magnetometer data in uT and correct its bias
    magXscl = (double)(magXraw*MAG_FS - mag_offset[0]) * mag_scale[0];
    magYscl = (double)(magYraw*MAG_FS - mag_offset[1]) * mag_scale[1];
    magZscl = (double)(magZraw*MAG_FS - mag_offset[2]) * mag_scale[2];
}

// calculate new quaternions using accel + gyro
void MadgwickIMU(){
    MadgwickAHRSupdateIMU(gyroXscl, gyroYscl, gyroZscl, acclXscl, acclYscl, acclZscl);
}

// calculate new quaternions using accel + gyro + mag
void MadgwickAHRS(){
    beta = 0.6;
    MadgwickAHRSupdate(gyroXscl, gyroYscl, gyroZscl, acclXscl, acclYscl, acclZscl, magYscl, magXscl , -magZscl);
}

// calculate directions for gravity based on current quaternions
void GravityVector(){
    // float prod[4] = {0,0,0,0};
    // float g[4] = {0,0,0,1};         // gravity vector in navigation frame
    // float q[4] = {q0,q1,q2,q3};     // current quaternion
    // float qj[4] = {q0,-q1,-q2,-q3}; // conjugate
    //
    // // g_sensor = q*g*q'
    // QuartenionProduct(prod, q, v);      // q*a
    // QuartenionProduct(prod, prod,qj);   // (q*a)*q'
    //
    // gravX = prod[1];
    // gravY = prod[2];
    // gravZ = prod[3];

    // ------------------------------------------------------
    gravX = -2*(q1*q3-q0*q2);
    gravY = -2*(q0*q1+q2*q3);
    gravZ = q0*q0-q1*q1-q2*q2+q3*q3;
}

// calculate linear acceleration directions based on current quaternions
void LinearAcceleration(){
    // remove gravity from each component of acceleration in the sensor frame
    acclXlin = acclXscl + gravX;
    acclYlin = acclYscl + gravY;
    acclZlin = acclZscl - gravZ;
}

// align 3D cartesian data with earth frame in m/s^2
void NEDAcceleration(){
    float prod[4] = {0,0,0,0};
    float v[4] = {0,acclXlin,acclYlin,acclZlin};    // aceleration vector
    float q[4] = {q0, q1, q2, q3};             // current quaternion
    float qj[4] = {q0,-q1,-q2,-q3};            // conjugate

    // aNED = q*a*q'
    QuartenionProduct(prod, q, v);      // q*a
    QuartenionProduct(prod, prod,qj);   // (q*a)*q'

    acclN = prod[1] * gravity;
    acclE = prod[2] * gravity;
    acclD = prod[3] * gravity;
}

// Euler Angles calculation by Quaternions in deg
void EulerAngles(){
    yaw = atan2f(2*q1*q2 + 2*q0*q3, 2*q0*q0 + 2*q1*q1 -1) * 180.0/M_PI;
    pitch = -asinf(2*q1*q3 - 2*q0*q2) * 180.0/M_PI;
    roll = atan2f(2*q2*q3 + 2*q0*q1, 2*q0*q0 + 2*q3*q3 -1) * 180.0/M_PI;
}


// writes quartenion product of a x b in q
void QuartenionProduct(float *q,float *a, float *b){
    float a0b0 = a[0]*b[0];
    float a0b1 = a[0]*b[1];
    float a0b2 = a[0]*b[2];
    float a0b3 = a[0]*b[3];
    float a1b0 = a[1]*b[0];
    float a1b1 = a[1]*b[1];
    float a1b2 = a[1]*b[2];
    float a1b3 = a[1]*b[3];
    float a2b0 = a[2]*b[0];
    float a2b1 = a[2]*b[1];
    float a2b2 = a[2]*b[2];
    float a2b3 = a[2]*b[3];
    float a3b0 = a[3]*b[0];
    float a3b1 = a[3]*b[1];
    float a3b2 = a[3]*b[2];
    float a3b3 = a[3]*b[3];

    q[0] = a0b0-a1b1-a2b2-a3b3;
    q[1] = a0b1+a1b0+a2b3-a3b2;
    q[2] = a0b2-a1b3+a2b0+a3b1;
    q[3] = a0b3+a1b2-a2b1+a3b0;
}

