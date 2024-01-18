# Update
As you can see, I kinda forgot about this project. It's been nearly three months since the last commit and it's just a simple bugfix.
If you want to mantain this project, message me on Telegram: @milk_cool

# flippulator
An emulator for the Flipper Zero\
Based on [Flipper Zero Firmware](https://github.com/flipperdevices/flipperzero-firmware)

![Screenshot](https://raw.githubusercontent.com/Milk-Cool/flippulator/main/images/screenshot.png)

## Requirements
A linux system\
Packages `make`, `gcc`, `libsdl2-dev:i386`, `gcc-12-multilib` (or an alternative that's compatible with your current gcc version), `libbsd-dev:i386`

## Usage
`npm start` will generate the `out_<app name>/<app name>` executable.

## Limitations
- The main function cannot be called `main`
- `furi.h` must be included
- Right now, only the `InputTypePress` and `InputTypeRelease` input types are supported
- A variable cannot be named `time`
- The `%lu` and `%ld` format specifiers expect just `long unsigned int`, so `uint32_t`s (defined as `unsigned int`s) and `int32`s (defined as `int`s) won't work with them despite having the same size. (As of October 25th, 2023, it does not look like it's possible to fix this. As an alternative, I can suggest using macros like PRIu32.)

## Known issues
- Defining a string as `char* str = "abc"` and then setting its value using snprintf gives you a segfault\

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

## FAQs
> *Q: How does the application tell if it's running in Flippulator?*

A: Flippulator defines `_FLIPPULATOR` and the app can use `#ifdef` to tell in which environment it's running.


## Uhm, akstually, it is not an emulator 🤓☝️...
Yes, it is not. It just compiles flipper applications for your computer.

## Credits and copyrights
The FontStruction “HaxrCorp 4089” (https://fontstruct.com/fontstructions/show/192981) by “sahwar” is licensed under a Creative Commons Attribution Share Alike license (http://creativecommons.org/licenses/by-sa/3.0/).