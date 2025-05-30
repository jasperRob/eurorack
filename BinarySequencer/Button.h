#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button {
private:
  bool state;
  int lastDebounceTime;

public:
  Button();
  void on();
  void off();
  void toggle();
  bool getState();
};

#endif
