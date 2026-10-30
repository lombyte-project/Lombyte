#include "rnc/ui/menus/stream_buffers.h"

#define OFFSET_CHECK(field, offset)               \
    typedef char check_menu_stream_buffer_##field \
        [((unsigned long)&((struct MenuStreamBuffer *)0)->field == (offset)) ? 1 : -1]

OFFSET_CHECK(address, 0x0);
OFFSET_CHECK(flags, 0x4);
typedef char check_menu_stream_buffer_size[sizeof(struct MenuStreamBuffer) == 0x8 ? 1 : -1];
typedef char check_menu_stream_buffers_size[sizeof(menu_stream_buffers) == 0x28 ? 1 : -1];
