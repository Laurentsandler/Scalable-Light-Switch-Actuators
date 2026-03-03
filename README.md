# Scalable Light Switch Actuators

Small battery-powered automatic light switch actuators powered by ESP32.

## Description

A solution for renters who cannot install smart light switches. These small, battery-powered actuators mount on the outside of existing light switches and physically toggle them using a servo motor. They connect together over ESP-NOW (no Wi-Fi needed per module) via a central base station that handles Blynk/HomeKit integration. Up to 20 modules can be controlled from one base station, due to EspNow restrictions

## My Motivation

I live in a rented house, so I cannot install smart light switches. The only solutions on the market are bulky actuators that use a proprietary remote and don't support smart home platforms like Amazon Alexa, Home Assistant, Zigbee, or Apple HomeKit. So I decided to build my own using a custom PCB.

## Hardware Design

The custom PCB was designed using EasyEDA. The PCB dimensions are 32 × 26 mm — fitting perfectly on the back of an SG90 servo.

### PCB Layout & 3D Renders

<img width="705" height="845" alt="image" src="https://github.com/user-attachments/assets/02a7d980-9cd9-42ca-b489-f4b5d95e7b9a" />
<img width="705" height="845" alt="image" src="https://github.com/user-attachments/assets/d8565e2a-75a2-4d45-8789-cd2af2e62995" />
<img width="705" height="845" alt="image" src="https://github.com/user-attachments/assets/318e321e-d93b-4aa1-a08c-ff579ac4a2ea" />


### Case

The case was designed in Onshape — as small as possible while fitting all components, with a snap-on lid and space for wires.

<img width="1368" height="1072" alt="image" src="https://github.com/user-attachments/assets/65cb1400-1bab-4378-80f6-4633a3bccadf" />
<img width="1552" height="1234" alt="image" src="https://github.com/user-attachments/assets/7d52b6f8-f530-4bfa-adb7-00ff565dbac0" />



## Onshape project link: https://cad.onshape.com/documents/06b28d6b3b71f47cc2911267/w/042815538e1a38e5d56684fa/e/e83a82b9ce472ea6086be9af?renderMode=0&uiState=69a43da28d82e5f4f3067d34

### 3d mockups

<img width="1552" height="1234" alt="image" src="https://github.com/user-attachments/assets/18cc6f26-a934-4de5-a8de-564f1d644599" />
<img width="1552" height="1234" alt="image" src="https://github.com/user-attachments/assets/542827b6-aa6c-4765-b5f2-80a8c4169754" />
<img width="1552" height="1234" alt="image" src="https://github.com/user-attachments/assets/81ba60f7-362b-48e3-9950-b7a97d8cbdaa" />
<img width="1552" height="1234" alt="image" src="https://github.com/user-attachments/assets/daf7cc10-0c4c-4fe1-acea-8b414793bbe7" />



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
[Blynk App] → [Base ESP32 (WiFi)]            --ESP-NOW--> [Module 1 (ESP32-C3)] → Servo 1
                                             --ESP-NOW--> [Module 2 (ESP32-C3)] → Servo 2
                                             --ESP-NOW--> [Module 3 (ESP32-C3)] → Servo 3
                                             --ESP-NOW--> [Module 4 (ESP32-C3)] → Servo 4
                                             --ESP-NOW--> [Module 5 (ESP32-C3)] → Servo 5
```

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


## API Services Used

- **Blynk**: Remote switch control (V0–V4)
- **ESP-NOW**: Local wireless communication between base and modules

## BOM


| ID | Name | Designator | Footprint | Quantity | Manufacturer Part | Manufacturer | Supplier | Supplier Part | Price | Pins | 3D Model | Contributor | JLCPCB Part Class | Value | Link |
|----|------|------------|-----------|----------|-------------------|--------------|----------|---------------|-------|------|----------|-------------|-------------------|-------|------|
| 1 | 10uF | C1,C5,C8,C9 | C0603 | 4 | CL10A106KP8NNNC | SAMSUNG | LCSC | C19702 | 0.007 | 2 | C0603_L1.6-W0.8-H0.8 | lcsc | Basic Part | | https://item.szlcsc.com/362304.html |
| 2 | 100nF | C4,C6 | C0603 | 2 | CC0603KRX7R9BB104 | YAGEO | LCSC | C14663 | 0.002 | 2 | C0603_L1.6-W0.8-H0.8 | lcsc | Basic Part | | https://item.szlcsc.com/362304.html |
| 3 | 1uF | C7 | C0805 | 1 | CL21B105KBFNNNE | SAMSUNG | LCSC | C28323 | 0.01 | 2 | C0805_L2.0-W1.3-H1.3 | lcsc | Basic Part | | https://item.szlcsc.com/373011.html |
| 4 | B2B-PH-K-S(LF)(SN) | CN1 | CONN-TH_B2B-PH-K-S | 1 | B2B-PH-K-S(LF)(SN) | JST | LCSC | C131337 | 0.031 | 2 | CONN-TH_PH2.00-LI-2P | LCSC | Extended Part | | https://item.szlcsc.com/142630.html |
| 5 | B5819W_C8598 | D1 | SOD-123_L2.7-W1.6-LS3.7-RD-1 | 1 | B5819W SL | CJ | LCSC | C8598 | 0.024 | 2 | SOD-123_L2.7-W1.6-LS3.7-RD-1 | lcsc | Basic Part | | https://atta.szlcsc.com/upload/public/pdf/source/20210423/C2828417_EF1BF8BE3D48F97E320B09B073C89160.pdf |
| 6 | AO3401A | Q1 | SOT-23_L2.9-W1.3-P1.90-LS2.4-BR | 1 | AO3401A | AOS | LCSC | C15127 | 0.057 | 3 | SOT-23-3P_L2.9-W1.3-H1.0-LS2.4-P0.95 | lcsc | Basic Part | | https://item.szlcsc.com/236142.html |
| 7 | 5.1kΩ | R1,R2 | R0603 | 2 | 0603WAF5101T5E | UNI-ROYAL | LCSC | C23186 | 0.001 | 2 | R0603 | lcsc | Basic Part | | https://www.mouser.in/datasheet/2/447/PYu_RT_1_to_0_01_RoHS_L_11-1669912.pdf |
| 8 | 100kΩ | R3 | R0603 | 1 | 0603WAF1003T5E | UNI-ROYAL | LCSC | C25803 | 0.001 | 2 | R0603 | lcsc | Basic Part | | https://www.mouser.in/datasheet/2/447/PYu_RT_1_to_0_01_RoHS_L_11-1669912.pdf |
| 9 | 2kΩ | R4 | R0603 | 1 | 0603WAF2001T5E | UNI-ROYAL | LCSC | C22975 | 0.001 | 2 | R0603 | lcsc | Basic Part | | https://www.mouser.in/datasheet/2/447/PYu_RT_1_to_0_01_RoHS_L_11-1669912.pdf |
| 10 | 10kΩ | R5,R6,R7,R8 | R0603 | 4 | 0603WAF1002T5E | UNI-ROYAL | LCSC | C25804 | 0.001 | 2 | R0603 | lcsc | Basic Part | | https://www.mouser.in/datasheet/2/447/PYu_RT_1_to_0_01_RoHS_L_11-1669912.pdf |
| 11 | TS-1088-AR02016 | SW1,SW2 | SW-SMD_L3.9-W3.0-P4.45 | 2 | TS-1088-AR02016 | XUNPU | LCSC | C720477 | 0.048 | 2 | SW-SMD_L3.9-W2.9-H2.0-LS4.8 | LCSC | Basic Part | | https://item.szlcsc.com/457636.html |
| 12 | TP4054A | U1 | SOT-23-5_L2.9-W1.6-P0.95-LS2.8-TL | 1 | TP4054A | YONGYUTAI | LCSC | C33551022 | 0.031 | 5 | SOT-23-5_L2.9-W1.6-P0.95-LS2.8-TL | lcsc | Extended Part | | https://item.szlcsc.com/2162298.html |
| 13 | ESP32-C3-WROOM-02-N4(4MB) | U5 | BULETM-SMD_ESPRESSIF_ESP32-C3-WROOM-02-N4-4MB | 1 | ESP32-C3-WROOM-02-N4 | ESPRESSIF | LCSC | C2934560 | 3.067 | 27 | BULETM-SMD_ESPRESSIF_ESP32-C3-WROOM-02-N4-4MB | LCSC | Extended Part | | https://atta.szlcsc.com/upload/public/pdf/source/20220106/F07459EFFE1D7CBA94B4FB8A075146A6.pdf |
| 14 | CH340N_C2977777 | U6 | SOP-8_L4.9-W3.9-P1.27-LS6.0-BL | 1 | CH340N | WCH | LCSC | C2977777 | 0.561 | 8 | SOP-8_L4.9-W3.9-P1.27-LS6.0-BL-1 | LCSC | Extended Part | | https://item.szlcsc.com/383602.html |
| 15 | TPS73633DBVR | U7 | SOT-23-5_L3.0-W1.7-P0.95-LS2.8-BR | 1 | TPS73633DBVR | TI | LCSC | C28038 | 0.422 | 5 | SOT-23-5_L2.9-W1.6-H1.1-LS2.8-P0.95 | lcsc | Extended Part | | https://item.szlcsc.com/236104.html |
| 16 | TYPE-C-31-M-12 | USBC1 | USB-C_SMD-TYPE-C-31-M-12_1 | 1 | TYPE-C-31-M-12 | 韩国韩荣 | LCSC | C165948 | 0.183 | 16 | USB-C_SMD-TYPE-C-31-M-12_1 | LCSC | Extended Part | | https://item.szlcsc.com/datasheet/TYPE-C-31-M-12/177331.html |
| 17 | R0603 | R9 | R0603 | 1 | 0603WAD1001T5E | UNI-ROYAL | LCSC | C51218 | 0.007 | 2 | R0603 | lcsc | Extended Part | 1kΩ | https://lcsc.com/eda_search?q=C51218&%26type=1&ref=editor |
| 18 | LED0805-R-RD | LED2 | LED0805-R-RD | 1 | 17-215UYC/S530-A3/TR8 | EVERLIGHT | LCSC | C72036 | 0.054 | 2 | LED0805-RD | lcsc | Extended Part | | https://lcsc.com/eda_search?q=C72036&%26type=1&ref=editor |
| 19 | SG90 Micro Servo Motor 9G | | N/A | 5 | SG90 | SopwMap | Amazon | B0DP3VG59G | 11.99 | 3 | | | | | https://www.amazon.com/dp/B0DP3VG59G |
| 20 | 582728 400mAh 3.7V LiPo Battery | | N/A | 1 | 582728 | Rainbow Battery Store | AliExpress | 1005007648840057 | 5.31 | 2 | | | | | https://www.aliexpress.com/item/1005007648840057.html |
| 21 | ESP32 DevKitC WIFI+Bluetooth Development Board (ESP32-32U) | | N/A | 1 | ESP32-32U | YouKeyi Store | AliExpress | 1005008949552110 | 5.09 | | | | | https://www.aliexpress.com/item/1005008949552110.html |

## License

MIT License
