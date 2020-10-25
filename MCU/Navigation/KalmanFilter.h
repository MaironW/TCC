// // Kalman Filter library
// // written by: Mairon de Souza Wolniewicz
// // last update: 18/09/2019
//
// #ifndef KALMANFILTER_H
// #define KALMANFILTER_H
//
// //===================================================
// // DEFINITIONS
//
// // Measurement Covariance (R)
// #define     R_Xvar    2.5       // ± [m] GPS datasheet p.6
// #define     R_Yvar    2.5       // ± [m] GPS datasheet p.6
// #define     R_Zvar    52        // ± [m] GPS calculated real-measure
// #define     R_vXvar   0.1       // ± [m/s] GPS datasheet p.6
// #define     R_vYvar   0.1       // ± [m/s] GPS datasheet p.6
// #define     R_vZvar   0.1       // ± [m/s] GPS
//
// // Process Noise Covariance (Q)
// #define     Q_aNvar   0.006     // ± [m/s^2] IMU calib.
// #define     Q_aEvar   0.012     // ± [m/s^2] IMU calib.
// #define     Q_aDvar   0.003     // ± [m/s^2] IMU calib.
//
// //===================================================
// // VARIABLES
//
// // one dimensional filter covariance inputs
// volatile float NorthCovariance[3];
// volatile float EastCovariance[3];
// volatile float DownCovariance[3];
//
// extern volatile float A[9][9];  // dynamic matrix
// extern volatile float H[9][9];  // measurement matrix
// extern volatile float K;        // Kalman gain matrix
// extern volatile float P[9][9];  // estimative uncertainty matrix
// // extern volatile float R[9][9];  // measurement covariance matrix
// // extern volatile float Q[9][9];  // process noise covariance matrix
//
// extern volatile float dNest, dEest, dDest; // estimated displacement
// extern volatile float vNest, vEest, vDest; // estimated velocity
//
// //===================================================
// // FUNCTIONS
//
// void OneDimensionalKalmanFilter(float d, float v, float a, float *covar);
//
//
//
//
//
//
// #endif
