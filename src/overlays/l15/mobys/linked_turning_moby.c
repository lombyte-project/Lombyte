/* Level 15 moby class linked_turning_moby (0x3a9): turns while its linked moby is in state 2 or 4, the way that moby's pvars word 0x10 says. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

extern short linked_state_yaw_rate_degrees_gp __asm__("D_L15_00161EC0") __attribute__((sda));

void linked_turning_moby_update(struct Moby *moby) __asm__("FUN_L15_002e46b0");

/* Moby class 937 */
void linked_turning_moby_update(struct Moby *moby) {
    int linked_moby_index = *(int *)moby->pvars;
    if (linked_moby_index != -1) {
        char *linked_moby = (char *)(linked_moby_index * 256 + (int)l15_moby_array);
        char *linked_moby_vars = *(char **)(linked_moby + 0x78);
        int   linked_moby_state = (unsigned char)linked_moby[0x20];
        float step;
        if (linked_moby_state == 2) {
            step = *(float *)&linked_state_yaw_rate_degrees_gp * DEG_TO_RAD * frame_time;
            if (*(int *)(linked_moby_vars + 0x10) != 0)
                step = -step;
        } else if (linked_moby_state == 4) {
            step = *(float *)&linked_state_yaw_rate_degrees_gp * DEG_TO_RAD * frame_time;
            if (*(int *)(linked_moby_vars + 0x10) == 0)
                step = -step;
        } else {
            return;
        }
        moby->rot.z = fast_add_rotations(moby->rot.z, step);
    }
}
