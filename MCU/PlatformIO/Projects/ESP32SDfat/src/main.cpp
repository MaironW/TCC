// Minimal sketch using SdFat library with ESP32
// from Arduino\libraries\SdFat-master\examples\LongFileName.ino

#include<SPI.h>
#include "SdFat.h" // https://github.com/greiman/SdFat

#define sd_cs_pin 5 // SD card CS pin
#define SDSPEED SD_SCK_MHZ(20) // result: 101 tracks in 771 ms

SdFat sd;
SdFile track;
File myFile;

void setup() {
Serial.begin(15200);
delay(10);
Serial.println("\n SdFAT_test");

if (!sd.begin(sd_cs_pin, SDSPEED)) {
Serial.println("Card Mount Failed");
return;
}
Serial.println("Card Mount OK");

myFile = sd.open("test.txt",FILE_WRITE);

if(myFile){
  for(int i=0;i<100;i++){
    myFile.println(i);
  }
  myFile.close();
  Serial.println("done.");
}else{
  Serial.println("Error opening test.txt");
}





}

void loop() {} // do nothing