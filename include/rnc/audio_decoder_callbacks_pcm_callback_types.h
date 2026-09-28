#ifndef RNC_AUDIO_DECODER_CALLBACKS_PCM_CALLBACK_TYPES_H
#define RNC_AUDIO_DECODER_CALLBACKS_PCM_CALLBACK_TYPES_H

#include "types.h"

struct M2c_arg1 {
    u8 pad_0[0x8];
    s32 unk8;
    s32 unkC;
};

struct M2c_arg2 {
    u8 pad_0[0x50008];
    s32 unk50008;
};

#endif /* RNC_AUDIO_DECODER_CALLBACKS_PCM_CALLBACK_TYPES_H */
