// // Kalman Filter library
// // written by: Mairon de Souza Wolniewicz
// // last update: 23/07/2019
//
// #include "KalmanFilter.h"
// #include "timer.h"
//
// //===================================================
// // VARIABLES
//
// // one dimensional filter covariance inputs
// float NorthCovariance[3] = {R_Xvar, R_vXvar, Q_aNvar};
// float EastCovariance[3] = {R_Yvar, R_vYvar, Q_aEvar};
// float DownCovariance[3] = {R_Zvar, R_vZvar, Q_aDvar};
//
// // identity matrix
// float I[2][2] = {
//     {1,0},
//     {0,1}
// };
//
// // dynamic matrix
// volatile float A[2][2] = {
//     {1,dt},     // displacement update
//     {0,1}       // velocity update
// };
//
// // control matrix
// volatile float B[2] = {0.5*dt*dt, dt}
//
// // measurement matrix
// volatile float H[9][9] = {
//     {1,0},
//     {0,1}
// };
//
// //===================================================
// // FUNCTIONS
//
// void OneDimensionalKalmanFilter(float d, float v, float a, float *covar){
//     // measurement covariance matrix
//     float R[2][2] = {
//         {covar[0]*covar[0],0},
//         {0,covar[1]*covar[1]}
//     };
//
//     // process noise covariance matrix
//     float Q[2][2] = {
//         {covar[2]*covar[2],0},
//         {0,covar[2]*covar[2]}
//     };
//
//     // create an observation vetor of noisy GPS sinals
//     float Z[2] = {d,v};
//
//     // control signal
//     float u = a;
//
//     // matrix dimensions
//     int nx = 2;
//     int ny = 2;
//     int nt = 1;
//
//
//
//
//
//
//
// }
