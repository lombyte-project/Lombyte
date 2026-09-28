#include "types.h"

extern void FUN_001f9a28(void *out, void *a, void *b);
extern f32 func_001F9AB0(void *a, void *b);
extern void FUN_001f9bf8(void *out, void *a, f32 len);
extern f32 FUN_001f9af0(void *a);
extern f32 func_001F9DF8(f32);
extern void FUN_00214890(void *out, void *dir, void *axis, f32 ang);

void compute_camera_angles(f32 *out, void *p0, void *p1, void *dir0, void *dir1,
                  void *axis) __asm__("FUN_001ec530");

void compute_camera_angles(f32 *out, void *p0, void *p1, void *dir0, void *dir1,
                  void *axis) {
    f32 diff[4];
    f32 proj[4];
    f32 perp[4];
    f32 unit[4];
    f32 rotated[4];
    f32 d1, d2, d3, d4, d5;
    f32 lenPerp, lenDiff;
    f32 angle1, angle2;
    f32 yaw, pitch;

    FUN_001f9a28(diff, p0, p1);
    d1 = func_001F9AB0(diff, axis);
    FUN_001f9bf8(proj, axis, d1);
    FUN_001f9a28(perp, diff, proj);

    d2 = func_001F9AB0(dir0, perp);
    lenPerp = FUN_001f9af0(perp);
    if (lenPerp == 0.0f) {
        lenPerp = 0.0001f;
    }
    angle1 = func_001F9DF8(d2 / lenPerp);
    yaw = 1.57079637f - angle1;

    FUN_001f9bf8(unit, perp, 1.0f);
    d3 = func_001F9AB0(dir1, unit);
    if (d3 < 0.0f) {
        yaw = -yaw;
    }
    out[0] = yaw;

    FUN_00214890(rotated, dir0, axis, yaw);
    d4 = func_001F9AB0(rotated, diff);
    lenDiff = FUN_001f9af0(diff);
    if (lenDiff == 0.0f) {
        lenDiff = 0.0001f;
    }
    angle2 = func_001F9DF8(d4 / lenDiff);
    pitch = -(1.57079637f - angle2);

    FUN_001f9bf8(unit, diff, 1.0f);
    d5 = func_001F9AB0(axis, unit);
    if (d5 < 0.0f) {
        pitch = 1.57079637f - angle2;
    }
    out[1] = pitch;

    out[2] = FUN_001f9af0(diff);
}

extern __typeof__(compute_camera_angles) func_001EC530 __attribute__((alias("FUN_001ec530")));
