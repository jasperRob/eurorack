import adafruit_midi
from adafruit_midi.timing_clock import TimingClock
from adafruit_midi.start import Start
from adafruit_midi.stop import Stop
import time
import board
import digitalio
import usb_midi

# Set up the onboard LED
led = digitalio.DigitalInOut(board.LED)
led.direction = digitalio.Direction.OUTPUT

# Setup GPIO for Clock and Run signals
clock_pin = digitalio.DigitalInOut(board.GP2)
clock_pin.direction = digitalio.Direction.OUTPUT
clock_pin.value = False  # Ensure it starts low

run_pin = digitalio.DigitalInOut(board.GP3)
run_pin.direction = digitalio.Direction.OUTPUT
run_pin.value = False  # Ensure it starts low

midi_usb = adafruit_midi.MIDI(midi_in=usb_midi.ports[0])

clock_high_time = None
run_high_time = None

while True:
    current_time = time.monotonic()
    msg = midi_usb.receive()
    if msg is not None:
        if isinstance(msg, TimingClock):
            # set clock pin high and record the time
            clock_pin.value = True
            led.value = True
            clock_high_time = current_time

        elif isinstance(msg, Start):
            # set run pin high and record the time
            run_pin.value = True
            run_high_time = current_time

    # check if we need to turn off the clock pulse
    if clock_high_time is not None and (
        current_time - clock_high_time >= 0.002  # 2ms pulse
    ):
        clock_pin.value = False
        led.value = False
        clock_high_time = None

    # check if we need to turn off the run pulse
    if run_high_time is not None and (
        current_time - run_high_time >= 0.005
    ):  # 5ms pulse
        run_pin.value = False
        run_high_time = None
