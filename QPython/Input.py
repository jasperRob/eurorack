from machine import Pin, ADC


class Input:
    def __init__(self, pin_number):
        self.adc: ADC = ADC(Pin(pin_number))
        self.voltage: float
        self.read()

    def read(self):
        """Read the voltage from the adc pin"""
        # Read analog value (0 to 65535)
        analog_value = self.adc.read_u16()
        # Convert to voltage (0V to 5V)
        self.voltage = (analog_value / 65535) * 5
