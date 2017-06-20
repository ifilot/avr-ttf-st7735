/**************************************************************************
 *   color.cpp  --  This file is part of AVR-ST7735.                      *
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

#include "color.h"

Color::Color(const char* hex) {
    for(int i=0; i<6; i++) {
        this->hexcode[i] = hex[i];
    }
    this->hexcode[6] = '\0';

    this->r = this->hex2int(hex[0]) * 16 + this->hex2int(hex[1]);
    this->g = this->hex2int(hex[2]) * 16 + this->hex2int(hex[3]);
    this->b = this->hex2int(hex[4]) * 16 + this->hex2int(hex[5]);

    this->calculate_cc565();
}

Color::Color(uint16_t _cc565) {
    this->cc565 = _cc565;
}

uint8_t Color::hex2int(char c) {
    if(c >= 48 && c <= 57) {
        return c - 48;
    }
    if(c >= 65 && c <= 70) {
        return c - 55;
    }
    if(c >= 97 && c <= 102) {
        return c - 87;
    }

    return 0;
}

void Color::calculate_cc565() {
    this->cc565 = 0;

    uint16_t j = 15;
    for(int8_t i=7; i>=3; --i) {
        if(this->r & (1 << i)) {
            this->cc565 |= (1 << j);
        }
        --j;
    }
    for(int8_t i=7; i>=2; --i) {
        if(this->g & (1 << i)) {
            this->cc565 |= (1 << j);
        }
        --j;
    }
    for(int8_t i=7; i>=3; --i) {
        if(this->b & (1 << i)) {
            this->cc565 |= (1 << j);
        }
        --j;
    }
}
