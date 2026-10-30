#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_HELICOPTER_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_HELICOPTER_H

#include "types.h"

void  qwark_rilgar_update(void *) __asm__("FUN_L03_002dc478");
void  update_homing_spark_projectile(void *) __asm__("FUN_L03_002d47c0");
void  attach_shared_manipulator_to_matching_class_moby(void *) __asm__("FUN_L03_002db480");
void  fred_1_update(s8 *) __asm__("FUN_L03_002dc310");
void  update_parent_following_effect(s8 *) __asm__("FUN_L03_002bae48");
void  blarg_helicopter_update(void *) __asm__("FUN_L03_002cca00");
s32   is_moby_class_23e_in_state_14(void *) __asm__("FUN_L03_002c6c60");
void  set_moby_state_14(void *) __asm__("FUN_L03_002c6c90");
void  draw_textured_grid(void *) __asm__("FUN_L03_00291918");
void *spawn_homing_spark_projectile(void *, void *, void *, s32, f32) __asm__("FUN_L03_002d5208");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_BLARG_HELICOPTER_H */
