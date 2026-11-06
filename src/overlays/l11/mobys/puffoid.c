/* Level 11 moby class puffoid (class 0x4DE): spawns on a path or a carrier moby, wanders, chases and attacks; its damage and path goal, group count, target exclusion test and suck cannon callbacks. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/knockback_motion.h"
#include "rnc/overlay/shared/mobys/ground_move_control.h"

#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/overlay/shared/camera/level_camera.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/mobys/resolve_moby_hit_response.h"
#include "rnc/overlay/l11/mobys/resort_owner.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"

/* pvars of the class 0x4DE creature run by puffoid_update. */
enum {
    PUFFOID_STATE_INITIALIZE = 0,
    PUFFOID_STATE_WAIT_FOR_HOME_JUMP = 1,
    PUFFOID_STATE_IDLE = 2,
    PUFFOID_STATE_WANDER_NEAR_HOME = 3,
    PUFFOID_STATE_CHASE_TARGET = 4,
    PUFFOID_STATE_SPRAY_ATTACK = 5,
    PUFFOID_STATE_RETURN_HOME = 6,
    PUFFOID_STATE_JUMP_HOME = 7,
    PUFFOID_STATE_JUMP_THEN_WAIT_FOR_HOME_JUMP = 8,
    PUFFOID_STATE_KNOCKBACK = 9,
    PUFFOID_STATE_REPOSITION_AND_FACE_TARGET = 10,
    PUFFOID_STATE_DEATH_KNOCKBACK = 11,
    PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP = 12,
    PUFFOID_STATE_CARRIER_JUMP = 13,
    PUFFOID_STATE_CARRIER_IDLE = 14,
    PUFFOID_STATE_CARRIER_CHASE_TARGET = 15,
    PUFFOID_STATE_CARRIER_SPRAY_ATTACK = 16,
    PUFFOID_STATE_CARRIER_KNOCKBACK = 17,
    PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK = 18,
    PUFFOID_STATE_FOLLOW_PATH = 19,
    PUFFOID_STATE_WAIT_TO_RESUME_COMMAND = 20
};

typedef struct {
    u8    pad00[0x20];
    f32   hp;         /* 0x20 */
    s16   max_health; /* 0x24 */
    u8    pad26[2];
    u8    hit_response_class; /* 0x28 */
    u8    unk29;              /* 0x29 */
    u8    pad2A[6];
    f32   unk30; /* 0x30 */
    u8    pad34[0xC];
    f32   vel[4]; /* 0x40: jump velocity; [2] falls by D_L11_001622F0 * frame_time_sq */
    u8    pad50[8];
    u8    unk58; /* 0x58 */
    u8    pad59;
    u8    unk5A; /* 0x5A */
    u8    pad5B[0x6D];
    s16   unkC8; /* 0xC8 */
    u8    padCA[6];
    void *unkD0; /* 0xD0 */
    u8    padD4[0x4C];
    struct KnockbackMotion   knockback;    /* 0x120: step_moby_motion / FUN_L00_002dc9f0 state */
    struct GroundMoveControl move_control; /* 0x180 */
    f32                      goal[4];      /* 0x1D0 */
    u8                       pad1E0[0x30];
    struct Moby             *target; /* 0x210 */
    s32 target_status; /* 0x214: status word of the target record at goal; 2 once the goal is out of reach */
    u8  pad218[8];
    f32 home[4];    /* 0x220 */
    s32 path_index; /* 0x230: path index for FUN_L00_00261968 / the landing check */
    s32 path;       /* 0x234: index into l11_paths */
    s32 mode;       /* 0x238: start mode */
    u8  pad23C[8];
    s32 timer; /* 0x244 */
    u8  pad248[4];
    f32 chase_heading_offset_radians;   /* 0x24C: heading offset for FUN_L11_00315f58 */
    f32 yaw_turn_velocity;              /* 0x250: turn speed */
    s32 carrier;                        /* 0x254: moby index of the carrier, or < 0 */
    f32 carrier_jump_local_x_threshold; /* 0x258 */
    s32 point;                          /* 0x25C: current path point */
    f32 wander_destination[4];          /* 0x260 */
    u8  pad270[0x10];
    u8  manipulator[0x10]; /* 0x280 */
} CreatureVars_4318;

extern char *D_L11_0015FFD8_4318 __asm__("D_L11_0015FFD8");
extern char  D_L11_001677C0_4318[] __asm__("D_L11_001677C0");
extern char  D_0013F3D0_4318[] __asm__("D_0013F3D0");
float        D_L11_001622C0 = 0;
extern float D_L11_001622CC __attribute__((sda));

#include "rnc/overlay/l11/mobys/puffoid_data/puffoid_update.h"

extern float l11_puffoid_jump_gravity __asm__("D_L11_001622F0") __attribute__((sda));
extern void  update_puffoid_damage_and_path_goal(struct Moby *) __asm__("FUN_L11_00315968");
extern void  update_moby_target_heading_and_conditional_movement(
     char *moby, float *target_position) __asm__("FUN_L11_00315f58");
extern short count_group_puffoids_in_states_13_to_17(struct Moby *self) __asm__("FUN_L11_00316090");
extern void  spawn_4318(struct Moby *, int, void *, float) __asm__("FUN_L00_00263ac8");
extern void  FUN_L00_0025a120_4318(struct Moby *) __asm__("FUN_L00_0025a120");
extern int   rand(void) __asm__("FUN_001160d8");
extern float FUN_L00_00259710(void *);
extern int   point_in_path_xy(void *, void *, int) __asm__("FUN_L00_00259740");
extern float jump_speed_4318(void *, void *, int, float, float) __asm__("FUN_L00_0025abf0");
extern int   FUN_L00_0025c698_4318(struct Moby *, void *) __asm__("FUN_L00_0025c698");
extern void  spawn_color_swapped_explosion_burst(struct Moby *, void *, int, float, float) __asm__("FUN_L00_0025f3e8");
extern void follow_carrier_4318(struct Moby *, void *, void *, void *, void *, void *) __asm__("FUN_L00_00260400");
extern int   test_path_segment_strict_crossing(int, void *, void *) __asm__("FUN_L00_00261968");
extern void  move_4318(struct Moby *, void *, float, float, float, int) __asm__("FUN_L00_00258830");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern void  smoke_4318(void *, void *, float, float) __asm__("FUN_L00_0026f080");
extern int   FUN_L00_002dc9f0_4318(struct Moby *, void *) __asm__("FUN_L00_002dc9f0");

/* Update for the class 0x4DE creature: spawns on a path or a carrier moby, wanders, chases and attacks
   the target, jumps between path points, and smokes while it dies. */
void puffoid_update(struct Moby *moby) __asm__("FUN_L11_00314318");

/* Moby class 1246 */
void puffoid_update(struct Moby *moby) {
    enum {
        PUFFOID_PATH_ANIM_TRAVEL = 4,
        PUFFOID_PATH_ANIM_ENTER_SPRAY_ATTACK = 5,
        PUFFOID_PATH_ANIM_SPRAY_ATTACK = 6,
        PUFFOID_PATH_ANIM_EXIT_SPRAY_ATTACK = 7
    };
    enum {
        PUFFOID_START_MODE_WAIT_FOR_HOME_JUMP = 1,
        PUFFOID_START_MODE_FOLLOW_PATH = 2
    };
    CreatureVars_4318 *puffoid_vars = (CreatureVars_4318 *)moby->pvars;
    struct Moby       *target;
    float              pos[4];
    int                i;

    if (moby->state != PUFFOID_STATE_WAIT_TO_RESUME_COMMAND) {
        puffoid_vars->unkC8 = 0;
        update_puffoid_damage_and_path_goal(moby);
    }
    target = puffoid_vars->target;
    spawn_4318(moby, 0, puffoid_vars->manipulator, 2.5f);
    if (moby->visible != 0 && fast_vec_dist(&moby->pos, D_L11_001677C0_4318) < 28.0f) {
        FUN_L00_0025a120_4318(moby);
        moby->shadow_range = 0x16;
    }
    switch (moby->state) {
    case PUFFOID_STATE_INITIALIZE:
        puffoid_vars->hp = 2.0f;
        puffoid_vars->max_health = 2;
        puffoid_vars->hit_response_class = 1;
        puffoid_vars->unk30 = 0.25f;
        puffoid_vars->unk58 = truncate_float_to_s32(8.0f);
        puffoid_vars->unk5A = truncate_float_to_s32(2.0f);
        puffoid_vars->unk29 = 1;
        puffoid_vars->unkD0 = &D_L11_001622C0;
        init_ground_move_control(&puffoid_vars->move_control);
        puffoid_vars->move_control.max_climb = 0.5f;
        puffoid_vars->move_control.max_drop = 0.5f;
        puffoid_vars->move_control.collision_radius_1024ths = truncate_float_to_s32(512.0f);
        puffoid_vars->move_control.step_height = 0.5f;
        puffoid_vars->move_control.movement_option_flags |= 0x10;
        if (rand() & 1) {
            moby->flags |= 0x8000;
        }
        if (puffoid_vars->mode == PUFFOID_START_MODE_FOLLOW_PATH) {
            moby->state = PUFFOID_STATE_FOLLOW_PATH;
            if (moby->prev_seq != 4) {
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
            }
            qcopy(&moby->pos, (char *)l11_paths[puffoid_vars->path] + 0x10);
            puffoid_vars->point = 1;
        } else if (puffoid_vars->mode == PUFFOID_START_MODE_WAIT_FOR_HOME_JUMP) {
            moby->state = PUFFOID_STATE_WAIT_FOR_HOME_JUMP;
            qcopy(&moby->pos, (char *)l11_paths[puffoid_vars->path] + 0x10);
            moby->pos.z -= 1.0f;
            qcopy(puffoid_vars->home, (char *)l11_paths[puffoid_vars->path] + 0x20);
        } else if (puffoid_vars->carrier >= 0) {
            moby->state = PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP;
            puffoid_vars->carrier_jump_local_x_threshold = random_float_between(-3.0f, 3.0f);
            moby->visible = 0;
            moby->collision = 0;
            moby->flags |= 1;
        } else {
            moby->pos.z = FUN_L00_00259710(&moby->pos);
            moby->state = PUFFOID_STATE_IDLE;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 5);
            }
            qcopy(puffoid_vars->home, &moby->pos);
        }
        break;
    case PUFFOID_STATE_WAIT_FOR_HOME_JUMP: {
        char *path = (char *)l11_paths[puffoid_vars->path];
        float home_jump_heading = fast_arc_tan(*(float *)(path + 0x20) - moby->pos.x,
                                               *(float *)(path + 0x24) - moby->pos.y);
        approach_angle(&moby->rot.z,
                       &puffoid_vars->yaw_turn_velocity,
                       home_jump_heading,
                       frame_time_sq * 8.726646f,
                       frame_time_sq * 8.726646f,
                       frame_time * 12.566371f);
        if (puffoid_vars->target_status != 2 && random_integer_below(9) == 0) {
            moby->state = PUFFOID_STATE_JUMP_HOME;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, 5);
            }
            fast_vec_sub(
                puffoid_vars->vel, (char *)l11_paths[puffoid_vars->path] + 0x20, &moby->pos);
            set_vector_length_xy(puffoid_vars->vel,
                                 puffoid_vars->vel,
                                 puffoid_jump_horizontal_speed_gp * frame_time);
            puffoid_vars->vel[2] = jump_speed_4318(&moby->pos,
                                                   (char *)l11_paths[puffoid_vars->path] + 0x20,
                                                   0,
                                                   puffoid_jump_horizontal_speed_gp * frame_time,
                                                   -(l11_puffoid_jump_gravity * frame_time_sq));
        }
        break;
    }
    case PUFFOID_STATE_JUMP_HOME:
    case PUFFOID_STATE_JUMP_THEN_WAIT_FOR_HOME_JUMP:
        fast_vec_add(&moby->pos, &moby->pos, puffoid_vars->vel);
        puffoid_vars->vel[2] -= l11_puffoid_jump_gravity * frame_time_sq;
        if (puffoid_vars->vel[2] < 0.0f) {
            float h;
            qcopy(pos, &moby->pos);
            pos[2] += 1.0f;
            h = probe_ground_height(pos, 0, 0.5f);
            if (moby->pos.z < h) {
                if (moby->state == PUFFOID_STATE_JUMP_HOME) {
                    moby->pos.z = h;
                    moby->state = PUFFOID_STATE_IDLE;
                } else {
                    moby->state = PUFFOID_STATE_WAIT_FOR_HOME_JUMP;
                    moby->pos.z = h - 1.0f;
                }
            }
        }
        break;
    case PUFFOID_STATE_IDLE:
        if (puffoid_vars->target_status != 2) {
            puffoid_vars->chase_heading_offset_radians =
                random_float_between(-puffoid_chase_heading_offset_limit_degrees_gp,
                                     puffoid_chase_heading_offset_limit_degrees_gp) *
                0.017453292f;
            moby->state = PUFFOID_STATE_CHASE_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if ((moby->anim_flags & 2) && random_integer_below(3) != 0 && moby->visible != 0 &&
                   fast_vec_dist(&moby->pos, D_L11_001677C0_4318) < 32.0f) {
            float a = random_angle_radians();
            float wander_radius = random_float_between(1.0f, 3.0f);
            puffoid_vars->wander_destination[0] = fast_cos(a) * wander_radius;
            puffoid_vars->wander_destination[1] = fast_sin(a) * wander_radius;
            puffoid_vars->wander_destination[2] = 0.0f;
            fast_vec_add(puffoid_vars->wander_destination,
                         puffoid_vars->wander_destination,
                         puffoid_vars->home);
            moby->state = PUFFOID_STATE_WANDER_NEAR_HOME;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case PUFFOID_STATE_WANDER_NEAR_HOME:
        update_moby_target_heading_and_conditional_movement((char *)moby,
                                                            puffoid_vars->wander_destination);
        if (puffoid_vars->target_status != 2) {
            puffoid_vars->chase_heading_offset_radians =
                random_float_between(-puffoid_chase_heading_offset_limit_degrees_gp,
                                     puffoid_chase_heading_offset_limit_degrees_gp) *
                0.017453292f;
            moby->state = PUFFOID_STATE_CHASE_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if ((moby->anim_flags & 2) && random_integer_below(3) == 0) {
            moby->state = PUFFOID_STATE_IDLE;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        }
        break;
    case PUFFOID_STATE_CHASE_TARGET:
        if ((moby->anim_flags & 2) && vector_distance_xy(&moby->pos, &target->pos) <
                                          puffoid_spray_target_distance_metric_threshold_gp) {
            moby->state = PUFFOID_STATE_SPRAY_ATTACK;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        } else {
            update_moby_target_heading_and_conditional_movement((char *)moby,
                                                                (float *)&target->pos);
        }
        if (puffoid_vars->target_status == 2) {
            moby->state = PUFFOID_STATE_RETURN_HOME;
        }
        break;
    case PUFFOID_STATE_REPOSITION_AND_FACE_TARGET:
        fast_vec_sub(pos, puffoid_vars->goal, &moby->pos);
        pos[2] = 0.0f;
        fast_vec_normalize(pos, pos, 2.5f);
        fast_vec_add(pos, pos, &moby->pos);
        if (test_path_segment_strict_crossing(puffoid_vars->path_index, &moby->pos, pos)) {
            float *rz = &moby->rot.z;
            float  target_facing_heading = fast_arc_tan(puffoid_vars->target->pos.x - moby->pos.x,
                                                       puffoid_vars->target->pos.y - moby->pos.y);
            approach_angle(rz,
                           &puffoid_vars->yaw_turn_velocity,
                           target_facing_heading,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_time * 12.566371f);
        } else {
            update_moby_target_heading_and_conditional_movement((char *)moby, puffoid_vars->goal);
        }
        if (moby->anim_flags & 2) {
            if (puffoid_vars->target_status != 2) {
                moby->state = PUFFOID_STATE_CHASE_TARGET;
            } else if (puffoid_vars->timer == 0) {
                moby->state = PUFFOID_STATE_RETURN_HOME;
            }
        }
        break;
    case PUFFOID_STATE_SPRAY_ATTACK:
        update_moby_target_heading_and_conditional_movement((char *)moby, (float *)&target->pos);
        if ((moby->anim_flags & 2) && puffoid_spray_target_distance_metric_threshold_gp <
                                          vector_distance_xy(&moby->pos, &target->pos)) {
            moby->state = PUFFOID_STATE_CHASE_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (puffoid_vars->target_status == 2) {
            moby->state = PUFFOID_STATE_RETURN_HOME;
        }
        qcopy(pos, &moby->pos);
        pos[2] += 0.333f;
        play_entity_sound(moby, pos, 1, D_L11_001622CC * 0.6f, 1.0f, 1.0f, 0, 1, 0);
        break;
    case PUFFOID_STATE_RETURN_HOME:
        if (vector_distance_xy(&moby->pos, puffoid_vars->home) < 1.0f) {
            moby->state = PUFFOID_STATE_IDLE;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
        } else if (puffoid_vars->target_status != 2) {
            puffoid_vars->chase_heading_offset_radians =
                random_float_between(-puffoid_chase_heading_offset_limit_degrees_gp,
                                     puffoid_chase_heading_offset_limit_degrees_gp) *
                0.017453292f;
            moby->state = PUFFOID_STATE_CHASE_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else {
            update_moby_target_heading_and_conditional_movement((char *)moby, puffoid_vars->home);
        }
        if ((moby->anim_flags & 2) && moby->prev_seq != 1) {
            blend_moby_animation(moby, 1, 0, 5);
        }
        break;
    case PUFFOID_STATE_KNOCKBACK: {
        int hit = FUN_L00_0025c698_4318(moby, &puffoid_vars->knockback);
        if (hit & 0x40) {
            char *path;
            if (moby->pos.z < 222.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            path = (char *)l11_paths[puffoid_vars->path_index];
            if (point_in_path_xy(&moby->pos, path + 0x10, *(int *)path) == 0) {
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = PUFFOID_STATE_IDLE;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
            return;
        }
        if (hit & 0x120) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case PUFFOID_STATE_DEATH_KNOCKBACK:
        if (FUN_L00_0025c698_4318(moby, &puffoid_vars->knockback) & 0x160) {
            clear_u64_value(pos);
            spawn_color_swapped_explosion_burst(moby, &moby->pos, -1, 0.5f, 10.0f);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP: {
        float basis[16];
        float dest[4];
        if (puffoid_vars->target_status != 2 && count_group_puffoids_in_states_13_to_17(moby) < 5) {
            struct Moby *carrier_moby =
                (struct Moby *)(D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8));
            transpose_rotation_matrix(basis, &carrier_moby->rot_row0);
            fast_vec_sub(pos, &moby->pos, &carrier_moby->pos);
            transform_vector(pos, pos, basis);
            if (pos[0] < puffoid_vars->carrier_jump_local_x_threshold && pos[0] > -3.1f) {
                float lo;
                float hi;
                int   scaled_animation_frames;
                dest[0] = pos[0];
                if (pos[1] > 0.0f) {
                    dest[1] = random_float_between(1.5f, 2.0f);
                } else {
                    dest[1] = random_float_between(-2.0f, -1.5f);
                }
                dest[2] = 1.0f;
                transform_vector_by_basis(dest, dest, &carrier_moby->rot_row0);
                fast_vec_add(dest, dest, &carrier_moby->pos);
                moby->state = PUFFOID_STATE_CARRIER_JUMP;
                if (moby->prev_seq != 1) {
                    blend_moby_animation(moby, 1, 0, 5);
                }
                fast_vec_sub(puffoid_vars->vel, dest, &moby->pos);
                scaled_animation_frames = scale_game_frames(0x3C);
                moby->anim_speed = (float)scaled_animation_frames *
                                   (puffoid_jump_horizontal_speed_gp * frame_time) /
                                   fast_vec_length(puffoid_vars->vel);
                set_vector_length_xy(puffoid_vars->vel,
                                     puffoid_vars->vel,
                                     puffoid_jump_horizontal_speed_gp * frame_time);
                puffoid_vars->vel[2] =
                    jump_speed_4318(&moby->pos,
                                    dest,
                                    0,
                                    puffoid_jump_horizontal_speed_gp * frame_time,
                                    -(l11_puffoid_jump_gravity * frame_time_sq));
                moby->rot.z = fast_arc_tan(puffoid_vars->vel[0], puffoid_vars->vel[1]);
            }
        }
        break;
    }
    case PUFFOID_STATE_CARRIER_JUMP: {
        char *cv =
            (char *)((struct Moby *)(D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8)))->pvars;
        fast_vec_add(&moby->pos, &moby->pos, puffoid_vars->vel);
        fast_vec_add(&moby->pos, &moby->pos, cv + 0x70);
        puffoid_vars->vel[2] -= l11_puffoid_jump_gravity * frame_time_sq;
        if (puffoid_vars->vel[2] < 0.0f) {
            qcopy(pos, &moby->pos);
            pos[2] += 1.0f;
            if (moby->pos.z < probe_ground_height(pos, 0, 0.5f)) {
                moby->anim_speed = 1.0f;
                moby->state = PUFFOID_STATE_CARRIER_IDLE;
            }
        }
        break;
    }
    case PUFFOID_STATE_CARRIER_IDLE:
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        if (puffoid_vars->target_status != 2) {
            puffoid_vars->chase_heading_offset_radians =
                random_float_between(-puffoid_chase_heading_offset_limit_degrees_gp,
                                     puffoid_chase_heading_offset_limit_degrees_gp) *
                0.017453292f;
            moby->state = PUFFOID_STATE_CARRIER_CHASE_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        }
        break;
    case PUFFOID_STATE_CARRIER_CHASE_TARGET:
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        update_moby_target_heading_and_conditional_movement((char *)moby, (float *)&target->pos);
        if ((moby->anim_flags & 2) && vector_distance_xy(&moby->pos, &target->pos) <
                                          puffoid_spray_target_distance_metric_threshold_gp) {
            moby->state = PUFFOID_STATE_CARRIER_SPRAY_ATTACK;
            if (moby->prev_seq != 2) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            }
        }
        if (puffoid_vars->target_status == 2) {
            moby->state = PUFFOID_STATE_CARRIER_IDLE;
        }
        break;
    case PUFFOID_STATE_CARRIER_SPRAY_ATTACK:
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        update_moby_target_heading_and_conditional_movement((char *)moby, (float *)&target->pos);
        if ((moby->anim_flags & 2) && puffoid_spray_target_distance_metric_threshold_gp <
                                          vector_distance_xy(&moby->pos, &target->pos)) {
            moby->state = PUFFOID_STATE_CARRIER_CHASE_TARGET;
            if (moby->prev_seq != 1) {
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            }
        } else if (puffoid_vars->target_status == 2) {
            moby->state = PUFFOID_STATE_CARRIER_IDLE;
        }
        qcopy(pos, &moby->pos);
        pos[2] += 0.333f;
        play_entity_sound(moby, pos, 1, D_L11_001622CC * 0.6f, 1.0f, 0.0f, 0, 1, 0);
        break;
    case PUFFOID_STATE_CARRIER_KNOCKBACK: {
        int hit;
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        hit = FUN_L00_0025c698_4318(moby, &puffoid_vars->knockback);
        if (hit & 0x40) {
            char *path;
            if (moby->pos.z < 222.0f) {
                mark_moby_for_removal(moby);
                return;
            }
            path = (char *)l11_paths[puffoid_vars->path_index];
            if (point_in_path_xy(&moby->pos, path + 0x10, *(int *)path) == 0) {
                mark_moby_for_removal(moby);
                return;
            }
            moby->state = PUFFOID_STATE_CARRIER_IDLE;
            if (moby->prev_seq != 0) {
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            }
            return;
        }
        if (hit & 0x120) {
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK:
        follow_carrier_4318(moby,
                            D_L11_0015FFD8_4318 + (puffoid_vars->carrier << 8),
                            &moby->pos,
                            &moby->rot,
                            &moby->pos,
                            &moby->rot);
        if (FUN_L00_0025c698_4318(moby, &puffoid_vars->knockback) & 0x160) {
            clear_u64_value(pos);
            spawn_color_swapped_explosion_burst(moby, &moby->pos, -1, 0.5f, 10.0f);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    case PUFFOID_STATE_FOLLOW_PATH: {
        float vel[4];
        float path_vertical_step_limit;
        float path_spray_distance_metric;
        qcopy(pos, (char *)l11_paths[puffoid_vars->path] + puffoid_vars->point * 16 + 0x10);
        if (puffoid_vars->carrier >= 0) {
            float h = ((struct Moby *)D_L11_0015FFD8_4318)[puffoid_vars->carrier].pos.z - 1.0f;
            if (h < pos[2]) {
                pos[2] = h;
            }
        }
        {
            float *rz = &moby->rot.z;
            float  path_point_heading = fast_arc_tan(pos[0] - moby->pos.x, pos[1] - moby->pos.y);
            approach_angle(rz,
                           &puffoid_vars->yaw_turn_velocity,
                           path_point_heading,
                           frame_time_sq * 12.566371f,
                           frame_time_sq * 12.566371f,
                           frame_time * 12.566371f);
        }
        vel[0] = fast_cos(moby->rot.z) * (frame_time + frame_time);
        vel[1] = fast_sin(moby->rot.z) * (frame_time + frame_time);
        vel[2] = pos[2] - moby->pos.z;
        path_vertical_step_limit = frame_time * 4.0f;
        if (vel[2] > path_vertical_step_limit) {
            vel[2] = path_vertical_step_limit;
        } else if (vel[2] < -path_vertical_step_limit) {
            vel[2] = -path_vertical_step_limit;
        }
        move_4318(moby, vel, 0.5f, 0.5f, 0.0f, 0x10);
        if (fast_vec_dist(pos, &moby->pos) < 1.0f) {
            puffoid_vars->point =
                (puffoid_vars->point + 1) % *(int *)(char *)l11_paths[puffoid_vars->path];
        }
        if (moby->anim_time == 0.0f) {
            path_spray_distance_metric = vector_distance_xy(&moby->pos, D_0013F3D0_4318);
            switch (moby->prev_seq) {
            case PUFFOID_PATH_ANIM_TRAVEL:
                if (path_spray_distance_metric < 6.0f) {
                    blend_moby_animation(moby, 5, 0, scale_game_frames(10));
                }
                break;
            case PUFFOID_PATH_ANIM_SPRAY_ATTACK:
                if (path_spray_distance_metric > 8.0f) {
                    blend_moby_animation(moby, 7, 0, scale_game_frames(10));
                } else if (path_spray_distance_metric < 4.0f) {
                    float spray_position[4];
                    qcopy(spray_position, &moby->pos);
                    spray_position[2] += 0.5f;
                    play_entity_sound(
                        moby, spray_position, 1, D_L11_001622CC * 0.55f, 1.0f, 1.0f, 0, 1, 0);
                }
                break;
            case PUFFOID_PATH_ANIM_ENTER_SPRAY_ATTACK:
                blend_moby_animation(moby, 6, 0, scale_game_frames(10));
                break;
            case PUFFOID_PATH_ANIM_EXIT_SPRAY_ATTACK:
                blend_moby_animation(moby, 4, 0, scale_game_frames(10));
                break;
            }
        }
        break;
    }
    case PUFFOID_STATE_WAIT_TO_RESUME_COMMAND:
        if (puffoid_vars->unkC8 != 0 && FUN_L00_002dc9f0_4318(moby, &puffoid_vars->knockback)) {
            moby->state = moby->command;
            puffoid_vars->unkC8 = 0;
        }
        break;
    }
    if (puffoid_vars->knockback.ground_bounces != 0.0f) {
        if (moby->state == PUFFOID_STATE_KNOCKBACK ||
            moby->state == PUFFOID_STATE_DEATH_KNOCKBACK ||
            moby->state == PUFFOID_STATE_CARRIER_KNOCKBACK ||
            moby->state == PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK) {
            for (i = 0; i < 3; i++) {
                float vel[4];
                float a;
                float smoke_horizontal_step;
                pos[0] = moby->pos.x + randf_sym(0.0f, 0.2f);
                pos[1] = moby->pos.y + randf_sym(0.0f, 0.2f);
                pos[2] = moby->pos.z + randf_sym(0.0f, 0.2f) + 0.8f;
                a = random_angle_radians();
                smoke_horizontal_step = random_float_between(0.2f, 1.3f) * frame_time;
                vel[0] = fast_cos(a) * smoke_horizontal_step;
                vel[1] = fast_sin(a) * smoke_horizontal_step;
                vel[2] = 0.0f;
                vel[2] = random_float_between(-0.2f, 0.4f) * frame_time;
                smoke_4318(pos, vel, random_float_between(2100.0f, 14700.0f), 222.5f);
            }
        }
    }
}

extern char *D_L11_0015FFD8;

extern float D_L11_001622CC __attribute__((sda));

#include "rnc/overlay/l11/mobys/puffoid_data/update_puffoid_damage_and_path_goal_3.h"

#include "rnc/overlay/l11/mobys/puffoid_globals.h"

extern float puffoid_hit_knockback_vertical_scale_gp __asm__("D_L11_001622D4") __attribute__((sda));

#include "rnc/overlay/l11/mobys/puffoid_data/update_puffoid_damage_and_path_goal_2.h"

extern float puffoid_death_knockback_vertical_scale_gp __asm__("D_L11_001622DC")
    __attribute__((sda));

#include "rnc/overlay/l11/mobys/puffoid_data/update_puffoid_damage_and_path_goal.h"

extern float l11_puffoid_horizontal_speed_limit __asm__("D_L11_001622FC") __attribute__((sda));

/* Private variables of the moby FUN_L11_00315968 updates (only the fields it uses). */
struct L11PatrolVars {
    u8                     pad0[0x20];
    f32                    hp; /* 0x20 */
    u8                     pad24[0x14];
    s32                    retimer; /* 0x38: nonzero restarts the timer at 0x244 */
    u8                     pad3C[0xD4];
    struct HitColorFlash   hit_flash;
    struct KnockbackMotion knockback; /* 0x120 */
    u8                     pad180[0x24];
    f32                    horizontal_movement_step_limit; /* 0x1A4 */
    u8                     pad1A8[0x28];
    Vec4f                  goal; /* 0x1D0 */
    u8                     pad1E0[0x30];
    struct Moby           *target;        /* 0x210 */
    s32                    target_status; /* 0x214 */
    u8                     pad218[0x18];
    s32                    path_index;  /* 0x230 */
    s32                    path2_index; /* 0x234 */
    u8                     pad238[4];
    f32                    range; /* 0x23C */
    u8                     pad240[4];
    s32                    timer; /* 0x244 */
    f32                    reach; /* 0x248 */
    u8                     pad24C[8];
    s32                    carrier; /* 0x254: listed moby index, -1: none */
};

/* What find_moby_hit reports about the moby's last hit. */
struct L11HitInfo {
    u8           pad0[0x10];
    Vec4         pos;  /* 0x10 */
    struct Moby *moby; /* 0x20: the moby that hit */
};

extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(void *, void *, s32, s32, s32, float) __asm__("FUN_L00_0025c558");
extern void  FUN_L00_00257470(void *, s32, s32);
extern void  start_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d458");
extern void  update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void  FUN_00212f90(void *, s32, s32, s32);
extern float scale_time(float) __asm__("FUN_001f96b0");
extern s32   FUN_L00_0025ff38(void *, void *, s32, s32, void *, s32, float);

/* Path patroller update: rides its carrier moby until dropped, takes hits (hit and death states), and follows its l11_paths path toward the hero. */
void       update_puffoid_damage_and_path_goal(struct Moby *moby) __asm__("FUN_L11_00315968");
extern int scale_game_frames(int);

void update_puffoid_damage_and_path_goal(struct Moby *moby) {
    enum {
        HIT_RESPONSE_NO_ADDITIONAL_REACTION = 0,
        HIT_RESPONSE_DEATH = 1,
        HIT_RESPONSE_DEATH_KNOCKBACK = 2,
        HIT_RESPONSE_KNOCKBACK = 3,
        HIT_RESPONSE_KNOCKBACK_ALTERNATE = 4,
        HIT_RESPONSE_KNOCKBACK_VARIANT = 5,
        HIT_RESPONSE_KNOCKBACK_REACTION = 6,
        HIT_RESPONSE_KNOCKBACK_ARC = 7,
        HIT_RESPONSE_KNOCKBACK_MOTION = 8,
        HIT_RESPONSE_NON_KNOCKBACK_REACTION = 9,
        HIT_RESPONSE_NON_KNOCKBACK_REACTION_ALTERNATE = 10,
        HIT_RESPONSE_COOLDOWN_NO_KNOCKBACK = 11
    };
    enum {
        HIT_PROCESSING_SKIP_DAMAGE_AND_REACTION = 1
    };
    struct L11PatrolVars *patrol_vars = (struct L11PatrolVars *)moby->pvars;
    Vec4                  position_scratch;
    s32                   hit_processing_status;
    float                 dmg;
    struct L11HitInfo    *hit;
    char                 *path;

    moby->scale = moby->pclass->scale * D_L11_001622CC;
    patrol_vars->horizontal_movement_step_limit = l11_puffoid_horizontal_speed_limit * frame_time;
    if (patrol_vars->carrier != -1 && moby->state != PUFFOID_STATE_DEATH_KNOCKBACK &&
        moby->state != PUFFOID_STATE_KNOCKBACK &&
        moby->state != PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK &&
        moby->state != PUFFOID_STATE_CARRIER_KNOCKBACK && moby->state != PUFFOID_STATE_INITIALIZE &&
        moby->state != PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP &&
        moby->state != PUFFOID_STATE_CARRIER_JUMP) {
        if (moby->pos.z < ((struct Moby *)D_L11_0015FFD8)[patrol_vars->carrier].pos.z + 0.5f) {
            spawn_color_swapped_explosion_burst(moby, &moby->pos, -1, 0.5f, 10.0f);
            mark_moby_for_removal(moby);
        }
    }
    if (moby->state == PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP && patrol_vars->carrier != -1) {
        if (is_moby_in_state_five(&((struct Moby *)D_L11_0015FFD8)[patrol_vars->carrier]) != 0 &&
            moby->visible == 0) {
            mark_moby_for_removal(moby);
            return;
        }
        if (((struct Moby *)D_L11_0015FFD8)[patrol_vars->carrier].state >= 2) {
            moby->visible = 1;
            moby->flags &= ~1;
            moby->collision = moby->pclass->collision;
        }
    }
    dmg = 0.0f;
    hit = find_moby_hit(moby, 0x330000, 0);
    {
        s32 kind = resolve_moby_hit_response(
            moby, hit, &patrol_vars->hp, 0, &hit_processing_status, &dmg, 0, 4);
        if (hit_processing_status != HIT_PROCESSING_SKIP_DAMAGE_AND_REACTION &&
            moby->state != PUFFOID_STATE_DEATH_KNOCKBACK) {
            patrol_vars->hp -= dmg;
            if (hit->moby->oclass == 0x47) {
                kind = HIT_RESPONSE_KNOCKBACK;
            }
            if (patrol_vars->hp <= 0.0f) {
                kind = HIT_RESPONSE_DEATH;
            }
            patrol_vars->knockback.collision_radius_1024ths = truncate_float_to_s32(512.0f);
            patrol_vars->knockback.collision_z_offset = 0.2f;
            patrol_vars->knockback.speed_approach_step = 0.0f;
            patrol_vars->knockback.gravity =
                puffoid_hit_motion_time_squared_scale_gp * frame_time_sq;
            patrol_vars->knockback.flags = 9;
            patrol_vars->knockback.unk3D = 0;
            switch (kind) {
            case HIT_RESPONSE_NO_ADDITIONAL_REACTION:
                break;
            case HIT_RESPONSE_NON_KNOCKBACK_REACTION:
            case HIT_RESPONSE_NON_KNOCKBACK_REACTION_ALTERNATE:
                patrol_vars->hit_flash.flash_red = 0xFA;
                break;
            case HIT_RESPONSE_KNOCKBACK:
            case HIT_RESPONSE_KNOCKBACK_ALTERNATE:
            case HIT_RESPONSE_KNOCKBACK_VARIANT:
            case HIT_RESPONSE_KNOCKBACK_REACTION:
            case HIT_RESPONSE_KNOCKBACK_ARC:
            case HIT_RESPONSE_KNOCKBACK_MOTION: {
                float yaw;
                patrol_vars->knockback.speed =
                    puffoid_hit_knockback_horizontal_scale_gp * frame_time;
                patrol_vars->knockback.vertical_speed =
                    puffoid_hit_knockback_vertical_scale_gp * frame_time;
                patrol_vars->knockback.apex_animation_frame = 7.5f;
                patrol_vars->knockback.landing_animation_frame = 11.0f;
                yaw = fast_arc_tan(moby->pos.x - hit->moby->pos.x, moby->pos.y - hit->moby->pos.y);
                position_scratch.q = hit->pos.q;
                trooper_knockback_arc(&position_scratch,
                                      &yaw,
                                      &patrol_vars->knockback.speed,
                                      &patrol_vars->knockback.vertical_speed);
                trooper_start_knockback(moby, &patrol_vars->knockback, 3, 1, 0, yaw);
                if (moby->state < PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP) {
                    moby->state = PUFFOID_STATE_KNOCKBACK;
                } else {
                    moby->state = PUFFOID_STATE_CARRIER_KNOCKBACK;
                }
                patrol_vars->hit_flash.flash_red = 0x78;
                break;
            }
            case HIT_RESPONSE_DEATH:
            case HIT_RESPONSE_DEATH_KNOCKBACK: {
                float yaw;
                moby->flags &= ~0x1000;
                patrol_vars->knockback.gravity =
                    puffoid_hit_motion_time_squared_scale_gp * frame_time_sq;
                patrol_vars->knockback.speed =
                    puffoid_death_knockback_horizontal_scale_gp * frame_time;
                patrol_vars->knockback.vertical_speed =
                    puffoid_death_knockback_vertical_scale_gp * frame_time;
                patrol_vars->knockback.apex_animation_frame = 7.5f;
                patrol_vars->knockback.landing_animation_frame = 11.0f;
                yaw = fast_arc_tan(moby->pos.x - hit->moby->pos.x, moby->pos.y - hit->moby->pos.y);
                position_scratch.q = hit->pos.q;
                trooper_knockback_arc(&position_scratch,
                                      &yaw,
                                      &patrol_vars->knockback.speed,
                                      &patrol_vars->knockback.vertical_speed);
                trooper_start_knockback(moby, &patrol_vars->knockback, 3, 1, 0, yaw);
                if (moby->state < PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP) {
                    moby->state = PUFFOID_STATE_DEATH_KNOCKBACK;
                } else {
                    moby->state = PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK;
                }
                patrol_vars->hit_flash.flash_red = 0xF0;
                FUN_L00_00257470(moby, 0, -1);
                break;
            }
            case HIT_RESPONSE_COOLDOWN_NO_KNOCKBACK:
                break;
            }
            start_hit_color_flash(moby, &patrol_vars->hit_flash);
        }
    }
    moby->hit_slot = 0xFF;
    update_hit_color_flash(moby, &patrol_vars->hit_flash);
    if (patrol_vars->timer != 0) {
        if (moby->state == PUFFOID_STATE_IDLE ||
            (moby->state == PUFFOID_STATE_WANDER_NEAR_HOME && (moby->anim_flags & 2))) {
            moby->state = PUFFOID_STATE_REPOSITION_AND_FACE_TARGET;
            if (moby->prev_seq != 2) {
                FUN_00212f90(moby, 2, 0, scale_game_frames(10));
            }
        }
    }
    if (patrol_vars->retimer != 0) {
        patrol_vars->timer =
            truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
        patrol_vars->retimer = 0;
    }
    fast_dec_timer(&patrol_vars->timer);
    if (patrol_vars->timer != 0) {
        patrol_vars->reach = patrol_vars->range + 64.0f;
    } else {
        patrol_vars->reach = patrol_vars->range;
    }
    path = (char *)l11_paths[patrol_vars->path_index];
    if (FUN_L00_0025ff38(
            moby, &patrol_vars->goal, 0, 0, path + 0x10, *(s32 *)path, patrol_vars->reach) != 2) {
        Vec4f *goal_check_position;
        if (moby->state == PUFFOID_STATE_WAIT_FOR_HOME_JUMP) {
            goal_check_position = (Vec4f *)((char *)l11_paths[patrol_vars->path2_index] + 0x20);
        } else {
            goal_check_position = &moby->pos;
        }
        qcopy(&position_scratch, goal_check_position);
        if (vector_distance_xy(position_scratch.f, &patrol_vars->goal.x) > patrol_vars->reach ||
            position_scratch.f[2] - patrol_vars->goal.z > 3.0f) {
            patrol_vars->target_status = 2;
        }
    }
    if (patrol_vars->target == 0) {
        patrol_vars->target = hero.moby;
        qcopy(&patrol_vars->goal, &hero.motion.pos);
    }
}

extern float frame_scale_inv_macro[] __asm__("D_0015ED68") MACRO_ADDR;
extern void  move_moby_with_ground_and_collision_checks(
     char *, char *, float *, float *, float) __asm__("FUN_L00_00258b50");

void update_moby_target_heading_and_conditional_movement(
    char *moby, float *target_position) __asm__("FUN_L11_00315f58");

void update_moby_target_heading_and_conditional_movement(char *moby, float *target_position) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    float planar_heading_vector[4];
    float helper_scratch_buffer[4];
    float target_heading_or_record_value;
    target_heading_or_record_value =
        fast_add_rotations(fast_arc_tan(target_position[0] - MOBY(moby)->pos.x,
                                        target_position[1] - MOBY(moby)->pos.y),
                           *(float *)(moby_vars + 0x24C));
    approach_angle(moby + 0x48,
                   moby_vars + 0x250,
                   target_heading_or_record_value,
                   frame_time_sq * 8.726646f,
                   frame_time_sq * 8.726646f,
                   frame_scale_inv_macro[1] * 12.566371f);
    target_heading_or_record_value = compute_interpolated_record_value(moby);
    if (MOBY(moby)->seq == MOBY(moby)->prev_seq && 8.0f < target_heading_or_record_value &&
        target_heading_or_record_value < 22.0f) {
        *(float *)(moby_vars + 0x1A4) = l11_puffoid_horizontal_speed_limit * frame_time;
        planar_heading_vector[0] = fast_cos(MOBY(moby)->rot.z) * 2.0f;
        planar_heading_vector[1] = fast_sin(MOBY(moby)->rot.z) * 2.0f;
        *(int *)&planar_heading_vector[2] = 0;
        move_moby_with_ground_and_collision_checks(
            moby, moby_vars + 0x180, planar_heading_vector, helper_scratch_buffer, 1.0f);
    }
}

extern short *D_L11_001AC240[];

/* Counts the listed mobys of class 0x4DE in states 13 to 17. */
short count_group_puffoids_in_states_13_to_17(struct Moby *self) __asm__("FUN_L11_00316090");

short count_group_puffoids_in_states_13_to_17(struct Moby *group_source_moby) {
    unsigned short *moby_list_entry;
    short           matching_moby_count;
    char           *level_moby_array_base;

    if (group_source_moby->group == 0xFF)
        return 0;
    moby_list_entry = (unsigned short *)D_L11_001AC240[group_source_moby->group];
    matching_moby_count = 0;
    if (moby_list_entry == 0)
        return 0;
    level_moby_array_base = D_L11_0015FFD8;
    for (;;) {
        struct Moby *moby =
            (struct Moby *)(level_moby_array_base + ((*moby_list_entry & 0x7FFF) << 8));
        if (moby->oclass == 0x4DE) {
            if (moby->state >= PUFFOID_STATE_CARRIER_JUMP) {
                if (moby->state < PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK)
                    matching_moby_count++;
            }
        }
        if ((short)*moby_list_entry++ < 0)
            return matching_moby_count;
    }
}

/* True when the moby is in state 0x14, 0xB, 0x12 or 0xC. */
s32 is_moby_in_bob_target_exclusion_state(const u8 *moby) __asm__("FUN_L11_00316128");

s32 is_moby_in_bob_target_exclusion_state(const u8 *moby) {
    u32 moby_state = moby[0x20];
    if (moby_state == PUFFOID_STATE_WAIT_TO_RESUME_COMMAND ||
        moby_state == PUFFOID_STATE_DEATH_KNOCKBACK ||
        moby_state == PUFFOID_STATE_CARRIER_DEATH_KNOCKBACK ||
        moby_state == PUFFOID_STATE_WAIT_FOR_CARRIER_JUMP) {
        return 1;
    }
    return 0;
}

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

/* Grabbed check: while held, park the moby in state 0x14 (remembering its state) unless it cannot be grabbed; restore the saved state once released. */
int process_puffoid_suck_cannon_suction(struct Moby *moby) __asm__("FUN_L11_00316160");

int process_puffoid_suck_cannon_suction(struct Moby *moby) {
    int grab_check_result = process_moby_suck_cannon_suction(moby);
    if (grab_check_result != 0) {
        if (moby->state == PUFFOID_STATE_IDLE || moby->state == PUFFOID_STATE_CHASE_TARGET ||
            moby->state == PUFFOID_STATE_SPRAY_ATTACK || moby->state == PUFFOID_STATE_RETURN_HOME ||
            moby->state == PUFFOID_STATE_CARRIER_IDLE ||
            moby->state == PUFFOID_STATE_CARRIER_CHASE_TARGET ||
            moby->state == PUFFOID_STATE_CARRIER_SPRAY_ATTACK ||
            moby->state == PUFFOID_STATE_WAIT_TO_RESUME_COMMAND) {
            if (moby->state != PUFFOID_STATE_WAIT_TO_RESUME_COMMAND) {
                moby->command = moby->state;
                moby->state = PUFFOID_STATE_WAIT_TO_RESUME_COMMAND;
            }
        } else {
            grab_check_result = 0;
        }
    } else if (moby->state == PUFFOID_STATE_WAIT_TO_RESUME_COMMAND) {
        moby->state = moby->command;
    }
    return grab_check_result;
}

/* Ported from rac1-decomp (src/overlays/l11_pokitaru/vendor_00312BD8.c: func_L11_00317680), where it is exact; names translated to the US level program. */

extern void *store_moby_in_suck_cannon(void *) __asm__("FUN_L00_002dbb20");

void *moby_park_state_on_find(struct Moby *arg) __asm__("FUN_L11_00316210");

void *moby_park_state_on_find(struct Moby *suction_moby) {
    void *found = store_moby_in_suck_cannon(suction_moby);
    if (found != 0) {
        unsigned char state = suction_moby->state;
        if (state != PUFFOID_STATE_WAIT_TO_RESUME_COMMAND) {
            suction_moby->command = state;
            suction_moby->state = PUFFOID_STATE_WAIT_TO_RESUME_COMMAND;
        }
    } else if (suction_moby->state == PUFFOID_STATE_WAIT_TO_RESUME_COMMAND) {
        suction_moby->state = suction_moby->command;
    }
    return found;
}
