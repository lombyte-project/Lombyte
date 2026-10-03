#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/buffers/setup_fs_aa_buffer/FUN_001fa978.s", FUN_001fa978);
#else
#include "types.h"
#include "eetypes.h"

typedef struct {
    u64 FBP : 9;
    u64 FBW : 6;
    u64 PSM : 5;
    u64 p0 : 12;
    u64 DBX : 11;
    u64 DBY : 11;
    u64 p1 : 10;
} GsDispfb;

typedef struct {
    u64 FBP : 9;
    u64 p0 : 7;
    u64 FBW : 6;
    u64 p1 : 2;
    u64 PSM : 6;
    u64 p2 : 2;
    u64 FBMSK : 32;
} GsFrame;

typedef struct {
    u64 NLOOP : 15;
    u64 EOP : 1;
    u64 pad16 : 16;
    u64 id : 14;
    u64 PRE : 1;
    u64 PRIM : 11;
    u64 FLG : 2;
    u64 NREG : 4;
    u64 REGS0 : 4;
    u64 REGS1 : 4;
    u64 REGS2 : 4;
    u64 REGS3 : 4;
    u64 REGS4 : 4;
    u64 REGS5 : 4;
    u64 REGS6 : 4;
    u64 REGS7 : 4;
    u64 REGS8 : 4;
    u64 REGS9 : 4;
    u64 REGS10 : 4;
    u64 REGS11 : 4;
    u64 REGS12 : 4;
    u64 REGS13 : 4;
    u64 REGS14 : 4;
    u64 REGS15 : 4;
} GifTag;

typedef struct {
    GsFrame frame1;
    u64 frame1addr;
    u64 zbuf1;
    u64 zbuf1addr;
    u64 xyoffset1;
    u64 xyoffset1addr;
    u64 scissor1;
    u64 scissor1addr;
    u64 prmodecont;
    u64 prmodecontaddr;
    u64 colclamp;
    u64 colclampaddr;
    u64 dthe;
    u64 dtheaddr;
    u64 test1;
    u64 test1addr;
} GsDrawEnv1;

typedef struct {
    u64 pmode;
    u64 smode2;
    GsDispfb dispfb;
    u64 display;
    u64 bgcolor;
    u64 pad28;
} GsDispEnv;

typedef struct {
    GsDispEnv disp;      /* 0x000 */
    GifTag giftag0;      /* 0x030 */
    GsDrawEnv1 draw0;    /* 0x040 */
    GifTag giftag1;      /* 0x0C0 */
    GsDrawEnv1 draw1;    /* 0x0D0 */
    s16 display_width;               /* 0x150 */
    s16 display_height;               /* 0x152 */
    s16 psm;             /* 0x154 */
    s16 fbp0;            /* 0x156 */
    s16 storage_width;              /* 0x158 */
    s16 storage_height;              /* 0x15A */
    s16 storage_psm;            /* 0x15C */
    s16 fbp1;            /* 0x15E */
    s16 pad160[2];
    s16 reserved164;            /* 0x164 */
    s16 pad166;
    s16 display_offset_x;              /* 0x168 */
    s16 display_offset_y;              /* 0x16A */
    s16 zpsm;            /* 0x16C */
    s16 zbp;             /* 0x16E */
    s32 reserved170;            /* 0x170 */
} FsAaBuf;

extern FsAaBuf D_00151780;
extern FsAaBuf *D_0015EEB8;
extern s32 D_0015EE80;
extern s32 D_0015EE84;
extern s32 D_0015EE88;
extern u8 D_00151B60_b[] __asm__("D_00151B60");
#define D_00151B60 ((u64 *)D_00151B60_b)
extern u8 D_00151DF0_b[] __asm__("D_00151DF0");
#define D_00151DF0 ((u64 *)D_00151DF0_b)
extern u8 D_00151900_b[] __asm__("D_00151900");
#define D_00151900 ((u64 *)D_00151900_b)
extern u8 D_00152040_b[] __asm__("D_00152040");
#define D_00152040 ((u64 *)D_00152040_b)
extern u8 D_0013CC90_b[] __asm__("D_0013CC90");
#define D_0013CC90 ((u64 *)D_0013CC90_b)
extern u8 D_0013CDD0_b[] __asm__("D_0013CDD0");
#define D_0013CDD0 ((u64 *)D_0013CDD0_b)

extern void sceGsSetDefDispEnv(void *, s16, s16, s16, s16, s16);
extern s32 sceGsSetDefDrawEnv(GsDrawEnv1 *, s16, s16, s16, s16, s16);

void setup_fs_aa_buffer(s32 display_width, s32 display_height, s32 storage_width, s32 storage_height, s32 display_offset_x, s32 display_offset_y) __asm__("FUN_001fa978");

void setup_fs_aa_buffer(s32 display_width, s32 display_height, s32 storage_width, s32 storage_height, s32 display_offset_x, s32 display_offset_y) {
    u64 *packet_word;
    s32 strip_index;
    s32 left_x, next_x, right_x;

    D_0015EEB8 = &D_00151780;
    D_00151780.display_width = display_width;
    D_00151780.display_height = display_height;
    D_00151780.storage_width = storage_width;
    D_00151780.storage_height = storage_height;
    D_00151780.display_offset_x = display_offset_x;
    D_00151780.display_offset_y = display_offset_y;
    D_00151780.reserved170 = 0;
    D_00151780.storage_psm = 0;
    D_00151780.psm = 0;
    D_00151780.reserved164 = 0;
    D_00151780.fbp0 = D_0015EE84 >> 13;
    D_00151780.zbp = D_0015EE88 >> 13;
    D_00151780.fbp1 = D_0015EE80 >> 13;
    D_00151780.zpsm = 0x31;
    sceGsSetDefDispEnv(&D_00151780, 0, storage_width, storage_height, display_offset_x, display_offset_y);
    D_0015EEB8->disp.dispfb.FBP = D_00151780.fbp1;
    sceGsSetDefDrawEnv(&D_0015EEB8->draw0, D_00151780.psm, D_00151780.display_width, D_00151780.display_height, 3,
                       D_00151780.zpsm);
    D_0015EEB8->draw0.frame1.FBP = D_00151780.fbp0;
    D_0015EEB8->draw0.zbuf1 = (u64)D_00151780.zbp | ((u64)(D_00151780.zpsm & 0xF) << 24);
    *(u128 *)&D_0015EEB8->giftag0 = 0;
    D_0015EEB8->giftag0.NLOOP = 8;
    D_0015EEB8->giftag0.EOP = 1;
    D_0015EEB8->giftag0.NREG = 1;
    D_0015EEB8->giftag0.REGS0 = 0xE;
    sceGsSetDefDrawEnv(&D_0015EEB8->draw1, D_00151780.storage_psm, D_00151780.storage_width, D_00151780.storage_height, 0, 0);
    D_0015EEB8->draw1.zbuf1 = (u64)1 << 32;
    D_0015EEB8->draw1.frame1.FBP = D_00151780.fbp1;
    *(u128 *)&D_0015EEB8->giftag1 = 0;
    D_0015EEB8->giftag1.NLOOP = 8;
    D_0015EEB8->giftag1.EOP = 1;
    D_0015EEB8->giftag1.NREG = 1;
    D_0015EEB8->giftag1.REGS0 = 0xE;

    D_00151B60[0] = 0x408B400000000001;
    D_00151B60[1] = 0xEEEE;
    D_00151B60[2] = 0x30000;
    D_00151B60[3] = 0x47;
    D_00151B60[4] = 5;
    D_00151B60[5] = 8;
    D_00151B60[6] = 0x100000261;
    D_00151B60[7] = 0x14;
    D_00151B60[8] = ((u64)D_0015EEB8->fbp0 << 5) | ((u64)((D_0015EEB8->display_width >> 6) & 0x3F) << 14) |
                    ((u64)D_0015EEB8->psm << 20) | 0xEA8000000;
    D_00151B60[9] = 6;
    D_00151B60[10] = 0x4400000000008010;
    D_00151B60[11] = 0x5353;
    packet_word = &D_00151B60[12];
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ = strip_index * D_0015EEB8->display_width;
        *packet_word++ = (strip_index * D_0015EEB8->storage_width + 0x8000 - (D_0015EEB8->storage_width << 3)) |
               ((u64)(0x7FF8 - (D_0015EEB8->storage_height << 3)) << 16);
        *packet_word++ = (strip_index + 1) * D_0015EEB8->display_width | ((u64)D_0015EEB8->display_height << 20);
        *packet_word++ = ((strip_index + 1) * D_0015EEB8->storage_width + 0x8000 - (D_0015EEB8->storage_width << 3)) |
               ((u64)((D_0015EEB8->storage_height << 3) + 0x7FF8) << 16);
    }
    D_00151B60[76] = 0x4400000000008001;
    D_00151B60[77] = 0x4410;
    D_00151B60[78] = 0x181;
    D_00151B60[79] = 0x80000000;
    D_00151B60[80] = 0x6FF8 | ((u64)(0x7FF8 - (D_0015EEB8->storage_height << 3)) << 16);
    D_00151B60[81] = 0x6FF8 | ((u64)((D_0015EEB8->storage_height << 3) + 0x7FF8) << 16);

    D_00151DF0[0] = 0x308B400000000001;
    D_00151DF0[1] = 0xEEE;
    D_00151DF0[2] = 0x30000;
    D_00151DF0[3] = 0x47;
    D_00151DF0[4] = 0x100000261;
    D_00151DF0[5] = 0x14;
    D_00151DF0[6] = ((u64)D_0015EEB8->fbp0 << 5) | ((u64)((D_0015EEB8->display_width >> 6) & 0x3F) << 14) |
                    ((u64)D_0015EEB8->psm << 20) | 0xEA8000000;
    D_00151DF0[7] = 6;
    D_00151DF0[8] = 0x4400000000008010;
    D_00151DF0[9] = 0x5353;
    packet_word = &D_00151DF0[10];
    left_x = 0x6FF8;
    next_x = 0x71F8;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ = strip_index * D_0015EEB8->display_width;
        *packet_word++ = left_x | ((u64)(0x7FF8 - (D_0015EEB8->storage_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ = (strip_index + 1) * D_0015EEB8->display_width | 0x1A000000;
        *packet_word++ = next_x | ((u64)((D_0015EEB8->storage_height << 3) + 0x7FF8) << 16);
        next_x += 0x200;
    }

    D_00151900[0] = 0x408B400000000001;
    D_00151900[1] = 0xEEEE;
    D_00151900[2] = 0x30000;
    D_00151900[3] = 0x47;
    D_00151900[4] = 5;
    D_00151900[5] = 8;
    D_00151900[6] = 0x100000261;
    D_00151900[7] = 0x14;
    D_00151900[8] = ((u64)D_0015EEB8->fbp0 << 5) | ((u64)((D_0015EEB8->display_width >> 6) & 0x3F) << 14) |
                    ((u64)D_0015EEB8->psm << 20) | 0xEA8000000;
    D_00151900[9] = 6;
    D_00151900[10] = 0x4400000000008010;
    D_00151900[11] = 0x5353;
    packet_word = &D_00151900[12];
    left_x = 0x7000;
    next_x = 0x200;
    right_x = 0x7200;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ = strip_index << 9;
        *packet_word++ = left_x | ((u64)(0x8000 - (D_0015EEB8->display_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ = next_x | ((u64)D_0015EEB8->display_height << 20);
        next_x += 0x200;
        *packet_word++ = right_x | ((u64)((D_0015EEB8->display_height << 3) + 0x7FF0) << 16);
        right_x += 0x200;
    }

    D_00152040[0] = 0x1000000000000001;
    D_00152040[1] = 0xE;
    D_00152040[2] = 0x30000;
    D_00152040[3] = 0x47;
    D_00152040[4] = 0x2400000000008001;
    D_00152040[5] = 0x10;
    D_00152040[6] = 0x106;
    D_00152040[7] = 0x80008000;
    D_00152040[8] = 0x2400000000008010;
    D_00152040[9] = 0x44;
    packet_word = &D_00152040[10];
    left_x = 0x6FF8;
    next_x = 0x71F8;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ = left_x | ((u64)(0x7FF8 - (D_0015EEB8->display_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ = next_x | ((u64)((D_0015EEB8->display_height << 3) + 0x7FF8) << 16);
        next_x += 0x200;
    }

    packet_word = &D_0013CC90[8];
    left_x = 0x6FF8;
    next_x = 0x71F8;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ = left_x | ((u64)(0x7FF8 - (D_0015EEB8->display_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ = next_x | ((u64)((D_0015EEB8->display_height << 3) + 0x7FF8) << 16);
        next_x += 0x200;
    }

    packet_word = &D_0013CDD0[8];
    left_x = 0x6FF8;
    next_x = 0x71F8;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ = left_x | ((u64)(0x7FF8 - (D_0015EEB8->storage_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ = next_x | ((u64)((D_0015EEB8->storage_height << 3) + 0x7FF8) << 16);
        next_x += 0x200;
    }
}
extern __typeof__(setup_fs_aa_buffer) func_001FA978 __attribute__((alias("FUN_001fa978")));
#endif /* NON_MATCHING */
