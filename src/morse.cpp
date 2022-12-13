#include "morse.h"

#define CODE(x1, x2, x3, x4, x5) x1 | (x2 << 2) | (x3 << 4) | (x4 << 6) | (x5 << 8)

uint16_t morse_codes[] = {
    CODE(DOT,  DASH,    NOTHING, NOTHING, NOTHING), // A — 0
    CODE(DASH, DOT,     DOT,     DOT,     NOTHING), // B — 1
    CODE(DASH, DOT,     DASH,    DOT,     NOTHING), // C — 2
    CODE(DASH, DOT,     DOT,     NOTHING, NOTHING), // D — 3
    CODE(DOT,  NOTHING, NOTHING, NOTHING, NOTHING), // E — 4
    CODE(DOT,  DOT,     DASH,    DOT,     NOTHING), // F — 5
    CODE(DASH, DASH,    DOT,     NOTHING, NOTHING), // G — 6
    CODE(DOT,  DOT,     DOT,     DOT,     NOTHING), // H — 7
    CODE(DOT,  DOT,     NOTHING, NOTHING, NOTHING), // I — 8
    CODE(DOT,  DASH,    DASH,    DASH,    NOTHING), // J — 9
    CODE(DASH, DOT,     DASH,    NOTHING, NOTHING), // K — 10
    CODE(DOT,  DASH,    DOT,     DOT,     NOTHING), // L — 11
    CODE(DASH, DASH,    NOTHING, NOTHING, NOTHING), // M — 12
    CODE(DASH, DOT,     NOTHING, NOTHING, NOTHING), // N — 13
    CODE(DASH, DASH,    DASH,    NOTHING, NOTHING), // O — 14
    CODE(DOT,  DASH,    DASH,    DOT,     NOTHING), // P — 15
    CODE(DASH, DASH,    DOT,     DASH,    NOTHING), // Q — 16
    CODE(DOT,  DASH,    DOT,     NOTHING, NOTHING), // R — 17
    CODE(DOT,  DOT,     DOT,     NOTHING, NOTHING), // S — 18
    CODE(DASH, NOTHING, NOTHING, NOTHING, NOTHING), // T — 19
    CODE(DOT,  DOT,     DASH,    NOTHING, NOTHING), // U — 20
    CODE(DOT,  DOT,     DOT,     DASH,    NOTHING), // V — 21
    CODE(DOT,  DASH,    DASH,    NOTHING, NOTHING), // W — 22
    CODE(DASH, DOT,     DOT,     DASH,    NOTHING), // X — 23
    CODE(DASH, DOT,     DASH,    DASH,    NOTHING), // Y — 24
    CODE(DASH, DASH,    DOT,     DOT,     NOTHING), // Z — 25
    CODE(DASH, DASH,    DASH,    DASH,    DASH),    // 0 — 26
    CODE(DOT,  DASH,    DASH,    DASH,    DASH),    // 1 — 27
    CODE(DOT,  DOT,     DASH,    DASH,    DASH),    // 2 — 28
    CODE(DOT,  DOT,     DOT,     DASH,    DASH),    // 3 — 29
    CODE(DOT,  DOT,     DOT,     DOT,     DASH),    // 4 — 30
    CODE(DOT,  DOT,     DOT,     DOT,     DOT),     // 5 — 31
    CODE(DASH, DOT,     DOT,     DOT,     DOT),     // 6 — 32
    CODE(DASH, DASH,    DOT,     DOT,     DOT),     // 7 — 33
    CODE(DASH, DASH,    DASH,    DOT,     DOT),     // 8 — 34
    CODE(DASH, DASH,    DASH,    DASH,    DOT)      // 9 — 35
};

Morse::Morse(uint16_t threshold) : signal_threshold(threshold), code(0), code_position(0), is_reset(true) {}

void Morse::reset() {
    this->code = 0;
    this->code_position = 0;
    this->is_reset = true;
}

bool Morse::set(char symbol) {
    uint8_t table_position = 0;

    if (symbol >= 65 && symbol < 91) {
        table_position = symbol - 65;
    } else if (symbol >= 48 && symbol < 58) {
        table_position = symbol - 48 + 26;
    } else {
        return false;
    }

    this->reset();
    this->code = morse_codes[table_position];
    this->is_reset = false; 

    return true;
}

bool Morse::append(MorseItem item) {
    if (this->code_position >= 5 || item == UNREAD) {
        return false;
    }

    this->code &= ~(3 << (this->code_position * 2));
    this->code |= item << (this->code_position * 2);
    this->code_position += 1;
    this->is_reset = false;

    return true;
}

MorseItem Morse::operator[](uint8_t position) const {
    if (position >= 5) {
        return NOTHING;
    }

    uint16_t mask = 3 << (position * 2);
    return MorseItem((code & mask) >> (position * 2));
}

char Morse::at(uint8_t position) const {
    MorseItem item = (*this)[position];

    if (item == DOT) {
        return '.';
    } else if (item == DASH) {
        return '-';
    } 

    return 0;
}

MorseItem Morse::decode(uint16_t signal) const {
    return signal >= this->signal_threshold ? DASH : DOT;
}

uint16_t Morse::letter_threshold() const {
    return this->signal_threshold * 5;
}

bool Morse::is_ready() const {
    return this->code_position == 5;
}

bool Morse::has_reseted() const {
    return this->is_reset;
}

char Morse::symbol() const {
    uint8_t code_number = 0;
    bool match = true;

    while (code_number < 36) {
        if (this->code == morse_codes[code_number]) {
            break;
        }
        code_number++;
    }

    if (code_number < 26) {
        return code_number + 65;
    } else if (code_number < 36) {
        return code_number + (48 - 26);
    }

    return 0;
}


uint8_t Morse::position() const {
    return this->code_position;
}

void Morse::state(char *buffer) const {
    for (int8_t i = 0; i < 5; i++) {
        buffer[i] = this->at(i);
    }
}