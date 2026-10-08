#ifndef LOMBYTE_RNC_RENDERING_FS_AA_BUFFER_H
#define LOMBYTE_RNC_RENDERING_FS_AA_BUFFER_H

#include "types.h"
#include "rnc/sdk/libgraph.h"

/* Full-screen anti-aliasing buffer at D_00151780, set up by
   setup_fs_aa_buffer: the display environment and two drawing contexts
   (each behind an A+D GIF tag), then the display and storage frame sizes.
   init_view_context and set_pal_mode size the screen from display_*. */
struct FsAaBuf {
    struct sceGsDispEnv disp;    /* 0x000 */
    u64 pad028;
    struct GifTag giftag0;       /* 0x030 */
    struct sceGsDrawEnv1 draw0;  /* 0x040 */
    struct GifTag giftag1;       /* 0x0C0 */
    struct sceGsDrawEnv1 draw1;  /* 0x0D0 */
    s16 display_width;  /* 0x150 */
    s16 display_height; /* 0x152 */
    s16 psm;            /* 0x154 */
    s16 fbp0;           /* 0x156 */
    s16 storage_width;  /* 0x158 */
    s16 storage_height; /* 0x15A */
    s16 storage_psm;    /* 0x15C */
    s16 fbp1;           /* 0x15E */
    s16 pad160[2];
    s16 reserved164; /* 0x164 */
    s16 pad166;
    s16 display_offset_x; /* 0x168 */
    s16 display_offset_y; /* 0x16A */
    s16 zpsm;             /* 0x16C */
    s16 zbp;              /* 0x16E */
    s32 reserved170;      /* 0x170 */
};

extern struct FsAaBuf fs_aa_buffer __asm__("D_00151780");

#endif /* LOMBYTE_RNC_RENDERING_FS_AA_BUFFER_H */
