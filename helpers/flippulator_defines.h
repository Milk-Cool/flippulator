#pragma once

// Settings
#define FLIPPULATOR_HQ_AUDIO

// Audio defines
#ifdef FLIPPULATOR_HQ_AUDIO
#define AUDIO_FREQUENCY 48000
#else
#define AUDIO_FREQUENCY 44100
#endif