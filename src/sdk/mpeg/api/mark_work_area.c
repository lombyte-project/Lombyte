#include "rnc/sdk/mpeg_work_area.h"

int mark_work_area(struct MpegWorkArea *area) __asm__("func_0012BC00");

int mark_work_area(struct MpegWorkArea *area) {
    int cursor = area->cursor;

    area->mark = cursor;
    return cursor;
}
