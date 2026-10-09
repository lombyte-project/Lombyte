#ifndef LOMBYTE_RNC_UI_VENDOR_VENDOR_CAPTURE_H
#define LOMBYTE_RNC_UI_VENDOR_VENDOR_CAPTURE_H

#include "types.h"

struct VendorItemPricing {
    s32 purchase_price;
    s32 discounted_purchase_price;
    u16 ammo_price;
    u16 discounted_ammo_price;
    u16 pad0C;
    u16 ammo_capacity;
    u8 pad10[8];
};

struct CaptureBoundsAdjustment {
    f32 first_origin;
    f32 second_origin;
    f32 first_extent;
    f32 second_extent;
};

extern struct VendorItemPricing vendor_item_prices[43] __asm__("D_001DFFB0");
extern volatile s32 capture_glyph_coordinates[64] __asm__("D_001E6018");
extern s32 capture_glyph_advances[64] __asm__("D_001E6118");
extern struct CaptureBoundsAdjustment capture_bounds_adjustments[6] __asm__("D_001E6218");

#endif /* LOMBYTE_RNC_UI_VENDOR_VENDOR_CAPTURE_H */
