#ifndef LOMBYTE_RNC_RENDERING_GRAPHICS_SETUP_H
#define LOMBYTE_RNC_RENDERING_GRAPHICS_SETUP_H

#include "types.h"

/* GIF packets that program the GS for the NTSC and PAL video modes. */
extern u8 ntsc_graphics_setup_packet[0x70] __asm__("D_001D7E50");
extern u8 pal_graphics_setup_packet[0x70] __asm__("D_001D7EC0");

#endif /* LOMBYTE_RNC_RENDERING_GRAPHICS_SETUP_H */
