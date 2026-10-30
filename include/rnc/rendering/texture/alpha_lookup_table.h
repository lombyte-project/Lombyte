#ifndef LOMBYTE_RNC_RENDERING_TEXTURE_ALPHA_LOOKUP_TABLE_H
#define LOMBYTE_RNC_RENDERING_TEXTURE_ALPHA_LOOKUP_TABLE_H

#include "types.h"

/* 256 alpha words indexed by a byte with bits 3 and 4 swapped
   (initialize_alpha_lookup_table fills every index). */
extern s32 bit_swap_lut[256] __asm__("D_0018E740");

#endif /* LOMBYTE_RNC_RENDERING_TEXTURE_ALPHA_LOOKUP_TABLE_H */
