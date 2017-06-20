/**************************************************************************
 *   ttf_st7735.cpp  --  This file is part of AVR-ST7735.                 *
 *                                                                        *
 *   Copyright (C) 2017, Ivo Filot <ivo@ivofilot.nl>                      *
 *                                                                        *
 *   AVR-ST7735 is free software:                                         *
 *   you can redistribute it and/or modify it under the terms of the      *
 *   GNU General Public License as published by the Free Software         *
 *   Foundation, either version 3 of the License, or (at your option)     *
 *   any later version.                                                   *
 *                                                                        *
 *   AVR-ST7735 is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include "ttf_st7735.h"

TTF_ST7735::TTF_ST7735() {
    spi.open();
    this->init();
    this->clear_screen();
}

void TTF_ST7735::random_pixel_test() {
    for (int i = 0; i < 4000; ++i) {
        int x = rand() % XMAX;          // random x coordinate
        int y = rand() % YMAX;          // random y coordinate
        this->draw_pixel(x, y, RED); // draw pixel at x,y
    }
}

void TTF_ST7735::draw_text(const char* txt, uint8_t x, uint8_t y, const Color& color) {
    int i = 0;
    char c = txt[i];
    while(c) {
        this->draw_char(c, x, y, color);

        x += 5;
        if(x > XMAX) {
            x = 5;
            y += 7;
        }

        ++i;
        c = txt[i];
    }
}

void TTF_ST7735::draw_char(char ch, uint8_t x, uint8_t y, const Color& color) {
    int pixel;
    uint8_t mask = 0x01;
    this->set_addr_window(x, y, x + 4, y + 6);
    this->write_command(RAMWR);
    for (uint8_t row = 0; row < 7; row++) {
        for (uint8_t col = 0; col < 5; col++) {
            uint8_t data = pgm_read_byte(&(FONT_CHARS[ch - 32][col]));
            uint8_t bit = data & mask;
            if (bit == 0) {
                pixel = BLACK;
            } else {
                pixel = color.get_cc565();
            }
            this->write_word(pixel);
        }
        mask <<= 1;
    }
}

void TTF_ST7735::write_command(uint8_t cmd) {
    PORTB &= ~(1 << 1);
    spi.transfer_byte(cmd);
    PORTB |= (1 << 1);
}

void TTF_ST7735::write_byte(uint8_t byte) {
    spi.transfer_byte(byte);
}

void TTF_ST7735::write_word(uint16_t word) {
    spi.transfer_byte(word >> 8);    // transfer upper 8 bits
    spi.transfer_byte(word & 0xFF);     // transfer lower 8 bits
}

void TTF_ST7735::write_16bit_pixels(uint16_t data, int count) {
    this->write_command(RAMWR);
    for (; count > 0; count--) {
        SPDR = (data >> 8);     // write hi byte
        while (!(SPSR & 0x80)) {}
        SPDR = (data & 0xFF);   // write lo byte
        while (!(SPSR & 0x80)) {}
    }
}

void TTF_ST7735::init() {
    this->reset();                  // initialize display controller
    this->write_command(SLPOUT);    // take display out of sleep mode
    _delay_ms(150);                 // wait 150 ms for TFT driver circuits
    this->write_command(COLMOD);    // select color mode:
    this->write_byte(0x05);         // mode 5 = 16 bit pixels (RGB565)
    this->write_command(MADCTL);
    this->write_byte(0xCC);
    this->write_command(DISPON);    // turn display on!
}

void TTF_ST7735::reset() {
    PORTB &= ~(1 << PINB0);
    _delay_ms(1);
    PORTB |= (1 << PINB0);
    _delay_ms(150);
}

void TTF_ST7735::set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    this->write_command(CASET);     // set column range (x0,x1)
    this->write_word(x0 + colstart);
    this->write_word(x1 + colstart);
    this->write_command(RASET);     // set row range (y0,y1)
    this->write_word(y0 + rowstart);
    this->write_word(y1 + rowstart);
    this->write_command(RAMWR);
}

void TTF_ST7735::draw_pixel(uint8_t x, uint8_t y, int color) {
    this->set_addr_window(x, y, x, y);
    this->write_16bit_pixels(color, 1);
}

void TTF_ST7735::clear_screen() {
    this->set_addr_window(0, 0, XMAX, YMAX); // set window to entire display
    this->write_command(RAMWR);
    for (unsigned int i = 40960; i > 0; --i) {
        SPDR = 0; // initiate transfer of 0x00
        while (!(SPSR & 0x80)) {}
    }
}

TTF_ST7735::~TTF_ST7735() {
    spi.close();
}

