/* Shared moby class large_amoeboid (and medium_amoeboid, small_amoeboid). */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/globals.h"
#include "rnc/overlay/shared/world/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/game/interpolated_record.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "qcopy.h"
#include "rnc/overlay/shared/world/level_path.h"
#include "sda.h"
#include "rnc/overlay/shared/enemies/large_amoeboid_data/large_amoeboid_update.h"
#include "rnc/overlay/shared/enemies/large_amoeboid_data/spawn_amoeboid_movement_particles_3.h"
#include "rnc/overlay/shared/enemies/large_amoeboid_data/spawn_amoeboid_movement_particles_2.h"
#include "rnc/overlay/shared/enemies/large_amoeboid_data/spawn_amoeboid_movement_particles.h"
#include "rnc/overlay/shared/enemies/large_amoeboid_data/spawn_amoeboid_burst_sparks_2.h"
#include "rnc/overlay/shared/enemies/large_amoeboid_data/spawn_amoeboid_burst_sparks.h"

enum {
    LARGE_AMOEBOID_INITIALIZE = 0,
    LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH = 1,
    LARGE_AMOEBOID_MOVE_TOWARD_STORED_POSITION = 2,
    LARGE_AMOEBOID_WAIT_FOR_PATH_ACQUISITION_ANIMATION_END = 3,
    LARGE_AMOEBOID_FOLLOW_PATH_TO_TARGET = 4,
    LARGE_AMOEBOID_RETURN_TO_STORED_POSITION = 5,
    LARGE_AMOEBOID_LUNGE_AT_TARGET = 6,
    LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESUMING_PATH = 7,
    LARGE_AMOEBOID_PROCESS_KNOCKBACK = 8,
    LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESTARTING_PATH = 9,
    LARGE_AMOEBOID_WAIT_FOR_ACTIVATION_VOLUME = 13,
    LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION = 14,
    LARGE_AMOEBOID_MOVE_DIRECTLY_TOWARD_TARGET = 15,
    LARGE_AMOEBOID_WAIT_WHILE_BLOCKED = 16
};

typedef struct LargeAmoeboidVars {
    u8    pad0[0x20];
    f32   health;
    u8    pad24[0x5];
    s8    unk29;
    u8    pad2A[0x6];
    f32   unk30;
    u8    pad34[0x4];
    s32   unk38;
    u8    pad3C[0x4];
    u8    move_displacement[0x18];
    s8    unk58;
    u8    pad59[0x1];
    s8    unk5A;
    u8    pad5B[0x5];
    u8    motion[0x10];
    f32   knockback_gravity;
    u8    pad74[0xc];
    s32   knockback_collision_radius;
    s32   knockback_motion_flags;
    f32   knockback_collision_z_offset;
    u8    pad8C[0x11];
    s8    unk9D;
    u8    pad9E[0x8a];
    u16   unk128;
    u8    pad12A[0x6];
    void *unk130;
    u8    pad134[0x3c];
    s32   move_control;
    u8    pad174[0x4];
    f32   move_max_drop;
    f32   move_max_climb;
    u8    pad180[0x14];
    f32   move_max_speed;
    u8    pad198[0x28];
    f32   path_search_point[4];
    u8    pad1D0[0x30];
    char *target;
    s32   target_selection_status;
    u8    pad208[0x8];
    f32   home_pos[4]; /* 0x210: position at spawn */
    u8    turn_velocity[0x4];
    f32   base_target_search_radius;
    f32   effective_target_search_radius;
    s32   pursuit_timer;
    s32   starts_active;
    f32   path_heading_offset;
    u8    pad238[0x4];
    f32   move_speed;
    s32   path_ids;
    s32   path_index;
    s32   alternate_target_list_index;
    s32   target_region_path_index;
    f32   size_scale; /* 0x250: 1, 0.7 or 0.49 by amoeboid size */
    f32   glow_cycle_phase;
    s32   removal_height_table_index;
    s32   activation_volume;
    s32   target_search_gate_moby_index;
    u8    pad264[0xc];
    u8    unk270[0x4];
} LargeAmoeboidVars;

typedef int u128_2F0040 __attribute__((mode(TI)));

typedef union {
    u128_2F0040 q;
    float       f[4];
} V_2F0040;

typedef struct {
    char           p0[0x10];
    char           pos[0x10];
    unsigned char  state;
    char           p21[0x13];
    unsigned short flags;
    char           p36[0x70];
    short          type;
    char           pA8[0x58];
} Ent;

extern char camera_pos[] __asm__("D_L01_00167240");
float       absolute_float(float input) __asm__("func_001F99C0");
extern void FUN_0020c828(void *);
extern int  func_L00_001EFFF0_t(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern void add_random_offset_to_vector_xyz(void *, float) __asm__("FUN_L00_0025f8e0");

extern void FUN_L00_00263ac8(float, void *, int, void *);
extern void FUN_L00_0025a120(void *);
extern void process_amoeboid_hits_and_update_targeting(char *) __asm__("FUN_L01_002eec68");
void        spawn_amoeboid_movement_particles(unsigned char *moby) __asm__("FUN_L01_002ef560");

extern void spawn_amoeboid_movement_particles(unsigned char *) __asm__("FUN_L01_002ef560");
void        spawn_amoeboid_burst_sparks(unsigned char *moby, f32 *mode) __asm__("FUN_L01_002ef770");

extern void spawn_amoeboid_burst_sparks(unsigned char *, float *) __asm__("FUN_L01_002ef770");
extern int  follow_path(int *, int, int, float, void *, void *, void *) __asm__("FUN_L01_00276fe8");
extern void FUN_L01_0026d930(void *);
extern void move_moby_with_ground_and_collision_checks(
    void *, void *, void *, void *, float) __asm__("FUN_L00_00258b50");
extern int              path_8808(int, void *, void *, void *) __asm__("FUN_L00_00261b48");
extern int              FUN_L00_002dc9f0(void *, void *);
extern int              FUN_L00_00257470(void *, int, int);
extern int              is_value_within_interpolated_window(float, void *) __asm__("FUN_00214cc8");
extern int              FUN_00214720(void *, int);
extern void             FUN_0022da68(int, int, char *);
extern int              FUN_00213260(int);
extern float            FUN_00213308(void);
extern float            frame_time_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float D_0015ED70 MACRO_ADDR;
extern float            D_0013F420[4];
extern float            D_L01_0015F580[] MACRO_ADDR;

extern char D_L01_00161A40 __attribute__((sda));
extern char D_L01_00161A50 __attribute__((sda));

extern char *activate_smaller_amoeboid(void *) __asm__("FUN_L01_002ef350");
extern char *find_nearest_class_26f_moby_in_list(char *, int) __asm__("FUN_L01_002ef478");

extern float fast_vec_dist(void *, void *);
extern char *D_L01_0015FFD8;
extern int   FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern int   fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern int   resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int);
extern short amoeboid_color_cycle_start_gp __asm__("D_L01_00161998") __attribute__((sda));
extern short amoeboid_color_cycle_end_gp __asm__("D_L01_0016199C") __attribute__((sda));
extern void  trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void
trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern float       D_0013F420[];
extern int         D_001413D0[];
extern const float l01_smaller_amoeboid_knockback_xy_scale __asm__("D_L01_001619A0")
    __attribute__((sda));
extern const float l01_smaller_amoeboid_knockback_z_scale __asm__("D_L01_001619A4")
    __attribute__((sda));
extern const float frame_time_macro __asm__("D_0015ED6C");

extern short *l01_moby_index_lists_by_group[] __asm__("D_L01_001ABCC0");
extern void   update_entity(void *) __asm__("FUN_L00_00250df8");

char *activate_smaller_amoeboid(void *source_moby) __asm__("FUN_L01_002ef350");

extern int amoeboid_first_movement_particle_spawn_gate_modulus_plus_one_gp __asm__("D_L01_001619E0")
    __attribute__((sda));
extern int
    amoeboid_second_movement_particle_spawn_gate_modulus_plus_one_gp __asm__("D_L01_001619E4")
        __attribute__((sda));
extern float amoeboid_movement_particle_max_spawn_radius_gp __asm__("D_L01_001619E8")
    __attribute__((sda));
extern float amoeboid_first_movement_particle_base_scale_max_gp __asm__("D_L01_001619B8")
    __attribute__((sda));

extern int amoeboid_first_movement_particle_lifetime_min_gp __asm__("D_L01_001619C0")
    __attribute__((sda));
extern int amoeboid_first_movement_particle_lifetime_max_gp __asm__("D_L01_001619C4")
    __attribute__((sda));

extern float amoeboid_second_movement_particle_base_scale_max_gp __asm__("D_L01_001619D0")
    __attribute__((sda));

extern int amoeboid_second_movement_particle_lifetime_min_gp __asm__("D_L01_001619D8")
    __attribute__((sda));
extern int amoeboid_second_movement_particle_lifetime_max_gp __asm__("D_L01_001619DC")
    __attribute__((sda));
extern void
spawn_type_52_particle(float *, float, float, int, int, int) __asm__("FUN_L01_00287158");

extern s32 amoeboid_burst_group_count_gp __asm__("D_L01_001619EC") __attribute__((sda));
extern s32 amoeboid_burst_sparks_per_group_gp __asm__("D_L01_001619F0") __attribute__((sda));
extern f32 amoeboid_burst_group_position_jitter_bound_gp __asm__("D_L01_001619F4")
    __attribute__((sda));
extern f32 amoeboid_burst_spark_position_jitter_bound_gp __asm__("D_L01_001619F8")
    __attribute__((sda));
extern f32 amoeboid_burst_spark_velocity_jitter_bound_gp __asm__("D_L01_001619FC")
    __attribute__((sda));
extern f32 amoeboid_burst_radial_speed_min_gp __asm__("D_L01_00161A00") __attribute__((sda));
extern f32 amoeboid_burst_radial_speed_max_gp __asm__("D_L01_00161A04") __attribute__((sda));
extern f32 amoeboid_burst_vertical_speed_min_gp __asm__("D_L01_00161A08") __attribute__((sda));
extern f32 amoeboid_burst_vertical_speed_max_gp __asm__("D_L01_00161A0C") __attribute__((sda));
extern f32 amoeboid_burst_downward_velocity_adjustment_factor_gp __asm__("D_L01_00161A10")
    __attribute__((sda));
extern f32 D_L01_00161A14 __attribute__((sda));
extern f32 D_L01_00161A18 __attribute__((sda));

extern s32 amoeboid_burst_spark_header_frame_count_gp __asm__("D_L01_00161A2C")
    __attribute__((sda));

extern s32 amoeboid_burst_spark_duration_base_gp __asm__("D_L01_00161A34") __attribute__((sda));
extern f32 amoeboid_burst_input_motion_scale_max_gp __asm__("D_L01_00161A38") __attribute__((sda));

extern f32  D_0015ED70;
extern void add_random_offset_to_vector_xyz(void *, f32) __asm__("FUN_L00_0025f8e0");
extern f32  FUN_00220e20(f32) __asm__("FUN_001f96b0");

extern int process_moby_suck_cannon_suction(void *) __asm__("FUN_L00_002db8f8");

void large_amoeboid_update(unsigned char *moby) __asm__("FUN_L01_002edca0");

/* Moby classes 572, 865, 866 */
void large_amoeboid_update(unsigned char *moby) {
    enum {
        LARGE_AMOEBOID_LARGE_VARIANT = 0x23c,
        LARGE_AMOEBOID_MEDIUM_VARIANT = 0x361,
        LARGE_AMOEBOID_SMALL_VARIANT = 0x362
    };
    enum {
        LARGE_AMOEBOID_DORMANT = 12
    };
    LargeAmoeboidVars *large_amoeboid_vars = (LargeAmoeboidVars *)MOBY(moby)->pvars;
    char              *target;
    char              *target_position;
    float              move[4];
    float              hit[4];
    float              angle;
    float              distance;
    int                path_hit;
    int                blocked;
    int                result;

    if (MOBY(moby)->state == LARGE_AMOEBOID_DORMANT) {
        MOBY(moby)->flags |= 1;
        return;
    }
    FUN_L00_00263ac8(2.9f, moby, 0, &large_amoeboid_vars->unk270);
    if (MOBY(moby)->visible != 0 && fast_vec_dist(&MOBY(moby)->pos, camera_pos) < 28.0f) {
        FUN_L00_0025a120(moby);
        MOBY(moby)->shadow_range = 0x16;
    }
    process_amoeboid_hits_and_update_targeting((char *)moby);
    target = large_amoeboid_vars->target;
    switch (MOBY(moby)->state) {
    case LARGE_AMOEBOID_INITIALIZE:
        qcopy(&large_amoeboid_vars->home_pos, &MOBY(moby)->pos);
        switch (MOBY(moby)->oclass) {
        case LARGE_AMOEBOID_LARGE_VARIANT:
            large_amoeboid_vars->unk58 = truncate_float_to_s32(14.0f);
            large_amoeboid_vars->unk5A = truncate_float_to_s32(10.4f);
            large_amoeboid_vars->size_scale = 1.0f;
            break;
        case LARGE_AMOEBOID_MEDIUM_VARIANT:
            large_amoeboid_vars->unk58 = truncate_float_to_s32(14.0f);
            large_amoeboid_vars->unk5A = truncate_float_to_s32(5.6f);
            large_amoeboid_vars->size_scale = 0.7f;
            break;
        case LARGE_AMOEBOID_SMALL_VARIANT:
            large_amoeboid_vars->unk58 = truncate_float_to_s32(14.0f);
            large_amoeboid_vars->unk5A = truncate_float_to_s32(4.0f);
            large_amoeboid_vars->size_scale = 0.49f;
            break;
        }
        large_amoeboid_vars->unk30 = large_amoeboid_vars->size_scale;
        large_amoeboid_vars->glow_cycle_phase = FUN_00213308();
        if (MOBY(moby)->oclass == 0x362)
            large_amoeboid_vars->unk130 = &D_L01_00161A50;
        else
            large_amoeboid_vars->unk130 = &D_L01_00161A40;
        large_amoeboid_vars->unk29 = 1;
        if (large_amoeboid_vars->removal_height_table_index != 0) {
            large_amoeboid_vars->health = 1.0f;
            large_amoeboid_vars->starts_active = 1;
        } else {
            large_amoeboid_vars->health = 3.0f;
        }
        if (large_amoeboid_vars->base_target_search_radius == 0.0f)
            large_amoeboid_vars->base_target_search_radius = 15.0f;
        FUN_L01_0026d930(&large_amoeboid_vars->move_control);
        large_amoeboid_vars->move_max_climb = 2.0f;
        large_amoeboid_vars->move_max_drop = 2.0f;
        result = truncate_float_to_s32(large_amoeboid_vars->size_scale * 0.8f * 1024.0f);
        angle = large_amoeboid_vars->move_speed * frame_time_gp;
        large_amoeboid_vars->move_control = result;
        large_amoeboid_vars->move_max_speed = angle;
        if (large_amoeboid_vars->starts_active != 0) {
            MOBY(moby)->state = LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH;
            MOBY(moby)->flags |= 0x1000;
        } else {
            if (large_amoeboid_vars->activation_volume == -1)
                MOBY(moby)->state = LARGE_AMOEBOID_DORMANT;
            else
                MOBY(moby)->state = LARGE_AMOEBOID_WAIT_FOR_ACTIVATION_VOLUME;
            MOBY(moby)->collision = 0;
            *(int *)(moby + 0x58) = 0;
            MOBY(moby)->visible = 0;
            distance = MOBY(moby)->pos.z - 20.0f;
            MOBY(moby)->flags = (MOBY(moby)->flags & 0xefff) | 1;
            MOBY(moby)->pos.z = distance;
        }
        break;
    case LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH:
        if (large_amoeboid_vars->target_selection_status != 2) {
            if (FUN_00213260(4) == 0) {
                path_hit = follow_path((int *)(&large_amoeboid_vars->path_ids),
                                       1,
                                       large_amoeboid_vars->path_index,
                                       large_amoeboid_vars->size_scale * 0.8f,
                                       &MOBY(moby)->pos,
                                       target + 0x10,
                                       move);
                if (path_hit != 0) {
                    MOBY(moby)->state = LARGE_AMOEBOID_FOLLOW_PATH_TO_TARGET;
                    if (MOBY(moby)->prev_seq != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                    clear_u64_value(&large_amoeboid_vars->move_displacement);
                    large_amoeboid_vars->path_heading_offset =
                        random_float_between(-0.34906584f, 0.34906584f);
                } else {
                    large_amoeboid_vars->target_selection_status = 2;
                }
            }
        }
        if (large_amoeboid_vars->pursuit_timer != 0 &&
            large_amoeboid_vars->target_selection_status == 2) {
            MOBY(moby)->state = LARGE_AMOEBOID_MOVE_DIRECTLY_TOWARD_TARGET;
            if (MOBY(moby)->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, 5);
            return;
        }
        large_amoeboid_vars->move_max_speed =
            large_amoeboid_vars->move_speed * 0.5f * frame_time_gp;
        if (MOBY(moby)->prev_seq != 1)
            blend_moby_animation(moby, 1, 0, 5);
        MOBY(moby)->state = LARGE_AMOEBOID_MOVE_TOWARD_STORED_POSITION;
        break;
    case LARGE_AMOEBOID_MOVE_TOWARD_STORED_POSITION:
        angle = fast_arc_tan(large_amoeboid_vars->home_pos[0] - MOBY(moby)->pos.x,
                             large_amoeboid_vars->home_pos[1] - MOBY(moby)->pos.y);
        approach_angle((float *)&MOBY(moby)->rot.z,
                       (float *)(&large_amoeboid_vars->turn_velocity),
                       angle,
                       D_0015ED70 * 1.0471976f,
                       D_0015ED70 * 1.0471976f,
                       frame_time_gp * 3.1415927f);
        move[0] = fast_cos(MOBY(moby)->rot.z);
        move[1] = fast_sin(MOBY(moby)->rot.z);
        move[2] = 0.0f;
        move_moby_with_ground_and_collision_checks(moby,
                                                   &large_amoeboid_vars->move_control,
                                                   move,
                                                   &large_amoeboid_vars->move_displacement,
                                                   1.0f);
        result = large_amoeboid_vars->target_selection_status;
        if (result != 2 && FUN_00213260(4) == 0) {
            path_hit = follow_path((int *)(&large_amoeboid_vars->path_ids),
                                   1,
                                   large_amoeboid_vars->path_index,
                                   large_amoeboid_vars->size_scale * 0.8f,
                                   &MOBY(moby)->pos,
                                   target + 0x10,
                                   hit);
            if (path_hit != 0) {
                MOBY(moby)->state = LARGE_AMOEBOID_WAIT_FOR_PATH_ACQUISITION_ANIMATION_END;
                if (MOBY(moby)->prev_seq != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                clear_u64_value(&large_amoeboid_vars->move_displacement);
                angle = random_float_between(-0.34906584f, 0.34906584f);
                distance = large_amoeboid_vars->move_speed * frame_time_gp;
                large_amoeboid_vars->path_heading_offset = angle;
                large_amoeboid_vars->move_max_speed = distance;
            } else {
                large_amoeboid_vars->target_selection_status = 2;
            }
        }
        if (large_amoeboid_vars->pursuit_timer != 0 &&
            large_amoeboid_vars->target_selection_status == 2) {
            MOBY(moby)->state = LARGE_AMOEBOID_MOVE_DIRECTLY_TOWARD_TARGET;
            if (MOBY(moby)->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, 5);
        }
        spawn_amoeboid_movement_particles(moby);
        return;
    case LARGE_AMOEBOID_MOVE_DIRECTLY_TOWARD_TARGET:
        blocked = 0;
        angle = fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(target + 0x14) - MOBY(moby)->pos.y);
        approach_angle((float *)&MOBY(moby)->rot.z,
                       (float *)(&large_amoeboid_vars->turn_velocity),
                       angle,
                       D_0015ED70 * 1.0471976f,
                       D_0015ED70 * 1.0471976f,
                       frame_time_gp * 3.1415927f);
        move[0] = fast_cos(MOBY(moby)->rot.z);
        move[1] = fast_sin(MOBY(moby)->rot.z);
        move[2] = 0.0f;
        move_moby_with_ground_and_collision_checks(moby,
                                                   &large_amoeboid_vars->move_control,
                                                   move,
                                                   &large_amoeboid_vars->move_displacement,
                                                   1.0f);
        if (path_8808(large_amoeboid_vars->path_ids, &MOBY(moby)->pos, target + 0x10, hit) != 0 &&
            fast_vec_dist(&MOBY(moby)->pos, hit) < 1.5f)
            blocked = 1;
        if (large_amoeboid_vars->pursuit_timer == 0) {
            MOBY(moby)->state = LARGE_AMOEBOID_RETURN_TO_STORED_POSITION;
        } else if (blocked) {
            MOBY(moby)->state = LARGE_AMOEBOID_WAIT_WHILE_BLOCKED;
            if (MOBY(moby)->prev_seq != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (vector_distance_xy(&MOBY(moby)->pos, target + 0x10) < 2.0f) {
            MOBY(moby)->state = LARGE_AMOEBOID_LUNGE_AT_TARGET;
            if (MOBY(moby)->prev_seq != 2)
                blend_moby_animation(moby, 2, 5, scale_game_frames(10));
            return;
        }
        break;
    case LARGE_AMOEBOID_WAIT_WHILE_BLOCKED:
        if (!(MOBY(moby)->anim_flags & 2) || large_amoeboid_vars->pursuit_timer != 0)
            return;
        MOBY(moby)->state = LARGE_AMOEBOID_RETURN_TO_STORED_POSITION;
        if (MOBY(moby)->prev_seq != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(20));
        return;
    case LARGE_AMOEBOID_FOLLOW_PATH_TO_TARGET: {
        char         *target_position;
        unsigned char path_hit;
        target_position = target + 0x10;
        distance = vector_distance_xy(&MOBY(moby)->pos, target_position);
        if (distance < large_amoeboid_path_lunge_distance_threshold_gp) {
            MOBY(moby)->state = LARGE_AMOEBOID_LUNGE_AT_TARGET;
            large_amoeboid_vars->move_max_speed =
                (large_amoeboid_vars->move_speed + large_amoeboid_vars->move_speed) * frame_time_gp;
            if (MOBY(moby)->prev_seq != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            return;
        } else {
            if (follow_path((int *)(&large_amoeboid_vars->path_ids),
                            1,
                            large_amoeboid_vars->path_index,
                            large_amoeboid_vars->size_scale * 0.8f,
                            &MOBY(moby)->pos,
                            target_position,
                            hit) != 0)
                path_hit = 1;
            else
                path_hit = follow_path((int *)(&large_amoeboid_vars->path_ids),
                                       1,
                                       large_amoeboid_vars->path_index,
                                       0.0f,
                                       &MOBY(moby)->pos,
                                       target_position,
                                       hit) != 0;
            if (path_hit != 0) {
                angle = fast_arc_tan(hit[0] - MOBY(moby)->pos.x, hit[1] - MOBY(moby)->pos.y);
                angle = fast_add_rotations(angle, large_amoeboid_vars->path_heading_offset);
                approach_angle((float *)&MOBY(moby)->rot.z,
                               (float *)(&large_amoeboid_vars->turn_velocity),
                               angle,
                               D_0015ED70 * 12.566371f,
                               D_0015ED70 * 12.566371f,
                               frame_time_gp * 25.132742f);
                move[0] = fast_cos(MOBY(moby)->rot.z);
                move[0] += move[0];
                move[1] = fast_sin(MOBY(moby)->rot.z);
                move[1] += move[1];
                move[2] = 0.0f;
                move_moby_with_ground_and_collision_checks(moby,
                                                           &large_amoeboid_vars->move_control,
                                                           move,
                                                           &large_amoeboid_vars->move_displacement,
                                                           1.0f);
            } else {
                large_amoeboid_vars->target_selection_status = 2;
            }
            if (large_amoeboid_vars->target_selection_status == 2)
                MOBY(moby)->state = LARGE_AMOEBOID_RETURN_TO_STORED_POSITION;
            spawn_amoeboid_movement_particles(moby);
            return;
        }
    }
    case LARGE_AMOEBOID_LUNGE_AT_TARGET: {
        char  *target_position;
        float *rotation = (float *)&MOBY(moby)->rot.z;
        angle = fast_arc_tan(*(float *)(target + 0x10) - MOBY(moby)->pos.x,
                             *(float *)(target + 0x14) - MOBY(moby)->pos.y);
        approach_angle(rotation,
                       (float *)(&large_amoeboid_vars->turn_velocity),
                       angle,
                       D_0015ED70 * 12.566371f,
                       D_0015ED70 * 12.566371f,
                       frame_time_gp * 25.132742f);
        distance = compute_interpolated_record_value(moby);
        if (MOBY(moby)->seq == MOBY(moby)->prev_seq && 10.0f <= distance && distance <= 16.0f) {
            move[0] = fast_cos(*rotation);
            move[0] += move[0];
            move[1] = fast_sin(*rotation);
            move[1] += move[1];
            move[2] = 0.0f;
            move_moby_with_ground_and_collision_checks(moby,
                                                       &large_amoeboid_vars->move_control,
                                                       move,
                                                       &large_amoeboid_vars->move_displacement,
                                                       1.0f);
        }
        if (is_value_within_interpolated_window(15.5f, moby) != 0 &&
            vector_distance_xy(&MOBY(moby)->pos, (target_position = target + 0x10)) < 2.0f &&
            absolute_float(*(float *)(target + 0x18) - MOBY(moby)->pos.z) <
                large_amoeboid_vars->size_scale + large_amoeboid_vars->size_scale) {
            move[0] = fast_cos(MOBY(moby)->rot.z);
            move[1] = fast_sin(MOBY(moby)->rot.z);
            move[2] = 0.0f;
            move[3] = 5627.9248f;
            qcopy(hit, target_position);
            hit[2] += 0.75f;
            spawn_nearby_effect(target, moby, 0x10001, hit, move, 1.0f);
            return;
        }
        if (MOBY(moby)->anim_flags & 2) {
            MOBY(moby)->state = LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESUMING_PATH;
            if (MOBY(moby)->prev_seq != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            large_amoeboid_vars->move_max_speed = large_amoeboid_vars->move_speed * frame_time_gp;
        }
        break;
    }
    case LARGE_AMOEBOID_WAIT_FOR_PATH_ACQUISITION_ANIMATION_END:
    case LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESUMING_PATH:
        if (!(MOBY(moby)->anim_flags & 2))
            return;
        MOBY(moby)->state = LARGE_AMOEBOID_FOLLOW_PATH_TO_TARGET;
        if (MOBY(moby)->prev_seq != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        return;
    case LARGE_AMOEBOID_RETURN_TO_STORED_POSITION: {
        int   blocked = 0;
        char *target_position;
        distance = vector_distance_xy(&MOBY(moby)->pos,
                                      (target_position = &large_amoeboid_vars->home_pos));
        if (distance < 1.0f) {
            MOBY(moby)->state = LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH;
        } else {
            if (follow_path((int *)(&large_amoeboid_vars->path_ids),
                            1,
                            large_amoeboid_vars->path_index,
                            large_amoeboid_vars->size_scale * 0.8f,
                            &MOBY(moby)->pos,
                            target_position,
                            hit) != 0)
                blocked = 1;
            else
                blocked = follow_path((int *)(&large_amoeboid_vars->path_ids),
                                      1,
                                      large_amoeboid_vars->path_index,
                                      0.0f,
                                      target_position,
                                      target_position,
                                      hit) != 0;
        }
        if (blocked != 0) {
            angle = fast_arc_tan(hit[0] - MOBY(moby)->pos.x, hit[1] - MOBY(moby)->pos.y);
            angle = fast_add_rotations(angle, large_amoeboid_vars->path_heading_offset);
            approach_angle((float *)&MOBY(moby)->rot.z,
                           (float *)(&large_amoeboid_vars->turn_velocity),
                           angle,
                           D_0015ED70 * 12.566371f,
                           D_0015ED70 * 12.566371f,
                           frame_time_gp * 25.132742f);
            move[0] = fast_cos(MOBY(moby)->rot.z);
            move[0] += move[0];
            move[1] = fast_sin(MOBY(moby)->rot.z);
            move[1] += move[1];
            move[2] = 0.0f;
            move_moby_with_ground_and_collision_checks(moby,
                                                       &large_amoeboid_vars->move_control,
                                                       move,
                                                       &large_amoeboid_vars->move_displacement,
                                                       1.0f);
        }
        if (large_amoeboid_vars->target_selection_status != 2 &&
            (MOBY(moby)->state = 4, MOBY(moby)->prev_seq != 1))
            blend_moby_animation(moby, 1, 0, scale_game_frames(3));
        spawn_amoeboid_movement_particles(moby);
        return;
    }
    case LARGE_AMOEBOID_PROCESS_KNOCKBACK:
        result = step_moby_motion(moby, &large_amoeboid_vars->motion);
        if (result & 1) {
            path_hit = follow_path((int *)(&large_amoeboid_vars->path_ids),
                                   1,
                                   large_amoeboid_vars->path_index,
                                   large_amoeboid_vars->size_scale * 0.8f,
                                   &MOBY(moby)->pos,
                                   &large_amoeboid_vars->home_pos,
                                   move);
            if (path_hit != 0) {
                FUN_0022da68(4, 0, (char *)moby);
                large_amoeboid_vars->move_max_speed =
                    large_amoeboid_vars->move_speed * frame_time_gp;
                MOBY(moby)->state = LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESTARTING_PATH;
                if (MOBY(moby)->prev_seq != 5)
                    blend_moby_animation(moby, 5, 4, scale_game_frames(10));
                qcopy(&large_amoeboid_vars->home_pos, &MOBY(moby)->pos);
                return;
            }
            spawn_amoeboid_burst_sparks(moby, D_L01_0015F580);
            FUN_L00_00257470(moby, 0, -1);
        } else if (!(result & 0x120)) {
            return;
        }
        FUN_0020c828(moby);
        return;
    case LARGE_AMOEBOID_WAIT_FOR_ANIMATION_BEFORE_RESTARTING_PATH:
        if (MOBY(moby)->anim_flags & 2) {
            MOBY(moby)->state = LARGE_AMOEBOID_FOLLOW_PATH_TO_TARGET;
            if (MOBY(moby)->prev_seq != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            clear_u64_value(&large_amoeboid_vars->move_displacement);
            angle = random_float_between(-0.34906584f, 0.34906584f);
            distance = large_amoeboid_vars->move_speed * frame_time_gp;
            large_amoeboid_vars->path_heading_offset = angle;
            large_amoeboid_vars->move_max_speed = distance;
        }
        break;
    case LARGE_AMOEBOID_WAIT_FOR_ACTIVATION_VOLUME:
        MOBY(moby)->flags |= 1;
        if (FUN_00214720(D_0013F420, large_amoeboid_vars->activation_volume) != 0) {
            u16   flags;
            float distance;
            int   result;
            distance = MOBY(moby)->pos.z + 20.0f;
            flags = MOBY(moby)->flags | 0x1000;
            MOBY(moby)->state = LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH;
            MOBY(moby)->flags = flags;
            MOBY(moby)->anim_speed = 1.0f;
            MOBY(moby)->pos.z = distance;
            result = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            MOBY(moby)->collision = result;
            MOBY(moby)->flags = flags & 0xfffe;
            MOBY(moby)->visible = 1;
        }
        return;
    case LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION:
        if (FUN_L00_002dc9f0(moby, &large_amoeboid_vars->motion)) {
            MOBY(moby)->state = LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH;
            large_amoeboid_vars->unk128 = 0;
        }
        break;
    }
}

/* Moby update: hit test against a target, hurt/debris on a hit, else path search and bob. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F0040), where it is exact; names translated to the US level program. */

void process_amoeboid_hits_and_update_targeting(char *moby) {
    V_2F0040           scratch_vector;
    V_2F0040           line_test_endpoint;
    int                flag;
    float              amount;
    float              first_spawn_angle;
    float              second_spawn_angle;
    LargeAmoeboidVars *moby_vars;
    char              *hit_record;
    char              *hit_record_moby;
    char              *x;
    char              *search_path;
    char              *spawned;
    char              *q;
    char              *cand;
    u128_2F0040       *wp;
    int                candidate_list_index;
    char              *hit_record_moby_for_class_check;
    char              *x2;
    char              *path_search_position;
    int                removal_height_entry_index;
    int                path_search_gate_moby_index;
    float              size_multiplier;
    float              rate;
    float              fixed_scale;
    float              hit_decrement;
    float              candidate_distance;
    float              color_cycle_phase;
    float              k;
    float              seven;

    amount = 0.0f;
    moby_vars = (LargeAmoeboidVars *)MOBY(moby)->pvars;
    hit_record = find_moby_hit(moby, 0x330000, 0);
    if (hit_record != 0) {
        qcopy(&scratch_vector, &MOBY(moby)->pos);
        scratch_vector.f[2] = scratch_vector.f[2] + 0.5f;
        hit_record_moby = *(char **)(hit_record + 0x20);
        if (hit_record_moby != 0) {
            short cls = MOBY(hit_record_moby)->oclass;
            if (cls == 0x47) {
                qcopy(&line_test_endpoint, ((char *)D_0013F420));
                wp = &line_test_endpoint.q;
            } else {
                x = ((char *)MOBY(hit_record_moby)->pclass);
                if ((x != 0 && *(short *)(x + 0x46) == 5) || cls == 0xBA) {
                    qcopy(&line_test_endpoint, &scratch_vector);
                    wp = &line_test_endpoint.q;
                } else {
                    qcopy(&line_test_endpoint, &MOBY(hit_record_moby)->pos);
                    wp = &line_test_endpoint.q;
                }
            }
        } else {
            qcopy(&line_test_endpoint, hit_record);
            wp = &line_test_endpoint.q;
        }
        if (fast_vec_dist(&scratch_vector, wp) < 24.0f) {
            if (func_L00_001EFFF0_t(&scratch_vector, wp, 2, moby, 0) != 0) {
                hit_record = 0;
            }
        }
    }
    resolve_moby_hit_response(moby, hit_record, &moby_vars->health, 0, &flag, &amount, 0, 4);
    if (flag != 1 && amount != 0.0f && MOBY(moby)->state != 0xB && MOBY(moby)->state != 0xA &&
        MOBY(moby)->state != LARGE_AMOEBOID_PROCESS_KNOCKBACK) {
        size_multiplier = 0.8f;
        allocate_voice_for_target_entry(0, 0, moby);
        hit_decrement = 1.0f;
        fixed_scale = 1024.0f;
        rate = 0.008f;
        moby_vars->health = moby_vars->health - hit_decrement;
        moby_vars->unk9D = 0;
        moby_vars->knockback_gravity = rate;
        moby_vars->knockback_motion_flags = 8;
        moby_vars->knockback_collision_radius =
            truncate_float_to_s32(moby_vars->size_scale * size_multiplier * fixed_scale);
        moby_vars->knockback_collision_z_offset = moby_vars->size_scale * size_multiplier;
        spawn_amoeboid_burst_sparks((unsigned char *)moby, (float *)(hit_record + 0x10));
        if (0.0f < moby_vars->health) {
            spawned = activate_smaller_amoeboid(moby);
            if (spawned != 0) {
                q = *(char **)(spawned + 0x78);
                q[0x9D] = 3;
                *(float *)(q + 0x70) = rate;
                *(int *)(q + 0x84) = 8;
                *(int *)(q + 0x80) =
                    truncate_float_to_s32(moby_vars->size_scale * size_multiplier * fixed_scale);
                k = frame_time_macro;
                seven = 7.0f;
                *(float *)(q + 0x88) = moby_vars->size_scale * size_multiplier;
                *(float *)(q + 0x78) = l01_smaller_amoeboid_knockback_xy_scale * k;
                *(float *)(q + 0x7C) = l01_smaller_amoeboid_knockback_z_scale * k;
                *(int *)(q + 0x84) = *(int *)(q + 0x84) | 4;
                *(float *)(q + 0xB0) = seven;
                *(float *)(q + 0xB4) = 13.0f;
                scratch_vector.q = *(u128_2F0040 *)(hit_record + 0x10);
                trooper_knockback_arc(&scratch_vector, &first_spawn_angle, q + 0x78, q + 0x7C);
                first_spawn_angle = fast_subtract_rotations(first_spawn_angle, 0.7853982f);
                trooper_start_knockback(spawned, q + 0x60, first_spawn_angle, 4, 8, 0);
                spawned[0x20] = 8;
                *(float *)(spawned + 0x58) = hit_decrement;
            }
            spawned = activate_smaller_amoeboid(moby);
            if (spawned != 0) {
                q = *(char **)(spawned + 0x78);
                *(float *)(q + 0x70) = rate;
                q[0x9D] = 3;
                *(int *)(q + 0x84) = 8;
                *(int *)(q + 0x80) =
                    truncate_float_to_s32(moby_vars->size_scale * size_multiplier * fixed_scale);
                k = frame_time_macro;
                seven = 7.0f;
                *(float *)(q + 0x88) = moby_vars->size_scale * size_multiplier;
                *(float *)(q + 0x78) = l01_smaller_amoeboid_knockback_xy_scale * k;
                *(float *)(q + 0x7C) = l01_smaller_amoeboid_knockback_z_scale * k;
                *(int *)(q + 0x84) = *(int *)(q + 0x84) | 4;
                *(float *)(q + 0xB0) = seven;
                *(float *)(q + 0xB4) = 13.0f;
                scratch_vector.q = *(u128_2F0040 *)(hit_record + 0x10);
                trooper_knockback_arc(&scratch_vector, &second_spawn_angle, q + 0x78, q + 0x7C);
                second_spawn_angle = fast_add_rotations(second_spawn_angle, 0.7853982f);
                trooper_start_knockback(spawned, q + 0x60, second_spawn_angle, 4, 8, 0);
                spawned[0x20] = 8;
                *(float *)(spawned + 0x58) = hit_decrement;
            }
            hit_record_moby_for_class_check = *(char **)(hit_record + 0x20);
            if (hit_record_moby_for_class_check != 0 &&
                (x2 = *(char **)(hit_record_moby_for_class_check + 0x24)) != 0 &&
                *(short *)(x2 + 0x46) == 5) {
                FUN_L00_00257470(moby, 0x800, -1);
            } else {
                FUN_L00_00257470(moby, 0, -1);
            }
            mark_moby_for_removal(moby);
            return;
        }
        MOBY(moby)->flags = MOBY(moby)->flags & 0xEFFF;
        mark_moby_for_removal(moby);
        return;
    }
    MOBY(moby)->hit_slot = 0xFF;
    removal_height_entry_index = moby_vars->removal_height_table_index;
    if (removal_height_entry_index != 0 && MOBY(moby)->state != LARGE_AMOEBOID_INITIALIZE &&
        MOBY(moby)->pos.z < *(float *)(removal_height_entry_index * 0x1190 + pool_draw_tiles + 8)) {
        clear_u64_value(&scratch_vector);
        spawn_amoeboid_burst_sparks((unsigned char *)moby, (float *)&scratch_vector);
        mark_moby_for_removal(moby);
        return;
    }
    if (moby_vars->unk38 != 0) {
        moby_vars->pursuit_timer = scale_game_frames(0xF0);
        moby_vars->unk38 = 0;
    }
    if (fast_dec_timer(&moby_vars->pursuit_timer) != 0) {
        moby_vars->effective_target_search_radius = moby_vars->base_target_search_radius;
    } else {
        moby_vars->effective_target_search_radius = moby_vars->base_target_search_radius + 20.0f;
    }
    path_search_gate_moby_index = moby_vars->target_search_gate_moby_index;
    if (path_search_gate_moby_index <= 0 ||
        *(unsigned char *)((path_search_gate_moby_index << 8) + (*(int *)&D_L01_0015FFD8) + 0xBC) !=
            0) {
        search_path = ((char *)l01_paths[moby_vars->target_region_path_index]);
        path_search_position = &moby_vars->path_search_point;
        if (FUN_L00_0025ff38(moby_vars->effective_target_search_radius,
                             moby,
                             path_search_position,
                             0,
                             0,
                             search_path + 0x10,
                             *(int *)search_path) != 2) {
            if (moby_vars->effective_target_search_radius <
                    vector_distance_xy(&moby_vars->home_pos, path_search_position) ||
                3.0f < absolute_float(MOBY(moby)->pos.z - moby_vars->path_search_point[2])) {
                moby_vars->target_selection_status = 2;
            }
        }
    }
    candidate_list_index = moby_vars->alternate_target_list_index;
    if (candidate_list_index >= 0) {
        cand = find_nearest_class_26f_moby_in_list(moby, candidate_list_index);
        if (cand != 0) {
            candidate_distance = vector_distance_xy(&MOBY(moby)->pos, cand + 0x10);
            if (candidate_distance < moby_vars->effective_target_search_radius) {
                if (moby_vars->target_selection_status == 2 ||
                    candidate_distance <
                        vector_distance_xy(&MOBY(moby)->pos, moby_vars->target + 0x10)) {
                    moby_vars->target = cand;
                    moby_vars->target_selection_status = 1;
                }
            }
        }
    }
    if (*(int *)&moby_vars->target == 0) {
        *(int *)&moby_vars->target = *(int *)(((char *)((char(*))D_001413D0)));
    }
    color_cycle_phase = fast_add_rotations(moby_vars->glow_cycle_phase, frame_time * 9.424778f);
    moby_vars->glow_cycle_phase = color_cycle_phase;
    MOBY(moby)->glow_color = fast_tween_color((fast_sin(color_cycle_phase) + 1.0f) * 0.5f,
                                              *(int *)&amoeboid_color_cycle_start_gp,
                                              *(int *)&amoeboid_color_cycle_end_gp);
}

char *activate_smaller_amoeboid(void *source_moby) {
    enum {
        LARGE_AMOEBOID_DORMANT = 12
    };
    struct Moby *source_amoeboid = source_moby;
    short       *list;
    char        *entities;
    int          group_index_or_list_entry;
    int          next_class = -1;
    if (source_amoeboid->oclass == 0x23C) {
        next_class = 0x361;
    } else if (source_amoeboid->oclass == 0x361) {
        next_class = 0x362;
    }
    group_index_or_list_entry = *(volatile u8 *)&source_amoeboid->group;
    entities = (char *)(l01_moby_index_lists_by_group + group_index_or_list_entry);
    /* Fakematch: the dead touch of the entity base gives retail's register for the list slot. */
    D_L01_0015FFD8++;
    D_L01_0015FFD8--;
    list = *(short **)entities;
    entities = D_L01_0015FFD8;
    if (list == 0)
        return 0;
    goto scan;
    do {
        struct Moby *smaller_amoeboid;
        int          index;
    advance:
        if ((short)group_index_or_list_entry < 0)
            return 0;
        list++;
    scan:
        index = *(unsigned short *)list;
        group_index_or_list_entry = index;
        index &= 0x7FFF;
        smaller_amoeboid = (struct Moby *)(entities + (index << 8));
        if (smaller_amoeboid->state != LARGE_AMOEBOID_DORMANT ||
            smaller_amoeboid->oclass != next_class)
            goto advance;
        {
            /* Retail reads both pvars pointers and update_dist before writing the entity. */
            LargeAmoeboidVars *other_vars =
                (LargeAmoeboidVars *)(*(u8 *volatile *)&smaller_amoeboid->pvars);
            u8                 update_dist = *(volatile u8 *)&source_amoeboid->update_dist;
            LargeAmoeboidVars *self_vars = (LargeAmoeboidVars *)source_amoeboid->pvars;
            smaller_amoeboid->update_dist = update_dist;
            smaller_amoeboid->draw_dist = source_amoeboid->draw_dist;
            {
                u64 spawn_frame = source_amoeboid->spawn_frame;
                smaller_amoeboid->visible = 1;
                smaller_amoeboid->spawn_frame = spawn_frame;
            }
            smaller_amoeboid->collision = smaller_amoeboid->pclass->collision;
            smaller_amoeboid->flags = source_amoeboid->flags;
            qcopy(&smaller_amoeboid->pos, &source_amoeboid->pos);
            qcopy(&smaller_amoeboid->rot, &source_amoeboid->rot);
            other_vars->health = self_vars->health;
            other_vars->base_target_search_radius = self_vars->base_target_search_radius;
            update_entity(smaller_amoeboid);
            return (char *)smaller_amoeboid;
        }
    } while (1);
    return 0;
}

/* Nearest live entity of type 0x26F in list idx within 120 units of self, or 0. */

char *find_nearest_class_26f_moby_in_list(char *reference_moby, int candidate_list_index) {
    short *list = l01_moby_index_lists_by_group[candidate_list_index];
    float  best = 120.0f;
    char  *nearest_entity = 0;
    if (list == 0) {
        return 0;
    }
    while (1) {
        int i = *(unsigned short *)list & 0x7FFF;
        if ((*(Ent **)&D_L01_0015FFD8)[i].flags & 0x1000) {
            if ((*(Ent **)&D_L01_0015FFD8)[i].type == 0x26F) {
                if ((*(Ent **)&D_L01_0015FFD8)[i].state < 0x7F) {
                    float candidate_distance = fast_vec_dist((*(Ent **)&D_L01_0015FFD8)[i].pos,
                                                             &MOBY(reference_moby)->pos);
                    if (candidate_distance < best) {
                        best = candidate_distance;
                        nearest_entity = D_L01_0015FFD8 + (i << 8);
                    }
                }
            }
        }
        if (*list++ < 0) {
            return nearest_entity;
        }
    }
}

void spawn_amoeboid_movement_particles(unsigned char *moby) {
    LargeAmoeboidVars *amoeboid_vars = (LargeAmoeboidVars *)MOBY(moby)->pvars;
    float              pos[3];
    float              angle, radius, lower, upper;
    int                time;
    if (MOBY(moby)->visible == 0)
        return;
    if (random_integer_below(amoeboid_first_movement_particle_spawn_gate_modulus_plus_one_gp - 1) ==
        0) {
        angle = random_angle_radians();
        radius = random_float_between(0.0f, amoeboid_movement_particle_max_spawn_radius_gp);
        lower = random_float_between(0.0f, amoeboid_first_movement_particle_base_scale_max_gp) *
                amoeboid_vars->size_scale;
        upper = random_float_between(amoeboid_first_movement_particle_base_scale_max_gp,
                                     amoeboid_first_movement_particle_secondary_sample_max_gp) *
                amoeboid_vars->size_scale;
        time = truncate_float_to_s32(
            random_float_between((float)amoeboid_first_movement_particle_lifetime_min_gp,
                                 (float)amoeboid_first_movement_particle_lifetime_max_gp));
        pos[0] = fast_cos(angle) * radius;
        pos[1] = fast_sin(angle) * radius;
        pos[2] = 0.0f;
        fast_vec_add(pos, pos, &MOBY(moby)->pos);
        spawn_type_52_particle(pos,
                               lower,
                               upper,
                               amoeboid_first_movement_particle_header_and_data_value_gp,
                               amoeboid_first_movement_particle_data_value_gp,
                               scale_game_frames(time));
    }
    if (random_integer_below(amoeboid_second_movement_particle_spawn_gate_modulus_plus_one_gp -
                             1) == 0) {
        angle = random_angle_radians();
        radius = random_float_between(0.0f, amoeboid_movement_particle_max_spawn_radius_gp);
        lower = random_float_between(0.0f, amoeboid_second_movement_particle_base_scale_max_gp) *
                amoeboid_vars->size_scale;
        upper = random_float_between(amoeboid_second_movement_particle_base_scale_max_gp,
                                     amoeboid_second_movement_particle_secondary_sample_max_gp) *
                amoeboid_vars->size_scale;
        time = truncate_float_to_s32(
            random_float_between((float)amoeboid_second_movement_particle_lifetime_min_gp,
                                 (float)amoeboid_second_movement_particle_lifetime_max_gp));
        pos[0] = fast_cos(angle) * radius;
        pos[1] = fast_sin(angle) * radius;
        pos[2] = 0.0f;
        fast_vec_add(pos, pos, &MOBY(moby)->pos);
        spawn_type_52_particle(pos,
                               lower,
                               upper,
                               amoeboid_second_movement_particle_header_and_data_value_gp,
                               amoeboid_second_movement_particle_data_value_gp,
                               scale_game_frames(time));
    }
}

void spawn_amoeboid_burst_sparks(unsigned char *moby, f32 *mode) {
    LargeAmoeboidVars *burst_amoeboid_vars = (LargeAmoeboidVars *)MOBY(moby)->pvars;
    f32                spawn_position[4], origin[4], velocity[4];
    u128              *dest;
    f32               *origin_copy;
    u128              *source;
    f32                angle, radius, vertical, lateral, scale, scaled_radius, scaled_vertical;
    s32                i, next_group_index, j, lower, upper, spark_header_frame_count,
        downward_adjustment_frame_count, duration;

    i = 0;
    if (amoeboid_burst_group_count_gp > 0) {
        source = (u128 *)&MOBY(moby)->pos;
        dest = (u128 *)spawn_position;
        origin_copy = origin;
        do {
            qcopy_nc(dest, source);
            add_random_offset_to_vector_xyz(dest,
                                            amoeboid_burst_group_position_jitter_bound_gp *
                                                burst_amoeboid_vars->size_scale);
            spawn_position[2] += 1.0f;
            angle = random_angle_radians();
            radius = random_float_between(amoeboid_burst_radial_speed_min_gp,
                                          amoeboid_burst_radial_speed_max_gp);
            lateral = random_float_between(amoeboid_burst_vertical_speed_min_gp,
                                           amoeboid_burst_vertical_speed_max_gp);
            vertical = random_float_between(0.0f, amoeboid_burst_input_motion_scale_max_gp);
            fast_vec_scale(origin_copy, mode, vertical * frame_time_gp);
            scale = fast_cos(angle);
            origin[0] += scale * (radius * frame_time_gp);
            scale = fast_sin(angle);
            scaled_radius = radius * frame_time_gp;
            origin[1] += scale * scaled_radius;
            scaled_vertical = lateral * frame_time_gp;
            origin[2] += scaled_vertical;
            qcopy(velocity, origin_copy);
            next_group_index = i + 1;
            velocity[2] -=
                amoeboid_burst_downward_velocity_adjustment_factor_gp * D_0015ED70 *
                (f32)scale_game_frames(amoeboid_burst_downward_velocity_adjustment_frames_gp);
            for (j = 0; j < amoeboid_burst_sparks_per_group_gp; j++) {
                scale = random_float_between(0.5f, 1.5f);
                origin[3] = scale * D_L01_00161A14;
                velocity[3] = scale * D_L01_00161A18;
                add_random_offset_to_vector_xyz(spawn_position,
                                                amoeboid_burst_spark_position_jitter_bound_gp);
                add_random_offset_to_vector_xyz(
                    velocity, amoeboid_burst_spark_velocity_jitter_bound_gp * frame_time_gp);
                lower = fast_tween_color(random_float_between(0.0f, 1.0f),
                                         amoeboid_burst_spark_spawn_color_randomization_start_gp,
                                         amoeboid_burst_spark_spawn_color_randomization_end_gp);
                upper =
                    fast_tween_color(random_float_between(0.0f, 1.0f),
                                     amoeboid_burst_spark_secondary_color_randomization_start_gp,
                                     amoeboid_burst_spark_secondary_color_randomization_end_gp);
                spark_header_frame_count =
                    scale_game_frames(amoeboid_burst_spark_header_frame_count_gp);
                downward_adjustment_frame_count =
                    scale_game_frames(amoeboid_burst_downward_velocity_adjustment_frames_gp);
                duration = truncate_float_to_s32(FUN_00220e20(
                    random_float_between((f32)amoeboid_burst_spark_duration_base_gp * 0.5f,
                                         (f32)amoeboid_burst_spark_duration_base_gp * 2.5f)));
                spawn_spark(spawn_position,
                            origin_copy,
                            velocity,
                            lower,
                            upper,
                            spark_header_frame_count,
                            downward_adjustment_frame_count,
                            duration,
                            -1);
            }
            i = next_group_index;
        } while (i < amoeboid_burst_group_count_gp);
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F0E60), where it is exact; names translated to the US level program. */

int process_amoeboid_suck_cannon_suction(struct Moby *moby) __asm__("FUN_L01_002efa88");

int process_amoeboid_suck_cannon_suction(struct Moby *moby) {
    int suction_result = process_moby_suck_cannon_suction(moby);
    if (suction_result != 0) {
        if (moby->oclass != 0x362) {
            return 0;
        } else if (moby->state == LARGE_AMOEBOID_PROCESS_KNOCKBACK) {
            return 0;
        } else {
            moby->state = LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION;
        }
    } else if (moby->state == LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION) {
        moby->state = LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH;
    }
    return suction_result;
}

/* Suck cannon methods of the amoeboids. */
extern int store_moby_in_suck_cannon(void *) __asm__("FUN_L00_002dbb20");

/* Moby state 0xE while FUN_L00_002dbb20 reports a hit, back to 1 when it stops. */
int moby_hold_state_if_found(struct Moby *a) __asm__("FUN_L01_002efaf8");

int moby_hold_state_if_found(struct Moby *target_moby) {
    int suck_cannon_store_result = store_moby_in_suck_cannon(target_moby);
    if (suck_cannon_store_result == 0) {
        if (target_moby->state == LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION)
            target_moby->state = LARGE_AMOEBOID_TRY_ACQUIRE_TARGET_PATH;
    } else {
        target_moby->state = LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION;
    }
    return suck_cannon_store_result;
}

extern int cancel_suck_cannon_suction(void *) __asm__("FUN_L00_002dbe20");

/* Runs FUN_L00_002dbe20 on the moby and sets its state to 0xE. */
int cancel_suck_cannon_suction_and_set_state_14(struct Moby *moby) __asm__("FUN_L01_002efb98");

int cancel_suck_cannon_suction_and_set_state_14(struct Moby *moby) {
    int helper_result = cancel_suck_cannon_suction(moby);
    moby->state = LARGE_AMOEBOID_PROCESS_SUCK_CANNON_INTERACTION;
    return helper_result;
}

typedef struct {
    char pad[0x78];
    s32  f;
} T6;

/* Returns the moby data pointer plus 0xC0, or 0 when either is null. */
s32 get_moby_data_plus_0xc0(T6 *moby) __asm__("FUN_L01_002efbc8");

s32 get_moby_data_plus_0xc0(T6 *moby) {
    s32 moby_data_address;
    if (moby != 0) {
        moby_data_address = moby->f;
        if (moby_data_address != 0) {
            return moby_data_address + 0xc0;
        }
        return 0;
    }
    return 0;
}

char  D_L05_001619F0 = 1;
char  D_L05_00161A00 = 1;
float D_L05_00161950 = 15.0f;
float D_L05_00161954 = 5.0f;
short D_L05_00161948 = -32640;
short D_L05_0016194C = -32704;
int   D_L05_00161990 = 0x14;
int   D_L05_00161994 = 5;
float D_L05_00161998 = 0.1f;
float D_L05_00161968 = 1.0f;
int   D_L05_00161970 = 0x78;
int   D_L05_00161974 = 0xb4;
float D_L05_00161980 = 3.5f;
int   D_L05_00161988 = 0x78;
int   D_L05_0016198C = 0xb4;
s32   D_L05_0016199C = 0x14;
s32   D_L05_001619A0 = 0xa;
f32   D_L05_001619A4 = 0.5f;
f32   D_L05_001619A8 = 0.2f;
f32   D_L05_001619AC = 0.5f;
f32   D_L05_001619B0 = 1.0f;
f32   D_L05_001619B4 = 8.0f;
f32   D_L05_001619B8 = 2.0f;
f32   D_L05_001619BC = 6.0f;
f32   D_L05_001619C0 = 2e+01f;
f32   D_L05_001619C4 = 0.125f;
f32   D_L05_001619C8 = 0.065f;
s32   D_L05_001619DC = 0xa;
s32   D_L05_001619E4 = 0xa;
f32   D_L05_001619E8 = 0;

char  D_L11_00161A40 = 1;
char  D_L11_00161A50 = 1;
float D_L11_001619A0 = 15.0f;
float D_L11_001619A4 = 5.0f;
short D_L11_00161998 = -32640;
short D_L11_0016199C = -32704;
int   D_L11_001619E0 = 0x14;
int   D_L11_001619E4 = 5;
float D_L11_001619E8 = 0.1f;
float D_L11_001619B8 = 1.0f;
int   D_L11_001619C0 = 0x78;
int   D_L11_001619C4 = 0xb4;
float D_L11_001619D0 = 3.5f;
int   D_L11_001619D8 = 0x78;
int   D_L11_001619DC = 0xb4;
s32   D_L11_001619EC = 0x14;
s32   D_L11_001619F0 = 0xa;
f32   D_L11_001619F4 = 0.5f;
f32   D_L11_001619F8 = 0.2f;
f32   D_L11_001619FC = 0.5f;
f32   D_L11_00161A00 = 1.0f;
f32   D_L11_00161A04 = 8.0f;
f32   D_L11_00161A08 = 2.0f;
f32   D_L11_00161A0C = 6.0f;
f32   D_L11_00161A10 = 2e+01f;
f32   D_L11_00161A14 = 0.125f;
f32   D_L11_00161A18 = 0.065f;
s32   D_L11_00161A2C = 0xa;
s32   D_L11_00161A34 = 0xa;
f32   D_L11_00161A38 = 0;
