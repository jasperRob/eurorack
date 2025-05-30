from machine import Pin  # ruff: noqa


class Encoder:
    def __init__(self, pin_number_a, pin_number_b, min, max):
        self.enc_a: Pin = Pin(pin_number_a, Pin.IN, Pin.PULL_UP)
        self.enc_b: Pin = Pin(pin_number_b, Pin.IN, Pin.PULL_UP)
        self.last_a_state: bool = False
        self.position: int = 0
        self.min: int = min
        self.max: int = max
        self.up: bool = False
        self.down: bool = False
        self.read()

    def read(self):
        # Read the current state of the A pin
        a_state = self.enc_a.value()
        self.up = False
        self.down = False
        # Detect a change in state
        if a_state != self.last_a_state:  # If A pin changes
            if (
                self.enc_b.value() != a_state
            ):  # Check the state of B pin to determine direction
                self.position += 1  # Clockwise rotation
                self.up = True
            else:
                self.position -= 1  # Counter-clockwise rotation
                self.down = True
        self.last_a_state = a_state  # Update the last state of A
        # Constrain the position to a range (0 to 100)
        self.position = max(self.min, min(self.max, self.position))
