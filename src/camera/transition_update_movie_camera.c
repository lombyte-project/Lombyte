/* Ported from rac1-decomp (src/game/transition.c, func_001EB338). */

#include "sda.h"
#include "rnc/camera/camera_globals.h"
#include "qcopy.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/ui/menus/menu_globals.h"

extern char   render_sequence[] __asm__("D_0018CB20");
extern float  view_tan_half_fov __asm__("D_0018CDB0") NOT_SDA;
extern void   sceVu0UnitMatrix(float *);
extern void   SceVu0RotMatrixX(float *, float *, float);
extern void   SceVu0RotMatrixY(float *, float *, float);
extern void   sceVu0RotMatrixZ(float *, float *, float);
unsigned char transition_update_movie_camera(void) __asm__("FUN_001eaf88");

unsigned char transition_update_movie_camera(void) {
    char *camera_keyframe_state = render_sequence;
    char *key =
        *(char **)(camera_keyframe_state + 0x54) + *(int *)(camera_keyframe_state + 0x38) * 32;
    unsigned char      flag = key[0xC];
    float             *keyframe_rotation_angles = (float *)(key + 0x10);
    char              *active_camera_position;
    struct GameCamera *camera_basis_write_base;
    float              camera_rotation_matrix[16];

    view_tan_half_fov = 0.63f;
    update_view_context();
    active_camera_position = ((char *)camera_position);
    qcopy(active_camera_position, key);
    sceVu0UnitMatrix(camera_rotation_matrix);
    SceVu0RotMatrixX(camera_rotation_matrix, camera_rotation_matrix, *(float *)(key + 0x10));
    SceVu0RotMatrixY(camera_rotation_matrix, camera_rotation_matrix, keyframe_rotation_angles[1]);
    sceVu0RotMatrixZ(camera_rotation_matrix, camera_rotation_matrix, keyframe_rotation_angles[2]);
    camera_basis_write_base = (struct GameCamera *)(active_camera_position - 0x140);
    camera_basis_write_base->forward.f[0] = -camera_rotation_matrix[8];
    camera_basis_write_base->right.f[0] = -camera_rotation_matrix[0];
    camera_basis_write_base->up.f[0] = camera_rotation_matrix[4];
    camera_basis_write_base->forward.f[1] = -camera_rotation_matrix[9];
    camera_basis_write_base->right.f[1] = -camera_rotation_matrix[1];
    camera_basis_write_base->up.f[1] = camera_rotation_matrix[5];
    camera_basis_write_base->forward.f[2] = -camera_rotation_matrix[10];
    camera_basis_write_base->right.f[2] = -camera_rotation_matrix[2];
    camera_basis_write_base->up.f[2] = camera_rotation_matrix[6];
    return flag;
}

extern __typeof__(transition_update_movie_camera) func_001EAF88
    __attribute__((alias("FUN_001eaf88")));
