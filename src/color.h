/**************************************************************************
 *   color.h  --  This file is part of AVR-ST7735.                        *
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

#ifndef _COLOR_H
#define _COLOR_H

#include <avr/io.h>

class Color {
private:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    char hexcode[7];

    uint16_t cc565;

public:
    Color(const char* hex);

    Color(uint16_t _cc565);

    inline uint16_t get_cc565() const {
        return this->cc565;
    }

    inline const char* get_hexcode() const {
        return this->hexcode;
    }

private:
    uint8_t hex2int(char c);
    void calculate_cc565();
};

#endif // _COLOR_H
