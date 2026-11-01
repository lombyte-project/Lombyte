#ifndef LOMBYTE_RNC_CAMERA_H
#define LOMBYTE_RNC_CAMERA_H

#include "types.h"

struct CameraOrbit;
struct CameraPath;
struct CameraProbePosition;

int  advance_alternate_camera_transition(char *, struct CameraOrbit *) __asm__("FUN_001eccd8");
void advance_timed_camera_control(void) __asm__("FUN_001eda60");
f32  cam_interp_values(f32, f32, f32, f32, f32, f32 *) __asm__("FUN_001ebd78");
f32  resolve_camera_surface_height(struct CameraProbePosition *, s32) __asm__("FUN_002135f0");
void sample_camera_path(struct CameraPath *, s32, void *, f32 *, s32, f32) __asm__("FUN_00214e58");
s32  update_all_cameras(void) __asm__("FUN_001ec420");
void update_camera_blend(char *) __asm__("FUN_001ed2b0");
void update_camera_environment_from_regions(void *) __asm__("FUN_001ee4b0");

#endif /* LOMBYTE_RNC_CAMERA_H */
