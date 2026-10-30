/* Level 11 moby classes: bob, resort_owner, fred_pokitaru, teleporter, pokitaru_boat. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/item_state.h"
#include "rnc/game/usage_stats.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/weapons/suck_aim.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/002630b8.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/audio/streaming/initialize_npc_dialogue.h"
#include "rnc/overlay/shared/audio/streaming/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/jet_reset_camera.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/audio/streaming/set_moby_dialogue_record_value.h"
#include "rnc/overlay/shared/props/trackdrive_left.h"
#include "rnc/overlay/shared/mobys/build_symmetric_adjacency_masks.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/ui/help/try_set_help_message.h"

/* Moby.state for the resort-owner callbacks; path progression is separate. */
enum ResortOwnerState {
    RESORT_OWNER_INITIALIZE = 0x0,
    RESORT_OWNER_CHECK_INITIAL_DIALOGUE = 0x1,
    RESORT_OWNER_INITIAL_DIALOGUE = 0x2,
    RESORT_OWNER_WAIT_TO_FOLLOW_HERO = 0x3,
    RESORT_OWNER_FOLLOW_HERO = 0x4,
    RESORT_OWNER_FOLLOW_DISTANT_HERO = 0x5,
    RESORT_OWNER_MOVE_TO_PATH_POINT = 0x6,
    RESORT_OWNER_JUMP_TO_TARGET = 0x7,
    RESORT_OWNER_MOVE_TO_GATE_TARGET = 0x8,
    RESORT_OWNER_WAIT_AT_GATE = 0x9,
    RESORT_OWNER_MOVE_TO_GATE_DEPARTURE_TARGET = 0xA,
    RESORT_OWNER_WAIT_TO_ADVANCE_FINAL_PATH_LEG = 0xB,
    RESORT_OWNER_CHECK_PATH_COMPLETION_DIALOGUE = 0xC,
    RESORT_OWNER_WAIT_FOR_PATH_COMPLETION_DIALOGUE_END = 0xD,
    RESORT_OWNER_CHECK_POST_PATH_DIALOGUE = 0xE,
    RESORT_OWNER_WAIT_FOR_POST_PATH_DIALOGUE_END = 0xF
};

/* Resort-owner path traversal, independent of Moby.state. */
enum ResortOwnerPathLeg {
    RESORT_OWNER_FIRST_PATH_LEG = 0x0,
    RESORT_OWNER_SECOND_PATH_LEG = 0x1,
    RESORT_OWNER_PENULTIMATE_PATH_LEG = 0x2,
    RESORT_OWNER_FINAL_PATH_LEG = 0x3
};

/* Per-frame update of a pickup moby: waits for the player to come near, then grants the item. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002CC828), where it is exact; names translated to the US level program. */

extern unsigned char D_0013D4E3[];
void                 mark_moby_for_removal(struct Moby *obj) __asm__("FUN_0020c828");

void update_pokitaru_pickup(struct Moby *moby) __asm__("FUN_L11_002cb668");

void update_pokitaru_pickup(struct Moby *moby) {
    enum {
        PICKUP_INITIALIZE = 0,
        PICKUP_IDLE = 1,
        PICKUP_CHECK_PROXIMITY = 2
    };
    switch (moby->state) {
    case PICKUP_INITIALIZE:
        if (D_0013D4C2[4] != 0) {
            mark_moby_for_removal(moby);
        } else {
            moby->pos.z += 0.75f;
            moby->flags |= 1;
            moby->state = 1;
        }
        break;
    case PICKUP_IDLE:
        break;
    case PICKUP_CHECK_PROXIMITY:
        fast_add_rotations(moby->rot.z, frame_time * 4.7123890f);
        if (vector_distance_xy(&moby->pos, D_0013E533 + 0xE9D) < 2.0f) {
            D_0013D4C2[4] = 1;
            D_0013D4E3[0xB] = 1;
            memcard_save_data(0, -1);
            help_request(0x2AF8, 0x3A);
            mark_moby_for_removal(moby);
        }
        break;
    }
}

#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002CC9D0), where it is exact; names translated to the US level program. */

extern float
approach_angle(float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");

void update_pokitaru_paired_rotation(struct Moby *moby) __asm__("FUN_L11_002cb810");

void update_pokitaru_paired_rotation(struct Moby *moby) {
    enum {
        ROTATION_INITIALIZE = 0,
        ROTATION_WAIT_FOR_TRIGGER = 1,
        ROTATION_APPROACH_OFFSET = 2
    };
    char *data = (char *)moby->pvars;
    int   state = (unsigned char)MOBY(moby)->state;
    switch (state) {
    case ROTATION_INITIALIZE:
        MOBY(moby)->state = 1;
        *(float *)(data + 4) = moby->rot.z;
        if ((moby->flags & 0x8000) == 0) {
            char *partner_moby = create_moby(0x41);
            MOBY(partner_moby)->draw_dist = moby->draw_dist;
            partner_moby[0x30] = moby->update_dist;
            MOBY(partner_moby)->flags = moby->flags | 0x8000;
            qcopy(partner_moby + 0x10, &moby->pos);
            qcopy(partner_moby + 0x40, &moby->rot);
            *(char **)(data + 8) = partner_moby;
            *(float *)(((char *)MOBY(partner_moby)->pvars) + 4) = moby->rot.z;
        }
        break;
    case ROTATION_WAIT_FOR_TRIGGER:
        break;
    case ROTATION_APPROACH_OFFSET: {
        float target_angle;
        if ((moby->flags & 0x8000) == 0)
            target_angle = fast_add_rotations(*(float *)(data + 4), 0.20769417f);
        else
            target_angle = fast_subtract_rotations(*(float *)(data + 4), 0.20769417f);
        if (approach_angle(&moby->rot.z,
                           target_angle,
                           (float *)data,
                           frame_time_sq * 0.34906584f,
                           frame_time_sq * 0.34906584f,
                           frame_time * 0.7853982f) == 0.0f)
            MOBY(moby)->state = 3;
        break;
    }
    }
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002CCB50), where it is exact; names translated to the US level program. */

void trigger_pokitaru_paired_rotation(struct Moby *moby) __asm__("FUN_L11_002cb990");

void trigger_pokitaru_paired_rotation(struct Moby *moby) {
    char *state = (char *)moby->pvars;
    if (moby->oclass == 0x41 && moby->state == 1) {
        char *child;
        D_0013D3E2[0] = moby->state;
        moby->state = 2;
        child = *(char **)(state + 8);
        if (child)
            child[0x20] = 2;
    }
}

typedef struct {
    char          pad00[0x10];
    float         position[4];
    unsigned char state;
    char          pad21[0x27];
    float         yaw;
    char          pad4C[7];
    unsigned char animation;
    char          pad54[0x24];
    char         *data;
} WM11;

typedef struct {
    char  pad00[0x80];
    float position[4];
    char  pad90[0x40];
    float aim[4];
} WP11;

typedef struct {
    float target[4];
    float eye[4];
    float delta[4];
} WS11;

typedef struct {
    char  pad00[4];
    short h04;
    char  pad06[6];
    float f0C;
    char  pad10[0x30];
    int   region;
    char  pad44[0x2C];
    char  body[0x64];
    float pitch;
    float yaw;
    char  padDC[4];
    float height;
    char  padE4[0xC];
    char  head[0x68];
    float head_yaw;
    char  pad15C[0x14];
    float target[4];
    char  pad180[4];
    int   moving_timer;
    int   target_timer;
} WD11;

extern void attach_big_head_cheat_manipulator(void *) __asm__("FUN_L05_00317398");
extern int  FUN_00214720(void *, int);
extern void begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern void clear_active_help_record(void) __asm__("FUN_L00_002039a0");
extern unsigned char D_0013D4C3 NOT_SDA;
extern int                      D_L11_0015F5C4;
extern float                    random_float_between(float, float) __asm__("FUN_002132a8");
extern float                    scale_time(float) __asm__("FUN_001f96b0");
extern void                     FUN_00214db0(void *, float, float, float);
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

/* Watcher update, same routine as commando_update with this level's states and data layout. */
void bob_update(WM11 *m) __asm__("FUN_L11_002d0710");

void bob_update(WM11 *moby) {
    enum {
        BOB_INITIALIZE = 0,
        BOB_CHECK_DIALOGUE = 1,
        BOB_WAIT_FOR_DIALOGUE_END = 2
    };
    WS11  scratch;
    WD11 *moby_vars = (WD11 *)moby->data;
    float rate;
    float head_rate;
    int   tracking;
    attach_big_head_cheat_manipulator(moby);
    switch (moby->state) {
    case BOB_INITIALIZE:
        if (D_0013D4C3) {
            moby->state = 3;
        } else {
            moby->state = 1;
        }
        initialize_npc_dialogue(moby, moby_vars);
        moby_vars->f0C = 3.7f;
        break;
    case BOB_CHECK_DIALOGUE:
        if (FUN_00214720(hero.motion.pos.f, moby_vars->region)) {
            if (update_npc_dialogue(moby, moby_vars)) {
                begin_dialogue_camera_transition(moby, 2.7f);
                moby->state = 2;
            }
        }
        break;
    case BOB_WAIT_FOR_DIALOGUE_END:
        if (D_L11_0015F5C4 != 2) {
            moby->state = 1;
            if (moby_vars->h04 == 2) {
                grant_inventory_item(3, 1);
                jet_reset_camera(hero.motion.pos.f, hero.motion.pos.f + 4);
                memcard_save_data(0, -1);
                clear_active_help_record();
                help_request(0x2AFB, 0x3D);
            }
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->animation == 0) {
        char *player = D_0013E533 + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(moby->position, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->yaw,
                fast_arc_tan(((WP11 *)(D_0013E533 + 0xE1D))->aim[0] - moby->position[0],
                             ((WP11 *)(D_0013E533 + 0xE1D))->aim[1] - moby->position[1])) <
                1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                moby_vars->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&moby_vars->moving_timer);
        } else if (moby_vars->moving_timer) {
            moby_vars->moving_timer = 0;
            qcopy(moby_vars->target, hero.motion.body_center.f);
        }
        if (fast_dec_timer(&moby_vars->target_timer)) {
            float heading;
            moby_vars->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->yaw, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            FUN_00214db0(
                moby_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(moby_vars->target, moby_vars->target, moby->position);
        }
        if (moby_vars->moving_timer) {
            qcopy(scratch.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, moby_vars->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        moby_vars->pitch = pitch;
        moby_vars->head_yaw = moby_vars->yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        moby_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->head, 1);
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002D2168), where it is exact; names translated to the US level program. */

extern float camera_pos_b358[] __asm__("D_L11_001677C0");
extern void  FUN_L00_0025a120(void *);
void         update_resort_owner_state_actions(void *mp) __asm__("FUN_L11_002d1030");

extern void update_resort_owner_state_actions(void *) __asm__("FUN_L11_002d1030");
void        apply_resort_owner_movement(void *mp) __asm__("FUN_L11_002d1340");

extern void apply_resort_owner_movement(void *) __asm__("FUN_L11_002d1340");
extern void update_resort_owner_state_transitions(void *) __asm__("FUN_L11_002d14b0");
void        update_resort_owner_look_tracking(WM11 *moby) __asm__("FUN_L11_002d27b0");

void update_resort_owner_look_tracking(WM11 *m) __asm__("FUN_L11_002d27b0");

void resort_owner_update(struct Moby *moby) __asm__("FUN_L11_002d0fa8");

void resort_owner_update(struct Moby *moby) {
    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    if (moby->visible && fast_vec_dist(&moby->pos, camera_pos_b358) < 38.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 30;
    }
    update_resort_owner_state_actions(moby);
    apply_resort_owner_movement(moby);
    update_resort_owner_state_transitions(moby);
    update_resort_owner_look_tracking(moby);
}

extern int   is_value_within_interpolated_window(void *, float) __asm__("FUN_00214cc8");
extern float FUN_L00_0025abf0(void *, void *, float, int, float);
extern void  transform_attachment_pose(void *, void *, void *, void *, void *, void *) __asm__("FUN_L00_002604f0");
extern void  FUN_00212f90(void *, int, int, int);
extern float D_0015ED70;
extern float D_0015ED6C;
float        D_L11_001613E0 = 3.2f;
int          D_L11_001613F0 = 0;
extern int   resort_owner_completion_rotation_trigger_state[] __asm__("D_L11_0016D260");
extern int   D_L11_0015FFD8;

int try_activate_pokitaru_gate(struct Moby *moby) __asm__("FUN_L11_0030a480");

void update_resort_owner_state_actions(void *mp) {
    struct Moby *moby = mp;
    char        *pvars = (char *)moby->pvars;
    float        target_yaw;
    float        motion_scalar;
    float        vertical_motion;
    float        vertical_motion_decrement;
    float        horizontal_speed;

    switch (moby->state) {
    case RESORT_OWNER_INITIALIZE:
    case RESORT_OWNER_CHECK_INITIAL_DIALOGUE:
    case RESORT_OWNER_INITIAL_DIALOGUE:
    case RESORT_OWNER_WAIT_TO_FOLLOW_HERO:
    case RESORT_OWNER_WAIT_TO_ADVANCE_FINAL_PATH_LEG:
    case RESORT_OWNER_CHECK_PATH_COMPLETION_DIALOGUE:
        break;
    case RESORT_OWNER_FOLLOW_HERO:
    case RESORT_OWNER_FOLLOW_DISTANT_HERO:
    case RESORT_OWNER_MOVE_TO_PATH_POINT:
    case RESORT_OWNER_MOVE_TO_GATE_TARGET:
    case RESORT_OWNER_MOVE_TO_GATE_DEPARTURE_TARGET: {
        char *yaw_ptr = (char *)moby + 0x48;
        target_yaw = fast_arc_tan(*(float *)(pvars + 0x120) - *(float *)((char *)moby + 0x10),
                                  *(float *)(pvars + 0x124) - *(float *)((char *)moby + 0x14));
        approach_angle((float *)(yaw_ptr),
                       target_yaw,
                       (float *)(pvars + 0x150),
                       D_0015ED70 * 12.566371f,
                       D_0015ED70 * 25.132742f,
                       D_0015ED6C * 12.566371f);
        *(float *)(pvars + 0x130) = fast_cos(*(float *)(yaw_ptr)) * *(float *)(pvars + 0x154);
        motion_scalar = fast_sin(*(float *)(yaw_ptr));
        vertical_motion = *(float *)(pvars + 0x148);
        vertical_motion_decrement = D_0015ED70 * 20.0f;
        horizontal_speed = *(float *)(pvars + 0x154);
        *(float *)(pvars + 0x134) = motion_scalar * horizontal_speed;
        vertical_motion = vertical_motion - vertical_motion_decrement;
        *(float *)(pvars + 0x138) = vertical_motion;
        if (0.0f < vertical_motion) {
            *(float *)(pvars + 0x138) = 0.0f;
            /* Fakematch: storing the speed back is a no-op that reload
             * deletes, but it keeps f3 live into this branch, so the
             * first scheduling pass hoists the 0x154 load and the
             * allocator gives it $f4 as in retail. */
            *(float *)(pvars + 0x154) = horizontal_speed;
        }
        break;
    }
    case RESORT_OWNER_JUMP_TO_TARGET:
        if (is_value_within_interpolated_window(moby, 9.0f)) {
            fast_vec_sub(pvars + 0x130, pvars + 0x120, (char *)moby + 0x10);
            motion_scalar = (float)scale_game_frames(0x3C);
            fast_vec_scale(pvars + 0x130, pvars + 0x130, 1.0f / motion_scalar);
            motion_scalar = vector_length_xy(pvars + 0x130);
            *(float *)(pvars + 0x138) = FUN_L00_0025abf0(
                (char *)moby + 0x10, pvars + 0x120, motion_scalar, 0, -(D_0015ED70 * 10.8f));
            *(float *)((char *)moby + 0x58) = 0.6666667f;
        }
        motion_scalar = fast_vec_length(pvars + 0x130);
        if (0.0f < motion_scalar)
            *(float *)(pvars + 0x138) = *(float *)(pvars + 0x138) - D_0015ED70 * 10.8f;
        if (is_value_within_interpolated_window(moby, 29.0f)) {
            clear_u64_value(pvars + 0x130);
            *(float *)((char *)moby + 0x58) = 1.0f;
        }
        break;
    case RESORT_OWNER_WAIT_AT_GATE: {
        int         *gate_moby_index_ptr = (int *)(pvars + 0xF0);
        unsigned int gate_slot_index = 0U < *(unsigned int *)(pvars + 0x158);
        char        *gate_moby;
        gate_moby_index_ptr += gate_slot_index;
        gate_moby = (char *)D_L11_0015FFD8;
        gate_moby += *gate_moby_index_ptr << 8;
        transform_attachment_pose(moby,
                                  gate_moby,
                                  &D_L11_001613E0,
                                  &D_L11_001613F0,
                                  (char *)moby + 0x10,
                                  (char *)moby + 0x40);
        if (try_activate_pokitaru_gate((struct Moby *)gate_moby) == 0)
            break;
    }
        if (moby->prev_seq != 5)
            FUN_00212f90(moby, 5, 0, scale_game_frames(0x14));
        break;
    case RESORT_OWNER_WAIT_FOR_PATH_COMPLETION_DIALOGUE_END:
        if (resort_owner_completion_rotation_trigger_state[12] != 1)
            break;
        if (resort_owner_completion_rotation_trigger_state[13] != scale_game_frames(0x7D0))
            break;
        trigger_pokitaru_paired_rotation(
            (struct Moby *)(D_L11_0015FFD8 + (*(int *)(pvars + 0x104) << 8)));
        break;
    default:
        break;
    }
}

#include "qcopy.h"

/* moves the parts' positions and stores the moby's displacement since last frame */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002D2500), where it is exact; names translated to the US level program. */

extern void FUN_L00_00258830(int, int, float, float, float, int);
extern void
resolve_point_path_segment_xy_clearance(float, void *, void *, void *) __asm__("FUN_L00_00261d78");

void apply_resort_owner_movement(void *mp) {
    unsigned char *moby = mp;
    char          *moby_vars = ((char *)MOBY(moby)->pvars);
    float          A[4];
    float          B[4];
    qcopy(A, moby + 0x10);
    qcopy(B, moby_vars + 0x130);
    FUN_L00_00258830((int)moby, (int)B, 0.5f, 0.3f, 0.0f, 0x10);
    if ((moby[0x20] >= 4 && moby[0x20] <= 6) ||
        (moby[0x20] == 7 && *(int *)(moby_vars + 0x15C) == 3)) {
        resolve_point_path_segment_xy_clearance(
            0.3f,
            *(void **)(moby_vars + 0x60 - -(*(int *)(moby_vars + 0x158) * 4)),
            moby + 0x10,
            moby + 0x10);
        switch (*(int *)(moby_vars + 0x158)) {
        case RESORT_OWNER_PENULTIMATE_PATH_LEG:
            resolve_point_path_segment_xy_clearance(
                0.3f, *(void **)(moby_vars + 0x80), moby + 0x10, moby + 0x10);
            resolve_point_path_segment_xy_clearance(
                0.3f, *(void **)(moby_vars + 0x84), moby + 0x10, moby + 0x10);
            resolve_point_path_segment_xy_clearance(
                0.3f, *(void **)(moby_vars + 0x88), moby + 0x10, moby + 0x10);
            break;
        case RESORT_OWNER_FINAL_PATH_LEG:
            resolve_point_path_segment_xy_clearance(
                0.3f, *(void **)(moby_vars + 0x80), moby + 0x10, moby + 0x10);
            resolve_point_path_segment_xy_clearance(
                0.3f, *(void **)(moby_vars + 0x84), moby + 0x10, moby + 0x10);
            break;
        }
    }
    fast_vec_sub(moby_vars + 0x140, moby + 0x10, A);
}

int classify_resort_owner_target_proximity(struct Moby *moby) __asm__("FUN_L11_002d2460");

extern int classify_resort_owner_target_proximity(struct Moby *moby) __asm__("FUN_L11_002d2460");
int try_update_resort_owner_path_target(struct Moby *moby, char *other) __asm__("FUN_L11_002d2340");

extern int try_update_resort_owner_path_target(struct Moby *moby,
                                               char        *other) __asm__("FUN_L11_002d2340");
int try_find_resort_owner_jump_target(struct Moby *moby, float *out) __asm__("FUN_L11_002d25e8");

extern int   try_find_resort_owner_jump_target(struct Moby *m,
                                               float       *out) __asm__("FUN_L11_002d25e8");
extern void  FUN_L11_002d2088_c(struct Moby *moby, int state) __asm__("FUN_L11_002d2088");
extern float ground_height_14b0(void *) __asm__("FUN_L00_00259710");
extern int   is_moby_in_state_five(struct Moby *arg) __asm__("FUN_L11_0030a830");
extern void  FUN_L11_0030a840(unsigned char *arg);
extern unsigned char D_0013D4EE NOT_SDA;
extern unsigned char D_0013D3DC NOT_SDA;
extern int                      D_0015ED84;
extern unsigned char            D_0014C050[];

#include "rnc/overlay/cuboid.h"

extern int  camera_secondary_mode __asm__("D_001413D4") NOT_SDA;
extern char resort_owner_completion_dialogue_pose[] __asm__("D_L11_0016D270");

#include "rnc/overlay/l11/mobys/bob_globals.h"

extern float resort_owner_gate_jump_target_local_z_offset_gp __asm__("D_L11_00161410")
    __attribute__((sda));
extern float resort_owner_gate_jump_target_local_y_offset_gp __asm__("D_L11_00161414")
    __attribute__((sda));
extern float resort_owner_gate_jump_target_local_x_offset_gp __asm__("D_L11_00161418")
    __attribute__((sda));

/* Pvars of the path-walking NPC run by resort_owner_update. */
struct PokKeeperVars {
    u8    pad00[0x20];
    u8    talk[0xC];  /* 0x20: talk prompt (initialize_npc_dialogue / update_npc_dialogue) */
    f32   talk_range; /* 0x2C */
    u8    pad30[0x26];
    s16   unk56;
    u8    pad58[8];
    s32   unk60[3]; /* 0x60: FUN_L05_0028be78 handles, set from unk70[] */
    u8    pad6C[4];
    s32   unk70[3];
    u8    pad7C[0x54];
    s32   unkD0[4];
    s32   points[4];  /* 0xE0: path point of each leg */
    s32   gates[2];   /* 0xF0: gate mobys */
    s32   volumes[2]; /* 0xF8 */
    s32   unk100;
    s32   unk104;
    s32   unk108;
    s32   unk10C;
    s32   unk110;
    s32   unk114;
    s32   unk118;
    u8    pad11C[4];
    Vec4f target; /* 0x120 */
    Vec4f unk130;
    Vec4f unk140;
    u8    pad150[8];
    s32   leg;        /* 0x158 */
    s32   next_state; /* 0x15C: state entered once the walk animation ends */
};

/* NPC that talks to the hero, then walks a chain of level path points ahead of him, opening the
   gate mobys (FUN_L11_0030a840) and moving path points as it goes. */
void update_resort_owner_state_transitions(void *resort_owner_moby) {
    struct Moby          *moby = resort_owner_moby;
    struct PokKeeperVars *moby_vars = (struct PokKeeperVars *)moby->pvars;
    int                   proximity = classify_resort_owner_target_proximity(moby);
    float                 position_scratch[4];
    float                 dialogue_rotation[4];

    if (moby->state >= 3 && moby->state <= 5 && proximity == 0) {
        FUN_L11_002d2088_c(moby, 6);
        return;
    }
    if (moby->state >= 4 && moby->state <= 6) {
        fast_vec_normalize(position_scratch, &moby->rot_row0, 3.0f);
        fast_vec_add(position_scratch, position_scratch, &moby->pos);
        if (try_find_resort_owner_jump_target(moby, position_scratch)) {
            qcopy(&moby_vars->target, position_scratch);
            moby_vars->target.z = ground_height_14b0(&moby_vars->target);
            moby_vars->next_state = 3;
            FUN_L11_002d2088_c(moby, 7);
        }
    }
    switch (moby->state) {
    case RESORT_OWNER_INITIALIZE:
        initialize_npc_dialogue(moby, moby_vars->talk);
        moby->update_dist = 0xFF;
        build_symmetric_adjacency_masks(&moby_vars->unk60[0], 1, moby_vars->unk70[0]);
        build_symmetric_adjacency_masks(&moby_vars->unk60[1], 1, moby_vars->unk70[1]);
        build_symmetric_adjacency_masks(&moby_vars->unk60[2], 1, moby_vars->unk70[2]);
        clear_u64_value(&moby_vars->unk130);
        clear_u64_value(&moby_vars->unk140);
        if (D_0013D4EE) {
            if (D_0014C050[moby_vars->unk10C + D_0015ED84 * 16] == 0xFF) {
                FUN_L11_0030a840((unsigned char *)(D_L11_0015FFD8 + (moby_vars->gates[0] << 8)));
                FUN_L11_0030a840((unsigned char *)(D_L11_0015FFD8 + (moby_vars->gates[1] << 8)));
                trigger_pokitaru_paired_rotation(
                    (struct Moby *)(D_L11_0015FFD8 + (moby_vars->unk104 << 8)));
                mark_moby_for_removal(moby);
                break;
            }
        } else {
            if (D_0014C050[moby_vars->unk10C + D_0015ED84 * 16] == 0xFF) {
                moby_vars->leg = 3;
                qcopy_nc(&moby->pos, &l11_cuboids[moby_vars->points[3]].pos);
                qcopy_nc(&moby->rot, &l11_cuboids[moby_vars->points[3]].rot);
                trigger_pokitaru_paired_rotation(
                    (struct Moby *)(D_L11_0015FFD8 + (moby_vars->unk104 << 8)));
                FUN_L11_002d2088_c(moby, 0xE);
                moby_vars->talk_range = 10.0f;
                FUN_L11_0030a840((unsigned char *)(D_L11_0015FFD8 + (moby_vars->gates[0] << 8)));
                FUN_L11_0030a840((unsigned char *)(D_L11_0015FFD8 + (moby_vars->gates[1] << 8)));
                break;
            }
        }
        if (D_0014C050[moby_vars->unk108 + D_0015ED84 * 16] == 0xFF &&
            fast_vec_dist(&hero.motion.pos, &l11_cuboids[moby_vars->points[1]].pos) < 12.0f) {
            moby_vars->leg = 1;
            qcopy_nc(&moby->pos, &l11_cuboids[moby_vars->points[1]].pos);
            qcopy_nc(&moby->rot, &l11_cuboids[moby_vars->points[1]].rot);
            FUN_L11_002d2088_c(moby, 6);
            l11_cuboids[moby_vars->unk114].pos.z -= 500.0f;
            FUN_L11_0030a840((unsigned char *)(D_L11_0015FFD8 + (moby_vars->gates[0] << 8)));
            break;
        }
        l11_cuboids[moby_vars->unk110].pos.z -= 500.0f;
        l11_cuboids[moby_vars->unk114].pos.z -= 500.0f;
        moby_vars->leg = 0;
        moby_vars->talk_range = 5.0f;
        FUN_L11_002d2088_c(moby, 1);
        break;
    case RESORT_OWNER_CHECK_INITIAL_DIALOGUE:
        if (moby_vars->unk56 == 0) {
            if (hero.air_frames.s == 0 && update_npc_dialogue(moby, moby_vars->talk)) {
                begin_dialogue_camera_transition(moby, 2.5f);
                FUN_L11_002d2088_c(moby, 2);
            }
        } else if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 6.0f) {
            FUN_L11_002d2088_c(moby, 3);
        }
        break;
    case RESORT_OWNER_INITIAL_DIALOGUE:
        if (D_L11_0015F5C4 != 2)
            FUN_L11_002d2088_c(moby, 3);
        break;
    case RESORT_OWNER_WAIT_TO_FOLLOW_HERO:
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) > 6.0f &&
            try_update_resort_owner_path_target(moby, (char *)&hero.motion.pos) && proximity == 1)
            FUN_L11_002d2088_c(moby, 4);
        break;
    case RESORT_OWNER_FOLLOW_HERO:
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 3.0f ||
            !try_update_resort_owner_path_target(moby, (char *)&hero.motion.pos) || proximity == 2)
            FUN_L11_002d2088_c(moby, 3);
        else if (vector_distance_xy(&moby->pos, &hero.motion.pos) > 8.0f)
            FUN_L11_002d2088_c(moby, 5);
        break;
    case RESORT_OWNER_FOLLOW_DISTANT_HERO:
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 3.0f ||
            !try_update_resort_owner_path_target(moby, (char *)&hero.motion.pos) || proximity == 2)
            FUN_L11_002d2088_c(moby, 3);
        else if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 4.0f)
            FUN_L11_002d2088_c(moby, 4);
        break;
    case RESORT_OWNER_MOVE_TO_PATH_POINT:
        try_update_resort_owner_path_target(
            moby, (char *)&l11_cuboids[moby_vars->points[moby_vars->leg]].pos);
        if (vector_distance_xy(&moby->pos, &moby_vars->target) < 1.0f) {
            switch (moby_vars->leg) {
            case RESORT_OWNER_FIRST_PATH_LEG: {
                struct Moby *o = (struct Moby *)(D_L11_0015FFD8 + (moby_vars->gates[0] << 8));
                qcopy(&moby_vars->target, &o->pos);
                fast_vec_normalize(position_scratch,
                                   &o->rot_row0,
                                   resort_owner_gate_jump_target_local_x_offset_gp);
                fast_vec_add(&moby_vars->target, &moby_vars->target, position_scratch);
                fast_vec_normalize(position_scratch,
                                   &o->rot_row1,
                                   resort_owner_gate_jump_target_local_y_offset_gp);
                fast_vec_add(&moby_vars->target, &moby_vars->target, position_scratch);
                fast_vec_normalize(position_scratch,
                                   &o->rot_row2,
                                   resort_owner_gate_jump_target_local_z_offset_gp);
                fast_vec_add(&moby_vars->target, &moby_vars->target, position_scratch);
                moby_vars->next_state = 8;
                FUN_L11_002d2088_c(moby, 7);
                break;
            }
            case RESORT_OWNER_SECOND_PATH_LEG: {
                struct Moby *o = (struct Moby *)(D_L11_0015FFD8 + (moby_vars->gates[1] << 8));
                qcopy(&moby_vars->target, &o->pos);
                fast_vec_normalize(position_scratch,
                                   &o->rot_row0,
                                   resort_owner_gate_jump_target_local_x_offset_gp);
                fast_vec_add(&moby_vars->target, &moby_vars->target, position_scratch);
                fast_vec_normalize(position_scratch,
                                   &o->rot_row1,
                                   -resort_owner_gate_jump_target_local_y_offset_gp);
                fast_vec_add(&moby_vars->target, &moby_vars->target, position_scratch);
                fast_vec_normalize(position_scratch,
                                   &o->rot_row2,
                                   resort_owner_gate_jump_target_local_z_offset_gp);
                fast_vec_add(&moby_vars->target, &moby_vars->target, position_scratch);
                l11_cuboids[moby_vars->unk110].pos.z += 500.0f;
                moby_vars->next_state = 8;
                FUN_L11_002d2088_c(moby, 7);
                break;
            }
            case RESORT_OWNER_PENULTIMATE_PATH_LEG:
                FUN_L11_002d2088_c(moby, 0xB);
                break;
            case RESORT_OWNER_FINAL_PATH_LEG:
                l11_cuboids[moby_vars->unk114].pos.z += 500.0f;
                FUN_L11_002d2088_c(moby, 0xC);
                break;
            }
        }
        break;
    case RESORT_OWNER_CHECK_PATH_COMPLETION_DIALOGUE:
        moby_vars->talk_range = 255.0f;
        if (update_npc_dialogue(moby, moby_vars->talk)) {
            jet_clear_persistent(moby_vars->unk10C);
            D_0013D3DC = 1;
            set_moby_dialogue_record_value(moby, 2);
            qcopy_nc(&moby->pos, &l11_cuboids[moby_vars->points[moby_vars->leg]].pos);
            qcopy_nc(&moby->rot, &l11_cuboids[moby_vars->points[moby_vars->leg]].rot);
            euler_rotation_basis(&moby->rot_row0, &moby->rot);
            fast_vec_normalize(position_scratch, &moby->rot_row0, 2.0f);
            fast_vec_add(position_scratch, position_scratch, &moby->pos);
            qcopy(dialogue_rotation, &moby->rot);
            dialogue_rotation[2] = fast_add_rotations(dialogue_rotation[2], 3.14159f);
            qcopy(resort_owner_completion_dialogue_pose, position_scratch);
            qcopy(resort_owner_completion_dialogue_pose + 0x10, dialogue_rotation);
            *(short *)(resort_owner_completion_dialogue_pose + 0x36) = 1;
            begin_dialogue_camera_transition(moby, 2.5f);
            FUN_L11_002d2088_c(moby, 0xD);
        }
        break;
    case RESORT_OWNER_WAIT_FOR_PATH_COMPLETION_DIALOGUE_END:
        if (D_L11_0015F5C4 != 2) {
            trigger_pokitaru_paired_rotation(
                (struct Moby *)(D_L11_0015FFD8 + (moby_vars->unk104 << 8)));
            FUN_L11_002d2088_c(moby, 0xE);
        }
        break;
    case RESORT_OWNER_JUMP_TO_TARGET:
        if (moby->anim_flags & 2)
            FUN_L11_002d2088_c(moby, moby_vars->next_state);
        break;
    case RESORT_OWNER_MOVE_TO_GATE_TARGET:
        if (vector_distance_xy(&moby->pos, &moby_vars->target) < 0.5f)
            FUN_L11_002d2088_c(moby, 9);
        break;
    case RESORT_OWNER_WAIT_AT_GATE: {
        int i = moby_vars->leg != 0;
        if (is_moby_in_state_five((struct Moby *)(D_L11_0015FFD8 + (moby_vars->gates[i] << 8))))
            FUN_L11_002d2088_c(moby, 0xA);
    } break;
    case RESORT_OWNER_MOVE_TO_GATE_DEPARTURE_TARGET:
        if (vector_distance_xy(&moby->pos, &moby_vars->target) < 0.5f) {
            int next_leg_index = ++moby_vars->leg;
            qcopy(&moby_vars->target, &l11_cuboids[moby_vars->unkD0[next_leg_index]].pos);
            moby_vars->next_state = 4;
            FUN_L11_002d2088_c(moby, 7);
        }
        break;
    case RESORT_OWNER_WAIT_TO_ADVANCE_FINAL_PATH_LEG:
        if (is_moby_in_state_five((struct Moby *)(D_L11_0015FFD8 + (moby_vars->unk100 << 8)))) {
            moby_vars->leg += 1;
            FUN_L11_002d2088_c(moby, 3);
        }
        break;
    case RESORT_OWNER_CHECK_POST_PATH_DIALOGUE:
        if (D_0014C050[moby_vars->unk118 + D_0015ED84 * 16] == 0xFF &&
            camera_secondary_mode != 0x32 && update_npc_dialogue(moby, moby_vars->talk)) {
            begin_dialogue_camera_transition(moby, 2.5f);
            FUN_L11_002d2088_c(moby, 0xF);
        }
        break;
    case RESORT_OWNER_WAIT_FOR_POST_PATH_DIALOGUE_END:
        if (D_L11_0015F5C4 != 2) {
            grant_inventory_item(6, 1);
            memcard_save_data(0, -1);
            help_request(0x2AF8, 0x3A);
            mark_moby_for_removal(moby);
        }
        break;
    }
}

extern float resort_owner_follow_hero_speed_gp __asm__("D_L11_001613D0") __attribute__((sda));
extern float resort_owner_distant_follow_and_path_speed_gp __asm__("D_L11_001613D4")
    __attribute__((sda));
float D_L11_00161400 = -3.0f;

void set_resort_owner_state(struct Moby *moby, unsigned char state) __asm__("FUN_L11_002d2088");

void set_resort_owner_state(struct Moby *moby, unsigned char state) {
    char *moby_vars = (char *)moby->pvars;
    float gate_target_offset[4];

    moby->state = state;
    switch (moby->state) {
    case RESORT_OWNER_CHECK_INITIAL_DIALOGUE:
    case RESORT_OWNER_WAIT_TO_FOLLOW_HERO:
    case RESORT_OWNER_WAIT_TO_ADVANCE_FINAL_PATH_LEG:
    case RESORT_OWNER_CHECK_PATH_COMPLETION_DIALOGUE:
    case RESORT_OWNER_CHECK_POST_PATH_DIALOGUE:
        if (moby->prev_seq != 0)
            FUN_00212f90(moby, 0, 0, scale_game_frames(0x14));
        if (moby->state == 14)
            *(float *)(moby_vars + 0x2C) = 10.0f;
        clear_u64_value(moby_vars + 0x130);
        if (moby->state == 14)
            *(short *)(moby_vars + 0x56) = 2;
        break;
    case RESORT_OWNER_FOLLOW_HERO:
        if (moby->prev_seq != 2)
            FUN_00212f90(moby, 2, 0, scale_game_frames(0x14));
        *(float *)(moby_vars + 0x154) = resort_owner_follow_hero_speed_gp * frame_time;
        break;
    case RESORT_OWNER_FOLLOW_DISTANT_HERO:
    case RESORT_OWNER_MOVE_TO_PATH_POINT:
        if (moby->prev_seq != 3)
            FUN_00212f90(moby, 3, 0, scale_game_frames(0x14));
        *(float *)(moby_vars + 0x154) = resort_owner_distant_follow_and_path_speed_gp * frame_time;
        break;
    case RESORT_OWNER_JUMP_TO_TARGET:
        if (moby->prev_seq != 4)
            FUN_00212f90(moby, 4, 0, scale_game_frames(0x14));
        clear_u64_value(moby_vars + 0x130);
        break;
    case RESORT_OWNER_MOVE_TO_GATE_TARGET:
        if (moby->prev_seq != 3)
            FUN_00212f90(moby, 3, 0, scale_game_frames(0x14));
        {
            int         *tbl = (int *)(moby_vars + 0xF0);
            unsigned int idx = 0U < *(unsigned int *)(moby_vars + 0x158);
            int          p;
            char        *o;
            tbl += idx;
            p = *tbl;
            o = (char *)D_L11_0015FFD8;
            o += p << 8;
            qcopy(moby_vars + 0x120, o + 0x10);
            transform_vector_by_basis(gate_target_offset, &D_L11_001613E0, o + 0xC0);
            fast_vec_add(moby_vars + 0x120, moby_vars + 0x120, gate_target_offset);
        }
        break;
    case RESORT_OWNER_WAIT_AT_GATE:
    case 16:
        if (moby->prev_seq != 0)
            FUN_00212f90(moby, 0, 0, scale_game_frames(0x14));
        clear_u64_value(moby_vars + 0x130);
        break;
    case RESORT_OWNER_MOVE_TO_GATE_DEPARTURE_TARGET:
        if (moby->prev_seq != 3)
            FUN_00212f90(moby, 3, 0, scale_game_frames(0x14));
        {
            int         *tbl = (int *)(moby_vars + 0xF0);
            unsigned int idx = 0U < *(unsigned int *)(moby_vars + 0x158);
            int          p;
            char        *o;
            tbl += idx;
            p = *tbl;
            o = (char *)D_L11_0015FFD8;
            o += p << 8;
            qcopy(moby_vars + 0x120, o + 0x10);
            transform_vector_by_basis(gate_target_offset, &D_L11_00161400, o + 0xC0);
            fast_vec_add(moby_vars + 0x120, moby_vars + 0x120, gate_target_offset);
        }
        break;
    default:
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002D3500), where it is exact; names translated to the US level program. */

struct TgtData {
    char pad0[0x60];
    int  a[4];
    int  b[4];
    int  c[4];
    char pad1[0x90];
    char e[0x38];
    int  idx;
};

#include "rnc/overlay/level_path.h"

extern int point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern int follow_path(int *, int, int, void *, void *, void *, float) __asm__("FUN_L01_00276fe8");

int try_update_resort_owner_path_target(struct Moby *moby, char *other) {
    struct TgtData *moby_vars = (struct TgtData *)moby->pvars;
    int            *leg_path_ids = moby_vars->a;
    int *ent = (int *)l11_paths[*(int *)((char *)leg_path_ids - (-(moby_vars->idx * 4)))];
    int  path_ids[8];
    int  path_id_count;
    if (point_in_path_xy(other, ent + 4, *ent) == 0)
        return 0;
    path_ids[0] = *(int *)((char *)leg_path_ids - (-(moby_vars->idx * 4)));
    path_id_count = 1;
    switch (moby_vars->idx) {
    case RESORT_OWNER_PENULTIMATE_PATH_LEG:
        path_id_count = 4;
        path_ids[1] = moby_vars->c[0];
        path_ids[2] = moby_vars->c[1];
        path_ids[3] = moby_vars->c[2];
        break;
    case RESORT_OWNER_FINAL_PATH_LEG:
        path_id_count = 3;
        path_ids[1] = moby_vars->c[0];
        path_ids[2] = moby_vars->c[1];
        break;
    default:
        break;
    }
    return follow_path(path_ids,
                       path_id_count,
                       moby_vars->b[moby_vars->idx],
                       &moby->pos,
                       other,
                       moby_vars->e,
                       0.2f) != 0;
}

extern int    is_moby_in_bob_target_exclusion_state(char *) __asm__("FUN_L11_00316128");
extern short *D_L11_001AC240[];
extern int    D_L11_0015FFF4;

struct TargetSets {
    char pad0[0x90];
    struct {
        short group;
        short pad;
    } sets[9][4];
    char pad120[0x38];
    int  set;
};

/* 2 when a live moby of the current target groups is within 10 units, 1 when one is alive, else 0. */
int classify_resort_owner_target_proximity(struct Moby *moby) {
    struct TargetSets *target_sets = (struct TargetSets *)moby->pvars;
    int                found = 0;
    int                i;
    int                group;
    short             *group_moby_entry;

    for (i = 0; i < 4; i++) {
        group = target_sets->sets[target_sets->set][i].group;
        if (group < 0 || group > D_L11_0015FFF4)
            break;
        group_moby_entry = D_L11_001AC240[group];
        if (group_moby_entry == 0)
            continue;
        do {
            int          offset = (*group_moby_entry & 0x7fff) << 8;
            struct Moby *other = (struct Moby *)(offset + D_L11_0015FFD8);
            if ((signed char)other->state >= 0) {
                if (other->oclass != 0x4de ||
                    is_moby_in_bob_target_exclusion_state((char *)other) == 0) {
                    found = 1;
                    if (fast_vec_dist(&moby->pos,
                                      &((struct Moby *)(D_L11_0015FFD8 + offset))->pos) < 10.0f)
                        return 2;
                }
            }
        } while (*group_moby_entry++ >= 0);
    }
    return found;
}
/* Tests whether a moby can drop onto the ground, writing the landing point. */
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002D37A8), where it is exact; names translated to the US level program. */

extern int   collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
void         FUN_L00_00259710(void *a);
extern float func_L00_0025A748_f(void *) __asm__("FUN_L00_00259710");

int try_find_resort_owner_jump_target(struct Moby *moby, float *out) {
    char *pvars = (char *)moby->pvars;
    int  *clip_volume_index_ptr = (int *)(pvars + 0xF8);
    int   i;
    int   found = 0;
    float candidate_position[4];
    float collision_start_position[4];
    float landing_ground_height;
    for (i = 0; i < 2; i++) {
        if (is_point_inside_clip_volume(&moby->pos, *clip_volume_index_ptr)) {
            found = 1;
            break;
        }
        clip_volume_index_ptr++;
    }
    if (found) {
        float *pos = &moby->pos.x;
        fast_vec_sub(candidate_position, out, pos);
        if (fast_vec_length(candidate_position) > 4.0f)
            fast_vec_normalize(candidate_position, candidate_position, 4.0f);
        fast_vec_add(candidate_position, candidate_position, pos);
        {
            int *path_entry =
                (int *)l11_paths[*(int *)(pvars - (-(*(int *)(pvars + 0x158) * 4)) + 0x60)];
            if (point_in_path_xy(candidate_position, path_entry + 4, *path_entry)) {
                candidate_position[2] += 5.0f;
                landing_ground_height = func_L00_0025A748_f(candidate_position);
                if (absolute_float(landing_ground_height - moby->pos.z) > 0.5f) {
                    qcopy(collision_start_position, pos);
                    collision_start_position[2] += 0.15f;
                    candidate_position[2] = landing_ground_height + 0.15f;
                    if (collision_line(collision_start_position, candidate_position, 2, moby, 0)) {
                        candidate_position[2] = landing_ground_height;
                        qcopy(out, candidate_position);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

typedef struct {
    char  pad000[0x160];
    char  body[0x64];
    float pitch;
    float yaw;
    char  pad1CC[4];
    float height;
    char  pad1D4[0xC];
    char  head[0x68];
    float head_yaw;
} WD11b;

/* Guard head update: in its watching states it turns its body and head toward the hero. */
void update_resort_owner_look_tracking(WM11 *moby) {
    WS11   scratch;
    WD11b *moby_vars = (WD11b *)moby->data;
    float  rate;
    float  head_rate;
    int    tracking;
    rate = 0.02f;
    head_rate = 0.3f;
    switch (moby->state) {
    case RESORT_OWNER_INITIALIZE:
    case RESORT_OWNER_CHECK_INITIAL_DIALOGUE:
    case RESORT_OWNER_INITIAL_DIALOGUE:
    case RESORT_OWNER_WAIT_TO_FOLLOW_HERO:
    case RESORT_OWNER_FOLLOW_HERO:
    case RESORT_OWNER_FOLLOW_DISTANT_HERO:
    case RESORT_OWNER_WAIT_TO_ADVANCE_FINAL_PATH_LEG:
    case RESORT_OWNER_CHECK_PATH_COMPLETION_DIALOGUE:
    case RESORT_OWNER_CHECK_POST_PATH_DIALOGUE:
    case 16:
        tracking = 1;
        qcopy(scratch.target, &hero.motion.pos);
        break;
    default:
        tracking = 0;
        break;
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, moby->position);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw = fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), moby->yaw);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.2217305f)
            yaw = 1.2217305f;
        else if (yaw < -1.2217305f)
            yaw = -1.2217305f;
        if (pitch > 0.2617994f)
            pitch = 0.2617994f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        moby_vars->pitch = pitch;
        moby_vars->yaw = yaw * 0.7f;
        moby_vars->head_yaw = yaw * 0.3f;
    }
    if (big_head_cheat_enabled)
        moby_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->head, 1);
}

/* Pokitaru sentry pvars (class of fred_pokitaru_update). */
struct PokSentryVars {
    char  pad00[4];
    short h04;
    char  pad06[6];
    float f0C;
    char  pad10[0x38];
    int   slot;
    char  pad4C[4];
    char  body[0x64];
    float pitch;
    float yaw;
    char  padBC[4];
    float height;
    char  padC4[0xC];
    char  head[0x68];
    float head_yaw;
    char  pad13C[0x14];
    float target[4];
    int   moving_timer;
    int   target_timer;
};

extern unsigned char D_0013D50B NOT_SDA;
extern struct UsageStat         help_records[] __asm__("D_00141968") NOT_SDA;

void fred_pokitaru_update(struct Moby *moby) __asm__("FUN_L11_002f0e40");

void fred_pokitaru_update(struct Moby *moby) {
    enum {
        FRED_POKITARU_INITIALIZE = 0,
        FRED_POKITARU_CHECK_DIALOGUE = 1,
        FRED_POKITARU_WAIT_FOR_DIALOGUE_END = 2
    };
    WS11                  scratch;
    struct PokSentryVars *moby_vars = (struct PokSentryVars *)moby->pvars;
    float                 rate;
    float                 head_rate;
    int                   tracking;

    attach_big_head_cheat_manipulator_to_matching_moby(moby);
    if (moby->visible && fast_vec_dist(&moby->pos, camera_pos_b358) < 30.0f) {
        FUN_L00_0025a120(moby);
        moby->shadow_range = 24;
    }
    switch (moby->state) {
    case FRED_POKITARU_INITIALIZE:
        moby->state = 1;
        if (moby->prev_seq != 1)
            FUN_00212f90(moby, 1, 0, scale_game_frames(20));
        if (D_0013D50B) {
            mark_moby_for_removal(moby);
            return;
        }
        initialize_npc_dialogue(moby, moby_vars);
        moby_vars->f0C = 4.0f;
        break;
    case FRED_POKITARU_CHECK_DIALOGUE:
        if (update_npc_dialogue(moby, moby_vars)) {
            begin_dialogue_camera_transition(moby, 3.0f);
            moby->state = 2;
        }
        if (alternate_item_available[1] == 0 && discount_purchase_pricing[0] == 0) {
            if (fast_vec_dist(&moby->pos, hero.motion.pos.f) < 4.0f) {
                int help_frame_delta =
                    scale_game_frames(D_0015EEA4) - help_records[0x86].unk2 * 600;
                if ((int)(scale_game_frames(18) * 60.0f) < help_frame_delta ||
                    help_records[0x86].unk2 * 600 == 0) {
                    help_request(0x2B03, 0x86);
                } else if (scale_game_frames(D_0015EEA4) / 600 > help_records[0x86].unk2) {
                    help_records[0x86].unk2 = scale_game_frames(D_0015EEA4) / 600;
                }
            }
        }
        if (D_0014C050[moby->mission + D_0015ED84 * 16] != 0xFF &&
            fast_vec_dist(hero.motion.pos.f, &moby->pos) < 4.0f &&
            absolute_float(hero.motion.pos.f[2] - moby->pos.z) < 4.0f) {
            char *entry;
            jet_clear_persistent(moby->mission);
            entry = (char *)l11_cuboids + moby_vars->slot * 128;
            jet_reset_camera(entry + 0x30, entry + 0x70);
        }
        break;
    case FRED_POKITARU_WAIT_FOR_DIALOGUE_END:
        if (D_L11_0015F5C4 != 2) {
            moby->state = 1;
            if (moby_vars->h04 == 2) {
                alternate_item_available[1] = 0;
                discount_purchase_pricing[0] = 1;
                D_0013D50B = 1;
                queue_dialogue_message(0x2B06, scale_game_frames(300));
                memcard_save_data(0, -1);
                help_request(0x2AF9, 0x3B);
            }
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    if (moby->prev_seq == 1) {
        char *player = D_0013E533 + 0xE9D;
        tracking = 1;
        if (vector_distance_xy(&moby->pos, player) < 8.0f &&
            fast_difference_between_rotations(
                moby->rot.z,
                fast_arc_tan(((WP11 *)(D_0013E533 + 0xE1D))->aim[0] - moby->pos.x,
                             ((WP11 *)(D_0013E533 + 0xE1D))->aim[1] - moby->pos.y)) < 1.5707964f) {
            if (fast_vec_length(player + 0x80) > 0.01f)
                moby_vars->moving_timer = scale_game_frames(120);
            else
                fast_dec_timer(&moby_vars->moving_timer);
        } else if (moby_vars->moving_timer) {
            moby_vars->moving_timer = 0;
            qcopy(moby_vars->target, hero.motion.body_center.f);
        }
        if (fast_dec_timer(&moby_vars->target_timer)) {
            float heading;
            moby_vars->target_timer =
                truncate_float_to_s32(scale_time(random_float_between(180.0f, 300.0f)));
            heading =
                fast_add_rotations(moby->rot.z, random_float_between(-90.0f, 90.0f) * DEG_TO_RAD);
            FUN_00214db0(
                moby_vars->target, 6.0f, heading, random_float_between(0.0f, 30.0f) * DEG_TO_RAD);
            fast_vec_add(moby_vars->target, moby_vars->target, &moby->pos);
        }
        if (moby_vars->moving_timer) {
            qcopy(scratch.target, hero.motion.body_center.f);
            rate = 0.04f;
            head_rate = 0.3f;
        } else {
            qcopy(scratch.target, moby_vars->target);
        }
    }
    if (tracking) {
        float yaw;
        float pitch;
        qcopy(scratch.eye, &moby->pos);
        scratch.eye[2] += 1.0f;
        fast_vec_sub(scratch.delta, scratch.target, scratch.eye);
        yaw =
            fast_subtract_rotations(fast_arc_tan(scratch.delta[0], scratch.delta[1]), moby->rot.z);
        pitch = -fast_arc_tan(vector_length_xy(scratch.delta), scratch.delta[2]);
        if (yaw > 1.5707964f)
            yaw = 1.5707964f;
        else if (yaw < -1.5707964f)
            yaw = -1.5707964f;
        if (pitch > 0.5235988f)
            pitch = 0.5235988f;
        else if (pitch < -0.5235988f)
            pitch = -0.5235988f;
        moby_vars->pitch = pitch;
        moby_vars->head_yaw = moby_vars->yaw = yaw * 0.5f;
    }
    if (big_head_cheat_enabled)
        moby_vars->height = 2.75f;
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->body, 0);
    update_joint_animation(
        rate * frame_scale_sq, head_rate * frame_scale_sq, moby, moby_vars->head, 1);
}

#include "qcopy.h"

extern float         D_0013F3D8_lui __asm__("D_0013F3D8") NOT_SDA; /* hero z */
extern int           current_level_index __asm__("D_0015ED84");
extern unsigned char D_0014C050_2518[] __asm__("D_0014C050");
extern int           D_0013CAE4_lui __asm__("D_0013CAE4") NOT_SDA;
extern int           D_L11_0015F594 __asm__("D_L11_0015F594");
extern char         *D_L11_0015FFD8_2518 __asm__("D_L11_0015FFD8");
extern unsigned char D_001413F5_lui __asm__("D_001413F5") NOT_SDA;
extern float         D_0015ED6C_2518 __asm__("D_0015ED6C");
extern float         D_0015ED70_2518 __asm__("D_0015ED70");
extern void          FUN_L11_002f3040_2518(char *) __asm__("FUN_L11_002f3040");
extern void          FUN_L11_002f2cd0_2518(char *) __asm__("FUN_L11_002f2cd0");
extern void          FUN_L11_002f2d58_2518(char *) __asm__("FUN_L11_002f2d58");
extern void          update_teleporter_child_transforms(char *) __asm__("FUN_L01_003092d0");
extern void          request_hero_target_pose(float *, int, float) __asm__("FUN_L00_00233ee8");
extern void FUN_L00_002598b0_2518(int, float, void *, int, float, float, int, int, int) __asm__("FUN_L00_002598b0");
extern int get_hero_target_pose_status(void) __asm__("FUN_L00_00233f38");
extern void
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void FUN_L00_00216f90_2518(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void FUN_L01_0027a248_2518(int, int) __asm__("FUN_L01_0027a248");
extern int  add_draw_callback(void (*)(char *), char *) __asm__("FUN_001f4600");

extern float teleporter_parts_acceleration_gp __asm__("D_L11_00161910") __attribute__((sda));
extern float teleporter_parts_deceleration_gp __asm__("D_L11_00161914") __attribute__((sda));
extern float teleporter_parts_max_speed_gp __asm__("D_L11_00161918") __attribute__((sda));
/* L11 twin of teleporter_update: a three-part switch moby that, once the hero
 * stands on it in mission state 0xB and presses the button, swings its parts
 * open and moves the linked moby (or its fallback) along. */
void teleporter_update(struct Moby *moby) __asm__("FUN_L11_002f2518");

void teleporter_update(struct Moby *moby) {
    enum {
        TELEPORTER_INITIALIZE = 0,
        TELEPORTER_WAIT_FOR_ACTIVATION = 1,
        TELEPORTER_OPEN_PARTS = 2,
        TELEPORTER_CONTRACT_RING = 3,
        TELEPORTER_RESET = 4
    };
    char *data = (char *)moby->pvars;

    switch (moby->state) {
    case TELEPORTER_INITIALIZE: {
        char **slots = (char **)(data + 0x14);
        int    i;
        int    j;
        moby->state = 1;
        for (i = 0; i < 3; i++) {
            struct Hero *g = &hero;
            char        *child_moby;
            slots[i] = create_moby(0x13B);
            *(short *)(slots[i] + 0x32) = 0x40;
            slots[i][0x31] = 1;
            *(long *)(slots[i] + 0x38) = *(long *)(((char *)g->moby) + 0x38);
            *(unsigned short *)(slots[i] + 0x34) = moby->flags;
            child_moby = slots[i];
            qcopy(child_moby + 0x10, &moby->pos);
            qcopy(child_moby + 0x40, &moby->rot);
            *(float *)(slots[i] + 0x48) =
                fast_add_rotations(MOBY(child_moby)->rot.z, (float)i * 2.0943952f);
        }
        if (*(int *)(data + 8) >= 0 &&
            D_0014C050_2518[*(int *)(data + 8) + current_level_index * 16] != 0xFF) {
            moby->visible = 0;
            moby->flags |= 1;
            moby->collision = 0;
            for (j = 0; j < 3; j++) {
                *(unsigned short *)(slots[j] + 0x34) |= 1;
                slots[j][0x31] = 0;
                *(int *)(slots[j] + 0x94) = 0;
            }
        }
        update_teleporter_child_transforms(moby);
        break;
    }
    case TELEPORTER_WAIT_FOR_ACTIVATION: {
        struct Hero *g;
        if (*(int *)data == -1)
            break;
        if ((moby->flags & 1) && *(int *)(data + 8) >= 0 &&
            D_0014C050_2518[*(int *)(data + 8) + current_level_index * 16] == 0xFF)
            FUN_L11_002f3040_2518(moby);
        g = &hero;
        if (((char *)g->ground_moby) != moby)
            break;
        if (g->air_frames.s != 0)
            break;
        FUN_L11_002f2cd0_2518(moby);
        if (!(D_0013CAE4_lui & 0x10))
            break;
        if (D_L11_0015F594 != 0xB)
            break;
        allocate_voice_for_target_entry(0, 0, (void *)moby);
        moby->state = 2;
        *(int *)(data + 0x28) = 0;
        *(int *)(data + 0x2C) = 0;
        request_hero_target_pose(&moby->pos.x, 0, moby->rot.z);
        FUN_L00_002598b0_2518((int)moby, 1.5f, &moby->pos, 0x10000, 20.0f, 1.0f, 0, 1, 0);
        break;
    }
    case TELEPORTER_OPEN_PARTS: {
        int teleport_transition_status = get_hero_target_pose_status();
        if (*(float *)(data + 0x20) < 1.0f) {
            advance_accelerated_scalar((float *)(data + 0x20),
                                       (float *)(data + 0x28),
                                       1.0f,
                                       teleporter_parts_acceleration_gp * D_0015ED70_2518,
                                       teleporter_parts_deceleration_gp * D_0015ED70_2518,
                                       teleporter_parts_max_speed_gp * D_0015ED6C_2518);
        } else if (*(float *)(data + 0x24) < 1.0f) {
            advance_accelerated_scalar((float *)(data + 0x24),
                                       (float *)(data + 0x2C),
                                       1.0f,
                                       teleporter_parts_acceleration_gp * D_0015ED70_2518,
                                       teleporter_parts_deceleration_gp * D_0015ED70_2518,
                                       teleporter_parts_max_speed_gp * D_0015ED6C_2518);
        } else if (D_L11_0015F594 != 0xB || !(*(float *)(data + 0x20) > 0.25f)) {
            if (teleport_transition_status == 2) {
                *(float *)(data + 0x3C) = 1.95f;
                moby->state = 3;
            } else if (teleport_transition_status == 0) {
                moby->state = 4;
            }
        }
        update_teleporter_child_transforms(moby);
        break;
    }
    case TELEPORTER_CONTRACT_RING:
        if (*(float *)(data + 0x3C) != 0.0f) {
            add_draw_callback(FUN_L11_002f2d58_2518, moby);
            approach_value((float *)(data + 0x3C), 0.0f, D_0015ED6C_2518 * 4.0f);
        } else {
            char *destination_moby = D_L11_0015FFD8_2518 + (*(int *)data << 8);
            char *td;
            float destination_position[4];
            if (*(int *)(data + 4) != -1 &&
                D_0014C050_2518[*(int *)(data + 0xC) + current_level_index * 16] == 0xFF)
                destination_moby = D_L11_0015FFD8_2518 + (*(int *)(data + 4) << 8);
            td = ((char *)MOBY(destination_moby)->pvars);
            qcopy(destination_position, destination_moby + 0x10);
            destination_position[2] += 0.2f;
            FUN_L00_00216f90_2518(destination_position, destination_moby + 0x40, 0, 1);
            D_001413F5_lui = 1;
            if (*(int *)(data + 0x30) != -1 && *(int *)(data + 0x34) != -1) {
                FUN_L01_0027a248_2518(*(int *)(data + 0x30), *(int *)(data + 0x34));
            }
            FUN_L11_002f3040_2518(destination_moby);
            MOBY(destination_moby)->state = 4;
            *(float *)(td + 0x3C) = 0.0f;
            *(float *)(td + 0x28) = 0.0f;
            *(float *)(td + 0x2C) = 0.0f;
            *(float *)(td + 0x20) = 1.0f;
            *(float *)(td + 0x24) = 1.0f;
            allocate_voice_for_target_entry(1, 0, (void *)destination_moby);
            moby->state = 1;
            *(float *)(data + 0x20) = 0.0f;
            *(float *)(data + 0x24) = 0.0f;
            *(float *)(data + 0x28) = 0.0f;
            *(float *)(data + 0x2C) = 0.0f;
            *(float *)(data + 0x3C) = 1.95f;
            update_teleporter_child_transforms(moby);
        }
        break;
    case TELEPORTER_RESET:
        if (D_0013F3D8_lui < 138.0f)
            D_0013D3DD = 1;
        if (*(float *)(data + 0x3C) < 1.95f) {
            add_draw_callback(FUN_L11_002f2d58_2518, moby);
            approach_value((float *)(data + 0x3C), 1.95f, D_0015ED6C_2518 * 4.0f);
        } else if (0.0f < *(float *)(data + 0x24)) {
            advance_accelerated_scalar((float *)(data + 0x24),
                                       (float *)(data + 0x28),
                                       0.0f,
                                       teleporter_parts_acceleration_gp * D_0015ED70_2518,
                                       teleporter_parts_deceleration_gp * D_0015ED70_2518,
                                       teleporter_parts_max_speed_gp * D_0015ED6C_2518);
        } else if (0.0f < *(float *)(data + 0x20)) {
            advance_accelerated_scalar((float *)(data + 0x20),
                                       (float *)(data + 0x2C),
                                       0.0f,
                                       teleporter_parts_acceleration_gp * D_0015ED70_2518,
                                       teleporter_parts_deceleration_gp * D_0015ED70_2518,
                                       teleporter_parts_max_speed_gp * D_0015ED6C_2518);
        } else {
            moby->state = 1;
        }
        update_teleporter_child_transforms(moby);
        break;
    }
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002F4040), where it is exact; names translated to the US level program. */

void request_teleporter_help_message(struct Moby *moby) __asm__("FUN_L11_002f2cd0");

void request_teleporter_help_message(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    switch (*(int *)(data + 0x10)) {
    case 0:
        try_set_help_message(0xB, 0x2B0D);
        break;
    case 3:
        try_set_help_message(0xB, 0x2B0B);
        break;
    case 1:
    case 2:
    case 4:
        try_set_help_message(0xB, 0x2B0C);
        break;
    default:
        try_set_help_message(0xB, 0);
        break;
    }
}

typedef struct {
    float              corner[4][4];
    unsigned int       color[4];
    float              uv[4][2];
    unsigned long long tag[4];
} QuadPacket;

extern unsigned long long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void               draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

extern float teleporter_ring_quad_uvs[] __asm__("D_L11_001D3530");

int          D_L11_00161920 = 0;
int          D_L11_00161924 = 2;
int          D_L11_00161928 = 0;
int          D_L11_0016192C = 1;
int          D_L11_00161930 = 0x13;
unsigned int l11_teleporter_ring_vertex_color __asm__("D_L11_00161934") = 0x60408080;
extern float teleporter_ring_local_z_center_gp __asm__("D_L11_00161938") __attribute__((sda));
/* Draws a ring of 32 textured quads around the moby; the band widens as the radius at pvars+0x3C shrinks below 1.15. */
void draw_teleporter_ring(struct Moby *moby) __asm__("FUN_L11_002f2d58");

void draw_teleporter_ring(struct Moby *moby) {
    QuadPacket quad;
    float      matrix[16];
    char      *data = (char *)moby->pvars;
    float      width;
    float      angle;
    int        i, j;

    copy_matrix_rotation(matrix, &moby->rot_row0);
    qcopy(matrix + 12, &moby->pos);
    quad.tag[1] = get_effect_texture(D_L11_00161930);
    quad.tag[3] = (unsigned long long)D_L11_00161920 | (unsigned long long)D_L11_00161924 << 2 |
                  (unsigned long long)D_L11_00161928 << 4 |
                  (unsigned long long)D_L11_0016192C << 6 | 0x8000000000ULL;
    quad.tag[2] = 0xff9000000260ULL;
    quad.tag[0] = 0;
    {
        float        *uv = teleporter_ring_quad_uvs;
        unsigned int *colors = quad.color;
        float        *dest1 = &quad.uv[0][1];
        float        *dest0 = &quad.uv[0][0];
        int           k = 3;
        do {
            float u = uv[0];
            float v = uv[1];
            *dest0 = u;
            uv += 2;
            *dest1 = v;
            dest0 += 2;
            *colors++ = l11_teleporter_ring_vertex_color;
            dest1 += 2;
            k--;
        } while (k >= 0);
    }
    width = 0.1f;
    if (*(float *)(data + 0x3c) < 1.15f) {
        float v = (1.15f - *(float *)(data + 0x3c)) + (1.15f - *(float *)(data + 0x3c));
        if (v > 1.0f)
            v = 1.0f;
        else if (v < 0.0f)
            v = 0.0f;
        width = v + 0.1f;
        if (*(float *)(data + 0x3c) < 0.65f)
            D_001413F5_lui = 1;
        else
            D_001413F5_lui = 0;
    }
    i = 0;
    do {
        j = 0;
        do {
            float *corner = quad.corner[j];
            angle = (float)(i + j / 2) * 6.28318f * 0.03125f - 3.14159f;
            corner[0] = fast_cos(angle) * *(float *)(data + 0x3c);
            corner[1] = fast_sin(angle) * *(float *)(data + 0x3c);
            corner[2] = 0.0f;
            if (j & 1)
                corner[2] = teleporter_ring_local_z_center_gp + width + 0.0f;
            else
                corner[2] = teleporter_ring_local_z_center_gp - width + 0.0f;
            corner[3] = 1.0f;
            j++;
        } while (j < 4);
        draw_geometry_quad(&quad, matrix, 0);
        i++;
    } while (i < 32);
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002F43B0), where it is exact; names translated to the US level program. */

void restore_teleporter_visibility_and_collision(struct Moby *moby) __asm__("FUN_L11_002f3040");

void restore_teleporter_visibility_and_collision(struct Moby *moby) {
    char  *data = (char *)moby->pvars;
    char **child = (char **)(data + 0x14);
    int    count;
    int    on;
    moby->visible = 1;
    moby->flags &= 0xFFFE;
    moby->collision = moby->pclass->collision;
    on = 1;
    count = 2;
    do {
        if (*child) {
            *(unsigned short *)(*child + 0x34) &= 0xFFFE;
            (*child)[0x31] = on;
            *(int *)(*child + 0x94) = *(int *)(*(char **)(*child + 0x24) + 0x10);
        }
        child++;
        count--;
    } while (count >= 0);
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002F46C0), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x88];
    float          timer;
    char           pad1[0x2FC - 0x8C];
    char          *current;
    char           pad2[0x30E - 0x300];
    short          active;
    char           pad3[0x2084 - 0x310];
    int            mode;
    char           pad4[0x22DA - 0x2088];
    unsigned short index;
} Globals;

void copy_pokitaru_pvar_to_hero(struct Moby *moby) __asm__("FUN_L11_002f3350");

void copy_pokitaru_pvar_to_hero(struct Moby *moby) {
    enum {
        MOBY_INITIALIZE = 0,
        MOBY_COPY_PVAR_TO_HERO = 1
    };
    char *state = (char *)moby->pvars;
    switch (moby->state) {
    case MOBY_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        if (*(int *)state == -1)
            mark_moby_for_removal(moby);
        break;
    case MOBY_COPY_PVAR_TO_HERO:
        if (hero.motion.pos.f[2] > 194.0f && hero.state.current != 0x32) {
            *(short unsigned int *)&hero.path_constrain_pending = *(unsigned short *)state;
        }
        break;
    }
}

/* Same source as the exact FUN_L13_002e6a58, spawning class 0x40A, without the target class store and the closing effect. */
typedef struct {
    char          _pad00[0x10];
    unsigned char nframes;
} AnimSeq_309378;

typedef struct {
    char            _pad00[0x48];
    AnimSeq_309378 *seqs[1];
} AnimClass_309378;

typedef struct {
    char              _pad00[0x24];
    AnimClass_309378 *pClass;
    char              _pad28[0x50 - 0x28];
    unsigned char     frame;
    unsigned char     nextFrame;
    unsigned char     seq;
    unsigned char     prevSeq;
    char              _pad54[0x5C - 0x54];
    float             unk5C;
    char              _pad60[0x68 - 0x60];
    float            *frameData;
    char              _pad6C[4];
    unsigned char     unk70;
} MobyAnim_309378;

extern char *spawn_source_attached_fading_effect_particle(char *, int, int, int, float) __asm__("FUN_L00_0026daa0");
void blend_moby_animation_309378(MobyAnim_309378 *arg0,
                                 int              arg1,
                                 int              arg2,
                                 int              arg3) __asm__("FUN_00212f90");

unsigned char *spawn_ship_projectile(struct Moby *spawning_moby,
                                     char        *spawn_position,
                                     char        *target,
                                     char        *spawn_rotation,
                                     int          arg,
                                     float        scale) __asm__("FUN_L11_00309378");

unsigned char *spawn_ship_projectile(struct Moby *spawning_moby,
                                     char        *spawn_position,
                                     char        *target,
                                     char        *spawn_rotation,
                                     int          duration_frames,

                                     float scale) {
    unsigned char *moby = create_moby(0x40A);
    if (moby != 0) {
        char *data = ((char *)MOBY(moby)->pvars);
        moby[0x30] = 0xFF;
        MOBY(moby)->draw_dist = 0xFF;
        moby[0x31] = 1;
        moby[0x20] = 0;
        *(char **)(data + 0x20) = spawning_moby;
        *(char **)(data + 0x24) = target;
        qcopy(moby + 0x10, spawn_position);
        qcopy(moby + 0x40, spawn_rotation);
        if (target != 0) {
            char *target_aux_data;
            target_aux_data = suck_get_aim_height(target);
            if (target_aux_data != 0) {
                *(unsigned short *)(target_aux_data + 0x1E) |= 0x80;
            }
        }
        *(int *)(data + 0x28) = duration_frames;
        *(float *)(data + 0x2C) = scale;
        *(int *)(data + 0x30) = 0;
        *(int *)(data + 0x34) = 0;
        build_spherical_offset(
            data, scale, *(float *)(spawn_rotation + 8), -*(float *)(spawn_rotation + 4));
        if (moby[0x53] != 1) {
            blend_moby_animation_309378(moby, 1, 0, 10);
        }
        fast_vec_add(moby + 0x10, moby + 0x10, data);
        if (*(char **)(data + 0x24) != 0) {
            qcopy(data + 0x10, *(char **)(data + 0x24) + 0x10);
        }
        MOBY(moby)->scale =
            *(float *)(((char *)MOBY(moby)->pclass) + 0x24) *
            (spawning_moby->scale / *(float *)(((char *)spawning_moby->pclass) + 0x24));
        update_entity(moby);
    }
    return moby;
}

struct PokPv {
    char  pad00[0x60];
    char  ctl60[0x3C];
    int   flags;
    int   path_a0;
    int   pad_a4;
    int   idx_a8;
    int   idx_ac;
    int   flag_b0;
    int   pad_b4;
    int   flag_b8;
    int   pad_bc;
    int   flag_c0;
    int  *path_c4;
    int   slot_c8;
    int   slot_cc;
    float approach_d0;
    int   seg_d4;
    float vel_d8[2];
    float angle_e0;
    float height_e4;
    float spr_e8;
    float spr_ec;
    float wobble_f0[4];
};

extern unsigned char D_0013E550_309ac0[] __asm__("D_0013E550");
extern void         *D_0013F64C_lui __asm__("D_0013F64C") NOT_SDA;
extern float         random_angle_radians(void) __asm__("FUN_00213308");
extern void          scale_vec_309ac0(float *, float) __asm__("FUN_L00_0025f730");
extern void update_moby_rotation_oscillation(void *, void *, void *, float, float, float) __asm__("FUN_L00_00262b80");
extern void follow_camera_set_distance(int, float, float) __asm__("FUN_L00_002e8450");
extern void follow_camera_set_height(float, float) __asm__("FUN_L00_002e84b8");
void        update_pokitaru_boat_joint_effects(struct Moby *moby) __asm__("FUN_L11_0030a500");

extern void update_pokitaru_boat_joint_effects(struct Moby *) __asm__("FUN_L11_0030a500");
int         is_pokitaru_boat_target_group_clear(struct Moby *moby) __asm__("FUN_L11_0030a850");

extern int is_pokitaru_boat_target_group_clear(struct Moby *) __asm__("FUN_L11_0030a850");

extern float pokitaru_boat_path_speed_gp __asm__("D_L11_00161DD4") __attribute__((sda));
extern float pokitaru_boat_turn_acceleration_degrees_gp __asm__("D_L11_00161DD8")
    __attribute__((sda));
extern float pokitaru_boat_max_turn_speed_degrees_gp __asm__("D_L11_00161DDC") __attribute__((sda));

#include "rnc/overlay/l11/mobys/bob_data/pokitaru_boat_update.h"

void pokitaru_boat_update(struct Moby *moby) __asm__("FUN_L11_00309ac0");

void pokitaru_boat_update(struct Moby *moby) {
    enum {
        POKITARU_BOAT_INITIALIZE = 0,
        POKITARU_BOAT_WAIT_FOR_ACTIVATION = 1,
        POKITARU_BOAT_FOLLOW_PATH = 2,
        POKITARU_BOAT_WAIT_FOR_GROUP_CLEAR = 3,
        POKITARU_BOAT_RISE_ABOVE_PATH = 4,
        POKITARU_BOAT_ADJUST_HEIGHT_FOR_HERO = 5
    };
    struct PokPv *boat_vars = (struct PokPv *)moby->pvars;
    float         start_pos[4];
    float         start_rot[4];
    float         delta[4];
    float         target[4];
    float         path_movement[4];
    float         spare_a[4];
    float         spare_b[4];
    float         point_offset_from_boat[4];

    qcopy(start_pos, &moby->pos);
    qcopy(start_rot, &moby->rot);
    if (moby->state != 0) {
        moby->pos.z = boat_vars->height_e4;
    }
    update_pokitaru_boat_joint_effects(moby);

    switch (moby->state) {
    case POKITARU_BOAT_INITIALIZE: {
        int *tab_a8;
        int *tab_ac;
        int *path;
        int  n;
        int  part_angle_index;

        moby->draw_dist = 0xFF;
        if (moby->prev_seq != 1) {
            blend_moby_animation_309378((MobyAnim_309378 *)moby, 1, 0, 1);
        }
        tab_a8 = (int *)l11_paths[boat_vars->idx_a8];
        tab_ac = (int *)l11_paths[boat_vars->idx_ac];
        transpose_rotation_matrix(target, &moby->rot_row0);
        for (n = 0; n < tab_a8[0]; n++) {
            fast_vec_sub(point_offset_from_boat, (char *)tab_a8 + 0x10 + n * 16, &moby->pos);
            transform_vector((char *)tab_ac + 0x10 + n * 16, point_offset_from_boat, target);
        }
        path = (int *)l11_paths[boat_vars->path_a0];
        boat_vars->path_c4 = path;
        qcopy(&moby->pos, (char *)path + 0x10);
        boat_vars->seg_d4 = 1;
        moby->state = 1;
        for (part_angle_index = 0; part_angle_index < 4; part_angle_index++) {
            boat_vars->wobble_f0[part_angle_index] = random_angle_radians();
        }
        boat_vars->slot_c8 = -1;
        boat_vars->slot_cc = -1;
        boat_vars->flags |= 1;
        break;
    }
    case POKITARU_BOAT_WAIT_FOR_ACTIVATION:
        if (boat_vars->flag_b8 != 0) {
            int   *path = (int *)l11_paths[boat_vars->path_a0];
            int    n;
            float *last;

            boat_vars->path_c4 = path;
            n = path[0];
            qcopy(&moby->pos, (char *)path + n * 16 - 16);
            last = (float *)((char *)path + (n - 1) * 16);
            moby->rot.z = fast_arc_tan(last[4] - moby->pos.x, last[5] - moby->pos.y);
            moby->state = 5;
        }
        break;
    case POKITARU_BOAT_FOLLOW_PATH: {
        int *tab_a8;
        int *tab_ac;
        int  i;

        qcopy(target, (char *)boat_vars->path_c4 + 0x10 + boat_vars->seg_d4 * 16);
        if (D_0013F64C_lui == moby) {
            if (!voice_is_playing(moby, boat_vars->slot_cc)) {
                boat_vars->slot_cc = allocate_voice_for_target_entry(1, 4, moby);
            }
            if (voice_is_playing(moby, boat_vars->slot_c8)) {
                if (boat_vars->slot_c8 != -1) {
                    unsigned char *ent = D_0013E550_309ac0 + boat_vars->slot_c8 * 0x70;
                    if (*(void **)(ent + 0x88) == moby && ent[0x74] != 0) {
                        release_voice_slot(boat_vars->slot_c8);
                    }
                }
                boat_vars->slot_c8 = -1;
            }
            approach_value(&boat_vars->approach_d0,
                           pokitaru_boat_path_speed_gp * frame_time,
                           frame_time_sq * 6.0f);
        } else {
            if (!voice_is_playing(moby, boat_vars->slot_c8)) {
                boat_vars->slot_c8 = allocate_voice_for_target_entry(0, 4, moby);
            }
            if (voice_is_playing(moby, boat_vars->slot_cc)) {
                if (boat_vars->slot_cc != -1) {
                    unsigned char *ent = D_0013E550_309ac0 + boat_vars->slot_cc * 0x70;
                    if (*(void **)(ent + 0x88) == moby && ent[0x74] != 0) {
                        release_voice_slot(boat_vars->slot_cc);
                    }
                }
                boat_vars->slot_cc = -1;
            }
            approach_value(&boat_vars->approach_d0, 0.0f, frame_time_sq * 6.0f);
        }
        fast_vec_sub(path_movement, target, &moby->pos);
        scale_vec_309ac0(path_movement, boat_vars->approach_d0);
        fast_vec_add(&moby->pos, &moby->pos, path_movement);
        if (boat_vars->seg_d4 < boat_vars->path_c4[0] - 1) {
            if (fast_vec_dist(&moby->pos, target) < pokitaru_boat_path_speed_gp * frame_time) {
                float *pt;
                boat_vars->seg_d4 += 1;
                pt = (float *)((char *)boat_vars->path_c4 + boat_vars->seg_d4 * 16);
                if (pt[7] == 37.0f) {
                    boat_vars->flag_c0 = 1;
                }
            }
        } else if (vector_length_xy(path_movement) < 0.001f) {
            if (voice_is_playing(moby, boat_vars->slot_cc)) {
                if (boat_vars->slot_cc != -1) {
                    unsigned char *ent = D_0013E550_309ac0 + boat_vars->slot_cc * 0x70;
                    if (*(void **)(ent + 0x88) == moby && ent[0x74] != 0) {
                        release_voice_slot(boat_vars->slot_cc);
                    }
                }
                boat_vars->slot_cc = -1;
            }
            if (voice_is_playing(moby, boat_vars->slot_c8)) {
                if (boat_vars->slot_c8 != -1) {
                    unsigned char *ent = D_0013E550_309ac0 + boat_vars->slot_c8 * 0x70;
                    if (*(void **)(ent + 0x88) == moby && ent[0x74] != 0) {
                        release_voice_slot(boat_vars->slot_c8);
                    }
                }
                boat_vars->slot_c8 = -1;
            }
            moby->state = 3;
        }
        approach_angle(&moby->rot.z,
                       fast_arc_tan(target[0] - moby->pos.x, target[1] - moby->pos.y),
                       boat_vars->vel_d8,
                       pokitaru_boat_turn_acceleration_degrees_gp * 0.017453292f * frame_time_sq,
                       pokitaru_boat_turn_acceleration_degrees_gp * 0.017453292f * frame_time_sq,
                       pokitaru_boat_max_turn_speed_degrees_gp * 0.017453292f * frame_time);
        if (boat_vars->flag_c0 != 0) {
            follow_camera_set_distance(0, pokitaru_boat_camera_distance_gp, 0.003f);
            follow_camera_set_height(pokitaru_boat_camera_height_gp, 0.003f);
        }
        tab_a8 = (int *)l11_paths[boat_vars->idx_a8];
        tab_ac = (int *)l11_paths[boat_vars->idx_ac];
        for (i = 0; i < tab_a8[0]; i++) {
            transform_vector_by_basis(
                (char *)tab_a8 + 0x10 + i * 16, (char *)tab_ac + 0x10 + i * 16, &moby->rot_row0);
            fast_vec_add(
                (char *)tab_a8 + 0x10 + i * 16, (char *)tab_a8 + 0x10 + i * 16, &moby->pos);
        }
        break;
    }
    case POKITARU_BOAT_WAIT_FOR_GROUP_CLEAR:
        if (is_pokitaru_boat_target_group_clear(moby) != 0) {
            if (boat_vars->flag_b0 != 0) {
                boat_vars->approach_d0 = 0.0f;
                moby->state = 4;
            } else {
                moby->state = 5;
            }
        }
        break;
    case POKITARU_BOAT_RISE_ABOVE_PATH:
        qcopy(target, (char *)boat_vars->path_c4 + 0x10 + boat_vars->seg_d4 * 16);
        advance_accelerated_scalar(&moby->pos.z,
                                   &boat_vars->approach_d0,
                                   target[2] + 6.0f,
                                   frame_time_sq,
                                   frame_time_sq,
                                   frame_time);
        if (target[2] + 5.9f <= moby->pos.z) {
            moby->state = 5;
        }
        break;
    case POKITARU_BOAT_ADJUST_HEIGHT_FOR_HERO:
        qcopy(target, (char *)boat_vars->path_c4 + 0x10 + boat_vars->seg_d4 * 16);
        if (boat_vars->flag_b0 != 0) {
            if (D_0013F3D8_lui < moby->pos.z) {
                advance_accelerated_scalar(&moby->pos.z,
                                           &boat_vars->approach_d0,
                                           target[2] + 1.0f,
                                           frame_time_sq,
                                           frame_time_sq,
                                           frame_time);
            } else {
                advance_accelerated_scalar(&moby->pos.z,
                                           &boat_vars->approach_d0,
                                           target[2] + 8.0f,
                                           frame_time_sq,
                                           frame_time_sq,
                                           frame_time);
            }
        }
        break;
    }

    {
        float ft = frame_time;
        float spin = ft * 1.5707964f;
        float tilt_a = ft * 0.43633232f;
        float tilt_b = ft * 0.61086524f;
        float amp;

        boat_vars->height_e4 = moby->pos.z;
        boat_vars->angle_e0 = fast_add_rotations(boat_vars->angle_e0, spin);
        amp = 0.13f;
        moby->pos.z = moby->pos.z + fast_sin(boat_vars->angle_e0) * amp;
        update_moby_rotation_oscillation(
            moby, &boat_vars->spr_e8, &boat_vars->spr_ec, 0.0418879017f, tilt_a, tilt_b);
    }
    fast_vec_sub(delta, &moby->pos, start_pos);
    update_entity_matrix(boat_vars->ctl60, delta, start_rot, &moby->rot);
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030B850), where it is exact; names translated to the US level program. */

int try_activate_pokitaru_gate(struct Moby *moby) {
    char *current;
    if (moby->state != 1)
        return 0;
    current = (char *)hero.ground_moby;
    if (current != (char *)moby || hero.air_frames.s)
        return 0;
    current[0x20] = 2;
    if (((unsigned char *)current)[0x53]) {
        int value = scale_game_frames(0x14);
        blend_moby_animation(current, 0, 0, value);
    }
    return 1;
}

extern void FUN_L00_001fff28(void *, int, float);
extern void FUN_L00_0026f080(float *, float *, float, float);
extern int  level_frame __asm__("D_L11_0015F5CC");

struct PartA500 {
    u8 unk0;
    u8 active;
    u8 pad2[0xE];
    u8 joint[0x30];
};

struct DataA500 {
    u8              pad0[0xF0];
    float           angle[4];
    struct PartA500 parts[4];
};

void update_pokitaru_boat_joint_effects(struct Moby *moby) {
    struct DataA500 *moby_vars = (struct DataA500 *)moby->pvars;
    float            base[4];
    float            pos[4];
    float            initial_velocity[4];
    float            speed;
    float            step;
    float            heading;
    int              i;
    int              iterations_per_joint;
    int              j;
    int              k;
    float            life;

    for (i = 0; i < 4; i++) {
        if (!((u8 *)moby_vars)[i * 0x40 + 0x101])
            attach_manipulator(moby, i, &moby_vars->parts[i]);
        step = frame_time * 12.217304f;
        if (moby->state == 1)
            step = frame_time * 2.6179938f;
        if (((u8 *)moby_vars)[i * 0x40 + 0x101]) {
            moby_vars->angle[i] = fast_add_rotations(moby_vars->angle[i], step);
            FUN_L00_001fff28(moby_vars->parts[i].joint, 0, moby_vars->angle[i]);
        }
    }
    if (vector_distance_xy(&moby->pos, camera_pos_b358) > 38.0f)
        return;
    if (moby->state == 1 && (level_frame & 1))
        return;
    if (!(moby->pos.z < 224.5f))
        return;
    for (j = 0; j < 4; j++) {
        moby_point_position(moby, j, base);
        iterations_per_joint = moby->state == 1 ? 1 : 2;
        for (k = 0; k < iterations_per_joint; k++) {
            pos[0] = base[0] + randf_sym(0.0f, 0.3f);
            pos[1] = base[1] + randf_sym(0.0f, 0.3f);
            pos[2] = base[2] + randf_sym(0.0f, 0.3f);
            heading = fast_add_rotations(moby->rot.z, randf_sym(0.0f, 0.122173049f) + 3.1415927f);
            speed = random_float_between(2.0f, 5.0f) * frame_time;
            if (moby->state == 1)
                speed *= 0.4f;
            initial_velocity[0] = fast_cos(heading) * speed;
            initial_velocity[1] = fast_sin(heading) * speed;
            initial_velocity[2] = 0.0f;
            initial_velocity[2] = random_float_between(-2.0f, 2.0f) * frame_time;
            life = random_float_between(6300.0f, 14700.0f);
            FUN_L00_0026f080(pos, initial_velocity, life, 222.5f);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030BC00), where it is exact; names translated to the US level program. */

int is_moby_in_state_five(struct Moby *moby) {
    return moby->state == 5;
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030BC10), where it is exact; names translated to the US level program. */

void FUN_L11_0030a840(unsigned char *gate_moby) {
    *(int *)(*(int *)(gate_moby + 0x78) + 0xB8) = 1;
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030BC20), where it is exact; names translated to the US level program. */

extern int D_L11_0015FFD8; /* no foreign declaration */
extern s32 D_L11_0015FFF4;

int is_pokitaru_boat_target_group_clear(struct Moby *moby) {
    int    moby_group_index = *(short *)((char *)moby->pvars + 0xB4);
    short *group_entry;
    if (moby_group_index < 0 || *(int *)&D_L11_0015FFF4 < moby_group_index)
        return 1;
    group_entry = D_L11_001AC240[moby_group_index];
    if (group_entry == 0)
        return 1;
    do {
        char *group_moby = (char *)(((*group_entry & 0x7FFF) << 8) + D_L11_0015FFD8);
        if (group_moby[0x20] >= 0 && MOBY(group_moby)->oclass == 0x4DE) {
            if (is_moby_in_bob_target_exclusion_state(group_moby) == 0)
                return 0;
        }
    } while (*group_entry++ >= 0);
    return 1;
}
