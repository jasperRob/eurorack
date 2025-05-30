#include "Input.h"
#include <Arduino.h>

Input::Input(int pin) {
  pin = pin;
  pinMode(pin, INPUT);
}

int Input::getPin() { return pin; }

byte Input::read() { digitalRead(pin); }
