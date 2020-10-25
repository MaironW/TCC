// KalmanFilter.h
// written by: Mairon de Souza Wolniewicz
// last update: 07/01/2020

#include <Eigen30.h>

//=====================================================================================================
#ifndef KalmanFilter_h
#define KalmanFilter_h

//----------------------------------------------------------------------------------------------------
// VARIABLES

// GPS position variance
#define Rd_N 2.5    // [m]
#define Rd_E 2.5    // [m]
#define Rd_D 25     // [m]

// GPS velocity variance
#define Rv_N 0.1    // [m/s]
#define Rv_E 0.1    // [m/s]
#define Rv_D 0.1    // [m/s] 

// IMU acceleration variance
#define Qa_N 0.005  // [m/s2]
#define Qa_E 0.005  // [m/s2]
#define Qa_D 0.005  // [m/s2]

//---------------------------------------------------------------------------------------------------
// FUNCTIONS

// setup for Kalman Filter
// init A, B, x_pred, P_pred, x_est, P_est
// input the time in seconds to be the period of Kalman filter
void KalmanFilterInit(float dt);

// realize Kalman Filtering for one dimension at a time
Eigen::Vector2d Linear1DKalmanFilter(float d, float v, float a, float dt, char direction);

#endif