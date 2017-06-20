/**************************************************************************
 *   main.cpp  --  This file is part of AVR-ST7735.                       *
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

#include <avr/io.h>

#include "ttf_st7735.h"

void setup_ports() {
    DDRB = 0x2F;        // 0010.1111; set B0-B3, B5 as outputs
    DDRC = 0x00;        // 0000.0000; set PORTC as inputs
    PORTB |= (1 << 0);  // start with TFT reset line inactive high
}

int main() {
    setup_ports(); // use PortB for LCD interface
    TTF_ST7735 ttf;
    ttf.clear_screen();

    const char* txt = "Hello world\0";

    Color red("FF0000");
    Color green("00FF00");
    Color blue("0000FF");

    ttf.draw_text(txt, 0, 0, red);
    ttf.draw_text(txt, 0, 7, green);
    ttf.draw_text(txt, 0, 14, blue);
}
