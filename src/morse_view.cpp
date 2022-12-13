#include "morse_view.h"

MorseView::MorseView(M5Display *display, Morse *morse, uint8_t font_factor) : 
    display(display), morse(morse), font_factor(font_factor),
    symbol_h(font_factor * SYMBOL_H), symbol_w(font_factor * SYMBOL_W) {}

void MorseView::init() {
    this->display->setRotation(0);

    // Compute display symbol size
    uint8_t swidth = this->display->width() / this->symbol_w;
    uint8_t sheight = this->display->height() / this->symbol_h;

    // Compute shift
    this->shift_x = (this->display->width() - swidth * symbol_w) / 2;
    this->shift_y = (this->display->height() - sheight * symbol_h) / 2;

    this->reset();
}

void MorseView::reset() {
    this->display->fillScreen(BLACK);
    this->display->setTextSize(this->font_factor);
    this->display->setCursor(this->shift_x, this->shift_y + this->symbol_h);

    this->display->fillRect(0, 
                            this->display->height() - 5,
                            this->display->width() - 1,
                            5,
                            RED);
}

void MorseView::reset_code() {
    this->display->fillRect(this->shift_x, this->shift_y, 
                            this->symbol_w * 5, this->symbol_h, 
                            BLACK);
}

void MorseView::add_code() {
    uint8_t position = this->morse->position() - 1;
    char code = this->morse->at(position);

    if (!code) {
        return;
    }

    if (!position) {
        this->reset_code();
    }

    uint8_t position_shift = this->shift_x + this->symbol_w * position;

    int16_t cursor_x = this->display->getCursorX();
    int16_t cursor_y = this->display->getCursorY();

    this->display->setCursor(position_shift, this->shift_y);
    this->display->setTextColor(RED);
    this->display->print(code);
    this->display->setTextColor(WHITE);
    this->display->setCursor(cursor_x, cursor_y);
}

void MorseView::add_symbol() {
    char symbol = this->morse->symbol();

    if (!symbol) {
        return;
    }

    if (this->is_full()) {
        this->display->fillRect(this->shift_x, this->shift_y + this->symbol_h, 
                                this->display->width(), this->display->height(), 
                                BLACK);
        this->display->setCursor(this->shift_x, this->shift_y + this->symbol_h);
    }

    this->display->setTextColor(WHITE);
    this->display->print(symbol);

    if (this->is_lineend()) {
        int16_t cursor_y = this->display->getCursorY(); 
        this->display->setCursor(this->shift_x, cursor_y + this->symbol_h);
    }
}

bool MorseView::del_symbol() {
    int16_t cursor_x = this->display->getCursorX();
    int16_t cursor_y = this->display->getCursorY();

    if (cursor_x == this->shift_x && cursor_y == this->shift_y + this->symbol_h) {
        return false;
    }

    if (cursor_x == this->shift_x && cursor_y != this->shift_y + this->symbol_h) {
        uint8_t swidth = this->display->width() / this->symbol_w;
        cursor_x = this->shift_x + this->symbol_w * (swidth - 1);
        cursor_y -= this->symbol_h;       
    } else {
        cursor_x -= this->symbol_w;
    }

    this->display->fillRect(cursor_x, cursor_y, this->symbol_w, this->symbol_h, BLACK);
    this->display->setCursor(cursor_x, cursor_y);
    
    return true;
}

bool MorseView::is_full() const {
    int16_t cursor_y = this->display->getCursorY();
    int16_t max_cursor_y = this->display->height() - this->shift_y - this->symbol_h - 1;

    return cursor_y >= max_cursor_y;
}

bool MorseView::is_lineend() const {
    int16_t cursor_x = this->display->getCursorX();

    return cursor_x >= this->display->width() - this->shift_x - 1;
}