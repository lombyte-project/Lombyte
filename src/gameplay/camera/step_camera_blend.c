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
    float *camera_blend_state = cam_blend;
    float target_rotation_quaternion[4];
    float blended_rotation_matrix[16];
    char *camera_publication_state;
    float eased_blend_factor;
    float *blended_rotation_quaternion;

    if (camera_blend_state[3] == 1.0f && camera_blend_state[0] == 1.0f) {
        return 1;
    }
    eased_blend_factor = cosine_interpolate(0.0f, 1.0f, camera_blend_state[3]);
    add_vector_xyz(camera_blend_state + 12, D_0013F490, camera_blend_state + 12);
    camera_publication_state = occl_cam_state;
    camera_blend_state[16] = camera_blend_state[12] + (to[12] - camera_blend_state[12]) * eased_blend_factor;
    camera_blend_state[17] = camera_blend_state[13] + (to[13] - camera_blend_state[13]) * eased_blend_factor;
    camera_blend_state[18] = camera_blend_state[14] + (to[14] - camera_blend_state[14]) * eased_blend_factor;
    if (*(int *)(camera_publication_state + 0x14) == 0) {
        qcopy(D_00187080, camera_blend_state + 16);
    }
    extract_matrix_rotation_quaternion(target_rotation_quaternion, to);
    blended_rotation_quaternion = camera_blend_state + 20;
    eased_blend_factor = cosine_interpolate(0.0f, 1.0f, camera_blend_state[0]);
    quaternion_nlerp(blended_rotation_quaternion, camera_blend_state + 8, target_rotation_quaternion, eased_blend_factor);
    quaternion_to_matrix(blended_rotation_quaternion, blended_rotation_matrix);
    if (*(int *)(camera_publication_state + 0x14) == 0) {
        copy_matrix3x4(camera_rows, blended_rotation_matrix);
    }
    camera_blend_state[3] += camera_blend_state[4] * frame_scale;
    if (1.0f < camera_blend_state[3]) {
        camera_blend_state[3] = 1.0f;
    }
    camera_blend_state[0] += camera_blend_state[1] * frame_scale;
    if (1.0f < camera_blend_state[0]) {
        camera_blend_state[0] = 1.0f;
    }
    return 0;
}
extern void add_vector_xyz(void *dst, void *a, void *b) __asm__("FUN_001f9a10");

extern __typeof__(step_camera_blend) func_001ECAF8 __attribute__((alias("FUN_001ecaf8")));
