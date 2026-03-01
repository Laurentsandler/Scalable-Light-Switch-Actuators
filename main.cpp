/*
 * ============================================================
 *  ESP32-C3 Light Switch Module – ESP-NOW Servo Controller
 * ============================================================
 *  Receives toggle commands from the base station over ESP-NOW
 *  and moves the servo ±N degrees to flip a physical light switch.
 *
 *  Flash the SAME sketch to all 5 modules – each one is
 *  identified automatically by its unique MAC address.
 * ============================================================
 */

#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <ESP32Servo.h>

// ========================  CONFIGURABLE  ========================

// --- Servo ---
#define SERVO_PIN           14       // GPIO connected to servo signal
#define SERVO_CENTER        90       // Neutral / center angle (degrees)
#define SERVO_MOVE_DEGREES  20       // Degrees to move per toggle

// Servo pulse range (adjust for your exact servo if needed)
#define SERVO_MIN_US        500
#define SERVO_MAX_US        2400

// --- ESP-NOW ---
#define ESPNOW_CHANNEL      1       // Must match the base station

// --- Base station MAC (optional – used to filter senders) ---
//     Set to all zeros to accept commands from any sender.
uint8_t baseMac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
#define FILTER_SENDER false          // set true to only accept from baseMac

// --- Servo movement limits ---
#define SERVO_MIN_ANGLE     0
#define SERVO_MAX_ANGLE     180

// ========================  END CONFIGURABLE  ====================

// --- Command packet (must match base definition) ---
typedef struct {
  uint8_t  moduleId;
  int16_t  moveDegrees;
  uint8_t  toggleCmd;
} __attribute__((packed)) CmdPacket;

// --- Globals ---
Servo myServo;
int currentAngle  = SERVO_CENTER;   // start at center
int direction     = 1;               // +1 or -1, flips each toggle

// ----------------------------------------------------------------
//  Move servo to a target angle (clamped to safe range)
// ----------------------------------------------------------------
void moveServo(int targetAngle) {
  // Clamp
  if (targetAngle < SERVO_MIN_ANGLE) targetAngle = SERVO_MIN_ANGLE;
  if (targetAngle > SERVO_MAX_ANGLE) targetAngle = SERVO_MAX_ANGLE;

  Serial.printf("[SERVO] Moving %d° → %d°\n", currentAngle, targetAngle);
  myServo.write(targetAngle);
  currentAngle = targetAngle;
}

// ----------------------------------------------------------------
//  Handle toggle: move ±degrees, flip direction for next call
// ----------------------------------------------------------------
void handleToggle(int16_t degrees) {
  int target = currentAngle + (degrees * direction);
  moveServo(target);
  direction *= -1;   // reverse for next press
}

// ----------------------------------------------------------------
//  ESP-NOW receive callback
// ----------------------------------------------------------------
void onDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  // Optional sender filter
  if (FILTER_SENDER) {
    bool match = true;
    for (int i = 0; i < 6; i++) {
      if (info->src_addr[i] != baseMac[i]) { match = false; break; }
    }
    if (!match) {
      Serial.println("[ESP-NOW] Ignored – sender not base station");
      return;
    }
  }

  if (len < (int)sizeof(CmdPacket)) {
    Serial.printf("[ESP-NOW] Packet too short (%d bytes)\n", len);
    return;
  }

  CmdPacket pkt;
  memcpy(&pkt, data, sizeof(pkt));

  Serial.printf("[ESP-NOW] Received – module:%d  degrees:%d  toggle:%d\n",
    pkt.moduleId, pkt.moveDegrees, pkt.toggleCmd);

  if (pkt.toggleCmd == 1) {
    handleToggle(pkt.moveDegrees > 0 ? pkt.moveDegrees : SERVO_MOVE_DEGREES);
  }
}

// ----------------------------------------------------------------
//  Setup
// ----------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== ESP32-C3 Light Switch Module ===");

  // --- Servo ---
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, SERVO_MIN_US, SERVO_MAX_US);
  myServo.write(SERVO_CENTER);
  Serial.printf("[SERVO] Attached on GPIO %d – center %d°\n", SERVO_PIN, SERVO_CENTER);

  // --- WiFi (station mode, no connection – just for ESP-NOW) ---
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();            // make sure we're not connected to an AP
  esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE);

  // Print MAC so you can paste it into the base sketch
  Serial.printf("[INFO] Module MAC: %s  ← paste this into base sketch\n",
    WiFi.macAddress().c_str());

  // --- ESP-NOW ---
  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERR] ESP-NOW init failed!");
    ESP.restart();
  }
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("Setup complete – waiting for commands…\n");
}

// ----------------------------------------------------------------
//  Loop
// ----------------------------------------------------------------
void loop() {
  // Nothing to do – all work happens in the ESP-NOW callback.
  delay(10);
}
