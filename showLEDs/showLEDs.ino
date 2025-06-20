#include "FastLED.h"

// A random subset of n LEDs light up in white (colorShow)
// for a few seconds, then all LEDs return to a base (off) color.
// MS_1 button - start
// MS_2 button - stop

// number of LEDs that light up
byte n;

bool started;

byte colorShow[3];

byte pinMButtons[4];

byte prevMButtons[4];

unsigned long checkInterval;
unsigned long showLEDPatternInterval;

unsigned long lastCheckTime;
unsigned long countdownStart;

// read LEDPattern from main buttons
byte readMButtons() {
    byte M_s = 0;
    // num_M_pressed in [1;4] interval
    byte num_M_pressed = 0;

    for (byte i = 0; i < 4; ++i) {
        byte M_state = analogRead(pinMButtons[i]) >= 512;
        if (M_state && prevMButtons[i] == 0) {
            num_M_pressed = i + 1;
            ++M_s;
        }
        prevMButtons[i] = M_state;
    }

    if (M_s == 0) {
        // no main buttons are pressed
        return 0;
    } else if (M_s == 1) {
        // one main button is pressed
        return num_M_pressed;
    } else {
        // many main buttons are pressed
        return 255;
    }
}

CRGB LEDs[36];

// Randomly select n LEDs to light up in each test
void genLEDPattern(byte* LEDPattern) {
    for (byte i = 0; i < n; ++i) {
        LEDPattern[i] = 1;
    }
    for (byte i = n; i < 36; ++i) {
        LEDPattern[i] = 0;
    }

    // Fisher–Yates shuffle
    for (byte i = 35; i > 0; --i) {
        byte j = random(i + 1);
        byte temp = LEDPattern[i];
        LEDPattern[i] = LEDPattern[j];
        LEDPattern[j] = temp;
    }
}

// show LED pattern based on LEDPattern array
void showLEDPattern(byte* LEDPattern) {
    FastLED.clear();
    for (int i = 0; i < 36; ++i) {
        if (LEDPattern[i]) {
            LEDs[i].setRGB(colorShow[0], colorShow[1], colorShow[2]);
        }
    }
    FastLED.show();
}

void reset() {
    started = false;
    lastCheckTime = 0;
    countdownStart = 0;
    FastLED.clear();
    FastLED.show();
}

void setup() {
    randomSeed(analogRead(18));
    
    Serial.begin(9600);
    FastLED.addLeds<WS2812, 14, GRB>(LEDs, 36);
    FastLED.setBrightness(100);
    
    // MS_1 - pin A1 (15)
    pinMButtons[0] = 15;
    // MS_2 - pin A2 (16)
    pinMButtons[1] = 16;
    // MS_3 - pin A6 (20)
    pinMButtons[2] = 20;
    // MS_4 - pin A7 (21)
    pinMButtons[3] = 21;
    
    // LEDs are lighting up
    colorShow[0] = 255;
    colorShow[1] = 255;
    colorShow[2] = 255;

    n = 6;
    checkInterval = 25;
    showLEDPatternInterval = 10000;

    reset();
}

void loop() {
    unsigned long now = millis();
    if (now - lastCheckTime < checkInterval) {
        return;
    }
    lastCheckTime = now;

    byte val_M = readMButtons();

    if (val_M == 2) {
        // MS_2 - stop
        reset();
        return;
    }

    if (!started) {
        if (val_M == 1) {
            // MS_1 - start
            started = true;
            countdownStart = now;

            byte LEDPattern[36];
            genLEDPattern(LEDPattern);
            showLEDPattern(LEDPattern);
        }
    } else {
        if (now - countdownStart >= showLEDPatternInterval) {
            reset();
        }
    }
}
