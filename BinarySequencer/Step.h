#ifndef STEP_H
#define STEP_H

#include <Arduino.h>

class Step {
private:
  bool state;

public:
  Step();
  void on();
  void off();
  void toggle();
  bool getState();
};

#endif
