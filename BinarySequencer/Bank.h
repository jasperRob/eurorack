#ifndef BANK_H
#define BANK_H

#include "Output.h"
#include "Step.h"
#include <Arduino.h>

class Bank {
private:
  Output *output;
  Step steps[16];

public:
  Bank(Output *out);
  void setStepState(int stepIndex, bool state);
  void toggleStep(int stepIndex);
};

#endif
