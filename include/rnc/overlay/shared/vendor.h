#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_H

#include "types.h"

void draw_vendor_screen(void) __asm__("FUN_L00_002a02b0");
void pool_splash(int, int, int, float, float, float, float) __asm__("FUN_L00_002a3ec8");
void draw_vendor_purchase_text(void *, s32, s32) __asm__("FUN_L00_0029ead8");
void flag_vendor_item(int) __asm__("FUN_L13_00316f48");
void vendor_update() __asm__("FUN_L00_002a57a8");
void vendor_orb_update(unsigned char *) __asm__("FUN_L00_002df730");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_H */
