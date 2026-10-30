/* Camera type 5, the scripted camera: springs or snaps to a requested pose; jet_mount mounts it, jet_end_mount hands back, and its track acceleration and snap setters. */

#include "types.h"
#include "rnc/hero/hero.h"
#include "sda.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/interpolation/cosine_interpolate.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/rendering/debug/debug_menu.h"

#define F(p, o) (*(float *)((char *)(p) + (o)))
#define F(p, o) (*(float *)((char *)(p) + (o)))

typedef struct {
    f32 v[4];
} __attribute__((aligned(16))) V_2eaaa0;

f32          cam_interp_values(void *, f32, f32, f32, f32, f32) __asm__("FUN_001ebd78");
f32          rope_spring_turn(void *, f32, f32, f32, f32, f32) __asm__("FUN_L00_001eb328");
extern char *find_camera_record_by_id(int) __asm__("FUN_L00_001eb1f8");
void         switch_active_camera_record(u8 *) __asm__("FUN_001ebf10");
void         compute_spherical_offset_from_positions(void *,
                                                     V_2eaaa0 *,
                                                     V_2eaaa0 *) __asm__("FUN_L00_001ed630");
void         initialize_camera_record_handoff_defaults(void) __asm__("FUN_L00_001eb448");
void         FUN_001f9bf8_2eac18(void *, void *, float) __asm__("FUN_001f9bf8");
void         FUN_001f9a10_2eac18(void *, void *, void *) __asm__("FUN_001f9a10");
extern float cubic_hermite_scalar(float, float, float, float, float) __asm__("FUN_L00_00257e20");
extern float
interpolate_wrapped_angle_cosine_eased(float, float, float) __asm__("FUN_L00_00257ef0");
extern void
compute_position_from_spherical_offset(void *, void *, void *) __asm__("FUN_L00_001ed580");
extern void
interpolate_vector_xyz_cosine_eased(void *, void *, void *, float) __asm__("FUN_L00_00257f70");
extern void interpolate_wrapped_rotations_cosine_eased(float *,
                                                       float *,
                                                       float *,
                                                       float) __asm__("FUN_L00_00258050");
extern void resolve_camera_movement_collisions(void *, float) __asm__("FUN_L00_001ed6a8");

extern LevelCamera          l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;
extern char *D_L00_00166E00 NOT_SDA;
extern V_2eaaa0             D_0013F3D0_2eaaa0 __asm__("D_0013F3D0") NOT_SDA;
extern float D_L00_00166DD8 NOT_SDA;

float D_L04_00166F58 NOT_SDA = 0;
float D_L07_00166E58 NOT_SDA = 0;
float D_L09_00166F58 NOT_SDA = 0;
float D_L10_00167258 NOT_SDA = 0;
float D_L13_001670D8 NOT_SDA = 0;

/* Copies a vector into the current object's slot, switching to state 5 first. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EBE88), where it is exact; names translated to the US level program. */
void set_camera_position(char *requested_position) __asm__("FUN_L00_002ea9d8");

void set_camera_position(char *requested_position) {
    char *current_camera_record = D_L00_00166E00;
    char *camera_record;
    if (*(short *)(current_camera_record + 0x86) != 5)
        camera_record = find_camera_record_by_id(5);
    else
        camera_record = current_camera_record;
    qcopy(*(char **)(camera_record + 0x70) + 0x80, requested_position);
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002F7700.c: func_L01_00318260), where it is exact; names translated to the US level program. */

extern char *D_L01_00167280_lui __asm__("D_L01_00167280") NOT_SDA;
void         set_camera_5_track_acceleration_yz(float camera_track_accel_y,
                                                float camera_track_accel_z) __asm__("FUN_L01_00316e88");

void set_camera_5_track_acceleration_yz(float camera_track_accel_y, float camera_track_accel_z) {
    char *camera_candidate = D_L01_00167280_lui;
    char *selected_camera;
    char *destination;
    if (*(short *)(camera_candidate + 0x86) != 5)
        selected_camera = find_camera_record_by_id(5);
    else
        selected_camera = camera_candidate;
    destination = *(char **)(selected_camera + 0x70) + 0xE0;
    *(float *)(destination + 0x44) = camera_track_accel_y;
    *(float *)(destination + 0x48) = camera_track_accel_z;
}

void jet_mount(V_2eaaa0 *pa, V_2eaaa0 *pb, s32 mode, s32 w, s32 x) __asm__("FUN_L00_002eaaa0");

void jet_mount(V_2eaaa0 *pa, V_2eaaa0 *pb, s32 mode, s32 transition_duration_frames, s32 x) {
    u8      *camera_record = find_camera_record_by_id(5);
    V_2eaaa0 requested_position, requested_rotation, saved_camera_position, saved_camera_rotation;
    u8      *camera_state;
    u8      *transition_state;
    u8      *target_position;
    qcopy(&requested_position, pa);
    qcopy(&requested_rotation, pb);
    qcopy(&saved_camera_position, g_debug_camera.position);
    qcopy(&saved_camera_rotation, g_debug_camera.angles);
    switch_active_camera_record(camera_record);
    camera_state = *(u8 **)(camera_record + 0x70);
    transition_state = camera_state + 0xE0;
    target_position = camera_state + 0x80;
    camera_record[0x7C] = 7;
    camera_record[0x7D] = 1;
    qcopy(camera_record + 0x30, &requested_position);
    qcopy(g_debug_camera.position, &requested_position);
    qcopy(camera_record + 0x40, &requested_rotation);
    qcopy(target_position, &requested_position);
    qcopy(camera_state + 0x90, &requested_rotation);
    *(s32 *)(transition_state + 0x40) = x;
    camera_record[0x88] = mode;
    if (mode != 2) {
        if (mode != 3)
            return;
        *(f32 *)(transition_state + 0x44) = 1.0f;
        *(f32 *)(transition_state + 0x48) = 0.0f;
        qcopy(camera_record + 0x30, &saved_camera_position);
        qcopy(camera_record + 0x40, &saved_camera_rotation);
        qcopy(camera_state + 0x110, &D_0013F3D0_2eaaa0);
        compute_spherical_offset_from_positions(
            camera_state + 0xFC, &saved_camera_position, &D_0013F3D0_2eaaa0);
    }
    *(s32 *)(transition_state + 4) = transition_duration_frames;
    *(s32 *)(camera_state + 0xE0) = transition_duration_frames;
}

void jet_end_mount(int mode) __asm__("FUN_L00_002eac18");

void jet_end_mount(int mode) {
    char *camera_globals = (char *)&l00_camera_lui;
    char *current_follow_camera = *(char **)(camera_globals + 0x180);
    initialize_camera_record_handoff_defaults();
    if (mode == 0) {
        float camera_up_offset[4] __attribute__((aligned(16)));
        float camera_backward_offset[4] __attribute__((aligned(16)));
        float hero_rotation_matrix[16] __attribute__((aligned(16)));
        char *hero_rotation = (char *)&hero.motion.rot;
        char *camera_position = current_follow_camera + 0x30;
        euler_rotation_matrix(hero_rotation_matrix, hero_rotation);
        copy_matrix3x4(current_follow_camera, hero_rotation_matrix);
        FUN_001f9bf8_2eac18(camera_backward_offset, hero_rotation_matrix, -1.0f);
        FUN_001f9bf8_2eac18(camera_up_offset, &hero_rotation_matrix[8], 1.2f);
        FUN_001f9a10_2eac18(camera_position, hero_rotation - 0x10, camera_up_offset);
        FUN_001f9a10_2eac18(camera_position, camera_position, camera_backward_offset);
        *(short *)(*(char **)(camera_globals + 0x180) + 0x8A) = 1;
    } else if (mode == 2) {
        *(char *)(camera_globals + 0x273) = 0;
        *(short *)(current_follow_camera + 0x7E) = 2;
        *(float *)(camera_globals + 0x294) = 0.018f;
        *(float *)(camera_globals + 0x288) = 0.018f;
    } else if (mode == 1) {
        *(short *)(current_follow_camera + 0x7E) = 3;
    } else if (mode == 4) {
        *(char *)(camera_globals + 0x273) = 0;
        *(short *)(current_follow_camera + 0x7E) = 5;
        *(float *)(camera_globals + 0x294) = 0.018f;
        *(float *)(camera_globals + 0x288) = 0.018f;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002D3DF8.c: func_L08_00315068), where it is exact; names translated to the US level program. */

extern char *D_L08_00167600 NOT_SDA;

/* Puts the current camera in mount mode 1, which snaps it to its requested pose each frame (the mounted turrets call it). */
void snap_scripted_camera_to_pose(void) __asm__("FUN_L08_00313ba8");

void snap_scripted_camera_to_pose(void) {
    D_L08_00167600[0x88] = 1;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EC210), where it is exact; names translated to the US level program. */
void initialize_camera_pose_springs(char *camera_object) __asm__("FUN_L00_002ead60");

void initialize_camera_pose_springs(char *camera_object) {
    float rotation_matrix[16];
    char *rotation_spring_state;
    char *camera_state;
    *(int *)(*(char **)(camera_object + 0x70) + 0xC0) = 4;
    rotation_spring_state = *(char **)(camera_object + 0x70) + 0x30;
    *(float *)(rotation_spring_state + 0x20) = 0.05f;
    *(float *)(rotation_spring_state + 0x24) = 0.3f;
    *(float *)(rotation_spring_state + 0x28) = 1.0f;
    *(short *)(rotation_spring_state + 0x2C) = 1;
    clear_u64_value(rotation_spring_state);
    camera_state = *(char **)(camera_object + 0x70);
    *(float *)(camera_state + 0x10) = 0.3f;
    *(float *)(camera_state + 0x14) = 0.3f;
    *(float *)(camera_state + 0x18) = 1.0f;
    *(short *)(camera_state + 0x1C) = 1;
    clear_u64_value(camera_state);
    euler_rotation_matrix(rotation_matrix, camera_object + 0x40);
    copy_matrix3x4(camera_object, rotation_matrix);
    *(short *)(camera_object + 0x7E) = 0;
}

/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EC2D0), where it is exact; names translated to the US level program. */
void update_camera_pose_from_targets(void *camera) __asm__("FUN_L00_002eae20");

void update_camera_pose_from_targets(void *camera) {
    char *camera_object = camera;
    float mtx[16];
    char *camera_state = *(char **)(camera_object + 0x70);
    char *rotation_spring_state = camera_state + 0x30;
    char *target_position = camera_state + 0x80;
    char *transition_state = camera_state + 0xE0;
    int   t = *(unsigned char *)(camera_object + 0x88);
    if (t == 1) {
        qcopy(camera_object + 0x30, target_position);
        qcopy(camera_object + 0x40, camera_state + 0x90);
    } else if (t == 2) {
        float transition_fraction;
        fast_dec_timer((int *)transition_state);
        transition_fraction =
            (float)(*(int *)(transition_state + 4) - *(int *)(camera_state + 0xE0)) /
            convert_integer_to_float(*(int *)(transition_state + 4));
        interpolate_vector_xyz_cosine_eased(
            camera_object + 0x30, camera_object + 0x30, target_position, transition_fraction);
        interpolate_wrapped_rotations_cosine_eased((float *)(camera_object + 0x40),
                                                   (float *)(camera_object + 0x40),
                                                   (float *)(camera_state + 0x90),
                                                   transition_fraction);
    } else if (t == 3) {
        if (fast_dec_timer((int *)transition_state) != 0) {
            qcopy(camera_object + 0x30, target_position);
            qcopy(camera_object + 0x40, camera_state + 0x90);
        } else {
            float t, x, y, transition_azimuth_delta, z;
            t = (float)(*(int *)(transition_state + 4) - *(int *)(camera_state + 0xE0)) /
                convert_integer_to_float(*(int *)(transition_state + 4));
            compute_spherical_offset_from_positions(
                camera_state + 0xE8, target_position, &hero.motion.pos);
            x = fast_subtract_rotations(F(transition_state, 8), F(transition_state, 0x1C));
            y = fast_subtract_rotations(F(target_position, 0x18), D_L00_00166DD8);
            if ((x > 0.34906584f && y < -0.34906584f) || (x < -0.34906584f && y > 0.34906584f)) {
                transition_azimuth_delta = F(transition_state, 8) - F(transition_state, 0x1C);
                z = 0;
                if (transition_azimuth_delta < z && y > z) {
                    transition_azimuth_delta = -transition_azimuth_delta;
                } else if (transition_azimuth_delta > z && y < z) {
                    transition_azimuth_delta = -transition_azimuth_delta;
                }
                F(transition_state, 0x1C) += cosine_interpolate(0, transition_azimuth_delta, t);
            } else {
                F(transition_state, 0x1C) = interpolate_wrapped_angle_cosine_eased(
                    F(transition_state, 0x1C), F(transition_state, 8), t);
            }
            F(transition_state, 0x24) =
                F(transition_state, 0x24) +
                (F(transition_state, 0x10) - F(transition_state, 0x24)) *
                    cubic_hermite_scalar(
                        F(transition_state, 0x44), 0, 1.0f, F(transition_state, 0x48), t);
            F(transition_state, 0x20) = interpolate_wrapped_angle_cosine_eased(
                F(transition_state, 0x20), F(transition_state, 0xC), t);
            interpolate_vector_xyz_cosine_eased(
                transition_state + 0x30, transition_state + 0x30, &hero.motion.pos, t);
            compute_position_from_spherical_offset(
                camera_object + 0x30, transition_state + 0x1C, transition_state + 0x30);
            interpolate_wrapped_rotations_cosine_eased((float *)(camera_object + 0x40),
                                                       (float *)(camera_object + 0x40),
                                                       (float *)(target_position + 0x10),
                                                       t);
        }
    } else {
        if (*(short *)(camera_state + 0x1C) == 1) {
            F(camera_object, 0x30) = cam_interp_values(camera_state,
                                                       F(camera_object, 0x30),
                                                       F(target_position, 0),
                                                       F(camera_state, 0x10),
                                                       F(camera_state, 0x14),
                                                       F(camera_state, 0x18));
            F(camera_object, 0x34) = cam_interp_values(camera_state + 4,
                                                       F(camera_object, 0x34),
                                                       F(target_position, 4),
                                                       F(camera_state, 0x10),
                                                       F(camera_state, 0x14),
                                                       F(camera_state, 0x18));
            F(camera_object, 0x38) = cam_interp_values(camera_state + 8,
                                                       F(camera_object, 0x38),
                                                       F(target_position, 8),
                                                       F(camera_state, 0x10),
                                                       F(camera_state, 0x14),
                                                       F(camera_state, 0x18));
        }
        if (*(short *)(rotation_spring_state + 0x2C) == 1) {
            F(camera_object, 0x40) = rope_spring_turn(rotation_spring_state,
                                                      F(camera_object, 0x40),
                                                      F(target_position, 0x10),
                                                      F(rotation_spring_state, 0x20),
                                                      F(rotation_spring_state, 0x24),
                                                      F(rotation_spring_state, 0x28));
            F(camera_object, 0x44) = rope_spring_turn(camera_state + 0x34,
                                                      F(camera_object, 0x44),
                                                      F(target_position, 0x14),
                                                      F(rotation_spring_state, 0x20),
                                                      F(rotation_spring_state, 0x24),
                                                      F(rotation_spring_state, 0x28));
            F(camera_object, 0x48) = rope_spring_turn(camera_state + 0x38,
                                                      F(camera_object, 0x48),
                                                      F(target_position, 0x18),
                                                      F(rotation_spring_state, 0x20),
                                                      F(rotation_spring_state, 0x24),
                                                      F(rotation_spring_state, 0x28));
        }
    }
    euler_rotation_matrix(mtx, camera_object + 0x40);
    copy_matrix3x4(camera_object, mtx);
}

/* Runs FUN_L00_002eae20; if word +0x120 of its object is set, clears 0x89 and calls 001ed6a8. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002EB0D8.c: func_L00_002EC6C8), where it is exact; names translated to the US level program. */
void update_camera_pose_with_optional_collision(char *camera_object) __asm__("FUN_L00_002eb218");

void update_camera_pose_with_optional_collision(char *camera_object) {
    char *transition_state = *(char **)(camera_object + 0x70) + 0xE0;
    update_camera_pose_from_targets(camera_object);
    if (*(int *)(transition_state + 0x40) != 0) {
        camera_object[0x89] = 0;
        resolve_camera_movement_collisions(camera_object + 0x30, 0.5f);
    }
}
