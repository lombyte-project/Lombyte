#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x34];
    struct M2c_temp_16_13 * unk40;
};
struct M2c_temp_16_13 {
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
extern s32 _decPicture(struct M2c_temp_16_13 *a0);
extern void _dispatchMpegCbNodata(struct M2c_arg0 *a0);
extern void _outputFrame(struct M2c_temp_16_13 *a0, s32 a1, s32 a2);
extern s32 _updateRefImage(struct M2c_temp_16_13 *a0, s32 a1);
s32 _decodeOrSkipFrame(struct M2c_arg0 *arg0, s32 arg1, s32 arg2) {
    s32 ret;
    s32 flag;
    s32 t;
    struct M2c_temp_16_13 *tmp;
    flag = 0;
    tmp = arg0->unk40;
    if ((arg2 == -1) || (arg1 < arg2)) {
        if (tmp->unk8 == 0) {
            arg0->unk8 = 0;
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
        _dispatchMpegCbNodata(arg0);
    }
    _outputFrame(tmp, tmp->unk118, tmp->unk4);
    if ((tmp->unk174 != 3) && (flag == 0)) {
        tmp->unk120 = (s32) (tmp->unk120 == 0);
    }
    arg0->unk8 = (s32) (tmp->unk118 - tmp->unkAC);
    if (tmp->unk120 == 0) {
        tmp->unk118 = (s32) (tmp->unk118 + 1);
        tmp->unk4 = (s32) (tmp->unk4 + 1);
    }
    return ret;
}
