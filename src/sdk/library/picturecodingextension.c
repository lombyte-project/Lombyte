#include "types.h"
struct MpegDecoder {
    u8 pad_0[0xD4];
    s32 unkD4;
    u8 pad_D8[0x8C];
    s32 unk164;
    s32 unk168;
    s32 unk16C;
    s32 unk170;
    s32 unk174;
    s32 unk178;
    s32 unk17C;
    s32 unk180;
    s32 unk184;
    s32 unk188;
};

extern s32 _nextBit();
void _pictureCodingExtension(struct MpegDecoder *mpeg) {
    s32 temp_2_35;
    s32 temp_6_65;
    s32 temp_6_77;
    s32 temp_6_89;

    mpeg->unk164 = _nextBit(mpeg, 4);
    mpeg->unk168 = _nextBit(mpeg, 4);
    mpeg->unk16C = _nextBit(mpeg, 4);
    mpeg->unk170 = _nextBit(mpeg, 4);
    *(s32 *)0x10002010 = (*(s32 *)0x10002010 & 0xFFFCFFFF) | (_nextBit(mpeg, 2) << 0x10);
    temp_2_35 = _nextBit(mpeg, 2);
    mpeg->unk174 = temp_2_35;
    if (mpeg->unkD4 == 0) {
        mpeg->unkD4 = temp_2_35;
    }
    mpeg->unk178 = _nextBit(mpeg, 1);
    mpeg->unk17C = _nextBit(mpeg, 1);
    mpeg->unk180 = _nextBit(mpeg, 1);
    temp_6_65 = (*(volatile u32 *)0x10002010 & 0xFFBFFFFF) | (_nextBit(mpeg, 1) << 0x16);
    *(volatile u32 *)0x10002010 = temp_6_65;
    temp_6_77 = (*(volatile u32 *)0x10002010 & 0xFFDFFFFF) | (_nextBit(mpeg, 1) << 0x15);
    *(volatile u32 *)0x10002010 = temp_6_77;
    temp_6_89 = (*(volatile u32 *)0x10002010 & 0xFFEFFFFF) | (_nextBit(mpeg, 1) << 0x14);
    *(volatile u32 *)0x10002010 = temp_6_89;
    mpeg->unk184 = _nextBit(mpeg, 1);
    _nextBit(mpeg, 1);
    mpeg->unk188 = _nextBit(mpeg, 1);
    if (_nextBit(mpeg, 1) != 0) {
        _nextBit(mpeg, 1);
        _nextBit(mpeg, 3);
        _nextBit(mpeg, 1);
        _nextBit(mpeg, 7);
        _nextBit(mpeg, 8);
    }
}
