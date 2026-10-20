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
    char *t = D_0018CB20;
    char *key = *(char **)(t + 0x54) + *(int *)(t + 0x38) * 32;
    unsigned char flag = key[0xC];
    float *ang = (float *)(key + 0x10);
    char *pos;
    char *cam;
    float camera_rotation_matrix[16];

    view_tan_half_fov = 0.63f;
    update_view_context();
    pos = D_00187080;
    qcopy(pos, key);
    sceVu0UnitMatrix(camera_rotation_matrix);
    SceVu0RotMatrixX(camera_rotation_matrix, camera_rotation_matrix, *(float *)(key + 0x10));
    SceVu0RotMatrixY(camera_rotation_matrix, camera_rotation_matrix, ang[1]);
    sceVu0RotMatrixZ(camera_rotation_matrix, camera_rotation_matrix, ang[2]);
    cam = pos - 0x140;
    *(float *)(cam + 0x350) = -camera_rotation_matrix[8];
    *(float *)(cam + 0x360) = -camera_rotation_matrix[0];
    *(float *)(cam + 0x370) = camera_rotation_matrix[4];
    *(float *)(cam + 0x354) = -camera_rotation_matrix[9];
    *(float *)(cam + 0x364) = -camera_rotation_matrix[1];
    *(float *)(cam + 0x374) = camera_rotation_matrix[5];
    *(float *)(cam + 0x358) = -camera_rotation_matrix[10];
    *(float *)(cam + 0x368) = -camera_rotation_matrix[2];
    *(float *)(cam + 0x378) = camera_rotation_matrix[6];
    return flag;
}

extern __typeof__(transition_update_movie_camera) func_001EAF88
    __attribute__((alias("FUN_001eaf88")));
