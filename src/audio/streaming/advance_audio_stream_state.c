#include "rnc/audio/streaming/advance_audio_stream_state.h"
#include "types.h"
#include "rnc/audio/music/music_stream_state.h"

struct Pair8 {
    s32 a;
    s32 b;
};
struct Globals_00137B80 {
    u8 pad_0[0x2C8];
    struct Pair8 e2C8[6];
    struct Pair8 e2F8[1];
};
extern struct Globals_00137B80 D_00137B80;
extern struct MusicStreamState D_001516D0;
extern s32 D_0015ED88;
extern s32 start_audio_stream_read() __asm__("FUN_00216788");

s32 advance_audio_stream_state(struct AudioStream *stream) __asm__("FUN_00220648");

s32 advance_audio_stream_state(struct AudioStream *stream) {

    switch (stream->unk44) {
    case 0:
        if (stream->unk48 == 0 || D_001516D0.pending_start_state != 0) {
            break;
        }
        if (D_00137B80.e2C8[D_0015ED88].b == 0) {
            break;
        }
        if (start_audio_stream_read(stream->unk48, D_00137B80.e2C8[D_0015ED88].a,
                                    D_00137B80.e2C8[D_0015ED88].b) != 0) {
            stream->unk44 = stream->unk44 + 1;
        } else {
            stream->unk44 = -1;
        }
        break;
    case 1:
        if (D_001516D0.pending_start_state == 0) {
            stream->unk44 = 2;
        }
        break;
    case 2:
        if (stream->unk4C == 0 || D_001516D0.pending_start_state != 0) {
            break;
        }
        if (D_00137B80.e2F8[D_0015ED88].b == 0) {
            break;
        }
        if (start_audio_stream_read(stream->unk4C, D_00137B80.e2F8[D_0015ED88].a,
                                    D_00137B80.e2F8[D_0015ED88].b) != 0) {
            stream->unk44 = stream->unk44 + 1;
        } else {
            stream->unk44 = -1;
        }
        break;
    case 3:
        if (D_001516D0.pending_start_state == 0) {
            stream->unk44 = 4;
        }
        break;
    }
    return 0;
}

extern __typeof__(advance_audio_stream_state) func_00220648 __attribute__((alias("FUN_00220648")));
