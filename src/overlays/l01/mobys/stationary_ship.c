/* Level 01 moby class stationary_ship. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"

#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/vehicles/gunship_follow_path.h"

#include "rnc/overlay/level_path.h"
#include "rnc/overlay/level_labels.h"

#include "rnc/overlay/cuboid.h"

extern float spring_turn(float *vel, float cur, float target, float k, float d, float max) __asm__("FUN_L00_00258110");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

#include "qcopy.h"

/* pvars of the rider carrier stationary_ship_update runs */
typedef struct {
    u8   pad0[0x60];
    s32  node; /* 0x60: path node it is at */
    u8   pad64[0x10];
    s32  path; /* 0x74: index into D_L01_001B0930, -1 none */
    u8   pad78[0x58];
    Vec4 dropoff; /* 0xD0 */
    u8   padE0[0x70];
    s32  riders[8]; /* 0x150: moby indices, -1 empty */
    s32  volume;    /* 0x170: clip volume that starts it, -1 none */
    s16  state;     /* 0x174 */
    s16  timer;     /* 0x176 */
    s32  arrived;   /* 0x178 */
    s32  stop;      /* 0x17C: first marked node of the path */
    s32  camera;    /* 0x180: moby index of the camera, -1 none */
    s32  keep;      /* 0x184 */
    f32  seconds;   /* 0x188 */
    f32  turn;      /* 0x18C */
    f32  max_deg;   /* 0x190 */
    s32  unk194;    /* 0x194 */
} CarrierVars;

typedef struct {
    u8  pad0[0x10];
    f32 x;   /* 0x10 */
    f32 y;   /* 0x14 */
    f32 yaw; /* 0x18 */
    u8  pad1C[8];
    s32 node; /* 0x24: index into l01_cuboids */
    u8  pad28[4];
    s32 frames; /* 0x2C */
} CarrierCamVars;

typedef struct {
    u8   pad0[0x1D0];
    Vec4 drop; /* 0x1D0 */
    u8   pad1E0[8];
    s32  dropped;    /* 0x1E8 */
    s16  drop_timer; /* 0x1EC */
} CarrierRiderVars;

typedef struct {
    Vec4 v[8];
} CarrierOffsets;

/* struct Moby fields this function uses (the unit keeps a file-local struct Moby of its own, so
   rnc/mobys/moby.h cannot be included here). */
typedef struct {
    u8    pad0[0x10];
    Vec4f pos;   /* 0x10 */
    u8    state; /* 0x20 */
    u8    pad21[0xF];
    u8    unk30; /* 0x30 */
    u8    pad31[3];
    u16   flags; /* 0x34 */
    u8    pad36[0xA];
    Vec4f rot; /* 0x40 */
    u8    pad50[2];
    u8    seq;      /* 0x52 */
    u8    prev_seq; /* 0x53 */
    u8    pad54[0x24];
    void *pvars; /* 0x78 */
    u8    pad7C[0x2A];
    s16   oclass; /* 0xA6 */
    u8    padA8[0x14];
    u8    unkBC; /* 0xBC */
    u8    padBD[3];
    Vec4f unkC0; /* 0xC0 */
    u8    padD0[0x30];
} CarrierMoby;

extern CarrierOffsets l01_carrier_rider_local_offsets __asm__("D_L01_0020B360");
extern void           advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void           update_entity(void *) __asm__("FUN_L00_00250df8");

/* Rider carrier: waits for the hero in its clip volume, then follows its path carrying up to eight rider mobys (and steers its camera moby), drops them off one by one at the marked node and removes itself at the end of the path. */
void stationary_ship_update(CarrierMoby *m) __asm__("FUN_L01_002f4960");

void stationary_ship_update(CarrierMoby *carrier_moby) {
    enum {
        CARRIER_STATE_INITIALIZE = 0,
        CARRIER_STATE_WAIT_FOR_TRIGGER = 1,
        CARRIER_STATE_FOLLOW_PATH = 2
    };
    enum {
        CARRIER_RIDER_CLASS_TURRET = 0x1CB
    };
    CarrierVars      *v;
    CarrierCamVars   *cv;
    LevelPath        *carrier_path;
    CarrierMoby      *level_moby_array;
    CarrierRiderVars *rv;
    CarrierOffsets    offs;
    s32               i;
    s32               j;
    s32               volume_or_path_node_index;
    s32               k;
    f32               camera_target_yaw;
    f32               camera_node_yaw_delta;

    v = (CarrierVars *)carrier_moby->pvars;
    volume_or_path_node_index = v->volume;
    if (volume_or_path_node_index == -1) {
        if (carrier_moby->prev_seq != 5) {
            blend_moby_animation(carrier_moby, 5, 0, 0);
        }
        return;
    }
    switch (v->state) {
    case CARRIER_STATE_INITIALIZE:
        v->unk194 = -1;
        v->state = 1;
        v->arrived = 0;
        v->keep = 0;
        carrier_moby->unk30 = 0xFF;
        carrier_moby->flags |= 0x41;
        return;
    case CARRIER_STATE_WAIT_FOR_TRIGGER:
        if (is_point_inside_clip_volume(&hero.motion.pos, volume_or_path_node_index) != 0) {
            v->state = 2;
            carrier_moby->flags &= ~0x41;
            if (v->camera != -1) {
                cv = (CarrierCamVars *)((CarrierMoby *)ents)[v->camera].pvars;
                cv->frames = scale_game_frames(truncate_float_to_s32(v->seconds * 60.0f));
                ((CarrierMoby *)ents)[v->camera].unkBC = 1;
            }
            if (v->path != -1) {
                carrier_path = l01_paths[v->path];
                for (volume_or_path_node_index = 0; volume_or_path_node_index < carrier_path->count;
                     volume_or_path_node_index++) {
                    if (carrier_path->points[volume_or_path_node_index].f[3] == 1.0f) {
                        v->stop = volume_or_path_node_index;
                        break;
                    }
                }
            }
        }
        break;
    case CARRIER_STATE_FOLLOW_PATH:
        gunship_follow_path((char *)carrier_moby);
        if (v->camera != -1) {
            cv = (CarrierCamVars *)((CarrierMoby *)ents)[v->camera].pvars;
            camera_target_yaw = fast_arc_tan(carrier_moby->pos.x - l01_camera.pos.f[0],
                                             carrier_moby->pos.y - l01_camera.pos.f[1]);
            camera_node_yaw_delta =
                fast_subtract_rotations(l01_cuboids[cv->node].rot.z, camera_target_yaw);
            if (camera_node_yaw_delta < 0.0f) {
                camera_target_yaw = l01_cuboids[cv->node].rot.z;
            } else if (v->max_deg * 0.017453292f < camera_node_yaw_delta) {
                camera_target_yaw =
                    fast_add_rotations(l01_cuboids[cv->node].rot.z, -v->max_deg * 0.017453292f);
            }
            cv->x = l01_cuboids[cv->node].rot.x;
            cv->y = l01_cuboids[cv->node].rot.y;
            cv->yaw = spring_turn(&v->turn,
                                  l01_camera.yaw,
                                  camera_target_yaw,
                                  frame_time_sq * 12.566371f,
                                  frame_time_sq * 125.6637039f,
                                  frame_time * 3.1415927f);
        }
        if (v->path != -1) {
            carrier_path = l01_paths[v->path];
            k = v->node;
            if (k == v->stop && v->arrived == 0) {
                if (carrier_moby->prev_seq != 2) {
                    blend_moby_animation(carrier_moby, 2, 0, scale_game_frames(0x14));
                }
                v->timer = 0;
                v->arrived = 1;
            } else if (k == carrier_path->count - 2 && v->keep == 0) {
                mark_moby_for_removal(carrier_moby);
                return;
            }
        }
        break;
    }
    fast_dec_timer_s16(&v->timer);
    if (v->arrived == 0 || v->timer != 0 || carrier_moby->seq != 2) {
        for (i = 0; i < 8; i++) {
            offs = l01_carrier_rider_local_offsets;
            if (v->riders[i] != -1) {
                if (v->state != 1) {
                    ((CarrierMoby *)ents)[v->riders[i]].flags &= ~0x41;
                }
                transform_vector_by_basis(&offs.v[i], &offs.v[i], &carrier_moby->unkC0);
                fast_vec_add(
                    &((CarrierMoby *)ents)[v->riders[i]].pos, &carrier_moby->pos, &offs.v[i]);
                qcopy_nc(&((CarrierMoby *)ents)[v->riders[i]].rot, &carrier_moby->rot);
                ((CarrierMoby *)ents)[v->riders[i]].rot.z =
                    fast_add_rotations(((CarrierMoby *)ents)[v->riders[i]].rot.z, 3.1415927f);
                advance_moby_animation(&((CarrierMoby *)ents)[v->riders[i]]);
                update_entity(&((CarrierMoby *)ents)[v->riders[i]]);
                ((CarrierMoby *)ents)[v->riders[i]].flags |= 6;
            }
        }
        return;
    }
    for (j = 0; j < 8; j++) {
        if (v->riders[j] != -1) {
            level_moby_array = (CarrierMoby *)ents;
            level_moby_array[v->riders[j]].flags &= ~6;
            clear_u64_value(&level_moby_array[v->riders[j]].rot);
            ((CarrierMoby *)ents)[v->riders[j]].rot.z =
                fast_arc_tan(hero.motion.pos.f[0] - carrier_moby->pos.x,
                             hero.motion.pos.f[1] - carrier_moby->pos.y);
            switch (((CarrierMoby *)ents)[v->riders[j]].oclass) {
            case 0x154:
                ((CarrierMoby *)ents)[v->riders[j]].state = 2;
                break;
            case 0x1AB:
                ((CarrierMoby *)ents)[v->riders[j]].state = 4;
                break;
            case CARRIER_RIDER_CLASS_TURRET:
                rv = (CarrierRiderVars *)((CarrierMoby *)ents)[v->riders[j]].pvars;
                rv->dropped = 1;
                rv->drop_timer = scale_game_frames(0x1E);
                fast_vec_sub(&rv->drop, &v->dropoff, &carrier_moby->pos);
                break;
            }
            v->riders[j] = -1;
            v->timer = random_int_between(0x14, 0x1E);
            return;
        }
    }
    if (carrier_moby->prev_seq != 0) {
        blend_moby_animation(carrier_moby, 0, 0, scale_game_frames(0x14));
    }
}
