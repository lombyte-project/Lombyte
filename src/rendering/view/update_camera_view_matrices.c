#include "types.h"
#include "eetypes.h"
#include "qcopy.h"

#include "rnc/math/vector.h"

struct G {
    f32 f[0x50];
};
extern struct G D_00186F40;
extern char camera_rows[] __asm__("D_00187290");
#include "rnc/gameplay/camera/update_cam.h"
extern u8 D_0018CD00[];
extern u8 sky_view_mtx[] __asm__("D_0018CF80");

extern void sceVu0UnitMatrix(void *);
extern void SceVu0RotMatrixX(void *, void *, f32);
extern void SceVu0RotMatrixY(void *, void *, f32);
extern void sceVu0RotMatrixZ(void *, void *, f32);
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void scale_vector(void *, void *, f32) __asm__("FUN_001f9a80");
extern void fast_vec_scale(void *, void *, f32) __asm__("FUN_001f9a68");

void update_camera_view_matrices(void) __asm__("FUN_001f2260");

void update_camera_view_matrices(void) {
    Vec4 camera_basis_matrix[4];
    struct G *g;
    u8 *x;
    f32 *xf;
    f32 a;
    f32 b;
    f32 c;

    if (camera_position_publication_suppressed[0] == 0) {
        qcopy(&camera_basis_matrix[0].q, camera_rows);
        qcopy(&camera_basis_matrix[1].q, camera_rows + 0x10);
        qcopy(&camera_basis_matrix[2].q, camera_rows + 0x20);
    } else {
        sceVu0UnitMatrix(camera_basis_matrix);
        SceVu0RotMatrixX(camera_basis_matrix, camera_basis_matrix, D_00186F40.f[0x150 / 4]);
        SceVu0RotMatrixY(camera_basis_matrix, camera_basis_matrix, D_00186F40.f[0x154 / 4]);
        sceVu0RotMatrixZ(camera_basis_matrix, camera_basis_matrix, D_00186F40.f[0x158 / 4]);
    }
    g = &D_00186F40;
    x = D_0018CD00 + 0xC0;
    xf = (f32 *)D_0018CD00;
    g->f[0] = -camera_basis_matrix[1].f[0];
    g->f[0x3C / 4] = 1.0f;
    *(s32 *)&g->f[0x30 / 4] = 0;
    g->f[0x10 / 4] = -camera_basis_matrix[1].f[1];
    g->f[0x20 / 4] = -camera_basis_matrix[1].f[2];
    g->f[4 / 4] = -camera_basis_matrix[2].f[0];
    g->f[0x14 / 4] = -camera_basis_matrix[2].f[1];
    g->f[0x24 / 4] = -camera_basis_matrix[2].f[2];
    g->f[8 / 4] = camera_basis_matrix[0].f[0];
    g->f[0x18 / 4] = camera_basis_matrix[0].f[1];
    g->f[0x28 / 4] = camera_basis_matrix[0].f[2];
    *(s32 *)&g->f[0x34 / 4] = 0;
    *(s32 *)&g->f[0x38 / 4] = 0;
    *(s32 *)&g->f[0xC / 4] = 0;
    *(s32 *)&g->f[0x1C / 4] = 0;
    *(s32 *)&g->f[0x2C / 4] = 0;
    multiply_matrix(&g->f[0x40 / 4], x, g);
    multiply_matrix(&g->f[0x80 / 4], x + 0x40, g);
    a = xf[0x1A0 / 4];
    b = xf[0x1A4 / 4];
    c = xf[0x1A8 / 4];
    g->f[0x80 / 4] += g->f[0x8C / 4] * a;
    g->f[0x84 / 4] += g->f[0x8C / 4] * b;
    g->f[0x88 / 4] += g->f[0x8C / 4] * c;
    g->f[0x90 / 4] += g->f[0x9C / 4] * a;
    g->f[0x94 / 4] += g->f[0x9C / 4] * b;
    g->f[0x98 / 4] += g->f[0x9C / 4] * c;
    g->f[0xA0 / 4] += g->f[0xAC / 4] * a;
    g->f[0xA4 / 4] += g->f[0xAC / 4] * b;
    g->f[0xA8 / 4] += g->f[0xAC / 4] * c;
    g->f[0xB0 / 4] += g->f[0xBC / 4] * a;
    g->f[0xB4 / 4] += g->f[0xBC / 4] * b;
    g->f[0xB8 / 4] += g->f[0xBC / 4] * c;
    multiply_matrix(&g->f[0xC0 / 4], x + 0x80, g);
    scale_vector(&g->f[0x100 / 4], x + 0x80, xf[0x1C0 / 4]);
    scale_vector(&g->f[0x110 / 4], x + 0x90, xf[0x1C0 / 4]);
    qcopy(&g->f[0x120 / 4], x + 0xA0);
    qcopy(&g->f[0x130 / 4], x + 0xB0);
    multiply_matrix(&g->f[0x100 / 4], &g->f[0x100 / 4], g);
    qcopy(sky_view_mtx, g);
    qcopy(sky_view_mtx + 0x10, &g->f[0x10 / 4]);
    qcopy(sky_view_mtx + 0x20, &g->f[0x20 / 4]);
    fast_vec_scale(sky_view_mtx + 0x30, &g->f[0x140 / 4], 1024.0f);
    *(f32 *)(sky_view_mtx + 0x3C) = 1024.0f;
}

extern __typeof__(update_camera_view_matrices) func_001F2260 __attribute__((alias("FUN_001f2260")));
