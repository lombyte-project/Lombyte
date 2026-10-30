#include "types.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"

/* One handler per memory card state; mode_freeze_state (0..0x18) picks it. */
extern void (*save_card_state_handlers[25])(void) __asm__("D_001A0438");
extern void save_card_state_init(void) __asm__("FUN_002088a8");
extern void save_card_state_good_save(void) __asm__("FUN_002088d0");
extern void save_card_state_warning(void) __asm__("FUN_00208980");
extern void save_card_state_no_card(void) __asm__("FUN_002089a8");
extern void save_card_state_wait_for_card(void) __asm__("FUN_002089d0");
extern void save_card_state_unformatted(void) __asm__("FUN_00208a38");
extern void save_card_state_prompt_format(void) __asm__("FUN_00208a78");
extern void save_card_state_format_pending(void) __asm__("FUN_00208af8");
extern void save_card_state_formatting(void) __asm__("FUN_00208b28");
extern void save_card_state_formatted(void) __asm__("FUN_00208b88");
extern void save_card_state_check_save(void) __asm__("FUN_00208bc0");
extern void save_card_state_checking_save(void) __asm__("FUN_00208c00");
extern void save_card_state_no_save(void) __asm__("FUN_00208c70");
extern void save_card_state_prompt_create_save(void) __asm__("FUN_00208ca8");
extern void save_card_state_create_save_pending(void) __asm__("FUN_00208d20");
extern void save_card_state_creating_save(void) __asm__("FUN_00208d60");
extern void save_card_state_new_card(void) __asm__("FUN_00208dd8");
extern void save_card_state_format_failed(void) __asm__("FUN_00208e68");
extern void save_card_state_create_failed(void) __asm__("FUN_00208e90");
extern void save_card_state_no_room(void) __asm__("FUN_00208eb8");
extern void save_card_state_load_failed(void) __asm__("FUN_00208ed8");
extern void save_card_state_save_failed(void) __asm__("FUN_00208f00");
extern void save_card_state_saving(void) __asm__("FUN_00208f28");
extern void save_card_state_prompt_begin_unformatted(void) __asm__("FUN_00208fa0");
extern void save_card_state_prompt_begin_no_save(void) __asm__("FUN_00208fe8");
void        run_state_handler(void) __asm__("FUN_00208840");

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
