/* Compile-time EE layout checks; never linked into the game. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/gameplay/camera/camera_spring.h"

SIZE_CHECK(spring, CameraSpring, 0x20);
OFFSET_CHECK(dist, CameraSpring, dist, 0);
OFFSET_CHECK(unk4, CameraSpring, unk4, 4);
OFFSET_CHECK(height, CameraSpring, height, 0x10);
