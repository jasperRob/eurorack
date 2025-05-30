#include "Button.h"
#include <Arduino.h>

Button::Button() { state = false; }

void Button::on() { state = true; }

void Button::off() { state = false; }

void Button::toggle() { state = !state; }

bool Button::getState() { return state; }
