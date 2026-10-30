/* Level 01 moby class beehive. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/math/asm_helpers.h"

extern void update_entity() __asm__("FUN_L00_00250df8");

#include "qcopy.h"

typedef struct {
    u8  pad0[0x10];
    s32 w10;
    u8  pad14[0x10];
    f32 f24;
} MobyClass;

typedef struct {
    u8  pad0[0x20];
    f32 f20;
    u8  pad24[0x14C];
    s32 w170;
    u8  pad174[0xB0];
    f32 f224;
    u8  pad228[0x28];
    f32 f250;
} ClonedVars;

typedef struct {
    u8         pad0[0x10];
    u128       pos;
    u8         pad20[4];
    MobyClass *mclass;
    u8         pad28[4];
    f32        scale;
    u8         b30;
    u8         b31;
    s16        h32;
    u16        flags;
    u8         pad36[2];
    u64        d38;
    u128       rot;
    u8         pad50[0x28];
    void      *pvars;
    u8         pad7C[0x18];
    s32        w94;
} ClonedMoby;

extern void get_first_moby_in_group(ClonedMoby **, s32, s32, s32) __asm__("FUN_L00_002591d0");

typedef struct {
    s32 src_id;
    u8  pad4[4];
    s32 count;
} SpawnerVars;

typedef struct {
    u8  pad0[0x60];
    u8  path[0x18];
    s32 w78;
    s32 w7C;
    s32 w80;
    s32 w84;
    f32 f88;
    u8  pad8C[0x24];
    f32 fB0;
    f32 fB4;
    u8  padB8[0x198];
    f32 f250;
} CloneAiVars;

typedef struct {
    u8    pad0[0x10];
    f32   x;
    f32   y;
    u8    pad18[8];
    u8    state;
    u8    pad21[0x57];
    void *pvars;
} SpawnerMoby;

typedef struct {
    u8  pad0[0x80];
    f32 x;
    f32 y;
} PlayerPosView;

extern s32         count_active_moby_group_members(s32, s32) __asm__("FUN_L01_0026e008");
extern ClonedMoby *initialize_beehive_spawn_moby(ClonedMoby *) __asm__("FUN_L01_00302328");
extern void trooper_start_knockback(void *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0025c558");
s32         D_L01_00161E90 = 5;

void beehive_update(SpawnerMoby *m) __asm__("FUN_L01_003021b8");

void beehive_update(SpawnerMoby *beehive_moby) {
    enum {
        BEEHIVE_INITIALIZE = 0,
        BEEHIVE_TRY_SPAWN_CLONE = 1
    };
    SpawnerVars *beehive_vars = beehive_moby->pvars;
    SpawnerMoby *spawned_clone;
    CloneAiVars *nv;
    f32          k;

    switch (beehive_moby->state) {
    case BEEHIVE_INITIALIZE:
        beehive_moby->state = 1;
        break;
    case BEEHIVE_TRY_SPAWN_CLONE:
        if (beehive_vars->src_id < 0) {
            break;
        }
        if (count_active_moby_group_members(beehive_vars->src_id, 0xC) != 0) {
            break;
        }
        if (beehive_vars->count >= D_L01_00161E90) {
            break;
        }
        beehive_vars->count++;
        spawned_clone = (SpawnerMoby *)initialize_beehive_spawn_moby((ClonedMoby *)beehive_moby);
        if (spawned_clone != 0) {
            nv = spawned_clone->pvars;
            k = 0.8f;
            nv->w84 = 8;
            nv->w80 = truncate_float_to_s32(nv->f250 * k * 1024.0f);
            nv->f88 = nv->f250 * k;
            nv->fB0 = 7.0f;
            nv->fB4 = 13.0f;
            nv->w78 = 0;
            nv->w7C = 0;
            trooper_start_knockback(
                spawned_clone,
                nv->path,
                5,
                8,
                0,
                fast_add_rotations(fast_arc_tan(hero.motion.pos.f[0] - beehive_moby->x,
                                                hero.motion.pos.f[1] - beehive_moby->y),
                                   3.1415927f));
            spawned_clone->state = 9;
        }
        break;
    }
}

ClonedMoby *
initialize_beehive_spawn_moby(ClonedMoby *clone_spawner_moby) __asm__("FUN_L01_00302328");

ClonedMoby *initialize_beehive_spawn_moby(ClonedMoby *clone_spawner_moby) {
    ClonedMoby *destination_moby;
    ClonedVars *destination_moby_vars;

    destination_moby = 0;
    get_first_moby_in_group(&destination_moby, *(s32 *)clone_spawner_moby->pvars, 0, 0);
    destination_moby_vars = destination_moby->pvars;
    destination_moby->b30 = 0x80;
    destination_moby->h32 = 0x80;
    destination_moby->d38 = clone_spawner_moby->d38;
    destination_moby->b31 = 1;
    destination_moby->w94 = destination_moby->mclass->w10;
    destination_moby->flags |= 0x1020;
    destination_moby->scale = destination_moby->mclass->f24;
    {
        ClonedMoby *transform_destination = destination_moby;
        qcopy(&transform_destination->pos, &clone_spawner_moby->pos);
        qcopy(&transform_destination->rot, &clone_spawner_moby->rot);
    }
    destination_moby_vars->f20 = 3.0f;
    destination_moby_vars->f250 = 1.0f;
    destination_moby_vars->w170 = truncate_float_to_s32(819.2f);
    destination_moby_vars->f224 = 30.0f;
    update_entity(destination_moby);
    return destination_moby;
}
