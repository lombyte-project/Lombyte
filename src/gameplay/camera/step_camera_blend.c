/* Ported from rac1-decomp (src/game/camera.c, func_001ECEA0). */
#include "sda.h"
#include "qcopy.h"
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/camera/camera_globals.h"
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern char D_00187080[];
extern float cosine_interpolate(float, float, float) __asm__("FUN_002133d0");
extern void extract_matrix_rotation_quaternion(void *, void *) __asm__("func_002144D8");
extern void quaternion_nlerp(void *, void *, void *, float) __asm__("FUN_001fa400");
extern void quaternion_to_matrix(void *, void *) __asm__("FUN_001fa4f8");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
/* Camera blend step toward to: while the position (cam[3]) or rotation
   (cam[0]) blend hasn't reached 1, move cam+0x40 from cam+0x30 toward
   to's position by the eased factor (cosine_interpolate), copy it to
   D_00187080 unless the D_0018C318 flag is set, slerp the rotation
   (quaternion_nlerp) into cam+0x50 and load it as the view matrix, then
   advance both blends by their rates times D_0015ED60, capped at 1.
   Returns 1 once both are complete. */
int step_camera_blend(void *to_blend, void *cam_blend) __asm__("FUN_001ecaf8");

extern f32 frame_scale __asm__("D_0015ED60") MACRO_ADDR;
int step_camera_blend(void *to_blend, void *cam_blend) {
    float *to = to_blend;
    float *cam = cam_blend;
    float target_rotation_quaternion[4];
    float blended_rotation_matrix[16];
    char *st;
    float t;
    float *rot;

    if (cam[3] == 1.0f && cam[0] == 1.0f) {
        return 1;
    }
    t = cosine_interpolate(0.0f, 1.0f, cam[3]);
    add_vector_xyz(cam + 12, D_0013F490, cam + 12);
    st = occl_cam_state;
    cam[16] = cam[12] + (to[12] - cam[12]) * t;
    cam[17] = cam[13] + (to[13] - cam[13]) * t;
    cam[18] = cam[14] + (to[14] - cam[14]) * t;
    if (*(int *)(st + 0x14) == 0) {
        qcopy(D_00187080, cam + 16);
    }
    extract_matrix_rotation_quaternion(target_rotation_quaternion, to);
    rot = cam + 20;
    t = cosine_interpolate(0.0f, 1.0f, cam[0]);
    quaternion_nlerp(rot, cam + 8, target_rotation_quaternion, t);
    quaternion_to_matrix(rot, blended_rotation_matrix);
    if (*(int *)(st + 0x14) == 0) {
        copy_matrix3x4(camera_rows, blended_rotation_matrix);
    }
    cam[3] += cam[4] * frame_scale;
    if (1.0f < cam[3]) {
        cam[3] = 1.0f;
    }
    cam[0] += cam[1] * frame_scale;
    if (1.0f < cam[0]) {
        cam[0] = 1.0f;
    }
    return 0;
}
extern void add_vector_xyz(void *dst, void *a, void *b) __asm__("FUN_001f9a10");

extern __typeof__(step_camera_blend) func_001ECAF8 __attribute__((alias("FUN_001ecaf8")));
