// MPU9250 library for Tiva C MCU
// written by: Mairon de Souza Wolniewicz
// last update: 17/06/2019

#ifndef MPU9250_H
#define MPU9250_H

//===================================================
// DEFINITIONS

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
// VARIABLES

extern volatile int16_t acclXraw, acclYraw, acclZraw;   // accelerometer raw data
extern volatile int16_t gyroXraw, gyroYraw, gyroZraw;   // gyro raw data
extern volatile int16_t magXraw, magYraw, magZraw;      // magnetometer raw data
extern volatile float acclXscl, acclYscl, acclZscl;     // accelerometer data scaled to specific force in g
extern volatile float gyroXscl, gyroYscl, gyroZscl;     // gyro data scaled to angular rate in °/s
extern volatile float magXscl, magYscl, magZscl;        // magnetometer scaled data
extern volatile float gravX, gravY, gravZ;              // gravity vector direction
extern volatile float acclXlin, acclYlin, acclZlin;     // linear acceleration direction
extern volatile float acclN, acclE, acclD;              // linear acceleration in NED reference

// time variables for Madgwick counter
extern volatile float dt;
extern volatile int now;
extern volatile int lastUpdate;
extern volatile float freq;

extern volatile float q0,q1,q2,q3;                      // quaternions
extern volatile float yaw, pitch, roll;                 // Euler angles

//===================================================
// HEADERS

// get sensor values with composition of high and low bits
// input: device address (MPU9250_ADDRESS or MAG_ADDRESS), register address
// output: a 16bit value
int16_t Read_Word_2c(uint8_t addr, uint8_t reg);

// initial setup
void InitMPU9250();

// configure the magnetometer. It needs to happen at every iteration
void InitMag();

// calculate bias and scale for gyro and accelerometer
// input: arrays for gyro and accelerometer bias and full scale factor
void CalibrateMPU(float *gyroBias, float *acclBias, uint16_t gyroscale, uint16_t accelscale);

// get new data for the accelerometer, gyro and mag
void UpdateDataMPU9250();

// calculate directions for gravity based on current quaternions
void GravityVector();

// calculate linear acceleration directions based on current quaternions
void LinearAcceleration();

// Euler Angles calculation by Quaternions in deg
void EulerAngles();

// writes quartenion product of a x b in q
void QuartenionProduct(float *q,float *a, float *b);

// align 3D cartesian data with earth frame in m/s^2
void NEDAcceleration();

#endif
