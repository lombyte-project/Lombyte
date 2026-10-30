/* Level 07 moby classes 0x459-0x45c: breakable props; the snagglebeast's rope attack (or class 0x415) smashes them into pieces. */
#include "types.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/overlay/shared/props/rock.h"

extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float D_0015ED6C;

#include "rnc/game/item_state.h"

extern void spawn_explosion(void *,
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
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern f32  D_0015ED6C;
extern void spawn_explosion(void *,
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

/* Hit record returned by the hit lookup find_moby_hit. */
typedef struct {
    u8           pad0[0x10];
    Vec4f        pos;  /* 0x10 */
    struct Moby *moby; /* 0x20: moby that dealt the hit */
    u8           pad24[8];
    f32          kind; /* 0x2C */
} MobyHit;

extern void spawn_explosion(void *,
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

/* Pvars of a breakable crate-like prop. */
typedef struct {
    s32  shards; /* 0x00: number of pieces to spawn */
    Vec3 size;   /* 0x04: local extent of the prop */
} BreakableVars;

extern float D_0015ED6C MACRO_ADDR;

/* Breakable prop: when hit by moby 0x415 in state 8 or 0x452 in state 9 with hit kind 1.000123, scatters its shard count of pieces over its rotated extent, spawns debris and removes itself. */
void update_breakable_prop(struct Moby *moby) __asm__("FUN_L07_00319f48");

void update_breakable_prop(struct Moby *moby) {
    BreakableVars *breakable_vars = (BreakableVars *)moby->pvars;
    MobyHit       *hit;
    Vec4f          ext, shard_position, shard_velocity;
    f32            scale;
    s32            shard_class;
    s32            i;

    hit = find_moby_hit(moby, 0x30000, 0);
    if (hit != NULL && hit->moby != NULL &&
        ((hit->moby->oclass == 0x415 && hit->moby->state == 8) ||
         (hit->moby->oclass == 0x452 && hit->moby->state == 9)) &&
        hit->kind == 1.000123f) {
        ext.x = breakable_vars->size.x;
        ext.y = breakable_vars->size.y;
        ext.z = breakable_vars->size.z;
        transform_vector_by_basis(&ext, &ext, &moby->rot_row0);
        for (i = 0; i < breakable_vars->shards; i++) {
            shard_position.x = random_float_between(-ext.x, ext.x) * 0.5f;
            shard_position.y = random_float_between(-ext.y, ext.y) * 0.5f;
            shard_position.z = random_float_between(1.5f, ext.z);
            fast_vec_add(&shard_position, &shard_position, &moby->pos);
            random_vector(&shard_velocity, D_0015ED6C * 5.0f, D_0015ED6C * 10.0f);
            if (shard_velocity.z < 0.0f)
                shard_velocity.z = -shard_velocity.z;
            if (shard_velocity.z < D_0015ED6C + D_0015ED6C)
                shard_velocity.z = shard_velocity.z + (D_0015ED6C + D_0015ED6C);
            shard_class = i % 3 + 0x2B8;
            scale = random_float_between(0.41f, 0.5f);
            spawn_debris_chunk(&shard_position,
                               &shard_velocity,
                               shard_class,
                               scale,
                               random_int_between(0x32, 0x50),
                               1.0f,
                               1.0f,
                               0.75f,
                               0);
        }
        spawn_explosion(moby,
                        &hit->pos,
                        hit,
                        3.0f,
                        1.0f,
                        10,
                        3,
                        16,
                        4.0f,
                        2.0f,
                        9.0f,
                        1.0f,
                        0,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        return;
    }
    moby->hit_slot = 0xFF;
}
