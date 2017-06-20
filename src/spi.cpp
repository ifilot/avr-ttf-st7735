/**************************************************************************
 *   spi.cpp  --  This file is part of AVR-ST7735.                        *
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

/*
 * SPI ROUTINES
 *
 *       b7   b6  b5   b4   b3   b2   b1   b0
 * SPCR: SPIE SPE DORD MSTR CPOL CPHA SPR1 SPR0
 *       0    1   0    1    0    0    0    1
 *
 * SPIE - enable SPI interrupt
 * SPE  - enable SPI
 * DORD - 0=MSB first, 1=LSB first
 * MSTR - 0=slave, 1=master
 * CPOL - 0=clock starts low, 1=clock starts high
 * CPHA - 0=read on rising-edge, 1=read on falling-edge
 * SPRx - 00=osc/4, 01=osc/16, 10=osc/64, 11=osc/128
 *
 * SPCR = 0x50: SPI enabled as Master, mode 0, at 16/4 = 4 MHz
 */

#include "spi.h"

SPI::SPI() :
is_open(false) {

}

void SPI::open() {
    SPCR = 0x50;            // SPI enabled as Master, Mode0 at 4 MHz
    SPSR |= (1 << SPI2X);   // double the SPI rate: 4-->8 MHz
    this->is_open = true;
}

void SPI::close() {
    SPCR = 0x00; // clear SPI enable bit
}

uint8_t SPI::transfer_byte(uint8_t data) {
    SPDR = data;    // initiate transfer
    while (!(SPSR & 0x80)){} // wait for transfer to complete
    return SPDR;
}

SPI::~SPI() {
    if(is_open) {
        this->close();
    }
}
