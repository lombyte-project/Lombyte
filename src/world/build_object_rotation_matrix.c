/* Ported from rac1-decomp (src/game/space.c, func_0022F128). */

#include "sda.h"
#include "qcopy.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/ui/menus/menu_globals.h"

extern char          D_00187080[];
extern char          render_sequence[] __asm__("D_0018CB20");
extern float         view_tan_half_fov __asm__("D_0018CDB0") NOT_SDA;
extern unsigned char D_0015EDB4_macro[4] __asm__("D_0015EDB4") MACRO_ADDR;
extern void          sceVu0UnitMatrix(float *);
extern void          SceVu0RotMatrixX(float *, float *, float);
extern void          SceVu0RotMatrixY(float *, float *, float);
extern void          sceVu0RotMatrixZ(float *, float *, float);
extern void          fast_vec_cross(void *, void *, void *) __asm__("func_001F9AD8");
unsigned char        build_object_rotation_matrix(void) __asm__("FUN_0022de10");

unsigned char build_object_rotation_matrix(void) {
    char         *gameplay_state = render_sequence;
    char         *key = *(char **)(gameplay_state + 0x54) + *(int *)(gameplay_state + 0x38) * 32;
    unsigned char flag = key[0xC];
    float        *keyframe_angles = (float *)(key + 0x10);
    char         *camera_position;
    struct GameCamera *camera_state;
    float              rotation_matrix[16];

    view_tan_half_fov = keyframe_angles[3];
    update_view_context();
    camera_position = D_00187080;
    qcopy(camera_position, key);
    sceVu0UnitMatrix(rotation_matrix);
    SceVu0RotMatrixX(rotation_matrix, rotation_matrix, *(float *)(key + 0x10));
    SceVu0RotMatrixY(rotation_matrix, rotation_matrix, keyframe_angles[1]);
    sceVu0RotMatrixZ(rotation_matrix, rotation_matrix, keyframe_angles[2]);
    camera_state = (struct GameCamera *)(camera_position - 0x140);
    camera_state->forward.f[0] = -rotation_matrix[8];
    camera_state->right.f[0] = -rotation_matrix[0];
    camera_state->up.f[0] = rotation_matrix[4];
    camera_state->forward.f[1] = -rotation_matrix[9];
    camera_state->right.f[1] = -rotation_matrix[1];
    camera_state->up.f[1] = rotation_matrix[5];
    camera_state->forward.f[2] = -rotation_matrix[10];
    camera_state->right.f[2] = -rotation_matrix[2];
    camera_state->up.f[2] = rotation_matrix[6];
    if (D_0015EDB4_macro[0] != 0) {
        fast_vec_cross(&camera_state->right, &camera_state->up, &camera_state->forward);
    }
    return flag;
}

extern __typeof__(build_object_rotation_matrix) func_0022DE10
    __attribute__((alias("FUN_0022de10")));
