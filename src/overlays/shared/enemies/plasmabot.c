/* Shared moby classes: plasmabot, plasmabot_hatch, mine. */
#include "types.h"
#include "rnc/overlay/level_spline.h"
#include "rnc/overlay/level_path.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/entities.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/mobys/animation/build_moby_bone_transform.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/vectors/compute_transformed_delta_component.h"
#include "rnc/rendering/view/build_look_at_matrix.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/enemies/trooper_play_sound.h"
#include "rnc/overlay/shared/rendering/update_entity.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/math/random/random_spherical_offset.h"
#include "rnc/overlay/shared/vendor/vendor_orb.h"
#include "rnc/overlay/shared/rendering/spawn_mode_controlled_rotating_alpha_fade_particle.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B44B8), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern int  FUN_L00_0025ff38(char *, char *, float, int, int, int *, int);

void update_plasmabot_target(char *moby) __asm__("FUN_L14_002b3268");

void update_plasmabot_target(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    int  *target_filter_record = ((int *)l14_paths[*(int *)(data + 0x244)]);
    if (FUN_L00_0025ff38(moby,
                         data + 0xD0,
                         *(float *)(data + 0x234),
                         0,
                         0,
                         target_filter_record + 4,
                         *target_filter_record) != 2) {
        if (*(float *)(data + 0x234) < vector_distance_xy(moby + 0x10, data + 0xD0)) {
            *(int *)(data + 0x114) = 2;
        }
    }
    if (*(int *)(data + 0x110) == 0) {
        *(int *)(data + 0x110) = *(int *)(D_0013E533 + 0x2E9D);
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
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    char *path;
    int   i;
    *(int *)(moby_vars + 0x250) = 0;
    *(int *)(moby_vars + 0x24C) = 0;
    *(float *)(moby_vars + 0x25C) = *(float *)(moby_vars + 0x250);
    *(float *)(moby_vars + 0x258) = *(float *)(moby_vars + 0x250);
    *(float *)(moby_vars + 0x254) = *(float *)(moby_vars + 0x250);
    if (*(int *)(moby_vars + 0x240) == -1)
        return;
    path = (char *)l14_paths[*(int *)(moby_vars + 0x240)];
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
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    *(float *)(moby_vars + 0x254) = *(float *)(moby_vars + 0x254) * planar_velocity_scale;
    *(float *)(moby_vars + 0x258) = *(float *)(moby_vars + 0x258) * planar_velocity_scale;
    MOBY(moby)->pos.x = MOBY(moby)->pos.x + *(float *)(moby_vars + 0x254);
    MOBY(moby)->pos.y = MOBY(moby)->pos.y + *(float *)(moby_vars + 0x258);
}
/* steers the pitch angle of a moby toward a target and updates the joint */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4A00), where it is exact; names translated to the US level program. */

extern void FUN_L00_001fff28(void *, int, float);

void update_plasmabot_aim_joint_rotation(char *moby, float x) __asm__("FUN_L14_002b37b0");

void update_plasmabot_aim_joint_rotation(char *moby, float x) {
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    float target_joint_angle = -*(float *)(moby_vars + 0x268);
    float heading_angle_delta = fast_subtract_rotations(x, MOBY(moby)->rot.z);
    target_joint_angle = fast_add_rotations(target_joint_angle, -heading_angle_delta);
    if (target_joint_angle > 1.2217304706573486f)
        target_joint_angle = 1.2217304706573486f;
    else if (target_joint_angle < -1.2217304706573486f)
        target_joint_angle = -1.2217304706573486f;
    {
        float k = frame_time_sq * 12.566370964050293f;
        approach_angle((float *)(moby_vars + 0x270),
                       (float *)(moby_vars + 0x274),
                       target_joint_angle,
                       k,
                       k,
                       frame_time * 6.2831854820251465f);
    }
    FUN_L00_001fff28(moby_vars + 0x130, 2, *(float *)(moby_vars + 0x270));
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
                char *data = ((char *)MOBY(moby)->pvars);
                if (MOBY(moby)->oclass == 0x1C) {
                    *(short *)(data + 0x260) = scale_game_frames(plasmabot_beam_cooldown_frames);
                }
            } while (*listed_moby_entry_cursor++ >= 0);
        }
    }
}

#include "qcopy.h"

/* Recomputes a path's segment lengths, aims the moby along its first segment, and recomputes a second path. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002FFD88), where it is exact; names translated to the US level program. */

void initialize_moby_path_state(char *moby) __asm__("FUN_L14_002fe900");

void initialize_moby_path_state(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    char *path;
    int   i;
    path = ((int *)l14_paths[*(int *)(data + 0xD8)]);
    for (i = 0; i < *(int *)path - 1; i++) {
        *(float *)(path + 0x1C + i * 0x10) =
            fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
    }
    *(int *)(data + 0xD0) = 0;
    *(int *)(data + 0xD4) = 0;
    qcopy(moby + 0x10, path + 0x10);
    MOBY(moby)->rot.z = fast_arc_tan(*(float *)(path + 0x20) - *(float *)(path + 0x10),
                                     *(float *)(path + 0x24) - *(float *)(path + 0x14));
    *(float *)(data + 0xF0) = *(float *)(data + 0xE4) = *(float *)(data + 0xE8) =
        *(float *)(data + 0xEC) = 0.0f;
    if (*(int *)(data + 0xFC) != 1) {
        path = ((int *)l14_paths[*(int *)(data + 0xDC)]);
        for (i = 0; i < *(int *)path - 1; i++) {
            *(float *)(path + 0x1C + i * 0x10) =
                fast_vec_dist(path + 0x10 + i * 0x10, path + 0x20 + i * 0x10);
        }
    }
}
/* rebuilds two vectors from the moby's transform and applies them to its linked object */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_003000B0), where it is exact; names translated to the US level program. */

extern float linked_moby_initial_offset_length_gp __asm__("D_L14_00161FC4") __attribute__((sda));
extern float linked_moby_additional_offset_length_gp __asm__("D_L14_00161FC8") __attribute__((sda));
void         update_linked_moby_position(char *moby) __asm__("FUN_L14_002fec28");

void update_linked_moby_position(char *moby) {
    char *data = ((char *)MOBY(moby)->pvars);
    float position_offset[4];
    if (*(char **)(data + 0xF4) != 0) {
        char *linked_position;
        fast_vec_normalize(position_offset, moby + 0xC0, linked_moby_initial_offset_length_gp);
        fast_vec_add(*(char **)(data + 0xF4) + 0x10, moby + 0x10, position_offset);
        fast_vec_normalize(position_offset, moby + 0xE0, linked_moby_additional_offset_length_gp);
        linked_position = *(char **)(data + 0xF4) + 0x10;
        fast_vec_add(linked_position, linked_position, position_offset);
    }
}
/* Spawns moby class 0x3A5 at the given position, owned by owner, with three randomised angle values. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_00300F80), where it is exact; names translated to the US level program. */

unsigned char *spawn_floating_mine(unsigned char *owner,
                                   char          *spawn_position) __asm__("FUN_L14_002ffaf8");

unsigned char *spawn_floating_mine(unsigned char *owner, char *spawn_position) {
    unsigned char *spawned_moby = create_moby(0x3A5);
    if (spawned_moby != 0) {
        char *spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        *(unsigned char **)spawned_vars = owner;
        *(int *)(spawned_vars + 8) = 0;
        *(int *)(spawned_vars + 0xC) = 0;
        *(float *)(spawned_vars + 4) = -1.0f;
        *(float *)(spawned_vars + 0x10) = random_float_between(0.0104719754f, 0.0209439509f);
        *(float *)(spawned_vars + 0x14) = random_float_between(0.00698131695f, 0.0139626339f);
        *(float *)(spawned_vars + 0x18) = random_float_between(0.00349065848f, 0.00698131695f);
        qcopy(spawned_moby + 0x10, spawn_position);
        MOBY(spawned_moby)->rot.z = *(float *)(owner + 0x48);
        spawned_moby[0x20] = 0;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"

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
extern void DebugPrint(char *, ...);
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
int count_listed_plasmabots_preparing_or_sweeping_beam(int index) __asm__("FUN_L14_002b3920");
int refresh_plasmabot_hatch_reset_delay(int i) __asm__("FUN_L14_002d9870");
unsigned char *spawn_plasmabot_death_debris(char *owner, char *pos) __asm__("FUN_L14_002dea98");
unsigned char *
spawn_path_or_ballistic_debris(char *owner, float *scale, int mode) __asm__("FUN_L14_002e0170");
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

void update_plasmabot(char *moby) __asm__("FUN_L14_002b17d8");

void update_plasmabot(char *moby) {
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
    char    *data;
    char    *owner;
    int      state;
    float    bone_transform_scratch[4][4];
    float    direction[4];
    float    end[4];
    LineInfo info;
    int      status;

    data = ((char *)MOBY(moby)->pvars);
    owner = 0;
    FUN_L00_00263ac8(2.5f, moby, 2, data + 0x120);
    if (*(int *)(data + 0x288) != 0) {
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
        owner = *(char **)(data + 0x110);
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
        if (((unsigned char *)data)[0x24A] != 0) {
            if (*(int *)(data + 0x240) == -1) {
                DebugPrint(plasmabot_missing_intro_path_format, ((short)MOBY(moby)->save_id));
                {
                    call_D678(moby);
                    return;
                }
            }
            if (*((int *)l14_paths[*(int *)(data + 0x240)]) == 0) {
                DebugPrint(plasmabot_empty_intro_path_format, ((short)MOBY(moby)->save_id));
                {
                    call_D678(moby);
                    return;
                }
            }
        }
        if (*(int *)(data + 0x244) == -1) {
            DebugPrint(plasmabot_missing_bound_path_format, ((short)MOBY(moby)->save_id));
            {
                call_D678(moby);
                return;
            }
        }
        if (*((int *)l14_paths[*(int *)(data + 0x244)]) == 0) {
            DebugPrint(plasmabot_empty_bound_path_format, ((short)MOBY(moby)->save_id));
            {
                call_D678(moby);
                return;
            }
        }
        if (((unsigned char *)data)[0x24A] != 0 && *(int *)(data + 0x264) < 0) {
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
        if (*(int *)(data + 0x240) != -1) {
            qcopy(moby + 0x10, (char *)l14_paths[*(int *)(data + 0x240)] + 0x10);
        }
        data[0x28] = 1;
        data[0x29] = 0;
        *(float *)(data + 0x20) = 2.0f;
        *(float *)(data + 0x30) = 1.25f;
        *(short *)(data + 0x24) = 2;
        data[0x58] = truncate_float_to_s32(15.0f);
        data[0x5A] = truncate_float_to_s32(16.0f);
        *(float *)(data + 0x27C) = 1.0f;
        *(short *)(data + 0x248) = 0;
        *(int *)(data + 0x23C) = 0;
        *(short *)(data + 0x260) = 0;
        *(int *)(data + 0x268) = 0;
        *(int *)(data + 0x26C) = 0;
        *(int *)(data + 0x280) = 0;
        *(int *)(data + 0x284) = 0;
        *(int *)(data + 0x270) = 0;
        *(int *)(data + 0x274) = 0;
        if (((unsigned char *)data)[0x121] == 0) {
            attach_manipulator(moby, 2, data + 0x120);
        }
        if (((unsigned char *)data)[0x161] == 0) {
            attach_manipulator(moby, 3, data + 0x160);
        }
        if (((unsigned char *)data)[0x24A] != 0) {
            deactivate_plasmabot(moby);
            ((unsigned char *)moby)[0x20] = 1;
            return;
        }
        MOBY(moby)->pos.z = probe_ground_height(0.5f, moby + 0x10, 0);
        if (*(int *)(data + 0x240) == -1) {
            ((unsigned char *)moby)[0x20] = 4;
        } else {
            ((unsigned char *)moby)[0x20] = 0xD;
        }
        return;
    }
    case PLASMABOT_WAIT_TO_START_PATH_TRAVERSAL: {
        float *path;
        if (*(int *)(data + 0x114) == 2)
            return;
        if (fast_dec_timer_s16(data + 0x248) == 0)
            return;
        if (refresh_plasmabot_hatch_reset_delay(*(int *)(data + 0x264)) == 0)
            return;
        if (call_4BF0(moby) == 0)
            return;
        *(short *)(data + 0x248) = scale_game_frames(plasmabot_path_traversal_delay_frames);
        activate_plasmabot(moby);
        ((unsigned char *)moby)[0x20] = 2;
        set_moby_animation(moby, 1, 0);
        path = (float *)l14_paths[*(int *)(data + 0x240)];
        MOBY(moby)->rot.z = fast_arc_tan(path[8] - path[4], path[9] - path[5]);
        *(int *)(data + 0x268) = 0;
        *(float *)(data + 0x27C) = 1.0f;
        *(int *)(data + 0x26C) = 0;
        *(int *)(data + 0x270) = 0;
        *(int *)(data + 0x274) = 0;
        *(int *)(data + 0x24C) = 0;
        *(int *)(data + 0x250) = 0;
        *(float *)(data + 0x25C) = *(float *)(data + 0x268);
        *(float *)(data + 0x258) = *(float *)(data + 0x268);
        *(float *)(data + 0x254) = *(float *)(data + 0x268);
        return;
    }
    case PLASMABOT_START_PATH_TRAVERSAL:
        if (compute_interpolated_record_value(moby) >= 3.0f) {
            follow_plasmabot_path(moby);
        }
        if ((moby[0x70] & 2) && ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            ((unsigned char *)moby)[0x20] = 3;
            blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            MOBY(moby)->anim_speed = 1.0f;
        }
        break;
    case PLASMABOT_RETURN_TO_PATH_START: {
        float *path = (float *)l14_paths[*(int *)(data + 0x240)];
        vendor_spring(
            (float *)&MOBY(moby)->pos, (float *)(data + 0x254), path[4], 0.01f, 0.2f, 0.0f);
        vendor_spring(
            (float *)&MOBY(moby)->pos.y, (float *)(data + 0x258), path[5], 0.01f, 0.2f, 0.0f);
        if ((moby[0x70] & 2) && ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
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
        ang = fast_arc_tan(*(float *)(owner + 0x10) - MOBY(moby)->pos.x,
                           *(float *)(owner + 0x14) - MOBY(moby)->pos.y);
        k = frame_time_sq * 12.566371f;
        call_CE58((float *)&MOBY(moby)->rot.z,
                  ang,
                  (float *)(data + 0x23C),
                  k,
                  k,
                  frame_time * 6.2831855f);
        if (*(int *)(data + 0x114) == 2)
            break;
        if (fast_dec_timer_s16(data + 0x248) == 0)
            break;
        ((unsigned char *)moby)[0x20] = 3;
        blend_moby_animation(moby, 3, 0, scale_game_frames(10));
        MOBY(moby)->anim_speed = 1.0f;
        *(int *)(data + 0x250) = 0;
        *(float *)(data + 0x27C) = 1.0f;
        *(int *)(data + 0x24C) = 0;
        *(float *)(data + 0x25C) = *(float *)(data + 0x250);
        *(float *)(data + 0x258) = *(float *)(data + 0x250);
        *(float *)(data + 0x254) = *(float *)(data + 0x250);
        break;
    }
    case PLASMABOT_FOLLOW_PATH_FORWARD:
        if (follow_plasmabot_path(moby) != 0) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 4;
        }
        break;
    case PLASMABOT_FOLLOW_PATH_BACKWARD:
        if (((unsigned char *)data)[0x24A] != 0) {
            refresh_plasmabot_hatch_reset_delay(*(int *)(data + 0x264));
        }
        if (follow_plasmabot_path(moby) != 0) {
            if (((unsigned char *)data)[0x24A] != 0) {
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
                ((unsigned char *)moby)[0x20] = 9;
            } else {
                *(short *)(data + 0x248) = scale_game_frames(plasmabot_path_traversal_delay_frames);
                ((unsigned char *)moby)[0x20] = 0xD;
            }
        }
        break;
    case PLASMABOT_AIM_AT_TARGET: {
        float ang;
        float k;
        float aim;
        scale_and_apply_plasmabot_planar_velocity(moby);
        ang = fast_arc_tan(*(float *)(owner + 0x10) - MOBY(moby)->pos.x,
                           *(float *)(owner + 0x14) - MOBY(moby)->pos.y);
        k = frame_time_sq * 12.566371f;
        call_CE58((float *)&MOBY(moby)->rot.z,
                  ang,
                  (float *)(data + 0x23C),
                  k,
                  k,
                  frame_time * 6.2831855f);
        aim = update_plasmabot_aim_quaternion(moby, 0, MOBY(moby)->rot.z);
        update_plasmabot_aim_joint_rotation(moby, ang);
        if (*(int *)(data + 0x114) == 2) {
            if (*(int *)(data + 0x240) == -1)
                break;
            ((unsigned char *)moby)[0x20] = 8;
            if (((unsigned char *)moby)[0x53] != 3) {
                blend_moby_animation(moby, 3, 0, scale_game_frames(0x14));
            }
            {
                int   index = *(int *)(data + 0x24C) - 1;
                char *path;
                *(int *)(data + 0x24C) = index;
                path = (char *)l14_paths[*(int *)(data + 0x240)];
                *(float *)(data + 0x250) = *(float *)(path + (index << 4) + 0x1C);
            }
            *(float *)(data + 0x27C) = -1.0f;
            *(float *)(data + 0x254) = *(float *)(data + 0x258) = *(float *)(data + 0x25C) = 0.0f;
            release_beam_slot_by_id(moby);
            break;
        }
        if (fast_dec_timer_s16(data + 0x260) == 0)
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
        if (test_path_segment_strict_crossing(*(int *)(data + 0x244), moby + 0x10, owner + 0x10) !=
            0)
            break;
        reset_listed_plasmabot_beam_cooldowns(((unsigned char *)moby)[0x21]);
        *(float *)(data + 0x278) = ang;
        ((unsigned char *)moby)[0x20] = 5;
        blend_moby_animation(moby, 4, 0, scale_game_frames(5));
        *(short *)(data + 0x262) = scale_game_frames(l14_plasmabot_aim_sweep_duration_frames);
        break;
    }
    case PLASMABOT_WAIT_ANIMATION_BEFORE_AIM_SWEEP: {
        float ang;
        ang = fast_arc_tan(*(float *)(owner + 0x10) - MOBY(moby)->pos.x,
                           *(float *)(owner + 0x14) - MOBY(moby)->pos.y);
        update_plasmabot_aim_quaternion(
            moby, 0, fast_subtract_rotations(ang, l14_plasmabot_aim_sweep_half_angle));
        update_plasmabot_aim_joint_rotation(moby, ang);
        if ((moby[0x70] & 2) && ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
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
        ang = fast_arc_tan(*(float *)(owner + 0x10) - MOBY(moby)->pos.x,
                           *(float *)(owner + 0x14) - MOBY(moby)->pos.y);
        progress = convert_integer_to_float(*(short *)(data + 0x262));
        progress = progress / convert_integer_to_float(
                                  scale_game_frames(l14_plasmabot_aim_sweep_duration_frames));
        sweep = l14_plasmabot_aim_sweep_half_angle;
        angle =
            fast_subtract_rotations(*(float *)(data + 0x278), -sweep + (sweep + sweep) * progress);
        if (((unsigned char *)data)[0x24B] != 0) {
            if (*(float *)(owner + 0x18) + 0.1f - MOBY(moby)->pos.z < 0.0f) {
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
        info.moby = moby;
        info.flags = 0x10001;
        info.count = 1;
        info.strength = 1.0f;
        info.x = fast_cos(MOBY(moby)->rot.z);
        info.y = fast_sin(MOBY(moby)->rot.z);
        info.scale = 1.0f;
        info.distance = 5627.9248f;
        info.enabled = 1;
        info.oclass = ((unsigned short)MOBY(moby)->oclass);
        info.mode = 0;
        if (collision_line(bone_transform_scratch[3], end, 0, moby, &info) != 0) {
            qcopy(end, D_L14_00174560);
        }
        set_beam_origin_transform_and_endpoint(moby, bone_transform_scratch, end);
        if (fast_dec_timer_s16(data + 0x262) != 0) {
            blend_moby_animation(moby, 6, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 7;
            release_beam_slot_by_id(moby);
        }
        break;
    }
    case PLASMABOT_WAIT_ANIMATION_BEFORE_AIM:
        if ((moby[0x70] & 2) && ((unsigned char *)moby)[0x52] == ((unsigned char *)moby)[0x53]) {
            blend_moby_animation(moby, 0, 0, scale_game_frames(10));
            ((unsigned char *)moby)[0x20] = 4;
        }
        break;
    case PLASMABOT_WAIT_ANIMATION_BEFORE_RESUMING_AIM: {
        long index = ((unsigned char *)moby)[0x52];
        if ((moby[0x70] & 2) && (unsigned char)index == ((unsigned char *)moby)[0x53]) {
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
        if ((mine_fall(moby, data + 0x60) & 0x160) == 0)
            return;
        release_beam_slot_by_id(moby);
        FUN_L00_0024f7c8(moby, 2, bone_transform_scratch);
        spawn_plasmabot_death_debris(moby, (char *)bone_transform_scratch);
        call_F4A8(moby,
                  data + 0x40,
                  moby + 0x10,
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
    if (!(state >= 4 && state <= 7) && state != 1) {
        update_plasmabot_aim_quaternion(moby, 0, MOBY(moby)->rot.z);
        update_plasmabot_aim_joint_rotation(moby, MOBY(moby)->rot.z);
    }
    FUN_L14_002b2600_c((struct Moby *)moby);
}

#include "rnc/math/vector.h"
#include "rnc/mobys/moby.h"

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
    char *data = ((char *)moby->pvars);
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

    qcopy(&quad_transform[3], data + 0x1E0);
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

    qcopy(&quad_transform[3], data + 0x200);
    pp = data + 0x220;
    fast_vec_sub(&quad_transform[0], ((char *)&D_L14_001674C0), &quad_transform[3]);
    beam_start_position = data + 0x210;
    nozzle_position = data + 0x1F0;
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

#include "sda.h"
#include "rnc/mobys/moby.h"

/* pvars of the moby FUN_L14_002b2ed8 updates */
typedef struct {
    u8  pad00[0x38];
    s32 restart; /* 0x38: nonzero picks a new random delay */
    u8  pad3C[0x1F8];
    f32 unk234; /* 0x234 */
    s32 delay;  /* 0x238: frames, counted down */
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
        delay_state->unk234 = plasmabot_base_target_range_gp + 6.0f;
    } else {
        delay_state->unk234 = plasmabot_base_target_range_gp;
    }
}

/* Enemy hit reaction: takes damage from attacks (not from its own kind), staggers or is knocked away and dies. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B41B0), where it is exact; names translated to the US level program. */

extern int
hit_reaction(void *, void *, void *, int, int *, float *, int, int) __asm__("FUN_L00_0025a478");
extern float plasmabot_stagger_motion_z_scale_gp __asm__("D_L14_001614C8") __attribute__((sda));
extern void  trooper_anim_update(void *m, short *p) __asm__("FUN_L00_0025d458");
extern void  trooper_anim_finish(void *, void *) __asm__("FUN_L00_0025d538");
extern void  func_L00_0025D5B0_i(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");

void process_plasmabot_hit_reaction(struct Moby *moby) {
    enum {
        HIT_REACTION_UNAVAILABLE = 0,
        HIT_REACTION_LETHAL = 1,
        HIT_REACTION_DEATH = 2,
        HIT_REACTION_STAGGER = 3,
        HIT_REACTION_NONE = 11
    };
    char *state = ((char *)moby->pvars);
    int   hit_kind;
    float damage;
    char *hit_info;
    int   result;
    if (((unsigned char *)moby)[0x20] == 0 || ((unsigned char *)moby)[0x20] == 0xB) {
        if (((unsigned char *)moby)[0x20] == 0xB) {
            ((unsigned char *)moby)[0xA4] = 0xFF;
            trooper_anim_finish(moby, state + 0xC0);
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
    result = hit_reaction(moby, hit_info, state + 0x20, 0, &hit_kind, &damage, 0, 4);
    if (hit_kind != 1 && ((unsigned char *)moby)[0x20] != 0xB) {
        *(float *)(state + 0x20) -= damage;
        if (*(float *)(state + 0x20) <= 0.0f)
            result = 1;
        switch (result) {
        case HIT_REACTION_STAGGER:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10: {
            char *reaction_motion_state = state + 0x60;
            int   stagger_duration_frames;
            float frame_duration, stagger_motion_z, k;
            release_beam_slot_by_id(moby);
            moby->state = 0xA;
            stagger_duration_frames = scale_game_frames(0x3C);
            frame_duration = frame_time;
            stagger_motion_z = plasmabot_stagger_motion_z_scale_gp * frame_duration;
            k = frame_duration * 8.0f;
            *(short *)(state + 0x26) = stagger_duration_frames;
            *(float *)(state + 0x78) = k;
            *(float *)(state + 0x7C) = stagger_motion_z;
            *(float *)(state + 0xB0) = 11.0f;
            *(float *)(state + 0xB4) = 29.0f;
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
                moby->state = 0xC;
            } else {
                moby->state = 0xB;
            }
            moby->flags &= 0xEFFF;
            {
                int scaled_previous_motion_parameter =
                    truncate_float_to_s32(*(float *)(state + 0x88) * 1024.0f);
                float k = frame_time * 10.0f;
                *(float *)(state + 0x88) = 0.3f;
                *(float *)(state + 0x78) = k;
                *(float *)(state + 0x7C) = k;
                *(float *)(state + 0xB0) = 9.0f;
                *(int *)(state + 0x80) = scaled_previous_motion_parameter;
                *(float *)(state + 0xB4) = 15.0f;
            }
            y = fast_arc_tan(*(float *)(hit_info + 0x10), *(float *)(hit_info + 0x14));
            func_L00_0025D5B0_i(moby, state + 0x60, y, 8, scale_game_frames(5), 0);
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
        trooper_anim_update(moby, (short *)(state + 0xC0));
    }
    ((unsigned char *)moby)[0xA4] = 0xFF;
    trooper_anim_finish(moby, state + 0xC0);
}
/* steers the moby toward its target: queries the path, eases x/y, then turns to face the heading */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002B4668), where it is exact; names translated to the US level program. */

extern int   sample_path(void *, void *, void *, void *, int, float) __asm__("FUN_L00_0025d808");
extern float func_L00_0025CE58_2B4668(
    float *p, float a, float *v, float b, float c, float d) __asm__("FUN_L00_0025be00");

extern float plasmabot_path_traversal_speed_gp __asm__("D_L14_001614D8") __attribute__((sda));

int follow_plasmabot_path(char *moby) {
    float path_target_position[4];
    float pre_spring_position[4];
    float target_offset[4];
    char *data = ((char *)MOBY(moby)->pvars);
    int   reached =
        sample_path(((int *)l14_paths[*(int *)(data + 0x240)]),
                    path_target_position,
                    data + 0x24C,
                    data + 0x250,
                    0,
                    plasmabot_path_traversal_speed_gp * frame_time * *(float *)(data + 0x27C));
    float path_target_heading;
    float k;
    float turn_step_limit;
    qcopy(pre_spring_position, moby + 0x10);
    vendor_spring((float *)&MOBY(moby)->pos,
                  (float *)(data + 0x254),
                  path_target_position[0],
                  0.01f,
                  0.2f,
                  0.0f);
    vendor_spring((float *)&MOBY(moby)->pos.y,
                  (float *)(data + 0x258),
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
                             (float *)(data + 0x23C),
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
    char *state = ((char *)MOBY(moby)->pvars);
    char *owner = *(char **)(state + 0x110);
    float turn = fast_subtract_rotations(target_angle, MOBY(moby)->rot.z);
    if (turn > 1.2217304706573486f)
        turn = 1.2217304706573486f;
    else if (turn < -1.2217304706573486f)
        turn = -1.2217304706573486f;
    {
        float yaw_easing_coefficient = frame_time_sq * 12.566370964050293f;
        func_L00_0025CE58_2B4828((float *)(state + 0x268),
                                 turn,
                                 (float *)(state + 0x26C),
                                 yaw_easing_coefficient,
                                 yaw_easing_coefficient,
                                 frame_time * 6.2831854820251465f);
    }
    FUN_L00_001fff28(state + 0x170, 2, *(float *)(state + 0x268));
    if (flag) {
        float        owner_aim_position[4];
        float        owner_aim_offset[4];
        float        pitch_quaternion[4];
        float       *bp;
        float       *cp;
        struct Hero *hero_state;
        float        target_pitch_angle;
        qcopy(owner_aim_position, owner + 0x10);
        hero_state = &hero;
        if (((char *)hero_state->moby) == owner)
            owner_aim_position[2] = owner_aim_position[2] - hero_state->ground_distance;
        bp = owner_aim_offset;
        fast_vec_sub(bp, owner_aim_position, moby + 0x10);
        owner_aim_offset[2] = owner_aim_offset[2] + 1.5f;
        target_pitch_angle = -fast_arc_tan(vector_length_xy(bp), owner_aim_offset[2]);
        func_L00_0025CE58_2B4828((float *)(state + 0x280),
                                 target_pitch_angle,
                                 (float *)(state + 0x284),
                                 frame_time_sq * 12.566370964050293f,
                                 frame_time_sq * 12.566370964050293f,
                                 frame_time * 6.2831854820251465f);
        cp = pitch_quaternion;
        FUN_L00_001fff28(cp, 1, *(float *)(state + 0x280));
        multiply_quaternions(state + 0x170, state + 0x170, cp);
    }
    return fast_difference_between_rotations(turn, *(float *)(state + 0x268));
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
        unsigned char *other = (unsigned char *)(D_L14_0015FFD8_3920 + ((v & 0x7FFF) << 8));
        if (*(short *)(other + 0xA6) == 0x1C) {
            if ((unsigned char)(other[0x20] - 5) <= 1)
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
        (moby_list_entry = (unsigned short *)D_L14_001ABF40[source_moby[0x21]]) == 0)
        return 1;
    do {
        char *moby = D_L14_0015FFD8 + ((*moby_list_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass == 0x1C && MOBY(moby)->state == 2)
            return 0;
    } while ((short)*moby_list_entry++ >= 0);
    return 1;
}

/* Level overlay code; generated by `decomp overlays stubs`, stubs replaced by C as functions are matched. */
#include "qcopy.h"
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

extern u8 *D_L14_00167500 NOT_SDA;

void plasmabot_hatch_update(struct Moby *moby) __asm__("FUN_L14_002d96e0");

void plasmabot_hatch_update(struct Moby *moby) {
    enum {
        HATCH_WAIT_ACTIVATION = 0,
        HATCH_WAIT_ACTIVATION_DELAY = 1,
        HATCH_WAIT_ACTIVATION_ANIMATION = 2,
        HATCH_WAIT_ACTIVATION_GATE_CLEAR = 3,
        HATCH_IDLE_AFTER_ACTIVATION_GATE_CLEAR = 4,
        HATCH_WAIT_ANIMATION_BEFORE_RESET_DELAY = 5,
        HATCH_WAIT_RESET_DELAY = 6,
        HATCH_WAIT_RESET_ANIMATION = 7
    };
    char *data = (char *)moby->pvars;
    if (hero.vehicle_mode == 2) {
        moby->collision = 0;
    } else {
        moby->collision = moby->pclass->collision;
    }
    switch (moby->state) {
    case HATCH_WAIT_ACTIVATION:
        if (*(s32 *)data != 0 && *(s16 *)(D_L14_00167500 + 0x86) == 0x14) {
            moby->state = 1;
            *(s32 *)(data + 8) = scale_game_frames(*(s32 *)(data + 4));
        }
        break;
    case HATCH_WAIT_ACTIVATION_DELAY:
        if (fast_dec_timer(data + 8) != 0) {
            moby->state = 2;
            set_moby_animation(moby, 1, 0);
        }
        break;
    case HATCH_WAIT_ACTIVATION_ANIMATION:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 3;
            set_moby_animation(moby, 2, 0);
        }
        break;
    case HATCH_WAIT_ACTIVATION_GATE_CLEAR:
        if (*(s16 *)(D_L14_00167500 + 0x86) != 0x14) {
            set_moby_animation(moby, 0, 0);
            moby->state = 4;
        }
        break;
    case HATCH_IDLE_AFTER_ACTIVATION_GATE_CLEAR:
        break;
    case HATCH_WAIT_ANIMATION_BEFORE_RESET_DELAY:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 6;
            set_moby_animation(moby, 2, 0);
        }
        break;
    case HATCH_WAIT_RESET_DELAY:
        if (fast_dec_timer(data + 8) != 0) {
            set_moby_animation(moby, 3, 0);
            moby->state = 7;
        }
        break;
    case HATCH_WAIT_RESET_ANIMATION:
        if ((moby->anim_flags & 2) != 0) {
            moby->state = 0;
            set_moby_animation(moby, 0, 0);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002DAC00), where it is exact; names translated to the US level program. */

typedef struct {
    char           pad0[0x10];
    char           pos[0x10];
    unsigned char  b20;
    char           pad21[0xB];
    float          f2C;
    unsigned char  b30;
    unsigned char  b31;
    short          s32;
    unsigned short h34;
    char           pad36[0x12];
    float          f48;
    char           pad4C[0x2C];
    Child         *child;
    char           pad7C[0x14];
    int            f90;
} Obj;

int refresh_plasmabot_hatch_reset_delay(int i) {
    struct Moby *moby = (unsigned char *)(D_L14_ffd0_e_macro[2] + i * 256);
    int         *data = ((int *)moby->pvars);
    int          hatch_reset_delay_active = 0;
    if (moby->state == 0) {
        moby->state = 5;
        set_moby_animation(moby, 1, 0);
    } else {
        hatch_reset_delay_active = moby->state == 6;
    }
    data[2] = scale_game_frames(0x3C);
    return hatch_reset_delay_active;
}

extern float D_0015ED60;
#define DE670_THRESHOLD D_L14_00161B20
#define DE670_DAMPING   D_L14_00161B1C
#define DE670_GRAVITY   D_L14_00161B18
extern void *D_001413D0_lui __asm__("D_001413D0") NOT_SDA;
extern void  FUN_L14_002dec40(struct Moby *);
extern s32   FUN_L00_00257b90(s32, s32);
extern s32   coll_sphere(f32, void *, s32, void *) __asm__("FUN_L00_001f0d60");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             f32,
                             f32,
                             s32,
                             s32,
                             s32,
                             f32,
                             f32,
                             f32,
                             f32,
                             s32,
                             f32,
                             s32,
                             s32,
                             s32,
                             s32) __asm__("FUN_L00_0025e450");
extern void
FUN_L00_0026cbb0_de670(void *, void *, s32, s32, s32, s32, f32) __asm__("FUN_L00_0026cbb0");
extern u8 *
FUN_L00_0026d000_de670(void *, f32, f32, f32, s32, void *, f32, s32) __asm__("FUN_L00_0026d000");

extern f32 D_L14_00161B18 __attribute__((sda));
extern f32 D_L14_00161B1C __attribute__((sda));
extern f32 D_L14_00161B20 __attribute__((sda));
void       update_spinning_moby_with_particle_trail(struct Moby *moby) __asm__("FUN_L14_002de670");

void update_spinning_moby_with_particle_trail(struct Moby *moby) {
    f32 *velocity = (f32 *)moby->pvars;
    s32  i;
    s32  signed_particle_variation;
    s32  alpha;
    s32  color;
    f32  size;
    f32  absolute_velocity;
    u8  *particle;
    u8  *fade;

    if (random_integer_below(2) != 0) {
        add_draw_callback(FUN_L14_002dec40, moby);
    }
    fast_vec_add(&moby->pos, &moby->pos, velocity);
    absolute_velocity = absolute_float(velocity[0]);
    if (DE670_THRESHOLD * D_0015ED60 < absolute_velocity) {
        velocity[0] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    absolute_velocity = absolute_float(velocity[1]);
    if (DE670_THRESHOLD * D_0015ED60 < absolute_velocity) {
        velocity[1] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    if (velocity[2] > 0.0f) {
        velocity[2] *= (DE670_DAMPING - 1.0f) * D_0015ED60 + 1.0f;
    }
    velocity[2] -= DE670_GRAVITY * frame_scale_sq;
    moby->rot.x = fast_add_rotations(moby->rot.x, velocity[4]);
    moby->rot.y = fast_add_rotations(moby->rot.y, velocity[5]);
    moby->rot.z = fast_add_rotations(moby->rot.z, velocity[6]);

    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f ||
        1021.0f < moby->pos.y || moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    if (fast_dec_timer(velocity + 8) != 0 ||
        coll_sphere(0.5f, &moby->pos, 0, *(void **)(velocity + 9)) != 0) {
        spawn_explosion(moby,
                        velocity,
                        &moby->pos,
                        0.0f,
                        0.0f,
                        5,
                        2,
                        4,
                        1.0f,
                        0.5f,
                        9.0f,
                        1.0f,
                        0,
                        15.0f,
                        1,
                        5,
                        -1,
                        0);
        coll_sphere(1.0f, &moby->pos, 0, D_001413D0_lui);
        mark_moby_for_removal(moby);
        return;
    }
    FUN_L00_0026cbb0_de670(&moby->pos,
                           D_L14_0015F580,
                           0x4F007FFF,
                           0x1FFFFFFF,
                           FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)),
                           1,
                           60000.0f);
    for (i = 0; i < 2; i++) {
        signed_particle_variation = random_integer_below(6);
        if (random_integer_below(2) != 0)
            signed_particle_variation = -signed_particle_variation;
        size = random_float_between(40000.0f, 100000.0f);
        color = FUN_L00_00257b90(0x30, 0xFF);
        alpha = 0x60000000;
        particle = FUN_L00_0026d000_de670(&moby->pos,
                                          0.025f,
                                          1.0f,
                                          1.0f,
                                          signed_particle_variation,
                                          D_L14_0015F580,
                                          size,
                                          color | ((color << 16) | ((color << 8) | alpha)));
        if (particle != 0) {
            particle[3] = 0x44;
            fade = particle + 0x20;
            *(s16 *)(particle + 0xA) = scale_game_frames(0x3C);
            *(s32 *)(fade + 4) = 2;
            fade[0xA] = 0x60;
            fade[0xB] = particle[0xA];
        }
    }
}

#include "sda.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B2A28.c: func_L14_002DFE98), where it is exact; names translated to the US level program. */

extern float FUN_L00_00257c48(float lo, float hi);
extern void
build_look_at_matrix(void *dst, void *vec, void *axis, float angle) __asm__("FUN_00214890");

extern float plasmabot_debris_launch_magnitude_range_start_gp __asm__("D_L14_00161B08")
    __attribute__((sda));
extern float plasmabot_debris_launch_magnitude_range_end_gp __asm__("D_L14_00161B0C")
    __attribute__((sda));
extern float plasmabot_debris_z_velocity_range_start_gp __asm__("D_L14_00161B10")
    __attribute__((sda));
extern float plasmabot_debris_z_velocity_range_end_gp __asm__("D_L14_00161B14")
    __attribute__((sda));

unsigned char *spawn_plasmabot_death_debris(char *owner, char *pos) {
    unsigned char *spawned_moby = create_moby(0x145);
    char          *spawned_vars;
    char          *hero_launch_reference;
    float          launch_vector_scratch[8];
    if (spawned_moby != 0) {
        spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        *(char **)(spawned_vars + 0x24) = owner;
        qcopy(spawned_moby + 0x10, pos);
        MOBY(spawned_moby)->rot.z = *(float *)(owner + 0x48);
        spawned_moby[0x20] = 0;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        hero_launch_reference = (char *)&hero.motion.pos;
        fast_vec_sub(launch_vector_scratch, spawned_moby + 0x10, hero_launch_reference);
        hero_launch_reference = hero_launch_reference + 0x210;
        launch_vector_scratch[2] = 0.0f;
        fast_vec_normalize(
            launch_vector_scratch,
            launch_vector_scratch,
            random_float_between(plasmabot_debris_launch_magnitude_range_start_gp * frame_scale,
                                 plasmabot_debris_launch_magnitude_range_end_gp * frame_scale));
        fast_vec_cross(launch_vector_scratch + 4, hero_launch_reference, launch_vector_scratch);
        build_look_at_matrix(spawned_vars,
                             launch_vector_scratch,
                             hero_launch_reference,
                             FUN_L00_00257c48(0.34906584f, 1.0471976f));
        *(float *)(spawned_vars + 8) =
            random_float_between(plasmabot_debris_z_velocity_range_start_gp * frame_scale,
                                 plasmabot_debris_z_velocity_range_end_gp * frame_scale);
        *(float *)(spawned_vars + 0x10) = FUN_L00_00257c48(0.0f, 0.008726646f);
        *(float *)(spawned_vars + 0x14) = FUN_L00_00257c48(0.05235988f, 0.13962634f);
        *(float *)(spawned_vars + 0x18) = FUN_L00_00257c48(0.0f, DEG_TO_RAD);
        *(int *)(spawned_vars + 0x20) = scale_game_frames(200);
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

extern void emit_debris_trail_particles(struct Moby *) __asm__("FUN_L14_002e03b8");
extern s32  D_001413D0_dfd10 __asm__("D_001413D0") NOT_SDA;
extern void spawn_explosion_dfd10(struct Moby *,
                                  void *,
                                  void *,
                                  f32,
                                  f32,
                                  s32,
                                  s32,
                                  s32,
                                  f32,
                                  f32,
                                  f32,
                                  f32,
                                  s32,
                                  f32,
                                  s32,
                                  s32,
                                  s32,
                                  s32) __asm__("FUN_L00_0025e450");

typedef struct {
    struct Moby *owner;
    s32          timer;
    f32          path_speed;
    s32          path;
    f32          path_t;
    f32          path_u;
    f32          offset_x;
    f32          offset_y;
    f32          spin_x;
    f32          spin_y;
    f32          spin_z;
    u8           pad2C[4];
    f32          velocity[3];
} Data;

extern f32 ballistic_debris_gravity_gp __asm__("D_L14_00161B40") __attribute__((sda));
extern f32 ballistic_debris_velocity_base_scale_gp __asm__("D_L14_00161B44") __attribute__((sda));
extern f32 ballistic_debris_planar_damping_speed_threshold_gp __asm__("D_L14_00161B48")
    __attribute__((sda));
/* A piece of debris: follows its race path until its timer runs out, then flies, spins and falls until it hits something or leaves the map. */
void update_path_or_ballistic_debris(struct Moby *moby) __asm__("FUN_L14_002dfd10");

void update_path_or_ballistic_debris(struct Moby *moby) {
    Data *data = (Data *)moby->pvars;
    Vec4  probe;
    f32   position_offset[4];
    f32  *velocity;
    void *points;
    f32   speed;

    if (moby->state == 0) {
        points = l14_splines[data->path].path;
        qcopy(&probe, &moby->pos);
        sample_path(points, &moby->pos, &data->path_t, &data->path_u, 0, data->path_speed);
        fast_vec_sub(position_offset, &moby->pos, &probe);
        data->offset_x = position_offset[0];
        data->offset_y = position_offset[1];
        emit_debris_trail_particles(moby);
        if (fast_dec_timer(&data->timer) == 0)
            return;
        fast_vec_normalize(position_offset, position_offset, 0.5f);
        qcopy(&probe, &moby->pos);
        probe.f[2] += 0.25f;
        trooper_play_sound(1, 0, moby, data->owner->oclass);
        spawn_explosion_dfd10(moby,
                              position_offset,
                              &probe,
                              0.0f,
                              0.0f,
                              5,
                              2,
                              4,
                              1.0f,
                              0.5f,
                              9.0f,
                              1.0f,
                              -1,
                              15.0f,
                              0,
                              3,
                              -1,
                              0);
        coll_sphere(1.0f, &moby->pos, 0, (void *)D_001413D0_dfd10);
        mark_moby_for_removal(moby);
        return;
    }
    velocity = data->velocity;
    fast_vec_add(&moby->pos, &moby->pos, velocity);
    speed = absolute_float(data->velocity[0]);
    if (ballistic_debris_planar_damping_speed_threshold_gp * frame_scale < speed)
        data->velocity[0] *= (ballistic_debris_velocity_base_scale_gp - 1.0f) * frame_scale + 1.0f;
    speed = absolute_float(data->velocity[1]);
    if (ballistic_debris_planar_damping_speed_threshold_gp * frame_scale < speed)
        data->velocity[1] *= (ballistic_debris_velocity_base_scale_gp - 1.0f) * frame_scale + 1.0f;
    if (data->velocity[2] > 0.0f)
        data->velocity[2] *= (ballistic_debris_velocity_base_scale_gp - 1.0f) * frame_scale + 1.0f;
    data->velocity[2] -= ballistic_debris_gravity_gp * frame_scale_sq;
    moby->rot.x = fast_add_rotations(moby->rot.x, data->spin_x);
    moby->rot.y = fast_add_rotations(moby->rot.y, data->spin_y);
    moby->rot.z = fast_add_rotations(moby->rot.z, data->spin_z);
    if (moby->pos.x < 2.0f || 1021.0f < moby->pos.x || moby->pos.y < 2.0f ||
        1021.0f < moby->pos.y || moby->pos.z < 2.0f || 1021.0f < moby->pos.z) {
        mark_moby_for_removal(moby);
        return;
    }
    fast_vec_normalize(position_offset, &moby->rot_row2, 0.52f);
    fast_vec_add(&probe, &moby->pos, position_offset);
    if (fast_dec_timer(&data->timer) != 0 || coll_sphere(0.5f, &probe, 0, data->owner) != 0) {
        trooper_play_sound(0, 0, moby, data->owner->oclass);
        spawn_explosion_dfd10(moby,
                              velocity,
                              &moby->pos,
                              0.0f,
                              0.0f,
                              5,
                              2,
                              4,
                              1.0f,
                              0.5f,
                              9.0f,
                              1.0f,
                              -1,
                              15.0f,
                              0,
                              3,
                              -1,
                              0);
        coll_sphere(1.0f, &moby->pos, 0, (void *)D_001413D0_dfd10);
        mark_moby_for_removal(moby);
        return;
    }
    if (random_integer_below(2) != 0)
        emit_debris_trail_particles(moby);
}

extern void create_effect_2e0170(
    void *, void *, void *, void *, void *, int, float, float, float) __asm__("FUN_L00_0025df68");

extern float ballistic_debris_launch_magnitude_range_start_gp __asm__("D_L14_00161B30")
    __attribute__((sda));
extern float ballistic_debris_launch_magnitude_range_end_gp __asm__("D_L14_00161B34")
    __attribute__((sda));
extern float ballistic_debris_z_velocity_range_start_gp __asm__("D_L14_00161B38")
    __attribute__((sda));
extern float ballistic_debris_z_velocity_range_end_gp __asm__("D_L14_00161B3C")
    __attribute__((sda));

unsigned char *spawn_path_or_ballistic_debris(char *owner, float *scale, int mode) {
    unsigned char *spawned_moby = create_moby(0x193);
    unsigned char *pos;
    if (spawned_moby != 0) {
        char *spawned_vars = ((char *)MOBY(spawned_moby)->pvars);
        float launch_vector_scratch[8];
        pos = spawned_moby + 0x10;
        qcopy(pos, owner + 0x10);
        MOBY(spawned_moby)->rot.z = *(float *)(owner + 0x48);
        spawned_moby[0x20] = mode;
        spawned_moby[0x30] = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        *(char **)spawned_vars = owner;
        if (spawned_moby[0x20] == 0) {
            int          index;
            LevelSpline *splines;
            int         *fields = (int *)spawned_vars;
            fields[1] = scale_game_frames(90);
            splines = l14_splines;
            *(float *)(spawned_vars + 8) = *scale * frame_time;
            index = *(int *)(*(char **)(owner + 0x78) + 0x60);
            fields[5] = 0;
            fields[3] = index;
            fields[4] = 0;
            create_effect_2e0170(splines[index].path,
                                 pos,
                                 launch_vector_scratch,
                                 fields + 4,
                                 fields + 5,
                                 0,
                                 20.0f,
                                 5.0f,
                                 *(float *)(spawned_vars + 0x14));
        } else {
            float angle;
            *(int *)(spawned_vars + 4) = scale_game_frames(200);
            fast_vec_sub(launch_vector_scratch, pos, &hero.motion.pos);
            launch_vector_scratch[2] = 0.0f;
            fast_vec_normalize(
                launch_vector_scratch,
                launch_vector_scratch,
                random_float_between(ballistic_debris_launch_magnitude_range_start_gp * D_0015ED60,
                                     ballistic_debris_launch_magnitude_range_end_gp * D_0015ED60));
            fast_vec_cross(launch_vector_scratch + 4, &hero.gravity_dir, launch_vector_scratch);
            angle = randf_sym(0.5759586691856384f, 1.3962633609771729f);
            build_look_at_matrix(
                spawned_vars + 0x30, launch_vector_scratch, &hero.gravity_dir, angle);
            *(float *)(spawned_vars + 0x38) =
                random_float_between(ballistic_debris_z_velocity_range_start_gp * D_0015ED60,
                                     ballistic_debris_z_velocity_range_end_gp * D_0015ED60);
            *(float *)(spawned_vars + 0x20) = randf_sym(0.0f, 0.008726646f);
            *(float *)(spawned_vars + 0x24) = randf_sym(0.05235988f, 0.13962634f);
            *(float *)(spawned_vars + 0x28) = randf_sym(0.0f, 0.017453293f);
        }
        update_entity(spawned_moby);
    }
    return spawned_moby;
}
/* Ported from rac1-decomp src/overlays/shared/vendor_002B2A28.c (func_L14_002E17B8) */
/* Exhaust puffs from the moby's nozzle: two coloured sparks and three white puffs of shrinking size. */

typedef struct Moby Moby;

typedef int u128_2E17B8 __attribute__((mode(TI)));

typedef union {
    u128_2E17B8 q;
    float       f[4];
} Vec4_2E17B8;

typedef struct Vars {
    char  pad0[0x18];
    float f18; /* 0x18 */
    float f1C; /* 0x1C */
} Vars;

typedef struct ParticleFade_2E17B8 {
    char          pad0[4];
    int           i4; /* 0x04 */
    char          pad8[2];
    unsigned char bA; /* 0x0A */
    unsigned char bB; /* 0x0B */
    char          padC[4];
    float         scale; /* 0x10 */
    char          pad14[2];
    short         mode;  /* 0x16 */
    int           color; /* 0x18 */
} ParticleFade_2E17B8;

typedef struct Particle_2E17B8 {
    char                pad0[3];
    unsigned char       b3; /* 0x03 */
    char                pad4[6];
    short               frames; /* 0x0A */
    float               life;   /* 0x0C */
    char                pad10[0x10];
    ParticleFade_2E17B8 fade; /* 0x20 */
} Particle_2E17B8;

typedef struct {
    unsigned char pad0[4];
    int           w4;
    unsigned char pad8[2];
    unsigned char bA;
    unsigned char bB;
} T_eca8;

typedef struct {
    unsigned char pad0[8];
    unsigned char b8;
    unsigned char b9;
    short         hA;
    unsigned char padC[0x14];
    T_eca8        tail;
} P_eca8;

char *
spawn_burst(void *pos, void *dir, int c, int d, int n, int k, float f) __asm__("FUN_L00_0026cbb0");
extern P_eca8 *FUN_L00_0026d000(void *, float, float, float, int, void *, float, int);
extern int     D_L14_0015F580_c __asm__("D_L14_0015F580");
int            FUN_L00_00257b90(int lo, int hi);
extern float   func_002140F8_2E17B8(float, float) __asm__("random_float_between");
extern int     func_002140B0_2E17B8(int) __asm__("random_integer_below");
extern void
func_L00_0026DA50_2E17B8(void *, void *, int, int, int, int, float) __asm__("FUN_L00_0026cbb0");
extern Particle_2E17B8 *func_L00_0026DEA0_2E17B8(
    void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");

void emit_debris_trail_particles(Moby *moby) {
    Vec4_2E17B8          pos;
    Vec4_2E17B8          emission_axis_offset;
    Vec4_2E17B8          particle_initial_velocity;
    Vec4_2E17B8          emission_position_offset;
    Vars                *debris_motion_state;
    Particle_2E17B8     *particle;
    Particle_2E17B8     *spawned_particle;
    ParticleFade_2E17B8 *fade;
    int                  i;
    int                  signed_particle_variation;
    int                  color_intensity;
    int                  alpha;
    float                size;

    debris_motion_state = *(Vars **)&moby->pvars;
    fast_vec_normalize(&emission_axis_offset, &*(Vec4_2E17B8 *)&moby->rot_row2, 0.2f);
    fast_vec_add(&pos, &*(Vec4_2E17B8 *)&moby->pos, &emission_axis_offset);
    random_spherical_offset(&particle_initial_velocity, 0.005f, 0.03f);
    particle = spawn_mode_controlled_rotating_alpha_fade_particle(
        &pos, &particle_initial_velocity, 0x7F, moby);
    if (particle != 0) {
        particle->life = func_002140F8_2E17B8(6000.0f, 32000.0f);
    }
    for (i = 0; i < 3; i++) {
        float f;
        int   color;
        particle =
            spawn_mode_controlled_rotating_alpha_fade_particle(&pos, &D_L14_0015F580_c, 0x7F, moby);
        if (particle == 0) {
            continue;
        }
        fade = &particle->fade;
        if (i == 2 && func_002140B0_2E17B8(8) == 0) {
            particle->life = 180000.0f;
        } else {
            particle->life = func_002140F8_2E17B8(80000.0f, 120000.0f);
        }
        particle->frames = scale_game_frames(2);
        f = 1.0f / convert_integer_to_float(particle->frames);
        color = 0x7F7F7F;
        fade->mode = 4;
        fade->color = color;
        fade->scale = f;
    }
    particle_initial_velocity.f[0] = debris_motion_state->f18;
    particle_initial_velocity.f[1] = debris_motion_state->f1C;
    particle_initial_velocity.f[2] = 0.0f;
    particle_initial_velocity.f[3] = 1.0f;
    if (moby->state == 0) {
        fast_vec_normalize(&emission_axis_offset, &*(Vec4_2E17B8 *)&moby->rot_row2, 0.95f);
    } else {
        fast_vec_normalize(&emission_axis_offset, &*(Vec4_2E17B8 *)&moby->rot_row2, 1.15f);
        particle_initial_velocity.f[2] = D_0015ED60 * 0.03f;
    }
    fast_vec_add(&pos, &*(Vec4_2E17B8 *)&moby->pos, &emission_axis_offset);
    emission_position_offset.f[0] = fast_cos(moby->rot.z) * 0.3f;
    emission_position_offset.f[1] = fast_sin(moby->rot.z) * 0.3f;
    emission_position_offset.f[2] = 0.0f;
    fast_vec_add(&pos, &pos, &emission_position_offset);
    if (moby->state == 1) {
        pos.f[0] += randf_sym(0.0f, 0.1f);
        pos.f[1] += randf_sym(0.0f, 0.1f);
    }
    func_L00_0026DA50_2E17B8(&pos,
                             &particle_initial_velocity,
                             0x4F007FFF,
                             0x1FFFFFFF,
                             random_int_between(scale_game_frames(10), scale_game_frames(20)),
                             1,
                             60000.0f);
    signed_particle_variation = func_002140B0_2E17B8(6);
    if (func_002140B0_2E17B8(2) != 0) {
        signed_particle_variation = -signed_particle_variation;
    }
    size = func_002140F8_2E17B8(60000.0f, 140000.0f);
    color_intensity = random_int_between(0x30, 0xFF);
    alpha = 0x60000000;
    spawned_particle = func_L00_0026DEA0_2E17B8(
        &pos,
        0.025f,
        1.0f,
        1.0f,
        signed_particle_variation,
        &particle_initial_velocity,
        size,
        color_intensity | ((color_intensity << 16) | ((color_intensity << 8) | alpha)));
    if (spawned_particle != 0) {
        ParticleFade_2E17B8 *ef;
        ef = &spawned_particle->fade;
        spawned_particle->b3 = 0x44;
        spawned_particle->frames = scale_game_frames(0x3C);
        ef->i4 = 2;
        ef->bA = 0x60;
        ef->bB = spawned_particle->frames;
    }
}

/* Exhaust puffs from the moby's nozzle: two coloured sparks and three white puffs of shrinking size. */
extern P_eca8 *
spawn_puff(void *, float, float, float, int, void *, float, int) __asm__("FUN_L00_0026d000");
extern float D_L14_0015F580_macro[] __asm__("D_L14_0015F580") MACRO_ADDR;

extern float particle_pair_moby_c0_offset_scale_gp __asm__("D_L14_00161FCC") __attribute__((sda));
extern float particle_pair_moby_e0_offset_scale_gp __asm__("D_L14_00161FD0") __attribute__((sda));
extern float particle_trio_moby_c0_offset_scale_gp __asm__("D_L14_00161FD4") __attribute__((sda));
extern float particle_trio_moby_e0_offset_scale_gp __asm__("D_L14_00161FD8") __attribute__((sda));
extern int   particle_pair_color_gp __asm__("D_L14_00161FE4") __attribute__((sda));
void         emit_moby_offset_particles(unsigned char *moby) __asm__("FUN_L14_002feca8");

void emit_moby_offset_particles(unsigned char *moby) {
    float emission_position_offset[4];
    float particle_spawn_position[4];
    int   i;
    int   j;
    int   signed_particle_variation;
    int   life;
    float size;
    fast_vec_scale(emission_position_offset, moby + 0xC0, particle_pair_moby_c0_offset_scale_gp);
    fast_vec_add(particle_spawn_position, moby + 0x10, emission_position_offset);
    fast_vec_scale(emission_position_offset, moby + 0xE0, particle_pair_moby_e0_offset_scale_gp);
    fast_vec_add(particle_spawn_position, particle_spawn_position, emission_position_offset);
    for (i = 0; i < 2; i++) {
        P_eca8 *part;
        T_eca8 *tail;
        signed_particle_variation = random_integer_below(0x10);
        part = spawn_puff(particle_spawn_position,
                          0.2f,
                          1.0f,
                          0.9f,
                          random_integer_below(2) == 0 ? signed_particle_variation
                                                       : -signed_particle_variation,
                          D_L14_0015F580_macro,
                          100000.0f,
                          particle_pair_color_gp);
        if (part) {
            tail = &part->tail;
            part->hA = scale_game_frames(0xC);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->hA;
        }
    }
    fast_vec_scale(emission_position_offset, moby + 0xC0, particle_trio_moby_c0_offset_scale_gp);
    fast_vec_add(particle_spawn_position, moby + 0x10, emission_position_offset);
    fast_vec_scale(emission_position_offset, moby + 0xE0, particle_trio_moby_e0_offset_scale_gp);
    fast_vec_add(particle_spawn_position, particle_spawn_position, emission_position_offset);
    signed_particle_variation = 0x10;
    life = scale_game_frames(2);
    size = 80000.0f;
    for (j = 0; j < 3; j++) {
        P_eca8 *part;
        T_eca8 *tail;
        part = spawn_puff(particle_spawn_position,
                          0.05f,
                          1.0f,
                          1.0f,
                          signed_particle_variation,
                          D_L14_0015F580_macro,
                          size,
                          0x7FFFFFFF);
        if (part) {
            tail = &part->tail;
            part->hA = life;
            part->b8 = random_integer_below(0xFF);
            tail->w4 = 2;
            tail->bA = 0x7F;
            tail->bB = part->hA;
        }
        signed_particle_variation = -signed_particle_variation;
        size -= 20000.0f;
        life *= 2;
    }
}

typedef struct {
    struct Moby *link;
    float        rest_z;
    float        z_speed;
    float        phase;
    float        spin[3];
} FloatMineVars;

typedef struct {
    float damage;
    s16   unk4;
    u8    pad6[0xA];
    float unk10;
    u8    pad14[0x26];
    u8    unk3A;
    u8    unk3B;
    u8    pad3C[4];
} HitInfo;

typedef struct {
    u8           pad0[0x20];
    struct Moby *moby;
} MobyHit;

extern char *floating_mine_respawn_address_limit __asm__("D_L14_0015FFDC");

#include "rnc/overlay/shared/enemies/plasmabot_data/update_floating_mine.h"

extern float probe_ground_height_ff640(float, void *, int) __asm__("FUN_00213508");
extern int   fun_00213928(struct Moby *, MobyHit *, HitInfo *, int, int *, int, int, int) __asm__("FUN_00213928");
extern void explode_ff640(struct Moby *,
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

/* Floating mine: spins and pulses (and bobs in level 16); blows up on a hit and respawns once out of view. */
void update_floating_mine(struct Moby *moby) __asm__("FUN_L14_002ff640");

void update_floating_mine(struct Moby *moby) {
    enum {
        FLOAT_MINE_INITIALIZE = 0,
        FLOAT_MINE_ACTIVE = 1,
        FLOAT_MINE_WAIT_OFFSCREEN_RESPAWN = 2
    };
    FloatMineVars *vars = (FloatMineVars *)moby->pvars;
    union {
        HitInfo info;
        float   sphere[4];
    } scratch;
    float    explosion_vector[4];
    int      count;
    MobyHit *hit;
    float    glow_pulse_sine;
    int      glow_red_intensity;
    int      glow_green_blue_intensity;

    switch (moby->state) {
    case FLOAT_MINE_INITIALIZE:
        vars->rest_z = -1.0f;
        if (vars->link == 0) {
            moby->pos.z = probe_ground_height_ff640(0.5f, &moby->pos, 0) + 0.7f;
            vars->spin[0] = random_float_between(0.010471975f, 0.020943951f);
            vars->spin[1] = random_float_between(0.006981317f, 0.013962634f);
            vars->spin[2] = random_float_between(0.0034906585f, 0.006981317f);
            moby->state = 1;
        } else if ((s8)vars->link->state < 0) {
            moby->state = 1;
        }
        break;
    case FLOAT_MINE_ACTIVE: {
        if (current_level_index == 16) {
            if (vars->rest_z < 0.0f)
                vars->rest_z = probe_ground_height_ff640(0.5f, &moby->pos, 0) + 0.7f;
            vendor_spring(&moby->pos.z,
                          &vars->z_speed,
                          vars->rest_z,
                          frame_scale_sq * 0.03f,
                          frame_scale_sq * 0.3f,
                          0.0f);
            if ((char *)moby > floating_mine_respawn_address_limit && hero.state.control_mode != 15)
                goto remove;
        }
        hit = find_moby_hit(moby, 0x230000, 0);
        fast_mem_zero16(&scratch.info, 0x40);
        scratch.info.unk4 = 1;
        scratch.info.damage = 1.0f;
        scratch.info.unk10 = 0.5f;
        scratch.info.unk3A = 7;
        scratch.info.unk3B = 0xFF;
        fun_00213928(moby, hit, &scratch.info, 0, &count, 0, 0, 4);
        if ((count >= 2 && hit != 0 && hit->moby->oclass != 0x283) ||
            hero.coll_hit_moby == (u8 *)moby) {
            clear_u64_value(explosion_vector);
            allocate_voice_for_target_entry(0, 0, moby);
            if (hero.coll_hit_moby == (u8 *)moby)
                explode_ff640(moby,
                              explosion_vector,
                              &moby->pos,
                              3.0f,
                              1.0f,
                              10,
                              20,
                              60,
                              3.0f,
                              5.0f,
                              9.0f,
                              1.0f,
                              -1,
                              15.0f,
                              1,
                              1,
                              -1,
                              0);
            else
                explode_ff640(moby,
                              explosion_vector,
                              &moby->pos,
                              0.0f,
                              0.0f,
                              10,
                              20,
                              60,
                              3.0f,
                              5.0f,
                              9.0f,
                              1.0f,
                              -1,
                              15.0f,
                              1,
                              1,
                              -1,
                              0);
            if ((char *)moby < floating_mine_respawn_address_limit) {
                moby->state = 2;
                moby->flags |= 0x41;
                moby->hit_slot = 0xFF;
                moby->collision = 0;
                moby->update_dist = 0xFF;
                return;
            }
        remove:
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    case FLOAT_MINE_WAIT_OFFSCREEN_RESPAWN: {
        qcopy(scratch.sphere, &moby->pos);
        scratch.sphere[3] = 3.0f;
        if (fast_bsphere_check(scratch.sphere, 64.0f) == -1) {
            moby->state = 1;
            moby->flags &= 0xFFBE;
            moby->collision = moby->pclass->collision;
        }
        break;
    }
    }
    vars->phase = fast_add_rotations(vars->phase, floating_mine_glow_phase_step_gp);
    glow_pulse_sine = fast_sin(vars->phase);
    glow_red_intensity = truncate_float_to_s32(glow_pulse_sine * 48.0f) + 0xCF;
    glow_green_blue_intensity = truncate_float_to_s32(glow_pulse_sine * 32.0f) + 0x30;
    moby->glow_color = glow_green_blue_intensity << 16 |
                       (0xFF000000 | glow_green_blue_intensity << 8) | glow_red_intensity;
    moby->rot.x = fast_add_rotations(moby->rot.x, vars->spin[0]);
    moby->rot.y = fast_add_rotations(moby->rot.y, vars->spin[1]);
    moby->rot.z = fast_add_rotations(moby->rot.z, vars->spin[2]);
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

float        D_L15_001613D8 = 2.0f;
float        D_L15_001613DC = 0.4f;
float        D_L15_001613E0 = 2.0f;
u32          D_L15_001613E4 = 0x207f6060;
f32          D_L15_00161A80 = 0.003f;
f32          D_L15_00161A84 = 0.97f;
f32          D_L15_00161A88 = 0.05f;
float        D_L15_00161A70 = 0.075f;
float        D_L15_00161A74 = 0.2f;
float        D_L15_00161A78 = 0.2f;
float        D_L15_00161A7C = 0.3f;
LevelSpline *l15_splines __asm__("D_L15_0015F70C") = 0;
f32          D_L15_00161AA8 = 0.003f;
f32          D_L15_00161AAC = 0.97f;
f32          D_L15_00161AB0 = 0.05f;
float        D_L15_00161A98 = 0.075f;
float        D_L15_00161A9C = 0.2f;
float        D_L15_00161AA0 = 0.2f;
float        D_L15_00161AA4 = 0.2f;
float        D_L16_00161D9C = 0.1f;
float        D_L16_00161DA0 = 0.1f;
float        D_L16_00161DA4 = -0.5f;
float        D_L16_00161DA8 = 0.6f;
float        D_L16_00161DAC = -0.4f;
float        D_L16_00161DB0 = 0.7f;
int          D_L16_00161DBC = 0x7f204080;

float plasmabot_glow_quad_vertex_offsets[4][4] __asm__("D_L14_001D8890") = {
    {-1.0f,  1.0f, 0.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, 1.0f},
    { 1.0f,  1.0f, 0.0f, 1.0f},
    { 1.0f, -1.0f, 0.0f, 1.0f},
};

int particle_pair_color_gp __asm__("D_L14_00161FE4") = 0x7f204080;
