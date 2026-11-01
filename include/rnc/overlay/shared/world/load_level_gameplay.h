#ifndef LOMBYTE_RNC_OVERLAY_SHARED_WORLD_LOAD_LEVEL_GAMEPLAY_H
#define LOMBYTE_RNC_OVERLAY_SHARED_WORLD_LOAD_LEVEL_GAMEPLAY_H

#include "types.h"

s8 *load_level_gameplay(s32) __asm__("FUN_L00_00241940");

void resolve_level_asset_block_pointers(s32 *) __asm__("FUN_L00_00241728");
void print_moby_class_sizes(int *, int) __asm__("FUN_L00_00241788");
s32  load_level_assets_and_gameplay(s32, s32) __asm__("FUN_L00_00244110");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_WORLD_LOAD_LEVEL_GAMEPLAY_H */
