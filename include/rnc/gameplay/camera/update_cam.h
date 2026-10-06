#ifndef LOMBYTE_RNC_GAMEPLAY_CAMERA_UPDATE_CAM_H
#define LOMBYTE_RNC_GAMEPLAY_CAMERA_UPDATE_CAM_H

#include "types.h"
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

#endif /* LOMBYTE_RNC_GAMEPLAY_CAMERA_UPDATE_CAM_H */
