#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/rendering/buffers/setup_fs_aa_buffer/FUN_001fa978.s",
    FUN_001fa978);
#else
#include "types.h"

#include "eetypes.h"
#include "rnc/rendering/fs_aa_buffer.h"

extern struct FsAaBuf *active_fs_aa_buffer __asm__("D_0015EEB8");
extern s32 display_buffer_address __asm__("D_0015EE80");
extern s32 draw_buffer_address __asm__("D_0015EE84");
extern s32 depth_buffer_address __asm__("D_0015EE88");
extern u8 fs_aa_transfer_packet[] __asm__("D_00151B60");
#define fs_aa_transfer_words ((u64 *)fs_aa_transfer_packet)
extern u8 fs_aa_resample_packet[] __asm__("D_00151DF0");
#define fs_aa_resample_words ((u64 *)fs_aa_resample_packet)
extern u8 fs_aa_draw_packet[] __asm__("D_00151900");
#define fs_aa_draw_words     ((u64 *)fs_aa_draw_packet)
extern u8 fs_aa_clear_packet[] __asm__("D_00152040");
#define fs_aa_clear_words    ((u64 *)fs_aa_clear_packet)
extern u8 first_clear_packet[] __asm__("D_0013CC90");
#define first_clear_words    ((u64 *)first_clear_packet)
extern u8 second_clear_packet[] __asm__("D_0013CDD0");
#define second_clear_words   ((u64 *)second_clear_packet)

extern void sceGsSetDefDispEnv(void *, s16, s16, s16, s16, s16);
extern s32 sceGsSetDefDrawEnv(struct sceGsDrawEnv1 *, s16, s16, s16, s16, s16);

void setup_fs_aa_buffer(s32 display_width, s32 display_height, s32 storage_width,
                        s32 storage_height, s32 display_offset_x,
                        s32 display_offset_y) __asm__("FUN_001fa978");

void setup_fs_aa_buffer(s32 display_width, s32 display_height, s32 storage_width,
                        s32 storage_height, s32 display_offset_x, s32 display_offset_y) {
    volatile u64 *packet_word;
    s32 strip_index;
    s32 left_x, next_x, right_x;

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
    fs_aa_buffer.reserved164 = 0;
    fs_aa_buffer.zpsm = 0x31;
    fs_aa_buffer.fbp1 = display_buffer_address >> 13;
    fs_aa_buffer.fbp0 = draw_buffer_address >> 13;
    fs_aa_buffer.zbp = depth_buffer_address >> 13;
    sceGsSetDefDispEnv(&fs_aa_buffer, 0, storage_width, storage_height, display_offset_x,
                       display_offset_y);
    ((struct GsDispfb *)&active_fs_aa_buffer->disp.dispfb)->FBP = fs_aa_buffer.fbp1;
    sceGsSetDefDrawEnv(&active_fs_aa_buffer->draw0, fs_aa_buffer.psm, fs_aa_buffer.display_width,
                       fs_aa_buffer.display_height, 3, fs_aa_buffer.zpsm);
    active_fs_aa_buffer->draw0.frame1.FBP = fs_aa_buffer.fbp0;
    *(volatile u64 *)&active_fs_aa_buffer->draw0.zbuf1 =
        (u64)fs_aa_buffer.zbp | ((u64)(fs_aa_buffer.zpsm & 0xF) << 24);
    *(volatile u128 *)&active_fs_aa_buffer->giftag0 = 0;
    active_fs_aa_buffer->giftag0.NLOOP = 8;
    active_fs_aa_buffer->giftag0.EOP = 1;
    active_fs_aa_buffer->giftag0.NREG = 1;
    active_fs_aa_buffer->giftag0.REGS0 = 0xE;
    sceGsSetDefDrawEnv(&active_fs_aa_buffer->draw1, fs_aa_buffer.storage_psm,
                       fs_aa_buffer.storage_width, fs_aa_buffer.storage_height, 0, 0);
    active_fs_aa_buffer->draw1.zbuf1 = (u64)1 << 32;
    active_fs_aa_buffer->draw1.frame1.FBP = fs_aa_buffer.fbp1;
    *(volatile u128 *)&active_fs_aa_buffer->giftag1 = 0;
    active_fs_aa_buffer->giftag1.NLOOP = 8;
    active_fs_aa_buffer->giftag1.EOP = 1;
    active_fs_aa_buffer->giftag1.NREG = 1;
    packet_word = &fs_aa_transfer_words[12];
    active_fs_aa_buffer->giftag1.REGS0 = 0xE;

    fs_aa_transfer_words[0] = 0x408B400000000001;
    fs_aa_transfer_words[1] = 0xEEEE;
    fs_aa_transfer_words[2] = 0x30000;
    fs_aa_transfer_words[3] = 0x47;
    fs_aa_transfer_words[4] = 5;
    fs_aa_transfer_words[5] = 8;
    fs_aa_transfer_words[6] = 0x100000261;
    fs_aa_transfer_words[7] = 0x14;
    fs_aa_transfer_words[8] = ((u64)active_fs_aa_buffer->fbp0 << 5) |
                              ((u64)((active_fs_aa_buffer->display_width >> 6) & 0x3F) << 14) |
                              ((u64)active_fs_aa_buffer->psm << 20) | 0xEA8000000;
    fs_aa_transfer_words[9] = 6;
    fs_aa_transfer_words[10] = 0x4400000000008010;
    fs_aa_transfer_words[11] = 0x5353;
    strip_index = 0;
    do {
        *packet_word++ = strip_index * active_fs_aa_buffer->display_width;
        *packet_word++ = (strip_index * active_fs_aa_buffer->storage_width +
                          (0x8000 - (active_fs_aa_buffer->storage_width << 3))) |
                         ((u64)(0x7FF8 - (active_fs_aa_buffer->storage_height << 3)) << 16);
        *packet_word++ = (strip_index + 1) * active_fs_aa_buffer->display_width |
                         ((u64)active_fs_aa_buffer->display_height << 20);
        *packet_word++ = ((strip_index + 1) * active_fs_aa_buffer->storage_width +
                          (0x8000 - (active_fs_aa_buffer->storage_width << 3))) |
                         ((u64)((active_fs_aa_buffer->storage_height << 3) + 0x7FF8) << 16);
        strip_index++;
    } while (strip_index < 16);
    packet_word[0] = 0x4400000000008001;
    fs_aa_transfer_words[77] = 0x4410;
    packet_word[2] = 0x181;
    packet_word[3] = 0x80000000;
    fs_aa_transfer_words[80] =
        0x6FF8 | ((u64)(0x7FF8 - (active_fs_aa_buffer->storage_height << 3)) << 16);
    packet_word[5] = 0x6FF8 | ((u64)((active_fs_aa_buffer->storage_height << 3) + 0x7FF8) << 16);

    fs_aa_resample_words[0] = 0x308B400000000001;
    fs_aa_resample_words[1] = 0xEEE;
    fs_aa_resample_words[2] = 0x30000;
    fs_aa_resample_words[3] = 0x47;
    fs_aa_resample_words[4] = 0x100000261;
    fs_aa_resample_words[5] = 0x14;
    fs_aa_resample_words[6] = ((u64)active_fs_aa_buffer->fbp0 << 5) |
                              ((u64)((active_fs_aa_buffer->display_width >> 6) & 0x3F) << 14) |
                              ((u64)active_fs_aa_buffer->psm << 20) | 0xEA8000000;
    fs_aa_resample_words[7] = 6;
    fs_aa_resample_words[8] = 0x4400000000008010;
    fs_aa_resample_words[9] = 0x5353;
    packet_word = &fs_aa_resample_words[10];
    left_x = 0x6FF8;
    next_x = 0x71F8;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ = strip_index * active_fs_aa_buffer->display_width;
        *packet_word++ =
            left_x | ((u64)(0x7FF8 - (active_fs_aa_buffer->storage_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ = (strip_index + 1) * active_fs_aa_buffer->display_width | 0x1A000000;
        *packet_word++ =
            next_x | ((u64)((active_fs_aa_buffer->storage_height << 3) + 0x7FF8) << 16);
        next_x += 0x200;
    }

    fs_aa_draw_words[0] = 0x408B400000000001;
    fs_aa_draw_words[1] = 0xEEEE;
    fs_aa_draw_words[2] = 0x30000;
    fs_aa_draw_words[3] = 0x47;
    fs_aa_draw_words[4] = 5;
    fs_aa_draw_words[5] = 8;
    fs_aa_draw_words[6] = 0x100000261;
    fs_aa_draw_words[7] = 0x14;
    fs_aa_draw_words[8] = ((u64)active_fs_aa_buffer->fbp0 << 5) |
                          ((u64)((active_fs_aa_buffer->display_width >> 6) & 0x3F) << 14) |
                          ((u64)active_fs_aa_buffer->psm << 20) | 0xEA8000000;
    fs_aa_draw_words[9] = 6;
    fs_aa_draw_words[10] = 0x4400000000008010;
    fs_aa_draw_words[11] = 0x5353;
    packet_word = &fs_aa_draw_words[12];
    left_x = 0x7000;
    next_x = 0x200;
    right_x = 0x7200;
    strip_index = 0;
    do {
        *packet_word++ = strip_index * 0x200;
        strip_index++;
        *packet_word++ =
            left_x | ((u64)(0x8000 - (active_fs_aa_buffer->display_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ = next_x | ((u64)active_fs_aa_buffer->display_height << 20);
        next_x += 0x200;
        *packet_word++ =
            right_x | ((u64)((active_fs_aa_buffer->display_height << 3) + 0x7FF0) << 16);
        right_x += 0x200;
    } while (strip_index < 16);

    fs_aa_clear_words[0] = 0x1000000000000001;
    fs_aa_clear_words[1] = 0xE;
    fs_aa_clear_words[2] = 0x30000;
    fs_aa_clear_words[3] = 0x47;
    fs_aa_clear_words[4] = 0x2400000000008001;
    fs_aa_clear_words[5] = 0x10;
    fs_aa_clear_words[6] = 0x106;
    fs_aa_clear_words[7] = 0x80008000;
    fs_aa_clear_words[8] = 0x2400000000008010;
    fs_aa_clear_words[9] = 0x44;
    packet_word = &fs_aa_clear_words[10];
    left_x = 0x6FF8;
    next_x = 0x71F8;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ =
            left_x | ((u64)(0x7FF8 - (active_fs_aa_buffer->display_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ =
            next_x | ((u64)((active_fs_aa_buffer->display_height << 3) + 0x7FF8) << 16);
        next_x += 0x200;
    }

    packet_word = &first_clear_words[8];
    left_x = 0x6FF8;
    next_x = 0x71F8;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ =
            left_x | ((u64)(0x7FF8 - (active_fs_aa_buffer->display_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ =
            next_x | ((u64)((active_fs_aa_buffer->display_height << 3) + 0x7FF8) << 16);
        next_x += 0x200;
    }

    packet_word = &second_clear_words[8];
    left_x = 0x6FF8;
    next_x = 0x71F8;
    for (strip_index = 0; strip_index < 16; strip_index++) {
        *packet_word++ =
            left_x | ((u64)(0x7FF8 - (active_fs_aa_buffer->storage_height << 3)) << 16);
        left_x += 0x200;
        *packet_word++ =
            next_x | ((u64)((active_fs_aa_buffer->storage_height << 3) + 0x7FF8) << 16);
        next_x += 0x200;
    }
}
extern __typeof__(setup_fs_aa_buffer) func_001FA978 __attribute__((alias("FUN_001fa978")));

#endif /* NON_MATCHING */
