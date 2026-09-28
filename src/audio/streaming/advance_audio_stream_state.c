#include "rnc/audio_streaming_advance_audio_stream_state_types.h"
#include "types.h"
#include "rnc/d_001516d0.h"

struct Pair8 {
    s32 a;
    s32 b;
};
struct M2c_D_00137B80 {
    u8 pad_0[0x2C8];
    struct Pair8 e2C8[6];
    struct Pair8 e2F8[1];
};
extern struct M2c_D_00137B80 D_00137B80;
extern struct M2c_D_001516D0 D_001516D0;
extern s32 D_0015ED88;
extern s32 start_audio_stream_read() __asm__("FUN_00216788");

s32 advance_audio_stream_state(struct M2c_arg0 *arg0) __asm__("FUN_00220648");

s32 advance_audio_stream_state(struct M2c_arg0 *arg0) {

    switch (arg0->unk44) {
    case 0:
        if (arg0->unk48 == 0 || D_001516D0.unk8 != 0) {
            break;
        }
        if (D_00137B80.e2C8[D_0015ED88].b == 0) {
            break;
        }
        if (start_audio_stream_read(arg0->unk48, D_00137B80.e2C8[D_0015ED88].a, D_00137B80.e2C8[D_0015ED88].b) != 0) {
            arg0->unk44 = arg0->unk44 + 1;
        } else {
            arg0->unk44 = -1;
        }
        break;
    case 1:
        if (D_001516D0.unk8 == 0) {
            arg0->unk44 = 2;
        }
        break;
    case 2:
        if (arg0->unk4C == 0 || D_001516D0.unk8 != 0) {
            break;
        }
        if (D_00137B80.e2F8[D_0015ED88].b == 0) {
            break;
        }
        if (start_audio_stream_read(arg0->unk4C, D_00137B80.e2F8[D_0015ED88].a, D_00137B80.e2F8[D_0015ED88].b) != 0) {
            arg0->unk44 = arg0->unk44 + 1;
        } else {
            arg0->unk44 = -1;
        }
        break;
    case 3:
        if (D_001516D0.unk8 == 0) {
            arg0->unk44 = 4;
        }
        break;
    }
    return 0;
}

extern __typeof__(advance_audio_stream_state) func_00220648 __attribute__((alias("FUN_00220648")));
