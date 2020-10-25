#include <Arduino.h>
#include <runtime.h>

Time::Time(){
    dt1 = 0.0;
    dt0 = 0.0;
    start = 0.0;
    // changed = false;
}

float Time::Update(){
    dt1 = (millis()-start)/1000.0;
    start = millis();

    // if(dt1!=dt0) changed = true;
    // else changed = false;
    
    dt0 = dt1;
    
    return dt1;
}

float Time::dt(){
    return dt1;
}