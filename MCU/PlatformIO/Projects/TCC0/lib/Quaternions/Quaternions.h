// QUATERNIONS UTILITARY FUNCTIONS
// written by: Mairon S. Wolniewicz
// last update: 23/12/2019

#ifndef Quaternions_h
#define Quaternions_h

extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
extern volatile float yaw, pitch, roll; // euler angles of sensor frame relative to auxiliary frame

extern volatile float gravX, gravY, gravZ;              // gravity vector direction
extern volatile float acclXlin, acclYlin, acclZlin;     // linear acceleration direction
extern volatile float acclN, acclE, acclD;              // linear acceleration in NED reference

// extern volatile bool quat_conv;

// calculate directions for gravity based on current quaternions
void GravityVector();

// calculate linear acceleration directions based on current quaternions
void LinearAcceleration(float acclX,float acclY,float acclZ);

// Euler Angles calculation by Quaternions in deg
void EulerAngles();

// writes quartenion product of a x b in q
void QuartenionProduct(float *q,float *a, float *b);

// align 3D cartesian data with earth frame in m/s^2
void NEDAcceleration();

// check if quaternions converged
// void VerifyConvergence();




#endif