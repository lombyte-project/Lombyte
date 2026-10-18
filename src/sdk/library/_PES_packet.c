/* libmpeg: parse one MPEG-2 PES packet header from the sysbit stream bs into
   hdr (stream id, length, PTS/DTS, payload offset/size); streams without a
   PES header (map, padding, private 2, ECM/EMM, directory, DSM-CC, H.222 E)
   are skipped whole. */

#include "types.h"

extern u8 D_001539C8[];
extern u8 D_001539D8[];
extern s32 _Error();
extern s32 _sysbitGet();
extern s32 _sysbitJump();
extern void _sysbitMarker();

struct PES_BS {
    u8 pad_0[0x18];
    s64 stamp;
};

struct PES_HDR {
    s64 unk0;
    s32 unk8;
    s32 unkC;
    s64 unk10;
    s64 unk18;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

struct PES_TBL {
    u8 v[16];
} __attribute__((packed));

s32 _PES_packet(s32 error_context, struct PES_BS *bs, struct PES_HDR *hdr) {
    struct PES_TBL tbl;
    s32 ctx;
    s32 flag1;
    s32 len;  /* PES_header_data_length */
    s32 base; /* (s32)bs->stamp, captured before the header fields */
    s64 now;  /* the same stamp, re-read at the sync point */
    s32 pts_dts_flags;   /* PTS_DTS_flags */
    s32 header_skip_table_index;   /* 4-bit length, indexes tbl */
    s32 pes_extension_flag;   /* PES_extension_flag */

    ctx = error_context;
    hdr->unk28 = (s32)bs->stamp;
    tbl = *(struct PES_TBL *)D_001539C8;
    _sysbitGet(bs, 0x18);
    hdr->unk0 = (s64)_sysbitGet(bs, 8) << 0x20;
    hdr->unk8 = _sysbitGet(bs, 0x10);
    hdr->unk18 = -1;
    hdr->unk10 = -1;
    if (hdr->unk0 != ((u64)0xBC00 << 0x18) && hdr->unk0 != ((u64)0xBE00 << 0x18) &&
        hdr->unk0 != ((u64)0xBF00 << 0x18) && hdr->unk0 != ((u64)0xF000 << 0x18) &&
        hdr->unk0 != ((u64)0xF100 << 0x18) && hdr->unk0 != ((u64)0xFF00 << 0x18) &&
        hdr->unk0 != ((u64)0xF200 << 0x18) && hdr->unk0 != ((u64)0xF800 << 0x18)) {
        _sysbitGet(bs, 2);
        hdr->unkC = _sysbitGet(bs, 2);
        _sysbitGet(bs, 4);
        pts_dts_flags = _sysbitGet(bs, 2);
        flag1 = _sysbitGet(bs, 1);
        header_skip_table_index = _sysbitGet(bs, 4);
        pes_extension_flag = _sysbitGet(bs, 1);
        len = _sysbitGet(bs, 8);
        base = bs->stamp;
        if (pts_dts_flags & 2) {
            u32 a, b, c;
            _sysbitGet(bs, 4);
            a = _sysbitGet(bs, 3);
            _sysbitMarker(bs);
            b = _sysbitGet(bs, 0xF);
            _sysbitMarker(bs);
            c = _sysbitGet(bs, 0xF);
            _sysbitMarker(bs);
            hdr->unk10 = ((s64)((a >> 2) & 1) << 0x20) | (s64)(u32)(a << 30 | b << 15 | c);
        }
        if (pts_dts_flags == 3) {
            u32 a, b, c;
            _sysbitGet(bs, 4);
            a = _sysbitGet(bs, 3);
            _sysbitMarker(bs);
            b = _sysbitGet(bs, 0xF);
            _sysbitMarker(bs);
            c = _sysbitGet(bs, 0xF);
            _sysbitMarker(bs);
            hdr->unk18 = ((s64)((a >> 2) & 1) << 0x20) | (s64)(u32)(a << 30 | b << 15 | c);
        }
        if (flag1 == 1) {
            _sysbitGet(bs, 0x30);
        }
        if (header_skip_table_index != 0) {
            _sysbitGet(bs, tbl.v[header_skip_table_index]);
        }
        if (pes_extension_flag == 1) {
            s32 skip_128_bits_flag;
            s32 pack_header_field_flag;
            s32 a3;
            s32 a4;
            s32 pes_extension_2_flag;
            skip_128_bits_flag = _sysbitGet(bs, 1);
            pack_header_field_flag = _sysbitGet(bs, 1);
            a3 = _sysbitGet(bs, 1);
            a4 = _sysbitGet(bs, 1);
            _sysbitGet(bs, 3);
            pes_extension_2_flag = _sysbitGet(bs, 1);
            if (skip_128_bits_flag == pes_extension_flag) {
                _sysbitGet(bs, 0x30);
                _sysbitGet(bs, 0x30);
                _sysbitGet(bs, 0x20);
            }
            if (pack_header_field_flag == pes_extension_flag) {
                _Error(ctx, D_001539D8);
                return 0;
            }
            if (a3 == pes_extension_flag) {
                _sysbitGet(bs, 0x10);
            }
            if (a4 == pes_extension_flag) {
                _sysbitGet(bs, 0x10);
            }
            if (pes_extension_2_flag == pes_extension_flag) {
                u32 i;
                u32 n;
                _sysbitMarker(bs);
                n = _sysbitGet(bs, 7);
                for (i = 0; i < n; i++) {
                    _sysbitGet(bs, 8);
                }
            }
        }
        now = bs->stamp;
        {
            s32 delta = len - (s32)((now - base) >> 3);
            if (delta != 0) {
                _sysbitJump(bs, delta);
            }
        }
        {
            s32 n = hdr->unk8 - len;
            s32 m;
            hdr->unk24 = n - 3;
            hdr->unk20 = (s32)bs->stamp;
            if (hdr->unk0 == ((u64)0xBD00 << 0x18)) {
                hdr->unk0 |= (u64)(u32)_sysbitGet(bs, 0x20);
                m = n - 7;
            } else {
                m = n - 3;
            }
            if (m != 0) {
                _sysbitJump(bs, m);
            }
        }
    } else if (hdr->unk0 == ((u64)0xBC00 << 0x18) || hdr->unk0 == ((u64)0xBF00 << 0x18) ||
               hdr->unk0 == ((u64)0xF000 << 0x18) || hdr->unk0 == ((u64)0xF100 << 0x18) ||
               hdr->unk0 == ((u64)0xFF00 << 0x18) || hdr->unk0 == ((u64)0xF200 << 0x18) ||
               hdr->unk0 == ((u64)0xF800 << 0x18)) {
        s32 n = hdr->unk8;
        if (hdr->unk0 == ((u64)0xBF00 << 0x18)) {
            n -= 4;
            hdr->unk0 |= (u64)(u32)_sysbitGet(bs, 0x20);
        }
        if (n != 0) {
            _sysbitJump(bs, n);
        }
    } else if (hdr->unk0 == ((u64)0xBE00 << 0x18)) {
        if (hdr->unk8 != 0) {
            _sysbitJump(bs, hdr->unk8);
        }
    }
    return 1;
}
