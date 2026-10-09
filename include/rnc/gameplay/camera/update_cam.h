#ifndef LOMBYTE_RNC_GAMEPLAY_CAMERA_UPDATE_CAM_H
#define LOMBYTE_RNC_GAMEPLAY_CAMERA_UPDATE_CAM_H

#include "types.h"
#include "sda.h"
#include "eetypes.h"
#include "rnc/math/vector.h"

/* One camera record. The name comes from the recovered symbols
   (Camera_runSetupToNewCam__FP9UpdateCam and friends). Records live in
   D_00187410[48]; D_001870C0[0] and [1] point at the current and previous
   one. Size 0xA0 (backup_current_cam copies 0xA0 bytes). */
struct UpdateCam {
    u128 m0;
    u128 m1;
    u128 m2;
    Vec4f pos;
    u8 pad40[0x24];
    Vec3 prev_pos;
    void *saved_state;
    u8 state;                /* activation rule, see camera_activation_check_priority */
    u8 pad75[3];
    f32 transition_duration;
    u8 active;
    u8 handoff_state;
    s16 transition_state;
    u8 pad80[4];
    s16 descriptor_index;
    s16 unk86;               /* 0 lets the hero collision moby spawn; 6 disables shake */
    u8 pad88[4];
    s16 type;                /* index into camera_types (D_001E8C00) */
    s16 activation_blocked;
    u8 pad90[0x10];
};

/* Per camera type callbacks, indexed by UpdateCam.type. */
struct CameraType {
    void *unk0;
    int (*check_priority)(void *cur, void *other);
    void (*run_setup)(struct UpdateCam *cam);
    void (*update)(struct UpdateCam *cam);
    void (*exit)(struct UpdateCam *cam);
};

/* The saved state of a camera record: 0x280 bytes. Three of them sit back to back
   from D_001893D0: switch_active_camera_record keeps the previous camera's state
   in [1] and the next one's in [0]; backup_current_cam copies [0] into [2]. */
struct CameraSavedState {
    u8 bytes[0x280];
};

extern struct CameraSavedState camera_saved_states[3] __asm__("D_001893D0");

/* Nonzero stops the camera code from publishing the camera position
   (update_camera_blend then skips its vector copy). */
extern s32 camera_position_publication_suppressed[1] __asm__("D_0018C32C") NOT_SDA;

#endif /* LOMBYTE_RNC_GAMEPLAY_CAMERA_UPDATE_CAM_H */
