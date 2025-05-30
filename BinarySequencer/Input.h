#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

class Input {
private:
  int pin;
  bool state;

public:
  Input(int pin);
  int getPin();
  byte read();
};

#endif
