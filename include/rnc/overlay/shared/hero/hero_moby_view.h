#ifndef LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_MOBY_VIEW_H
#define LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_MOBY_VIEW_H

#include "types.h"
#include "rnc/math/vector.h"

/* The pvars of a swingshot target (Hero.swing_target_moby), as the level copies of the
   hero motion update read them while the hero swings. */
struct SwingTargetVars {
    u8  pad0[0xC];
    f32 max_speed; /* 0x0C: the swing speed is eased down to it */
    u8  pad10[0x24];
    s32 align_yaw; /* 0x34: nonzero eases Hero.swing_yaw toward the target's rot.z */
};

/* A moby as the level copies of the hero motion update read it: the layout and names of
   struct Moby (rnc/mobys/moby.h). The unions keep the single-float loads of pos and rot. */
struct MobyView {
    u8 pad0[0x10];
    union {
        Vec4 v;
        f32  x;
        struct {
            u8  pad[0x4];
            f32 v;
        } y;
        struct {
            u8  pad[0x8];
            f32 v;
        } z;
    } pos; /* 0x10 */
    u8  pad20[0x3];
    u8  alpha; /* 0x23 */
    u8  pad24[0xC];
    u8  update_dist; /* 0x30 */
    u8  visible;
    s16 draw_dist;
    u16 flags; /* 0x34 */
    u8  pad36[0xA];
    union {
        Vec4 v;
        struct {
            u8  pad[0x8];
            f32 v;
        } z;
    } rot; /* 0x40 */
    u8                      frame;
    u8                      prev_frame;
    u8                      seq;
    u8                      prev_seq;
    u8                      pad54[0x1C];
    u8                      anim_flags; /* 0x70 */
    u8                      pad71[0x7];
    struct SwingTargetVars *pvars; /* 0x78: only read on the swing target */
    u8                      pad7C[0x2A];
    s16                     oclass; /* 0xA6 */
    u8                      padA8[0x14];
    u8                      command; /* 0xBC */
    u8                      padBD[0x3];
    Vec4                    rot_row0; /* 0xC0 */
};

#define MV(p) ((struct MobyView *)(p))

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_HERO_HERO_MOBY_VIEW_H */
