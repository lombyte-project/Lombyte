#ifndef LOMBYTE_RNC_AUDIO_STREAMING_ADVANCE_AUDIO_STREAM_STATE_H
#define LOMBYTE_RNC_AUDIO_STREAMING_ADVANCE_AUDIO_STREAM_STATE_H

#include "types.h"

struct AudioStream {
    u8 pad_0[0x44];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
};

struct AudioStreamTableA {
    u8 pad_0[0x2C8];
    s32 unk2C8;
    s32 unk2CC;
};

struct AudioStreamTableB {
    u8 pad_0[0x2F8];
    s32 unk2F8;
    s32 unk2FC;
};

#endif /* LOMBYTE_RNC_AUDIO_STREAMING_ADVANCE_AUDIO_STREAM_STATE_H */
