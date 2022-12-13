#include <Arduino.h>
#include <M5StickCPlus.h>
#include <BleKeyboard.h>
#include "morse.h"
#include "morse_view.h"

BleKeyboard blekey("Morse Keyboard");
Morse morse(200);
MorseView mview(&M5.Lcd, &morse);

void setup() {
    Serial.begin(115200);
    M5.begin();
    blekey.begin();

    mview.init();
    M5.Beep.setVolume(1);
}

void loop() {
    M5.update();

    static uint last_press = 0;
    static uint last_release = 0;

    bool has_released = last_release > last_press;
    uint16_t no_action_time = millis() - last_release;

    if (morse.is_ready() || (has_released && no_action_time > morse.letter_threshold() && !morse.has_reseted())) {
        char symbol = morse.symbol();
        mview.add_symbol();
        if (symbol) {
            Serial.print(symbol);
            if (blekey.isConnected()) {
                blekey.print(symbol);
            }
        }
        morse.reset();
    }

    if (M5.BtnA.wasPressed()) {
        last_press = millis();
        M5.Beep.tone(440);
    } else if (M5.BtnA.wasReleased()) {
        last_release = millis();
        morse.append(morse.decode(last_release - last_press));
        mview.add_code();
        M5.Beep.mute();
    } else if (M5.BtnB.wasReleased()) {
        morse.reset();
        if (mview.del_symbol() && blekey.isConnected()) {
            blekey.write(KEY_BACKSPACE);
        }
    } else if (M5.Axp.GetBtnPress() == 0x02) {
        Serial.println();
        morse.reset();
        mview.reset();
    }
}