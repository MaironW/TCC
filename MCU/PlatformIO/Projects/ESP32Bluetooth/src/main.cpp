/*Program to control LED (ON/OFF) from ESP32 using Serial Bluetooth
 * Thanks to Neil Kolbans for his efoorts in adding the support to Arduino IDE
 * Turotial on: www.circuitdigest.com 
 */

#include <Arduino.h>
#include <BluetoothSerial.h> //Header File for Serial Bluetooth, will be added by default into Arduino

BluetoothSerial ESP_BT; //Object for Bluetooth

int incoming;
int LED_BUILTIN = 2;

// double x =1.0,y=1.0,z=1.0;
// String message = "";

void setup() {
  Serial.begin(115200); //Start Serial monitor in 115200
  ESP_BT.begin("ESP32_Mairon"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

  pinMode (LED_BUILTIN, OUTPUT);//Specify that LED pin is output
}

void loop() {

  // message = String(x) + ',' + String(y) + ',' + String(z);
  // x = x*3.14;
  // y = y+2*3.14;
  // z = x/2;

  // if(x>100000) x = 1;
  // if(y>100000) y = 1;
  // if(z<1e-5) z = 1;
  // Serial.println(message);
  // ESP_BT.println(message);
  
  if (ESP_BT.available()){ //Check if we receive anything from Bluetooth
    incoming = ESP_BT.read(); //Read what we recevive 
    // Serial.print("Received:"); Serial.println(incoming);
    if(incoming) Serial.print(char(incoming));

    if (incoming == 49){
        
        
        ESP_BT.println("LED turned ON");
    }
        
    else if (incoming == 48){
        digitalWrite(LED_BUILTIN, LOW);
        ESP_BT.println("LED turned OFF");
    }

    else{
      ESP_BT.println(" ");
    }

  }
  delay(20);
}