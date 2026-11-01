#ifndef LOMBYTE_RNC_UI_MENUS_WIDGETS_H
#define LOMBYTE_RNC_UI_MENUS_WIDGETS_H

#include "types.h"

struct MenuScreen;
struct OptMenu;
struct ThreeOptionMenu;

s32 draw_localized_three_option_menu(struct ThreeOptionMenu *) __asm__("FUN_00222a98");
s32 draw_menu_choice_list(struct OptMenu *) __asm__("FUN_00221460");
s32 draw_menu_icon_list(struct MenuScreen *) __asm__("FUN_00219fa0");
s32 draw_menu_item_grid(struct MenuScreen *) __asm__("FUN_0021d948");
s32 draw_menu_selection_text(struct MenuScreen *) __asm__("FUN_00222d98");
int draw_menu_side_indicator(struct MenuScreen *) __asm__("FUN_00221f58");
int draw_menu_text_box(struct MenuScreen *) __asm__("FUN_00222768");
s32 draw_menu_text_list(struct MenuScreen *) __asm__("FUN_0021d4a8");
int draw_prompt_box(struct MenuScreen *) __asm__("FUN_00221b50");
s32 draw_selected_menu_item_text(struct MenuScreen *) __asm__("FUN_002220f0");
int enter_menu_choice_list(void) __asm__("FUN_0021a308");
s32 enter_menu_help_text(struct MenuScreen *) __asm__("FUN_0021d1f8");
s32 enter_menu_prompt(struct MenuScreen *) __asm__("FUN_00221a48");
int leave_menu_choice_list(void) __asm__("FUN_0021a310");
s32 leave_menu_prompt(struct MenuScreen *) __asm__("FUN_00221a88");
int render_configured_text_label(struct MenuScreen *) __asm__("FUN_0021a328");
s32 restore_menu_help_text(struct MenuScreen *) __asm__("FUN_0021d2c8");
s32 update_menu_choice_list(struct MenuScreen *) __asm__("FUN_002212b8");
int update_menu_help_text_load(struct MenuScreen *) __asm__("FUN_0021d338");

#endif /* LOMBYTE_RNC_UI_MENUS_WIDGETS_H */
