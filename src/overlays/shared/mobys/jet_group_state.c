/* Moby groups: sets the state of a moby group. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/mobys/moby.h"

/* Ported from rac1-decomp (src/overlays/shared/mobyutil_00266060.c: func_L13_00266060), where it is exact; names translated to the US level program. */

void jet_group_state(int idx, int a, int b, int c) __asm__("FUN_L13_002650a8");
void jet_group_state(int group_index,
                     int clear_flag_bit_1,
                     int clear_flag_bit_0,

                     int copy_or_clear_field_94) {
    short *list = D_L13_001ABB40[group_index];
    if (list) {
        short *group_member_entry = list;
        do {
            int   moby_byte_offset = (*(unsigned short *)group_member_entry & 0x7FFF) << 8;
            char *moby = (char *)(moby_byte_offset + (int)moby_base);
            if (moby[0x20] >= 0) {
                if (clear_flag_bit_1 != -1) {
                    if (clear_flag_bit_1)
                        MOBY(moby)->flags &= 0xFFFD;
                    else
                        MOBY(moby)->flags |= 2;
                }
                if (clear_flag_bit_0 != -1) {
                    if (clear_flag_bit_0) {
                        moby[0x31] = 1;
                        MOBY(moby)->flags &= 0xFFFE;
                    } else {
                        moby[0x31] = 0;
                        MOBY(moby)->flags |= 1;
                    }
                }
                if (copy_or_clear_field_94 != -1) {
                    if (copy_or_clear_field_94)
                        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
                    else
                        MOBY(moby)->collision = 0;
                }
            }
        } while (*group_member_entry++ >= 0);
    }
}
