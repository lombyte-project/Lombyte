#ifndef LOMBYTE_RNC_UI_MENUS_STREAM_IMAGE_H
#define LOMBYTE_RNC_UI_MENUS_STREAM_IMAGE_H

#include "types.h"

struct MenuScreen;
struct MenuStreamImageScreen;

s32 draw_menu_stream_image(struct MenuStreamImageScreen *) __asm__("FUN_00220790");
s32 enter_menu_stream_image(struct MenuScreen *) __asm__("FUN_0021fce0");
s32 leave_menu_stream_image(struct MenuScreen *) __asm__("FUN_0021fd78");
s32 update_menu_stream_image(struct MenuScreen *) __asm__("FUN_0021f990");

#endif /* LOMBYTE_RNC_UI_MENUS_STREAM_IMAGE_H */
