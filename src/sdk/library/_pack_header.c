#include "types.h"
struct PackHeaderInfo {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

extern s32 SignExtendPackedValue();
extern s32 _sysbitGet();
extern s32 _sysbitMarker();
extern s32 _system_header();
s32 _pack_header(s32 arg0, struct PackHeaderInfo *arg1) {
    u32 temp_16_20;
    u32 temp_17_26;
    u32 temp_18_32;
    u32 temp_20_45;
    u32 var_21_16;

    var_21_16 = 0;
    _sysbitGet(arg0, 0x22);
    temp_16_20 = _sysbitGet(arg0, 3);
    _sysbitMarker(arg0);
    temp_17_26 = _sysbitGet(arg0, 0xF);
    _sysbitMarker(arg0);
    temp_18_32 = _sysbitGet(arg0, 0xF);
    _sysbitMarker(arg0);
    arg1->unk0 = _sysbitGet(arg0, 9);
    _sysbitGet(arg0, 0x1E);
    temp_20_45 = _sysbitGet(arg0, 3);
    arg1->unk8 = (s32)((temp_16_20 >> 2) & 1);
    arg1->unk4 = (s32)((temp_16_20 << 0x1E) | (temp_17_26 << 0xF) | temp_18_32);
    if (temp_20_45 != 0) {
        do {
            do {
                _sysbitGet(arg0, 8);
                var_21_16 += 1;
            } while (var_21_16 < temp_20_45);
        } while (0);
    }
    if (SignExtendPackedValue(arg0, 0x20) == 0x1BB) {
        arg1->unkC = 1;
        _system_header(arg0, arg1);
    } else {
        arg1->unkC = 0;
    }
    return 1;
}
