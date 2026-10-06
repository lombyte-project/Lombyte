#include "rnc/ui/menus/item_preview/preview_animation.h"
#include "sda.h"

/* Retail forms the queue head from the address one request before it. */
extern PreviewAnimationRequest preview_request_address_base[] __asm__("D_001D5E88");
extern s32 preview_request_count __asm__("D_00160350");
extern s32 preview_request_count_absolute __asm__("D_00160350") MACRO_ADDR;
s32 advance_preview_animation_queue(void) __asm__("FUN_00226670");

s32 advance_preview_animation_queue(void) {
    s32 request_address;
    s32 end_address;
    s32 base_address;

    base_address = (s32)&preview_request_address_base[0];
    request_address = base_address + 0x38;
    end_address = base_address + 0x1C0;
    do {
        *(PreviewAnimationRequest *)request_address =
            *(PreviewAnimationRequest *)(request_address + 0x38);
        request_address += 0x38;
    } while (request_address < end_address);
    preview_request_count = preview_request_count_absolute - 1;
    return 0;
}

extern __typeof__(advance_preview_animation_queue) func_00226670
    __attribute__((alias("FUN_00226670")));
