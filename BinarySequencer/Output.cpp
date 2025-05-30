#include "Output.h"
#include <Arduino.h>

Output::Output(int pin) {
  pin = pin;
  pinMode(pin, OUTPUT);
  state = false;
}

void Output::write() { digitalWrite(pin, state ? HIGH : LOW); }

bool Output::getState() { return state; }
