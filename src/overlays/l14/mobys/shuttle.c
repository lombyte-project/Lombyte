/* Level 14 shuttle moby class. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/vehicles/gunship_follow_path.h"
#include "rnc/overlay/l14/mobys/shuttle_globals.h"

typedef struct {
    u8  pad0[0x74];
    s32 path; /* 0x74: path index into D_L14_001B0BB0 */
    u8  pad78[0x84];
    f32 height; /* 0xFC */
    u8  pad100[0x50];
    s32 rider;     /* 0x150: moby index carried along */
    s32 anchor;    /* 0x154: joint the rider follows, -1 for the moby position */
    s32 mode;      /* 0x158: 0x3039 when the rider is attached */
    s32 sound;     /* 0x15C */
    f32 offset[4]; /* 0x160 */
    u8  pad170[0x10];
    u8  spring[0x40]; /* 0x180 */
    u8  manip[0x10];  /* 0x1C0 */
    u8  wobble[0x30]; /* 0x1D0 */
    f32 angle;        /* 0x200 */
    s16 riding;       /* 0x204 */
    s16 h206;         /* 0x206 */
    u8  pad208[4];
    s16 timer; /* 0x20C */
    s16 pad20E;
    f32 timer_inv; /* 0x210 */
    f32 from;      /* 0x214 */
    f32 to;        /* 0x218 */
    s32 clip;      /* 0x21C */
} L14LiftVars;

extern struct Moby *D_L14_0015FFD8_lift __asm__("D_L14_0015FFD8");

extern char shuttle_missing_target_error_format[] __asm__("D_L14_001FC840");

extern s32 shuttle_enter_riding_transition_frames_gp __asm__("D_L14_00161B8C") __attribute__((sda));

extern s32 shuttle_exit_riding_transition_frames_gp __asm__("D_L14_00161B90") __attribute__((sda));

extern f32 shuttle_horizontal_rider_correction_limit_scale_gp __asm__("D_L14_00161B88")
    __attribute__((sda));

extern void debug_print(const char *, short) __asm__("FUN_001e93b0");

extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");

extern void FUN_L00_001fff28(void *, int, float);

void shuttle_update(struct Moby *m) __asm__("FUN_L14_002e7bd8");

void shuttle_update(struct Moby *shuttle_moby) {
    L14LiftVars *shuttle_vars;
    Vec4         old;
    Vec4         rot;
    Vec4         delta;
    Vec4         base;
    Vec4         rotated_rider_offset;
    f32          shuttle_rotation_matrix[16];
    struct Moby *rider;
    f32          horizontal_rider_correction_distance;
    f32          max;

    if (shuttle_moby == 0)
        return;
    shuttle_vars = (L14LiftVars *)shuttle_moby->pvars;
    if (shuttle_vars == 0)
        return;
    if (shuttle_vars->clip >= 0 &&
        is_point_inside_clip_volume(&hero.motion.pos, shuttle_vars->clip) != 0) {
        shuttle_moby->collision = 0;
        shuttle_moby->flags |= 0x41;
        if (shuttle_vars->rider >= 0) {
            struct Moby *r = &D_L14_0015FFD8_lift[shuttle_vars->rider];
            r->collision = 0;
            r->flags = (r->flags | 0x41) & 0xEFFF;
        }
        return;
    }
    if (shuttle_moby->collision == 0) {
        shuttle_moby->flags &= 0xFFBE;
        shuttle_moby->collision = shuttle_moby->pclass->collision;
        if (shuttle_vars->rider >= 0) {
            struct Moby *r = &D_L14_0015FFD8_lift[shuttle_vars->rider];
            r->flags &= 0xFFBE;
            r->collision = r->pclass->collision;
            r->flags |= 0x1000;
        }
    }
    qcopy(&old, &shuttle_moby->pos);
    qcopy(&rot, &shuttle_moby->rot);
    if (shuttle_moby->state == 0) {
        qcopy(&shuttle_moby->pos, ((char *)l14_paths[shuttle_vars->path]) + 0x10);
        shuttle_vars->angle = 0.0f;
        shuttle_vars->sound = -1;
        shuttle_vars->h206 = 0;
        attach_manipulator(shuttle_moby, 0, shuttle_vars->manip);
        if (shuttle_vars->rider == -1) {
            debug_print(shuttle_missing_target_error_format, shuttle_moby->save_id);
            mark_moby_for_removal(shuttle_moby);
            return;
        }
        shuttle_vars->riding = 0;
        shuttle_vars->from = shuttle_vars->height;
    }
    gunship_follow_path(shuttle_moby);
    rider = &D_L14_0015FFD8_lift[shuttle_vars->rider];
    if (shuttle_vars->mode == 0x3039) {
        if (shuttle_vars->anchor != -1)
            moby_point_position(shuttle_moby, shuttle_vars->anchor, &base);
        else
            qcopy(&base, &shuttle_moby->pos);
        euler_rotation_matrix(shuttle_rotation_matrix, &shuttle_moby->rot);
        transform_vector(&rotated_rider_offset, shuttle_vars->offset, shuttle_rotation_matrix);
        fast_vec_add(&rider->pos, &base, &rotated_rider_offset);
    }
    if (shuttle_vars->sound == -1 ||
        (voice_is_playing(shuttle_moby, shuttle_vars->sound) == 0 && shuttle_vars->sound == -1))
        shuttle_vars->sound = allocate_voice_for_target_entry(0, 4, shuttle_moby);
    shuttle_vars->angle = fast_add_rotations(shuttle_vars->angle, 0.17453292f);
    FUN_L00_001fff28(shuttle_vars->wobble, 0, shuttle_vars->angle);
    fast_vec_sub(&delta, &shuttle_moby->pos, &old);
    update_entity_matrix(shuttle_vars->spring, &delta, &rot, &shuttle_moby->rot);
    if (hero.state.control_mode == 13 && hero.grapple_target == rider) {
        if (shuttle_vars->riding == 0) {
            shuttle_vars->riding = 1;
            shuttle_vars->timer = scale_game_frames(shuttle_enter_riding_transition_frames_gp);
            shuttle_vars->timer_inv = 1.0f / convert_integer_to_float(shuttle_vars->timer);
        }
    } else {
        if (shuttle_vars->riding == 1) {
            if (hero.air_frames.s == 0 || hero.motion.pos.f[2] <= shuttle_moby->pos.z - 3.0f) {
                shuttle_vars->riding = 0;
                shuttle_vars->timer = scale_game_frames(shuttle_exit_riding_transition_frames_gp);
                shuttle_vars->timer_inv = 1.0f / convert_integer_to_float(shuttle_vars->timer);
                shuttle_vars->to = shuttle_vars->height;
            } else {
                qcopy(&base, &hero.motion.pos);
                fast_vec_sub(&delta, &rider->pos, &base);
                delta.f[2] = 0.0f;
                horizontal_rider_correction_distance = fast_vec_length(&delta);
                max = shuttle_horizontal_rider_correction_limit_scale_gp * frame_time;
                if (max < horizontal_rider_correction_distance)
                    horizontal_rider_correction_distance = max;
                fast_vec_normalize(
                    &hero.motion.platform_delta, &delta, horizontal_rider_correction_distance);
            }
        }
        if (shuttle_vars->riding == 0) {
            fast_dec_timer_s16(&shuttle_vars->timer);
            {
                f32 t = convert_integer_to_float(shuttle_vars->timer) * shuttle_vars->timer_inv;
                shuttle_vars->height =
                    shuttle_vars->from + (shuttle_vars->to - shuttle_vars->from) * t;
            }
            return;
        }
    }
    fast_dec_timer_s16(&shuttle_vars->timer);
    {
        f32 t = convert_integer_to_float(shuttle_vars->timer) * shuttle_vars->timer_inv;
        shuttle_vars->height = (shuttle_vars->from - 0.005f) * t + 0.005f;
    }
}

s32 shuttle_enter_riding_transition_frames_gp __asm__("D_L14_00161B8C") = 0x3c;

s32 shuttle_exit_riding_transition_frames_gp __asm__("D_L14_00161B90") = 0x5a;
