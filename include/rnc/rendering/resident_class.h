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

/* Class slots of the resident class pool (0xE0 slots). The material maps of
   all slots start out as -1 (initialize_level_runtime fills them). */
extern MaterialMap resident_class_material_maps[0xE0] __asm__("D_001B6880");

/* Class table copied from the level header (0x10-byte entries), and the
   per class resource tables built beside it (0x18 classes). */
extern char resident_indexed_textures[0x1000] __asm__("D_001CAAC0");
extern s32 class_resource_ids[0x18] __asm__("D_001CBAC0");
extern s32 compressed_class_resources[0x18] __asm__("D_001CBB20");
extern char class_material_maps[0x18][0x10] __asm__("D_001CBBE0");
extern s16 class_runtime_indices[0x18][0x10] __asm__("D_001CBD60");

#endif /* LOMBYTE_RNC_RENDERING_RESIDENT_CLASS_H */
