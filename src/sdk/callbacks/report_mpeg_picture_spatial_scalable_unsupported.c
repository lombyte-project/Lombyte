#include "types.h"
#include "asm.h"

extern char D_00153B90[];
extern void _Error(void *pState, const void *message);

void report_mpeg_picture_spatial_scalable_unsupported(void *pState) __asm__("FUN_0012cb10");

void report_mpeg_picture_spatial_scalable_unsupported(void *pState) {
    _Error(pState, D_00153B90);
}
