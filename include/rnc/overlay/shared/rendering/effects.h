#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_H

#include "types.h"

struct Moby;

void           create_type_31_part(int, int *, int *, float *, int) __asm__("FUN_L03_00260130");
void           create_type_49_particle(void *, int, void *, float) __asm__("FUN_L08_0027f5f0");
void           debris_update(unsigned char *) __asm__("FUN_L00_002af5b8");
void           draw_eleven_entry_scrolled_texture_batch(int) __asm__("FUN_L08_002f0c18");
void           glow_particle_update() __asm__("FUN_L00_002acad8");
void           parent_following_effect_update(char *) __asm__("FUN_L03_002bae48");
void           radial_particle_emitter_update(char *) __asm__("FUN_L08_003085f0");
void           register_part_types(void) __asm__("FUN_L00_00268900");
unsigned char *create_type_47_particle(
    void *, float *, float *, int, int, int, int, int, char *) __asm__("FUN_L15_002647f0");
void timed_sprite_effect_update(struct Moby *) __asm__("FUN_L06_0030be60");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_H */
