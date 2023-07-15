#ifndef AUDIO_H
#define AUDIO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Initialize the audio driver
void audio_init();

// Play audio from a buffer
void audio_play(const uint8_t* buffer, size_t size);

// Stop audio playback
void audio_stop();

#endif