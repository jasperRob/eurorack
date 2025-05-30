#include "Bank.h"
#include "Output.h"
#include <Arduino.h>

Bank::Bank(Output *out) { output = out; }

void Bank::setStepState(int stepIndex, bool state) {
  if (stepIndex >= 0 && stepIndex < 16) {
    if (state) {
      steps[stepIndex].on();
    } else {
      steps[stepIndex].off();
    }
  }
}

void Bank::toggleStep(int stepIndex) {
  if (stepIndex >= 0 && stepIndex < 16) {
    steps[stepIndex].toggle();
  }
}
