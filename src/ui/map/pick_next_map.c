/* Ported from rac1-decomp (src/game/map.c, func_00205918). */
#include "sda.h"
#include "rnc/ui/map/map_state.h"
extern unsigned char D_0013D4E1 NOT_SDA;
extern int *D_001601E0 MACRO_ADDR;
extern int find_map_entry_slot(int) __asm__("FUN_002050a0");
/* Picks the map to load next: the current map id (D_001A00F0.cur,
   +0x100 when D_0013D4E1 is set) unless FUN_002050a0 finds it already
   in a slot, otherwise the nearest entry of the 20-id list D_001601E0
   around the current one (+1, -1, +2, -2, +3, -3) that is not loaded;
   -1 if none. The two bounds tests are nested so that fold does not
   merge them into one unsigned compare, and the step update is the
   ternary that gives retail's select (slti/movn). */
int pick_next_map(void) __asm__("FUN_002050e8");

int pick_next_map(void) {
    int off = D_0013D4E1 ? 0x100 : 0;
    int key;
    int i;
    int step;

    key = D_001A00F0.cur + off;
    if (find_map_entry_slot(key) == -1) {
        return key;
    }
    i = 0;
    if (D_001A00F0.cur < 20) {
        while (D_001601E0[i] != D_001A00F0.cur) {
            i++;
        }
    }
    step = 1;
    do {
        int j = i + step;
        if (j >= 0) {
            if (j < 20 && D_001601E0[j] != 0) {
                key = D_001601E0[j] + off;
                if (find_map_entry_slot(key) == -1) {
                    return key;
                }
            }
        }
        step = (step > 0) ? -step : 1 - step;
    } while (step != 4);
    return -1;
}

extern __typeof__(pick_next_map) func_002050E8 __attribute__((alias("FUN_002050e8")));
