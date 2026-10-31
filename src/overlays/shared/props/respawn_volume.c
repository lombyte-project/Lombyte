/* Shared moby class 0x40F: watches a volume and triggers when the hero enters it. */

#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/l06/hero/hero_set_state.h"

extern int  scale_game_frames(int);
extern void hero_respawn(void) __asm__("FUN_L00_00211250");
extern void fade_to_black(s32 n) __asm__("FUN_001f4a58");

/* Watches the moby's two 32-entry trigger lists against the hero and fires the matching events. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002D9548.c: func_L06_002F8D60), where it is exact; names translated to the US level program. */
void respawn_volume_update(char *moby) __asm__("FUN_L06_002f7930");

/* Moby class 1039 */
void respawn_volume_update(char *moby) {
    enum {
        INITIALIZE_TRIGGER_MONITOR = 0,
        CHECK_HERO_TRIGGER_VOLUMES = 1
    };
    char        *trigger_vars = ((char *)MOBY(moby)->pvars);
    struct Hero *hero_data;
    int         *list;
    int         *list2;
    char        *ship_moby;
    int          i;
    int          j;
    switch (((unsigned char *)moby)[0x20]) {
    case INITIALIZE_TRIGGER_MONITOR:
        moby[0x20] = 1;
        ((unsigned char *)moby)[0x30] = 0xFF;
        return;
    case CHECK_HERO_TRIGGER_VOLUMES:
        break;
    default:
        return;
    }
    list = (int *)trigger_vars;
    hero_data = &hero;
    if (hero_data->state.current == 0x32) {
        ship_moby = ((char *)hero_data->ship_moby);
        if (ship_moby != 0 && ((unsigned char *)ship_moby)[0x20] != 0xFE &&
            ((unsigned char *)ship_moby)[0x20] != 0xFD && hero_data->ship_oclass == 0x45) {
            return;
        }
    }
    for (i = 0; i < 32; i++) {
        if (list[i] >= 0 && is_point_inside_clip_volume(&hero.motion.pos, list[i])) {
            struct Hero *hero_ptr = &hero;
            if (hero_ptr->vehicle_mode != 0) {
                fade_to_black(scale_game_frames(10));
                hero_respawn();
            } else if (hero_ptr->state.current != 0x77) {
                hero_set_state(0x77, 1);
            }
        }
    }
    list2 = (int *)(trigger_vars + 0x80);
    for (j = 0; j < 32; j++) {
        if (list2[j] >= 0 && is_point_inside_clip_volume(&hero.motion.pos, list2[j])) {
            fade_to_black(scale_game_frames(10));
            hero_respawn();
        }
    }
}
