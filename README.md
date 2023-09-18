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

## Known issues
- Defining a string as `char* str = "abc"` and then setting its value using snprintf gives you a segfault

## TODOs
- [x] Use ~~GTK~~ SDL instead of ncurses
- [ ] Write support for FuriTypes (FuriString, FuriMutex, etc.)


### Uhm, akstually, it is not an emulator 🤓☝️...
Yes, it is not. It just compiles flipper applications for your computer.

## Credits and copyrights
The FontStruction “HaxrCorp 4089” (https://fontstruct.com/fontstructions/show/192981) by “sahwar” is licensed under a Creative Commons Attribution Share Alike license (http://creativecommons.org/licenses/by-sa/3.0/).