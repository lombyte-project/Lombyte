#include "types.h"
#include "rnc/sdk/library/_decMB0.h"

extern u8 D_00153898[];
extern void _Error();
extern void _flushBuf();
extern s16 _ipuVdec(struct MpegDecoder *, s32);
extern void _motionVector();
extern void _motionVectors();
extern s32 _nextBit(struct MpegDecoder *, s32);
extern void _sendIpuCommand();
extern void _waitIpuIdle();
extern void receiveDataFromIPU();
int _decMB0(struct MpegDecoder *decoder, int *mb_type, int *motion_type, int *dct_type_out, struct MpegMbState *arg4,
            int *arg5, int arg6) {
    int *sp20;
    int *sp24;
    u32 g1;
    u32 g2;
    int var_19_84;
    int temp_2_32;
    int temp_3_210;
    int temp_4_287;
    int new_var;
    int temp_6_241;
    int command_base;
    int temp_6_75;
    int temp_7_163;
    int temp_7_200;
    int var_20_86;
    int var_23_95;
    int var_2_103;
    int motion_flag;
    int var_2_172;
    int var_2_213;
    *((int *)0x10002010) = ((*((int *)0x10002010)) & 0xF8FFFFFF) | (decoder->unk150 << 0x18);
    sp20 = dct_type_out;
    sp24 = arg5;
    temp_2_32 = _ipuVdec(decoder, 1);
    *mb_type = temp_2_32;
    if (temp_2_32 == 0) {
        _Error(decoder, D_00153898);
        decoder->unk11C = 1;
        return 0;
    }
    if (temp_2_32 & 0xC) {
        if ((decoder->unk174 == 3) && (decoder->unk17C != 0)) {
            *motion_type = 2;
        } else {
            *motion_type = _nextBit(decoder, 2);
        }
    } else if ((temp_2_32 & 1) && (decoder->unk180 != 0)) {
        *motion_type = ((decoder->unk174 ^ 3) == 0) ? (2) : (1);
    }
    temp_6_75 = decoder->unk174;
    if (temp_6_75 == 3) {
        var_19_84 = (((*motion_type) ^ 1) == 0) ? (2) : (1);
        var_20_86 = (*motion_type) == 2;
    } else {
        new_var = 2;
        var_20_86 = 0;
        var_19_84 = (((*motion_type) ^ new_var) == 0) ? (2) : (1);
    }
    var_23_95 = 0;
    motion_flag = (*motion_type) == 3;
    if (var_20_86 == 0) {
        var_23_95 = temp_6_75 == 3;
    }
    var_2_103 = 0;
    if (((decoder->unk174 == 3) && (decoder->unk17C == 0)) && (((*mb_type) & 3) != 0)) {
        var_2_103 = _nextBit(decoder, 1);
    } else {
        var_2_103 = 0;
    }
    *sp20 = var_2_103;
    if ((*mb_type) & 0x10) {
        decoder->unk1B4 = _nextBit(decoder, 5);
    }
    if (((*mb_type) & 8) || (((*mb_type) & 1) && (decoder->unk180 != 0))) {
        if (decoder->unk848 != 0) {
            _motionVectors(decoder, arg4, arg6, sp24, 0, var_19_84, var_20_86, decoder->unk164 - 1,
                           decoder->unk168 - 1, motion_flag, var_23_95);
        } else {
            temp_7_163 = decoder->unk158 - 1;
            _motionVector(decoder, arg4, arg6, temp_7_163, temp_7_163, 0, 0, decoder->unk154);
        }
    }
    var_2_172 = 0;
    if (decoder->unk11C == 0) {
        if ((*mb_type) & 4) {
            if (decoder->unk848 != 0) {
                _motionVectors(decoder, arg4, arg6, sp24, 1, var_19_84, var_20_86, decoder->unk16C - 1,
                               decoder->unk170 - 1, 0, var_23_95);
            } else {
                temp_7_200 = decoder->unk160 - 1;
                _motionVector(decoder, ((u8 *)arg4) + 8, arg6, temp_7_200, temp_7_200, 0, 0,
                              decoder->unk15C);
            }
        }
        var_2_172 = 0;
        if (decoder->unk11C == 0) {
            temp_3_210 = *mb_type;
            var_2_213 = temp_3_210 & 3;
            if (temp_3_210 & 1) {
                if (decoder->unk180 != 0) {
                    _flushBuf(decoder, 1);
                };
            }
            if (((*mb_type) & 3) != 0) {
                receiveDataFromIPU(
                    *((int *)(((u8 *)(((u8 *)decoder) + (decoder->unk810 * 0x140))) + 0x594)), 0x300);
                _waitIpuIdle(decoder);
                g1 = (((*mb_type) & 1) << 0x1B) | (decoder->unk1B4 << 0x10);
                g2 = ((decoder->unk1B0) << 0x1A) | 0x20000000;
                _sendIpuCommand(decoder, (g1 | g2) | ((*sp20) << 0x19));
            } else {
                *((int *)(((u8 *)(((u8 *)decoder) + (decoder->unk810 * 0x140))) + 0x6CC)) = 1;
            }
            decoder->unk1B0 = 0;
            if (decoder->unk11C != 0) {
                if (decoder->unk174) {
                    return 0;
                }
                return 0;
            }
            if ((((*mb_type) & 1) || ((decoder->unk1B0 = 1, ((*mb_type) & 1) != 0))) &&
                (decoder->unk180 == 0)) {
                arg4->unk14 = 0;
                arg4->unk10 = 0;
                arg4->unk4 = 0;
                arg4->unk0 = 0;
                arg4->unk1C = 0;
                arg4->unk18 = 0;
                arg4->unkC = 0;
                arg4->unk8 = 0;
            }
            temp_4_287 = decoder->unk150;
            var_2_172 = 1;
            if (temp_4_287 == 2) {
                var_2_172 = 1;
                if (!((*mb_type) & 9)) {
                    arg4->unk0 = (arg4->unk4 = (arg4->unk10 = (arg4->unk14 = 0)));
                    if (decoder->unk174 == 3) {
                        *motion_type = temp_4_287;
                    } else {
                        *motion_type = 1;
                        *sp24 = decoder->unk174 == 2;
                    }
                    var_2_172 = 1;
                }
            }
            return var_2_172;
        }
        return var_2_172;
    }
    return var_2_172;
}
