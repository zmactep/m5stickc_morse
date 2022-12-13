#pragma once

#include <inttypes.h>

enum MorseItem
{
    NOTHING = 0,
    DOT = 1,
    DASH = 2,
    UNREAD = 3
};

extern uint16_t morse_codes[];

class Morse
{
private:
    uint16_t code;
    uint8_t code_position;
    bool is_reset;

    uint16_t signal_threshold;

public:
    Morse(uint16_t threshold = 200);

    void reset();
    bool set(char symbol);
    bool append(MorseItem item);
    MorseItem operator[](uint8_t position) const;
    char at(uint8_t position) const;

    MorseItem decode(uint16_t signal) const;
    uint16_t letter_threshold() const;
    uint8_t position() const;
    bool is_ready() const;
    bool has_reseted() const;
    char symbol() const;
    void state(char *buffer) const;
};