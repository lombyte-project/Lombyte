/* Level 14 bolt thief moby class and its path movement. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/level_path.h"
#include "rnc/overlay/l14/mobys/bolt_thief_globals.h"

typedef struct {
    char          pad00[0x60];
    char          effect[7]; /* 0x60: record func_L00_0025E590 updates */
    unsigned char ticks;     /* 0x67 */
    char          pad68[8];
    float         knock[4];      /* 0x70: record func_L00_0025D5B0 fills (velocity first) */
    float         knock_drag;    /* 0x80 */
    float         knock_gravity; /* 0x84 */
    float         knock_speed;   /* 0x88 */
    float         knock_rise;    /* 0x8C */
    int           unknown90;
    int           knock_flags; /* 0x94 */
    char          pad98[0x15];
    unsigned char knock_bAD; /* 0xAD */
    char          padAE[0x22];
    int           counter;    /* 0xD0 */
    float         timer;      /* 0xD4 */
    int           path;       /* 0xD8: index into D_L14_001B0BB0 */
    int           other_path; /* 0xDC */
    int           trigger;    /* 0xE0 */
    float         vx;         /* 0xE4 */
    float         vy;         /* 0xE8 */
    float         vz;         /* 0xEC */
    float         turn;       /* 0xF0 */
    char         *child;      /* 0xF4 */
    int           unknownF8;
    int           mode; /* 0xFC */
} L16CrateData;

typedef struct {
    char pad00[0x10];
    int  bounds;
} L16CrateModel;

typedef struct {
    char           pad00[0x10];
    float          position[4]; /* 0x10 */
    unsigned char  state;       /* 0x20 */
    char           pad21[3];
    L16CrateModel *model; /* 0x24 */
    char           pad28[8];
    unsigned char  collision; /* 0x30 */
    char           pad31[3];
    unsigned short flags; /* 0x34 */
    char           pad36[10];
    float          rotation[4]; /* 0x40 */
    char           pad50[2];
    unsigned char  frame;     /* 0x52 */
    unsigned char  animation; /* 0x53 */
    char           pad54[0x1C];
    unsigned char  status; /* 0x70 */
    char           pad71[7];
    L16CrateData  *data; /* 0x78 */
    char           pad7C[0x18];
    int            bounds; /* 0x94 */
    char           pad98[0xC];
    unsigned char  opacity; /* 0xA4 */
    char           padA5[0xD];
    short          id; /* 0xB2 */
} L16CrateMoby;

extern char *spawn_bolt_thief_mine(void *, void *) __asm__("FUN_L14_002ffaf8");

extern char D_L14_001FCF20[], D_L14_001FCF58[], D_L14_001FCFA0[], D_L14_001FCFD8[],
    D_L14_001FD020[];

extern int debug_print() __asm__("FUN_001e93b0");

int update_bolt_thief_path_movement(struct Moby *moby) __asm__("FUN_L14_002fea80");

extern int update_bolt_thief_path_movement(struct Moby *moby) __asm__("FUN_L14_002fea80");

float bolt_thief_path_spring_gain_gp __asm__("D_L14_00161FBC") = 0.01f;

float bolt_thief_path_spring_damping_gp __asm__("D_L14_00161FC0") = 0.2f;

extern void FUN_L00_00257470(void *, int, int);

extern void
trooper_start_knockback(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");

extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

extern void moby_set_state_one(void *) __asm__("FUN_L14_002ffc08");

extern void initialize_moby_path_state(void *) __asm__("FUN_L14_002fe900");

extern void update_linked_moby_position(void *) __asm__("FUN_L14_002fec28");

extern void emit_moby_offset_particles(void *) __asm__("FUN_L14_002feca8");

extern void FUN_L00_00258278(char *, float, float *, float, float, float);

void bolt_thief_update(L16CrateMoby *moby) __asm__("FUN_L14_002fe2a0");

/* Moby class 923 */
void bolt_thief_update(L16CrateMoby *moby) {
    enum {
        BOLT_THIEF_STATE_INITIALIZE = 0,
        BOLT_THIEF_STATE_FACE_HERO = 1,
        BOLT_THIEF_STATE_WAIT_FOR_TRIGGER = 2,
        BOLT_THIEF_STATE_SETTLE_AT_PATH_END = 3,
        BOLT_THIEF_STATE_FOLLOW_PRIMARY_PATH = 4,
        BOLT_THIEF_STATE_FOLLOW_SECONDARY_PATH = 5,
        BOLT_THIEF_STATE_KNOCKBACK = 6
    };
    L16CrateData *moby_data = moby->data;
    char         *hit;

    if ((moby->status & 2) && moby->frame == moby->animation && moby->frame == 1)
        blend_moby_animation(moby, 4, 0, scale_game_frames(5));
    if (hero.coll_hit_moby == moby && moby->animation != 1 && moby->frame != 1)
        blend_moby_animation(moby, 1, 0, scale_game_frames(5));
    hit = find_moby_hit(moby, 0x210000, 0);
    if (hit && moby->state != 6) {
        struct Hero *player = &hero;

        moby_data->knock_drag = 0.008f;
        moby_data->knock_gravity = 0.0005f;
        moby_data->knock_speed = frame_time * 24.0f;
        moby_data->knock_rise = frame_time * 12.0f;
        moby_data->knock_flags = 1;
        moby_data->knock_bAD = 0;
        trooper_start_knockback(moby,
                                moby_data->knock,
                                1,
                                1,
                                0,
                                fast_arc_tan(moby->position[0] - player->motion.pos.f[0],
                                             moby->position[1] - player->motion.pos.f[1]));
        moby_data->ticks = 0x78;
        FUN_L00_00257470(moby, 0, -1);
        moby->state = 6;
    }
    moby->opacity = 0xFF;
    switch (moby->state) {
    case BOLT_THIEF_STATE_INITIALIZE:
        if (moby_data->mode == 2) {
            moby->state = 1;
            moby_data->turn = 0.0f;
            set_moby_animation(moby, 4, 0);
        } else {
            if (moby_data->path == -1) {
                debug_print(D_L14_001FCF20, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            if (*(int *)l14_paths[moby_data->path] == 0) {
                debug_print(D_L14_001FCF58, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            if (moby_data->mode != 1) {
                if (moby_data->other_path == -1) {
                    debug_print(D_L14_001FCFA0, moby->id);
                    mark_moby_for_removal(moby);
                    return;
                }
                if (*(int *)l14_paths[moby_data->other_path] == 0) {
                    debug_print(D_L14_001FCFD8, moby->id);
                    mark_moby_for_removal(moby);
                    return;
                }
            }
            if (moby_data->trigger == -1) {
                debug_print(D_L14_001FD020, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            initialize_moby_path_state(moby);
            moby->state = 2;
            moby->collision = 0x80;
            moby->flags |= 0x41;
            moby->bounds = 0;
        }
        moby_data->child = 0;
        break;
    case BOLT_THIEF_STATE_FACE_HERO: {
        struct Hero *player = &hero;

        FUN_L00_00258278((char *)moby,
                         fast_arc_tan(player->motion.pos.f[0] - moby->position[0],
                                      player->motion.pos.f[1] - moby->position[1]),
                         &moby_data->turn,
                         0.005f,
                         0.2f,
                         0.0f);
        emit_moby_offset_particles(moby);
        break;
    }
    case BOLT_THIEF_STATE_WAIT_FOR_TRIGGER:
        if (is_point_inside_clip_volume(&hero.motion.pos, moby_data->trigger)) {
            if (moby_data->mode == 1) {
                set_moby_animation(moby, 0, 0);
            } else {
                moby_data->child = spawn_bolt_thief_mine(moby, moby->position);
                update_linked_moby_position(moby);
                set_moby_animation(moby, 2, 0);
            }
            moby->flags &= 0xFFBE;
            moby->bounds = moby->model->bounds;
            moby->state = 4;
        }
        break;
    case BOLT_THIEF_STATE_FOLLOW_PRIMARY_PATH:
        if (moby_data->mode == 0)
            update_linked_moby_position(moby);
        if (update_bolt_thief_path_movement((char *)moby)) {
            moby->state = 3;
            blend_moby_animation(moby, 3, 0, 5);
        }
        emit_moby_offset_particles(moby);
        break;
    case BOLT_THIEF_STATE_SETTLE_AT_PATH_END: {
        L16CrateData *settle_data = moby->data;
        char         *path = ((char *)l14_paths[settle_data->path]);
        float         target[4];
        float         zero;

        if (settle_data->mode == 0)
            update_linked_moby_position(moby);
        qcopy(target, path + *(int *)path * 16);
        zero = 0.0f;
        vendor_spring(moby->position,
                      &settle_data->vx,
                      target[0],
                      bolt_thief_path_spring_gain_gp,
                      bolt_thief_path_spring_damping_gp,
                      zero);
        vendor_spring(moby->position + 1,
                      &settle_data->vy,
                      target[1],
                      bolt_thief_path_spring_gain_gp,
                      bolt_thief_path_spring_damping_gp,
                      zero);
        vendor_spring(moby->position + 2,
                      &settle_data->vz,
                      target[2],
                      bolt_thief_path_spring_gain_gp,
                      bolt_thief_path_spring_damping_gp,
                      zero);
        if ((moby->status & 2) && moby->frame == moby->animation) {
            if (settle_data->mode == 1) {
                moby->state = 1;
                settle_data->turn = zero;
                set_moby_animation(moby, 4, 0);
            } else {
                if (settle_data->child) {
                    moby_set_state_one(settle_data->child);
                    settle_data->child = 0;
                }
                moby->state = 5;
                blend_moby_animation(moby, 0, 0, 20);
                settle_data->timer = zero;
                settle_data->counter = 0;
            }
        }
        emit_moby_offset_particles(moby);
        break;
    }
    case BOLT_THIEF_STATE_FOLLOW_SECONDARY_PATH:
        if (update_bolt_thief_path_movement((char *)moby)) {
            char *path;

            moby->state = 2;
            moby->bounds = 0;
            moby->flags |= 0x41;
            moby_data->timer = 0.0f;
            moby_data->counter = 0;
            moby_data->turn = moby_data->vx = moby_data->vy = moby_data->vz = moby_data->timer;
            path = ((char *)l14_paths[moby_data->path]);
            qcopy(moby->position, path + 0x10);
            moby->rotation[2] = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                             *(float *)(path + 0x24) - *(float *)(path + 0x14));
        } else {
            emit_moby_offset_particles(moby);
        }
        break;
    case BOLT_THIEF_STATE_KNOCKBACK:
        if (step_moby_motion(moby, moby_data->knock) & 3) {
            cart_burst(moby, moby->position, -1, 1.0f, 13.0f);
            mark_moby_for_removal(moby);
            return;
        }
        if (moby->position[2] < 5.0f) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    trooper_anim_finish(moby, moby_data->effect);
}

extern int sample_path(char *, float *, float *, float *, int, float) __asm__("FUN_L00_0025d808");

float bolt_thief_default_path_sample_rate_gp __asm__("D_L14_00161FB8") = 25.0f;

extern void FUN_L00_00258278(char *, float, float *, float, float, float);

int update_bolt_thief_path_movement(struct Moby *moby) {
    float pos[4];
    float delta[4];
    char *bolt_thief_state = (char *)moby->pvars;
    float path_sample_step = bolt_thief_default_path_sample_rate_gp * frame_time;
    int   path_sample_result;
    char *path;
    if (current_level_index == 0x10)
        path_sample_step = frame_time * 20.0f;
    if (*(float *)(bolt_thief_state + 0xF8) != 0.0f)
        path_sample_step = *(float *)(bolt_thief_state + 0xF8) * frame_time;
    if (moby->state == 5)
        path = ((char *)l14_paths[*(int *)(bolt_thief_state + 0xDC)]);
    else
        path = ((char *)l14_paths[*(int *)(bolt_thief_state + 0xD8)]);
    path_sample_result = sample_path(path,
                                     pos,
                                     (float *)(bolt_thief_state + 0xD0),
                                     (float *)(bolt_thief_state + 0xD4),
                                     0,
                                     path_sample_step);
    vendor_spring(&moby->pos.x,
                  (float *)(bolt_thief_state + 0xE4),
                  pos[0],
                  bolt_thief_path_spring_gain_gp,
                  bolt_thief_path_spring_damping_gp,
                  0.0f);
    vendor_spring(&moby->pos.y,
                  (float *)(bolt_thief_state + 0xE8),
                  pos[1],
                  bolt_thief_path_spring_gain_gp,
                  bolt_thief_path_spring_damping_gp,
                  0.0f);
    vendor_spring(&moby->pos.z,
                  (float *)(bolt_thief_state + 0xEC),
                  pos[2],
                  bolt_thief_path_spring_gain_gp,
                  bolt_thief_path_spring_damping_gp,
                  0.0f);
    fast_vec_sub(delta, pos, &moby->pos);
    if (absolute_float(delta[0]) > 0.01f) {
        if (absolute_float(delta[1]) > 0.01f) {
            FUN_L00_00258278(moby,
                             fast_arc_tan(delta[0], delta[1]),
                             (float *)(bolt_thief_state + 0xF0),
                             bolt_thief_path_spring_gain_gp,
                             bolt_thief_path_spring_damping_gp,
                             0.0f);
        }
    }
    return path_sample_result;
}
