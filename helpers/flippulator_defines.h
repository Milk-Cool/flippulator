#pragma once

// Settings
#define FLIPPULATOR_HQ_AUDIO
// #define FLIPPULATOR_SINE_WAVE

// Audio defines
#ifdef FLIPPULATOR_HQ_AUDIO
#define AUDIO_FREQUENCY 48000
#else
#define AUDIO_FREQUENCY 44100
#endif
#ifdef FLIPPULATOR_SINE_WAVE
#define AUDIO_WAVE_TYPE 0
#else
#define AUDIO_WAVE_TYPE 1
#endif

// WebAssembly defines
#if defined(__EMSCRIPTEN__) || defined(__wasm__) || defined(__wasm32__) || defined(__wasm64__)
#define FLIPPULATOR_USES_WEBASSEMBLY
#endif