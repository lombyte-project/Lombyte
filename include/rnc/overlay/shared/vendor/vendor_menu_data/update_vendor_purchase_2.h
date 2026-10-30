#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_DATA_UPDATE_VENDOR_PURCHASE_2_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_DATA_UPDATE_VENDOR_PURCHASE_2_H

#include "types.h"

/* Scalar data for src/overlays/shared/vendor/vendor_menu.c.
 * Keep its include at this declaration block: SN small-data ordering matters. */
extern s32 vendor_auxiliary_moby_local_offset_gp __asm__("D_L00_00161080") __attribute__((sda));
extern s32 vendor_item_display_common_local_offset_gp __asm__("D_L00_001610A0")
    __attribute__((sda));
extern s32 vendor_rotating_display_local_offset_gp __asm__("D_L00_001610B0") __attribute__((sda));
extern s32 vendor_menu_rotating_display_local_offset_gp __asm__("D_L00_001610C0")
    __attribute__((sda));
extern s32 vendor_close_delay_frames_remaining __asm__("D_L00_00161E14");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VENDOR_VENDOR_MENU_DATA_UPDATE_VENDOR_PURCHASE_2_H */
