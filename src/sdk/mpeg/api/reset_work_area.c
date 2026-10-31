#include "rnc/sdk/mpeg_work_area.h"

int reset_work_area(struct MpegWorkArea *area) __asm__("func_0012BC10");

int reset_work_area(struct MpegWorkArea *area) {
    int mark = area->mark;

    area->cursor = mark;
    return mark;
}
