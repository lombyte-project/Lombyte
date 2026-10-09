#ifndef LOMBYTE_RNC_RENDERING_IMAGE_CLEAR_BUFFER_H
#define LOMBYTE_RNC_RENDERING_IMAGE_CLEAR_BUFFER_H

#include "types.h"

/* Pixels uploaded to the GS to clear it: set_pal_mode fills all 0x1000 bytes (one
   32x32 PSMCT32 tile) with zero, init_once the first 0x100 (8x8) with 0x80808080.
   FillTransferWords counts bytes. */
extern u8 image_clear_buffer[0x1000] __asm__("D_001941C0");

#endif /* LOMBYTE_RNC_RENDERING_IMAGE_CLEAR_BUFFER_H */
