// Display data that was read from buttons
// MS_1 button - start
// MS_2 button - stop

#include "U8glib.h"

byte pinMButtons[4];

byte prevMButtons[4];
byte prevButtons[36];


unsigned long checkInterval;
unsigned long showResultInterval;

bool started;
bool showResult;
unsigned long lastCheckTime;
unsigned long countdownStart;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

char* lastMessage;

// read data from main buttons
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

// read data from buttons
byte readButtons() {
    byte s = 0;
    // num_pressed in [1;36] pressed
    byte num_pressed = 0;

    for (byte i = 8; i <= 13; ++i) {
        digitalWrite(i, LOW);
        for (byte j = 2; j <= 7; ++j) {
            byte state = digitalRead(j) == LOW;
            byte idx = 6 * (i - 8) + (j - 2);
            if (state && prevButtons[idx] == 0) {
                num_pressed = idx + 1;
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
        return num_pressed;
    } else {
        // many buttons are pressed
        return 255;
    }
}

void setupButtons() {
    for (byte j = 2; j <= 7; ++j) {
        pinMode(j, INPUT_PULLUP);
    }
    for (byte i = 8; i <= 13; ++i) {
        pinMode(i, OUTPUT);
        digitalWrite(i, HIGH);
    }
}

// make message about pressed buttons
char* convertButtons(byte val) {
    if (val == 255) {
        return "Many buttons pressed";
    } else if (val == 0) {
        return "No buttons pressed";
    } else if (val >= 1 && val <= 36) {
        static char buf[20];
        sprintf(buf, "S%d pressed", val);
        return buf;
    } else {
        return "ERROR";
    }
}

void drawStartScreen() {
    u8g.firstPage();
    do {
        u8g.setFont(u8g_font_6x13);
        u8g.drawStr(10, 20, "Brain Train Project");
        u8g.drawStr(10, 40, "To start, press MS1");
    } while (u8g.nextPage());
}

void drawPromptScreen() {
    u8g.firstPage();
    do {
        u8g.setFont(u8g_font_6x13);
        u8g.drawStr(10, 20, "Press one of the");
        u8g.drawStr(10, 40, "buttons");
    } while (u8g.nextPage());
}

// display the message
void drawResult(char* message) {
    u8g.firstPage();
    do {
        u8g.setFont(u8g_font_6x13);
        u8g.drawStr(10, 30, message);
    } while (u8g.nextPage());
}

void reset() {
    for (byte i = 0; i < 4; ++i) {
        prevMButtons[i] = 0;
    }
    for (byte i = 0; i < 36; ++i) {
        prevButtons[i] = 0;
    }
    lastCheckTime = 0;
    countdownStart = 0;

    started = false;
    showResult = false;

    lastMessage = nullptr;
    
    drawStartScreen();
}

void setup() {
    Serial.begin(9600);
    setupButtons();

    // MS_1 - pin A1 (15)
    pinMButtons[0] = 15;
    // MS_2 - pin A2 (16)
    pinMButtons[1] = 16;
    // MS_3 - pin A6 (20)
    pinMButtons[2] = 20;
    // MS_4 - pin A7 (21)
    pinMButtons[3] = 21;

    checkInterval = 25;
    showResultInterval = 600;

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
            showResult = false;
            drawPromptScreen();
        }
    }

    if (showResult) {
        byte val = readButtons();
        if (val > 0 && val != 255) {
            // another button pressed
            lastMessage = convertButtons(val);
            drawResult(lastMessage);
            countdownStart = now;
        } else if (now - countdownStart > showResultInterval) {
            showResult = false;
            drawPromptScreen();
        }
        return;
    }

    byte val = readButtons();
    if (val > 0 && val != 255) {
        lastMessage = convertButtons(val);
        drawResult(lastMessage);
        showResult = true;
        countdownStart = now;
    }
}
