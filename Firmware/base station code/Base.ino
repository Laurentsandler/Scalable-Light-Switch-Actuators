/*
 * ============================================================
 *  ESP32 Base Station – Blynk + ESP-NOW Light Switch Controller
 * ============================================================
 *  Connects to WiFi / Blynk and relays virtual-pin button
 *  presses to remote ESP32-C3 modules over ESP-NOW.
 *  Each module drives a servo to toggle a physical light switch.
 * ============================================================
 */

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <BlynkSimpleEsp32.h>

// ========================  CONFIGURABLE  ========================

// --- WiFi / Blynk credentials ---
#define WIFI_SSID           "YOUR_WIFI_SSID"
#define WIFI_PASS           "YOUR_WIFI_PASSWORD"
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "LightActuator"
#define BLYNK_AUTH_TOKEN    "YOUR_BLYNK_AUTH_TOKEN"

// --- Number of modules (max 20 for ESP-NOW peer limit) ---
#define NUM_MODULES 5

// --- Servo movement in degrees each toggle ---
#define SERVO_MOVE_DEGREES 20

// --- ESP-NOW channel (must match modules) ---
#define ESPNOW_CHANNEL 1

// --- Virtual pins mapped to each module (V0 → module 0, etc.) ---
//     Blynk buttons should be set to "Switch" mode (0/1).
#define VPIN_MODULE_0  V0
#define VPIN_MODULE_1  V1
#define VPIN_MODULE_2  V2
#define VPIN_MODULE_3  V3
#define VPIN_MODULE_4  V4

// --- MAC addresses of the 5 ESP32-C3 modules ---
//     Replace with the actual MAC of each module.
//     Run the module sketch once and check Serial for its MAC.
uint8_t moduleMacs[NUM_MODULES][6] = {
  {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x01},   // Module 0
  {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x02},   // Module 1
  {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x03},   // Module 2
  {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x04},   // Module 3
  {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0x05},   // Module 4
};

// ========================  END CONFIGURABLE  ====================

// --- Command packet sent over ESP-NOW ---
typedef struct {
  uint8_t  moduleId;       // which module (0-4)
  int16_t  moveDegrees;    // signed degrees to move
  uint8_t  toggleCmd;      // 1 = toggle the switch
} __attribute__((packed)) CmdPacket;

// Track send status
volatile bool lastSendOk = false;

// ----------------------------------------------------------------
//  ESP-NOW send callback
// ----------------------------------------------------------------
void onDataSent(const uint8_t *mac, esp_now_send_status_t status) {
  lastSendOk = (status == ESP_NOW_SEND_SUCCESS);
  Serial.printf("[ESP-NOW] Send to %02X:%02X:%02X:%02X:%02X:%02X – %s\n",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5],
    lastSendOk ? "OK" : "FAIL");
}

// ----------------------------------------------------------------
//  Register all module peers
// ----------------------------------------------------------------
void registerPeers() {
  for (int i = 0; i < NUM_MODULES; i++) {
    esp_now_peer_info_t peer = {};
    memcpy(peer.peer_addr, moduleMacs[i], 6);
    peer.channel = ESPNOW_CHANNEL;
    peer.encrypt = false;

    if (esp_now_add_peer(&peer) == ESP_OK) {
      Serial.printf("[ESP-NOW] Peer %d registered: %02X:%02X:%02X:%02X:%02X:%02X\n",
        i, moduleMacs[i][0], moduleMacs[i][1], moduleMacs[i][2],
        moduleMacs[i][3], moduleMacs[i][4], moduleMacs[i][5]);
    } else {
      Serial.printf("[ESP-NOW] FAILED to register peer %d\n", i);
    }
  }
}

// ----------------------------------------------------------------
//  Send toggle command to a specific module
// ----------------------------------------------------------------
void sendToggle(uint8_t moduleIndex) {
  if (moduleIndex >= NUM_MODULES) {
    Serial.printf("[ERR] Module index %d out of range\n", moduleIndex);
    return;
  }

  CmdPacket pkt;
  pkt.moduleId   = moduleIndex;
  pkt.moveDegrees = SERVO_MOVE_DEGREES;
  pkt.toggleCmd  = 1;

  esp_err_t result = esp_now_send(moduleMacs[moduleIndex],
                                   (uint8_t *)&pkt, sizeof(pkt));
  if (result == ESP_OK) {
    Serial.printf("[CMD] Toggle sent to module %d\n", moduleIndex);
  } else {
    Serial.printf("[CMD] Error sending to module %d: %d\n", moduleIndex, result);
  }
}

// ----------------------------------------------------------------
//  Blynk virtual-pin handlers  (button press → toggle module)
// ----------------------------------------------------------------
BLYNK_WRITE(VPIN_MODULE_0) {
  if (param.asInt() == 1) sendToggle(0);
}

BLYNK_WRITE(VPIN_MODULE_1) {
  if (param.asInt() == 1) sendToggle(1);
}

BLYNK_WRITE(VPIN_MODULE_2) {
  if (param.asInt() == 1) sendToggle(2);
}

BLYNK_WRITE(VPIN_MODULE_3) {
  if (param.asInt() == 1) sendToggle(3);
}

BLYNK_WRITE(VPIN_MODULE_4) {
  if (param.asInt() == 1) sendToggle(4);
}

// ----------------------------------------------------------------
//  Setup
// ----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== ESP32 Base Station ===");

  // --- WiFi ---
  WiFi.mode(WIFI_AP_STA);   // AP+STA needed for ESP-NOW alongside WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to WiFi");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 40) {
    delay(500);
    Serial.print(".");
    tries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("\nWiFi connected – IP: %s\n", WiFi.localIP().toString().c_str());
  } else {
    Serial.println("\nWiFi FAILED – Blynk will not work, but ESP-NOW will still run.");
  }

  // Force the STA interface to the ESP-NOW channel so peers line up
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  // Print own MAC (useful for debugging)
  Serial.printf("Base MAC (STA): %s\n", WiFi.macAddress().c_str());

  // --- ESP-NOW ---
  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERR] ESP-NOW init failed!");
    ESP.restart();
  }
  esp_now_register_send_cb(onDataSent);
  registerPeers();

  // --- Blynk ---
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect(5000);   // non-blocking 5 s timeout

  Serial.println("Setup complete.\n");
}

// ----------------------------------------------------------------
//  Loop
// ----------------------------------------------------------------
void loop() {
  Blynk.run();
}
