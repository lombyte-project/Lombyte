#include "types.h"

struct MotionVector {
    u8  pad_0[0x4];
    s32 unk4;
};

struct MotionVectorStore {
    s32 unk0;
    s32 unk4;
};

extern s32 decode_dmvector() __asm__("InitializeMemoryCardDirectory");
extern s32 _ipuVdec();
extern s32 _nextBit();
extern s32 decode_motion_vector() __asm__("func_001280A8");

void _motionVector(s32                       decoder_state,
                   struct MotionVector      *motion_vector,
                   struct MotionVectorStore *auxiliary_vlc_values,
                   s32                       first_component_bit_count,
                   s32                       second_component_bit_count,
                   s32                       decode_auxiliary_vlc,
                   s32                       halve_second_component_for_decode,

                   s32 use_half_scale) {
    s32 code;
    s32 bits;

    code = _ipuVdec(decoder_state, 2);
    if (first_component_bit_count != 0 && code != 0) {
        bits = _nextBit(decoder_state, first_component_bit_count);
    } else {
        bits = 0;
    }
    decode_motion_vector(motion_vector, first_component_bit_count, code, bits, use_half_scale);
    if (decode_auxiliary_vlc != 0) {
        auxiliary_vlc_values->unk0 = decode_dmvector(decoder_state);
    }
    code = _ipuVdec(decoder_state, 2);
    if (second_component_bit_count != 0 && code != 0) {
        bits = _nextBit(decoder_state, second_component_bit_count);
    } else {
        bits = 0;
    }
    if (halve_second_component_for_decode != 0) {
        motion_vector->unk4 >>= 1;
    }
    decode_motion_vector(
        (u8 *)motion_vector + 4, second_component_bit_count, code, bits, use_half_scale);
    if (halve_second_component_for_decode != 0) {
        motion_vector->unk4 *= 2;
    }
    if (decode_auxiliary_vlc != 0) {
        auxiliary_vlc_values->unk4 = decode_dmvector(decoder_state);
    }
}
