# flippulator
An emulator for the Flipper Zero\
Based on [Flipper Zero Firmware](https://github.com/flipperdevices/flipperzero-firmware)

![Screenshot](https://raw.githubusercontent.com/Milk-Cool/flippulator/main/images/screenshot.png)

## Requirements
A linux system\
Packages `make`, `gcc`, `libsdl2-dev`

## Usage
`npm start` will generate the `out_<app name>/<app name>` executable.

## Limitations
- The main function cannot be called `main`
- `furi.h` must be included
- Right now, only the `InputTypePress` and `InputTypeRelease` input types are supported
- A variable cannot be named `time`

## Known issues
- Defining a string as `char* str = "abc"` and then setting its value using snprintf gives you a segfault
- Storage calls might interact with the outer filesystem

## What's supported so far:
- Canvas and viewports (no icons)
- Input
- Some core functions
- Records
- Vibration
- FuriStrings
- FuriThreads
- FuriSemaphores
- FuriMutexes
- FuriPubSubs
- FuriEventFlags
- FuriApiLocks
- Timers
- Crashing
- CLI (although very limited)
- Storage

## TODOs
- [x] Use ~~GTK~~ SDL instead of ncurses
- [ ] Write support for FuriTypes (FuriString, FuriMutex, etc.)
- [ ] Add settings to the notification service
- [ ] Storage CLI
- [ ] Remove code duplicating in storage_ext.c and storage_int.c


### Uhm, akstually, it is not an emulator 🤓☝️...
Yes, it is not. It just compiles flipper applications for your computer.

## Credits and copyrights
The FontStruction “HaxrCorp 4089” (https://fontstruct.com/fontstructions/show/192981) by “sahwar” is licensed under a Creative Commons Attribution Share Alike license (http://creativecommons.org/licenses/by-sa/3.0/).