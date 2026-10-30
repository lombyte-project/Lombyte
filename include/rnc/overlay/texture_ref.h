#ifndef LOMBYTE_RNC_OVERLAY_TEXTURE_REF_H
#define LOMBYTE_RNC_OVERLAY_TEXTURE_REF_H

#include "types.h"

/* A 16-byte texture reference of the level core header (tfrag, moby and tie
   lists): offset into the texture data and kind, whose log2dim gives the
   size bits. */
typedef struct {
    s32 offset;
    u16 kind;
    u16 pad6;
    s32 pad8[2];
} TextureRef;

/* The level loader copies the core header's moby texture references here;
   the texture bank loader overwrites entries one at a time. */
extern TextureRef moby_texture_table[] __asm__("D_L00_001AEBC0");

#endif /* LOMBYTE_RNC_OVERLAY_TEXTURE_REF_H */
