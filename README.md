# AVR example code for ATMega64A

...and others MCUs with some changes probably.

This is a collection of sample programs I created to learn how to use I/O, timers and interrupts etc. on AVR microcontrollers.

## Example 1. Blink LEDs

### What it does

Alternatingly flashes two LEDs connected to pins 14 & 15 on the ATMega64A.

### Circuit

```
────┐                                       
    ├─ Pin 14 (PB4) ─ ( 1kΩ ) ─ ( LED ) ──┐ 
MCU │                                     │ 
    ├─ Pin 15 (PB5) ─ ( 1kΩ ) ─ ( LED ) ──┤ 
────┘                                    GND
```

## Example 2. Serial Reading & Writing

### What it does

Writes a 16 bit counter to a [LDP-M3604RI-UR](https://www.tme.eu/details/ldp-m3604ri-ur/) 5 digit 7-segment LED numeric display uisng USART1 on the MCU, increments the counter, then reads from USART1 unitl the display responds, and loops as fast as possible forever.

## Circuit

```
────┐                                                      
    ├─ Pin 27 (PD2/RXD1) ────┐                             
MCU │                        │                             
    ├─ Pin 28 (PD3/TXD1) ──┐ │          ┌─────────────────┐
────┘                      │ └──── TX1 ─┤                 │
                           └────── RX1 ─┤ LDP-M3604RI-UR  │
+3.3V ────────────────────────── +3.3V ─┤     [65535]     │
 GND ───────────────────────────── GND ─┤                 │
                                        └─────────────────┘
```

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
avr-gcc -mmcu=atmega64a -Os -DF_CPU=8000000UL -c blink.c -o blink.o
# Link
avr-gcc -mmcu=atmega64a blink.o -o blink.elf
# Create hex file
avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature \
  blink.elf blink.hex
# Flash
avrdude -c usbasp -p m64 -B 16 -U flash:w:blink.hex:i
```

## Makefile

A [`makefile`](makefile) is provided for building, setting fuses and uploading.

### Usage

```sh
make                Compile and convert all examples
make upload-<name>  Upload program to MCU
make fuses          Program ATmega64A fuses
make clean          Remove only build folder
make clean-all      Remove build and hex folders
```

---

```txt
      (I am box)
        |/      
┌───┬───┐       
│   │   │       
├───┼───┤       
│   │   │       
└───┴───┘       
```
