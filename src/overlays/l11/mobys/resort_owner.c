/* Level 11 moby class resort_owner: the path-walking NPC, its states, movement, jumps and look tracking. */
#include "types.h"

#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/item_state.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/cutscenes/update_npc_dialogue.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/mobys/attach_big_head_cheat_manipulator.h"
#include "rnc/overlay/shared/mobys/build_symmetric_adjacency_masks.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/watch.h"
#include "rnc/overlay/l11/mobys/watcher.h"
#include "rnc/overlay/l11/mobys/resort_owner.h"

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

void mark_moby_for_removal(struct Moby *obj) __asm__("FUN_0020c828");

#include "qcopy.h"
#include "rnc/overlay/l11/mobys/puffoid.h"

extern float
approach_angle(float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");

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

extern void begin_dialogue_camera_transition(void *, float) __asm__("FUN_L01_002783a8");
extern int  D_L11_0015F5C4;
extern void update_joint_animation(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_002D2168), where it is exact; names translated to the US level program. */

extern float camera_pos_b358[] __asm__("D_L11_001677C0");
extern void  FUN_L00_0025a120(void *);
void         update_resort_owner_state_actions(void *mp) __asm__("FUN_L11_002d1030");

extern void update_resort_owner_state_actions(void *) __asm__("FUN_L11_002d1030");
void        apply_resort_owner_movement(void *mp) __asm__("FUN_L11_002d1340");

extern void apply_resort_owner_movement(void *) __asm__("FUN_L11_002d1340");
extern void update_resort_owner_state_transitions(void *) __asm__("FUN_L11_002d14b0");
void        update_resort_owner_look_tracking(WatcherMoby *moby) __asm__("FUN_L11_002d27b0");

void update_resort_owner_look_tracking(WatcherMoby *m) __asm__("FUN_L11_002d27b0");

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
extern void  FUN_L11_0030a840(unsigned char *arg);
extern unsigned char D_0013D4EE NOT_SDA;
extern unsigned char D_0013D3DC NOT_SDA;
extern int                      D_0015ED84;
extern unsigned char            D_0014C050[];

#include "rnc/overlay/cuboid.h"

#include "rnc/camera/camera_globals.h"

extern char resort_owner_completion_dialogue_pose[] __asm__("D_L11_0016D270");

#include "rnc/overlay/l11/mobys/resort_owner_globals.h"

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
            camera_secondary_mode[0] != 0x32 && update_npc_dialogue(moby, moby_vars->talk)) {
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

extern s32 D_L11_0015FFF4;

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
                    is_moby_in_bob_target_exclusion_state((const u8 *)other) == 0) {
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
void update_resort_owner_look_tracking(WatcherMoby *moby) {
    L16WatchScratch scratch;
    WD11b          *moby_vars = (WD11b *)moby->data;
    float           rate;
    float           head_rate;
    int             tracking;
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
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030BC00), where it is exact; names translated to the US level program. */

int is_moby_in_state_five(struct Moby *arg) __asm__("FUN_L11_0030a830");

int is_moby_in_state_five(struct Moby *moby) {
    return moby->state == 5;
}
/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_002CC828.c: func_L11_0030BC10), where it is exact; names translated to the US level program. */

void FUN_L11_0030a840(unsigned char *gate_moby) {
    *(int *)(*(int *)(gate_moby + 0x78) + 0xB8) = 1;
}
