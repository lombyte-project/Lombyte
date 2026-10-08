/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off)                                                       \
    typedef char offset_check_##name[((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]

#include "rnc/rendering/view.h"

OFFSET_CHECK(near_clip, struct View, near_clip, 0xA0);
OFFSET_CHECK(fov, struct View, fov, 0xB0);
OFFSET_CHECK(half_width, struct View, half_width, 0x200);
OFFSET_CHECK(fog_near_dist, struct View, fog_near_dist, 0x218);
OFFSET_CHECK(fog_far_int, struct View, fog_far_int, 0x22C);
OFFSET_CHECK(fog_r, struct View, fog_r, 0x230);
