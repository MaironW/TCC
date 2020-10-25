#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
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

#include "i2c1.h"
#include "uartout.h"
#include "timer.h"

// configs
#define MPU9250_ADDRESS         0x68
#define MPU9250_WHO_I_AM        0x75    // should return 0x71
#define MPU9250_USER_CTRL       0x64
#define MPU9250_INT_PIN_CFG     0x37    // set bypass mode writing 0x02
#define MPU9250_PWR_MGMT_1      0x6B
#define MPU9250_PWR_MGMT_2      0x6C
#define MPU9250_INT_ENABLE      0x38
#define MPU9250_FIFO_EN         0x23
#define MPU9250_I2C_MST_CTRL    0x24
#define MPU9250_CONFIG          0x1A
#define MPU9250_GYRO_CONFIG     0x1B
#define MPU9250_ACCEL_CONFIG    0X1C
#define MPU9250_ACCEL_CONFIG2   0X1D
#define MPU9250_SMPLRT_DIV      0x19

#define MAG_ADDRESS             0x0C
#define MAG_WHO_I_AM            0x00    // should return 0x48
#define MAG_CNTL                0x0A
#define MAG_ST1                 0x02    // status 1
#define MAG_ST2                 0x09    // status 2

// accel raw data outputs
#define ACCEL_XOUT_H            0x3B
#define ACCEL_XOUT_L            0x3C
#define ACCEL_YOUT_H            0x3D
#define ACCEL_YOUT_L            0x3E
#define ACCEL_ZOUT_H            0x3F
#define ACCEL_ZOUT_L            0x40

// gyro raw data outputs
#define GYRO_XOUT_H             0x43
#define GYRO_XOUT_L             0x44
#define GYRO_YOUT_H             0x45
#define GYRO_YOUT_L             0x46
#define GYRO_ZOUT_H             0x47
#define GYRO_ZOUT_L             0x48

// gyro full scale configuration values
#define GYRO_SCALE_250_DPS      0x00    // 250  °/s
#define GYRO_SCALE_500_DPS      0x08    // 500  °/s
#define GYRO_SCALE_1000_DPS     0x10    // 1000 °/s
#define GYRO_SCALE_2000_DPS     0x18    // 2000 °/s

// gyro full scale factors
#define GYRO_FS_250             131.0   // LBS/(°/s)
#define GYRO_FS_500             65.5    // LBS/(°/s)
#define GYRO_FS_1000            32.8    // LBS/(°/s)
#define GYRO_FS_2000            16.4    // LBS/(°/s)

// accelerometer full scale configuration values
#define ACCEL_SCALE_2_G         0x00    // 2.0  g
#define ACCEL_SCALE_4_G         0x08    // 4.0  g
#define ACCEL_SCALE_8_G         0x10    // 8.0  g
#define ACCEL_SCALE_16_G        0x18    // 16.0 g

// accelerometer full scale fators
#define ACCEL_FS_2              16384   // LSB/g
#define ACCEL_FS_4              8192    // LSB/g
#define ACCEL_FS_8              4096    // LSB/g
#define ACCEL_FS_16             2048    // LSB/g

#define MAG_SCALE               4800    // uT
#define MAG_FS                  0.6     // magnetometer full scale factor uT/LSB (10mG/LSB)

// mag raw data outputs
#define MAG_X_L                 0x03
#define MAG_X_H                 0x04
#define MAG_Y_L                 0x05
#define MAG_Y_H                 0x06
#define MAG_Z_L                 0x07
#define MAG_Z_H                 0x08



//===================================================
// HEADERS

// get sensor values with composition of high and low bits
// input: device address (MPU9250_ADDRESS or MAG_ADDRESS), register address
// output: a 16bit value
int16_t Read_Word_2c(uint8_t addr, uint8_t reg);

// initial setup
void Setup();

// configure the magnetometer. It needs to happen at every iteration
void SetupMag();


//===================================================
// VARIABLES
//int16_t acclXraw, acclYraw, acclZraw;   // accelerometer raw data

float acclBias[3] = {0,0,0};           // bias corrections for accelerometer axis
float gyroBias[3] = {0,0,0};           // bias corrections for gyroscope axis

uint8_t mag_aj[3] = {181,182,171};      // magnetometer adjustment values (calculated once, they won't change)

// when you get good values for local calibration, write them down here and do not calibrate the magnetometer
//double mag_offset[3] = {-10816.0,9454.0,4484.0};  // last magnetometer hard iron offset values
//double mag_scale[3] = {0.994783,1.022035,0.983947}; // last magnetometer soft iron scale values
double mag_offset[3] = {0,0,0};
double mag_scale[3] = {1,1,1};

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
void Setup(){
    // Set the clocking to run directly from the external crystal/oscillator.
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    //initialize Timer
    TimerBegin();

    //initialize I2C module 1
    InitI2C1();

    //initialize UART module 0
    UARTsetup(115200);

    WriteI2C1(MPU9250_ADDRESS, MPU9250_CONFIG, 0x06);
    WriteI2C1(MPU9250_ADDRESS, MPU9250_ACCEL_CONFIG2,0x06);
    WriteI2C1(MPU9250_ADDRESS, MPU9250_GYRO_CONFIG,0x06);
    WriteI2C1(MPU9250_ADDRESS, MPU9250_ACCEL_CONFIG,ACCEL_SCALE_8_G);
    WriteI2C1(MPU9250_ADDRESS, MPU9250_GYRO_CONFIG,GYRO_SCALE_2000_DPS);

    SetupMag();
}

// configure the magnetometer. It needs to happen at every iteration
void SetupMag(){
    WriteI2C1(MPU9250_ADDRESS,MPU9250_USER_CTRL,0x00);    // disable master aux I2C mode
    WriteI2C1(MPU9250_ADDRESS,MPU9250_INT_PIN_CFG,0x02);  // enable I2C aux bypass mode
    WriteI2C1(MAG_ADDRESS,MAG_CNTL, 0 << 4 | 6);              // continuous magnetometer measurements in 16 bits at 100Hz sample rate
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
    for(i=0;i<2000000;i++){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
    }

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


//===================================================
// MAIN

int main(void){
    Setup();

    // quick calibrate accelerometer and gyroscope before start
    CalibrateMPU(gyroBias, acclBias, GYRO_FS_2000, ACCEL_FS_8);

    int16_t acclXraw, acclYraw, acclZraw;   // accelerometer raw data
    int16_t gyroXraw, gyroYraw, gyroZraw;   // gyro raw data
    int16_t magXraw, magYraw, magZraw;      // magnetometer raw data
    float acclXscl, acclYscl, acclZscl;     // accelerometer data scaled to specific force in g
    float gyroXscl, gyroYscl, gyroZscl;     // gyro data scaled to angular rate in °/s
    float magXscl, magYscl, magZscl;        // magnetometer scaled data

    float dt = 0.0;
    int now = 0;
    int lastUpdate = 0;

    float freq = 0.0;

    while(1){
        now = millis;
        dt = (now - lastUpdate)*1e-3;
        lastUpdate = now;
        freq = 1/dt;

        // get accelerometer raw data
        acclXraw = Read_Word_2c(MPU9250_ADDRESS, ACCEL_XOUT_H);
        acclYraw = Read_Word_2c(MPU9250_ADDRESS, ACCEL_YOUT_H);
        acclZraw = Read_Word_2c(MPU9250_ADDRESS, ACCEL_ZOUT_H);

        // get gyroscope raw data
        gyroXraw = Read_Word_2c(MPU9250_ADDRESS, GYRO_XOUT_H);
        gyroYraw = Read_Word_2c(MPU9250_ADDRESS, GYRO_YOUT_H);
        gyroZraw = Read_Word_2c(MPU9250_ADDRESS, GYRO_ZOUT_H);

        SetupMag(); // initialize mag before read data

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
        gyroXscl = ((float)gyroXraw/GYRO_FS_2000 - gyroBias[0]) *0.0174533;
        gyroYscl = ((float)gyroYraw/GYRO_FS_2000 - gyroBias[1]) *0.0174533;
        gyroZscl = ((float)gyroZraw/GYRO_FS_2000 - gyroBias[2]) *0.0174533;

        // scale magnetometer data in uT and correct its bias
        magXscl = (double)(magXraw*MAG_FS*mag_scale[0]) - mag_offset[0];
        magYscl = (double)(magYraw*MAG_FS*mag_scale[1]) - mag_offset[1];
        magZscl = (double)(magZraw*MAG_FS*mag_scale[2]) - mag_offset[2];

        char buffer[100];

//        sprintf(buffer,"%8f\t%8f\t%8f\t%8f\t%8f\t%8f\t%8g\t%8g\t%8g\t\n",acclXscl,acclYscl,acclZscl,gyroXscl,gyroYscl,gyroZscl,magXscl,magYscl,magZscl);
        sprintf(buffer,"%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t%8.2f\t\n",acclXscl,acclYscl,acclZscl,gyroXscl,gyroYscl,gyroZscl,magXscl,magYscl,magZscl);
        UARTprintf("%s",buffer);
    }
}

