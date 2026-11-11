/* Level 16 chomper. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

#include "qcopy.h"
#include "rnc/overlay/shared/mobys/moby_anim.h"
#include "rnc/overlay/shared/mobys/entities.h"
#include "rnc/overlay/shared/mobys/watch.h"
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

enum {
    CHOMPER_INITIALIZE = 0,
    CHOMPER_WAIT_TO_APPROACH_TARGET = 1,
    CHOMPER_APPROACH_TARGET = 4,
    CHOMPER_FACE_TARGET = 5,
    CHOMPER_RETURN_HOME = 6,
    CHOMPER_KNOCKBACK = 8
};

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
s32  is_value_within_interpolated_window(struct InterpolatedStateEntry *anim,
                                         f32 fparg0) __asm__("FUN_00214cc8");

short D_L16_00161E48 = 0;
void  chomper_update(struct Moby *m) __asm__("FUN_L16_002e5e08");

typedef struct ChomperVars {
    u8  pad_0[0x20];
    f32 health;
    u16 max_health;
    u8  pad_26[0x2];
    u8  hit_response_class;
    u8  pad_29[0x2F];
    u8  look_range; /* 0x58: enemy target info at pvars+0x20, see moby_target_info.h */
    u8  pad_59[0x1];
    u8  look_height;
    u8  pad_5B[0x6D];
    u16 suck_state;
    u8  pad_CA[0x6];
    s32 suck_sequences;
    u8  pad_D4[0x4C];
    u8  motion;
    u8  pad_121[0x5F];
    f32 path_search_point[4];
    u8  pad_190[0x34];
    s32 pursuit_target_selection_status;
    u8  pad_1C8[0x8];
    f32 home_position[4];
    u8  pad_1E0[0x10];
    s32 path; /* 0x1F0: index into D_L16_001B0930, -1: none */
    f32 yaw_velocity;
    f32 heading_offset;
    u8  pad_1FC[0x4];
    u8  pad_200[0x4];
    f32 travel_distance_velocity;
    f32 previous_height_drop;
    u8  pad_20C[0x14];
    u8  big_head_manipulator;
} ChomperVars;

/* Moby class 1445 */
void chomper_update(struct Moby *chomper_moby) {
    enum {
        CHOMPER_TARGET_SELECTION_REJECTED = 2
    };
    ChomperVars *chomper_data = (ChomperVars *)chomper_moby->pvars;
    update_chomper_hit_reaction_and_target(chomper_moby);
    FUN_L00_00263ac8(2.1f, (int)chomper_moby, 1, &chomper_data->big_head_manipulator);
    if (chomper_moby->visible != 0 && fast_vec_dist(&chomper_moby->pos, D_L16_001671C0) < 29.0f) {
        FUN_L00_0025a120(chomper_moby);
        chomper_moby->shadow_range = 23;
    }
    switch (chomper_moby->state) {
    case CHOMPER_INITIALIZE:
        if (chomper_data->path == -1) {
            mark_moby_for_removal(chomper_moby);
            return;
        }
        chomper_data->max_health = 1;
        chomper_data->hit_response_class = 0;
        chomper_data->health = 1.0f;
        chomper_data->suck_sequences = (int)&D_L16_00161E48;
        if (rand() & 1)
            chomper_moby->flags |= 0x8000;
        qcopy(chomper_data->home_position, &chomper_moby->pos);
        chomper_moby->state = CHOMPER_WAIT_TO_APPROACH_TARGET;
        if (chomper_moby->prev_seq != 2)
            blend_moby_animation(chomper_moby, 2, 0, scale_game_frames(20));
        chomper_data->look_range = truncate_float_to_s32(8.0f);
        chomper_data->look_height = truncate_float_to_s32(3.2f);
        break;
    case CHOMPER_WAIT_TO_APPROACH_TARGET:
        if (chomper_data->pursuit_target_selection_status != CHOMPER_TARGET_SELECTION_REJECTED) {
            chomper_moby->state = CHOMPER_APPROACH_TARGET;
            if (chomper_moby->prev_seq != 0)
                blend_moby_animation(chomper_moby, 0, 0, scale_game_frames(20));
        }
        break;
    case 2:
    case 3:
        break;
    case CHOMPER_APPROACH_TARGET:
        if (move_chomper_toward_position(chomper_moby, chomper_data->path_search_point) < 1.25f) {
            chomper_moby->state = CHOMPER_FACE_TARGET;
            if (chomper_moby->prev_seq != 1)
                blend_moby_animation(chomper_moby, 1, 0, scale_game_frames(20));
        } else if (chomper_data->pursuit_target_selection_status ==
                   CHOMPER_TARGET_SELECTION_REJECTED)
            chomper_moby->state = CHOMPER_RETURN_HOME;
        break;
    case CHOMPER_FACE_TARGET: {
        float *yaw = &chomper_moby->rot.z;
        float  angle = fast_arc_tan(chomper_data->path_search_point[0] - chomper_moby->pos.x,
                                   chomper_data->path_search_point[1] - chomper_moby->pos.y);
        approach_angle(yaw,
                       &chomper_data->yaw_velocity,
                       angle,
                       frame_time_sq * 12.566371f,
                       frame_time_sq * 12.566371f,
                       frame_time * 12.566371f);
        if (is_value_within_interpolated_window(chomper_moby, 13.0f)) {
            collect_mobies_in_sphere_at_moby_point(chomper_moby, 0, 1, 0.333f, 1.0f, 1.0f, 0, 1, 0);
        } else if ((chomper_moby->anim_flags & 2) &&
                   fast_vec_dist(&chomper_moby->pos, chomper_data->path_search_point) > 1.5f) {
            chomper_moby->state = CHOMPER_APPROACH_TARGET;
            if (chomper_moby->prev_seq != 0)
                blend_moby_animation(chomper_moby, 0, 0, scale_game_frames(20));
        }
        break;
    }
    case CHOMPER_RETURN_HOME:
        if (move_chomper_toward_position(chomper_moby, chomper_data->home_position) < 1.0f) {
            chomper_moby->state = CHOMPER_WAIT_TO_APPROACH_TARGET;
            if (chomper_moby->prev_seq != 2)
                blend_moby_animation(chomper_moby, 2, 0, scale_game_frames(20));
        } else if (chomper_data->pursuit_target_selection_status !=
                   CHOMPER_TARGET_SELECTION_REJECTED)
            chomper_moby->state = CHOMPER_APPROACH_TARGET;
        break;
    case 7:
        if (FUN_L00_002dc9f0(chomper_moby, &chomper_data->motion)) {
            chomper_moby->state = CHOMPER_WAIT_TO_APPROACH_TARGET;
            chomper_data->suck_state = 0;
        }
        break;
    case CHOMPER_KNOCKBACK:
        if (step_moby_motion(chomper_moby, &chomper_data->motion) & 0x140) {
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
extern void update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");

#include "rnc/overlay/shared/math/quad.h"

/* Start the knock-back when the moby is hit, then steer it along its path and note when it strays. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E7670), where it is exact; names translated to the US level program. */

typedef struct {
    char                 pad0[0x110];
    struct HitColorFlash hit_flash;
    float                knock[4];     /* 0x120: record func_L00_0025D5B0 fills */
    float                speed;        /* 0x130 */
    float                acceleration; /* 0x134 */
    float                jump_speed;   /* 0x138 */
    float                turn_speed;   /* 0x13C */
    int                  flags;        /* 0x140 */
    int                  kind;         /* 0x144 */
    float                strength;     /* 0x148 */
    char                 pad14C[0x11];
    unsigned char        pending; /* 0x15D */
    char                 pad15E[0xE];
    float                timer; /* 0x16C */
    float                knockback_ascent_animation_target;
    float                knockback_descent_animation_target;
    char                 pad178[8];
    float                target[4]; /* 0x180 */
    char                 pad190[0x30];
    int                  target_moby;
    int                  target_selection_status;
    char                 pad1C8[8];
    float                home[4]; /* 0x1D0 */
    char                 pad1E0[0x10];
    int                  path; /* 0x1F0: index into D_L16_001B0C30 */
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
extern void  start_hit_color_flash(void *m, short *p) __asm__("FUN_L00_0025d458");

void update_chomper_hit_reaction_and_target(void *chomper_instance) {
    enum {
        CHOMPER_TARGET_SELECTION_REJECTED = 2
    };
    L16PursuitMoby *chomper_moby = chomper_instance;
    L16PursuitData *pursuit_data = chomper_moby->data;
    char           *hit;
    int             reaction_kind_or_path_address; /* the reaction kind, later the path */
    int reaction_flags_or_path_entry_address; /* the reaction flags, later the path's slot in the table */
    float knockback_vector_and_heading
        [8]; /* [0..3] where the hit came from, [4] the heading that gives */

    if (chomper_moby->state == CHOMPER_INITIALIZE) {
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
        *(OvlQuad *)knockback_vector_and_heading = *(OvlQuad *)(hit + 0x10);
        trooper_knockback_arc(knockback_vector_and_heading,
                              knockback_vector_and_heading + 4,
                              &pursuit_data->jump_speed,
                              &pursuit_data->turn_speed);
        trooper_start_knockback(
            chomper_moby, pursuit_data->knock, 5, 1, 0, knockback_vector_and_heading[4]);
        pursuit_data->knockback_ascent_animation_target = 8.0f;
        pursuit_data->knockback_descent_animation_target = 16.0f;
        chomper_moby->state = CHOMPER_KNOCKBACK;
        chomper_moby->i94 = 0;
        pursuit_data->hit_flash.flash_red = 0x78;
        start_hit_color_flash(chomper_moby, &pursuit_data->hit_flash);
    }
    chomper_moby->bA4 = 0xFF;
    update_hit_color_flash(chomper_moby, &pursuit_data->hit_flash);
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
            pursuit_data->target_selection_status = CHOMPER_TARGET_SELECTION_REJECTED;
        }
    }
    if (pursuit_data->target_moby == 0) {
        struct Hero *player = &hero;
        pursuit_data->target_moby = (int)player->moby;
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
    char        *moby = m_v;
    float       *target_position = t_v;
    ChomperVars *moby_data = (ChomperVars *)MOBY(moby)->pvars;
    float        interpolated_travel_distance;
    float        movement_delta[4];
    float        distance_to_target;
    float        oldz;
    float        target_yaw = fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                                    target_position[1] - MOBY(moby)->pos.y);
    target_yaw = fast_add_rotations(target_yaw, moby_data->heading_offset);
    oldz = MOBY(moby)->pos.z;
    interpolated_travel_distance = 0.0f;
    distance_to_target = fast_vec_dist(moby + 0x10, target_position);
    approach_angle((float *)&MOBY(moby)->rot.z,
                   &moby_data->yaw_velocity,
                   target_yaw,
                   frame_time_sq * 12.566371f,
                   frame_time_sq * 12.566371f,
                   frame_time * 12.566371f);
    advance_accelerated_scalar(&interpolated_travel_distance,
                               distance_to_target,
                               frame_time_sq * 8.0f,
                               frame_time_sq * 12.0f,
                               frame_time * 4.0f,
                               &moby_data->travel_distance_velocity);
    fast_vec_normalize(movement_delta, moby + 0xC0, moby_data->travel_distance_velocity);
    movement_delta[2] = moby_data->previous_height_drop - frame_time_sq * 10.0f;
    FUN_L00_00258830(moby, movement_delta, 0.5f, 0.5f, 0.0f, 0x10);
    resolve_point_path_segment_xy_clearance(
        0.333f, *(void **)&moby_data->path, moby + 0x10, moby + 0x10);
    moby_data->previous_height_drop = oldz - MOBY(moby)->pos.z;
    return distance_to_target;
}
