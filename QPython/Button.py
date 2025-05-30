from machine import Pin
import utime


class Button:
    def __init__(self, pin_number):
        # Setting up as PULLUP (High/True means not pressed)
        self.pin: Pin = Pin(pin_number, Pin.IN, Pin.PULL_UP)
        self.last_pressed_time: int = 0  # ms
        self.last_state: bool = True  # Button not pressed initially
        self.debounce_delay: int = 200  # ms
        self.pressed: bool = False

    def read(self):
        current_time = utime.ticks_ms()  # Get the current time in milliseconds
        current_button_state = self.pin.value()  # Read the current state of the button
        # Check for a transient button press (HIGH to LOW transition)
        if self.last_state and not current_button_state:  # Button just pressed
            # Debounce - only process if sufficient time has passed since last press
            if (
                utime.ticks_diff(current_time, self.last_pressed_time)
                > self.debounce_delay
            ):
                self.last_pressed_time = current_time  # Update last press time
                self.pressed = True  # Button pressed
        self.pressed = False  # Button not pressed
