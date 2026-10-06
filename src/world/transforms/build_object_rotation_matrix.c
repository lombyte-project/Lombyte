/* Ported from rac1-decomp (src/game/space.c, func_0022F128). */

#include "sda.h"
#include "qcopy.h"

extern void update_view_context(void) __asm__("FUN_001f2d98");
extern char D_00187080[];
extern char D_0018CB20_c[] __asm__("D_0018CB20");
extern float D_0018CDB0 NOT_SDA;
extern unsigned char D_0015EDB4_m[4] __asm__("D_0015EDB4") MACRO_ADDR;
extern void sceVu0UnitMatrix(float *);
extern void SceVu0RotMatrixX(float *, float *, float);
extern void SceVu0RotMatrixY(float *, float *, float);
extern void sceVu0RotMatrixZ(float *, float *, float);
extern void fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
unsigned char build_object_rotation_matrix(void) __asm__("FUN_0022de10");

unsigned char build_object_rotation_matrix(void) {
    char *gameplay_state = D_0018CB20_c;
    char *key = *(char **)(gameplay_state + 0x54) + *(int *)(gameplay_state + 0x38) * 32;
    unsigned char flag = key[0xC];
    float *keyframe_angles = (float *)(key + 0x10);
    char *camera_position;
    char *camera_state;
    float rotation_matrix[16];

    D_0018CDB0 = keyframe_angles[3];
    update_view_context();
    camera_position = D_00187080;
    qcopy(camera_position, key);
    sceVu0UnitMatrix(rotation_matrix);
    SceVu0RotMatrixX(rotation_matrix, rotation_matrix, *(float *)(key + 0x10));
    SceVu0RotMatrixY(rotation_matrix, rotation_matrix, keyframe_angles[1]);
    sceVu0RotMatrixZ(rotation_matrix, rotation_matrix, keyframe_angles[2]);
    camera_state = camera_position - 0x140;
    *(float *)(camera_state + 0x350) = -rotation_matrix[8];
    *(float *)(camera_state + 0x360) = -rotation_matrix[0];
    *(float *)(camera_state + 0x370) = rotation_matrix[4];
    *(float *)(camera_state + 0x354) = -rotation_matrix[9];
    *(float *)(camera_state + 0x364) = -rotation_matrix[1];
    *(float *)(camera_state + 0x374) = rotation_matrix[5];
    *(float *)(camera_state + 0x358) = -rotation_matrix[10];
    *(float *)(camera_state + 0x368) = -rotation_matrix[2];
    *(float *)(camera_state + 0x378) = rotation_matrix[6];
    if (D_0015EDB4_m[0] != 0) {
        fast_vec_cross(camera_position + 0x220, camera_position + 0x230, camera_position + 0x210);
    }
    return flag;
}

extern __typeof__(build_object_rotation_matrix) func_0022DE10
    __attribute__((alias("FUN_0022de10")));
