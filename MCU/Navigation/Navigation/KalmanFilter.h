// Kalman Filter library
// written by: Mairon de Souza Wolniewicz
// last update: 23/07/2019

#ifndef KALMANFILTER_H
#define KALMANFILTER_H

//===================================================
// DEFINITIONS

// Measurement Covariance (R)
#define     R_Xvar    2.5       // ± [m] GPS datasheet p.6
#define     R_Yvar    2.5       // ± [m] GPS datasheet p.6
#define     R_Zvar    52        // ± [m] GPS calculated real-measure
#define     R_vXvar   0.1       // ± [m/s] GPS datasheet p.6
#define     R_vYvar   0.1       // ± [m/s] GPS datasheet p.6
#define     R_vZvar   0.1       // ± [m/s] GPS datasheet p.6
#define     R_aXvar   0.0       // ± [m/s^2] IMU ?
#define     R_aYvar   0.0       // ± [m/s^2] IMU ?
#define     R_aZvar   0.0       // ± [m/s^2] IMU ?

// Process Noise Covariance (Q)
#define     Q_Nvar    0.003706  // ± [m] GPS calib.
#define     Q_Evar    0.002465  // ± [m] GPS calib.
#define     Q_Dvar    0.003450  // ± [m] GPS calib.
#define     Q_vNvar   0.0       // ± [m/s] GPS calib.
#define     Q_vEvar   0.000083  // ± [m/s] GPS calib.
#define     Q_vDvar   0.000040  // ± [m/s] GPS calib.
#define     Q_aNvar   0.006     // ± [m/s^2] IMU calib.
#define     Q_aEvar   0.012     // ± [m/s^2] IMU calib.
#define     Q_aDvar   0.003     // ± [m/s^2] IMU calib.

//===================================================
// VARIABLES

extern volatile float A[9][9];  // dynamic matrix
extern volatile float H[9][9];  // measurement matrix
extern volatile float K;        // Kalman gain matrix
extern volatile float P[9][9];  // estimative uncertainty matrix
extern volatile float R[9][9];  // measurement covariance matrix
extern volatile float Q[9][9];  // process noise covariance matrix








#endif
