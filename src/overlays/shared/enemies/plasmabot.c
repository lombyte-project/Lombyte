/* Shared moby class plasmabot: its update, path walk, aiming, beam, hit reaction and effects. */
#include "types.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random/random_spherical_offset.h"
#include "rnc/overlay/shared/rendering/effects/beam_slots.h"
#include "rnc/overlay/shared/rendering/effects/particles.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"
#include "rnc/overlay/shared/enemies/plasmabot_hatch.h"
#include "rnc/overlay/shared/enemies/plasmabot_death_debris.h"
#include "rnc/overlay/shared/enemies/path_or_ballistic_debris.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/sdk/debug.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B44B8), where it is exact; names translated to the US level program. */

enum {
    PLASMABOT_INITIALIZE = 0,
    PLASMABOT_WAIT_TO_START_PATH_TRAVERSAL = 1,
    PLASMABOT_START_PATH_TRAVERSAL = 2,
    PLASMABOT_FOLLOW_PATH_FORWARD = 3,
    PLASMABOT_AIM_AT_TARGET = 4,
    PLASMABOT_WAIT_ANIMATION_BEFORE_AIM_SWEEP = 5,
    PLASMABOT_SWEEP_AIM_AND_TRACE_LINE = 6,
    PLASMABOT_WAIT_ANIMATION_BEFORE_AIM = 7,
    PLASMABOT_FOLLOW_PATH_BACKWARD = 8,
    PLASMABOT_RETURN_TO_PATH_START = 9,
    PLASMABOT_WAIT_ANIMATION_BEFORE_RESUMING_AIM = 10,
    PLASMABOT_FALL_THEN_EXPLODE = 11,
    PLASMABOT_SPAWN_DEBRIS_AND_REMOVE = 12,
    PLASMABOT_FACE_TARGET_AND_WAIT = 13
};

enum {
    HIT_KIND_SKIP_DAMAGE_AND_REACTION = 1
};

typedef struct PlasmabotVars {
    u8  pad0[0x20];
    f32 health;
    s16 max_health;
    s16 stagger_reaction_duration_frames;
    s8  hit_response_class;
    s8  unk29;
    u8  pad2A[0x6];
    f32 unk30;
    u8  pad34[0xc];
    u8  unk40[0x18];
    s8  unk58;
    u8  pad59[0x1];
    s8  unk5A;
    u8  pad5B[0x5];
    u8  motion[0x18];
    f32 knockback_speed;
    f32 knockback_vertical_speed;
    s32 knockback_collision_radius;
    u8  pad84[0x4];
    f32 knockback_collision_z_offset;
    u8  pad8C[0x24];
    f32 knockback_apex_animation_frame;
    f32 knockback_landing_animation_frame;
    u8  padB8[0x8];
    u8  trooper_anim[0x10];
    u8  path_search_point[0x40];
    s32 target; /* 0x110: target moby */
    s32 target_selection_status;
    u8  pad118[0x8];
    u8  aim_joint_manipulator[0x10];
    u8  aim_joint_rotation[0x30];
    u8  manipulator_attachment_header[0x10];
    u8  aim_quaternion_storage[0x70];
    u8  unk1E0[0x10];
    u8  nozzle_position[0x10];
    u8  unk200[0x10];
    u8  beam_start_position_storage[0x10];
    u8  beam_end_position_storage[0x14];
    f32 target_range;
    u8  pad238[0x4];
    s32 heading_angular_velocity;
    s32 path_index; /* 0x240: into l14_paths, -1: none */
    s32 target_path_index;
    s16 path_traversal_delay_timer;
    u8  pad24A[0x2];
    s32 path_segment_index;
    f32 path_offset_and_spring_velocities[4];
    s16 beam_cooldown_timer;
    s16 aim_sweep_timer;
    s32 hatch_index;
    f32 aim_yaw_offset;
    s32 aim_yaw_angular_velocity;
    s32 aim_joint_angle;
    s32 aim_joint_angular_velocity;
    f32 aim_sweep_center_angle;
    f32 path_traversal_direction;
    s32 aim_pitch_angle;
    s32 aim_pitch_angular_velocity;
    s32 unk288;
} PlasmabotVars;

extern char D_0013E533[];
extern int  FUN_L00_0025ff38(char *, char *, float, int, int, int *, int);

void update_plasmabot_target(char *moby) __asm__("FUN_L14_002b3268");

void update_plasmabot_target(char *moby) {
    enum {
        PLASMABOT_TARGET_STATUS_REJECTED = 2
    };
    PlasmabotVars *plasmabot_private_vars = (PlasmabotVars *)MOBY(moby)->pvars;
    int *target_filter_record = ((int *)l14_paths[plasmabot_private_vars->target_path_index]);
    if (FUN_L00_0025ff38(moby,
                         &plasmabot_private_vars->path_search_point,
                         plasmabot_private_vars->target_range,
                         0,
                         0,
                         target_filter_record + 4,
                         *target_filter_record) != 2) {
        if (plasmabot_private_vars->target_range <
            vector_distance_xy(&MOBY(moby)->pos, &plasmabot_private_vars->path_search_point)) {
            plasmabot_private_vars->target_selection_status = PLASMABOT_TARGET_STATUS_REJECTED;
        }
    }
    if (plasmabot_private_vars->target == 0) {
        plasmabot_private_vars->target = *(int *)(D_0013E533 + 0x2E9D);
    }
}

/* 0x002b3318, 24 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Manipulate moby flags and clear field
void deactivate_plasmabot(char *moby) __asm__("FUN_L14_002b3318");

void deactivate_plasmabot(char *moby) {
    unsigned short *field = (unsigned short *)&MOBY(moby)->flags;
    *field = (*field | 0x41) & 0xEFFF;
    MOBY(moby)->collision = 0;
}

/* 0x002b3330, 36 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
// Clears two flag bits, sets another, and copies a word from the moby's linked record.
void activate_plasmabot(char *moby) __asm__("FUN_L14_002b3330");

void activate_plasmabot(char *moby) {
    unsigned short *field = (unsigned short *)&MOBY(moby)->flags;
    int             linked_record_value;
    *field &= 0xFFBE;
    linked_record_value = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
    *field |= 0x1000;
    MOBY(moby)->collision = linked_record_value;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B45A8), where it is exact; names translated to the US level program. */

void initialize_plasmabot_path_state(char *moby) __asm__("FUN_L14_002b3358");

void initialize_plasmabot_path_state(char *moby) {
    PlasmabotVars *moby_vars = (PlasmabotVars *)MOBY(moby)->pvars;
    char          *path;
    int            i;
    *(int *)&moby_vars->path_offset_and_spring_velocities = 0;
    moby_vars->path_segment_index = 0;
    moby_vars->path_offset_and_spring_velocities[3] =
        moby_vars->path_offset_and_spring_velocities[0];
    moby_vars->path_offset_and_spring_velocities[2] =
        moby_vars->path_offset_and_spring_velocities[0];
    moby_vars->path_offset_and_spring_velocities[1] =
        moby_vars->path_offset_and_spring_velocities[0];
    if (moby_vars->path_index == -1)
        return;
    path = (char *)l14_paths[moby_vars->path_index];
    for (i = 0; i < *(int *)path; i++) {
        int point_offset = i * 16;
        *(float *)(path + point_offset + 0x1C) = fast_vec_dist(
            path + (point_offset + 0x10), path + (((i + 1) % *(int *)path) * 16 + 0x10));
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B47C8), where it is exact; names translated to the US level program. */

#include "rnc/overlay/shared/enemies/plasmabot_globals.h"

extern float plasmabot_planar_velocity_base_scale_gp __asm__("D_L14_001614DC") __attribute__((sda));
void         scale_and_apply_plasmabot_planar_velocity(char *moby) __asm__("FUN_L14_002b3578");

void scale_and_apply_plasmabot_planar_velocity(char *moby) {
    float planar_velocity_scale =
        (plasmabot_planar_velocity_base_scale_gp - 1.0f) * frame_scale + 1.0f;
    PlasmabotVars *moby_vars = (PlasmabotVars *)MOBY(moby)->pvars;
    moby_vars->path_offset_and_spring_velocities[1] =
        moby_vars->path_offset_and_spring_velocities[1] * planar_velocity_scale;
    moby_vars->path_offset_and_spring_velocities[2] =
        moby_vars->path_offset_and_spring_velocities[2] * planar_velocity_scale;
    MOBY(moby)->pos.x = MOBY(moby)->pos.x + moby_vars->path_offset_and_spring_velocities[1];
    MOBY(moby)->pos.y = MOBY(moby)->pos.y + moby_vars->path_offset_and_spring_velocities[2];
}
/* steers the pitch angle of a moby toward a target and updates the joint */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4A00), where it is exact; names translated to the US level program. */

extern void build_axis_rotation_quaternion(void *, int, float) __asm__("FUN_L00_001fff28");

void update_plasmabot_aim_joint_rotation(char *moby, float x) __asm__("FUN_L14_002b37b0");

void update_plasmabot_aim_joint_rotation(char *moby, float x) {
    PlasmabotVars *moby_vars = (PlasmabotVars *)MOBY(moby)->pvars;
    float          target_joint_angle = -moby_vars->aim_yaw_offset;
    float          heading_angle_delta = fast_subtract_rotations(x, MOBY(moby)->rot.z);
    target_joint_angle = fast_add_rotations(target_joint_angle, -heading_angle_delta);
    if (target_joint_angle > 1.2217304706573486f)
        target_joint_angle = 1.2217304706573486f;
    else if (target_joint_angle < -1.2217304706573486f)
        target_joint_angle = -1.2217304706573486f;
    {
        float k = frame_time_sq * 12.566370964050293f;
        approach_angle((float *)(&moby_vars->aim_joint_angle),
                       (float *)(&moby_vars->aim_joint_angular_velocity),
                       target_joint_angle,
                       k,
                       k,
                       frame_time * 6.2831854820251465f);
    }
    build_axis_rotation_quaternion(
        &moby_vars->aim_joint_rotation, 2, *(float *)&moby_vars->aim_joint_angle);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4AE0), where it is exact; names translated to the US level program. */

extern char  *D_L14_ffd0_e_macro[] __asm__("D_L14_0015FFD0") MACRO_ADDR;
extern short *D_L14_001ABF40[];

#include "rnc/overlay/shared/enemies/plasmabot_data/fun_l14_002b3890.h"

void reset_listed_plasmabot_beam_cooldowns(int selected_moby_list_index) __asm__("FUN_L14_002b3890");

void reset_listed_plasmabot_beam_cooldowns(int selected_moby_list_index) {
    short *listed_moby_entry_cursor;
    if (selected_moby_list_index != -1) {
        listed_moby_entry_cursor = D_L14_001ABF40[selected_moby_list_index];
        if (listed_moby_entry_cursor != 0) {
            do {
                char *moby = D_L14_ffd0_e_macro[2] +
                             ((*(unsigned short *)listed_moby_entry_cursor & 0x7FFF) << 8);
                PlasmabotVars *listed_moby_private_vars = (PlasmabotVars *)MOBY(moby)->pvars;
                if (MOBY(moby)->oclass == 0x1C) {
                    listed_moby_private_vars->beam_cooldown_timer =
                        scale_game_frames(plasmabot_beam_cooldown_frames);
                }
            } while (*listed_moby_entry_cursor++ >= 0);
        }
    }
}

/* Plasmabot update: validates its path and target, walks the path, turns to aim at its owner's target and fires, dies or despawns. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B2A28), where it is exact; names translated to the US level program. */

typedef struct {
    float          x;
    float          y;
    float          scale;
    float          distance;
    char          *moby;
    int            flags;
    unsigned char  mode;
    unsigned char  enabled;
    unsigned short oclass;
    float          strength;
    int            count;
} LineInfo;

extern char  D_001413F4[];
extern char  plasmabot_missing_intro_path_format[] __asm__("D_L14_001FBB60");
extern char  plasmabot_empty_intro_path_format[] __asm__("D_L14_001FBBA0");
extern char  plasmabot_missing_bound_path_format[] __asm__("D_L14_001FBBF8");
extern char  plasmabot_empty_bound_path_format[] __asm__("D_L14_001FBC38");
extern char  plasmabot_missing_hatch_index_format[] __asm__("D_L14_001FBC90");
extern float probe_ground_height(float, void *, int);
extern int   D_L14_0015F5C4;
extern int   test_path_segment_strict_crossing(int, void *, void *) __asm__("FUN_L00_00261968");
extern int   release_beam_slot_by_id(char *) __asm__("FUN_L00_002e2040");
extern int   FUN_L14_002b39a0_c(unsigned char *) __asm__("FUN_L14_002b39a0");
extern float l14_plasmabot_beam_trace_endpoint_distance __asm__("D_L14_001614E4")
    __attribute__((sda));
extern float l14_plasmabot_aim_sweep_half_angle __asm__("D_L14_001614E8") __attribute__((sda));
extern int   l14_plasmabot_aim_sweep_duration_frames __asm__("D_L14_001614EC") __attribute__((sda));
extern float l14_plasmabot_beam_origin_offset_distance __asm__("D_L14_001614F4")
    __attribute__((sda));
extern u8   D_L14_00174560[];
extern void FUN_L00_0024f7c8(char *, int, void *);
extern void FUN_L00_00257470(void *, int, int);
extern void FUN_L00_0025e450_c(struct Moby *,
                               void *,
                               void *,
                               float,
                               float,
                               int,
                               int,
                               int,
                               float,
                               float,
                               float,
                               float,
                               int,
                               float,
                               int,
                               int,
                               int,
                               int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00263ac8(float, char *, int, char *);
extern void build_moby_bone_transform_c(char *, int, void *) __asm__("build_moby_bone_transform");
float       update_plasmabot_aim_quaternion(char *moby,
                                            int   flag,
                                            float angle) __asm__("FUN_L14_002b35d8");
int         follow_plasmabot_path(char *moby) __asm__("FUN_L14_002b3418");
int  count_listed_plasmabots_preparing_or_sweeping_beam(int index) __asm__("FUN_L14_002b3920");
void FUN_L14_002b2600_c(struct Moby *m) __asm__("FUN_L14_002b2600");
void FUN_L14_002b2ed8_c(struct Moby *m) __asm__("FUN_L14_002b2ed8");
void process_plasmabot_hit_reaction(struct Moby *m) __asm__("FUN_L14_002b2f60");
void update_plasmabot_aim_joint_rotation(char *moby, float angle) __asm__("FUN_L14_002b37b0");
void reset_listed_plasmabot_beam_cooldowns(int idx) __asm__("FUN_L14_002b3890");

#define call_4BF0(m) FUN_L14_002b39a0_c((unsigned char *)(m))
#define call_CE58    ((float (*)(float *, float, float *, float, float, float))approach_angle)
#define call_F4A8      \
    ((void (*)(char *, \
               void *, \
               void *, \
               float,  \
               float,  \
               int,    \
               int,    \
               int,    \
               float,  \
               float,  \
               float,  \
               float,  \
               int,    \
               float,  \
               int,    \
               int,    \
               int,    \
               int))FUN_L00_0025e450_c)
#define call_D678 ((void (*)(char *))mark_moby_for_removal)

void plasmabot_update(char *moby) __asm__("FUN_L14_002b17d8");

/* Moby class 28 */
void plasmabot_update(char *moby) {
    PlasmabotVars *plasmabot_update_private_vars;
    char          *plasmabot_aim_target_moby;
    int            state;
    float          bone_transform_scratch[4][4];
    float          direction[4];
    float          end[4];
    LineInfo       beam_line_trace_descriptor;
    int            status;

    plasmabot_update_private_vars = (PlasmabotVars *)MOBY(moby)->pvars;
    plasmabot_aim_target_moby = 0;
    FUN_L00_00263ac8(2.5f, moby, 2, &plasmabot_update_private_vars->aim_joint_manipulator);
    if (plasmabot_update_private_vars->unk288 != 0) {
        if (((unsigned char *)D_001413F4)[0] != 2) {
            MOBY(moby)->flags |= 0x41;
            MOBY(moby)->collision = 0;
            if (((unsigned char *)moby)[0x53] == 5) {
                release_beam_slot_by_id(moby);
                FUN_L00_00257470(moby, 0, -1);
                {
                    call_D678(moby);
                    return;
                }
            }
            ((unsigned char *)moby)[0x30] = 0xFF;
            MOBY(moby)->flags &= 0xEFFF;
            return;
        }
        MOBY(moby)->flags &= 0xFFBE;
        if (((unsigned char *)moby)[0x20] != 0xB) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
        }
        ((unsigned char *)moby)[0x30] = 0x40;
        MOBY(moby)->flags |= 0x1000;
    }
    if (((unsigned char *)moby)[0x20] != 0) {
        process_plasmabot_hit_reaction(moby);
        update_plasmabot_target(moby);
        FUN_L14_002b2ed8_c((struct Moby *)moby);
        plasmabot_aim_target_moby = *(char **)(&plasmabot_update_private_vars->target);
    }
    if (D_L14_0015F5C4 == 2) {
        ((unsigned char *)moby)[0x31] = 0;
        MOBY(moby)->flags |= 1;
    } else if (((unsigned char *)moby)[0x20] != 1) {
        ((unsigned char *)moby)[0x31] = 1;
        MOBY(moby)->flags &= 0xFFFE;
    }
    switch (((unsigned char *)moby)[0x20]) {
    case PLASMABOT_INITIALIZE: {
        if (((unsigned char *)plasmabot_update_private_vars)[0x24A] != 0) {
            if (plasmabot_update_private_vars->path_index == -1) {
                DebugPrint(plasmabot_missing_intro_path_format, ((short)MOBY(moby)->save_id));
                {
                    call_D678(moby);
                    return;
                }
            }
            if (*((int *)l14_paths[plasmabot_update_private_vars->path_index]) == 0) {
                DebugPrint(plasmabot_empty_intro_path_format, ((short)MOBY(moby)->save_id));
                {
                    call_D678(moby);
                    return;
                }
            }
        }
        if (plasmabot_update_private_vars->target_path_index == -1) {
            DebugPrint(plasmabot_missing_bound_path_format, ((short)MOBY(moby)->save_id));
            {
                call_D678(moby);
                return;
            }
        }
        if (*((int *)l14_paths[plasmabot_update_private_vars->target_path_index]) == 0) {
            DebugPrint(plasmabot_empty_bound_path_format, ((short)MOBY(moby)->save_id));
            {
                call_D678(moby);
                return;
            }
        }
        if (((unsigned char *)plasmabot_update_private_vars)[0x24A] != 0 &&
            plasmabot_update_private_vars->hatch_index < 0) {
            DebugPrint(plasmabot_missing_hatch_index_format, ((short)MOBY(moby)->save_id));
            {
                call_D678(moby);
                return;
            }
        }
        if (random_integer_below(2) != 0) {
            MOBY(moby)->flags |= 0x8000;
        }
        initialize_plasmabot_path_state(moby);
        if (plasmabot_update_private_vars->path_index != -1) {
            qcopy(&MOBY(moby)->pos,
                  (char *)l14_paths[plasmabot_update_private_vars->path_index] + 0x10);
        }
        plasmabot_update_private_vars->hit_response_class = 1;
        plasmabot_update_private_vars->unk29 = 0;
        plasmabot_update_private_vars->health = 2.0f;
        plasmabot_update_private_vars->unk30 = 1.25f;
        plasmabot_update_private_vars->max_health = 2;
        plasmabot_update_private_vars->unk58 = truncate_float_to_s32(15.0f);
        plasmabot_update_private_vars->unk5A = truncate_float_to_s32(16.0f);
        plasmabot_update_private_vars->path_traversal_direction = 1.0f;
        plasmabot_update_private_vars->path_traversal_delay_timer = 0;
        plasmabot_update_private_vars->heading_angular_velocity = 0;
        plasmabot_update_private_vars->beam_cooldown_timer = 0;
        *(int *)&plasmabot_update_private_vars->aim_yaw_offset = 0;
        plasmabot_update_private_vars->aim_yaw_angular_velocity = 0;
        plasmabot_update_private_vars->aim_pitch_angle = 0;
        plasmabot_update_private_vars->aim_pitch_angular_velocity = 0;
        plasmabot_update_private_vars->aim_joint_angle = 0;
        plasmabot_update_private_vars->aim_joint_angular_velocity = 0;
        if (((unsigned char *)plasmabot_update_private_vars)[0x121] == 0) {
            attach_manipulator(moby, 2, &plasmabot_update_private_vars->aim_joint_manipulator);
        }
        if (((unsigned char *)plasmabot_update_private_vars)[0x161] == 0) {
            attach_manipulator(
                moby, 3, &plasmabot_update_private_vars->manipulator_attachment_header);
        }
        if (((unsigned char *)plasmabot_update_private_vars)[0x24A] != 0) {
            deactivate_plasmabot(moby);
            ((unsigned char *)moby)[0x20] = 1;
            return;
        }
        MOBY(moby)->pos.z = probe_ground_height(0.5f, &MOBY(moby)->pos, 0);
        if (plasmabot_update_private_vars->path_index == -1) {
            ((unsigned char *)moby)[0x20] = 4;
        } else {
            ((unsigned char *)moby)[0x20] = 0xD;
        }
        return;
    }
    case PLASMABOT_WAIT_TO_START_PATH_TRAVERSAL: {
        float *path;
        if (plasmabot_update_private_vars->target_selection_status == 2)
            return;
        if (fast_dec_timer_s16(&plasmabot_update_private_vars->path_traversal_delay_timer) == 0)
            return;
        if (refresh_plasmabot_hatch_reset_delay(plasmabot_update_private_vars->hatch_index) == 0)
            return;
        if (call_4BF0(moby) == 0)
            return;
        plasmabot_update_private_vars->path_traversal_delay_timer =
            scale_game_frames(plasmabot_path_traversal_delay_frames);
        activate_plasmabot(moby);
        ((unsigned char *)moby)[0x20] = 2;
        set_moby_animation(moby, 1, 0);
        path = (float *)l14_paths[plasmabot_update_private_vars->path_index];
        MOBY(moby)->rot.z = fast_arc_tan(path[8] - path[4], path[9] - path[5]);
        *(int *)&plasmabot_update_private_vars->aim_yaw_offset = 0;
        plasmabot_update_private_vars->path_traversal_direction = 1.0f;
        plasmabot_update_private_vars->aim_yaw_angular_velocity = 0;
        plasmabot_update_private_vars->aim_joint_angle = 0;
        plasmabot_update_private_vars->aim_joint_angular_velocity = 0;
        plasmabot_update_private_vars->path_segment_index = 0;
        *(int *)&plasmabot_update_private_vars->path_offset_and_spring_velocities = 0;
        plasmabot_update_private_vars->path_offset_and_spring_velocities[3] =
            plasmabot_update_private_vars->aim_yaw_offset;
        plasmabot_update_private_vars->path_offset_and_spring_velocities[2] =
            plasmabot_update_private_vars->aim_yaw_offset;
        plasmabot_update_private_vars->path_offset_and_spring_velocities[1] =
            plasmabot_update_private_vars->aim_yaw_offset;
        return;
    }
    case PLASMABOT_START_PATH_TRAVERSAL:
        if (compute_interpolated_record_value(moby) >= 3.0f) {
            follow_plasmabot_path(moby);
        }
        if ((MOBY(moby)->anim_flags & 2) &&
            ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            ((unsigned char *)moby)[0x20] = 3;
            blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            MOBY(moby)->anim_speed = 1.0f;
        }
        break;
    case PLASMABOT_RETURN_TO_PATH_START: {
        float *path = (float *)l14_paths[plasmabot_update_private_vars->path_index];
        vendor_spring(
            (float *)&MOBY(moby)->pos,
            (float *)(&plasmabot_update_private_vars->path_offset_and_spring_velocities[1]),
            path[4],
            0.01f,
            0.2f,
            0.0f);
        vendor_spring(
            (float *)&MOBY(moby)->pos.y,
            (float *)(&plasmabot_update_private_vars->path_offset_and_spring_velocities[2]),
            path[5],
            0.01f,
            0.2f,
            0.0f);
        if ((MOBY(moby)->anim_flags & 2) &&
            ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 1;
            MOBY(moby)->anim_speed = 1.0f;
            deactivate_plasmabot(moby);
            return;
        }
        break;
    }
    case PLASMABOT_FACE_TARGET_AND_WAIT: {
        float ang;
        float k;
        ang = fast_arc_tan(MOBY(plasmabot_aim_target_moby)->pos.x - MOBY(moby)->pos.x,
                           MOBY(plasmabot_aim_target_moby)->pos.y - MOBY(moby)->pos.y);
        k = frame_time_sq * 12.566371f;
        call_CE58((float *)&MOBY(moby)->rot.z,
                  ang,
                  (float *)(&plasmabot_update_private_vars->heading_angular_velocity),
                  k,
                  k,
                  frame_time * 6.2831855f);
        if (plasmabot_update_private_vars->target_selection_status == 2)
            break;
        if (fast_dec_timer_s16(&plasmabot_update_private_vars->path_traversal_delay_timer) == 0)
            break;
        ((unsigned char *)moby)[0x20] = 3;
        blend_moby_animation(moby, 3, 0, scale_game_frames(10));
        MOBY(moby)->anim_speed = 1.0f;
        *(int *)&plasmabot_update_private_vars->path_offset_and_spring_velocities = 0;
        plasmabot_update_private_vars->path_traversal_direction = 1.0f;
        plasmabot_update_private_vars->path_segment_index = 0;
        plasmabot_update_private_vars->path_offset_and_spring_velocities[3] =
            plasmabot_update_private_vars->path_offset_and_spring_velocities[0];
        plasmabot_update_private_vars->path_offset_and_spring_velocities[2] =
            plasmabot_update_private_vars->path_offset_and_spring_velocities[0];
        plasmabot_update_private_vars->path_offset_and_spring_velocities[1] =
            plasmabot_update_private_vars->path_offset_and_spring_velocities[0];
        break;
    }
    case PLASMABOT_FOLLOW_PATH_FORWARD:
        if (follow_plasmabot_path(moby) != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 4;
        }
        break;
    case PLASMABOT_FOLLOW_PATH_BACKWARD:
        if (((unsigned char *)plasmabot_update_private_vars)[0x24A] != 0) {
            refresh_plasmabot_hatch_reset_delay(plasmabot_update_private_vars->hatch_index);
        }
        if (follow_plasmabot_path(moby) != 0) {
            if (((unsigned char *)plasmabot_update_private_vars)[0x24A] != 0) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
                ((unsigned char *)moby)[0x20] = 9;
            } else {
                plasmabot_update_private_vars->path_traversal_delay_timer =
                    scale_game_frames(plasmabot_path_traversal_delay_frames);
                ((unsigned char *)moby)[0x20] = 0xD;
            }
        }
        break;
    case PLASMABOT_AIM_AT_TARGET: {
        float ang;
        float k;
        float aim;
        scale_and_apply_plasmabot_planar_velocity(moby);
        ang = fast_arc_tan(MOBY(plasmabot_aim_target_moby)->pos.x - MOBY(moby)->pos.x,
                           MOBY(plasmabot_aim_target_moby)->pos.y - MOBY(moby)->pos.y);
        k = frame_time_sq * 12.566371f;
        call_CE58((float *)&MOBY(moby)->rot.z,
                  ang,
                  (float *)(&plasmabot_update_private_vars->heading_angular_velocity),
                  k,
                  k,
                  frame_time * 6.2831855f);
        aim = update_plasmabot_aim_quaternion(moby, 0, MOBY(moby)->rot.z);
        update_plasmabot_aim_joint_rotation(moby, ang);
        if (plasmabot_update_private_vars->target_selection_status == 2) {
            if (plasmabot_update_private_vars->path_index == -1)
                break;
            ((unsigned char *)moby)[0x20] = 8;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
            }
            {
                int   index = plasmabot_update_private_vars->path_segment_index - 1;
                char *path;
                plasmabot_update_private_vars->path_segment_index = index;
                path = (char *)l14_paths[plasmabot_update_private_vars->path_index];
                plasmabot_update_private_vars->path_offset_and_spring_velocities[0] =
                    *(float *)(path + (index << 4) + 0x1C);
            }
            plasmabot_update_private_vars->path_traversal_direction = -1.0f;
            plasmabot_update_private_vars->path_offset_and_spring_velocities[1] =
                plasmabot_update_private_vars->path_offset_and_spring_velocities[2] =
                    plasmabot_update_private_vars->path_offset_and_spring_velocities[3] = 0.0f;
            release_beam_slot_by_id(moby);
            break;
        }
        if (fast_dec_timer_s16(&plasmabot_update_private_vars->beam_cooldown_timer) == 0)
            break;
        if (count_listed_plasmabots_preparing_or_sweeping_beam(((unsigned char *)moby)[0x21]) >= 2)
            break;
        if (register_beam_slot(moby, 1, 0) == 0)
            break;
        if (random_integer_below(4) != 0)
            break;
        if (!(fast_difference_between_rotations(MOBY(moby)->rot.z, ang) < 0.017453292f))
            break;
        if (!(aim < 0.017453292f))
            break;
        if (test_path_segment_strict_crossing(plasmabot_update_private_vars->target_path_index,
                                              &MOBY(moby)->pos,
                                              &MOBY(plasmabot_aim_target_moby)->pos) != 0)
            break;
        reset_listed_plasmabot_beam_cooldowns(((unsigned char *)moby)[0x21]);
        plasmabot_update_private_vars->aim_sweep_center_angle = ang;
        ((unsigned char *)moby)[0x20] = 5;
        blend_moby_animation(moby, 4, 0, scale_game_frames(5));
        plasmabot_update_private_vars->aim_sweep_timer =
            scale_game_frames(l14_plasmabot_aim_sweep_duration_frames);
        break;
    }
    case PLASMABOT_WAIT_ANIMATION_BEFORE_AIM_SWEEP: {
        float ang;
        ang = fast_arc_tan(MOBY(plasmabot_aim_target_moby)->pos.x - MOBY(moby)->pos.x,
                           MOBY(plasmabot_aim_target_moby)->pos.y - MOBY(moby)->pos.y);
        update_plasmabot_aim_quaternion(
            moby, 0, fast_subtract_rotations(ang, l14_plasmabot_aim_sweep_half_angle));
        update_plasmabot_aim_joint_rotation(moby, ang);
        if ((MOBY(moby)->anim_flags & 2) &&
            ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            blend_moby_animation(moby, 5, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 6;
        }
        break;
    }
    case PLASMABOT_SWEEP_AIM_AND_TRACE_LINE: {
        int   flag;
        float ang;
        float progress;
        float sweep;
        float angle;
        flag = 0;
        ang = fast_arc_tan(MOBY(plasmabot_aim_target_moby)->pos.x - MOBY(moby)->pos.x,
                           MOBY(plasmabot_aim_target_moby)->pos.y - MOBY(moby)->pos.y);
        progress = convert_integer_to_float(plasmabot_update_private_vars->aim_sweep_timer);
        progress = progress / convert_integer_to_float(
                                  scale_game_frames(l14_plasmabot_aim_sweep_duration_frames));
        sweep = l14_plasmabot_aim_sweep_half_angle;
        angle = fast_subtract_rotations(plasmabot_update_private_vars->aim_sweep_center_angle,
                                        -sweep + (sweep + sweep) * progress);
        if (((unsigned char *)plasmabot_update_private_vars)[0x24B] != 0) {
            if (MOBY(plasmabot_aim_target_moby)->pos.z + 0.1f - MOBY(moby)->pos.z < 0.0f) {
                flag = 1;
            }
        }
        update_plasmabot_aim_quaternion(moby, flag, angle);
        update_plasmabot_aim_joint_rotation(moby, ang);
        build_moby_bone_transform_c(moby, 0, bone_transform_scratch);
        qcopy(direction, bone_transform_scratch[1]);
        if (flag == 0) {
            direction[2] = 0.0f;
        }
        fast_vec_normalize(direction, direction, l14_plasmabot_beam_trace_endpoint_distance);
        fast_vec_add(end, bone_transform_scratch[3], direction);
        fast_vec_normalize(direction, direction, l14_plasmabot_beam_origin_offset_distance);
        fast_vec_add(bone_transform_scratch[3], bone_transform_scratch[3], direction);
        beam_line_trace_descriptor.moby = moby;
        beam_line_trace_descriptor.flags = 0x10001;
        beam_line_trace_descriptor.count = 1;
        beam_line_trace_descriptor.strength = 1.0f;
        beam_line_trace_descriptor.x = fast_cos(MOBY(moby)->rot.z);
        beam_line_trace_descriptor.y = fast_sin(MOBY(moby)->rot.z);
        beam_line_trace_descriptor.scale = 1.0f;
        beam_line_trace_descriptor.distance = 5627.9248f;
        beam_line_trace_descriptor.enabled = 1;
        beam_line_trace_descriptor.oclass = ((unsigned short)MOBY(moby)->oclass);
        beam_line_trace_descriptor.mode = 0;
        if (collision_line(bone_transform_scratch[3], end, 0, moby, &beam_line_trace_descriptor) !=
            0) {
            qcopy(end, D_L14_00174560);
        }
        set_beam_origin_transform_and_endpoint(moby, bone_transform_scratch, end);
        if (fast_dec_timer_s16(&plasmabot_update_private_vars->aim_sweep_timer) != 0) {
            blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 7;
            release_beam_slot_by_id(moby);
        }
        break;
    }
    case PLASMABOT_WAIT_ANIMATION_BEFORE_AIM:
        if ((MOBY(moby)->anim_flags & 2) &&
            ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 4;
        }
        break;
    case PLASMABOT_WAIT_ANIMATION_BEFORE_RESUMING_AIM: {
        long index = ((unsigned char *)moby)[0x52];
        if ((MOBY(moby)->anim_flags & 2) && (unsigned char)index == ((unsigned char *)moby)[0x53]) {
            if ((unsigned char)index != 7)
                goto check_next;
            goto idle;
        }
        if ((unsigned char)index == 7)
            break;
    check_next:
        if (((unsigned char *)moby)[0x53] == 7)
            break;
    idle:
        blend_moby_animation(moby, 0, 0, 10);
        ((unsigned char *)moby)[0x20] = 4;
        break;
    }
    case PLASMABOT_FALL_THEN_EXPLODE: {
        if ((step_moby_motion(moby, &plasmabot_update_private_vars->motion) & 0x160) == 0)
            return;
        release_beam_slot_by_id(moby);
        FUN_L00_0024f7c8(moby, 2, bone_transform_scratch);
        spawn_plasmabot_death_debris(moby, (char *)bone_transform_scratch);
        call_F4A8(moby,
                  &plasmabot_update_private_vars->unk40,
                  &MOBY(moby)->pos,
                  0.0f,
                  0.0f,
                  5,
                  2,
                  4,
                  2.0f,
                  1.0f,
                  9.0f,
                  1.0f,
                  -1,
                  15.0f,
                  1,
                  1,
                  -1,
                  0);
        if (((unsigned char *)D_001413F4)[0] == 2) {
            FUN_L00_00257470(moby, 0x200, -1);
            {
                call_D678(moby);
                return;
            }
        }
        FUN_L00_00257470(moby, 0, -1);
        {
            call_D678(moby);
            return;
        }
    }
    case PLASMABOT_SPAWN_DEBRIS_AND_REMOVE: {
        FUN_L00_0024f7c8(moby, 2, bone_transform_scratch);
        spawn_plasmabot_death_debris(moby, (char *)bone_transform_scratch);
        status = 0;
        ((void (*)(char *, int *, int))spawn_path_or_ballistic_debris)(moby, &status, 1);
        release_beam_slot_by_id(moby);
        call_D678(moby);
        return;
    }
    }
    state = ((unsigned char *)moby)[0x20];
    if (!(state >= PLASMABOT_AIM_AT_TARGET && state <= PLASMABOT_WAIT_ANIMATION_BEFORE_AIM) &&
        state != PLASMABOT_WAIT_TO_START_PATH_TRAVERSAL) {
        update_plasmabot_aim_quaternion(moby, 0, MOBY(moby)->rot.z);
        update_plasmabot_aim_joint_rotation(moby, MOBY(moby)->rot.z);
    }
    FUN_L14_002b2600_c((struct Moby *)moby);
}

typedef struct {
    u8   pad0[0x1E0];
    Vec4 exhaust;    /* 0x1E0 */
    Vec4 nozzle;     /* 0x1F0 */
    Vec4 joint6;     /* 0x200 */
    Vec4 beam_start; /* 0x210 */
    Vec4 beam_end;   /* 0x220 */
} ShipVars;

typedef struct {
    u8  pad0[0x10];
    f32 scale; /* 0x10 */
    u8  pad14[2];
    s16 mode;  /* 0x16 */
    s32 color; /* 0x18 */
} ParticleFade;

typedef struct {
    u8           pad0[0xA];
    s16          frames; /* 0x0A */
    f32          life;   /* 0x0C */
    u8           pad10[0x10];
    ParticleFade fade; /* 0x20 */
} Particle;

extern Vec4 D_L14_001674C0 NOT_SDA;
extern u8                  D_L14_0015F580[] MACRO_ADDR;

#include "rnc/overlay/shared/enemies/plasmabot_data/update_plasmabot_visual_effects.h"

extern void FUN_L14_002b3a20_c(struct Moby *) __asm__("FUN_L14_002b3a20");
extern void FUN_L14_002b2928_u(struct Moby *) __asm__("FUN_L14_002b2928");

/* Ship engine effects: puffs exhaust particles from joint 1 and places the nozzle and beam points from joints 7 and 2. */
void update_plasmabot_visual_effects(struct Moby *moby) __asm__("FUN_L14_002b2600");

void update_plasmabot_visual_effects(struct Moby *moby) {
    Vec4          particle_spawn_position;
    Vec4          exhaust_position_offset;
    Vec4          particle_initial_velocity;
    Vec4          bone_transform_scratch[4];
    Vec4          dx;
    Vec4          dy;
    Vec4          dz;
    ShipVars     *effect_positions;
    Particle     *particle;
    ParticleFade *fade;
    s32           i;
    f32           inverse_lifetime_frames;

    effect_positions = (ShipVars *)moby->pvars;
    add_draw_callback(FUN_L14_002b3a20_c, moby);
    moby_point_position(moby, 1, &particle_spawn_position);
    fast_vec_sub(&exhaust_position_offset, &particle_spawn_position, &D_L14_001674C0);
    fast_vec_normalize(&exhaust_position_offset, &exhaust_position_offset, -0.5f);
    fast_vec_add(&effect_positions->exhaust, &particle_spawn_position, &exhaust_position_offset);
    random_spherical_offset(&particle_initial_velocity, 0.005f, 0.03f);
    particle = spawn_mode_controlled_rotating_alpha_fade_particle(
        &particle_spawn_position, &particle_initial_velocity, 0x7F, moby);
    if (particle != NULL) {
        particle->life = random_float_between(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        particle = spawn_mode_controlled_rotating_alpha_fade_particle(
            &particle_spawn_position, D_L14_0015F580, 0x7F, moby);
        if (particle == NULL) {
            continue;
        }
        fade = &particle->fade;
        if (i == 2 && random_integer_below(8) == 0) {
            particle->life = 180000.0f;
        } else {
            particle->life = random_float_between(80000.0f, 120000.0f);
        }
        particle->frames = scale_game_frames(2);
        inverse_lifetime_frames = 1.0f / convert_integer_to_float(particle->frames);
        fade->mode = 3;
        fade->color = 0x7F7F7F;
        fade->scale = inverse_lifetime_frames;
    }
    moby_point_position(moby, 6, &effect_positions->joint6);
    build_moby_bone_transform(moby, 7, bone_transform_scratch);
    fast_vec_scale(&dx, &bone_transform_scratch[0], plasmabot_nozzle_bone_x_offset_gp);
    fast_vec_add(&effect_positions->nozzle, &bone_transform_scratch[3], &dx);
    build_moby_bone_transform(moby, 2, bone_transform_scratch);
    fast_vec_scale(&dx, &bone_transform_scratch[0], plasmabot_beam_bone_x_offset_gp);
    fast_vec_scale(&dy, &bone_transform_scratch[1], plasmabot_beam_endpoint_bone_y_offset_gp);
    fast_vec_scale(&dz, &bone_transform_scratch[2], plasmabot_beam_bone_z_offset_gp);
    fast_vec_add(&effect_positions->beam_start, &bone_transform_scratch[3], &dx);
    fast_vec_add(&effect_positions->beam_start, &effect_positions->beam_start, &dy);
    fast_vec_add(&effect_positions->beam_start, &effect_positions->beam_start, &dz);
    fast_vec_scale(&dy, &dy, -2.0f);
    fast_vec_add(&effect_positions->beam_end, &effect_positions->beam_start, &dy);
    add_draw_callback(FUN_L14_002b2928_u, moby);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B3B78), where it is exact; names translated to the US level program. */

typedef u32 VecQuad __attribute__((mode(TI), aligned(16)));

typedef union {
    VecQuad q;
    f32     f[4];
    s32     i[4];
} Vec4_2b2928;

extern Vec4_2b2928 D_L14_001D8850_lui[] __asm__("D_L14_001D8850") NOT_SDA;
extern char        D_0013F5E0[] NOT_SDA;
extern float       D_L14_001614AC __attribute__((sda)); /* no foreign declaration */
extern float       D_L14_001614BC __attribute__((sda)); /* no foreign declaration */
extern float       D_L14_001614C0 __attribute__((sda)); /* no foreign declaration */
extern s64         get_effect_texture(s32) __asm__("func_001F44B8");
extern void        fast_draw_quad_real(void *, void *, s32) __asm__("FUN_001f7d30");
extern float       D_L14_001614AC_lui __asm__("D_L14_001614AC");
extern float       D_L14_001614BC_lui __asm__("D_L14_001614BC");
extern float       D_L14_001614C0_lui __asm__("D_L14_001614C0");

void draw_plasmabot_effect_quads(struct Moby *moby) __asm__("FUN_L14_002b2928");

void draw_plasmabot_effect_quads(struct Moby *moby) {
    PlasmabotVars *plasmabot_effect_private_vars = (PlasmabotVars *)moby->pvars;
    struct {
        Vec4_2b2928  o[4];
        unsigned int c[4];
        float        g[8];
        u64          t[4];
    } pk;
    Vec4_2b2928 quad_transform[4];
    Vec4_2b2928 position_offset;
    char       *pp;
    char       *nozzle_position;
    char       *beam_start_position;
    int         i;
    int         j;
    int         alpha;

    qcopy(&quad_transform[3], &plasmabot_effect_private_vars->unk1E0);
    quad_transform[3].f[3] = 1.0f;
    fast_vec_sub(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    fast_vec_cross(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    fast_vec_cross(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    pk.t[1] = get_effect_texture(11);
    pk.t[3] = 0x8000000048ULL;
    pk.t[2] = 0xFF9000000260ULL;
    pk.t[0] = 5;
    pk.c[3] = 0x80FFFFFF;
    pk.c[2] = 0x80FFFFFF;
    pk.c[1] = 0x80FFFFFF;
    pk.c[0] = 0x80FFFFFF;
    pk.g[0] = 0.0f;
    pk.g[1] = 0.0f;
    pk.g[2] = 0.0f;
    pk.g[3] = 1.0f;
    pk.g[4] = 1.0f;
    pk.g[5] = 0.0f;
    pk.g[6] = 1.0f;
    pk.g[7] = 1.0f;
    for (i = 0; i < 4; i++) {
        fast_vec_scale(&pk.o[i], &D_L14_001D8850_lui[i], 0.4f);
        transform_vector(&pk.o[i], &pk.o[i], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);

    qcopy(&quad_transform[3], &plasmabot_effect_private_vars->unk200);
    pp = &plasmabot_effect_private_vars->beam_end_position_storage;
    fast_vec_sub(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    beam_start_position = &plasmabot_effect_private_vars->beam_start_position_storage;
    nozzle_position = &plasmabot_effect_private_vars->nozzle_position;
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    fast_vec_cross(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    fast_vec_cross(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    pk.c[3] = 0x202020FF;
    pk.c[2] = 0x202020FF;
    pk.c[1] = 0x202020FF;
    pk.c[0] = 0x202020FF;
    for (j = 0; j < 4; j++) {
        fast_vec_scale(&pk.o[j], &D_L14_001D8850_lui[j], 0.2f);
        transform_vector(&pk.o[j], &pk.o[j], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);

    qcopy(&quad_transform[3], nozzle_position);
    fast_vec_sub(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    fast_vec_cross(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    fast_vec_cross(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    fast_vec_scale(&position_offset, &quad_transform[0], D_L14_001614AC_lui);
    fast_vec_add(&quad_transform[3], &quad_transform[3], &position_offset);
    pk.c[3] = 0x4020FF20;
    pk.c[2] = 0x4020FF20;
    pk.c[1] = 0x4020FF20;
    pk.c[0] = 0x4020FF20;
    alpha = 0x40;
    for (j = 0; j < 4; j++) {
        fast_vec_scale(&pk.o[j], &D_L14_001D8850_lui[j], 0.2f);
        transform_vector(&pk.o[j], &pk.o[j], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);

    qcopy(&quad_transform[3], beam_start_position);
    fast_vec_sub(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    fast_vec_cross(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    fast_vec_cross(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    fast_vec_scale(&position_offset, &quad_transform[0], D_L14_001614BC_lui);
    fast_vec_add(&quad_transform[3], &quad_transform[3], &position_offset);
    pk.c[3] = (alpha << 24) | 0xFF2020;
    pk.c[2] = (alpha << 24) | 0xFF2020;
    pk.c[1] = (alpha << 24) | 0xFF2020;
    pk.c[0] = (alpha << 24) | 0xFF2020;
    for (j = 0; j < 4; j++) {
        fast_vec_scale(&pk.o[j], &D_L14_001D8850_lui[j], D_L14_001614C0_lui);
        transform_vector(&pk.o[j], &pk.o[j], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);

    qcopy(&quad_transform[3], pp);
    fast_vec_sub(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    fast_vec_normalize(&quad_transform[0], &quad_transform[0], 1.0f);
    fast_vec_cross(&quad_transform[1], &quad_transform[0], D_0013F5E0);
    fast_vec_normalize(&quad_transform[1], &quad_transform[1], -1.0f);
    fast_vec_cross(&quad_transform[2], &quad_transform[1], &quad_transform[0]);
    fast_vec_scale(&position_offset, &quad_transform[0], D_L14_001614BC_lui);
    fast_vec_add(&quad_transform[3], &quad_transform[3], &position_offset);
    pk.c[3] = (alpha << 24) | 0xFF2020;
    pk.c[2] = (alpha << 24) | 0xFF2020;
    pk.c[1] = (alpha << 24) | 0xFF2020;
    pk.c[0] = (alpha << 24) | 0xFF2020;
    for (j = 0; j < 4; j++) {
        fast_vec_scale(&pk.o[j], &D_L14_001D8850_lui[j], D_L14_001614C0_lui);
        transform_vector(&pk.o[j], &pk.o[j], &quad_transform[0]);
    }
    fast_draw_quad_real(&pk, 0, 0);
}

/* pvars of the moby FUN_L14_002b2ed8 updates */
typedef struct {
    u8  pad00[0x38];
    s32 restart; /* 0x38: nonzero picks a new random delay */
    u8  pad3C[0x1F8];
    f32 target_range; /* 0x234 */
    s32 delay;        /* 0x238: frames, counted down */
} DelayVars;

extern f32 scale_time(f32) __asm__("FUN_001f96b0");

extern f32 plasmabot_base_target_range_gp __asm__("D_L14_001614C4") __attribute__((sda));
void       update_plasmabot_target_range(struct Moby *moby) __asm__("FUN_L14_002b2ed8");

void update_plasmabot_target_range(struct Moby *moby) {
    DelayVars *delay_state = (DelayVars *)moby->pvars;

    if (delay_state->restart != 0) {
        delay_state->delay =
            truncate_float_to_s32(scale_time(random_float_between(180.0f, 240.0f)));
        delay_state->restart = 0;
    }
    fast_dec_timer(&delay_state->delay);
    if (delay_state->delay != 0) {
        delay_state->target_range = plasmabot_base_target_range_gp + 6.0f;
    } else {
        delay_state->target_range = plasmabot_base_target_range_gp;
    }
}

/* Enemy hit reaction: takes damage from attacks (not from its own kind), staggers or is knocked away and dies. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B41B0), where it is exact; names translated to the US level program. */

extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern float plasmabot_stagger_motion_z_scale_gp __asm__("D_L14_001614C8") __attribute__((sda));
extern void  start_hit_color_flash(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void  func_L00_0025D5B0_i(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");

void process_plasmabot_hit_reaction(struct Moby *moby) {
    enum {
        HIT_REACTION_UNAVAILABLE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DEATH = 2,
        HIT_REACTION_STAGGER = 3,
        HIT_REACTION_NONE = 11
    };
    PlasmabotVars *state = (PlasmabotVars *)moby->pvars;
    int            hit_kind;
    float          damage;
    char          *hit_info;
    int            reaction_outcome;
    if (((unsigned char *)moby)[0x20] == 0 || ((unsigned char *)moby)[0x20] == 0xB) {
        if (((unsigned char *)moby)[0x20] == 0xB) {
            ((unsigned char *)moby)[0xA4] = 0xFF;
            update_hit_color_flash(moby, &state->trooper_anim);
        }
        return;
    }
    damage = 0.0f;
    hit_info = find_moby_hit(moby, 0x330000, 0);
    if (hit_info != 0 && *(char **)(hit_info + 0x20) != 0 &&
        (*(short *)(*(char **)(hit_info + 0x20) + 0xA6) == moby->oclass ||
         *(short *)(*(char **)(hit_info + 0x20) + 0xA6) == 0x370)) {
        hit_info = 0;
    }
    reaction_outcome = hit_reaction(moby, hit_info, &state->health, 0, &hit_kind, &damage, 0, 4);
    if (hit_kind != HIT_KIND_SKIP_DAMAGE_AND_REACTION && ((unsigned char *)moby)[0x20] != 0xB) {
        state->health -= damage;
        if (state->health <= 0.0f)
            reaction_outcome = 1;
        switch (reaction_outcome) {
        case HIT_REACTION_STAGGER:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10: {
            char *reaction_motion_state = &state->motion;
            int   stagger_duration_frames;
            float frame_duration, stagger_motion_z, k;
            release_beam_slot_by_id(moby);
            moby->state = PLASMABOT_WAIT_ANIMATION_BEFORE_RESUMING_AIM;
            stagger_duration_frames = scale_game_frames(0x3C);
            frame_duration = frame_time;
            stagger_motion_z = plasmabot_stagger_motion_z_scale_gp * frame_duration;
            k = frame_duration * 8.0f;
            state->stagger_reaction_duration_frames = stagger_duration_frames;
            state->knockback_speed = k;
            state->knockback_vertical_speed = stagger_motion_z;
            state->knockback_apex_animation_frame = 11.0f;
            state->knockback_landing_animation_frame = 29.0f;
            func_L00_0025D5B0_i(
                moby,
                reaction_motion_state,
                fast_arc_tan(*(float *)(hit_info + 0x10), *(float *)(hit_info + 0x14)),
                7,
                1,
                0);
        }
            ((unsigned char *)state)[0xC7] = 0xFA;
            break;
        case HIT_REACTION_LETHAL:
        case HIT_REACTION_DEATH: {
            float y;
            if (*(int *)(hit_info + 0x24) & 0x800000) {
                moby->state = PLASMABOT_SPAWN_DEBRIS_AND_REMOVE;
            } else {
                moby->state = PLASMABOT_FALL_THEN_EXPLODE;
            }
            moby->flags &= 0xEFFF;
            {
                int scaled_previous_motion_parameter =
                    truncate_float_to_s32(state->knockback_collision_z_offset * 1024.0f);
                float k = frame_time * 10.0f;
                state->knockback_collision_z_offset = 0.3f;
                state->knockback_speed = k;
                state->knockback_vertical_speed = k;
                state->knockback_apex_animation_frame = 9.0f;
                state->knockback_collision_radius = scaled_previous_motion_parameter;
                state->knockback_landing_animation_frame = 15.0f;
            }
            y = fast_arc_tan(*(float *)(hit_info + 0x10), *(float *)(hit_info + 0x14));
            func_L00_0025D5B0_i(moby, &state->motion, y, 8, scale_game_frames(5), 0);
            ((unsigned char *)state)[0xC7] = 0xFA;
            if (((unsigned char *)D_0013E533)[0x2EC1] == 2) {
                FUN_L00_00257470(moby, 0x200, -1);
            } else {
                FUN_L00_00257470(moby, 0, -1);
            }
            moby->collision = 0;
            break;
        }
        case HIT_REACTION_UNAVAILABLE:
        case HIT_REACTION_NONE:
            break;
        }
        start_hit_color_flash(moby, (short *)(&state->trooper_anim));
    }
    ((unsigned char *)moby)[0xA4] = 0xFF;
    update_hit_color_flash(moby, &state->trooper_anim);
}
/* steers the moby toward its target: queries the path, eases x/y, then turns to face the heading */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4668), where it is exact; names translated to the US level program. */

extern float func_L00_0025CE58_2B4668(
    float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");

extern float plasmabot_path_traversal_speed_gp __asm__("D_L14_001614D8") __attribute__((sda));

int follow_plasmabot_path(char *moby) {
    float          path_target_position[4];
    float          pre_spring_position[4];
    float          target_offset[4];
    PlasmabotVars *plasmabot_private_state = (PlasmabotVars *)MOBY(moby)->pvars;
    int   reached = FUN_L00_0025d808(((int *)l14_paths[plasmabot_private_state->path_index]),
                                   path_target_position,
                                   &plasmabot_private_state->path_segment_index,
                                   &plasmabot_private_state->path_offset_and_spring_velocities,
                                   0,
                                   plasmabot_path_traversal_speed_gp * frame_time *
                                       plasmabot_private_state->path_traversal_direction);
    float path_target_heading;
    float k;
    float turn_step_limit;
    qcopy(pre_spring_position, &MOBY(moby)->pos);
    vendor_spring((float *)&MOBY(moby)->pos,
                  (float *)(&plasmabot_private_state->path_offset_and_spring_velocities[1]),
                  path_target_position[0],
                  0.01f,
                  0.2f,
                  0.0f);
    vendor_spring((float *)&MOBY(moby)->pos.y,
                  (float *)(&plasmabot_private_state->path_offset_and_spring_velocities[2]),
                  path_target_position[1],
                  0.01f,
                  0.2f,
                  0.0f);
    fast_vec_sub(target_offset, path_target_position, pre_spring_position);
    path_target_heading = fast_arc_tan(target_offset[0], target_offset[1]);
    k = frame_time_sq * 12.566371f;
    turn_step_limit = frame_time * 6.2831855f;
    func_L00_0025CE58_2B4668((float *)&MOBY(moby)->rot.z,
                             path_target_heading,
                             (float *)(&plasmabot_private_state->heading_angular_velocity),
                             k,
                             k,
                             turn_step_limit);
    return reached != 0;
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4828), where it is exact; names translated to the US level program. */

extern float
func_L00_0025CE58_2B4828(float *, float, float *, float, float, float) __asm__("FUN_L00_0025be00");

/* Eases the moby's yaw offset toward target_angle (clamped to about 70 degrees); with `flag` also eases the pitch toward its owner.
   Returns the remaining difference between the target and the current yaw offset. */
float update_plasmabot_aim_quaternion(char *moby, int flag, float target_angle) {
    PlasmabotVars *state = (PlasmabotVars *)MOBY(moby)->pvars;
    char          *aim_target_moby = *(char **)(&state->target);
    float          turn = fast_subtract_rotations(target_angle, MOBY(moby)->rot.z);
    if (turn > 1.2217304706573486f)
        turn = 1.2217304706573486f;
    else if (turn < -1.2217304706573486f)
        turn = -1.2217304706573486f;
    {
        float yaw_easing_coefficient = frame_time_sq * 12.566370964050293f;
        func_L00_0025CE58_2B4828((float *)(&state->aim_yaw_offset),
                                 turn,
                                 (float *)(&state->aim_yaw_angular_velocity),
                                 yaw_easing_coefficient,
                                 yaw_easing_coefficient,
                                 frame_time * 6.2831854820251465f);
    }
    build_axis_rotation_quaternion(&state->aim_quaternion_storage, 2, state->aim_yaw_offset);
    if (flag) {
        float        owner_aim_position[4];
        float        owner_aim_offset[4];
        float        pitch_quaternion[4];
        float       *bp;
        float       *cp;
        struct Hero *hero_state;
        float        target_pitch_angle;
        qcopy(owner_aim_position, &MOBY(aim_target_moby)->pos);
        hero_state = &hero;
        if (((char *)hero_state->moby) == aim_target_moby)
            owner_aim_position[2] = owner_aim_position[2] - hero_state->ground_distance;
        bp = owner_aim_offset;
        fast_vec_sub(bp, owner_aim_position, &MOBY(moby)->pos);
        owner_aim_offset[2] = owner_aim_offset[2] + 1.5f;
        target_pitch_angle = -fast_arc_tan(vector_length_xy(bp), owner_aim_offset[2]);
        func_L00_0025CE58_2B4828((float *)(&state->aim_pitch_angle),
                                 target_pitch_angle,
                                 (float *)(&state->aim_pitch_angular_velocity),
                                 frame_time_sq * 12.566370964050293f,
                                 frame_time_sq * 12.566370964050293f,
                                 frame_time * 6.2831854820251465f);
        cp = pitch_quaternion;
        build_axis_rotation_quaternion(cp, 1, *(float *)&state->aim_pitch_angle);
        multiply_quaternions(&state->aim_quaternion_storage, &state->aim_quaternion_storage, cp);
    }
    return fast_difference_between_rotations(turn, state->aim_yaw_offset);
}

extern short *D_L14_001ABF40_3920[] __asm__("D_L14_001ABF40");
extern char  *D_L14_0015FFD8_3920 __asm__("D_L14_0015FFD8");

int count_listed_plasmabots_preparing_or_sweeping_beam(int moby_list_index) {
    int    count = 0;
    short *moby_list_entry;

    if (moby_list_index == -1)
        return 0;
    moby_list_entry = D_L14_001ABF40_3920[moby_list_index];
    if (moby_list_entry == 0)
        return 0;
    do {
        unsigned short v = *(unsigned short *)moby_list_entry;
        unsigned char *listed_moby = (unsigned char *)(D_L14_0015FFD8_3920 + ((v & 0x7FFF) << 8));
        if (MOBY(listed_moby)->oclass == 0x1C) {
            if ((unsigned char)(MOBY(listed_moby)->state - 5) <= 1)
                count++;
        }
    } while (*moby_list_entry++ >= 0);
    return count;
}
/* Returns 0 if any listed moby of class 0x1C is in state 2, else 1. */

extern char *D_L14_0015FFD8;

int check_listed_plasmabots_not_starting_path_traversal(unsigned char *source_moby) __asm__("FUN_L14_002b39a0");

int check_listed_plasmabots_not_starting_path_traversal(unsigned char *source_moby) {
    unsigned short *moby_list_entry;
    if (*(volatile unsigned char *)(source_moby + 0x21) == 0xFF ||
        (moby_list_entry = (unsigned short *)D_L14_001ABF40[MOBY(source_moby)->group]) == 0)
        return 1;
    do {
        char *moby = D_L14_0015FFD8 + ((*moby_list_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass == 0x1C && MOBY(moby)->state == PLASMABOT_START_PATH_TRAVERSAL)
            return 0;
    } while ((short)*moby_list_entry++ >= 0);
    return 1;
}

/* Draws the moby's glow quad, pulled toward the camera so it is not hidden by the moby. */
/* Ported from rac1-decomp (src/overlays/l14_oltanis/vendor_002ACCC0.c: func_L14_002AF4A0), where it is exact; names translated to the US level program. */

typedef struct {
    float         m[4][4];
    u32           col[4];
    float         uv[8];
    unsigned long a, b, c, d;
} Pk;

extern float plasmabot_glow_quad_vertex_offsets[4][4] __asm__("D_L14_001D8890");
extern float plasmabot_glow_quad_scale_gp __asm__("D_L14_001614F8") __attribute__((sda));
extern float plasmabot_glow_camera_pull_distance_limit_gp __asm__("D_L14_001614FC")
    __attribute__((sda));
extern float plasmabot_glow_camera_clearance_distance_gp __asm__("D_L14_00161500")
    __attribute__((sda));
extern u32  plasmabot_glow_vertex_color_gp __asm__("D_L14_00161504") __attribute__((sda));
extern void draw_geometry_quad(void *, void *, int);
extern void vu1_add_g_sregister(s32, unsigned long) __asm__("func_00233980");

void draw_plasmabot_glow_quad(char *m) __asm__("FUN_L14_002b3a20");

void draw_plasmabot_glow_quad(char *m) {
    float glow_position[4];
    Pk    pk;
    float camera_displacement[4];
    float position_offset[4];
    float camera_to_glow_distance;
    u32   col;
    int   j;
    qcopy(glow_position, m + 0x10);
    glow_position[2] += 0.01f;
    glow_position[3] = 1.0f;
    pk.b = get_effect_texture(0xB);
    pk.c = 0x0000FF9000000260ULL;
    pk.d = 0x8000000048ULL;
    pk.a = 0;
    vu1_add_g_sregister(0x4A, 0);
    vu1_add_g_sregister(0x47, 0x51001);
    fast_vec_sub(camera_displacement, ((char *)&D_L14_001674C0), glow_position);
    camera_to_glow_distance = fast_vec_length(camera_displacement);
    if (0.0f < camera_to_glow_distance) {
        float position_offset_distance = plasmabot_glow_camera_pull_distance_limit_gp;
        float unclamped_offset_distance =
            camera_to_glow_distance - plasmabot_glow_camera_clearance_distance_gp;
        if (unclamped_offset_distance < position_offset_distance) {
            position_offset_distance = unclamped_offset_distance;
            if (unclamped_offset_distance < 0.0f)
                position_offset_distance = 0.0f;
        }
        fast_vec_normalize(position_offset, camera_displacement, position_offset_distance);
        fast_vec_add(glow_position, glow_position, position_offset);
    }
    pk.uv[0] = 1.0f;
    pk.uv[1] = 1.0f;
    pk.uv[2] = 0.0f;
    pk.uv[3] = 1.0f;
    pk.uv[4] = 1.0f;
    pk.uv[5] = 0.0f;
    pk.uv[6] = 0.0f;
    pk.uv[7] = 0.0f;
    col = plasmabot_glow_vertex_color_gp;
    pk.col[3] = col;
    pk.col[2] = col;
    pk.col[1] = col;
    pk.col[0] = col;
    for (j = 0; j < 4; j++) {
        fast_vec_scale(
            pk.m[j], plasmabot_glow_quad_vertex_offsets[j], plasmabot_glow_quad_scale_gp);
        fast_vec_add(pk.m[j], pk.m[j], glow_position);
    }
    draw_geometry_quad(&pk, 0, 0);
}

float D_L15_001613BC = 0.92f;
float D_L15_001613C4 = 25.0f;
float D_L15_001613C8 = 0.5235988f;
int   D_L15_001613CC = 0x78;
float D_L15_001613D4 = 0.25f;
float D_L15_0016138C = 0.3f;
float D_L15_0016139C = 0.3f;
float D_L15_001613A0 = 0.15f;
f32   D_L15_001613A4 = 4e+01f;
float D_L15_001613A8 = 3.0f;
float D_L15_001613B8 = 6.0f;

float D_L15_001CBFA0[4][4] = {
    {-1.0f,  1.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    { 1.0f,  1.0f, 0.0f, 1.0f},
    { 1.0f, -1.0f, 0.0f, 1.0f},
};

float D_L15_001613D8 = 2.0f;
float D_L15_001613DC = 0.4f;
float D_L15_001613E0 = 2.0f;
u32   D_L15_001613E4 = 0x207f6060;

float plasmabot_glow_quad_vertex_offsets[4][4] __asm__("D_L14_001D8890") = {
    {-1.0f,  1.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    { 1.0f,  1.0f, 0.0f, 1.0f},
    { 1.0f, -1.0f, 0.0f, 1.0f},
};
