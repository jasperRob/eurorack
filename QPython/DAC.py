from machine import Pin, I2C
import struct


class DAC:
    def __init__(self, address):
        # Initialize I2C (I2C0: SDA = GP0, SCL = GP1)
        self.i2c: I2C = I2C(0, scl=Pin(1), sda=Pin(0), freq=400000)
        # I2C addresses of MCP4725 ICs (DAC - Voltage Output)
        self.address: int = address

    def write(self, voltage):
        """Set DAC output voltage (0-3.3V mapped to 0-4095)"""
        # Convert voltage to 12-bit value
        value = int((voltage / 3.3) * 4095)
        # MCP4725 uses left-aligned 12-bit
        data = struct.pack(">H", value << 4)
        # Send data over I2C
        self.i2c.writeto(self.address, data)
