#include <Arduino.h>
#include <Eigen30.h>

void setup() {
  Serial.begin(115200);


  Eigen::Matrix2d A,B;
  B << 2,3, 
       4,6;
  A << 1,2,
       5,6;

  A = B.transpose();

  Serial.println(String(A(0,0))+' '+String(A(0,1)));
  Serial.println(String(A(1,0))+' '+String(A(1,1)));
}

void loop() {
  // put your main code here, to run repeatedly:
}