#include "Output.h"
#include "Input.h"
#include "Button.h"
#include "LED.h"
#include "Step.h"
#include "Bank.h"

const int SHIFT165_PL = 2;  // Parallel load for 74HC165
const int SHIFT165_CLK = 3;  // Clock for 74HC165
const int SHIFT165_Q7 = 4;  // Serial out for 74HC165

const int SHIFT164_DATA = 5; // Data in for 74LS164
const int SHIFT164_CLK = 6; // Clock for 74LS164

const int CLOCK_INPUT_PIN = 2; // Clock Input
const int BANK_1_OUTPUT_PIN = 3; // Bank 1 Trigger Output Pin
const int BANK_2_OUTPUT_PIN = 4; // Bank 2 Trigger Output Pin
const int BANK_3_OUTPUT_PIN = 5; // Bank 3 Trigger Output Pin
const int BANK_4_OUTPUT_PIN = 6; // Bank 4 Trigger Output Pin

const int DEBOUNCE_DELAY_MS = 10;  // Debounce time in ms
const int TRIGGER_DURATION_MS = 10;  // Debounce time in ms

class Sequencer {
  private:
    Bank *banks;
    Button buttons[16];
    int selectedBank;
    int step;

  public:
    Sequencer(Output outputs[4]) {
      banks = {Bank(&outputs[0]), Bank(&outputs[1]), Bank(&outputs[2]),
              Bank(&outputs[3])};
      selectedBank = 0;
      step = 0;
    }

    void stepForward() {
      step++;
      if (step == 16) {
        step = 0;
      }
    }

    void readButtonStates() {
      byte data[NUM_SHIFT_REGS];

      // Load parallel data into shift registers
      digitalWrite(LOAD_PIN, LOW);
      delayMicroseconds(5);
      digitalWrite(LOAD_PIN, HIGH);

      // Read data serially from the shift registers
      for (int i = 0; i < NUM_SHIFT_REGS; i++) {
          data[i] = shiftIn(DATA_PIN, CLOCK_PIN, MSBFIRST);
      }

      // TODO: debounce update buttons
      // TODO: update active bank steps according to button states
      // TODO: update selectedBank if other bank selected
    }

    void updateActiveBankSteps() {
      // update steps in active bank to the corresponding button state
      for (int i = 0; i < 16; i++) {
        banks[selectedBank].setStepState(i, buttons[i].getState())
      }
    }

    void updateOutputs() {
      for (int i = 0; i < 4; i++) {
        // TODO: If state is high, only set low if trigger delay has elapsed for bank
      }
    }

    void updateLEDs() {
      // MSB means this should count down
      for (int i = 3; i > 0; i--) {
        // TODO: Shift unused bits
        digitalWrite(DATA_PIN, 0); // Set data line
        digitalWrite(CLOCK_PIN, HIGH);
        // TODO: This delay should be tweaked
        delay(10);
        digitalWrite(CLOCK_PIN, LOW);
        delay(10);
      }
      for (int i = 3; i > 0; i--) {
        // TODO: Shift bank selection state bits
        digitalWrite(DATA_PIN, selectedBank & i);
        digitalWrite(CLOCK_PIN, HIGH); // Pulse clock
        // TODO: This delay should be tweaked
        delay(10);
        digitalWrite(CLOCK_PIN, LOW);
        delay(10);
      }
      for (int i = 15; i > 0; i--) {
        // TODO: Shift active bank step bits
        digitalWrite(DATA_PIN, bank[selectedBank].getState() & 1);
        digitalWrite(CLOCK_PIN, HIGH); // Pulse clock
        // TODO: This delay should be tweaked
        delay(10);
        digitalWrite(CLOCK_PIN, LOW);
        delay(10);
      }
    }


};


Input clock = Input(2);
Output outputs[4] = {
  Output(BANK_1_OUTPUT_PIN), 
  Output(BANK_2_OUTPUT_PIN), 
  Output(BANK_3_OUTPUT_PIN), 
  Output(BANK_4_OUTPUT_PIN)
  };

Sequencer sequencer = Sequencer(outputs);

void clockInterruptHandler() {
  sequencer.stepForward();
}

void setup() {
  attachInterrupt(digitalPinToInterrupt(clock.getPin()), clockInterruptHandler, RISING);
  Serial.begin(115200);
}

void loop() {
  sequencer.readButtonStates()
  sequencer.updateActiveBankSteps()
  sequencer.updateOutputs()
  sequencer.updateLEDs()
}

