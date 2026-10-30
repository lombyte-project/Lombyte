#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_H

#include "types.h"

/* Vendor menu records: shared by src/overlays/shared/vendor/vendor_menu.c and the vendor data
 * that src/overlays/shared/cutscenes/update_cutscene_playback.c defines. */

typedef struct {
    s32 item;
    s32 selected;
    s32 state;
    s32 unused;
    s32 sound;
} VendorListEntry;

typedef struct {
    u8              pad[0xD0];
    VendorListEntry entries[16];
    s32             count;
} VendorList;

typedef struct {
    s32 sound;
    u8  pad4[0xC];
    s32 message;
} VendorSound;

typedef struct {
    u8  b0;
    u8  hidden;
    u8  pad2[0x32];
    f32 offset;
    u8  pad38[8];
} VendorSlot;

void open_vendor_menu(void *) __asm__("FUN_L00_0029a6b0");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_H */
