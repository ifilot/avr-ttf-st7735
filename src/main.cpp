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
    DDRD = 0b00000011;
    PORTB |= (1 << 0);  // start with TFT reset line inactive high
}

int main() {
    setup_ports(); // use PortB for LCD interface
    PORTD |= (1 << 0);	// boot program indicator on PIND0
    TTF_ST7735 ttf;
    ttf.clear_screen();

    Color colors[5];

    colors[0] = Color("EC4E20");
    colors[1] = Color("00FF00");
    colors[2] = Color("FF9505");
    colors[3] = Color("016FB9");
    colors[4] = Color("353531");

    uint8_t y = 0;
    for(unsigned int i=0; i<5; i++) {
        ttf.draw_text(colors[i].get_hexcode(), 10, y, colors[i]);
        ttf.draw_rectangle_solid(0,y,7,7,colors[i]);
        y += 7;
    }

    // blinking led on PIND1
    while(1) {
        PORTD |= (1 << 1);
        _delay_ms(500);
        PORTD &= ~(1 << 1);
        _delay_ms(500);
    }
}
