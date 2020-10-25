#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <esp_gap_bt_api.h>
 
bool initBluetooth(const char *deviceName)
{
  if (!btStart()) {
    Serial.println("Failed to initialize controller");
    return false;
  }
 
  if (esp_bluedroid_init()!= ESP_OK) {
    Serial.println("Failed to initialize bluedroid");
    return false;
  }
 
  if (esp_bluedroid_enable()!= ESP_OK) {
    Serial.println("Failed to enable bluedroid");
    return false;
  }
 
  esp_bt_dev_set_device_name(deviceName);
 
  esp_bt_gap_set_scan_mode(ESP_BT_SCAN_MODE_CONNECTABLE_DISCOVERABLE);
 
}
 
void printDeviceAddress() {
 
  const uint8_t* point = esp_bt_dev_get_address();
 
  for (int i = 0; i < 6; i++) {
 
    char str[3];
 
    sprintf(str, "%02X", (int)point[i]);
    Serial.print(str);
 
    if (i < 5){
      Serial.print(":");
    }
 
  }
}
 
void setup() {
  Serial.begin(115200);
 
  initBluetooth("ESP32 BT");
  printDeviceAddress();
}
 
void loop() {}