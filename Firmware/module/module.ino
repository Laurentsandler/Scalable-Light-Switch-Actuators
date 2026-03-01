/*
  Scalable Light Switch Actuators - Module Firmware (ESP32-C3)
  
  Each module receives toggle commands from the base station over ESP-NOW
  and moves its servo ±20° to flip a light switch. After acting, the module
  returns to deep sleep to preserve battery life (~2 months per charge).

  Setup:
  1. Flash this to each ESP32-C3 module
  2. Open Serial monitor and note the MAC address printed
  3. Paste that MAC into the base station firmware
*/

#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Servo configuration
#define SERVO_PIN     5
#define SERVO_CENTER  90
#define SERVO_OFFSET  20  // degrees to move to toggle switch

Servo lightSwitch;

// TODO: Implement module logic
// - Print MAC address on boot
// - Initialize ESP-NOW and register receive callback
// - On receiving toggle command: wake, move servo, sleep
// - Physical button: toggle servo manually

void setup() {
  Serial.begin(115200);
  
  // Print MAC address for base station configuration
  WiFi.mode(WIFI_STA);
  Serial.print("Module MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  // TODO: setup ESP-NOW, servo, button, deep sleep
}

void loop() {
  // Most logic handled via ESP-NOW callbacks and deep sleep
}
