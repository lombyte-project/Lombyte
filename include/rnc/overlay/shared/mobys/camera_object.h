#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_H

#include "types.h"

void bolt_update(void *) __asm__("FUN_L00_002a5dd8");
void camera_object_update(s8 *) __asm__("FUN_L00_002a4e48");
void vendor_update(void *) __asm__("FUN_L00_002a57a8");
void wrench_update(void *) __asm__("FUN_L00_002a84c8");
void set_tracked_class_5f3_moby_position(void *, void *) __asm__("FUN_L00_002a9b80");
void      *
spawn_homing_bolt(void *, void *, f32, f32, s32, void *, f32, f32) __asm__("FUN_L00_002a96f8");
void emit_randomized_sparkle_pairs(void *, s32) __asm__("FUN_L00_002a7e20");
s32  begin_bolt_attraction(void *, void *, f32, f32) __asm__("FUN_L00_002a7210");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_CAMERA_OBJECT_H */
