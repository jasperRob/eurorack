// Parameters
float snareFrequency = 180.0;  // Snare tone frequency (Hz)
unsigned long noiseDuration = 200;  // Duration of noise burst in milliseconds
unsigned long toneDuration = 300;  // Duration of tone
float decayFactor = 0.98;  // Factor for exponential decay (lower is faster fade-out)

void setup() {
}

void loop() {
  // Start the white noise burst
  whiteNoise(noiseDuration);
  // Generate the tone with an envelope
  toneWithEnvelope(snareFrequency, toneDuration);
  delay(500);  // Wait for a bit before repeating the sound
}

void whiteNoise(unsigned long duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    // Generate random noise and output to the piezo
    int noiseSignal = random(0, 255);  // Random noise between 0 and 255
    analogWrite(PIEZO_PIN, noiseSignal);  // Send the noise signal
  }
  analogWrite(PIEZO_PIN, 0);  // Stop the noise after the duration
}

void toneWithEnvelope(float frequency, unsigned long duration) {
  unsigned long startTime = millis();
  float decay = 1.0;  // Start with full amplitude
  
  while (millis() - startTime < duration) {
    // Apply a sine wave signal at the given frequency
    int toneSignal = (sin(2 * PI * frequency * (millis() - startTime) / 1000.0) * 127.5) + 127; // Convert to 8-bit range
    analogWrite(PIEZO_PIN, toneSignal * decay);  // Apply decay to tone amplitude

    // Apply exponential decay to the tone amplitude
    decay *= decayFactor;  // Decay the signal over time
  }
}
