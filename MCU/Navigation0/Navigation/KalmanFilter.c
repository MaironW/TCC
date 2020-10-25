// Kalman Filter library
// written by: Mairon de Souza Wolniewicz
// last update: 23/07/2019

#include "KalmanFilter.h"

//===================================================
// VARIABLES

#define dt  0

// identity matrix
float I[9][9] = {
    {1,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,1}
};

// dynamic matrix
volatile float A[9][9] = {
    {1,0,0,dt,0,0,0.5*dt*dt,0,0},   // x update
    {0,1,0,0,dt,0,0,0.5*dt*dt,0},   // y update
    {0,0,1,0,0,dt,0,0,0.5*dt*dt},   // z update
    {0,0,0,1,0,0,dt,0,0},           // vx update
    {0,0,0,0,1,0,0,dt,0},           // vy update
    {0,0,0,0,0,1,0,0,dt},           // vz update
    {0,0,0,0,0,0,1,0,0},            // ax update
    {0,0,0,0,0,0,0,1,0},            // ay update
    {0,0,0,0,0,0,0,0,1}             // az update
};

// measurement matrix
volatile float H[9][9] = {
    {1,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,1}
};

// estimative uncertainty matrix DEVE SER ALTERADA DEPOIS
volatile float P[9][9] = {
    {1,0,0,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0,0},
    {0,0,1,0,0,0,0,0,0},
    {0,0,0,1,0,0,0,0,0},
    {0,0,0,0,1,0,0,0,0},
    {0,0,0,0,0,1,0,0,0},
    {0,0,0,0,0,0,1,0,0},
    {0,0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,0,0,1}
};

volatile float K;    // Kalman gain matrix

// measurement covariance matrix
volatile float R[9][9] = {
    {R_Xvar*R_Xvar,0,0,0,0,0,0,0,0},
    {0,R_Yvar*R_Yvar,0,0,0,0,0,0,0},
    {0,0,R_Zvar*R_Zvar,0,0,0,0,0,0},
    {0,0,0,R_vXvar*R_vXvar,0,0,0,0,0},
    {0,0,0,0,R_vYvar*R_vYvar,0,0,0,0},
    {0,0,0,0,0,R_vZvar*R_vZvar,0,0,0},
    {0,0,0,0,0,0,R_aXvar*R_aXvar,0,0},
    {0,0,0,0,0,0,0,R_aYvar*R_aYvar,0},
    {0,0,0,0,0,0,0,0,R_aZvar*R_aZvar}
};

// process noise covariance matrix
volatile float Q[9][9] = {
    {Q_Nvar*Q_Nvar,0,0,0,0,0,0,0,0},
    {0,Q_Evar*Q_Evar,0,0,0,0,0,0,0},
    {0,0,Q_Dvar*Q_Dvar,0,0,0,0,0,0},
    {0,0,0,Q_vNvar*Q_vNvar,0,0,0,0,0},
    {0,0,0,0,Q_vEvar*Q_vEvar,0,0,0,0},
    {0,0,0,0,0,Q_vDvar*Q_vDvar,0,0,0},
    {0,0,0,0,0,0,Q_aNvar*Q_aNvar,0,0},
    {0,0,0,0,0,0,0,Q_aEvar*Q_aEvar,0},
    {0,0,0,0,0,0,0,0,Q_aDvar*Q_aDvar}
};
