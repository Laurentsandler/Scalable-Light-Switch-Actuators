/*
  Scalable Light Switch Actuators - Base Station Firmware
  
  The base ESP32 connects to WiFi/Blynk and communicates with up to 5
  ESP32-C3 modules over ESP-NOW. When a Blynk button (V0-V4) is pressed,
  the base sends a toggle command to the matching module.

  Setup:
  1. Flash each C3 module with module firmware and note its MAC address
  2. Paste those MACs below along with your WiFi and Blynk credentials
  3. In Blynk, create 5 button widgets on V0-V4 (Push mode, Integer 0/1)
*/

// --------- USER CONFIG ----------
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
const char* BLYNK_AUTH = "YOUR_BLYNK_AUTH_TOKEN";

// Paste MAC addresses from each module's serial output here
uint8_t module1MAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t module2MAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t module3MAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t module4MAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t module5MAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// --------------------------------

#include <WiFi.h>
#include <esp_now.h>
#include <BlynkSimpleEsp32.h>

// TODO: Implement base station logic
// - Connect to WiFi and Blynk
// - Register ESP-NOW peers (modules 1-5)
// - On Blynk V0-V4 press, send toggle command to corresponding module via ESP-NOW

void setup() {
  Serial.begin(115200);
  // TODO: setup
}

void loop() {
  Blynk.run();
}
