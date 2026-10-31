/* Level 16 bolt_thief. */
#include "types.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern int scale_game_frames(int);

/* Update of the crate platform: hit reaction, then its seven states. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E4C08), where it is exact; names translated to the US level program. */

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
    int           path;       /* 0xD8: index into D_L16_001B0C30 */
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

extern char *D_L16_001B0930[];
extern char *spawn_bolt_thief_mine(void *, void *) __asm__("FUN_L14_002ffaf8");
extern char  D_L16_001E86A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8A58[],
    D_L16_001E8AA0[];
extern char D_L16_001E89A0[], D_L16_001E86D8[], D_L16_001E8A20[], D_L16_001E8A58[],
    D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8720[], D_L16_001E8A58[],
    D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8758[],
    D_L16_001E8AA0[] __asm__("D_L16_001E8AA0");
extern char D_L16_001E89A0[], D_L16_001E89D8[], D_L16_001E8A20[], D_L16_001E8A58[],
    D_L16_001E87A0[];
extern float frame_time, D_0015EE70;
extern int   debug_print() __asm__("FUN_001e93b0");
int          advance_bolt_thief_path(struct Moby *moby) __asm__("FUN_L16_002e3fa0");

extern int  advance_bolt_thief_path(struct Moby *moby) __asm__("FUN_L16_002e3fa0");
extern f32  D_L16_00161D94;
extern f32  D_L16_00161D98_gp __asm__("D_L16_00161D98") __attribute__((sda));
extern void FUN_L00_00257470(void *, int, int);
extern void
trooper_start_knockback(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void set_moby_state_14(void *) __asm__("FUN_L03_002c6c90");
extern void initialize_moby_path_state(void *) __asm__("FUN_L14_002fe900");
extern void update_linked_moby_position(void *) __asm__("FUN_L14_002fec28");
extern void emit_moby_offset_particles(void *) __asm__("FUN_L14_002feca8");
extern void
func_L00_002592B0_path54(char *, float, float *, float, float, float) __asm__("FUN_L00_00258278");

void bolt_thief_update(L16CrateMoby *moby) __asm__("FUN_L16_002e37a0");

/* Moby class 1401 */
void bolt_thief_update(L16CrateMoby *moby) {
    enum {
        BOLT_THIEF_INITIALIZE = 0,
        BOLT_THIEF_FACE_HERO = 1,
        BOLT_THIEF_WAIT_FOR_HERO_TRIGGER = 2,
        BOLT_THIEF_SETTLE_AT_PATH_END = 3,
        BOLT_THIEF_FOLLOW_PRIMARY_PATH = 4,
        BOLT_THIEF_FOLLOW_OTHER_PATH = 5,
        BOLT_THIEF_KNOCKBACK = 6
    };
    L16CrateData *bolt_thief_data = moby->data;
    char         *hit;

    if ((moby->status & 2) && moby->frame == moby->animation && moby->frame == 1)
        blend_moby_animation(moby, 4, 0, scale_game_frames(5));
    if (hero.coll_hit_moby == moby && moby->animation != 1 && moby->frame != 1)
        blend_moby_animation(moby, 1, 0, scale_game_frames(5));
    hit = find_moby_hit(moby, 0x210000, 0);
    if (!hit && bolt_thief_data->child)
        hit = find_moby_hit(bolt_thief_data->child, 0x210000, 0);
    if (hit && moby->state != 6) {
        struct Hero *player = &hero;

        bolt_thief_data->knock_drag = 0.008f;
        bolt_thief_data->knock_gravity = 0.0005f;
        bolt_thief_data->knock_speed = frame_time * 24.0f;
        bolt_thief_data->knock_rise = frame_time * 12.0f;
        bolt_thief_data->knock_flags = 1;
        bolt_thief_data->knock_bAD = 0;
        trooper_start_knockback(moby,
                                bolt_thief_data->knock,
                                1,
                                1,
                                0,
                                fast_arc_tan(moby->position[0] - player->motion.pos.f[0],
                                             moby->position[1] - player->motion.pos.f[1]));
        bolt_thief_data->ticks = 0x78;
        FUN_L00_00257470(moby, 0, -1);
        moby->state = 6;
    }
    moby->opacity = 0xFF;
    switch (moby->state) {
    case BOLT_THIEF_INITIALIZE:
        if (bolt_thief_data->mode == 2) {
            moby->state = 1;
            bolt_thief_data->turn = 0.0f;
            set_moby_animation(moby, 4, 0);
        } else {
            if (bolt_thief_data->path == -1) {
                debug_print(D_L16_001E86A0, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            if (*(int *)D_L16_001B0930[bolt_thief_data->path] == 0) {
                debug_print(D_L16_001E86D8, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            if (bolt_thief_data->mode != 1) {
                if (bolt_thief_data->other_path == -1) {
                    debug_print(D_L16_001E8720, moby->id);
                    mark_moby_for_removal(moby);
                    return;
                }
                if (*(int *)D_L16_001B0930[bolt_thief_data->other_path] == 0) {
                    debug_print(D_L16_001E8758, moby->id);
                    mark_moby_for_removal(moby);
                    return;
                }
            }
            if (bolt_thief_data->trigger == -1) {
                debug_print(D_L16_001E87A0, moby->id);
                mark_moby_for_removal(moby);
                return;
            }
            initialize_moby_path_state(moby);
            moby->state = 2;
            moby->collision = 0x80;
            moby->flags |= 0x41;
            moby->bounds = 0;
        }
        bolt_thief_data->child = 0;
        break;
    case BOLT_THIEF_FACE_HERO: {
        struct Hero *player = &hero;

        func_L00_002592B0_path54((char *)moby,
                                 fast_arc_tan(player->motion.pos.f[0] - moby->position[0],
                                              player->motion.pos.f[1] - moby->position[1]),
                                 &bolt_thief_data->turn,
                                 0.005f,
                                 0.2f,
                                 0.0f);
        emit_moby_offset_particles(moby);
        break;
    }
    case BOLT_THIEF_WAIT_FOR_HERO_TRIGGER:
        if (is_point_inside_clip_volume(&hero.motion.pos, bolt_thief_data->trigger)) {
            if (bolt_thief_data->mode == 1) {
                set_moby_animation(moby, 0, 0);
            } else {
                bolt_thief_data->child = spawn_bolt_thief_mine(moby, moby->position);
                update_linked_moby_position(moby);
                set_moby_animation(moby, 2, 0);
            }
            moby->flags &= 0xFFBE;
            moby->bounds = moby->model->bounds;
            moby->state = 4;
        }
        break;
    case BOLT_THIEF_FOLLOW_PRIMARY_PATH:
        if (bolt_thief_data->mode == 0)
            update_linked_moby_position(moby);
        if (advance_bolt_thief_path((char *)moby)) {
            moby->state = 3;
            blend_moby_animation(moby, 3, 0, 5);
        }
        emit_moby_offset_particles(moby);
        break;
    case BOLT_THIEF_SETTLE_AT_PATH_END: {
        L16CrateData *settle_data = moby->data;
        char         *path = D_L16_001B0930[settle_data->path];
        float         target[4];
        float         zero;

        if (settle_data->mode == 0)
            update_linked_moby_position(moby);
        qcopy(target, path + *(int *)path * 16);
        zero = 0.0f;
        vendor_spring(moby->position,
                      &settle_data->vx,
                      target[0],
                      *(float *)&D_L16_00161D94,
                      *(float *)&D_L16_00161D98_gp,
                      zero);
        vendor_spring(moby->position + 1,
                      &settle_data->vy,
                      target[1],
                      *(float *)&D_L16_00161D94,
                      *(float *)&D_L16_00161D98_gp,
                      zero);
        vendor_spring(moby->position + 2,
                      &settle_data->vz,
                      target[2],
                      *(float *)&D_L16_00161D94,
                      *(float *)&D_L16_00161D98_gp,
                      zero);
        if ((moby->status & 2) && moby->frame == moby->animation) {
            if (settle_data->mode == 1) {
                moby->state = 1;
                settle_data->turn = zero;
                set_moby_animation(moby, 4, 0);
            } else {
                if (settle_data->child) {
                    set_moby_state_14(settle_data->child);
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
    case BOLT_THIEF_FOLLOW_OTHER_PATH:
        if (advance_bolt_thief_path((char *)moby)) {
            char *path;

            moby->state = 2;
            moby->bounds = 0;
            moby->flags |= 0x41;
            bolt_thief_data->timer = 0.0f;
            bolt_thief_data->counter = 0;
            bolt_thief_data->turn = bolt_thief_data->vx = bolt_thief_data->vy =
                bolt_thief_data->vz = bolt_thief_data->timer;
            path = D_L16_001B0930[bolt_thief_data->path];
            qcopy(moby->position, path + 0x10);
            moby->rotation[2] = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                             *(float *)(path + 0x24) - *(float *)(path + 0x14));
        } else {
            emit_moby_offset_particles(moby);
        }
        break;
    case BOLT_THIEF_KNOCKBACK:
        if (step_moby_motion(moby, bolt_thief_data->knock) & 3) {
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
    trooper_anim_finish(moby, bolt_thief_data->effect);
}

#include "rnc/overlay/l16/mobys/bolt_thief_globals.h"

extern float bolt_thief_path_speed_gp __asm__("D_L16_00161D90") __attribute__((sda));
extern float D_L16_00161D94_gp __asm__("D_L16_00161D94") __attribute__((sda));
extern float D_L16_00161D98_gp __asm__("D_L16_00161D98") __attribute__((sda));
extern int
FUN_L00_0025d808_path54(char *, float *, float *, float *, int, float) __asm__("FUN_L00_0025d808");

int advance_bolt_thief_path(struct Moby *moby) {
    float pos[4];
    float delta[4];
    char *bolt_thief_path_data = (char *)moby->pvars;
    float scaled_frame_time = bolt_thief_path_speed_gp * frame_time;
    int   path_sample_result;
    char *path;
    if (moby->state == 5)
        path = D_L16_001B0930[*(int *)(bolt_thief_path_data + 0xDC)];
    else
        path = D_L16_001B0930[*(int *)(bolt_thief_path_data + 0xD8)];
    path_sample_result = FUN_L00_0025d808_path54(path,
                                                 pos,
                                                 (float *)(bolt_thief_path_data + 0xD0),
                                                 (float *)(bolt_thief_path_data + 0xD4),
                                                 0,
                                                 scaled_frame_time);
    vendor_spring(&moby->pos.x,
                  (float *)(bolt_thief_path_data + 0xE4),
                  pos[0],
                  D_L16_00161D94_gp,
                  D_L16_00161D98_gp,
                  0.0f);
    vendor_spring(&moby->pos.y,
                  (float *)(bolt_thief_path_data + 0xE8),
                  pos[1],
                  D_L16_00161D94_gp,
                  D_L16_00161D98_gp,
                  0.0f);
    vendor_spring(&moby->pos.z,
                  (float *)(bolt_thief_path_data + 0xEC),
                  pos[2],
                  D_L16_00161D94_gp,
                  D_L16_00161D98_gp,
                  0.0f);
    fast_vec_sub(delta, pos, &moby->pos);
    if (absolute_float(delta[0]) > 0.01f) {
        if (absolute_float(delta[1]) > 0.01f) {
            func_L00_002592B0_path54(moby,
                                     fast_arc_tan(delta[0], delta[1]),
                                     (float *)(bolt_thief_path_data + 0xF0),
                                     D_L16_00161D94_gp,
                                     D_L16_00161D98_gp,
                                     0.0f);
        }
    }
    return path_sample_result;
}
