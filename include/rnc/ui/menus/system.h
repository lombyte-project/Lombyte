#ifndef LOMBYTE_RNC_UI_MENUS_SYSTEM_H
#define LOMBYTE_RNC_UI_MENUS_SYSTEM_H

#include "types.h"

struct MenuScreen;
struct Moby;
struct SceneTrigger;

int  clear_menu_selected_index(int *) __asm__("FUN_0021a318");
void close_menu_system(void) __asm__("FUN_002191b8");
void enter_main_menu(void);
int  enter_timed_menu_sequence(struct MenuScreen *) __asm__("FUN_002223d8");
s32  handle_menu_action_buttons(struct MenuScreen *) __asm__("FUN_00221968");
int  menu_draw_noop(void) __asm__("FUN_0021df28");
int  menu_leave_noop(void) __asm__("FUN_0021d1f0");
int  menu_update_noop(void) __asm__("FUN_0021bd98");
void process_menu_entry(struct Moby *, struct SceneTrigger *, s32) __asm__("FUN_00216c48");
s32  reset_menu_graphics_buffers(void) __asm__("FUN_0021cae0");
int  setup_menu_display_text(struct MenuScreen *) __asm__("FUN_0021ef78");
s32  update_menu_entry_actions(struct MenuScreen *) __asm__("FUN_0021abf8");
s32  update_menu_resource_stream(struct MenuScreen *) __asm__("FUN_0021fdc8");
s32  update_timed_menu_sequence(struct MenuScreen *) __asm__("FUN_002223f0");

#endif /* LOMBYTE_RNC_UI_MENUS_SYSTEM_H */
