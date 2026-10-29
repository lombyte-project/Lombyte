#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern s32 _nextBit();
void _pictureCodingExtension(struct MpegDecoder *mpeg) {
    s32 temp_2_35;
    s32 temp_6_65;
    s32 temp_6_77;
    s32 temp_6_89;

    mpeg->f_code_forward_h = _nextBit(mpeg, 4);
    mpeg->f_code_forward_v = _nextBit(mpeg, 4);
    mpeg->f_code_backward_h = _nextBit(mpeg, 4);
    mpeg->f_code_backward_v = _nextBit(mpeg, 4);
    *(s32 *)0x10002010 = (*(s32 *)0x10002010 & 0xFFFCFFFF) | (_nextBit(mpeg, 2) << 0x10);
    temp_2_35 = _nextBit(mpeg, 2);
    mpeg->picture_structure = temp_2_35;
    if (mpeg->unkD4 == 0) {
        mpeg->unkD4 = temp_2_35;
    }
    mpeg->top_field_first = _nextBit(mpeg, 1);
    mpeg->frame_pred_frame_dct = _nextBit(mpeg, 1);
    mpeg->concealment_motion_vectors = _nextBit(mpeg, 1);
    temp_6_65 = (*(volatile u32 *)0x10002010 & 0xFFBFFFFF) | (_nextBit(mpeg, 1) << 0x16);
    *(volatile u32 *)0x10002010 = temp_6_65;
    temp_6_77 = (*(volatile u32 *)0x10002010 & 0xFFDFFFFF) | (_nextBit(mpeg, 1) << 0x15);
    *(volatile u32 *)0x10002010 = temp_6_77;
    temp_6_89 = (*(volatile u32 *)0x10002010 & 0xFFEFFFFF) | (_nextBit(mpeg, 1) << 0x14);
    *(volatile u32 *)0x10002010 = temp_6_89;
    mpeg->repeat_first_field = _nextBit(mpeg, 1);
    _nextBit(mpeg, 1);
    mpeg->progressive_frame = _nextBit(mpeg, 1);
    if (_nextBit(mpeg, 1) != 0) {
        _nextBit(mpeg, 1);
        _nextBit(mpeg, 3);
        _nextBit(mpeg, 1);
        _nextBit(mpeg, 7);
        _nextBit(mpeg, 8);
    }
}
