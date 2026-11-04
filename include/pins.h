#pragma once
// Caterpillar bot - Arduino Nano pin map
// Source of truth for wiring: docs/WIRING.md

#include <Arduino.h>

// ---- A4988 stepper driver --------------------------------------------------
// MS1/MS2/MS3 unconnected -> full-step mode.
// RESET and SLEEP tied to 5V (driver always awake).
constexpr uint8_t PIN_STEPPER_STEP = 3;   // D3 -> A4988 STEP
constexpr uint8_t PIN_STEPPER_DIR  = 4;   // D4 -> A4988 DIR
constexpr uint8_t PIN_STEPPER_EN   = 5;   // D5 -> A4988 EN (active LOW)

constexpr uint8_t STEPPER_ENABLE_LEVEL  = LOW;
constexpr uint8_t STEPPER_DISABLE_LEVEL = HIGH;

// ---- Servo (60 kg·cm, powered from 7.4V buck) -------------------------------
constexpr uint8_t PIN_SERVO = 9;          // D9 -> servo signal
// Note: the Servo library uses Timer1, so analogWrite() on D9/D10 is unavailable.
