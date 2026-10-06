#include "types.h"

struct MpegPictureContext {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 pad_C[0xA0];
    s32 unkAC;
    u8 pad_B0[0x24];
    s32 unkD4;
    u8 pad_D8[0x40];
    s32 unk118;
    s32 unk11C;
    s32 unk120;
    u8 pad_124[0x50];
    s32 unk174;
};
struct MpegDecoder {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x34];
    struct MpegPictureContext *unk40;
};
extern s32 _decPicture();
extern s32 _dispatchMpegCbNodata();
extern s32 _nextHeader();
extern s32 _sceMpegFlush();
extern s32 _updateRefImage();
extern void _outputFrame();
s32 _decodeOrSkipField(struct MpegDecoder *decoder, s32 arg1, s32 arg2) {
    struct MpegPictureContext *p;
    s32 bVar3;
    unsigned int new_var2;
    s32 iVar4;
    long lVar5;
    s32 uVar2;
    s32 uVar7;
    short new_var;
    s32 gate;
    bVar3 = 0;
    p = decoder->unk40;
    p->unk120 = 0;
    if ((arg2 == (-1)) || (arg1 < arg2)) {
        bVar3 = 1;
    };
    if (p->unk8 == 0) {
        decoder->unk8 = 0;
        p->unk8 = 1;
    }
    lVar5 = _updateRefImage(p, 0);
    if ((lVar5 != 0) && (bVar3 != 0)) {
        _decPicture(p);
    }
    p->unk120 = 1;
    lVar5 = _nextHeader(p);
    if (lVar5 == 0) {
        _sceMpegFlush(decoder);
        p->unk0 = 1;
        return 0;
    }
    iVar4 = 2;
    if (p->unkD4 != 1) {
        iVar4 = 1;
    }
    if (p->unk174 != iVar4) {
        return -1;
    }
    new_var2 = _updateRefImage(p, 1);
    gate = 0;
    if (new_var2 != 0) {
        gate = 1;
    }
    uVar7 = 0;
    if (gate != 0) {
        if (bVar3 == 0) {
            uVar2 = p->unk118;
            goto out;
        }
        lVar5 = _decPicture(p);
        if (lVar5 != 0) {
            uVar7 = 1;
        }
    };
out:
    _outputFrame(p, p->unk118, p->unk4);

    p->unk120 = 0;
    decoder->unk8 = p->unk118 - p->unkAC;
    p->unk118 = p->unk118 + 1;
    p->unk4 = (unsigned long long)(p->unk4 + 1);
    if (bVar3 == 0) {
        _dispatchMpegCbNodata(decoder);
    }
    return uVar7;
}
