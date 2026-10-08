#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern s32 _decPicture(struct MpegDecoder *a0);
extern void _dispatchMpegCbNodata(struct sceMpeg *a0);
extern void _outputFrame(struct MpegDecoder *a0, s32 a1, s32 a2);
extern s32 _updateRefImage(struct MpegDecoder *a0, s32 a1);
s32 _decodeOrSkipFrame(struct sceMpeg *decoder, s32 arg1, s32 arg2) {
    s32 ret;
    s32 flag;
    s32 t;
    struct MpegDecoder *tmp;
    flag = 0;
    tmp = decoder->sys;
    if ((arg2 == -1) || (arg1 < arg2)) {
        if (tmp->unk8 == 0) {
            decoder->frameCount = 0;
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
    _outputFrame(tmp, tmp->frame_count, tmp->unk4);
    if ((tmp->picture_structure != 3) && (flag == 0)) {
        tmp->second_field = (s32)(tmp->second_field == 0);
    }
    decoder->frameCount = (s32)(tmp->frame_count - tmp->frame_base);
    if (tmp->second_field == 0) {
        tmp->frame_count = (s32)(tmp->frame_count + 1);
        tmp->unk4 = (s32)(tmp->unk4 + 1);
    }
    return ret;
}
