/* Ported from rac1-decomp (src/game/camera.c, func_001ECEA0). */
#include "sda.h"
#include "qcopy.h"
#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/globals.h"
#include "rnc/math/interpolation/cosine_interpolate.h"
#include "rnc/math/quaternions/quaternion_to_matrix.h"
#include "rnc/math/asm_helpers.h"

extern char D_00187080[];
extern void extract_matrix_rotation_quaternion(void *, void *) __asm__("func_002144D8");
extern void copy_matrix3x4(void *, void *) __asm__("func_001FA2B8");
int         step_camera_blend(void *to_blend, struct CameraBlend *blend) __asm__("FUN_001ecaf8");

int step_camera_blend(void *to_blend, struct CameraBlend *blend) {
    float               *to = to_blend;
    float                target_rotation_quaternion[4];
    float                blended_rotation_matrix[16];
    struct OcclCamState *camera_publication_state;
    float                eased_blend_factor;
    float               *blended_rotation_quaternion;

    if (blend->position_progress == 1.0f && blend->rotation_progress == 1.0f) {
        return 1;
    }
    eased_blend_factor = cosine_interpolate(0.0f, 1.0f, blend->position_progress);
    fast_vec_add(blend->from_position, D_0013F490, blend->from_position);
    camera_publication_state = &occl_cam_state;
    blend->position[0] =
        blend->from_position[0] + (to[12] - blend->from_position[0]) * eased_blend_factor;
    blend->position[1] =
        blend->from_position[1] + (to[13] - blend->from_position[1]) * eased_blend_factor;
    blend->position[2] =
        blend->from_position[2] + (to[14] - blend->from_position[2]) * eased_blend_factor;
    if (camera_publication_state->position_frozen == 0) {
        qcopy(D_00187080, blend->position);
    }
    extract_matrix_rotation_quaternion(target_rotation_quaternion, to);
    blended_rotation_quaternion = blend->rotation;
    eased_blend_factor = cosine_interpolate(0.0f, 1.0f, blend->rotation_progress);
    quaternion_nlerp(blended_rotation_quaternion,
                     blend->from_rotation,
                     target_rotation_quaternion,
                     eased_blend_factor);
    quaternion_to_matrix(blended_rotation_quaternion, blended_rotation_matrix);
    if (camera_publication_state->position_frozen == 0) {
        copy_matrix3x4(camera_rows, blended_rotation_matrix);
    }
    blend->position_progress += blend->position_rate * frame_scale;
    if (1.0f < blend->position_progress) {
        blend->position_progress = 1.0f;
    }
    blend->rotation_progress += blend->rotation_rate * frame_scale;
    if (1.0f < blend->rotation_progress) {
        blend->rotation_progress = 1.0f;
    }
    return 0;
}

extern __typeof__(step_camera_blend) func_001ECAF8 __attribute__((alias("FUN_001ecaf8")));
