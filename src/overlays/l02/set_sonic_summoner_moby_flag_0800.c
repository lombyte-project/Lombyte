/* Level 02 setup: sets flag 0x800 on every sonic_summoner moby (class 0x1B1). */
#include "types.h"
#include "rnc/globals.h"

/* Mark every sonic_summoner moby (class 0x1B1) in the active level. */
/* Ported from rac1-decomp (src/overlays/l02/initonce_00240A00.c: func_L02_00240A00), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x34];
    unsigned short flags;
    char           pad36[0x70];
    short          class_id;
} Level02Moby;

typedef struct {
    char         pad0[0x44];
    short        count;
    char         pad46[0x132];
    Level02Moby *mobys[1];
} MobyList;

extern MobyList talk_camera __asm__("D_L02_0016CE60");
void            set_sonic_summoner_moby_flag_0800(void) __asm__("FUN_L02_002400c8");

void set_sonic_summoner_moby_flag_0800(void) {
    int i;
    if (current_level_index == 2) {
        for (i = 0; i < talk_camera.count; i++) {
            Level02Moby *moby = talk_camera.mobys[i];
            if (moby->class_id == 0x1B1) {
                moby->flags |= 0x800;
            }
        }
    }
}

#include "qcopy.h"
