#ifndef LOMBYTE_RNC_UI_MAP_H
#define LOMBYTE_RNC_UI_MAP_H

#include "types.h"

struct MenuScreen;

void draw_map_overlay(void) __asm__("FUN_00205640");
s32  draw_map_screen(struct MenuScreen *) __asm__("FUN_0021be60");
s32  draw_map_screen_layers(void) __asm__("FUN_00220850");
s32  draw_map_screen_overlay(void) __asm__("FUN_0021f330");
s32  leave_map_screen(void) __asm__("FUN_0021bda0");
void update_map_icons(s32, s32) __asm__("FUN_0020bf90");
void world_to_map_coords(f32, f32, f32 *, f32 *, s32) __asm__("FUN_00208408");

#endif /* LOMBYTE_RNC_UI_MAP_H */
