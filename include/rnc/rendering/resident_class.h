#ifndef LOMBYTE_RNC_RENDERING_RESIDENT_CLASS_H
#define LOMBYTE_RNC_RENDERING_RESIDENT_CLASS_H

#include "types.h"
#include "eetypes.h"

/* One texture of a resident render class (tfrag, shrub and object
   classes). The block offsets are relative to gs_texture_allocation_base
   (D_0015EE8C) and go into TEX0 / MIPTBP1. */
typedef struct ResidentRenderTextureDefinition {
    s32 unk0;  /* unused here; a word, so the record is 4-byte aligned */
    s16 width;
    s16 height;
    s16 draw_control_count;
    s16 clut_block_offset;  /* 0x0A: TEX0.CBP (bit 37) after adding the base */
    s16 mip_block_offset_0;
    s16 mip_block_offset_1;
} ResidentRenderTextureDefinition;

/* Material remap of one class slot: 16 material indices, also copied as
   one quadword. */
typedef union MaterialMap {
    u128 q;
    u8 b[16];
} MaterialMap;

#endif /* LOMBYTE_RNC_RENDERING_RESIDENT_CLASS_H */
