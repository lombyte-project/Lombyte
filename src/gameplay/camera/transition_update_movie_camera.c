/* Ported from rac1-decomp (src/game/transition.c, func_001EB338). */

#include "sda.h"
#include "qcopy.h"

extern char D_0018CB20[];
extern float view_tan_half_fov __asm__("D_0018CDB0") NOT_SDA;
extern char D_00187080[];
extern void update_view_context(void) __asm__("FUN_001f2d98");
extern void sceVu0UnitMatrix(float *);
extern void SceVu0RotMatrixX(float *, float *, float);
extern void SceVu0RotMatrixY(float *, float *, float);
extern void sceVu0RotMatrixZ(float *, float *, float);
unsigned char transition_update_movie_camera(void) __asm__("FUN_001eaf88");

unsigned char transition_update_movie_camera(void) {
    char *camera_keyframe_state = D_0018CB20;
    char *key = *(char **)(camera_keyframe_state + 0x54) + *(int *)(camera_keyframe_state + 0x38) * 32;
    unsigned char flag = key[0xC];
    float *keyframe_rotation_angles = (float *)(key + 0x10);
    char *active_camera_position;
    char *camera_basis_write_base;
    float camera_rotation_matrix[16];

    view_tan_half_fov = 0.63f;
    update_view_context();
    active_camera_position = D_00187080;
    qcopy(active_camera_position, key);
    sceVu0UnitMatrix(camera_rotation_matrix);
    SceVu0RotMatrixX(camera_rotation_matrix, camera_rotation_matrix, *(float *)(key + 0x10));
    SceVu0RotMatrixY(camera_rotation_matrix, camera_rotation_matrix, keyframe_rotation_angles[1]);
    sceVu0RotMatrixZ(camera_rotation_matrix, camera_rotation_matrix, keyframe_rotation_angles[2]);
    camera_basis_write_base = active_camera_position - 0x140;
    *(float *)(camera_basis_write_base + 0x350) = -camera_rotation_matrix[8];
    *(float *)(camera_basis_write_base + 0x360) = -camera_rotation_matrix[0];
    *(float *)(camera_basis_write_base + 0x370) = camera_rotation_matrix[4];
    *(float *)(camera_basis_write_base + 0x354) = -camera_rotation_matrix[9];
    *(float *)(camera_basis_write_base + 0x364) = -camera_rotation_matrix[1];
    *(float *)(camera_basis_write_base + 0x374) = camera_rotation_matrix[5];
    *(float *)(camera_basis_write_base + 0x358) = -camera_rotation_matrix[10];
    *(float *)(camera_basis_write_base + 0x368) = -camera_rotation_matrix[2];
    *(float *)(camera_basis_write_base + 0x378) = camera_rotation_matrix[6];
    return flag;
}

extern __typeof__(transition_update_movie_camera) func_001EAF88
    __attribute__((alias("FUN_001eaf88")));
