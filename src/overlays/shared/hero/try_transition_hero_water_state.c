/* Hero: tries the water state transition; split out of hero_set_state.c so it builds with the SN compiler on its own. */
#include "sda.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/world/collision/collision_line.h"
#include "types.h"
#include "qcopy.h"
#include "rnc/hero/hero.h"
#include "rnc/math/vector.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/game/item_state.h"

extern unsigned int pad_held_lui __asm__("D_0013CAE0") NOT_SDA;
extern void
queue_animation_update(int, int, void *, void *, void *, void *, int) __asm__("FUN_001ff308");
extern void hud_init_particles(void) __asm__("FUN_L00_00236268");
extern void hud_update_particles(void) __asm__("FUN_L00_00236458");
extern void draw_hud_stars(void) __asm__("FUN_L00_002365a8");
/* The same function under a void-returning prototype, as an older
   declaration of it would have read; see the 0x33 call below. */
extern void hero_set_state_v(int, int) __asm__("FUN_L00_002223f8");

int try_transition_hero_water_state(void) __asm__("FUN_L00_00227778");

int try_transition_hero_water_state(void) {
    Vec4 start;
    Vec4 end;
    int  decrement;
    int  state;

    if (hero.state.control_mode == 0x11) {
        if (item_available[6] != 0) {
            decrement = 0;
        } else {
            decrement = 10000 / (scale_game_frames(0x3C) * 15);
            queue_animation_update(4,
                                   0x753F,
                                   hud_init_particles,
                                   hud_update_particles,
                                   draw_hud_stars,
                                   &hero.underwater_air,
                                   10000);
        }
        hero.underwater_air -= decrement;
        if (hero.underwater_air < 0)
            hero.underwater_air = 0;
        if (hero.underwater_air == 0 && (!(hero.water_surface_z - 2.0f < hero.motion.pos.f[2]) ||
                                         !(hero.motion.rot.f[1] < -0.87266463f) ||
                                         !(frame_time < hero.motion.displacement.f[2]))) {
            state = 0x6A;
        } else if (hero.water_surface_z + 0.4f < hero.motion.pos.f[2]) {
            D_L00_00167014[0] = 0;
            state = 6;
        } else {
            goto collision_mode;
        }
    change_state:
        hero_set_state(state, 1);
        return 1;
    } else if (hero.state.current != 0x6A && hero.state.current != 0x76) {
        hero.underwater_air = 10000;
    }
collision_mode:
    if (hero.state.control_mode != 0x12)
        return 0;
    qcopy(&start, &hero.motion.pos);
    start.f[2] = hero.water_surface_z + 0.01f;
    qcopy(&end, &start);
    end.f[2] += 0.3f;
    if (collision_line(&start, &end, 2, hero.moby, 0)) {
        if (item_available[4] != 0 && (pad_held_lui & 10) != 0) {
            state = 0x35;
            goto change_state;
        }
        /* Retail keeps this as its own call site. Through the int-returning
           prototype, jump2 cross-jumping merges it into the shared
           change_state call above; the void-returning declaration keeps it
           separate. That the original source declared it this way is a
           hypothesis. */
        hero_set_state_v(0x33, 1);
        return 1;
    }
    return 0;
}
