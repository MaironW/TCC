#include <Arduino.h>
#include <SPI.h>
#include <SDCard.h>

// define CS pin for SD card module
#define SD_CS 5

String dataMessage = "0.000000,0.000000,0.000000,0.000000,-0.677746,-0.126574,-16.424084,0.000000,0.000000,4.232326,-1.139050\r\n";
float start = 0.0;
float t1 = 0.0;


void setup() {
  Serial.begin(57600);

  InitSDCard();
  
}


void loop() {
  t1 = (millis()-start)/1000.0;
  start = millis();

  logSDCard(dataMessage);

  Serial.println(String(t1,6));

  // delay(1000);
}