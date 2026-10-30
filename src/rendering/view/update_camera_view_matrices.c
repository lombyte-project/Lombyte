#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"
#include "rnc/math/asm_helpers.h"

#include "rnc/ui/menus/menu_globals.h"
#include "rnc/camera/update_cam.h"
#include "rnc/camera/camera_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/rendering/view/sky_view_matrix.h"

extern u8 D_0018CD00[];

extern void sceVu0UnitMatrix(void *);
extern void SceVu0RotMatrixX(void *, void *, f32);
extern void SceVu0RotMatrixY(void *, void *, f32);
extern void sceVu0RotMatrixZ(void *, void *, f32);
extern void scale_vector(void *, void *, f32) __asm__("FUN_001f9a80");

void update_camera_view_matrices(void) __asm__("FUN_001f2260");

void update_camera_view_matrices(void) {
    Vec4                  camera_basis_matrix[4];
    struct PreviewCamera *camera_view_matrices;
    u8                   *x;
    f32                  *xf;
    f32                   homogeneous_offset_x;
    f32                   homogeneous_offset_y;
    f32                   homogeneous_offset_z;

    if (camera_position_publication_suppressed[0] == 0) {
        qcopy(&camera_basis_matrix[0].q, camera_rows);
        qcopy(&camera_basis_matrix[1].q, camera_rows + 0x10);
        qcopy(&camera_basis_matrix[2].q, camera_rows + 0x20);
    } else {
        sceVu0UnitMatrix(camera_basis_matrix);
        SceVu0RotMatrixX(camera_basis_matrix, camera_basis_matrix, preview_camera.rotation.f[0]);
        SceVu0RotMatrixY(camera_basis_matrix, camera_basis_matrix, preview_camera.rotation.f[1]);
        sceVu0RotMatrixZ(camera_basis_matrix, camera_basis_matrix, preview_camera.rotation.f[2]);
    }
    camera_view_matrices = &preview_camera;
    x = D_0018CD00 + 0xC0;
    xf = (f32 *)D_0018CD00;
    camera_view_matrices->view_matrices[0] = -camera_basis_matrix[1].f[0];
    camera_view_matrices->view_matrices[0x3C / 4] = 1.0f;
    *(s32 *)&camera_view_matrices->view_matrices[0x30 / 4] = 0;
    camera_view_matrices->view_matrices[0x10 / 4] = -camera_basis_matrix[1].f[1];
    camera_view_matrices->view_matrices[0x20 / 4] = -camera_basis_matrix[1].f[2];
    camera_view_matrices->view_matrices[4 / 4] = -camera_basis_matrix[2].f[0];
    camera_view_matrices->view_matrices[0x14 / 4] = -camera_basis_matrix[2].f[1];
    camera_view_matrices->view_matrices[0x24 / 4] = -camera_basis_matrix[2].f[2];
    camera_view_matrices->view_matrices[8 / 4] = camera_basis_matrix[0].f[0];
    camera_view_matrices->view_matrices[0x18 / 4] = camera_basis_matrix[0].f[1];
    camera_view_matrices->view_matrices[0x28 / 4] = camera_basis_matrix[0].f[2];
    *(s32 *)&camera_view_matrices->view_matrices[0x34 / 4] = 0;
    *(s32 *)&camera_view_matrices->view_matrices[0x38 / 4] = 0;
    *(s32 *)&camera_view_matrices->view_matrices[0xC / 4] = 0;
    *(s32 *)&camera_view_matrices->view_matrices[0x1C / 4] = 0;
    *(s32 *)&camera_view_matrices->view_matrices[0x2C / 4] = 0;
    multiply_matrix(&camera_view_matrices->view_matrices[0x40 / 4], x, camera_view_matrices);
    multiply_matrix(&camera_view_matrices->view_matrices[0x80 / 4], x + 0x40, camera_view_matrices);
    homogeneous_offset_x = xf[0x1A0 / 4];
    homogeneous_offset_y = xf[0x1A4 / 4];
    homogeneous_offset_z = xf[0x1A8 / 4];
    camera_view_matrices->view_matrices[0x80 / 4] +=
        camera_view_matrices->view_matrices[0x8C / 4] * homogeneous_offset_x;
    camera_view_matrices->view_matrices[0x84 / 4] +=
        camera_view_matrices->view_matrices[0x8C / 4] * homogeneous_offset_y;
    camera_view_matrices->view_matrices[0x88 / 4] +=
        camera_view_matrices->view_matrices[0x8C / 4] * homogeneous_offset_z;
    camera_view_matrices->view_matrices[0x90 / 4] +=
        camera_view_matrices->view_matrices[0x9C / 4] * homogeneous_offset_x;
    camera_view_matrices->view_matrices[0x94 / 4] +=
        camera_view_matrices->view_matrices[0x9C / 4] * homogeneous_offset_y;
    camera_view_matrices->view_matrices[0x98 / 4] +=
        camera_view_matrices->view_matrices[0x9C / 4] * homogeneous_offset_z;
    camera_view_matrices->view_matrices[0xA0 / 4] +=
        camera_view_matrices->view_matrices[0xAC / 4] * homogeneous_offset_x;
    camera_view_matrices->view_matrices[0xA4 / 4] +=
        camera_view_matrices->view_matrices[0xAC / 4] * homogeneous_offset_y;
    camera_view_matrices->view_matrices[0xA8 / 4] +=
        camera_view_matrices->view_matrices[0xAC / 4] * homogeneous_offset_z;
    camera_view_matrices->view_matrices[0xB0 / 4] +=
        camera_view_matrices->view_matrices[0xBC / 4] * homogeneous_offset_x;
    camera_view_matrices->view_matrices[0xB4 / 4] +=
        camera_view_matrices->view_matrices[0xBC / 4] * homogeneous_offset_y;
    camera_view_matrices->view_matrices[0xB8 / 4] +=
        camera_view_matrices->view_matrices[0xBC / 4] * homogeneous_offset_z;
    multiply_matrix(&camera_view_matrices->view_matrices[0xC0 / 4], x + 0x80, camera_view_matrices);
    scale_vector(&camera_view_matrices->view_matrices[0x100 / 4], x + 0x80, xf[0x1C0 / 4]);
    scale_vector(&camera_view_matrices->view_matrices[0x110 / 4], x + 0x90, xf[0x1C0 / 4]);
    qcopy(&camera_view_matrices->view_matrices[0x120 / 4], x + 0xA0);
    qcopy(&camera_view_matrices->view_matrices[0x130 / 4], x + 0xB0);
    multiply_matrix(&camera_view_matrices->view_matrices[0x100 / 4],
                    &camera_view_matrices->view_matrices[0x100 / 4],
                    camera_view_matrices);
    qcopy(sky_view_mtx, camera_view_matrices);
    qcopy(sky_view_mtx[1], &camera_view_matrices->view_matrices[0x10 / 4]);
    qcopy(sky_view_mtx[2], &camera_view_matrices->view_matrices[0x20 / 4]);
    fast_vec_scale(sky_view_mtx[3], &camera_view_matrices->position, 1024.0f);
    sky_view_mtx[3][3] = 1024.0f;
}

extern __typeof__(update_camera_view_matrices) func_001F2260 __attribute__((alias("FUN_001f2260")));

f32 sky_view_mtx[4][4] = {0};
