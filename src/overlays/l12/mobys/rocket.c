/* Level 12 moby class rocket: spawning and flying the shot. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/collision.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/camera/camera_globals.h"

#include "qcopy.h"

extern float fast_cos(float);
extern float fast_sin(float);

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_002EE068), where it is exact; names translated to the US level program. */

extern char *create_moby(int);

char *spawn_rocket(char *initial_velocity,
                   char *pos,
                   int   owner,
                   int   seed,
                   float yaw,
                   float f1,
                   float f2) __asm__("FUN_L12_002ecce8");

char *spawn_rocket(char *initial_velocity,
                   char *pos,
                   int   firing_moby,
                   int   seed,
                   float yaw,
                   float nominal_speed,

                   float f2) {
    struct Moby *moby = (struct Moby *)create_moby(0x199);
    if (moby != 0) {
        char *rocket_vars;
        int   scaled_frames;
        rocket_vars = (char *)moby->pvars;
        moby->update_dist = 0xFF;
        moby->draw_dist = 0x7E;
        moby->visible = 1;
        moby->state = 1;
        qcopy(&moby->pos, pos);
        qcopy(rocket_vars, initial_velocity);
        moby->rot.z = fast_arc_tan(*(float *)rocket_vars, *(float *)(rocket_vars + 4));
        moby->rot.y = -fast_arc_tan(vector_length_xy(rocket_vars), *(float *)(rocket_vars + 8));
        scaled_frames = scale_game_frames(seed);
        *(int *)(rocket_vars + 0x10) = firing_moby;
        *(float *)(rocket_vars + 0x18) = f2;
        *(float *)(rocket_vars + 0x1C) = yaw;
        *(float *)(rocket_vars + 0x24) = nominal_speed;
        *(int *)(rocket_vars + 0x28) = 0;
        *(int *)(rocket_vars + 0x14) = scaled_frames;
        update_entity(moby);
    }
    return (char *)moby;
}

typedef struct {
    float        vel[4];
    struct Moby *owner;
    int          timer;
    float        unk18;
    float        yaw;
} ShotVars;

typedef struct {
    float dir[4];
    float unk10[2];
    char  unk18;
    char  unk19;
    short oclass;
    int   pad1C;
    int   pad20[4];
} ShotHit;

extern CollisionHit D_L12_001742C0_lui __asm__("D_L12_001742C0") NOT_SDA;
extern void         mark_moby_for_removal(struct Moby *) __asm__("FUN_0020c828");
extern void spawn_effect(void *, void *, int, int, int, int, int, int) __asm__("FUN_L00_00269958");
extern int  collision_line(void *, void *, int, struct Moby *, int) __asm__("FUN_001efa68");
extern int  coll_sphere(void *, float, int, struct Moby *) __asm__("FUN_L00_001f0d60");
extern void record_moby_hit(int, void *) __asm__("FUN_L00_00259a88");
extern void spawn_explosion(struct Moby *,
                            void *,
                            void *,
                            float,
                            float,
                            int,
                            int,
                            int,
                            float,
                            float,
                            float,
                            float,
                            int,
                            float,
                            int,
                            int,
                            int,
                            int) __asm__("FUN_L00_0025e450");

/* Level 12 shot: flies with a smoke trail, damages the first moby it hits other than its owner, then pops. */
void rocket_update(struct Moby *moby) __asm__("FUN_L12_002ece00");

/* Moby class 409 */
void rocket_update(struct Moby *moby) {
    enum {
        ROCKET_STATE_FLYING = 1,
        ROCKET_STATE_DETONATE = 2
    };
    ShotVars *rocket_state = (ShotVars *)moby->pvars;
    float     old_pos[4];
    float     side[4];
    float     back[4];
    float     trail[4];
    ShotHit   hit;
    switch (moby->state) {
    case ROCKET_STATE_FLYING: {
        char *touch = find_moby_hit(moby, 0x230000, 0);
        int   damage;
        if (touch != 0 && *(struct Moby **)(touch + 0x20) != rocket_state->owner) {
            moby->state = 2;
            break;
        }
        if (current_level_index == 12 && camera_secondary_mode[0] == 50 &&
            rocket_state->owner != 0) {
            if ((signed char)rocket_state->owner->state < 0) {
                mark_moby_for_removal(moby);
                return;
            }
            if (rocket_state->owner->state < 2) {
                moby->state = 2;
                break;
            }
        }
        qcopy(old_pos, &moby->pos);
        fast_vec_add(&moby->pos, &moby->pos, rocket_state);
        moby->rot.z = rocket_state->yaw;
        moby->rot.x = fast_add_rotations(moby->rot.x, frame_time * 6.2831855f);
        fast_vec_normalize(side, rocket_state, 0.2f);
        fast_vec_normalize(back, rocket_state, -0.2f);
        add_vectors(trail, back, &moby->pos);
        spawn_effect(back,
                     side,
                     0x6F00AFFF,
                     0xFF,
                     scale_game_frames(random_int_between(15, 22)),
                     40,
                     random_int_between(20, 35),
                     1);
        spawn_effect(back,
                     side,
                     0x1FFFFFFF,
                     0x4F4F4F,
                     scale_game_frames(random_int_between(30, 60)),
                     40,
                     random_int_between(50, 75),
                     0);
        if (collision_line(&moby->pos, old_pos, 0, moby, 0) ||
            coll_sphere(&moby->pos, 0.2f, 0, moby)) {
            if (D_L12_001742C0_lui.moby != 0 &&
                D_L12_001742C0_lui.moby != (int)rocket_state->owner) {
                damage = 1;
                if (current_level_index == 12 && camera_secondary_mode[0] == 50) {
                    damage = D_L12_00161F20 >= 16 ? 2 : 3;
                }
                make_hit_descriptor(&hit, moby, 0x10003, damage, rocket_state);
                hit.dir[0] = fast_cos(rocket_state->yaw);
                hit.dir[1] = fast_sin(rocket_state->yaw);
                hit.dir[2] = 1.0f;
                hit.dir[3] = 5627.9248f;
                hit.unk18 = 1;
                hit.unk19 = 1;
                hit.oclass = moby->oclass;
                record_moby_hit(D_L12_001742C0_lui.moby, &hit);
            }
            moby->state = 2;
        }
        if (fast_dec_timer(&rocket_state->timer)) {
            moby->state = 2;
        }
        break;
    }
    case ROCKET_STATE_DETONATE:
        fast_vec_sub(old_pos, &moby->pos, D_L12_00167240);
        fast_vec_normalize(old_pos, old_pos, 0.4f);
        fast_vec_add(&moby->pos, &moby->pos, old_pos);
        if (current_level_index == 12 && camera_secondary_mode[0] == 50) {
            char *camera = D_L12_00167240 - 0x140;
            spawn_explosion(moby,
                            &D_L12_0015F580,
                            0,
                            0.0f,
                            0.0f,
                            3,
                            3,
                            5,
                            0.3f,
                            0.2f,
                            0.0f,
                            0.15f,
                            -1,
                            0.0f,
                            0,
                            0,
                            -1,
                            0);
            *(float *)(camera + 0x160) = 0.025f;
            *(int *)(camera + 0x168) = scale_game_frames(20);
        } else {
            cart_burst(moby, &moby->pos, 0, 0.25f, 13.0f);
        }
        mark_moby_for_removal(moby);
        break;
    }
}
