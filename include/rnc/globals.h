#ifndef LOMBYTE_RNC_GLOBALS_H
#define LOMBYTE_RNC_GLOBALS_H

#include "types.h"

/*
 * Named game globals, one declaration each. The C name is ours; the label
 * keeps the D_<addr> symbol the linker script places.
 *
 * Only the plain spelling lives here. Files that need a different placement
 * (MACRO_ADDR, NOT_SDA or sda from sda.h) keep their own local declaration,
 * because that attribute changes the code the compiler emits.
 */

/* Level the game is running (index into the level tables). */
extern s32 current_level_index __asm__("D_0015ED84");

/* Language picked from the console settings at boot (0 English, 2-5 others). */
extern s32 game_language __asm__("D_0015ED88");

extern s32 pal_mode __asm__("D_0015ED80");

/* Top-level game mode (recovered symbol GameMode). */
extern s32 game_mode __asm__("D_0015F604");

/*
 * Frame timing, set by set_video_timing for 60 Hz (50 Hz in brackets):
 * frame_time is the seconds per frame, 1/60 (1/50), with its square and cube
 * for accelerations; frame_scale is the speed factor 1.0 (1.2), with its square
 * and inverse.
 */
extern f32 frame_scale __asm__("D_0015ED60");
extern f32 frame_scale_sq __asm__("D_0015ED64");
extern f32 frame_scale_inv __asm__("D_0015ED68");
extern f32 frame_time __asm__("D_0015ED6C");
extern f32 frame_time_sq __asm__("D_0015ED70");
extern f32 frame_time_cube __asm__("D_0015ED74");

/* Bolts the player holds (the HUD prints it, vendors subtract prices). */
extern s32 current_bolt_count __asm__("D_0015ED98");

/* Pause/freeze state of the current game mode and its flags. */
extern s32 mode_freeze_state __asm__("D_0015EEB0");
extern s32 mode_freeze_flags __asm__("D_0015EEB4");

/* GS address of the depth buffer (ZBUF_1 takes it >> 13). */
extern s32 depth_buffer_address __asm__("D_0015EE88");

/* GS texture memory: next free address, and where level textures start. */
extern s32 gs_texture_allocation_cursor __asm__("D_0015EE74");
extern s32 gs_texture_allocation_start __asm__("D_0015EE78");

#endif /* LOMBYTE_RNC_GLOBALS_H */
