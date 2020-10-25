// QUATERNIONS UTILITARY FUNCTIONS
// written by: Mairon S. Wolniewicz
// last update: 23/12/2019
// last update: 06/05/2020

#ifndef Quaternions_h
#define Quaternions_h

// #define MAGNETIC_DECLINATION    -19.8333 // [deg] value for Joinville
#define MAGNETIC_DECLINATION    -18.7333 // [deg] value for Ot.Costa (06/05/2020)

extern volatile float q0, q1, q2, q3;	// quaternion of sensor frame relative to auxiliary frame
extern volatile float yaw, pitch, roll; // euler angles of sensor frame relative to auxiliary frame

extern volatile float gravX, gravY, gravZ;              // gravity vector direction
extern volatile float acclXlin, acclYlin, acclZlin;     // linear acceleration direction
extern volatile float acclN, acclE, acclD;              // linear acceleration in NED reference

// calculate directions for gravity based on current quaternions
void GravityVector();

// calculate linear acceleration directions based on current quaternions
void LinearAcceleration(float acclX,float acclY,float acclZ);

// Euler Angles calculation by Quaternions in deg
void EulerAngles();

// writes quartenion product of a x b in q
void QuaternionProduct(float *q,float *a, float *b);

// align 3D cartesian data with earth frame in m/s^2
void NEDAcceleration();

// correct for magnetic declination when usign Magnetometer alignment
void DeclinationAlign();



#endif