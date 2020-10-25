#include <Arduino.h>
#include <gps.h>

// ----------------------------------------
// DECLARATIONS

volatile float lat0=0, lon0=0, alt0=0;   // [deg] old GPS position coordinates
volatile float distance=0;                 // [m]   distance between two gps points
volatile float bearing=0;                  // [rad] direction angle from North
volatile float vN=0, vE=0, vD=0;         // [m/s] geographic speeds
volatile float dN=0, dE=0, dD=0;         // [m/s] geographic displacement
