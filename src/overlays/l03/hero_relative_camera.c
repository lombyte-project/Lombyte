/* Level 03 hero relative camera type (camera 14): setup behind the hero, smoothed update and the clip volume check. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "qcopy.h"
#include "sda.h"

/* places the camera behind the hero and rebuilds its orientation axes */
void rebuild_hero_relative_camera_transform(char *camera) __asm__("FUN_L03_002eb7e0");

void rebuild_hero_relative_camera_transform(char *camera) {
    char  *camera_data = *(char **)(camera + 0x70);
    float *camera_target_data = (float *)(camera_data + 0xB0);
    float *camera_offset_direction = (float *)(camera_data + 0xF0);
    float *camera_look_direction = (float *)(camera_data + 0x100);
    float  back[4];
    float  side[4];
    float  pos[4];
    float  look[4];
    float  camera_look_displacement[4];

    fast_vec_normalize(side, &hero.gravity_dir, -*(float *)(camera_data + 0xE0));
    fast_vec_normalize(back, camera_offset_direction, -*(float *)(camera_data + 0xD0));
    fast_vec_add(pos, &hero.motion.pos, side);
    fast_vec_add(camera + 0x30, pos, back);
    fast_vec_normalize(look, &hero.gravity_dir, -camera_target_data[5]);
    fast_vec_add(look, &hero.motion.pos, look);
    fast_vec_sub(camera_look_displacement, look, camera + 0x30);
    fast_vec_normalize(camera_look_direction, camera_look_displacement, 1.0f);
    fast_vec_normalize(camera, camera_look_direction, 1.0f);
    fast_vec_normalize(camera + 0x40, camera, 1.0f);
    qcopy(camera, camera + 0x40);
    fast_vec_cross(camera + 0x10, camera, &hero.gravity_dir);
    fast_vec_normalize(camera + 0x10, camera + 0x10, -1.0f);
    fast_vec_cross(camera + 0x20, camera + 0x10, camera);
}

extern s32 D_L03_00167074 NOT_SDA;
/* vendor camera init: aims the camera data at the hero, resets its blend state and places it (FUN_L03_002eb7e0) */
void initialize_hero_relative_camera(void *camera_instance) __asm__("FUN_L03_002eb978");

void initialize_hero_relative_camera(void *camera_instance) {
    char  *camera = camera_instance;
    char  *camera_config = *(char **)((char *)D_L03_0015EF50 + *(s16 *)(camera + 0x84) * 32 + 0x1C);
    char  *target;
    char  *pos;
    char  *camera_data;
    char  *timer;
    char  *camera_back_direction_data;
    int    camera_table_base;
    float *camera_parameter_values;
    int    i;
    float  f;
    float  mtx[12];
    float  camera_rotation_angles[4];

    target = *(char **)(camera + 0x70) + 0xB0;
    *(float *)(target + 0x14) = 1.5f;
    *(struct Moby **)(target + 0x10) = hero.moby;
    qcopy(target, &hero.motion.pos);
    *(s32 *)(target + 0xC) = 0;
    camera_data = *(char **)(camera + 0x70);
    camera_parameter_values = (float *)(camera_data + 0xD0);
    f = *(float *)(camera_config + 0x20);
    camera_parameter_values[2] = f;
    *(float *)(camera_data + 0xD0) = f;
    camera_parameter_values[4] = *(float *)(camera_config + 0x24);
    pos = *(char **)(camera + 0x70);
    *(s32 *)(pos + 0x28) = 0;
    *(float *)(pos + 0x20) = 0.01f;
    *(float *)(pos + 0x24) = 0.2f;
    clear_u64_value(pos + 0x10);
    qcopy(pos, &hero.motion.pos);
    timer = *(char **)(camera + 0x70);
    ((s32 *)timer)[12] = scale_game_frames(0x78);
    camera_table_base = D_L03_0015EF50;
    camera_back_direction_data = *(char **)(camera + 0x70) + 0xF0;
    *(s32 *)(camera_back_direction_data + 0x30) = 0;
    *(s32 *)(camera_back_direction_data + 0x28) = 0;
    *(s32 *)(camera_back_direction_data + 0x2C) = 0;
    *(float *)(camera_back_direction_data + 0x34) = hero.motion.pos.f[2];
    *(s32 *)(camera_back_direction_data + 0x20) = 0;
    *(s32 *)(camera_back_direction_data + 0x24) = 0;
    i = (*(s16 *)(camera + 0x84) << 5) + camera_table_base;
    i += 0x10;
    camera_parameter_values = (float *)i;
    camera_rotation_angles[0] = camera_parameter_values[0];
    camera_rotation_angles[1] = camera_parameter_values[1];
    camera_rotation_angles[2] = camera_parameter_values[2];
    camera_rotation_angles[3] = 0.0f;
    euler_rotation_basis(mtx, camera_rotation_angles);
    qcopy(camera_back_direction_data, mtx);
    rebuild_hero_relative_camera_transform(camera);
    *(s16 *)(camera + 0x7E) = 0;
    D_L03_00167074 = 0x78;
}

extern float cam_interp_values(void *, float, float, float, float, float) __asm__("FUN_001ebd78");
void         update_smoothed_hero_relative_camera_position(void *camera_instance) __asm__("FUN_L03_002ebb00");

void update_smoothed_hero_relative_camera_position(void *camera_instance) {
    char        *camera = camera_instance;
    char        *camera_data = *(char **)(camera + 0x70);
    char        *camera_offset_params = camera_data + 0xD0;
    char        *camera_back_direction_data = camera_data + 0xF0;
    struct Hero *hero_for_height_update = &hero;
    float        camera_back_offset[4];
    float        gravity_offset[4];
    float        camera_position[4];
    struct Hero *hero_ptr;
    if (hero_for_height_update->unk2284 != 0x50 || hero_for_height_update->state.current == 0x11) {
        *(float *)(camera_back_direction_data + 0x34) = hero_for_height_update->motion.pos.f[2];
    }
    fast_vec_normalize(gravity_offset, &hero.gravity_dir, -*(float *)(camera_offset_params + 0x10));
    fast_vec_normalize(
        camera_back_offset, camera_back_direction_data, -*(float *)camera_offset_params);
    hero_ptr = &hero;
    *(float *)camera_data = cam_interp_values(camera_data + 0x10,
                                              *(float *)camera_data,
                                              hero_ptr->motion.pos.f[0],
                                              *(float *)(camera_data + 0x20),
                                              *(float *)(camera_data + 0x24),
                                              *(float *)(camera_data + 0x28));
    *(float *)(camera_data + 4) = cam_interp_values(camera_data + 0x14,
                                                    *(float *)(camera_data + 4),
                                                    hero_ptr->motion.pos.f[1],
                                                    *(float *)(camera_data + 0x20),
                                                    *(float *)(camera_data + 0x24),
                                                    *(float *)(camera_data + 0x28));
    *(float *)(camera_data + 8) = cam_interp_values(camera_data + 0x18,
                                                    *(float *)(camera_data + 8),
                                                    *(float *)(camera_back_direction_data + 0x34),
                                                    *(float *)(camera_data + 0x20),
                                                    *(float *)(camera_data + 0x24),
                                                    *(float *)(camera_data + 0x28));
    fast_vec_add(camera_position, camera_data, gravity_offset);
    fast_vec_add(camera_position, camera_position, camera_back_offset);
    *(float *)(camera + 0x30) = camera_position[0];
    *(float *)(camera + 0x34) = camera_position[1];
    *(float *)(camera + 0x38) = camera_position[2];
}

/* smooths the moby's data floats and rebuilds its orientation axes */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002ED020), where it is exact; names translated to the US level program. */

extern char  camera_look_offset_direction[] __asm__("D_L03_00166F40");
extern short camera_target_height_stiffness __asm__("D_L03_00161E70") __attribute__((sda));
extern short camera_target_height_damping __asm__("D_L03_00161E74") __attribute__((sda));
f32 cam_interp_values(void *p, f32 to, f32 stiffness, f32 damping, f32 max, f32 vel) __asm__("FUN_001ebd78");

void rebuild_camera_orientation_axes(void *camera_instance) __asm__("FUN_L03_002ebc58");

void rebuild_camera_orientation_axes(void *camera_instance) {
    char  *camera = camera_instance;
    char  *camera_data = *(char **)(camera + 0x70);
    float *camera_target_data = (float *)(camera_data + 0xB0);
    float  look_direction[4];
    float  look_point[4];
    float  look_offset[4];
    qcopy(look_point, camera_data);
    camera_target_data[2] = cam_interp_values(camera_data + 0xBC,
                                              camera_target_data[2],
                                              hero.motion.pos.f[2],
                                              *(float *)&camera_target_height_stiffness,
                                              *(float *)&camera_target_height_damping,
                                              0.0f);
    fast_vec_normalize(look_offset, camera_look_offset_direction, camera_target_data[5]);
    fast_vec_add(look_point, look_offset, look_point);
    fast_vec_sub(look_direction, look_point, camera + 0x30);
    fast_vec_normalize(camera, look_direction, 1.0f);
    qcopy(camera + 0x40, camera);
    fast_vec_cross(camera + 0x10, camera, camera_look_offset_direction - 0x10);
    fast_vec_normalize(camera + 0x10, camera + 0x10, 1.0f);
    fast_vec_cross(camera + 0x20, camera + 0x10, camera);
}

/* Returns -1 while the game is in one of two blocking states, else 0. */
int FUN_L03_002ebd70(void) {
    struct Hero *hero_ptr = &hero;

    if ((hero_ptr->unk2284 == 0x50 && hero_ptr->state.current != 0x11) ||
        hero_ptr->state.current == 6) {
        return -1;
    }
    return 0;
}

/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002ED178), where it is exact; names translated to the US level program. */

extern void update_smoothed_hero_relative_camera_position(void *) __asm__("FUN_L03_002ebb00");
extern void rebuild_camera_orientation_axes(void *) __asm__("FUN_L03_002ebc58");
void        noop_main_callback(void *) __asm__("FUN_001e93e8");

void update_camera_position_and_orientation(void *camera) __asm__("FUN_L03_002ebdb0");

void update_camera_position_and_orientation(void *camera) {
    update_smoothed_hero_relative_camera_position(camera);
    noop_main_callback(camera);
    rebuild_camera_orientation_axes(camera);
}
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_002CB280.c: func_L03_002ED1B0), where it is exact; names translated to the US level program. */

void update_camera_state_outside_clip_volume(char *moby) __asm__("FUN_L03_002ebde8");

void update_camera_state_outside_clip_volume(char *moby) {
    char *camera_table_entry = (char *)((*(short *)(moby + 0x84) << 5) + D_L03_0015EF50);
    char *sub = *(char **)(camera_table_entry + 0x1C);
    int   clip_volume_index = *(int *)(sub + 0xC);
    if (clip_volume_index >= 0 &&
        is_point_inside_clip_volume(D_0013E533 + 0xE9D, clip_volume_index) == 0) {
        *(short *)(moby + 0x7E) = 3;
    }
}
