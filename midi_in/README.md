# MIDI Input Module

A purpose built midi input module used to send clock and run signals via USB to a eurorack setup.

Built using a Raspberry Pi Pico, and currently very much in the prototype phase. 

## Uploading

The `python/main.py` script can be uploaded to a RPI Pico running MicroPython, and it will start receiving MIDI Clock and Run signals and outputting the pulses on GPIO2 and GPIO3 respectively.

I had an unsuccessful attempt at using the C/C++ SDK to upload a similarly working hex file to the board, but ran into issues with the MIDI library. Regardless, I have put these artefacts in the `cpp` directory.
