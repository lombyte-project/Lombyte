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
#include "rnc/storage/disc_table.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/globals.h"

extern s32 start_audio_stream_read() __asm__("FUN_00216788");

s32 advance_audio_stream_state(struct AudioStream *stream) __asm__("FUN_00220648");

s32 advance_audio_stream_state(struct AudioStream *stream) {

    switch (stream->unk44) {
    case 0:
        if (stream->unk48 == 0 || music_stream_state.read_state != 0) {
            break;
        }
        if (disc_table.unk2C8[game_language].size == 0) {
            break;
        }
        if (start_audio_stream_read(stream->unk48, disc_table.unk2C8[game_language].sector,
                                    disc_table.unk2C8[game_language].size) != 0) {
            stream->unk44 = stream->unk44 + 1;
        } else {
            stream->unk44 = -1;
        }
        break;
    case 1:
        if (music_stream_state.read_state == 0) {
            stream->unk44 = 2;
        }
        break;
    case 2:
        if (stream->unk4C == 0 || music_stream_state.read_state != 0) {
            break;
        }
        if (disc_table.unk2F8[game_language].size == 0) {
            break;
        }
        if (start_audio_stream_read(stream->unk4C, disc_table.unk2F8[game_language].sector,
                                    disc_table.unk2F8[game_language].size) != 0) {
            stream->unk44 = stream->unk44 + 1;
        } else {
            stream->unk44 = -1;
        }
        break;
    case 3:
        if (music_stream_state.read_state == 0) {
            stream->unk44 = 4;
        }
        break;
    }
    return 0;
}

extern __typeof__(advance_audio_stream_state) func_00220648 __attribute__((alias("FUN_00220648")));
