# Wiring Diagram

This document describes the hardware connections for the Scalable Light Switch Actuator modules.

## Module Connections (ESP32-C3 PCB)

### SG90 Servo
The servo wires are soldered directly to pads on the bottom of the PCB next to the USB-C port.

```
Servo Wire  →  PCB Pad
----------------------
Red (VCC)   →  5V / VBAT
Brown (GND) →  GND
Orange (PWM)→  GPIO5
```

### Physical Button
```
Button      →  ESP32-C3 Pin
----------------------------
One side    →  GPIO9
Other side  →  GND
(INPUT_PULLUP mode - no external resistor needed)
```

### USB-C (Charging & Programming)
- USB-C port on PCB is used for both **charging the battery** and **programming the ESP32-C3** via the on-board CH340 USB-serial chip.

### Battery
- LiPo battery connects via JST connector on the PCB
- Charging is handled by the on-board charging IC
- Power path management automatically switches between USB-C power and battery

## Base Station Connections

The base station is a standard ESP32 dev board connected to USB power (always-on).

```
ESP32 Dev Board  →  Power
--------------------------
USB              →  Wall adapter (always on)
```

No additional wiring needed — the base station communicates wirelessly via ESP-NOW.

## PCB Block Diagram

```
USB-C ──► [Power Path IC] ──► 3.3V Rail ──► ESP32-C3
              │                              │
              ▼                              ├──► CH340 (USB-Serial)
         [Charger IC]                        ├──► Button (GPIO9)
              │                              └──► Servo PWM (GPIO5)
              ▼
          [LiPo Battery]
```

## Notes

- PCB dimensions: 32 × 26 mm
- All SMD components for compact form factor
- The PCB mounts directly on the back of the SG90 servo
