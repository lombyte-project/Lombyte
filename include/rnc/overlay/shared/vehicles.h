#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_H

#include "types.h"

struct Moby;

void background_ship_1_update(char *) __asm__("FUN_L02_002ec4b8");
void background_ship_2_update(char *) __asm__("FUN_L02_002ece18");
void fighter_jet_missile_update(struct Moby *) __asm__("FUN_L11_003089d0");
void fighter_jet_rocket_update() __asm__("FUN_L13_002e6c08");
void giant_clank_charged_blast_update() __asm__("FUN_L15_0029edb0");
void giant_clank_hit_sphere_update(struct Moby *) __asm__("FUN_L15_0029ead0");
void giant_clank_missile_update() __asm__("FUN_L15_0029d6c0");
void giant_clank_pad_update(struct Moby *) __asm__("FUN_L15_002ed068");
void hoverboard_update(unsigned char *) __asm__("FUN_L05_002f87a8");
void ship_rocket_update(struct Moby *) __asm__("FUN_L11_003094f8");
void veldin_ship_update() __asm__("FUN_L00_002d1e80");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_H */
