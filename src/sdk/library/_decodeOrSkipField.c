#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern s32 _decPicture();
extern s32 _dispatchMpegCbNodata();
extern s32 _nextHeader();
extern s32 _sceMpegFlush();
extern s32 _updateRefImage();
extern void _outputFrame();
s32 _decodeOrSkipField(struct sceMpeg *decoder, s32 arg1, s32 arg2) {
    struct MpegDecoder *p;
    s32 bVar3;
    unsigned int new_var2;
    s32 iVar4;
    long lVar5;
    s32 uVar2;
    s32 uVar7;
    short new_var;
    s32 gate;
    bVar3 = 0;
    p = decoder->sys;
    p->second_field = 0;
    if ((arg2 == (-1)) || (arg1 < arg2)) {
        bVar3 = 1;
    };
    if (p->unk8 == 0) {
        decoder->frameCount = 0;
        p->unk8 = 1;
    }
    lVar5 = _updateRefImage(p, 0);
    if ((lVar5 != 0) && (bVar3 != 0)) {
        _decPicture(p);
    }
    p->second_field = 1;
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
    if (p->picture_structure != iVar4) {
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
            uVar2 = p->frame_count;
            goto out;
        }
        lVar5 = _decPicture(p);
        if (lVar5 != 0) {
            uVar7 = 1;
        }
    };
out:
    _outputFrame(p, p->frame_count, p->unk4);

    p->second_field = 0;
    decoder->frameCount = p->frame_count - p->frame_base;
    p->frame_count = p->frame_count + 1;
    p->unk4 = (unsigned long long)(p->unk4 + 1);
    if (bVar3 == 0) {
        _dispatchMpegCbNodata(decoder);
    }
    return uVar7;
}
