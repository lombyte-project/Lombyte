#ifndef RNC_DEBUG_MENU_TYPES_H
#define RNC_DEBUG_MENU_TYPES_H

/* Descriptive names recovered from retail menu labels and consumers. */
#include "types.h"
typedef struct {
    s32 unknown_00;
    s32 draw_flags;
    s32 update_flags;
    s32 selected_column;
    s32 selected_row;
    s32 control_mode;
    s32 profiler_mode;
    s32 occlusion_mode;
    s32 bookmark_enabled;
    s32 invincibility_enabled;
    s32 draw_distance_override;
    s32 collision_mode;
    s32 television_mode;
    s32 screen_mode;
    s32 scene_index;
    s32 segment_index;
    s32 unknown_40;
    s32 unknown_44;
    s32 unknown_48;
    s32 unknown_4c;
    s32 unknown_50;
    s32 unknown_54;
    s32 unknown_58;
    s32 unknown_5c;
    s32 unknown_60;
    s32 unknown_64;
    s32 unknown_68;
    s32 unknown_6c;
    f32 movement_speed;
    f32 vertical_speed;
    f32 roll_speed;
    f32 pitch_speed;
    f32 yaw_speed;
    f32 target_yaw;
    f32 target_angle;
    f32 target_distance;
    f32 target_height;
    s32 unknown_94;
    s32 unknown_98;
    s32 unknown_9c;
    s32 unknown_a0;
    s32 unknown_a4;
    s32 unknown_a8;
    s32 unknown_ac;
    s32 unknown_b0;
    s32 unknown_b4;
    s32 unknown_b8;
    s32 unknown_bc;
    s32 unknown_c0;
    s32 unknown_c4;
    s32 unknown_c8;
    s32 unknown_cc;
    s32 unknown_d0;
    s32 unknown_d4;
    s32 unknown_d8;
    s32 unknown_dc;
    s32 unknown_e0;
    s32 unknown_e4;
    s32 unknown_e8;
    s32 unknown_ec;
    s32 battle_camera_enabled;
    s32 actuator_enabled;
    u8 start_capture_status;
    u8 sample_capture_status;
} DebugMenuState;

typedef struct {
    f32 basis[12];
    f32 position[4];
    f32 previous_basis[4];
    u8 reserved_50[0x14];
    f32 camera_position[3];
} DebugCameraTarget;
typedef struct {
    f32 position[4];
    f32 angles[4];
    u8 reserved_20[0x20];
    DebugCameraTarget *target;
    u8 reserved_44[0x1CC];
    f32 basis[12];
} DebugCameraState;
extern DebugMenuState g_debug_menu __asm__("D_L00_0016C058");
extern DebugCameraState g_debug_camera __asm__("D_L00_00166DC0");

#endif
