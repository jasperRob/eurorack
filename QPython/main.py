# from enum import Enum
from Quantizer.Button import Button
from Quantizer.DAC import DAC
from Quantizer.Encoder import Encoder
from Quantizer.Input import Input
import numpy as np


class Quantizer:
    def __init__(self):
        # Initialize buttons
        self.encoder_button = Button(15)  # Button pin (GP15) - For Encoder Button
        # Initialize encoders
        self.encoder = Encoder(28, 29, 0, 100)  # (GP28, GP29)
        # Initialize Analog Inputs
        self.input0 = Input(26)
        # Initialize Output DACs
        self.output0 = DAC(0x60)
        self.output1 = DAC(0x61)
        # settings and vars
        self.scale = 0  # C Major
        self.shift = 2.5

        # Musical scales
        self.scales = {
            "C Major": ["C", "D", "E", "F", "G", "A", "B"],
            "D Minor": ["D", "E", "F", "G", "A", "A#", "C"],
            "G Major": ["G", "A", "B", "C", "D", "E", "F#"],
            "A Minor": ["A", "B", "C", "D", "E", "F", "G"],
            "C Blues": ["C", "Eb", "F", "F#", "G", "Bb"],
            "Chromatic": [
                "C",
                "C#",
                "D",
                "D#",
                "E",
                "F",
                "F#",
                "G",
                "G#",
                "A",
                "A#",
                "B",
            ],
            "C Dorian": ["C", "D", "Eb", "F", "G", "A", "Bb"],
            "C Phrygian": ["C", "Db", "Eb", "F", "G", "Ab", "Bb"],
            "C Lydian": ["C", "D", "E", "F#", "G", "A", "B"],
            "C Mixolydian": ["C", "D", "E", "F", "G", "A", "Bb"],
            "C Harmonic Minor": ["C", "D", "Eb", "F", "G", "Ab", "B"],
            "C Melodic Minor": ["C", "D", "Eb", "F", "G", "A", "B"],
            "C Whole Tone": ["C", "D", "E", "F#", "G#", "A#"],
            "C Major Pentatonic": ["C", "D", "E", "G", "A"],
            "A Minor Pentatonic": ["A", "C", "D", "E", "G"],
            "C Octatonic": ["C", "D", "D#", "F", "F#", "G#", "A", "B"],
            "C Enigmatic": ["C", "D", "E", "F#", "G#", "A", "B"],
            "C Double Harmonic Major": ["C", "Db", "E", "F", "G", "Ab", "B"],
        }
        # Standard 12-TET chromatic scale
        self.chromatic_scale = [
            "C",
            "C#",
            "D",
            "D#",
            "E",
            "F",
            "F#",
            "G",
            "G#",
            "A",
            "A#",
            "B",
        ]
        # Map each note in chromatic scale to its index
        self.note_to_index = {
            note: index for index, note in enumerate(self.chromatic_scale)
        }

    def update_settings(self):
        """Update the system settings based on encoder movement"""
        if self.encoder.up:
            self.scale += 1
        elif self.encoder.down:
            self.scale -= 1
        self.select_scale(self.scales)

    def select_scale(self, scales):
        """Function to select scale based on input voltage (automatically adjusts for any number of scales)"""
        # Total number of scales
        num_scales = len(scales)
        # Divide voltage range across the scales
        scale_index = int(np.floor((self.input0.voltage / 5) * num_scales))
        # Ensure we don't exceed the available scale indices
        scale_index = min(scale_index, num_scales - 1)
        # Get the scale name from the scales dictionary
        scale_name = list(scales.keys())[scale_index]
        return scale_name

    def voltage_to_note(self, voltage, v_min=0, v_max=5):
        """Convert voltage to an index representing the chromatic scale"""
        # Normalize voltage to the range of 0-11 (12 notes in a chromatic scale)
        index = int(np.round(((voltage - v_min) / (v_max - v_min)) * 11))
        return self.chromatic_scale[index]

    def quantize_voltage(self, voltage, v_min=0, v_max=5):
        """Quantize a voltage to a key"""
        # Convert voltage to chromatic note
        chromatic_note = self.voltage_to_note(voltage, v_min, v_max)
        # Find the closest note in the scale
        chromatic_index = self.note_to_index[chromatic_note]
        # Find the closest scale note
        closest_note = min(
            self.scale,
            key=lambda x: abs(self.note_to_index.get(x, 0) - chromatic_index),
        )
        return closest_note

    def precision_shift_voltage(self, voltage):
        """Shift quantized voltage by an amount"""
        # Calculate difference (0 - 2.5V subtracts, 2.5 - 5V adds)
        if self.shift < 2.5:
            return voltage - (2.5 - self.shift)
        self.precision_shifted_voltage = voltage + self.shift - 2.5

    def run(self):
        # Read Inputs
        self.input0.read()
        self.encoder.read()
        self.encoder_button.read()
        # update settings using encoder information
        self.update_settings()
        # Quantize Input
        voltage_quantized = self.quantize_voltage(self.input0.voltage)
        # Create a second precision shifted voltage
        voltage_shifted = self.precision_shift_voltage(voltage_quantized)
        voltage_quantized_and_shifted = self.quantize_voltage(voltage_shifted)
        # Write to DAC Outputs
        self.output0.write(voltage_quantized)
        self.output1.write(voltage_quantized_and_shifted)


quantizer = Quantizer()
while True:
    quantizer.run()
