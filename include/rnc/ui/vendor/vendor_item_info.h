#ifndef LOMBYTE_RNC_UI_VENDOR_VENDOR_ITEM_INFO_H
#define LOMBYTE_RNC_UI_VENDOR_VENDOR_ITEM_INFO_H

#include "types.h"

/* Shared 0x18-byte item/price rows. The halfwords at 0x08, 0x0C and
   0x0E are read by the vendor, item grant and ammo-shortfall paths. */
typedef struct {
    s32 f0;                         /* bolt price (scene/menu callers) */
    s32 f4;
    u16 h8;
    u16 hA;
    u16 hC;
    u16 hE;
    u16 h10;
    u16 h12;
    u8 pad14[4];
} VendorItemInfo;

extern VendorItemInfo D_L00_001C40B0[] __asm__("D_L00_001C40B0");

#endif
