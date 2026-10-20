#include "types.h"
#include "asm.h"
#include "rnc/globals.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/rendering/buffers/setup_fs_aa_buffer/FUN_001fa978.s",
    FUN_001fa978);
#else
#include "sda.h"
#include "eetypes.h"
#include "rnc/rendering/fs_aa_buffer.h"
#include "rnc/rendering/fs_aa_packets.h"

extern struct FsAaBuf *active_fs_aa_buffer __asm__("D_0015EEB8") MACRO_ADDR;
extern s32 display_buffer_address_macro __asm__("D_0015EE80") MACRO_ADDR;
extern s32 draw_buffer_address_macro __asm__("D_0015EE84") MACRO_ADDR;
extern s32 depth_buffer_address_macro __asm__("D_0015EE88") MACRO_ADDR;

extern void sceGsSetDefDispEnv(void *, s16, s16, s16, s16, s16);
extern s32 sceGsSetDefDrawEnv(void *, s16, s16, s16, s16, s16);

#define FA ((struct FsAaBuf *)active_fs_aa_buffer)

void setup_fs_aa_buffer(s32 display_width, s32 display_height, s32 storage_width,
                        s32 storage_height, s32 display_offset_x,
                        s32 display_offset_y) __asm__("FUN_001fa978");

void setup_fs_aa_buffer(s32 display_width, s32 display_height, s32 storage_width,
                        s32 storage_height, s32 display_offset_x, s32 display_offset_y) {
    u8 *q;

    active_fs_aa_buffer = &fs_aa_buffer;
    fs_aa_buffer.display_width = display_width;
    fs_aa_buffer.display_height = display_height;
    fs_aa_buffer.storage_width = storage_width;
    fs_aa_buffer.storage_height = storage_height;
    fs_aa_buffer.display_offset_x = display_offset_x;
    fs_aa_buffer.display_offset_y = display_offset_y;
    fs_aa_buffer.reserved170 = 0;
    fs_aa_buffer.storage_psm = 0;
    fs_aa_buffer.psm = 0;
    fs_aa_buffer.target_psm = 0;
    fs_aa_buffer.zpsm = 0x31;
    fs_aa_buffer.fbp1 = display_buffer_address_macro >> 13;
    fs_aa_buffer.fbp0 = draw_buffer_address_macro >> 13;
    fs_aa_buffer.zbp = depth_buffer_address_macro >> 13;
    sceGsSetDefDispEnv(&fs_aa_buffer, 0, storage_width, storage_height, display_offset_x,
                       display_offset_y);
    ((struct GsDispfb *)&FA->disp.dispfb)->FBP = fs_aa_buffer.fbp1;
    sceGsSetDefDrawEnv(&FA->draw0, fs_aa_buffer.psm, fs_aa_buffer.display_width,
                       fs_aa_buffer.display_height, 3, fs_aa_buffer.zpsm);
    FA->draw0.frame1.FBP = fs_aa_buffer.fbp0;
    *(s64 *)&FA->draw0.zbuf1 = (s64)fs_aa_buffer.zbp | ((u64)(fs_aa_buffer.zpsm & 0xF) << 24);
    *(u128 *)&FA->giftag0 = 0;
    FA->giftag0.NLOOP = 8;
    FA->giftag0.EOP = 1;
    FA->giftag0.NREG = 1;
    FA->giftag0.REGS0 = 0xE;
    sceGsSetDefDrawEnv(&FA->draw1, fs_aa_buffer.storage_psm, fs_aa_buffer.storage_width,
                       fs_aa_buffer.storage_height, 0, 0);
    FA->draw1.frame1.FBP = fs_aa_buffer.fbp1;
    *(s64 *)&FA->draw1.zbuf1 = (s64)1 << 32;
    *(u128 *)&FA->giftag1 = 0;
    FA->giftag1.NLOOP = 8;
    FA->giftag1.EOP = 1;
    FA->giftag1.NREG = 1;
    FA->giftag1.REGS0 = 0xE;

    q = (u8 *)fs_aa_transfer_packet;
    *(u64 *)(q + 0x0) = 0x408B400000000001UL;
    *(u64 *)(q + 0x8) = 0xEEEE;
    *(u64 *)(q + 0x10) = 0x30000;
    *(u64 *)(q + 0x18) = 0x47;
    *(u64 *)(q + 0x20) = 5;
    *(u64 *)(q + 0x28) = 8;
    *(u64 *)(q + 0x30) = 0x100000261UL;
    *(u64 *)(q + 0x38) = 0x14;
    *(u64 *)(q + 0x40) = ((s64)FA->fbp0 << 5) | ((u64)((FA->display_width >> 6) & 0x3F) << 14) |
           ((s64)FA->psm << 20) | 0xEA8000000UL;
    *(u64 *)(q + 0x48) = 6;
    *(u64 *)(q + 0x50) = 0x4400000000008010UL;
    *(u64 *)(q + 0x58) = 0x5353;
    {
        volatile u64 *p;
        s32 i;
        for (i = 0, p = (u64 *)q + 12; i < 16; i++) {
            *p++ = i * FA->display_width;
            *p++ = (i * FA->storage_width + (0x8000 - FA->storage_width * 8)) |
                   ((s64)(0x7FF8 - FA->storage_height * 8) << 16);
            *p++ = (i + 1) * FA->display_width | ((s64)FA->display_height << 20);
            *p++ = ((i + 1) * FA->storage_width + (0x8000 - FA->storage_width * 8)) |
                   ((s64)(FA->storage_height * 8 + 0x7FF8) << 16);
        }
    }
    *(u64 *)(q + 0x260) = 0x4400000000008001UL;
    *(u64 *)(q + 0x268) = 0x4410;
    *(u64 *)(q + 0x270) = 0x181;
    *(u64 *)(q + 0x278) = 0x80000000UL;
    *(u64 *)(q + 0x280) = 0x6FF8 | ((s64)(0x7FF8 - FA->storage_height * 8) << 16);
    *(u64 *)(q + 0x288) = 0x6FF8 | ((s64)(FA->storage_height * 8 + 0x7FF8) << 16);

    q = (u8 *)fs_aa_resample_packet;
    *(u64 *)(q + 0x0) = 0x308B400000000001UL;
    *(u64 *)(q + 0x8) = 0xEEE;
    *(u64 *)(q + 0x10) = 0x30000;
    *(u64 *)(q + 0x18) = 0x47;
    *(u64 *)(q + 0x20) = 0x100000261UL;
    *(u64 *)(q + 0x28) = 0x14;
    *(u64 *)(q + 0x30) = ((s64)FA->fbp0 << 5) | ((u64)((FA->display_width >> 6) & 0x3F) << 14) |
           ((s64)FA->psm << 20) | 0xEA8000000UL;
    *(u64 *)(q + 0x38) = 6;
    *(u64 *)(q + 0x40) = 0x4400000000008010UL;
    *(u64 *)(q + 0x48) = 0x5353;
    {
        volatile u64 *p = (u64 *)q + 10;
        s32 i;
        for (i = 0; i < 16; i++) {
            s32 x0 = i * 0x200 + 0x6FF8;
            s32 x1 = (i + 1) * 0x200 + 0x6FF8;

            *p++ = i * FA->display_width;
            *p++ = x0 | ((s64)(0x7FF8 - FA->storage_height * 8) << 16);
            *p++ = (i + 1) * FA->display_width | 0x1A000000;
            *p++ = x1 | ((s64)(FA->storage_height * 8 + 0x7FF8) << 16);
        }
    }

    q = (u8 *)fs_aa_draw_packet;
    *(u64 *)(q + 0x0) = 0x408B400000000001UL;
    *(u64 *)(q + 0x8) = 0xEEEE;
    *(u64 *)(q + 0x10) = 0x30000;
    *(u64 *)(q + 0x18) = 0x47;
    *(u64 *)(q + 0x20) = 5;
    *(u64 *)(q + 0x28) = 8;
    *(u64 *)(q + 0x30) = 0x100000261UL;
    *(u64 *)(q + 0x38) = 0x14;
    *(u64 *)(q + 0x40) = ((s64)FA->fbp0 << 5) | ((u64)((FA->display_width >> 6) & 0x3F) << 14) |
           ((s64)FA->psm << 20) | 0xEA8000000UL;
    *(u64 *)(q + 0x48) = 6;
    *(u64 *)(q + 0x50) = 0x4400000000008010UL;
    *(u64 *)(q + 0x58) = 0x5353;
    {
        volatile u64 *p = (u64 *)q + 12;
        s32 i;
        for (i = 0; i < 16; i++) {
            s32 x0 = i * 0x200 + 0x7000;
            s32 x1 = (i + 1) * 0x200;
            s32 x2 = (i + 1) * 0x200 + 0x7000;

            /* Fakematch: a plain store lets combine fold the sign extension into the sd. */
            *(u64 *)p++ = i * 0x200;
            *p++ = x0 | ((s64)(0x8000 - FA->display_height * 8) << 16);
            *p++ = x1 | ((s64)FA->display_height << 20);
            *p++ = x2 | ((s64)(FA->display_height * 8 + 0x7FF0) << 16);
        }
    }

    q = (u8 *)fs_aa_clear_packet;
    *(u64 *)(q + 0x0) = 0x1000000000000001UL;
    *(u64 *)(q + 0x8) = 0xE;
    *(u64 *)(q + 0x10) = 0x30000;
    *(u64 *)(q + 0x18) = 0x47;
    *(u64 *)(q + 0x20) = 0x2400000000008001UL;
    *(u64 *)(q + 0x28) = 0x10;
    *(u64 *)(q + 0x30) = 0x106;
    *(u64 *)(q + 0x38) = 0x80008000UL;
    *(u64 *)(q + 0x40) = 0x2400000000008010UL;
    *(u64 *)(q + 0x48) = 0x44;
    {
        volatile u64 *p = (u64 *)q + 10;
        s32 i;
        for (i = 0; i < 16; i++) {
            s32 x0 = i * 0x200 + 0x6FF8;
            s32 x1 = (i + 1) * 0x200 + 0x6FF8;

            *p++ = x0 | ((s64)(0x7FF8 - FA->display_height * 8) << 16);
            *p++ = x1 | ((s64)(FA->display_height * 8 + 0x7FF8) << 16);
        }
    }

    q = (u8 *)first_clear_packet;
    {
        volatile u64 *p = (u64 *)q + 8;
        s32 i;
        for (i = 0; i < 16; i++) {
            s32 x0 = i * 0x200 + 0x6FF8;
            s32 x1 = (i + 1) * 0x200 + 0x6FF8;

            *p++ = x0 | ((s64)(0x7FF8 - FA->display_height * 8) << 16);
            *p++ = x1 | ((s64)(FA->display_height * 8 + 0x7FF8) << 16);
        }
    }

    q = (u8 *)second_clear_packet;
    {
        volatile u64 *p = (u64 *)q + 8;
        s32 i;
        for (i = 0; i < 16; i++) {
            s32 x0 = i * 0x200 + 0x6FF8;
            s32 x1 = (i + 1) * 0x200 + 0x6FF8;

            *p++ = x0 | ((s64)(0x7FF8 - FA->storage_height * 8) << 16);
            *p++ = x1 | ((s64)(FA->storage_height * 8 + 0x7FF8) << 16);
        }
    }
}
extern __typeof__(setup_fs_aa_buffer) func_001FA978 __attribute__((alias("FUN_001fa978")));

#endif /* NON_MATCHING */
