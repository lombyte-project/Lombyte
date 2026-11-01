#ifndef LOMBYTE_RNC_AUDIO_STREAMING_H
#define LOMBYTE_RNC_AUDIO_STREAMING_H

#include "types.h"

struct AudioStream;

s32  advance_audio_stream_state(struct AudioStream *) __asm__("FUN_00220648");
void finish_audio_stream_read(s32) __asm__("FUN_00216950");

#endif /* LOMBYTE_RNC_AUDIO_STREAMING_H */
