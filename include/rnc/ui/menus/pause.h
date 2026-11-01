#ifndef LOMBYTE_RNC_UI_MENUS_PAUSE_H
#define LOMBYTE_RNC_UI_MENUS_PAUSE_H

#include "types.h"

struct MenuScreen;
struct PauseState;

int assign_heli_pack_label_text_table(void *) __asm__("FUN_0021ca98");
int assign_pause_level_sprite_list(char *) __asm__("FUN_00221d28");
int build_pause_item_list(void) __asm__("FUN_002215f8");
s32 draw_pause_ring(struct PauseState *) __asm__("FUN_00220b20");
s32 draw_quit_game_menu(struct MenuScreen *) __asm__("FUN_0021e890");
int find_next_pause_page(struct MenuScreen *, int) __asm__("FUN_0021b7a8");
int pause_page_enter_noop(void) __asm__("FUN_002217f8");
int pause_stage_sprite_lists(u32 *) __asm__("FUN_0021ca60");
int set_menu_pause_flag_state(struct MenuScreen *) __asm__("FUN_0021ddd0");
int set_pause_action_list(int *) __asm__("FUN_0021a1b0");

#endif /* LOMBYTE_RNC_UI_MENUS_PAUSE_H */
