#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_ENEMY_FIGHTER_DATA_SPAWN_ENEMY_FIGHTER_EXHAUST_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_ENEMY_FIGHTER_DATA_SPAWN_ENEMY_FIGHTER_EXHAUST_H

#include "types.h"

/* Scalar data for src/overlays/l14/mobys/enemy_fighter.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern f32 enemy_fighter_exhaust_base_row0_offset_gp __asm__("D_L14_00161F30") __attribute__((sda));
extern f32 enemy_fighter_exhaust_paired_emitter_offset_gp __asm__("D_L14_00161F34")
    __attribute__((sda));
extern f32 enemy_fighter_exhaust_base_row2_offset_gp __asm__("D_L14_00161F38") __attribute__((sda));
extern f32 enemy_fighter_exhaust_ring_row0_offset_gp __asm__("D_L14_00161F3C") __attribute__((sda));
extern s32 enemy_fighter_exhaust_paired_puff_color_gp __asm__("D_L14_00161F44")
    __attribute__((sda));

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_ENEMY_FIGHTER_DATA_SPAWN_ENEMY_FIGHTER_EXHAUST_H */
