/* Level 10 tunnel barrier (class record block_cave). */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/globals.h"

extern int D_L10_001BA950[];

/* Collectible trigger: deletes itself once taken, else watches for its toucher. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DEA10), where it is exact; names translated to the US level program. */

extern char          moby_pair_records[] __asm__("D_L10_00178580");
extern unsigned char D_0014C190[];

void tunnel_barrier_update(struct Moby *m) __asm__("FUN_L10_002dd650");

/* Moby class 1100 */
void tunnel_barrier_update(struct Moby *moby) {
    int            i;
    char          *table_entry;
    float          transposed_rotation_matrix[16];
    float          relative_position[4];
    unsigned short id = moby->save_id;
    if (l10_save_state.cleared[(short)id] != 0 ||
        (*(int *)(D_0014C190 + (((short)id >> 5) * 4 + (current_level_index << 8))) >>
         (id & 0x1F)) &
            1) {
        mark_moby_for_removal(moby);
        return;
    }
    moby->draw_dist = 0x80;
    table_entry = moby_pair_records;
    for (i = 0; i < 0x40; i++, table_entry += 0x40) {
        if (*(char **)(table_entry + 0x34) == moby) {
            if (*(int *)(table_entry + 0x24) & 0x20000) {
                char *other_moby = *(char **)(table_entry + 0x20);
                if (MOBY(other_moby)->oclass == 0xAC || MOBY(other_moby)->oclass == 0x99) {
                    fast_vec_sub(relative_position, &moby->pos, other_moby + 0x10);
                    transpose_rotation_matrix(transposed_rotation_matrix, &moby->rot_row0);
                    transform_vector(
                        relative_position, relative_position, transposed_rotation_matrix);
                    if (relative_position[0] > 0.25f) {
                        *(int *)(D_0014C190 +
                                 (((short)moby->save_id >> 5) * 4 + (current_level_index << 8))) |=
                            1 << (moby->save_id & 0x1F);
                        D_L10_001BA950[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 0x1F);
                        mark_moby_for_removal(moby);
                        return;
                    }
                    *(int *)(table_entry + 0x34) = 0;
                } else {
                    *(int *)(table_entry + 0x34) = 0;
                }
            }
        }
    }
}
