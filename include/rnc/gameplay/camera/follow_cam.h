#ifndef LOMBYTE_RNC_GAMEPLAY_CAMERA_FOLLOW_CAM_H
#define LOMBYTE_RNC_GAMEPLAY_CAMERA_FOLLOW_CAM_H

#include "types.h"
#include "rnc/gameplay/camera/update_cam.h"

/*
 * State of the follow camera: the saved state of its camera record
 * (UpdateCam.saved_state, +0x70). follow_cam_focus_velocity writes `velocity`,
 * follow_camera_transition reads `follow` for its spring constants, and
 * fast_dec_timer counts `blend_timer` down. The follow_camera_set_* setters
 * (FUN_L00_002e8450..002e8680) each write one field and do nothing while the
 * record's unk86 is set. The shot cameras (volume_camera_swing, volume_camera_aim)
 * snap every vector at once when a shot starts.
 */
typedef struct {
    f32 focus[4];          /* 0x40: point the camera looks at, moved by velocity each frame */
    f32 velocity[4];       /* 0x50: focus displacement for this frame */
    u8 pad60[0x20];
    f32 look_target[4];    /* 0x80: focus offset blended towards the hero */
    f32 eye[4];            /* 0x90: resulting camera position */
    f32 offset[4];         /* 0xA0: eased offset, split into a lateral and an up part */
    f32 lateral_ease[4];   /* 0xB0: cam_interp_values state of the lateral part */
    f32 up_ease[4];        /* 0xC0: cam_interp_values state of the up part */
    f32 unkD0[4];          /* 0xD0: set to look_target when a shot camera snaps */
    u8 padE0[0x10];
    f32 look_height;       /* 0xF0: height of look_target above the hero */
    u8 padF4[0x22];
    u8 blending_in;        /* 0x116: nonzero while blend_timer counts up */
    u8 pad117;
    s16 blend_timer;       /* 0x118: frames into the focus blend */
    u8 pad11A[2];
    f32 lateral_stiffness; /* 0x11C */
    f32 lateral_damping;   /* 0x120 */
    f32 up_stiffness;      /* 0x124 */
    f32 up_damping;        /* 0x128 */
    u8 pad12C[4];
} FollowCamBlock;

/* 0x1A8: FUN_L00_002e8680 and the shot cameras reach `flags` through it. */
typedef struct {
    u8 pad1A8[8];
    f32 unk1B0;            /* 0x1B0: aim cameras start only while it is (nearly) 0 */
    u8 pad1B4[4];
    s32 flags;             /* 0x1B8: bits 0 and 1 set by the shot cameras and FUN_L00_002e8680 */
} FollowCamFlags;

/* 0x1D0: written when a shot camera snaps. */
typedef struct {
    u8 pad1D0[0x20];
    f32 unk1F0[4];         /* 0x1F0: set to the record's position */
    f32 unk200;            /* 0x200: unk15C minus the length of unk130 */
    u8 pad204[8];
} FollowCamSnap;

typedef struct {
    f32 unk130[4];         /* 0x130: eye offset; its length is the eye distance */
    f32 unk140[4];         /* 0x140: copy of unk130 */
    u8 pad150[0xC];
    f32 unk15C;            /* 0x15C: eye distance, set with unk130 by the shot cameras */
    f32 distance;          /* 0x160: distance of the eye behind the hero */
    f32 unk164;            /* 0x164: aim cameras start only while it is (nearly) 0 */
    u8 pad168[0x40];
    FollowCamFlags ctl;    /* 0x1A8 */
    u8 pad1BC[0x10];
    f32 unk1CC;            /* 0x1CC: set by FUN_L00_002e8540 */
    FollowCamSnap snap;    /* 0x1D0 */
    f32 zoom_scale;        /* 0x20C */
    f32 zoom;              /* 0x210: 1.0 is no zoom */
    f32 zoom_range;        /* 0x214 */
    u8 pad218[0xE];
    s16 unk226;            /* 0x226: set to 1 by FUN_L00_002e8620 */
    u8 pad228[4];
    f32 unk22C;            /* 0x22C: set by FUN_L00_002e8490 */
    s32 mode;              /* 0x230: 0 normal; shot cameras of type 10 and 11 set 0x14D and 2 */
} FollowCamTail;

/* 0x20..0x3B: eased values reset when a shot camera snaps. */
typedef struct {
    u8 pad20[2];
    s16 unk22;             /* 0x22 */
    f32 look_height;       /* 0x24: copy of follow.look_height */
    f32 distance;          /* 0x28: copy of tail.distance */
    f32 unk2C;             /* 0x2C */
    f32 unk30;             /* 0x30 */
    f32 unk34;             /* 0x34 */
    f32 unk38;             /* 0x38 */
    u8 pad3C[4];
} FollowCamReset;

typedef struct FollowCamState {
    f32 position[4];       /* 0x00: the record's position when a shot camera snaps */
    s16 unk10;             /* 0x10: set by FUN_L00_002e8568 */
    u8 pad12[0xE];
    FollowCamReset reset;  /* 0x20 */
    FollowCamBlock follow; /* 0x40 */
    FollowCamTail tail;    /* 0x130 */
} FollowCamState;

#define FOLLOW_CAM_STATE(cam) ((FollowCamState *)(cam)->saved_state)

/* The camera record the follow camera code works on. */
extern struct UpdateCam *current_follow_cam __asm__("D_L00_00166E00") __attribute__((section(".data")));

void follow_camera_set_distance(s32 add, f32 distance, f32 rate) __asm__("FUN_L00_002e8450");
void follow_camera_set_height(f32 height, f32 rate) __asm__("FUN_L00_002e84b8");
void follow_camera_set_offset_scale(s32 add, f32 scale, f32 rate) __asm__("FUN_L00_002e84f0");
void follow_camera_start_slide(void) __asm__("FUN_L00_002e8648");
/* Write follow.lateral_stiffness/damping and up_stiffness/damping; 0 keeps a value. */
void follow_camera_set_lateral_spring(f32 stiffness, f32 damping) __asm__("FUN_L00_002e8590");
void follow_camera_set_up_spring(f32 stiffness, f32 damping) __asm__("FUN_L00_002e85d8");

#endif /* LOMBYTE_RNC_GAMEPLAY_CAMERA_FOLLOW_CAM_H */
