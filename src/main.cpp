// Caterpillar bot - stepper + servo bring-up test
//
// On power-up / RESET:
//   1. Stepper: forward 5 s, pause, backward 5 s, then driver disabled.
//   2. Servo:   centre (90°) -> slowly to 0° -> slowly to 180° -> back to 90°.
// Then it stops. Press RESET on the Nano to run again.

#include <Arduino.h>
#include <Servo.h>
#include "pins.h"

// ---- Stepper settings -------------------------------------------------------
constexpr uint16_t STEPS_PER_SECOND = 10;      // full-step; very slow (~20 s/rev for a 1.8° motor)
constexpr uint32_t RUN_TIME_MS      = 5000;    // per direction
constexpr uint32_t PAUSE_MS         = 500;     // between directions
constexpr uint32_t STEP_INTERVAL_US = 1000000UL / STEPS_PER_SECOND;

// ---- Servo settings ---------------------------------------------------------
constexpr uint8_t  SERVO_CENTER_DEG = 90;
constexpr uint8_t  SERVO_MIN_DEG    = 0;
constexpr uint8_t  SERVO_MAX_DEG    = 180;
constexpr uint16_t SERVO_STEP_MS    = 15;      // ms per degree -> 180° takes ~2.7 s
// Pulse width range. Library default is 544..2400 us. Many 60 kg servos use
// 500..2500 us; adjust if the servo doesn't reach the ends or buzzes at them.
constexpr uint16_t SERVO_MIN_US     = 544;
constexpr uint16_t SERVO_MAX_US     = 2400;

Servo servo;
uint8_t servoAngle = SERVO_CENTER_DEG;

void stepperRunFor(bool forward, uint32_t durationMs) {
  digitalWrite(PIN_STEPPER_DIR, forward ? HIGH : LOW);
  delayMicroseconds(5);                         // DIR setup time

  Serial.println(forward ? F("stepper forward") : F("stepper backward"));
  uint32_t start = millis();
  while (millis() - start < durationMs) {
    digitalWrite(PIN_STEPPER_STEP, HIGH);
    delayMicroseconds(2);                       // A4988 needs >= 1 us
    digitalWrite(PIN_STEPPER_STEP, LOW);
    delay(STEP_INTERVAL_US / 1000);             // interval > 16 ms, so use delay()
  }
}

// Move one degree at a time: smooth motion and lower current spikes than a jump.
void servoMoveSlow(uint8_t target) {
  Serial.print(F("servo -> ")); Serial.println(target);
  while (servoAngle != target) {
    servoAngle += (target > servoAngle) ? 1 : -1;
    servo.write(servoAngle);
    delay(SERVO_STEP_MS);
  }
}

void setup() {
  // Stepper pins: driver disabled first so nothing moves unexpectedly.
  pinMode(PIN_STEPPER_EN, OUTPUT);
  digitalWrite(PIN_STEPPER_EN, STEPPER_DISABLE_LEVEL);
  pinMode(PIN_STEPPER_STEP, OUTPUT);
  digitalWrite(PIN_STEPPER_STEP, LOW);
  pinMode(PIN_STEPPER_DIR, OUTPUT);

  // Servo: set the start angle before attach so the first pulse is 90°.
  servo.write(SERVO_CENTER_DEG);
  servo.attach(PIN_SERVO, SERVO_MIN_US, SERVO_MAX_US);

  Serial.begin(115200);
  delay(1000);                                  // let supplies settle

  // 1. Stepper test
  digitalWrite(PIN_STEPPER_EN, STEPPER_ENABLE_LEVEL);
  stepperRunFor(true, RUN_TIME_MS);
  delay(PAUSE_MS);
  stepperRunFor(false, RUN_TIME_MS);
  digitalWrite(PIN_STEPPER_EN, STEPPER_DISABLE_LEVEL);

  // 2. Servo test
  delay(PAUSE_MS);
  servoMoveSlow(SERVO_MIN_DEG);
  delay(PAUSE_MS);
  servoMoveSlow(SERVO_MAX_DEG);
  delay(PAUSE_MS);
  servoMoveSlow(SERVO_CENTER_DEG);

  Serial.println(F("done - press RESET to repeat"));
}

void loop() {}
