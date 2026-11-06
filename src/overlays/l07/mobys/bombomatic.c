/* Level 07 moby class 0x411: bombomatic. */
#include "types.h"
#include "rnc/overlay/shared/mobys/hit_color_flash.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "qcopy.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"

extern char *create_moby(int) __asm__("FUN_0020c4f8");
extern void  update_entity(void *) __asm__("FUN_L00_00250df8");

/* spawns a moby with random velocity and copies position */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030CE70), where it is exact; names translated to the US level program. */
char *spawn_bombomatic_projectile(char *spawn_position,
                                  char *initial_velocity,
                                  int   spawning_moby,
                                  short burst_index) __asm__("FUN_L07_0030ba90");

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E820), where it is exact; names translated to the US level program. */
extern char debris_far_point[] __asm__("D_L07_00166E40");
extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern int  resolve_moby_hit_response(void *, void *, void *, int, int *, float *, int, int);
extern void start_hit_color_flash(void *, short *) __asm__("FUN_L00_0025d458");
extern void update_hit_color_flash(void *, void *) __asm__("FUN_L00_0025d538");
extern void spawn_explosion(void *,
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
extern f32  random_float_between(f32, f32) __asm__("FUN_002132a8");
extern int  allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void spawn_explosion(void *,
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

#include "qzero.h"

/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_002CE470.c: func_L07_0030E8B8), where it is exact; names translated to the US level program. */
char *spawn_bombomatic_projectile(char *pos, char *vec, int c, short d) __asm__("FUN_L07_0030ba90");
extern char              *FUN_L00_0025a420(void *, int, int);
extern float              D_0015ED6C_gp __asm__("D_0015ED6C") MACRO_ADDR;
extern float D_0015ED70   MACRO_ADDR;
extern float              D_L07_0015F580[] MACRO_ADDR;
extern float              FUN_L00_0025abf0(void *, void *, void *, float, float);
extern int D_L07_0015F5CC MACRO_ADDR;
extern void *
FUN_L00_00263fd8(char *, int, float *, void *, int, int, float, float *, float *, float *);
extern void  build_axis_rotation_quaternion(void *, int, float) __asm__("FUN_L00_001fff28");
extern void  FUN_L00_0024f7c8(void *, int, void *);
extern void  FUN_L00_00257470(void *, int, int);
extern void  FUN_L00_0025a120(void *);
extern void  spawn_explosion(void *,
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
extern void  get_target_sight(void *, void *, float) __asm__("FUN_L07_002886d8");
void         attach_manipulator(void *owner, s32 slot_index, void *manip) __asm__("FUN_0020cb10");
void         bombomatic_update(char *m) __asm__("FUN_L07_0030d4d8");
extern int   D_L07_0015F5CC;
void         mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern void  spawn_explosion(void *,
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
extern float random_float_between(float, float) __asm__("FUN_002132a8");
s32          allocate_voice_for_target_entry(s32   entry_index,
                                             s32   flags,
                                             void *target) __asm__("FUN_0022da68");

char *spawn_bombomatic_projectile(char *spawn_position,
                                  char *initial_velocity,
                                  int   spawning_moby,

                                  short burst_index) {
    char *spawned_moby = create_moby(0x372);
    char *moby_data;
    if (spawned_moby) {
        moby_data = ((char *)MOBY(spawned_moby)->pvars);
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        MOBY(spawned_moby)->rot.x = random_angle_radians();
        MOBY(spawned_moby)->rot.y = random_angle_radians();
        MOBY(spawned_moby)->rot.z = random_angle_radians();
        *(float *)(moby_data + 0x18) = randf_sym(100.0f, 300.0f) * DEG_TO_RAD * frame_time;
        *(float *)(moby_data + 0x1C) = randf_sym(100.0f, 300.0f) * DEG_TO_RAD * frame_time;
        qcopy(spawned_moby + 0x10, spawn_position);
        qcopy(moby_data, initial_velocity);
        *(int *)(moby_data + 0x10) = spawning_moby;
        *(int *)(moby_data + 0x14) = burst_index;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

typedef struct BombomaticVars {
    u8                   pad_0[0x20];
    f32                  health;
    u8                   pad_24[0x2];
    s16                  unk26;
    u8                   pad_28[0x38];
    struct HitColorFlash hit_flash;
    u8                   manipulator_70;
    u8                   pad_71[0xF];
    u8                   spin_quaternion; /* quat of manipulator_70, turned by spin_angle */
    u8                   pad_81[0x2F];
    s32                  start_delay_frames;
    s32 next_phase_frame; /* game frame (D_L07_0015F5CC) of the next state change */
    s16 timer_B8;
    s16 launch_burst_index;
    f32 launch_yaw;
    f32 spin_angle;
    s32 alternate_launch; /* toggled per shot; between 40 and 90 away only every other shot fires */
} BombomaticVars;

/* Moby class 1041 */
void bombomatic_update(char *moby) {
    enum {
        BOMBOMATIC_INITIALIZE = 0x0,
        BOMBOMATIC_WAIT_TO_LAUNCH = 0x1,
        BOMBOMATIC_LAUNCH_BURST = 0x2
    };
    enum {
        HIT_REACTION_NONE = 0x0,
        BOMBOMATIC_CLEAR_HEALTH_RESPONSE = 0x1,
        BOMBOMATIC_ALTERNATE_CLEAR_HEALTH_RESPONSE = 0x2,
        HIT_REACTION_START_KNOCKBACK = 0x3,
        HIT_REACTION_KNOCKBACK_RESPONSE = 0x4,
        HIT_REACTION_ALTERNATE_KNOCKBACK_RESPONSE = 0x5,
        HIT_REACTION_START_HIT_KNOCKBACK_RECOVERY = 0x6,
        HIT_REACTION_LAUNCH_HIT_KNOCKBACK_RECOVERY = 0x7,
        HIT_REACTION_ENTER_HIT_KNOCKBACK_RECOVERY = 0x8,
        HIT_REACTION_HIT_ANIMATION_RESPONSE = 0x9,
        HIT_REACTION_ALTERNATE_HIT_ANIMATION_RESPONSE = 0xA,
        HIT_REACTION_NO_ANIMATION_RESPONSE = 0xB
    };
    BombomaticVars *moby_data;
    char           *hit_info;
    char           *health_ptr;
    int             ia;
    int             hit_reaction_result;

    float hit_damage;
    float tv;
    float launch_work_vector[4];
    float target_buffer[20];
    float launch_rotation[4];
    float debris_velocity[4];
    float vA0[4];
    float vB0[16];
    float vF0[4];
    float candidate_point_position[4];

    if (moby == 0) {
        return;
    }
    moby_data = (BombomaticVars *)MOBY(moby)->pvars;
    if (((unsigned char *)moby)[0x31] != 0) {
        if (fast_vec_dist(moby + 0x10, debris_far_point) < 27.0f) {
            FUN_L00_0025a120(moby);
            MOBY(moby)->shadow_range = 0x15;
        }
    }
    if (moby_data == 0) {
        return;
    }
    hit_info = FUN_L00_0025a420(moby, 0x230000, 0);
    health_ptr = &moby_data->health;
    hit_reaction_result = resolve_moby_hit_response(moby, hit_info, health_ptr, 0, &ia, 0, 0, 4);
    switch (hit_reaction_result) {
    case HIT_REACTION_NONE:
        break;
    case BOMBOMATIC_CLEAR_HEALTH_RESPONSE:
        *(int *)health_ptr = 0;
        break;
    case BOMBOMATIC_ALTERNATE_CLEAR_HEALTH_RESPONSE:
        *(int *)health_ptr = 0;
        break;
    case HIT_REACTION_START_KNOCKBACK:
        break;
    case HIT_REACTION_KNOCKBACK_RESPONSE:
        break;
    case HIT_REACTION_ALTERNATE_KNOCKBACK_RESPONSE:
        break;
    case HIT_REACTION_START_HIT_KNOCKBACK_RECOVERY:
        break;
    case HIT_REACTION_LAUNCH_HIT_KNOCKBACK_RECOVERY:
        break;
    case HIT_REACTION_ENTER_HIT_KNOCKBACK_RECOVERY:
        break;
    case HIT_REACTION_HIT_ANIMATION_RESPONSE:
        break;
    case HIT_REACTION_ALTERNATE_HIT_ANIMATION_RESPONSE:
        break;
    case HIT_REACTION_NO_ANIMATION_RESPONSE:
        break;
    }

    if (ia >= 2) {
        char *hit_owner;
        if (hit_info == 0) {
            hit_damage = *(float *)(hit_info + 0x2C);
        } else {
            hit_owner = *(char **)(hit_info + 0x20);
            if (hit_owner == 0) {
                hit_damage = *(float *)(hit_info + 0x2C);
            } else {
                if (*(short *)(hit_owner + 0xA6) == 0x372) {
                    goto eb20;
                }
                hit_damage = *(float *)(hit_info + 0x2C);
            }
        }
        tv = *(float *)health_ptr;
        if (tv <= hit_damage) {
            FUN_L00_00257470(moby, 0, -1);
            *(float *)health_ptr = 0.0f;
            clear_u64_value(launch_work_vector);
            spawn_explosion(moby,
                            launch_work_vector,
                            0,
                            0.0f,
                            0.0f,
                            10,
                            3,
                            16,
                            4.0f,
                            2.0f,
                            9.0f,
                            1.0f,
                            1,
                            15.0f,
                            1,
                            1,
                            -1,
                            0);
            FUN_L00_00263fd8(moby,
                             0x621,
                             (float *)&MOBY(moby)->pos,
                             moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L07_0015F580,
                             D_L07_0015F580,
                             D_L07_0015F580);
            FUN_L00_00263fd8(moby,
                             0x622,
                             (float *)&MOBY(moby)->pos,
                             moby + 0x40,
                             0,
                             0,
                             0.0f,
                             D_L07_0015F580,
                             D_L07_0015F580,
                             D_L07_0015F580);
            mark_moby_for_removal(moby);
            return;
        }
        tv -= hit_damage;
        *(float *)health_ptr = tv;
        moby_data->hit_flash.flash_red = 0xFA;
        moby_data->unk26 = scale_game_frames(60);
        start_hit_color_flash(moby, &moby_data->hit_flash);
    }
eb20:
    ((unsigned char *)moby)[0xA4] = 0xFF;
    update_hit_color_flash(moby, &moby_data->hit_flash);
    get_target_sight(moby, target_buffer, 20.0f);
    switch (((unsigned char *)moby)[0x20]) {
    case BOMBOMATIC_INITIALIZE: {
        int initial_delay_frames = scale_game_frames(moby_data->start_delay_frames);
        int random_initial_delay;
        int random_initial_delay_frames;
        random_initial_delay = random_int_between(0, 10);
        random_initial_delay_frames = scale_game_frames(random_initial_delay);
        moby_data->next_phase_frame =
            D_L07_0015F5CC + initial_delay_frames + random_initial_delay_frames;
        attach_manipulator(moby, 0, &moby_data->manipulator_70);
        *(int *)&moby_data->spin_angle = 0;
        allocate_voice_for_target_entry(0, 4, (int)moby);
        MOBY(moby)->state = BOMBOMATIC_WAIT_TO_LAUNCH;
        moby_data->alternate_launch = 0;
        break;
    }
    case BOMBOMATIC_WAIT_TO_LAUNCH: {
        if (moby_data->next_phase_frame <= D_L07_0015F5CC) {
            int r9 = scale_game_frames(0xB4);
            moby_data->next_phase_frame = D_L07_0015F5CC + r9;
            moby_data->timer_B8 = 0;
            moby_data->launch_yaw = random_angle_radians();
            moby_data->launch_burst_index = 0;
            MOBY(moby)->state = BOMBOMATIC_LAUNCH_BURST;
        }
        break;
    }
    case BOMBOMATIC_LAUNCH_BURST: {
        int i;
        if (fast_dec_timer_s16(&moby_data->timer_B8) != 0) {
            moby_data->timer_B8 = scale_game_frames(7);
            if (((unsigned char *)moby)[0x31] == 0) {
                if (!(vector_distance_xy(moby + 0x10, debris_far_point) < 90.0f &&
                      (moby_data->alternate_launch & 1) != 0)) {
                    if (!(vector_distance_xy(moby + 0x10, debris_far_point) < 40.0f)) {
                        goto eeE4;
                    }
                }
            }
            moby_data->alternate_launch ^= 1;
            if (vector_distance_xy(moby + 0x10, target_buffer) < 20.0f) {
                moby_data->launch_yaw = fast_arc_tan(target_buffer[0] - MOBY(moby)->pos.x,
                                                     target_buffer[1] - MOBY(moby)->pos.y);
            }
            launch_work_vector[0] = random_float_between(3.0f, 11.0f);
            launch_work_vector[1] = random_float_between(-2.0f, 2.0f);
            launch_work_vector[2] = 0.0f;
            vF0[3] = -1.0f;
            {
                float best = 3.1415927f;
                float h;
                for (i = 1; i < 4; i++) {
                    float f;
                    float g;
                    FUN_L00_0024f7c8(moby, i, candidate_point_position);
                    f = fast_arc_tan(candidate_point_position[0] - MOBY(moby)->pos.x,
                                     candidate_point_position[1] - MOBY(moby)->pos.y);
                    g = fast_subtract_rotations(moby_data->launch_yaw,
                                                D_0015ED6C_gp * 17.80235863f);
                    f = fast_difference_between_rotations(f, g);
                    if (f < best) {
                        qcopy(vF0, candidate_point_position);
                        best = f;
                    }
                }
                qcopy(vA0, vF0);
            }
            qzero(launch_rotation);
            launch_rotation[2] = moby_data->launch_yaw;
            euler_rotation_matrix(vB0, launch_rotation);
            transform_vector(launch_work_vector, launch_work_vector, vB0);
            fast_vec_normalize(debris_velocity, launch_work_vector, D_0015ED6C_gp * 7.0f);
            fast_vec_add(launch_work_vector, launch_work_vector, moby + 0x10);
            debris_velocity[2] = FUN_L00_0025abf0(
                vA0, launch_work_vector, 0, D_0015ED6C_gp * 7.0f, -(D_0015ED70 * 9.8f));
            fast_vec_add(vA0, vA0, debris_velocity);
            spawn_bombomatic_projectile(
                vA0, debris_velocity, (int)moby, moby_data->launch_burst_index);
            moby_data->launch_burst_index = moby_data->launch_burst_index + 1;
        }
    eeE4:
        if (moby_data->next_phase_frame <= D_L07_0015F5CC) {
            int r9 = scale_game_frames(0x78);
            moby_data->next_phase_frame = D_L07_0015F5CC + r9;
            MOBY(moby)->state = BOMBOMATIC_WAIT_TO_LAUNCH;
        }
        break;
    }
    }

    if (((unsigned char *)moby)[0x20] != 0) {
        float f = D_0015ED6C_gp * 17.80235863f;
        float g = fast_add_rotations(moby_data->spin_angle, f);
        moby_data->spin_angle = g;
        build_axis_rotation_quaternion(&moby_data->spin_quaternion, 2, g);
    }
}
