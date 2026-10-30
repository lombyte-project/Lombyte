/* Ported from rac1-decomp (src/game/pause.c, func_00222D70). */
#include "sda.h"
#include "types.h"
#include "rnc/globals.h"

#include "rnc/ui/menus/menu_pages.h"

/* `in_level_movie_lists[(unsigned)D_0015ED84 % 19]`; the older near-miss
   predated MACRO_ADDR. */
int assign_pause_level_sprite_list(char *destination) __asm__("FUN_00221d28");

int assign_pause_level_sprite_list(char *destination) {
    *(int *)(destination + 0x34) =
        (int)in_level_movie_lists[(unsigned int)current_level_index % 19];
    return 0;
}

extern __typeof__(assign_pause_level_sprite_list) func_00221D28
    __attribute__((alias("FUN_00221d28")));
