#ifndef LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H
#define LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H

#include "types.h"
#include "rnc/math/vector.h"

struct Moby;

/* The level's game camera. Turrets and cannons check targets against its
   position, pitch and yaw. A shot sets the two amount/frames pairs (0.03 for
   a few frames), a hard landing sets the first one (0.2 for 40 frames). */
typedef struct {
    f32          unk0[12]; /* 0x000: a 3x4 basis; shots copy it as their launch rotation */
    u8           pad30[0x110];
    Vec4         pos; /* 0x140 */
    f32          unk150;
    f32          pitch; /* 0x154 */
    f32          yaw;   /* 0x158 */
    u8           pad15C[4];
    f32          shake_amount; /* 0x160 */
    u8           pad164[4];
    s32          shake_frames; /* 0x168 */
    u8           pad16C[4];
    f32          shake2_amount; /* 0x170 */
    u8           pad174[4];
    s32          shake2_frames; /* 0x178 */
    u8           pad17C[4];
    struct Moby *unk180; /* 0x180: the camera moby (its pvars hold the follow camera state) */
    char        *unk184; /* 0x184 */
    u8           pad188[0x38];
    f32          unk1C0[4]; /* 0x1C0 */
    u8           pad1D0[0x84];
    s32          unk254; /* 0x254 */
    u8           pad258[4];
    struct Moby *
        focus_moby; /* 0x25C: a moby that takes the camera's attention (blarg helicopter); 0: none */
    s32 focus_time; /* 0x260: reset to 0 when focus_moby is set */
    u8  pad264[0xC];
    s16 unk270; /* 0x270 */
    u8  pad272;
    u8  unk273; /* 0x273 */
    u8  pad274[0x14];
    f32 unk288; /* 0x288 */
    u8  pad28C[8];
    f32 unk294; /* 0x294 */
    u8  pad298[0xB8];
    f32 unk350[12]; /* 0x350: basis the hero's input direction is turned by */
    u8  pad380[0x10];
    void (*unk390)(void); /* 0x390: called each frame by the follow camera when set */
    u8 pad394[0xC];
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

#endif /* LOMBYTE_RNC_OVERLAY_LEVEL_CAMERA_H */
