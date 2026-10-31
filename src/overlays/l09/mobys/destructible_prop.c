/* Level 09 moby class 0x117: destructible prop; loses health to hits, then bursts into sparks and leaves its wreck (class 0x11c). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "sda.h"

extern int FUN_L00_00257b90(int, int);

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EF460), where it is exact; names translated to the US level program. */
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vx;

extern char *FUN_L00_0026dd20(char *pos, char *vel, int c, int d, float f);
extern char *
spawn_destructible_prop_wreck(char *owner, void *position, float angle) __asm__("FUN_L09_002ee380");
extern void          trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
void                 destructible_prop_update(struct Moby *moby) __asm__("FUN_L09_002ee110");
typedef unsigned int u128_ee380 __attribute__((mode(TI)));

/* Ported from rac1-decomp (src/overlays/l09_gaspar/vendor_002C2B08.c: func_L09_002EF6D0), where it is exact; names translated to the US level program. */
char *
spawn_destructible_prop_wreck(char *owner, void *position, float angle) __asm__("FUN_L09_002ee380");

/* Moby class 279 */
void destructible_prop_update(struct Moby *moby) {
    char *moby_vars_or_random_frames = (char *)moby->pvars;
    char *hit_record = find_moby_hit(moby, 0x10000, 0);
    Vx    v;
    Vx    effect_velocity_or_spawn_position;
    int   i;
    if (hit_record != 0) {
        *(float *)moby_vars_or_random_frames =
            *(float *)moby_vars_or_random_frames - *(float *)(hit_record + 0x2C);
        if (0.0f < *(float *)moby_vars_or_random_frames) {
            moby_vars_or_random_frames[0x47] = 0x46;
            trooper_anim_update(moby, (short *)(moby_vars_or_random_frames + 0x40));
        } else {
            char *pos = &moby->pos;
            v.x = fast_cos(moby->rot.z) * (frame_time * 20.0f);
            v.y = fast_sin(moby->rot.z) * (frame_time * 20.0f);
            v.z = 0.0f;
            for (i = 24; i >= 0; i--) {
                union {
                    OvlQuad q;
                    Vx      v;
                } t;
                t.q = 0;
                t.v.x = random_float_between(-1.0f, 1.0f);
                t.v.y = random_float_between(-1.0f, 1.0f);
                t.v.z = random_float_between(-1.0f, 1.0f);
                *(OvlQuad *)&effect_velocity_or_spawn_position = t.q;
                fast_vec_normalize(&effect_velocity_or_spawn_position,
                                   &effect_velocity_or_spawn_position,
                                   fast_vec_length(&v) * (frame_scale * -0.35000002f + 1.0f));
                fast_vec_add(
                    &effect_velocity_or_spawn_position, &v, &effect_velocity_or_spawn_position);
                fast_vec_normalize(&effect_velocity_or_spawn_position,
                                   &effect_velocity_or_spawn_position,
                                   random_float_between(frame_time * 3.0f, frame_time * 6.0f));
                moby_vars_or_random_frames =
                    (char *)FUN_L00_00257b90(scale_game_frames(5), scale_game_frames(10));
                FUN_L00_0026dd20(pos,
                                 (char *)&effect_velocity_or_spawn_position,
                                 0x5F7F4F2F,
                                 (int)moby_vars_or_random_frames,
                                 random_float_between(20000.0f, 50000.0f));
            }
            *(OvlQuad *)&effect_velocity_or_spawn_position = *(OvlQuad *)&moby->pos;
            spawn_destructible_prop_wreck(moby, &effect_velocity_or_spawn_position, moby->rot.z);
            mark_moby_for_removal(moby);
            return;
        }
    }
    moby->hit_slot = 0xFF;
}

char *spawn_destructible_prop_wreck(char *source_moby, void *position, float angle) {
    u128_ee380 position_copy;
    void      *copy = &position_copy;
    char      *moby;
    position_copy = *(u128_ee380 *)position;
    moby = create_moby(0x11C);
    if (moby != 0) {
        unsigned char state;
        ((unsigned char *)moby)[0x30] = ((unsigned char *)source_moby)[0x30];
        state = ((unsigned char *)source_moby)[0x30];
        moby[0x31] = 1;
        MOBY(moby)->draw_dist = state;
        qcopy(moby + 0x10, copy);
        qcopy(moby + 0x40, source_moby + 0x40);
        MOBY(moby)->rot.z = angle;
        MOBY(moby)->spawn_frame = *(long *)(source_moby + 0x38);
        update_entity(moby);
    }
    return moby;
}
