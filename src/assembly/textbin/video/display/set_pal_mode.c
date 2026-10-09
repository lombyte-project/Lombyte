#include "types.h"
#include "asm.h"
#include "sda.h"
#include "rnc/rendering/draw_environment.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/display/set_pal_mode/FUN_001f34e8.s",
            FUN_001f34e8);
#else
#include "types.h"
#include "rnc/rendering/fs_aa_buffer.h"
#include "rnc/rendering/image_clear_buffer.h"
#include "rnc/rendering/screen.h"
#include "rnc/rendering/draw_environment.h"

extern s32 pal_mode __asm__("D_0015ED80");
extern s32 first_image_buffer_address __asm__("D_0015EE74");
extern s32 second_image_buffer_address __asm__("D_0015EE78");
extern s32 display_buffer_address __asm__("D_0015EE80");
extern s32 draw_buffer_address __asm__("D_0015EE84");
extern s32 depth_buffer_address __asm__("D_0015EE88");
extern s32 image_buffer_address __asm__("D_0015EE8C");
extern void FillTransferWords(u8 *, s32, s32);
extern void FlushCache(s32);
extern void func_00120558(s32, s32);
extern void setup_fs_aa_buffer(s32, s32, s32, s32, s32, s32) __asm__("func_001FA978");
extern void put_disp_buffer(void) __asm__("func_001FB2A8");
extern void put_draw_buffer_large(void) __asm__("func_001FB2D0");
extern void append_gif_transfer_packet(void) __asm__("func_001FB368");
extern s32 sceGsExecLoadImage(sceGsLoadImage *, u8 *);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *, s16, s16, s16, s16, s16, s16, s16);

void set_pal_mode(void) __asm__("FUN_001f34e8");

void set_pal_mode(void) {
    sceGsLoadImage image_transfer;
    s32 tile_count;
    s32 tile_index;
    u64 zbuf;
    u64 frame;
    u64 scissor;
    s32 display_width;

    FlushCache(0);
    if (pal_mode != 0) {
        draw_buffer_address = 0x100000;
        depth_buffer_address = 0x1E0000;
        display_buffer_address = 0;
        image_buffer_address = 0x2C0000;
        setup_fs_aa_buffer(0x200, 0x1C0, 0x200, 0x200, 4, 0);
    } else {
        draw_buffer_address = 0xE0000;
        depth_buffer_address = 0x1B0000;
        display_buffer_address = 0;
        image_buffer_address = 0x280000;
        setup_fs_aa_buffer(0x200, 0x1A0, 0x200, 0x1C0, 0, 0);
    }
    /* Retail sign-extends the 16-bit dimensions before halving them. */
    display_width = fs_aa_buffer.display_width;
    screen_extent.width = display_width;
    screen_extent.half_width = display_width >> 1;
    screen_extent.half_height = fs_aa_buffer.display_height >> 1;
    screen_extent.bottom = (screen_extent.half_height + 0x800) << 4;
    screen_extent.height = fs_aa_buffer.display_height;
    screen_extent.left = (0x800 - screen_extent.half_width) << 4;
    screen_extent.right = (screen_extent.half_width + 0x800) << 4;
    screen_extent.top = (0x800 - screen_extent.half_height) << 4;
    FlushCache(0);
    func_00120558(0, 0);
    zbuf = (depth_buffer_address >> 13) | 0x1000000;
    frame = (draw_buffer_address >> 13) | ((u64)(screen_extent.width >> 6) << 16);
    scissor = ((u64)(screen_extent.width - 1) << 16) | ((u64)(screen_extent.height - 1) << 48);
    draw_environment.scissor1 = scissor;
    first_image_buffer_address = image_buffer_address;
    masked_depth_buffer_register = zbuf | ((u64)0x8000 << 17);
    second_image_buffer_address = image_buffer_address;
    draw_environment.zbuf2 = zbuf;
    draw_environment.zbuf1 = zbuf;
    draw_environment.frame2 = draw_environment.frame1 = frame;
    draw_environment.xyoffset2 = screen_extent.left | ((u64)screen_extent.top << 32);
    draw_environment.xyoffset1 = screen_extent.left | ((u64)screen_extent.top << 32);
    depth_buffer_register = zbuf;
    draw_environment.scissor2 = scissor;
    FlushCache(0);
    put_draw_buffer_large();
    append_gif_transfer_packet();
    FlushCache(0);
    func_00120558(0, 0);
    put_disp_buffer();
    FillTransferWords(image_clear_buffer, 0, 0x1000);
    tile_count = (fs_aa_buffer.storage_width * fs_aa_buffer.storage_height) >> 10;
    for (tile_index = 0; tile_index < tile_count; tile_index++) {
        sceGsSetDefLoadImage(&image_transfer, tile_index << 4, 1, 0, 0, 0, 32, 32);
        FlushCache(0);
        sceGsExecLoadImage(&image_transfer, image_clear_buffer);
        func_00120558(0, 0);
    }
}

extern __typeof__(set_pal_mode) func_001F34E8 __attribute__((alias("FUN_001f34e8")));

#endif /* NON_MATCHING */

u64 depth_buffer_register NOT_SDA = 0x31000000;

