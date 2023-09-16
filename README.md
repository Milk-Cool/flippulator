# flippulator
An emulator for the Flipper Zero\
Based on [Flipper Zero Firmware](https://github.com/flipperdevices/flipperzero-firmware)

## Requirements
A linux system\
Packages `make`, `gcc`, `libncurses5`

## Usage

## Limitations
* The main function cannot be called `main`
* `furi.h` must be included
* `furi.h` includes `ncurses.h`, which reserves some function names such as `clear` and `refresh`
* Due to ncurses limitations, only the `InputTypePress` input type is supported.