#include "types.h"
struct MpegDecoder {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x34];
    struct MpegPictureContext *unk40;
};
struct MpegPictureContext {
    u8 pad_0[0x4];
    s32 unk4;
    s32 unk8;
    u8 pad_C[0xA0];
    s32 unkAC;
    u8 pad_B0[0x68];
    s32 unk118;
    u8 pad_11C[0x4];
    s32 unk120;
    u8 pad_124[0x50];
    s32 unk174;
};
extern s32 _decPicture(struct MpegPictureContext *a0);
extern void _dispatchMpegCbNodata(struct MpegDecoder *a0);
extern void _outputFrame(struct MpegPictureContext *a0, s32 a1, s32 a2);
extern s32 _updateRefImage(struct MpegPictureContext *a0, s32 a1);
s32 _decodeOrSkipFrame(struct MpegDecoder *decoder, s32 arg1, s32 arg2) {
    s32 ret;
    s32 flag;
    s32 t;
    struct MpegPictureContext *tmp;
    flag = 0;
    tmp = decoder->unk40;
    if ((arg2 == -1) || (arg1 < arg2)) {
        if (tmp->unk8 == 0) {
            decoder->unk8 = 0;
            tmp->unk8 = 1;
        }
        if (_updateRefImage(tmp, 0) == 0) {
            t = 0;
        } else {
            t = 0;
            t = _decPicture(tmp) != t;
        }
        ret = t;
    } else {
        flag = 1;
        ret = _updateRefImage(tmp, 0);
        _dispatchMpegCbNodata(decoder);
    }
    _outputFrame(tmp, tmp->unk118, tmp->unk4);
    if ((tmp->unk174 != 3) && (flag == 0)) {
        tmp->unk120 = (s32)(tmp->unk120 == 0);
    }
    decoder->unk8 = (s32)(tmp->unk118 - tmp->unkAC);
    if (tmp->unk120 == 0) {
        tmp->unk118 = (s32)(tmp->unk118 + 1);
        tmp->unk4 = (s32)(tmp->unk4 + 1);
    }
    return ret;
}
