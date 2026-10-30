/* Level 02 moby class rocks. */
#include "types.h"
#include "rnc/math/fast_math.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"

extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");

extern float D_0015ED6C;

#include "rnc/overlay/quad.h"

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");

#include "qcopy.h"

extern float fast_cos(float);
extern float fast_sin(float);

/* State machine for a moby that picks a new animation each time the previous one finishes. */
/* Ported from rac1-decomp (src/overlays/l02_aridia/vendor_002A59D8.c: func_L02_002E05E0), where it is exact; names translated to the US level program. */

extern int FUN_0022da68(int, int, int);

#include "rnc/mobys/moby.h"

typedef struct {
    u32 c[3];
} Colors0720;

int               D_L02_00161CC4 = 0x5f243632;
int               D_L02_00161CC8 = 0x61311;
extern Colors0720 l02_rocks_debris_colors __asm__("D_L02_00161CD0");
/* Spark emitter; the size float is declared before the int lifetime (same registers) so the calls
 * run in retail order: colour index, size, lifetime. */
extern void
spawn_debris_chunk(void *, void *, u32, f32, s32, f32, f32, f32, s32) __asm__("FUN_L01_002f8530");

/* One-shot burst: when hit, flags D_0013D39D, sprays 200 dust puffs and 50 sparks, then removes itself. */
void rocks_update(struct Moby *moby) __asm__("FUN_L02_002e0720");

void rocks_update(struct Moby *moby) {
    enum {
        ROCKS_INITIALIZE = 0,
        ROCKS_WAIT_FOR_HIT = 1
    };
    char *hit_record;
    int   i;
    int   j;

    hit_record = find_moby_hit(moby, 0x800000, 0);
    switch (moby->state) {
    case ROCKS_INITIALIZE:
        if (menu_item_enabled_9) {
            mark_moby_for_removal(moby);
        } else {
            moby->state = 1;
        }
        break;
    case ROCKS_WAIT_FOR_HIT:
        if (hit_record) {
            menu_item_enabled_9 = 1;
            FUN_0022da68(0, 0, (int)moby);
            /* Statement-expression bodies: their temporaries share stack slots with the second loop's, as in retail. */
            for (i = 0; i < 200; i++)
                ({
                    OvlVec4 dir;
                    OvlVec4 pos;
                    OvlVec4 tmp;
                    pos.q = 0;
                    pos.f[0] = random_float_between(-1.0f, 1.0f);
                    pos.f[1] = random_float_between(-1.0f, 1.0f);
                    pos.f[2] = random_float_between(-1.0f, 1.0f);
                    dir.q = pos.q;
                    tmp.q = 0;
                    tmp.f[0] = random_float_between(-2.0f, 2.0f) * fast_cos(moby->rot.z);
                    tmp.f[1] = random_float_between(-1.0f, 1.0f) * fast_sin(moby->rot.z);
                    tmp.f[2] = random_float_between(0.0f, 4.0f);
                    pos.q = tmp.q;
                    fast_vec_add(&pos, &pos, &moby->pos);
                    fast_vec_normalize(&dir, &dir, random_float_between(1.5f, 3.5f) * D_0015ED6C);
                    spawn_effect_particle_with_target_scalar(
                        &pos,
                        &dir,
                        D_L02_00161CC4,
                        D_L02_00161CC8,
                        random_float_between(1.5f, 3.5f) * 210000.0f,
                        scale_game_frames(random_int_between(0x78, 0xF0)));
                });
            for (j = 0; j < 50; j++) {
                Colors0720 colors;
                OvlVec4    dir;
                OvlVec4    pos;
                OvlVec4    tmp;
                colors = l02_rocks_debris_colors;
                pos.q = 0;
                pos.f[0] = random_float_between(-1.0f, 1.0f);
                pos.f[1] = random_float_between(-1.0f, 1.0f);
                pos.f[2] = random_float_between(-1.0f, 1.0f);
                dir.q = pos.q;
                tmp.q = 0;
                tmp.f[0] = random_float_between(-2.0f, 2.0f) * fast_cos(moby->rot.z);
                tmp.f[1] = random_float_between(-1.0f, 1.0f) * fast_sin(moby->rot.z);
                tmp.f[2] = random_float_between(1.0f, 4.0f);
                pos.q = tmp.q;
                fast_vec_add(&pos, &pos, &moby->pos);
                fast_vec_normalize(&dir, &dir, random_float_between(1.0f, 5.0f) * D_0015ED6C);
                spawn_debris_chunk(&pos,
                                   &dir,
                                   colors.c[random_integer_below(3)],
                                   random_float_between(0.05f, 0.15f),
                                   random_int_between(0x3C, 0xB4),
                                   1.0f,
                                   1.0f,
                                   0.75f,
                                   0);
            }
            mark_moby_for_removal(moby);
        }
        break;
    }
}
