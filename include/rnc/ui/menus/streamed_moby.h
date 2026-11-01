#ifndef LOMBYTE_RNC_UI_MENUS_STREAMED_MOBY_H
#define LOMBYTE_RNC_UI_MENUS_STREAMED_MOBY_H

#include "types.h"

struct MenuScreen;
struct MenuStreamedMobyScreen;

int enter_streamed_moby_screen(struct MenuStreamedMobyScreen *) __asm__("FUN_00225588");
s32 leave_streamed_moby_screen(struct MenuScreen *) __asm__("FUN_00225660");
s32 update_streamed_moby_animation(struct MenuScreen *) __asm__("FUN_002256e8");

#endif /* LOMBYTE_RNC_UI_MENUS_STREAMED_MOBY_H */
