#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_LEVEL_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_LEVEL_CAMERA_H

#include "types.h"
#include "rnc/math/vector.h"

struct Moby;
struct UpdateCam;

/* The level's game camera, the level programs' copy of GameCamera (same layout). Turrets and cannons check targets against its
   position, pitch and yaw. A shot sets the two amount/frames pairs (0.03 for
   a few frames), a hard landing sets the first one (0.2 for 40 frames). */
typedef struct LevelCamera {
    f32 view_matrix
        [12]; /* 0x000: GameCamera.view_matrices, a 3x4 basis; shots copy it as their launch rotation */
    u8                pad30[0x110];
    Vec4              pos;        /* 0x140 */
    f32               rotation_x; /* 0x150: GameCamera.rotation x; pitch and yaw are its y and z */
    f32               pitch;      /* 0x154 */
    f32               yaw;        /* 0x158 */
    u8                pad15C[0x4];
    f32               shake_amount;      /* 0x160: the same two channels as GameCamera.shake */
    f32               shake_offset;      /* 0x164 */
    s32               shake_frames;      /* 0x168 */
    s32               shake_frames_max;  /* 0x16C */
    f32               shake2_amount;     /* 0x170 */
    f32               shake2_offset;     /* 0x174 */
    s32               shake2_frames;     /* 0x178 */
    s32               shake2_frames_max; /* 0x17C */
    struct UpdateCam *current_camera;    /* 0x180: the record the view follows */
    struct UpdateCam *previous_camera;   /* 0x184 */
    u8                pad188[0x8];
    u8                smoothed_hero
        [0x30]; /* 0x190: hero position with its z springed, +0x10 the spring step, +0x20 the smoothed up */
    f32 gravity_up[4]; /* 0x1C0: minus the hero's gravity direction */
    u8  tracking
        [0x84]; /* 0x1D0: previous gravity_up, +0x10 up spring steps, +0x20 previous hero position */
    s32 camera_moby; /* 0x254: moby handle_camera_collision_with_hero spawns while the follow camera is current */
    u8 pad258[0x4];
    struct Moby *
        focus_moby; /* 0x25C: a moby that takes the camera's attention (blarg helicopter); 0: none */
    s32 focus_time; /* 0x260: reset to 0 when focus_moby is set */
    u8  pad264[0xc];
    s16 transition_mode; /* 0x270: GameCamera.transition_mode */
    u8  pad272[0x1];
    u8 transition_requested_kind; /* 0x273: CameraTransition.requested_kind (CAMERA_TRANSITION_KIND_*) */
    u8  pad274[0x14];
    f32 transition_rotation_rate; /* 0x288: CameraBlend.configured_rotation_rate */
    u8  pad28C[0x8];
    f32 transition_position_rate; /* 0x294: CameraBlend.configured_position_rate */
    u8  pad298[0x5c];
    s32 transition_orbit_frames; /* 0x2F4: CameraOrbit.length; start_camera_blend adds one */
    u8  pad2F8[0x58];
    f32 rows[12]; /* 0x350: forward, right and up; the hero's input direction is turned by them */
    u8  pad380[0x10];
    void (*unk390)(void); /* 0x390: called each frame by the follow camera when set */
    s32 underwater;       /* 0x394 */
    s32 frame_count;      /* 0x398 */
    u8  pad39C[0x4];
} LevelCamera;

/* Level 0 clears 0x3A0 bytes of it when the level loads. */
extern LevelCamera l00_camera __asm__("D_L00_00166C80");
extern LevelCamera l01_camera __asm__("D_L01_00167100");
extern LevelCamera l02_camera __asm__("D_L02_00167280");
extern LevelCamera l03_camera __asm__("D_L03_00166D80");
extern LevelCamera l05_camera __asm__("D_L05_00167080");
extern LevelCamera l06_camera __asm__("D_L06_00167400");
extern LevelCamera l08_camera __asm__("D_L08_00167480");
extern LevelCamera l11_camera __asm__("D_L11_00167680");
extern LevelCamera l12_camera __asm__("D_L12_00167100");
extern LevelCamera l14_camera __asm__("D_L14_00167380");
extern LevelCamera l15_camera __asm__("D_L15_00167280");
extern LevelCamera l16_camera __asm__("D_L16_00167080");
extern LevelCamera l17_camera __asm__("D_L17_00167580");
extern LevelCamera l18_camera __asm__("D_L18_00167680");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CAMERA_LEVEL_CAMERA_H */
