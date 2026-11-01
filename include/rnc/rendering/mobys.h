#ifndef LOMBYTE_RNC_RENDERING_MOBYS_H
#define LOMBYTE_RNC_RENDERING_MOBYS_H

#include "types.h"

struct MenuStreamedMobyScreen;

s32  draw_moby_entries_from_object(struct MenuStreamedMobyScreen *) __asm__("FUN_00225a68");
void patch_moby_gifs(void) __asm__("FUN_0020cef8");
void register_entity_render_resources(void) __asm__("FUN_00235898");
void render_environment_mapped_object(void) __asm__("FUN_002327a0");

#endif /* LOMBYTE_RNC_RENDERING_MOBYS_H */
