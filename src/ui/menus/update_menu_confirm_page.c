/* Ported from rac1-decomp (src/overlays/shared/pause_00277208.c, func_L00_00280740). */
/* Confirm page update: pad bit 0x20 answers yes, 0x10 no; both go back. */
#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#define NOT_SDA __attribute__((section(".data")))
extern unsigned char D_0013A4E0[] NOT_SDA;
int update_menu_confirm_page(void) __asm__("FUN_00221af0");

int update_menu_confirm_page(void) {
    int confirm_input_flags = *(int *)(D_0013A4E0 + 0x2604);
    if (confirm_input_flags & 0x20) {
        struct MenuPage *page;
        menu_system.unkD4 = 0;
        page = menu_system.current;
        menu_system.next = page->back;
        page->confirmed = 1;
    } else if (confirm_input_flags & 0x10) {
        struct MenuPage *page = menu_system.current;
        menu_system.next = page->back;
        page->confirmed = 0;
    }
    return 0;
}

extern __typeof__(update_menu_confirm_page) func_00221AF0 __attribute__((alias("FUN_00221af0")));
