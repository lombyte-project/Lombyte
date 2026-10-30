#include "rnc/sdk/mpeg_work_area.h"

void init_work_area(struct MpegWorkArea *area, s32 base, s32 size) __asm__("InitializeStateRecord");

void init_work_area(struct MpegWorkArea *area, s32 base, s32 size) {
    area->base = base;
    area->cursor = base;
    area->size = size;
    area->mark = base;
}
