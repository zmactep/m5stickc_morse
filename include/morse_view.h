#pragma once

#include <M5StickCPlus.h>
#include "morse.h"

#define SYMBOL_W 6
#define SYMBOL_H 8

class MorseView
{
private:
    M5Display *display;
    Morse *morse;

    uint8_t font_factor;

    uint16_t shift_x;
    uint16_t shift_y;

    uint8_t symbol_w;
    uint8_t symbol_h;

public:
    MorseView(M5Display *display, Morse *morse, uint8_t font_factor = 4);

    void init();
    void reset();

    void reset_code();
    void add_code();

    void add_symbol();
    void del_symbol();
};