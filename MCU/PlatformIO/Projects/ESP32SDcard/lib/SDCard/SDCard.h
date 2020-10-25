// SD Card library for ESP 32 based on 
// Rui Santos project at https://randomnerdtutorials.com 
// adapted by Mairon S. Wolniewicz
// last update: 12/03/2020

#ifndef SDCard_h
#define SDCard_h

#include <FS.h>

#define SD_CS 5 // define CS pin for SD card module

// write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message);

// append data to the SD card
void appendFile(fs::FS &fs, const char * path, const char * message);

// write data on the SD card
void logSDCard(String message);

// setup SD card
void InitSDCard();



#endif
