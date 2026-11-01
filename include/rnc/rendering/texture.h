#ifndef LOMBYTE_RNC_RENDERING_TEXTURE_H
#define LOMBYTE_RNC_RENDERING_TEXTURE_H

#include "types.h"

struct Descriptor;
struct MenuScreen;
struct ParticleHdr;

void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("FUN_001f5450");
void pack_image_descriptor(struct Descriptor *, struct MenuScreen *);
void parse_particle_textures(struct ParticleHdr *, s32, s32 *, s32) __asm__("FUN_002026c8");
void reset_gs_texture_allocation(void);

#endif /* LOMBYTE_RNC_RENDERING_TEXTURE_H */
