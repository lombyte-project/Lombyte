/* Ported from rac1-decomp (src/game/camera.c, func_001ECC48). */
#include "sda.h"
#include "rnc/gameplay/hero.h"
#include "qcopy.h"
extern void fast_vec_normalize(void *dst, void *src, float len) __asm__("FUN_001f9bf8"); /* dst = normalize(src) * len */
extern void compute_camera_angles(float *out, void *p0, void *p1, void *dir0, void *dir1,
                                  void *axis) __asm__("func_001EC530");
extern char cam_trans_state[] __asm__("D_001871B0");
extern void capture_camera_orientation(void) __asm__("func_001EC710");
extern void save_camera_vectors(void) __asm__("func_001EC7F0");
extern void restore_camera_vectors(void) __asm__("func_001EC868");
extern short D_001871B0_h __asm__("D_001871B0") NOT_SDA;
extern void extract_matrix_rotation_quaternion(void *, void *) __asm__("func_002144D8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern float convert_integer_to_float(int) __asm__("func_001FA6C0");
/* Camera mode update from arg (its +0x30 vector is copied). In mode 1
   the sub-mode at +3 picks what is captured from it: 0 the +0x50/+0x60
   pair, 2 the +0xC0/+0xD0 pair and capture_camera_orientation's angles, otherwise
   D_0013F350's axes and compute_camera_angles's yaw/pitch/dist into +0x70. Other
   modes restore through func_001EC7F0/func_001EC868. Then the mode
   becomes 3, +2 takes the sub-mode, and either the +0x10 blend resets or
   the +0x70 timer advances. */
void start_camera_blend(void *arg) __asm__("FUN_001ec8a0");

void start_camera_blend(void *source_camera_transform) {
    char *blend_source_transform = source_camera_transform;
    char *r = cam_trans_state;
    char orbit_reference_direction[16];
    char orbit_yaw_side_direction[16];
    char orbit_axis[16];
    unsigned char sub;

    if (*(short *)r == 1) {
        sub = r[3];
        if (sub == 0) {
            qcopy(r + 0x50, blend_source_transform + 0x30);
            extract_matrix_rotation_quaternion(r + 0x60, blend_source_transform);
        } else if (sub == 2) {
            qcopy(r + 0xC0, blend_source_transform + 0x30);
            extract_matrix_rotation_quaternion(r + 0xD0, blend_source_transform);
            capture_camera_orientation();
        } else {
            struct Hero *g = &hero;

            fast_vec_normalize(orbit_reference_direction, (char *)g->moby + 0xC0, 1.0f);
            fast_vec_normalize(orbit_yaw_side_direction, (char *)g->moby + 0xD0, 1.0f);
            fast_vec_normalize(orbit_axis, (char *)g->moby + 0xE0, 1.0f);
            compute_camera_angles((float *)(r + 0x70), blend_source_transform + 0x30, *(char **)(r - 0xF0) + 0x30,
                                  orbit_reference_direction, orbit_yaw_side_direction, orbit_axis);
            extract_matrix_rotation_quaternion(r + 0xB0, blend_source_transform);
            qcopy(r + 0xD0, r + 0xB0);
        }
    } else {
        sub = r[3];
        if (sub == 2) {
            save_camera_vectors();
            capture_camera_orientation();
        } else if (sub == 1) {
            save_camera_vectors();
            qcopy(r + 0xB0, r + 0xD0);
        } else if (sub == 0) {
            restore_camera_vectors();
        }
    }
    D_001871B0_h = 3;
    *(unsigned char *)(r + 2) = r[3];
    if (*(unsigned char *)(r + 2) == 0) {
        char *primary_transition_state = r + 0x10;

        *(float *)(primary_transition_state + 0x10) = *(float *)(primary_transition_state + 0x14);
        *(int *)(primary_transition_state + 0xC) = 0;
        qcopy(r + 0x40, r + 0x50);
        *(float *)(primary_transition_state + 0x0) = 0;
        *(float *)(primary_transition_state + 0x4) = *(float *)(primary_transition_state + 0x8);
        qcopy(r + 0x30, r + 0x60);
    } else {
        char *b = r + 0x70;
        int configured_frame_count = ++*(int *)(b + 0x14);

        *(int *)(b + 0xC) = scale_game_frames(configured_frame_count);
        *(float *)(b + 0x10) = 1.0f / convert_integer_to_float(*(int *)(b + 0xC));
    }
}
extern float convert_integer_to_float(int arg0) __asm__("func_001FA6C0");
extern void fast_vec_normalize(void *dst, void *src, float len) __asm__("FUN_001f9bf8");

extern __typeof__(start_camera_blend) func_001EC8A0 __attribute__((alias("FUN_001ec8a0")));
