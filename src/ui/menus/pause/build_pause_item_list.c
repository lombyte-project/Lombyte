/* Ported from rac1-decomp (src/game/pause.c, func_00222640). */
#include "sda.h"
#include "rnc/game/item_state.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/ui/menus/menu_pages.h"

typedef struct {
    unsigned short a;   /* +0 */
    short          b;   /* +2 */
    short          c;   /* +4 */
    short          id;  /* +6 */
    short          idx; /* +8 */
} Item0A;

extern Item0A pause_item_list_entries[] __asm__("D_001D60E0");
extern int    pause_item_variant_values[] __asm__("D_001D6178");
/* Builds the pause-menu item list from the 15 entries of pause_item_candidate_entries
   that item_available enables. The flag needs its own `f = b != 0`:
   `(b != 0) << 2` folds into a branch. */
int build_pause_item_list(void) __asm__("FUN_002215f8");

int build_pause_item_list(void) {
    int   i;
    int   item_count = 0;
    char *item_count_storage_base;

    for (i = 0; i < 15; i++) {
        int id = pause_item_candidate_entries[i].id;
        if (item_available[id] != 0) {
            Item0A       *pause_item_entry = &pause_item_list_entries[item_count];
            MenuItemInfo *rec = &menu_item_info[id];
            unsigned char b = item_text_variant[id];
            int           has_text_variant = b != 0;
            pause_item_entry->a = rec->unk38;
            pause_item_entry->b = has_text_variant << 2;
            pause_item_entry->c = 0;
            pause_item_entry->id = id;
            pause_item_entry->idx = i;
            pause_item_variant_values[item_count] = b ? rec->unk42 : rec->unk40;
            item_count++;
        }
    }
    item_count_storage_base = (char *)&weapon_help_icon_list;
    *(int *)(item_count_storage_base + 0x40) = item_count;
    return 0;
}

extern __typeof__(build_pause_item_list) func_002215F8 __attribute__((alias("FUN_002215f8")));

/* Written by build_pause_item_list: at most one entry per candidate (15). */
Item0A pause_item_list_entries[15] = {0};

int pause_item_variant_values[15] = {0};
