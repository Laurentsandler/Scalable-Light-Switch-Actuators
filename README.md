# Scalable Light Switch Actuators

Small battery-powered automatic light switch actuators powered by ESP32.

## Description

A solution for renters who cannot install smart light switches. These small, battery-powered actuators mount on the outside of existing light switches and physically toggle them using a servo motor. They connect together over ESP-NOW (no Wi-Fi needed per module) via a central base station that handles Blynk/HomeKit integration. Up to 5 modules can be controlled from one base station.

## My Motivation

I live in a rented house, so I cannot install smart light switches. The only solutions on the market are bulky actuators that use a proprietary remote and don't support smart home platforms like Amazon Alexa, Home Assistant, Zigbee, or Apple HomeKit. So I decided to build my own using a custom PCB.

## Hardware Design

The custom PCB was designed using EasyEDA. The PCB dimensions are 32 × 26 mm — fitting perfectly on the back of an SG90 servo.

### PCB Layout & 3D Renders

*(Images to be added to the PCB/ folder)*

### Case

The case was designed in Onshape — as small as possible while fitting all components, with a snap-on lid and space for wires.

*(Images to be added to the Images/ folder)*

## Features

- **Servo Control**: Physically toggles light switches ±20° via SG90 servo
- **ESP-NOW**: Low-power mesh-style communication between modules and base station — no per-module Wi-Fi needed
- **Blynk Integration**: Control up to 5 switches via Blynk virtual pins V0–V4
- **Apple HomeKit / BLE**: Supported via ESPHome/Matter on the ESP32-C3
- **Long Battery Life**: ~2 months per module using deep sleep + ESP-NOW architecture
- **USB-C Charging**: On-board battery charging and power path management
- **Physical Button**: User can still manually toggle the switch via button on the device
- **Compact PCB**: 32 × 26 mm, fits on the back of an SG90 servo
- **Snap-fit 3D Printed Case**: Designed in Onshape, easily printable

## System Architecture

```
[Blynk App] → [Base ESP32 (WiFi)] --ESP-NOW--> [Module 1 (ESP32-C3)] → Servo 1
                                             --ESP-NOW--> [Module 2 (ESP32-C3)] → Servo 2
                                             --ESP-NOW--> [Module 3 (ESP32-C3)] → Servo 3
                                             --ESP-NOW--> [Module 4 (ESP32-C3)] → Servo 4
                                             --ESP-NOW--> [Module 5 (ESP32-C3)] → Servo 5
```

## Hardware Requirements

| Component | Specification | Notes |
|-----------|---------------|-------|
| Module MCU | ESP32-C3-WROOM-02 | ~18×20mm, ~5µA deep sleep, Wi-Fi + BLE |
| Base MCU | ESP32 Dev Board | Always-on, handles Blynk & ESP-NOW relay |
| Servo | SG90 | 3 wires soldered to PCB bottom |
| Battery | LiPo (fits SG90 footprint) | High capacity, fits behind PCB+servo |
| Charging IC | On-board (USB-C) | Power path management, 5V→3.3V step-down |
| USB-Serial | CH340 | On-board for programming |
| Button | Tactile | Physical switch toggle |

## Software Dependencies

Install these libraries via Arduino IDE Library Manager:
- WiFi (built-in)
- ESP-NOW (built-in)
- BlynkSimpleEsp32 (Blynk library)
- ESP32Servo

## Configuration

### Module Setup
1. Flash each ESP32-C3 module with the Module firmware
2. Note the MAC address printed on Serial for each module
3. Paste the MAC addresses into the Base firmware

### Base Setup
Edit the following in `Firmware/base/base.ino`:

```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASSWORD";
const char* BLYNK_AUTH = "YOUR_BLYNK_AUTH_TOKEN";

uint8_t module1MAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Replace with actual MACs
uint8_t module2MAC[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// ... etc
```

## Usage

### Blynk App Setup
1. Create a new Blynk project
2. Add 5 Button widgets on virtual pins **V0–V4**
3. Set each button to **Push mode**, Integer **0/1**
4. Each V0–V4 button controls Module 1–5 respectively

### Physical Button
- Press the button on any module to manually toggle the light switch

### LED Status Indicators
- **Solid**: Connected and idle
- **Blinking**: Receiving command / moving servo

## API Services Used

- **Blynk**: Remote switch control (V0–V4)
- **ESP-NOW**: Local wireless communication between base and modules

## BOM

| Component | Footprint | Quantity | Notes | Est. Price |
|-----------|-----------|----------|-------|------------|
| ESP32-C3-WROOM-02 | SMD Module | 1 per module | ~18×20mm | ~$3 |
| SG90 Servo | Through-hole wires | 1 per module | Standard micro servo | ~$2 |
| LiPo Battery | Fits SG90 footprint | 1 per module | High capacity | ~$5 |
| CH340 USB-Serial | SMD | 1 per module | For programming | ~$0.50 |
| USB-C Connector | SMD | 1 per module | For charging | ~$0.30 |
| Tactile Button | SMD | 1 per module | Physical toggle | ~$0.10 |
| ESP32 Dev Board | Through-hole | 1 (base only) | Always-on base station | ~$5 |

## License

MIT License
