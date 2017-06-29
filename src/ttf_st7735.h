/**************************************************************************
 *   ttf_st7735.h  --  This file is part of AVR-ST7735.                   *
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

#ifndef _TTF_ST7735_H
#define _TTF_ST7735_H

#include <util/delay.h>
#include <stdlib.h>

#include "chars.h"
#include "spi.h"
#include "color.h"

class TTF_ST7735 {
private:
    static const uint8_t SWRESET = 0x01;    // software reset;
    static const uint8_t SLPOUT  = 0x11;    // sleep out;
    static const uint8_t DISPOFF = 0x28;    // display off;
    static const uint8_t DISPON  = 0x29;    // display on;
    static const uint8_t CASET   = 0x2A;    // column address set;
    static const uint8_t RASET   = 0x2B;    // row address set;
    static const uint8_t RAMWR   = 0x2C;    // RAM write;

    /*
     * MADCTL determines how the display operates
     *
     * MY : Row address order
     * MX : Column address order
     * MV : Row/Column exchange
     * ML : Scan address order
     * RB : RGB / BGR order
     * MH : Horizontal order
     * D1 : Not used
     * D0 : Not used
     *
     *        MY MX MV ML RB MH D1 D0
     * 0x36 | 0  0  1  1  0  1  1  0 --> Regular display, regular RGB
     * 0xFE | 1  1  1  1  1  1  1  0 --> Mirror X and Y, swap R and B channels
     *
     */
    static const uint8_t MADCTL  = 0b00110110;    // display control;

    static const uint8_t COLMOD  = 0x3A;          // color mode;

    // 1.8" TFT display constants
    static const uint8_t XSIZE = 128;
    static const uint8_t YSIZE = 160;
    static const uint8_t XMAX = XSIZE - 1;
    static const uint8_t YMAX = YSIZE - 1;

    // color constants
    static const uint16_t BLACK   = 0x0000;
    static const uint16_t BLUE    = 0x001F;
    static const uint16_t RED     = 0xF800;
    static const uint16_t GREEN   = 0x0400;
    static const uint16_t LIME    = 0x07E0;
    static const uint16_t CYAN    = 0x07FF;
    static const uint16_t MAGENTA = 0xF81F;
    static const uint16_t YELLOW  = 0xFFE0;
    static const uint16_t WHITE   = 0xFFFF;

    /*
     * these are offsets; some displays show random pixels when these are set to zero
     * if you see for instance one column and two rows that show randomnly colored
     * pixels, than colstart = 1 and rowstart = 2.
     *
     * Default values are 0 and most displays work out-of-the-box with 0
     */
    uint8_t colstart = 0;
    uint8_t rowstart = 0;

    SPI spi;

public:
    TTF_ST7735();

    void clear_screen();
    void random_pixel_test();
    void draw_rectangle_solid(uint8_t x0, uint8_t y0, uint8_t dx, uint8_t dy, const Color& color);
    void draw_text(const char* txt, uint8_t x, uint8_t y, const Color& color);
    void draw_char(char ch, uint8_t x, uint8_t y, const Color& color);

    ~TTF_ST7735();
private:
    void reset();
    void init();
    void write_command(uint8_t byte);
    void write_byte(uint8_t byte);
    void write_word(uint16_t word);
    void write_16bit_pixels(uint16_t data, int count);
    void set_addr_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
    void draw_pixel(uint8_t x, uint8_t y, int color);
    void parse_commands(const uint8_t *addr);
};

#endif //_TTF_ST7735_H
