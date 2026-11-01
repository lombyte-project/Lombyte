#include "types.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/save_data/save_card_state_check_save.h"
#include "rnc/ui/menus/save_data/save_card_state_checking_save.h"
#include "rnc/ui/menus/save_data/save_card_state_create_failed.h"
#include "rnc/ui/menus/save_data/save_card_state_create_save_pending.h"
#include "rnc/ui/menus/save_data/save_card_state_creating_save.h"
#include "rnc/ui/menus/save_data/save_card_state_format_failed.h"
#include "rnc/ui/menus/save_data/save_card_state_format_pending.h"
#include "rnc/ui/menus/save_data/save_card_state_formatted.h"
#include "rnc/ui/menus/save_data/save_card_state_formatting.h"
#include "rnc/ui/menus/save_data/save_card_state_good_save.h"
#include "rnc/ui/menus/save_data/save_card_state_init.h"
#include "rnc/ui/menus/save_data/save_card_state_load_failed.h"
#include "rnc/ui/menus/save_data/save_card_state_new_card.h"
#include "rnc/ui/menus/save_data/save_card_state_no_card.h"
#include "rnc/ui/menus/save_data/save_card_state_no_room.h"
#include "rnc/ui/menus/save_data/save_card_state_no_save.h"
#include "rnc/ui/menus/save_data/save_card_state_prompt_begin_no_save.h"
#include "rnc/ui/menus/save_data/save_card_state_prompt_begin_unformatted.h"
#include "rnc/ui/menus/save_data/save_card_state_prompt_create_save.h"
#include "rnc/ui/menus/save_data/save_card_state_prompt_format.h"
#include "rnc/ui/menus/save_data/save_card_state_save_failed.h"
#include "rnc/ui/menus/save_data/save_card_state_saving.h"
#include "rnc/ui/menus/save_data/save_card_state_unformatted.h"
#include "rnc/ui/menus/save_data/save_card_state_wait_for_card.h"
#include "rnc/ui/menus/save_data/save_card_state_warning.h"

/* One handler per memory card state; mode_freeze_state (0..0x18) picks it. */
extern void (*save_card_state_handlers[25])(void) __asm__("D_001A0438");
void run_state_handler(void) __asm__("FUN_00208840");

void run_state_handler(void) {
    s32 state = mode_freeze_state;

    save_card_state_handlers[state]();
    D_00161280++;
    if (mode_freeze_state != state) {
        D_00161280 = 0;
    }
}

void (*save_card_state_handlers[25])(void) __asm__("D_001A0438") = {
    save_card_state_init,
    save_card_state_good_save,
    save_card_state_warning,
    save_card_state_no_card,
    save_card_state_wait_for_card,
    save_card_state_unformatted,
    save_card_state_prompt_format,
    save_card_state_format_pending,
    save_card_state_formatting,
    save_card_state_formatted,
    save_card_state_check_save,
    save_card_state_checking_save,
    save_card_state_no_save,
    save_card_state_prompt_create_save,
    save_card_state_create_save_pending,
    save_card_state_creating_save,
    save_card_state_new_card,
    save_card_state_format_failed,
    save_card_state_create_failed,
    save_card_state_no_room,
    save_card_state_load_failed,
    save_card_state_save_failed,
    save_card_state_saving,
    save_card_state_prompt_begin_unformatted,
    save_card_state_prompt_begin_no_save,
};

extern __typeof__(run_state_handler) func_00208840 __attribute__((alias("FUN_00208840")));
