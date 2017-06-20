# WinAVR cross-compiler toolchain is used here
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
DUDE = avrdude
F_CPU = 12000000
FUSEL = 0xd8
FUSEH = 0xff
HEX = image.hex
OPTS = -Os

# set dirs
OBJDIR = obj
SRCDIR = src

# update the lines below to match your configuration
CFLAGS = -Wall $(OPTS) -mmcu=atmega328p -DF_CPU=$(F_CPU) -DDEBUG_LEVEL=0 -std=c++11 -I$(SRCDIR)
OBJFLAGS = -j .text -j .data -O ihex
DUDEFLAGS = -p atmega328p -c usbasp

# Object files for the firmware (usbdrv/oddebug.o not strictly needed I think)
SOURCES = main.cpp ttf_st7735.cpp spi.cpp color.cpp
OBJECTS= $(SOURCES:%.cpp=$(OBJDIR)/%.o)

# By default, build the firmware and command-line client, but do not flash
all: $(OBJDIR)/$(HEX)

# With this, you can flash the firmware by just typing "make flash" on command-line
flash: $(OBJDIR)/$(HEX)
	sudo $(DUDE) $(DUDEFLAGS) -U flash:w:$<

# rule for programming fuse bits:
fuse:
	@[ "$(FUSEH)" != "" -a "$(FUSEL)" != "" ] || \
	{ echo "*** Edit Makefile and choose values for FUSEL and FUSEH!"; exit 1; }
	sudo $(DUDE) $(DUDEFLAGS) -U hfuse:w:$(FUSEH):m -U lfuse:w:$(FUSEL):m -v

# Housekeeping if you want it
clean:
	$(RM) $(OBJDIR)/*.o $(OBJDIR)/*.hex $(OBJDIR)/*.elf

# From .elf file to .hex
$(OBJDIR)/%.hex: $(OBJDIR)/%.elf
	$(OBJCOPY) $(OBJFLAGS) $< $@

# Main.elf requires additional objects to the firmware, not just image.o
$(OBJDIR)/image.elf: $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) -o $@

# Without this dependance, .o files will not be recompiled if you change
# the config! I spent a few hours debugging because of this...
#$(OBJECTS):

# From C source to .o object file
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

# From C source to .o object file
%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

# From assembler source to .o object file
%.o: %.S
	$(CXX) $(CFLAGS) -x assembler-with-cpp -c $< -o $@
