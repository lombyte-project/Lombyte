#ifndef LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_H
#define LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_H

#include "types.h"

struct ItemPreviewBinding;
struct ItemPreviewLink;
struct ItemPreviewRotation;
struct MenuPreviewObjects;
struct MenuPreviewPair;
struct MenuScreen;
struct RotatingPreviewMoby;
struct RotatingPreviewOwner;

s32  advance_preview_animation_queue(void) __asm__("FUN_00226670");
s32  clear_menu_preview_pair(struct MenuPreviewPair *) __asm__("FUN_0021df58");
s32  create_rotating_preview_moby(struct RotatingPreviewOwner *) __asm__("FUN_0021ea48");
s32  draw_available_item_preview_mobys(struct MenuScreen *) __asm__("FUN_0021e608");
s32  draw_item_preview_screen(struct MenuScreen *) __asm__("FUN_0021e110");
s32  draw_menu_preview_objects(struct MenuPreviewObjects *) __asm__("FUN_0021ddf8");
int  enter_item_preview(struct MenuScreen *) __asm__("FUN_0021df30");
int  initialize_menu_preview_objects(struct MenuScreen *) __asm__("FUN_002240c8");
void load_preview_resource_bindings(s32, s32) __asm__("FUN_00226848");
s32  release_item_preview_moby(struct ItemPreviewLink *) __asm__("FUN_0021eaf0");
s32  release_menu_preview_objects(s32) __asm__("FUN_002242b8");
void rotate_item_preview_moby(struct ItemPreviewRotation *) __asm__("FUN_0021e1f8");
void rotate_preview_moby_x(struct RotatingPreviewMoby *) __asm__("FUN_0021f120");
s32  update_item_preview_binding(struct ItemPreviewBinding *) __asm__("FUN_0021e230");
int  update_item_preview_moby(struct MenuScreen *) __asm__("FUN_0021df98");

#endif /* LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_H */
