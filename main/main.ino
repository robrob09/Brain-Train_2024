// Brain Train

// MS1 — starts the test: triggers a countdown, then n random LEDs light up
// Matrix buttons (S1–S36) are used to input which LEDs lit up
// MS3 — cancels the last recorded button press during input phase
// MS4 — shows a hint or final result if exactly n buttons were pressed
// The result is shown as a percentage of correct selections and
// the number of correctly pressed buttons
// MS2 — resets everything and returns to the start screen


#include <FastLED.h>
#include <U8glib.h>

CRGB LEDs[36];
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

byte pinMButtons[4];
byte prevMButtons[4];
byte prevButtons[36];

unsigned long checkInterval;
unsigned long showLEDsInterval;
unsigned long showResultInterval;
unsigned long countdownStart;
unsigned long lastCheckTime;
unsigned long delayBeforeLEDs;
unsigned long beforeStartInterval;

byte LEDPattern[36];
int arr[36];
int it;
byte n;

byte colorShow[3];

bool isStartScreen;
bool isCountdown;
bool isShowLEDs;
bool isButtonInput;
bool isFinalStage;
bool isHelpPrompt;

char message[32];
bool showMessage;
unsigned long messageTimestamp;

void setupButtons() {
    for (byte j = 2; j <= 7; ++j) {
        pinMode(j, INPUT_PULLUP);
    }
    for (byte i = 8; i <= 13; ++i) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
}

// read data from main buttons
byte readMButtons() {
    byte M_s = 0;
    byte num_M_pressed = 0;
    for (byte i = 0; i < 4; ++i) {
        byte state = analogRead(pinMButtons[i]) >= 512;
        if (state && prevMButtons[i] == 0) {
            num_M_pressed = i + 1;
            ++M_s;
        }
        prevMButtons[i] = state;
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

// read data from buttons
byte readButtons() {
    byte s = 0;
    // num_pressed in [1;36] pressed
    byte num = 0;
    for (byte i = 8; i <= 13; ++i) {
        digitalWrite(i, LOW);
        for (byte j = 2; j <= 7; ++j) {
            byte state = digitalRead(j) == LOW;
            byte idx = 6 * (i - 8) + (j - 2);
            if (state && prevButtons[idx] == 0) {
                num = idx + 1;
                ++s;
            }
            prevButtons[idx] = state;
        }
        digitalWrite(i, HIGH);
    }
    if (s == 0) {
        // no buttons are pressed
        return 0;
    } else if (s == 1) {
        // one button is pressed
        return num;
    } else {
        // many buttons are pressed
        return 255;
    }
}

// make message
// 3 lines
void drawText3(char* l1, char* l2, char* l3) {
    u8g.firstPage();
    do {
        u8g.setFont(u8g_font_6x10);
        u8g.drawStr(2, 16, l1);
        u8g.drawStr(2, 32, l2);
        u8g.drawStr(2, 48, l3);
    } while (u8g.nextPage());
}

// make message
// 2 lines
void drawText2(char* l1, char* l2) {
    u8g.firstPage();
    do {
        u8g.setFont(u8g_font_6x13);
        u8g.drawStr(6, 20, l1);
        u8g.drawStr(6, 40, l2);
    } while (u8g.nextPage());
}

// make message
// 1 line
void drawMessage(char* msg) {
    u8g.firstPage();
    do {
        u8g.setFont(u8g_font_6x13);
        u8g.drawStr(6, 30, msg);
    } while (u8g.nextPage());
}

// Randomly select n LEDs to light up in each test
void genLEDPattern() {
    for (byte i = 0; i < 36; ++i) {
        if (i < n) {
            LEDPattern[i] = 1;
        } else {
            LEDPattern[i] = 0;
        }
    }
    
    // Fisher–Yates shuffle
    for (byte i = 35; i > 0; --i) {
        byte j = random(i + 1);
        byte tmp = LEDPattern[i];
        LEDPattern[i] = LEDPattern[j];
        LEDPattern[j] = tmp;
    }
}

// show LED pattern based on LEDPattern array
void showLEDPattern() {
    FastLED.clear();
    for (int i = 0; i < 36; ++i) {
        if (LEDPattern[i]) {
            LEDs[i].setRGB(colorShow[0], colorShow[1], colorShow[2]);
        }
    }
    FastLED.show();
}

void reset() {
    for (byte i = 0; i < 36; ++i) {
        prevButtons[i] = 0;
        arr[i] = 0;
    }
    for (byte i = 0; i < 4; ++i) {
        prevMButtons[i] = 0;
    }

    it = 0;
    countdownStart = 0;
    lastCheckTime = 0;
    delayBeforeLEDs = 0;

    FastLED.clear();
    FastLED.show();

    isStartScreen = true;
    isCountdown = false;
    isShowLEDs = false;
    isButtonInput = false;
    isFinalStage = false;
    isHelpPrompt = false;
    showMessage = false;

    drawText2("Brain Train Project", "To start, press MS1");
}

void setup() {
    randomSeed(analogRead(18));

    Serial.begin(9600);
    setupButtons();
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

    n = 6;
    checkInterval = 25;
    showLEDsInterval = 12000;
    showResultInterval = 600;
    beforeStartInterval = 3000;

    // LEDs are lighting up
    colorShow[0] = 255;
    colorShow[1] = 255;
    colorShow[2] = 255;

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

    if (isFinalStage) {
        return;
    }

    if (val_M == 4 && isButtonInput) {
        int count = 0;
        for (int i = 0; i < 36; ++i) {
            if (arr[i] > 0) {
                count++;
            }
        }
        if (count < n) {
            sprintf(message, "%d buttons pressed", count);
            char buf[20];
            sprintf(buf, "Press %d more", n - count);
            drawText2(message, buf);
            showMessage = true;
            messageTimestamp = now;
        } else if (count > n) {
            sprintf(message, "%d buttons pressed", count);
            char buf[20];
            sprintf(buf, "Cancel %d ones", count - n);
            drawText2(message, buf);
            showMessage = true;
            messageTimestamp = now;
        } else {
            int s = 0;
            for (int i = 0; i < 36; ++i) {
                if (arr[i] > 0 && LEDPattern[i] == 1) {
                    s++;
                }
            }
            int percent = 100.0 * s / n + 0.5;
            sprintf(message, "Your result: %d%%", percent);
            char buf1[20];
            sprintf(buf1, "%d buttons correct", s);
            drawText3(message, buf1, "To restart, press MS2");
            isFinalStage = true;
        }
        return;
    }

    if (showMessage && now - messageTimestamp > showResultInterval) {
        showMessage = false;
        drawText2("Press buttons", "respectively to LEDs");
    }

    if (isStartScreen && val_M == 1) {
        // MS_1 - start
        countdownStart = now;
        isStartScreen = false;
        isCountdown = true;
    }

    if (isCountdown) {
        int secLeft = (beforeStartInterval - (now - countdownStart)) / 1000;
        if (secLeft <= 0) {
            delayBeforeLEDs = now - countdownStart;
            genLEDPattern();
            showLEDPattern();
            countdownStart = now;
            isCountdown = false;
            isShowLEDs = true;
        } else {
            char buf[20];
            sprintf(buf, "in %d sec", secLeft);
            sprintf(message, "%d LEDs lighting up", n);
            drawText2(message, buf);
        }
        return;
    }

    if (isShowLEDs) {
        int timeLeft = (showLEDsInterval - (now - countdownStart)) / 1000;
        if (timeLeft <= 0) {
            FastLED.clear();
            FastLED.show();
            drawText2("Press buttons", "respectively to LEDs");
            isShowLEDs = false;
            isButtonInput = true;
            return;
        } else {
            char buf[20];
            sprintf(buf, "%d sec left", timeLeft);
            sprintf(message, "%d LEDs lighting up", n);
            drawText2(message, buf);
            return;
        }
    }

    if (isButtonInput) {
        if (val_M == 3) {
            int maxVal = 0;
            byte maxIdx = 255;
            for (byte i = 0; i < 36; ++i) {
                if (arr[i] > maxVal) {
                    maxVal = arr[i];
                    maxIdx = i;
                }
            }
            if (maxVal == 0) {
                drawText2("Nothing to", "cancel");
            } else {
                arr[maxIdx] = 0;
                sprintf(message, "Press S%d", maxIdx + 1);
                drawText2(message, "cancelled");
            }
            showMessage = true;
            messageTimestamp = now;
            return;
        }

        byte val = readButtons();
        if (val > 0 && val <= 36) {
            arr[val - 1] = ++it;
            sprintf(message, "S%d pressed", val);
            drawMessage(message);
            showMessage = true;
            messageTimestamp = now;
        }
    }
}
