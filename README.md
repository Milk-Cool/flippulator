# flippulator
An emulator for the Flipper Zero\
Based on [Flipper Zero Firmware](https://github.com/flipperdevices/flipperzero-firmware)

![Screenshot](https://raw.githubusercontent.com/Milk-Cool/flippulator/main/images/screenshot.png)

## Requirements
A linux system\
Packages `make`, `gcc`, `libncurses5`

## Usage
`npm start` will generate the `out` executable.

## Limitations
- The main function cannot be called `main`
- `furi.h` must be included
- Right now, only the `InputTypePress` and `InputTypeRelease` input types are supported.

## TODOs
- [x] Use ~~GTK~~ SDL instead of ncurses
- [ ] Write support for FuriTypes (FuriString, FuriMutex, etc.)


### Uhm, akstually, it is not an emulator 🤓☝️...
Yes, it is not. It just compiles flipper applications for your computer.