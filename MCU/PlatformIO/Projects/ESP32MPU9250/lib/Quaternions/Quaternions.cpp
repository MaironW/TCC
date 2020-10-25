// QUATERNIONS UTILITARY FUNCTIONS
// written by: Mairon S. Wolniewicz
// last update: 23/12/2019

#include <Arduino.h>
#include <Quaternions.h>

#define GRAVITY 9.81;   // [m/s2]

volatile float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;	// quaternion of sensor frame relative to auxiliary frame
volatile float yaw = 0.0f, pitch = 0.0f, roll=0.0f;         // Euler angles

volatile float gravX = 0.0, gravY = 0.0, gravZ = 0.0;              // gravity vector direction
volatile float acclXlin = 0.0, acclYlin = 0.0, acclZlin = 0.0;     // linear acceleration direction
volatile float acclN = 0.0, acclE = 0.0, acclD = 0.0;              // linear acceleration in NED reference

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

// Euler Angles calculation by Quaternions in deg
void EulerAngles(){
    yaw = atan2f(2*q1*q2 + 2*q0*q3, 2*q0*q0 + 2*q1*q1 -1) * 180.0/PI;
    pitch = -asinf(2*q1*q3 - 2*q0*q2) * 180.0/PI;
    roll = atan2f(2*q2*q3 + 2*q0*q1, 2*q0*q0 + 2*q3*q3 -1) * 180.0/PI;
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

    acclN = prod[1] * GRAVITY;
    acclE = prod[2] * GRAVITY;
    acclD = prod[3] * GRAVITY;
}

// calculate linear acceleration directions based on current quaternions
void LinearAcceleration(float acclX,float acclY,float acclZ){
    // remove gravity from each component of acceleration in the sensor frame
    acclXlin = acclX + gravX;
    acclYlin = acclY + gravY;
    acclZlin = acclZ + gravZ;
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