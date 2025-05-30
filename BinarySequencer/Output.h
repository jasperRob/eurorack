#ifndef OUTPUT_H
#define OUTPUT_H

#include <Arduino.h>

class Output {
private:
  int pin;
  bool state;

public:
  Output(int pin);
  void write();
  bool getState();
};

#endif
