#ifndef LOMBYTE_RNC_AUDIO_STREAMING_REQUEST_AUDIO_STREAM_BREAK_H
#define LOMBYTE_RNC_AUDIO_STREAMING_REQUEST_AUDIO_STREAM_BREAK_H

#include "types.h"

struct MusicStreamState {
    u8 pad_0[0x8];
    s16 unk8;
    u8 unkA;
    u8 pad_B;
    s32 unkC;
    s32 unk10;
    s32 unk14;
};

#endif /* LOMBYTE_RNC_AUDIO_STREAMING_REQUEST_AUDIO_STREAM_BREAK_H */