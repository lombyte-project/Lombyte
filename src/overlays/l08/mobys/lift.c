/* Level 08 moby classes 0x1d3, 0x1d8: lift. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"

#include "rnc/ui/menus/menu_globals.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"

extern char *D_L08_0015FFD8_g __asm__("D_L08_0015FFD8");
extern void  approach_value(char *, float, float) __asm__("FUN_00213ed8");

void lift_update(char *moby) __asm__("FUN_L08_002ea398");

/* Moby classes 467, 472 */
void lift_update(char *moby) {
    enum {
        POSITION_BASELINE_INITIALIZE = 0,
        POSITION_FOLLOW_DRIVER = 1,
        POSITION_APPROACH_TARGET = 2
    };
    char        *position_vars = ((char *)MOBY(moby)->pvars);
    struct Moby *driver_moby;
    int          i;
    switch (MOBY(moby)->state) {
    case POSITION_BASELINE_INITIALIZE:
        *(float *)(position_vars + 4) = MOBY(moby)->pos.z;
        MOBY(moby)->state = 1;
        break;
    case POSITION_FOLLOW_DRIVER:
        i = *(int *)(position_vars + 8);
        if (i < 0)
            break;
        driver_moby = (char *)((i << 8) + (int)D_L08_0015FFD8_g);
        if (driver_moby->oclass == 0x267 || driver_moby->oclass == 0x23F) {
            if (driver_moby->state != 4)
                break;
            menu_item_enabled_41 = 1;
            allocate_voice_for_target_entry(0, 0, moby);
            MOBY(moby)->state = 2;
        } else {
            approach_value(moby + 0x18,
                           (*(float *)position_vars - *(float *)(position_vars + 4)) *
                                   *((float *)driver_moby->pvars) +
                               *(float *)(position_vars + 4),
                           frame_time * 10.0f);
        }
        break;
    case POSITION_APPROACH_TARGET:
        approach_value(moby + 0x18, *(float *)position_vars, frame_time * 10.0f);
        break;
    }
}
