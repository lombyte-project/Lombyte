#ifndef LOMBYTE_RNC_CAMERA_CAMERA_GLOBALS_H
#define LOMBYTE_RNC_CAMERA_CAMERA_GLOBALS_H

#include "types.h"
#include "rnc/math/vector.h"

/* step_camera_blend's two eased blends: position and rotation each
   advance by their rate times frame_scale up to 1. start_camera_blend
   restarts both from the configured rates. */
struct CameraBlend {
    f32 rotation_progress;        /* 0x00 */
    f32 rotation_rate;            /* 0x04 */
    f32 configured_rotation_rate; /* 0x08 */
    f32 position_progress;        /* 0x0C */
    f32 position_rate;            /* 0x10 */
    f32 configured_position_rate; /* 0x14 */
    u8  pad_18[0x8];
    f32 from_rotation[4]; /* 0x20: quaternion */
    f32 from_position[4]; /* 0x30 */
    f32 position[4];      /* 0x40 */
    f32 rotation[4];      /* 0x50: quaternion */
}; /* size 0x60 */

/* advance_alternate_camera_transition's orbit around the focus, from
   orbit_angles toward the angles compute_camera_angles gives. */
struct CameraOrbit {
    f32 angles[3];   /* 0x00: yaw, pitch, distance (compute_camera_angles) */
    s32 frames;      /* 0x0C */
    f32 frame_step;  /* 0x10: 1 / frames */
    s32 start_count; /* 0x14: counted up on each start */
    u8  pad_18[0x8];
    f32 forward[4];        /* 0x20 */
    f32 up[4];             /* 0x30 */
    f32 rotation[4];       /* 0x40: quaternion */
    f32 saved_position[4]; /* 0x50: save_camera_vectors' copy of the blend position */
    f32 saved_rotation[4]; /* 0x60: quaternion, the same for the blend rotation */
}; /* size 0x70 */

/* The camera transition at D_001871B0 (GameCamera 0x270). kind 0 runs
   blend (step_camera_blend); any other kind runs orbit
   (advance_alternate_camera_transition). */
struct CameraTransition {
    s16                mode;           /* 0x00: GameCamera.transition_mode */
    u8                 kind;           /* 0x02 */
    u8                 requested_kind; /* 0x03: becomes kind when a transition starts */
    u8                 pad_04[0xC];
    struct CameraBlend blend; /* 0x10 */
    struct CameraOrbit orbit; /* 0x70 */
}; /* size 0xE0, camera_rows follows */

/* One camera shake channel. apply_camera_shake counts frames down and moves
   the camera along a view row by amplitude, fading out over frames_max. */
struct CameraShake {
    f32 amplitude;  /* 0x0 */
    f32 offset;     /* 0x4: this frame's move */
    s32 frames;     /* 0x8: frames left */
    s32 frames_max; /* 0xC: the longest count since the shake started */
};

/* The active camera (D_00186F40): the view matrices update_camera_view_matrices
   builds, the camera position and basis, and the camera-switch state.
   initialize_level_render_state clears all 0x3A0 bytes. */
struct UpdateCam;

struct GameCamera {
    f32               view_matrices[0x50]; /* 0x000 */
    Vec4              position;            /* 0x140 */
    Vec4              rotation; /* 0x150: basis angles while the camera rows are not published */
    struct CameraShake shake[2];       /* 0x160: apply_camera_shake, one per view row */
    struct UpdateCam *current_camera;  /* 0x180: the record the view follows */
    struct UpdateCam *previous_camera; /* 0x184: start_camera_blend's source */
    u8                pad188[0x258 - 0x188];
    f32  timed_control_step; /* 0x258: advance_timed_camera_control subtracts it from sequence */
    u8   pad25C[0x270 - 0x25C];
    s16  transition_mode; /* 0x270: cam_trans_state's mode; 1/2 start a blend, 3 blends */
    u8   pad272[0x350 - 0x272];
    Vec4 forward; /* 0x350 */
    Vec4 right;   /* 0x360 */
    Vec4 up;      /* 0x370 */
    u8   pad380[0x394 - 0x380];
    s32  underwater;  /* 0x394: camera focus below a water surface */
    s32  frame_count; /* 0x398: update_camera counts its frames */
    u8   pad39C[0x4];
}; /* size 0x3A0 */

extern struct GameCamera game_camera __asm__("D_00186F40");

extern s32                     camera_secondary_mode[] __asm__("D_001413D4");
extern u8                      camera_position[] __asm__("D_00187080");
extern struct CameraTransition cam_trans_state __asm__("D_001871B0");
extern char                    camera_rows[] __asm__("D_00187290");

/* The camera's occlusion state (D_0018C318). */
struct OcclCamState {
    u8  pad_00[0x14];
    s32 position_frozen; /* 0x14: nonzero keeps update_camera and step_camera_blend from
                            writing the camera position and rows */
    u8  pad_18[0x4];
    s32 unk1C; /* 0x1C: cleared by initialize_level_render_state */
};

extern struct OcclCamState occl_cam_state __asm__("D_0018C318");

#endif /* LOMBYTE_RNC_CAMERA_CAMERA_GLOBALS_H */
