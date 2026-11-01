#include "types.h"

struct PackHeaderInfo {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};

extern s32 peek_bits() __asm__("SignExtendPackedValue");
extern s32 _sysbitGet();
extern s32 _sysbitMarker();
extern s32 _system_header();

s32 _pack_header(s32 bitstream_context, struct PackHeaderInfo *pack_header_info) {
    u32 scr_high;
    u32 scr_mid;
    u32 scr_low;
    u32 stuffing_length;
    u32 skipped;

    skipped = 0;
    _sysbitGet(bitstream_context, 0x22);
    scr_high = _sysbitGet(bitstream_context, 3);
    _sysbitMarker(bitstream_context);
    scr_mid = _sysbitGet(bitstream_context, 0xF);
    _sysbitMarker(bitstream_context);
    scr_low = _sysbitGet(bitstream_context, 0xF);
    _sysbitMarker(bitstream_context);
    pack_header_info->unk0 = _sysbitGet(bitstream_context, 9);
    _sysbitGet(bitstream_context, 0x1E);
    stuffing_length = _sysbitGet(bitstream_context, 3);
    pack_header_info->unk8 = (s32)((scr_high >> 2) & 1);
    pack_header_info->unk4 = (s32)((scr_high << 0x1E) | (scr_mid << 0xF) | scr_low);
    if (stuffing_length != 0) {
        do {
            do {
                _sysbitGet(bitstream_context, 8);
                skipped += 1;
            } while (skipped < stuffing_length);
        } while (0);
    }
    if (peek_bits(bitstream_context, 0x20) == 0x1BB) {
        pack_header_info->unkC = 1;
        _system_header(bitstream_context, pack_header_info);
    } else {
        pack_header_info->unkC = 0;
    }
    return 1;
}
