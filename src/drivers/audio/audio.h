#ifndef AUDIO_H
#define AUDIO_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    AUDIO_OK,
    AUDIO_ERROR_INIT,
    AUDIO_ERROR_PLAY,
    AUDIO_ERROR_STOP,
    AUDIO_ERROR_TIMEOUT
} audio_status_t;

typedef struct {
    uint32_t sample_rate;
    uint8_t bit_depth;
    uint8_t channels;
} audio_config_t;

audio_status_t audio_init(const audio_config_t *config);
audio_status_t audio_play(const uint8_t *buffer, size_t size);
audio_status_t audio_stop();
audio_status_t audio_queue_buffer(const uint8_t *buffer, size_t size);
audio_status_t audio_get_buffer_status(size_t *remaining_bytes);
audio_status_t audio_set_volume(uint8_t volume);
audio_status_t audio_get_volume(uint8_t *volume);
void audio_interrupt_handler();

#endif // AUDIO_H
