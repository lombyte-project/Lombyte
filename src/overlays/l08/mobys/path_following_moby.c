/* Level 08 moby class 0x1b6: path following moby. */
#include "types.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/rendering/effects/spawn_spark.h"
#include "rnc/math/random.h"

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002B94B0) */
#include "qcopy.h"

extern float D_0015ED70;

#include "rnc/overlay/level_path.h"

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

#include "rnc/overlay/collision.h"

extern float D_0015ED6C MACRO_ADDR;
extern int              coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void             spawn_explosion(void *,
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
extern float            D_0015ED6C;
extern void             FUN_L00_00263d40(int, int);

/* Ported from rac1-decomp (src/overlays/l08_batalia/vendor_002B9438.c: func_L08_002D6C58), where it is exact; names translated to the US level program. */
extern float scale_time(float) __asm__("FUN_001f96b0");

extern void sample_camera_path(void *, s32, void *, void *, s32, f32) __asm__("FUN_00214e58");

/* Ported from rac1-decomp src/overlays/l08_batalia/vendor_002B9438.c (func_L08_002DFBC0) */
/* Update function: switch on the state byte at +0x20. State 0 reads the table D_L08_001B0FB0[*data] (deleting
 * the moby if the index is -1), 1 steps along it, 2 falls under gravity and deletes the moby below z = 10.0
 * or on a hit. */
extern int camera_secondary_mode_lui __asm__("D_001413D4") NOT_SDA;

#include "rnc/overlay/l08/mobys/path_following_moby_data/update_path_following_moby.h"

extern unsigned char D_0013D408[];
extern void          FUN_L08_002dec90_c(char *m, char *pos) __asm__("FUN_L08_002dec90");
void                 update_path_following_moby(char *moby) __asm__("FUN_L08_002de848");

/* Spawns a burst of particles at an offset from the moby, with random sizes and colours from level tuning values. */
extern int   fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern float moby_motion_spark_first_displacement_scale_gp __asm__("D_L08_00161A48")
    __attribute__((sda));
extern float moby_motion_spark_second_displacement_scale_gp __asm__("D_L08_00161A4C")
    __attribute__((sda));
extern float moby_motion_spark_jitter_magnitude_range_end_gp __asm__("D_L08_00161A50")
    __attribute__((sda));
extern int moby_motion_spark_frame_count_random_span_gp __asm__("D_L08_00161A54")
    __attribute__((sda));
extern int moby_motion_spark_second_frame_count_base_gp __asm__("D_L08_00161A58")
    __attribute__((sda));
extern int moby_motion_spark_third_frame_count_base_gp __asm__("D_L08_00161A5C")
    __attribute__((sda));
extern float moby_motion_spark_frame_count_variation_extent_gp __asm__("D_L08_00161A60")
    __attribute__((sda));
extern float moby_motion_spark_first_vector_w_range_start_gp __asm__("D_L08_00161A64")
    __attribute__((sda));
extern float moby_motion_spark_first_vector_w_range_end_gp __asm__("D_L08_00161A68")
    __attribute__((sda));
extern float moby_motion_spark_second_vector_w_range_start_gp __asm__("D_L08_00161A6C")
    __attribute__((sda));
extern float moby_motion_spark_second_vector_w_range_end_gp __asm__("D_L08_00161A70")
    __attribute__((sda));
extern int moby_motion_spark_initial_color_start_gp __asm__("D_L08_00161A74") __attribute__((sda));
extern int moby_motion_spark_initial_color_end_gp __asm__("D_L08_00161A78") __attribute__((sda));
extern int moby_motion_spark_second_color_start_gp __asm__("D_L08_00161A7C") __attribute__((sda));
extern int moby_motion_spark_second_color_end_gp __asm__("D_L08_00161A80") __attribute__((sda));
void       spawn_moby_motion_spark(struct Moby *moby, char *pos) __asm__("FUN_L08_002dec90");

int moby_motion_spark_initial_color_end_gp __asm__("D_L08_00161A78") = 0x6020a0c0;

int moby_motion_spark_second_color_end_gp __asm__("D_L08_00161A80") = 0x20601020;

void update_path_following_moby(char *moby) {
    enum {
        MOBY_STATE_INITIALIZE_PATH = 0,
        MOBY_STATE_FOLLOW_PATH = 1,
        MOBY_STATE_FALLING = 2
    };
    float previous_position[4];
    float frame_displacement[4];
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    char *moby_position = moby + 0x10;
    int  *path;
    qcopy(previous_position, moby_position);
    path = (int *)l08_paths[*(int *)moby_vars];
    switch (MOBY(moby)->state) {
    case MOBY_STATE_INITIALIZE_PATH:
        if (*(int *)moby_vars == -1) {
            mark_moby_for_removal(moby);
            return;
        }
        {
            float first_path_segment_length =
                fast_vec_dist((char *)path + 0x10, (char *)path + 0x20);
            fast_vec_dist((char *)path + 0x10, (char *)path + (*path << 4));
            *(float *)(moby_vars + 8) =
                path_following_moby_speed_gp * D_0015ED6C / first_path_segment_length;
            *(float *)(moby_vars + 4) = *(float *)(moby_vars + 4) * (float)*path;
            *(float *)(moby_vars + 0xC) = randf_sym(0.0f, 5.0f);
        }
        moby[0x20] = 1;
        MOBY(moby)->update_dist = 0xFF;
    case MOBY_STATE_FOLLOW_PATH: {
        float path_point_count;
        sample_camera_path(
            path, 1, moby + 0x10, (float *)&MOBY(moby)->rot, 0, *(float *)(moby_vars + 4));
        MOBY(moby)->rot.y = MOBY(moby)->rot.y * 0.5f;
        MOBY(moby)->pos.z = MOBY(moby)->pos.z + *(float *)(moby_vars + 0xC);
        *(float *)(moby_vars + 4) = *(float *)(moby_vars + 4) + *(float *)(moby_vars + 8);
        path_point_count = (float)*path;
        if (path_point_count < *(float *)(moby_vars + 4)) {
            *(float *)(moby_vars + 4) = *(float *)(moby_vars + 4) - path_point_count;
        }
        break;
    }
    case MOBY_STATE_FALLING: {
        float falling_gravity = 15.0f;
        float falling_removal_height = 10.0f;
        float dd;
        float falling_rotation_step;
        char *falling_velocity = moby_vars + 0x10;
        falling_rotation_step = D_0015ED6C * 7.3303828f;
        *(float *)(moby_vars + 0x18) = *(float *)(moby_vars + 0x18) - D_0015ED70 * falling_gravity;
        dd = fast_add_rotations(MOBY(moby)->rot.x, falling_rotation_step);
        MOBY(moby)->rot.x = MOBY(moby)->rot.x + dd;
        MOBY(moby)->rot.y =
            -fast_arc_tan(vector_length_xy(falling_velocity), *(float *)(moby_vars + 0x18));
        fast_vec_add(moby_position, moby_position, falling_velocity);
        if (MOBY(moby)->pos.z < falling_removal_height) {
            mark_moby_for_removal(moby);
            return;
        }
        if (coll_sphere(1.25f, moby_position, 0, moby)) {
            spawn_explosion(moby,
                            falling_velocity,
                            moby_position,
                            0.0f,
                            0.0f,
                            0x14,
                            0x28,
                            0x10,
                            falling_removal_height,
                            5.0f,
                            9.0f,
                            1.0f,
                            -1,
                            falling_gravity,
                            1,
                            1,
                            -1,
                            0);
            mark_moby_for_removal(moby);
            return;
        }
        break;
    }
    default:
        break;
    }
    if (MOBY(moby)->pos.x < 4.0f || MOBY(moby)->pos.y < 4.0f || MOBY(moby)->pos.z < 4.0f) {
        MOBY(moby)->collision = 0;
    } else {
        MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
    }
    if (find_moby_hit(moby, 0x10000, 0) != 0 && MOBY(moby)->state != 2) {
        if (camera_secondary_mode_lui == 0x32 && D_0013D408[0xC] == 0) {
            D_0013D408[0xC] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            FUN_L00_00263d40(0x53D6, -1);
        }
        fast_vec_sub(frame_displacement, moby + 0x10, previous_position);
        qcopy(moby_vars + 0x10, frame_displacement);
        *(float *)(moby_vars + 0x18) = *(float *)(moby_vars + 0x18) + (D_0015ED6C + D_0015ED6C);
        spawn_explosion(moby,
                        frame_displacement,
                        moby + 0x10,
                        0.0f,
                        0.0f,
                        0x14,
                        0x28,
                        0x10,
                        10.0f,
                        5.0f,
                        9.0f,
                        1.0f,
                        -1,
                        15.0f,
                        1,
                        1,
                        -1,
                        0);
        moby[0x20] = 2;
    }
    MOBY(moby)->hit_slot = 0xFF;
    if (MOBY(moby)->visible != 0) {
        FUN_L08_002dec90_c(moby, (char *)previous_position);
    }
}

void spawn_moby_motion_spark(struct Moby *moby, char *pos) {
    float scaled_displacement_vector[4];
    float jittered_displacement_vector[4];
    float frame_displacement[4];
    float random_jitter[4];
    float spark_position[4];
    float local_spawn_offset[4];
    int initial_spark_color, second_spark_color, frame_count, second_frame_count, third_frame_count;
    char *spawned_spark;
    fast_vec_sub(frame_displacement, &moby->pos, pos);
    *(OvlQuad *)local_spawn_offset = 0;
    local_spawn_offset[0] = -2.3f;
    local_spawn_offset[2] = 0.3f;
    transform_vector_by_basis(spark_position, local_spawn_offset, &moby->rot_row0);
    fast_vec_add(spark_position, spark_position, &moby->pos);
    fast_vec_scale(scaled_displacement_vector,
                   frame_displacement,
                   moby_motion_spark_first_displacement_scale_gp);
    fast_vec_scale(jittered_displacement_vector,
                   frame_displacement,
                   moby_motion_spark_second_displacement_scale_gp);
    random_vector(
        random_jitter, 0.0f, moby_motion_spark_jitter_magnitude_range_end_gp * frame_time);
    fast_vec_add(jittered_displacement_vector, jittered_displacement_vector, random_jitter);
    scaled_displacement_vector[3] =
        random_float_between(moby_motion_spark_first_vector_w_range_start_gp,
                             moby_motion_spark_first_vector_w_range_end_gp);
    jittered_displacement_vector[3] =
        random_float_between(moby_motion_spark_second_vector_w_range_start_gp,
                             moby_motion_spark_second_vector_w_range_end_gp);
    initial_spark_color = fast_tween_color(moby_motion_spark_initial_color_start_gp,
                                           moby_motion_spark_initial_color_end_gp,
                                           random_float_between(0.0f, 1.0f));
    second_spark_color = fast_tween_color(moby_motion_spark_second_color_start_gp,
                                          moby_motion_spark_second_color_end_gp,
                                          random_float_between(0.0f, 1.0f));
    frame_count = truncate_float_to_s32(scale_time(
        (float)moby_motion_spark_frame_count_random_span_gp * random_float_between(0.0f, 1.0f) +
        1.0f));
    second_frame_count = truncate_float_to_s32(
        scale_time((float)moby_motion_spark_second_frame_count_base_gp *
                   (random_float_between(-moby_motion_spark_frame_count_variation_extent_gp,
                                         moby_motion_spark_frame_count_variation_extent_gp) +
                    1.0f)));
    third_frame_count = truncate_float_to_s32(
        scale_time((float)moby_motion_spark_third_frame_count_base_gp *
                   (random_float_between(-moby_motion_spark_frame_count_variation_extent_gp,
                                         moby_motion_spark_frame_count_variation_extent_gp) +
                    1.0f)));
    spawned_spark = spawn_spark(spark_position,
                                scaled_displacement_vector,
                                jittered_displacement_vector,
                                initial_spark_color,
                                second_spark_color,
                                frame_count,
                                second_frame_count,
                                third_frame_count,
                                -1);
    if (spawned_spark != 0) {
        spawned_spark[9] = truncate_float_to_s32(8.0f) - 0x70;
    }
}
