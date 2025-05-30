#include "Step.h"
#include <Arduino.h>

Step::Step() { state = false; }

void Step::on() { state = true; }

void Step::off() { state = false; }

void Step::toggle() { state = !state; }

bool Step::getState() { return state; }
