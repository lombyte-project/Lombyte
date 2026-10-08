/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/overlay/collision.h"
#include "rnc/overlay/entities.h"

OFFSET_CHECK(hit_moby, CollisionHit, moby, 0x18);
OFFSET_CHECK(hit_point, CollisionHit, point, 0x20);
OFFSET_CHECK(hit_normal_x, CollisionHit, normal_x, 0x40);
OFFSET_CHECK(hit_normal_z, CollisionHit, normal_z, 0x48);
OFFSET_CHECK(link_pos, OvlMobyEntry40, pos, 0x10);
OFFSET_CHECK(link_target, OvlMobyEntry40, target, 0x20);
OFFSET_CHECK(link_unk24, OvlMobyEntry40, unk24, 0x24);
OFFSET_CHECK(link_unk28, OvlMobyEntry40, unk28, 0x28);
OFFSET_CHECK(link_flags, OvlMobyEntry40, flags, 0x30);
OFFSET_CHECK(link_owner, OvlMobyEntry40, owner, 0x34);
SIZE_CHECK(moby_link, OvlMobyEntry40, 0x40);
SIZE_CHECK(ovl_vec4, OvlVec4, 0x10);
