/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off)                                                       \
    typedef char offset_check_##name[((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size)                                                               \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/rendering/fs_aa_buffer.h"

SIZE_CHECK(gif_tag, struct GifTag, 0x10);
SIZE_CHECK(load_image, sceGsLoadImage, 0x60);
OFFSET_CHECK(giftag0, struct FsAaBuf, giftag0, 0x30);
OFFSET_CHECK(draw0, struct FsAaBuf, draw0, 0x40);
OFFSET_CHECK(giftag1, struct FsAaBuf, giftag1, 0xC0);
OFFSET_CHECK(display_width, struct FsAaBuf, display_width, 0x150);
OFFSET_CHECK(storage_width, struct FsAaBuf, storage_width, 0x158);
OFFSET_CHECK(reserved170, struct FsAaBuf, reserved170, 0x170);
