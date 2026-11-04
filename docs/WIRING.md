# Caterpillar Hardware — Components and Wiring

## 1. Components

| Component | Specifications |
|---|---|
| Microcontroller | Arduino Nano (ATmega328P) |
| Servo motor | 60 kg·cm high-torque servo |
| Servo power supply | Buck converter, 7.4V output |
| Stepper motor | D42HSCL413B-24 |
| Stepper lot number | 190515 |
| Stepper connector | 6 pins, only 4 wires brought out |
| Stepper driver | A4988 red driver module |
| Stepper driver motor supply | External regulated 12V DC (planned) |
| Capacitor | 100 µF, 25V or higher, across VMOT and GND |

## 2. Stepper Motor Specifications

| Parameter | Value |
|---|---|
| Model | D42HSCL413B-24 |
| Type | Stepper motor with two accessible isolated windings |
| Connector | 6 pins |
| Available wires | 4 |
| Winding 1 resistance | 6 Ω |
| Winding 2 resistance | 7.2 Ω |
| Continuity between windings | No continuity |
| Rated phase current | Not yet confirmed |
| Step angle | Not yet confirmed |

## 3. Arduino Nano to A4988 Connections

| Arduino Nano | A4988 Driver |
|---|---|
| D3 | STEP |
| D4 | DIR |
| D5 | EN |
| 5V | VDD |
| 5V | RESET |
| 5V | SLEEP |
| GND | Logic GND |
| Unconnected | MS1 |
| Unconnected | MS2 |
| Unconnected | MS3 |

MS1, MS2 and MS3 are left unconnected for full-step operation on a standard A4988 module.

## 4. Stepper Motor to A4988 Connections

| Stepper Motor | A4988 |
|---|---|
| Winding 1 — Wire 1 (6 Ω pair) | 1A |
| Winding 1 — Wire 2 (6 Ω pair) | 1B |
| Winding 2 — Wire 1 (7.2 Ω pair) | 2A |
| Winding 2 — Wire 2 (7.2 Ω pair) | 2B |

Only the four identified winding wires are used. The unused center-tap connections remain disconnected.

## 5. Servo Motor Connections

| Servo Motor | Connection |
|---|---|
| Red (V+) | Buck converter OUT+ (7.4V) |
| Black/Brown (GND) | Buck converter OUT− |
| Yellow/Orange (signal) | Arduino Nano D9 |
| Common GND | Arduino Nano GND |

Servo voltage compatibility and buck converter current capacity must be verified against the exact servo model.

## 6. A4988 Power Connections

| Connection | Destination |
|---|---|
| VMOT | External regulated 12V DC positive |
| GND (beside VMOT) | External 12V negative |
| VDD | Arduino Nano 5V |
| Logic GND | Arduino Nano GND |
| Capacitor positive | VMOT |
| Capacitor negative | GND beside VMOT |
| Arduino Nano power | USB during testing |

All control grounds must be common. Keep the 12V and 7.4V positive supply rails separate.

**Note:** These are the selected wiring connections, not a claim that powered operation has been verified. The A4988 current limit must be set appropriately before running the stepper motor.

## 7. Bring-up Notes (measured)

| Item | Value / Result |
|---|---|
| A4988 Vref (12V connected) | 0.5 V |
| Stepper motion | Forward / backward verified at low speed |
| Stepper coil order on A4988 (top → bottom) | 2B, 2A, 1B, 1A — each adjacent pair is one coil |
| Nano bootloader | New (PlatformIO env `nano`), CH340 USB |
| Servo | Wired per section 5; motion not yet verified |
