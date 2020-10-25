// SD Card library for ESP 32 based on 
// Rui Santos project at https://randomnerdtutorials.com 
// adapted by Mairon S. Wolniewicz
// last update: 12/03/2020

#include <Arduino.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>

#include "SDCard.h"

String fileAddress = "/log0000.dat";
uint8_t fileIndex = 0;
char fileIndexfmt[4] = {'0','0','0','0'};

// write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path,FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        // Serial.println("File written");
    }else{
        Serial.println("Write failed");
    }
    file.close();
}

// append data to the SD card
void appendFile(fs::FS &fs, const char * path, const char * message){
    // Serial.printf("Appending to the file: %s\n", path);

    File file = fs.open(path,FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        // Serial.println("Message appended");
    }else{
        Serial.println("Append failed");
    }
    file.close();
}

// write data on the SD card
void logSDCard(String message){
    Serial.println("__"+message);
    appendFile(SD,fileAddress.c_str(), message.c_str());
}

// setup SD card
void InitSDCard(){
    // initialize SD card
    SD.begin(SD_CS);
    if(!SD.begin(SD_CS)){
        Serial.println("Card Mount Failed");
        return;
    }

    uint8_t cardType = SD.cardType();
    if(cardType==CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.println("Initializing SD card...");
    if(!SD.begin(SD_CS)){
        Serial.println("ERROR - SD card initialization failed.");
        return;
    }

    // if logXXXX.dat doesn't exist
    // create file on SD and write the data labels
    File file = SD.open(fileAddress);
    Serial.println(fileAddress);
    while(file){
        fileIndex++;
        sprintf(fileIndexfmt,"%04d",fileIndex);
        fileAddress = "/log"+String(fileIndexfmt)+".dat";    
        file = SD.open(fileAddress);
    }

    Serial.println("Creating new log file...");
    // writeFile(SD,fileAddress.c_str(), "Header \r\n"); // write header if needed

    file.close();
}

