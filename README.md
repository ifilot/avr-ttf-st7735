# AVR-TTF-ST7735 Library

A C++ library for the AVR to control the ST7735 1.8" TTF display.

## Connections


### RB-RFR1.8-V2 from JOY-IT Europe GMBH
```
AVR         |  TTF display
PB0        --> RES
PB1        --> RS/DC
PB3 (MOSI) --> SDA
PB5 (CLK)  --> SCL
GND        --> CS
GND        --> GND
VCC        --> VCC
```

### ST7735R (bought from Ebay)
```
AVR         |  TTF display
PB0        --> RESET
PB1        --> A0
PB3 (MOSI) --> MOSI
PB5 (SCK)  --> SCL
GND        --> CS
GND        --> GND
VCC        --> VCC
GND        --> LED- (backlight)
VCC        --> LED+ (backlight)
```
