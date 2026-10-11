/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/gameplay/camera/follow_cam.h"

OFFSET_CHECK(reset, FollowCamState, reset, 0x20);
OFFSET_CHECK(reset_distance, FollowCamState, reset.distance, 0x28);
OFFSET_CHECK(reset_unk38, FollowCamState, reset.unk38, 0x38);
OFFSET_CHECK(follow, FollowCamState, follow, 0x40);
OFFSET_CHECK(unkD0, FollowCamState, follow.unkD0, 0xD0);
OFFSET_CHECK(look_height, FollowCamState, follow.look_height, 0xF0);
OFFSET_CHECK(blend_timer, FollowCamState, follow.blend_timer, 0x118);
OFFSET_CHECK(up_damping, FollowCamState, follow.up_damping, 0x128);
OFFSET_CHECK(tail, FollowCamState, tail, 0x130);
OFFSET_CHECK(unk15C, FollowCamState, tail.unk15C, 0x15C);
OFFSET_CHECK(distance, FollowCamState, tail.distance, 0x160);
OFFSET_CHECK(ctl, FollowCamState, tail.ctl, 0x1A8);
OFFSET_CHECK(flags, FollowCamState, tail.ctl.flags, 0x1B8);
OFFSET_CHECK(unk1CC, FollowCamState, tail.unk1CC, 0x1CC);
OFFSET_CHECK(snap, FollowCamState, tail.snap, 0x1D0);
OFFSET_CHECK(unk200, FollowCamState, tail.snap.unk200, 0x200);
OFFSET_CHECK(zoom_scale, FollowCamState, tail.zoom_scale, 0x20C);
OFFSET_CHECK(zoom_range, FollowCamState, tail.zoom_range, 0x214);
OFFSET_CHECK(unk226, FollowCamState, tail.unk226, 0x226);
OFFSET_CHECK(unk22C, FollowCamState, tail.unk22C, 0x22C);
OFFSET_CHECK(mode, FollowCamState, tail.mode, 0x230);
SIZE_CHECK(follow_cam_state, FollowCamState, 0x234);
