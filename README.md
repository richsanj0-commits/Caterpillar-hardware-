# Caterpillar Bot — Hardware Firmware

PlatformIO firmware for the caterpillar bot on an **Arduino Nano (ATmega328P)**:
one A4988-driven stepper and one 60 kg·cm servo.

- Pin map: [include/pins.h](include/pins.h)
- Full wiring & components: [docs/WIRING.md](docs/WIRING.md)

## Pin summary

| Nano | Goes to |
|---|---|
| D3 | A4988 STEP |
| D4 | A4988 DIR |
| D5 | A4988 EN (active LOW) |
| D9 | Servo signal |
| 5V | A4988 VDD, RESET, SLEEP |
| GND | A4988 logic GND, buck OUT− (servo GND), 12V GND — all common |

## Build / upload

```sh
pio run                         # build
pio run -t upload               # upload (our Nano uses the new bootloader -> env "nano")
pio run -e nano_old -t upload   # only for clones with the old bootloader
pio device monitor              # serial monitor @ 115200
```

In VS Code: PlatformIO toolbar at the bottom (✓ build, → upload, plug = serial monitor).
If the toolbar is missing, run **Developer: Reload Window**.

## Current firmware — bring-up test ([src/main.cpp](src/main.cpp))

Runs once on power-up / RESET, then stops:

1. **Stepper:** forward 5 s → pause 0.5 s → backward 5 s → driver disabled.
   Speed `STEPS_PER_SECOND = 10` (full-step, ~20 s/rev for a 1.8° motor).
2. **Servo:** 90° → slowly to 0° → slowly to 180° → back to 90° (1°/15 ms).
   Pulse range `SERVO_MIN_US`/`SERVO_MAX_US` = 544/2400 µs (try 500/2500 if it doesn't reach the ends).

## Bring-up status

| Item | Status |
|---|---|
| PlatformIO build (nano, nano_old) | ✅ builds |
| Upload to Nano (CH340, `nano` env) | ✅ works |
| Stepper forward / backward | ✅ working (after fixing coil pairing) |
| A4988 Vref | 0.5 V measured with 12V connected |
| Servo | ⏳ not moving yet — under investigation |

## Stepper notes (lessons learned)

- Motor **vibrating but not spinning** = coil wires crossed between pin pairs.
  Each pair (2B–2A, 1B–1A) must be one coil (~6–7 Ω); across pairs must read open.
- Swapping the two wires *within* one coil only reverses direction.
- Measure Vref **with 12V connected** (red probe on pot screw, black on GND).
  Without VMOT the reading is meaningless (we saw 4.6 V).
- Never connect/disconnect the motor while VMOT is powered.

## Next steps

- Get the servo moving (check common GND, D9 signal, buck output voltage/current).
- Autonomous-rover sequence: home → move to set points → return to start, repeat forever.
  Recommended: home/limit switch on a spare pin, accel/decel ramps (AccelStepper), driver kept enabled.
