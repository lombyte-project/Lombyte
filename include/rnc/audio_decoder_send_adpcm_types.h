#ifndef RNC_AUDIO_DECODER_SEND_ADPCM_TYPES_H
#define RNC_AUDIO_DECODER_SEND_ADPCM_TYPES_H

#include "types.h"

struct M2c_D_001612BC {
    u8 pad_0[0x1];
    s32 unk1;
    u8 pad_5[0xC];
    s32 unk11;
    u8 pad_15[0x3DC];
    u8 unk3F1;
    u8 pad_3F2[0x3];
};

struct M2c_arg0 {
    s32 unk0;
    u8 pad_4[0x14];
    s32 unk18;
    s32 unk1C;
    u8 pad_20[0x14];
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    s32 unk40;
    u8 pad_44[0xC];
    s32 unk50;
    u8 pad_54[0xC];
    s32 unk60;
};

#endif /* RNC_AUDIO_DECODER_SEND_ADPCM_TYPES_H */
