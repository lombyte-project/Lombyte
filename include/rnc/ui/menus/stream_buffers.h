#ifndef LOMBYTE_RNC_UI_MENUS_STREAM_BUFFERS_H
#define LOMBYTE_RNC_UI_MENUS_STREAM_BUFFERS_H

#include "types.h"

enum {
    MENU_STREAM_BUFFER_COUNT = 5,
    MENU_STREAM_BUFFER_LARGE = 1,
    MENU_STREAM_BUFFER_ALLOCATED = 2,
    MENU_STREAM_BUFFER_READ_ACTIVE = 4
};

struct MenuStreamBuffer {
    s32 address;
    s32 flags;
};

extern struct MenuStreamBuffer menu_stream_buffers[MENU_STREAM_BUFFER_COUNT] __asm__("D_001D60B8");

#endif /* LOMBYTE_RNC_UI_MENUS_STREAM_BUFFERS_H */
