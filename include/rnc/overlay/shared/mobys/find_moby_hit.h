#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_FIND_MOBY_HIT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_FIND_MOBY_HIT_H

#include "types.h"

struct Ent_z;

struct Ent_z *find_moby_hit(void *p, s32 mask, s32 flag) __asm__("FUN_L00_0025a420");
void make_hit_descriptor(void *a, int b, int c, float d, void *q) __asm__("FUN_L00_00259888");
void spawn_nearby_effect(unsigned char *m, int x, int y, void *p, void *q, float f) __asm__("FUN_L00_00259bc8");
int play_entity_sound(void *moby,
                      void *position,
                      int   c,
                      float d,
                      float e,
                      float f,
                      int   g,
                      int   h,
                      int   i) __asm__("FUN_L00_002598b0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_FIND_MOBY_HIT_H */
