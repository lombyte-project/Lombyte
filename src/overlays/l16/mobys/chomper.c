/* Level 16 chomper. */
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

extern int   scale_game_frames(int);
extern float frame_time, D_0015EE70;
extern void  FUN_L00_00257470(void *, int, int);
extern float frame_time, D_0015EE70_c __asm__("D_0015EE70");

extern char  D_L16_001671C0[];
extern float move_chomper_toward_position(void *, void *) __asm__("FUN_L16_002e6428");
extern int   FUN_L00_002dc9f0(void *, void *);
extern s32   rand();
extern void  collect_mobies_in_sphere_at_moby_point(
     void *, int, int, float, float, float, int, int, int) __asm__("FUN_L00_002599e8");
extern void FUN_L00_0025a120(void *);
extern void FUN_L00_00263ac8(float, int, int, unsigned char *);
void update_chomper_hit_reaction_and_target(void *chomper_instance) __asm__("FUN_L16_002e6208");

void update_chomper_hit_reaction_and_target(void *arg) __asm__("FUN_L16_002e6208");
s32  is_value_within_interpolated_window(struct InterpolatedStateEntry *arg0,
                                         f32 fparg0) __asm__("FUN_00214cc8");

short D_L16_00161E48 = 0;
void  chomper_update(struct Moby *m) __asm__("FUN_L16_002e5e08");

void chomper_update(struct Moby *chomper_moby) {
    enum {
        CHOMPER_INITIALIZE = 0,
        CHOMPER_WAIT_TO_APPROACH_TARGET = 1,
        CHOMPER_APPROACH_TARGET = 4,
        CHOMPER_FACE_TARGET = 5,
        CHOMPER_RETURN_HOME = 6,
        CHOMPER_KNOCKBACK = 8
    };
    char *chomper_data = (char *)chomper_moby->pvars;
    update_chomper_hit_reaction_and_target(chomper_moby);
    FUN_L00_00263ac8(2.1f, (int)chomper_moby, 1, (unsigned char *)(chomper_data + 0x220));
    if (chomper_moby->visible != 0 && fast_vec_dist(&chomper_moby->pos, D_L16_001671C0) < 29.0f) {
        FUN_L00_0025a120(chomper_moby);
        chomper_moby->shadow_range = 23;
    }
    switch (chomper_moby->state) {
    case CHOMPER_INITIALIZE:
        if (*(int *)(chomper_data + 0x1F0) == -1) {
            mark_moby_for_removal(chomper_moby);
            return;
        }
        *(unsigned short *)(chomper_data + 0x24) = 1;
        *(unsigned char *)(chomper_data + 0x28) = 0;
        *(float *)(chomper_data + 0x20) = 1.0f;
        *(int *)(chomper_data + 0xD0) = (int)&D_L16_00161E48;
        if (rand() & 1)
            chomper_moby->flags |= 0x8000;
        qcopy(chomper_data + 0x1D0, &chomper_moby->pos);
        chomper_moby->state = 1;
        if (chomper_moby->prev_seq != 2)
            blend_moby_animation(chomper_moby, 2, 0, scale_game_frames(20));
        *(unsigned char *)(chomper_data + 0x58) = truncate_float_to_s32(8.0f);
        *(unsigned char *)(chomper_data + 0x5A) = truncate_float_to_s32(3.2f);
        break;
    case CHOMPER_WAIT_TO_APPROACH_TARGET:
        if (*(int *)(chomper_data + 0x1C4) != 2) {
            chomper_moby->state = 4;
            if (chomper_moby->prev_seq != 0)
                blend_moby_animation(chomper_moby, 0, 0, scale_game_frames(20));
        }
        break;
    case 2:
    case 3:
        break;
    case CHOMPER_APPROACH_TARGET:
        if (move_chomper_toward_position(chomper_moby, chomper_data + 0x180) < 1.25f) {
            chomper_moby->state = 5;
            if (chomper_moby->prev_seq != 1)
                blend_moby_animation(chomper_moby, 1, 0, scale_game_frames(20));
        } else if (*(int *)(chomper_data + 0x1C4) == 2)
            chomper_moby->state = 6;
        break;
    case CHOMPER_FACE_TARGET: {
        float *yaw = &chomper_moby->rot.z;
        float  angle = fast_arc_tan(*(float *)(chomper_data + 0x180) - chomper_moby->pos.x,
                                   *(float *)(chomper_data + 0x184) - chomper_moby->pos.y);
        approach_angle(yaw,
                       (float *)(chomper_data + 0x1F4),
                       angle,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        if (is_value_within_interpolated_window(chomper_moby, 13.0f)) {
            collect_mobies_in_sphere_at_moby_point(chomper_moby, 0, 1, 0.333f, 1.0f, 1.0f, 0, 1, 0);
        } else if ((chomper_moby->anim_flags & 2) &&
                   fast_vec_dist(&chomper_moby->pos, chomper_data + 0x180) > 1.5f) {
            chomper_moby->state = 4;
            if (chomper_moby->prev_seq != 0)
                blend_moby_animation(chomper_moby, 0, 0, scale_game_frames(20));
        }
        break;
    }
    case CHOMPER_RETURN_HOME:
        if (move_chomper_toward_position(chomper_moby, chomper_data + 0x1D0) < 1.0f) {
            chomper_moby->state = 1;
            if (chomper_moby->prev_seq != 2)
                blend_moby_animation(chomper_moby, 2, 0, scale_game_frames(20));
        } else if (*(int *)(chomper_data + 0x1C4) != 2)
            chomper_moby->state = 4;
        break;
    case 7:
        if (FUN_L00_002dc9f0(chomper_moby, chomper_data + 0x120)) {
            chomper_moby->state = 1;
            *(unsigned short *)(chomper_data + 0xC8) = 0;
        }
        break;
    case CHOMPER_KNOCKBACK:
        if (step_moby_motion(chomper_moby, chomper_data + 0x120) & 0x140) {
            cart_burst(chomper_moby, &chomper_moby->pos, 6, 0.5f, 13.0f);
            FUN_L00_00257470(chomper_moby, 0, -1);
            mark_moby_for_removal(chomper_moby);
        }
        break;
    }
}

extern char *D_L16_001B0930[];
extern void
trooper_start_knockback(void *, void *, int, int, int, float) __asm__("FUN_L00_0025c558");
extern void trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");

#include "rnc/overlay/quad.h"

/* Start the knock-back when the moby is hit, then steer it along its path and note when it strays. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E7670), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[0x110];
    short         reaction[3]; /* 0x110: record func_L00_0025E4B0 and func_L00_0025E590 take */
    char          pad116;
    unsigned char reaction_time; /* 0x117 */
    char          pad118[8];
    float         knock[4];     /* 0x120: record func_L00_0025D5B0 fills */
    float         speed;        /* 0x130 */
    float         acceleration; /* 0x134 */
    float         jump_speed;   /* 0x138 */
    float         turn_speed;   /* 0x13C */
    int           flags;        /* 0x140 */
    int           kind;         /* 0x144 */
    float         strength;     /* 0x148 */
    char          pad14C[0x11];
    unsigned char pending; /* 0x15D */
    char          pad15E[0xE];
    float         timer; /* 0x16C */
    float         f170;
    float         f174;
    char          pad178[8];
    float         target[4]; /* 0x180 */
    char          pad190[0x30];
    int           i1C0;
    int           i1C4;
    char          pad1C8[8];
    float         home[4]; /* 0x1D0 */
    char          pad1E0[0x10];
    int           path; /* 0x1F0: index into D_L16_001B0C30 */
} L16PursuitData;

typedef struct {
    char            pad0[0x10];
    float           position[4]; /* 0x10 */
    unsigned char   state;       /* 0x20 */
    char            pad21[0x13];
    unsigned short  flags; /* 0x34 */
    char            pad36[0x42];
    L16PursuitData *data; /* 0x78 */
    char            pad7C[0x18];
    int             i94;
    char            pad98[0xC];
    unsigned char   bA4;
} L16PursuitMoby;

#include "rnc/overlay/l16/mobys/chomper_globals.h"

extern float chomper_knockback_horizontal_speed_approach_rate_gp __asm__("D_L16_00161E58")
    __attribute__((sda));
extern float chomper_knockback_gravity_gp __asm__("D_L16_00161E54") __attribute__((sda));
extern float chomper_knockback_vertical_speed_gp __asm__("D_L16_00161E5C") __attribute__((sda));
extern float chomper_knockback_horizontal_speed_gp __asm__("D_L16_00161E60") __attribute__((sda));
extern int   FUN_L00_0025ff38(void *, void *, float, int, int, void *, int);
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");

void update_chomper_hit_reaction_and_target(void *chomper_instance) {
    L16PursuitMoby *chomper_moby = chomper_instance;
    L16PursuitData *pursuit_data = chomper_moby->data;
    char           *hit;
    int             reaction_kind_or_path_address; /* the reaction kind, later the path */
    int reaction_flags_or_path_entry_address; /* the reaction flags, later the path's slot in the table */
    float scratch[8]; /* [0..3] where the hit came from, [4] the heading that gives */

    if (chomper_moby->state == 0) {
        return;
    }
    hit = find_moby_hit(chomper_moby, 0x330000, 0);
    if (hit) {
        reaction_flags_or_path_entry_address = 0x200;
        pursuit_data->flags = reaction_flags_or_path_entry_address;
        pursuit_data->strength = 0.5f;
        pursuit_data->speed = chomper_knockback_gravity_gp * frame_time_sq;
        pursuit_data->acceleration =
            chomper_knockback_horizontal_speed_approach_rate_gp * frame_time_sq;
        pursuit_data->jump_speed = chomper_knockback_horizontal_speed_gp * frame_time;
        pursuit_data->turn_speed = chomper_knockback_vertical_speed_gp * frame_time;
        reaction_kind_or_path_address = 0x29;
        pursuit_data->kind = reaction_kind_or_path_address;
        pursuit_data->pending = 0;
        pursuit_data->timer = frame_time * 2.0f;
        chomper_moby->flags &= ~0x1000;
        *(OvlQuad *)scratch = *(OvlQuad *)(hit + 0x10);
        trooper_knockback_arc(
            scratch, scratch + 4, &pursuit_data->jump_speed, &pursuit_data->turn_speed);
        trooper_start_knockback(chomper_moby, pursuit_data->knock, 5, 1, 0, scratch[4]);
        pursuit_data->f170 = 8.0f;
        pursuit_data->f174 = 16.0f;
        chomper_moby->state = 8;
        chomper_moby->i94 = 0;
        pursuit_data->reaction_time = 0x78;
        trooper_anim_update(chomper_moby, pursuit_data->reaction);
    }
    chomper_moby->bA4 = 0xFF;
    trooper_anim_finish(chomper_moby, pursuit_data->reaction);
    reaction_flags_or_path_entry_address = (int)&D_L16_001B0930[pursuit_data->path];
    reaction_kind_or_path_address = *(int *)reaction_flags_or_path_entry_address;
    if (FUN_L00_0025ff38(chomper_moby,
                         pursuit_data->target,
                         16.0f,
                         0,
                         0,
                         (void *)(reaction_kind_or_path_address + 0x10),
                         *(int *)reaction_kind_or_path_address) != 2) {
        if (vector_distance_xy(pursuit_data->home, pursuit_data->target) > 16.0f ||
            absolute_float(chomper_moby->position[2] - pursuit_data->target[2]) > 3.0f) {
            pursuit_data->i1C4 = 2;
        }
    }
    if (pursuit_data->i1C0 == 0) {
        struct Hero *player = &hero;
        pursuit_data->i1C0 = (int)player->moby;
        qcopy(pursuit_data->target, &player->motion.pos);
    }
}

extern float fast_add_rotations(float, float);
extern float D_0015EE6C_u;
extern void  FUN_L00_00258830(void *, void *, float, float, float, int);
extern void
resolve_point_path_segment_xy_clearance(float, void *, void *, void *) __asm__("FUN_L00_00261d78");
extern float
advance_accelerated_scalar(float *, float, float, float, float, float *) __asm__("FUN_00213f38");

float move_chomper_toward_position(void *m_v, void *t_v) {
    char  *moby = m_v;
    float *target_position = t_v;
    char  *moby_data = ((char *)MOBY(moby)->pvars);
    float  interpolated_travel_distance;
    float  movement_delta[4];
    float  distance_to_target;
    float  oldz;
    float  target_yaw = fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                                    target_position[1] - MOBY(moby)->pos.y);
    target_yaw = fast_add_rotations(target_yaw, *(float *)(moby_data + 0x1F8));
    oldz = MOBY(moby)->pos.z;
    interpolated_travel_distance = 0.0f;
    distance_to_target = fast_vec_dist(moby + 0x10, target_position);
    approach_angle((float *)&MOBY(moby)->rot.z,
                   (float *)(moby_data + 0x1F4),
                   target_yaw,
                   frame_time_sq * 12.566371f,
                   frame_time_sq * 12.566371f,
                   frame_time * 12.566371f);
    advance_accelerated_scalar(&interpolated_travel_distance,
                               distance_to_target,
                               frame_time_sq * 8.0f,
                               frame_time_sq * 12.0f,
                               frame_time * 4.0f,
                               (float *)(moby_data + 0x204));
    fast_vec_normalize(movement_delta, moby + 0xC0, *(float *)(moby_data + 0x204));
    movement_delta[2] = *(float *)(moby_data + 0x208) - frame_time_sq * 10.0f;
    FUN_L00_00258830(moby, movement_delta, 0.5f, 0.5f, 0.0f, 0x10);
    resolve_point_path_segment_xy_clearance(
        0.333f, *(void **)(moby_data + 0x1F0), moby + 0x10, moby + 0x10);
    *(float *)(moby_data + 0x208) = oldz - MOBY(moby)->pos.z;
    return distance_to_target;
}
