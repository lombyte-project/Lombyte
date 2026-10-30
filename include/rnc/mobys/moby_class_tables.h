#ifndef LOMBYTE_RNC_MOBYS_MOBY_CLASS_TABLES_H
#define LOMBYTE_RNC_MOBYS_MOBY_CLASS_TABLES_H

#include "types.h"
#include "rnc/rendering/tex_remap.h"

/* Class data of the loaded classes, one pointer per class slot. register_moby_class
   hands out slots with a counter (D_0015FF00) and records them in three tables that
   follow each other: this one (4 bytes per slot, D_001B3200), moby_class_updates and
   resident_class_id_by_slot; the slot count is the 0xE0 their sizes agree on. The pointee type
   differs per user (class record, streamed resource, preview resource): cast at use. */
extern void *moby_class_resources[224] __asm__("D_001B3200");

/* Slot -> the class's update function (assign_moby_class_data); 0: none. */
extern s32 moby_class_updates[224] __asm__("D_001B3580");

/* Slot -> class id, the reverse of resident_class_slot_by_id. */
extern s16 resident_class_id_by_slot[224] __asm__("D_001B3900");

/* Class id -> slot in the table above; initialize_level_runtime and transition_load_wad
   fill all 0x800 bytes with 0xFF before the classes register. */
extern u8 resident_class_slot_by_id[0x800] __asm__("D_001B3AC0");

/* One per moby class a level program defines (its lvl.vtbl section; the
   executable's own list is empty): assign_moby_class_data looks a class up and
   stores its update function and method table. A -1 class ends the list and
   holds the defaults. */
struct MobyClassUpdate {
    s32    oclass;
    void  *update;
    void **methods;
};

/* Moby texture tables, filled per level and read by the moby texture DMA builder
   (FUN_00211408). That builder flags the textures each class uses in a 0x100-byte
   scratchpad table indexed by a u8 texture index, then walks the moby textures
   (D_0015FF08 of them) through both tables below: 256 entries each. */

/* Texture index -> image address in the level data, log2 width in bits 28-31
   (initialize_level_runtime / transition_load_wad). */
extern s32 moby_texture_lookup_table[256] __asm__("D_001B5980");

/* Texture index -> GS location of its uploaded image (lo/hi halves); patch_moby_gifs
   patches the class GIF packets with it. */
extern struct TexRemap moby_gs_loc_stash[256] __asm__("D_001B5D80");

/* Slot -> the word register_moby_class copies from the class header's +0x2C; one
   per class slot (0xE0). */
extern s32 D_001B6180[224];

/* Class slots whose GIF packets need patching, ended by -1 (written by the moby
   texture DMA builder, read by patch_moby_gifs); one per class slot (0xE0). */
extern s32 moby_class_rectify[224] __asm__("D_001B6500");

#endif /* LOMBYTE_RNC_MOBYS_MOBY_CLASS_TABLES_H */
