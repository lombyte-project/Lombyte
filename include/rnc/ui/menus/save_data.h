#ifndef LOMBYTE_RNC_UI_MENUS_SAVE_DATA_H
#define LOMBYTE_RNC_UI_MENUS_SAVE_DATA_H

#include "types.h"

struct LoadMenuDescriptor;
struct MenuScreen;

s32  draw_checking_memory_card_data_menu(struct MenuScreen *) __asm__("FUN_00220348");
s32  draw_save_slot_list(struct MenuScreen *) __asm__("FUN_002239e0");
s32  enter_save_slot_screen(struct MenuScreen *) __asm__("FUN_00222f18");
s32  leave_save_slot_screen(struct MenuScreen *) __asm__("FUN_00222f58");
s32  loading_data_menu(struct LoadMenuDescriptor *) __asm__("FUN_002232d8");
void prepare_save_game(int, int) __asm__("FUN_002269c0");
int  reset_pause_memory_card_state(void) __asm__("FUN_0021cac8");
void save_card_state_check_save(void) __asm__("FUN_00208bc0");
void save_card_state_checking_save(void) __asm__("FUN_00208c00");
void save_card_state_create_failed(void) __asm__("FUN_00208e90");
void save_card_state_create_save_pending(void) __asm__("FUN_00208d20");
void save_card_state_creating_save(void) __asm__("FUN_00208d60");
void save_card_state_format_failed(void) __asm__("FUN_00208e68");
void save_card_state_format_pending(void) __asm__("FUN_00208af8");
void save_card_state_formatted(void) __asm__("FUN_00208b88");
void save_card_state_formatting(void) __asm__("FUN_00208b28");
void save_card_state_good_save(void) __asm__("FUN_002088d0");
void save_card_state_init(void) __asm__("FUN_002088a8");
void save_card_state_load_failed(void) __asm__("FUN_00208ed8");
void save_card_state_new_card(void) __asm__("FUN_00208dd8");
void save_card_state_no_card(void) __asm__("FUN_002089a8");
void save_card_state_no_room(void) __asm__("FUN_00208eb8");
void save_card_state_no_save(void) __asm__("FUN_00208c70");
void save_card_state_prompt_begin_no_save(void) __asm__("FUN_00208fe8");
void save_card_state_prompt_begin_unformatted(void) __asm__("FUN_00208fa0");
void save_card_state_prompt_create_save(void) __asm__("FUN_00208ca8");
void save_card_state_prompt_format(void) __asm__("FUN_00208a78");
void save_card_state_save_failed(void) __asm__("FUN_00208f00");
void save_card_state_saving(void) __asm__("FUN_00208f28");
void save_card_state_unformatted(void) __asm__("FUN_00208a38");
void save_card_state_wait_for_card(void) __asm__("FUN_002089d0");
void save_card_state_warning(void) __asm__("FUN_00208980");
s32  saving_data_menu(struct MenuScreen *) __asm__("FUN_00222f88");
s32  saving_data_menu2(struct MenuScreen *) __asm__("FUN_002235b8");
void start_memory_card_save(s32, s32) __asm__("FUN_00226a70");

#endif /* LOMBYTE_RNC_UI_MENUS_SAVE_DATA_H */
