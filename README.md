# Blink LEDs with ATMega64A mcu

## What it does

This is a sample program to learn how to use I/O, timers and interrupts on AVR microcontrollers. The program alternatingly flashes LEDs connected to pins 14 & 15 on the ATMega64A.

## Requirements

On a debian based system install the requirements

```sh
sudo apt install gcc-avr binutils-avr avr-libc avrdude -y
```

or build them yourself using these [instructions](avr-gcc.md).

Add your `avr/include/` location to the include path in your editor.

## Building

```sh
# Compile
avr-gcc -mmcu=atmega64a -Os -DF_CPU=8000000UL -I/usr/lib/avr/include/ -c blink.c -o blink.o
# Link
avr-gcc -mmcu=atmega64a blink.o -o blink.elf
# Create hex file
avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature ./$BUILD_FOLDER/blink.elf ./$BUILD_FOLDER/blink.hex
# Flash
avrdude -c usbasp -p m64 -B 16 -U flash:w:blink.hex:i
```

`makefile` is provided for building, setting fuses and uploading.

## Circuit
```
────┐
    │─ Pin 14 (PB4) ─ ( 1kΩ ) ─ ( LED ) ──┐
MCU │                                     │
    │─ Pin 15 (PB5) ─ ( 1kΩ ) ─ ( LED ) ──┤
────┘                                    GND
```
