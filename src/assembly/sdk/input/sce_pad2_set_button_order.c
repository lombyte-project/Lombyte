#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/input/sce_pad2_set_button_order/scePad2SetButtonOrder.s", scePad2SetButtonOrder);
#else
#include "types.h"
struct Pad2Capability {
    s32 present;
    s32 width;
    s32 byteIndex;
    s32 bitIndex;
};

/* Decode forty LSB-first presence bits into sixteen-byte capability records.
 * Absent entries leave the width field untouched, as in the retail code. */
s32 scePad2SetButtonOrder(u8 *profile, s32 *order) {
    s32 capabilityIndex;
    s32 bitIndex;
    s32 bitPosition;
    s32 byteIndex;
    u8 *profileByte;
    struct Pad2Capability *capability;

    profileByte = profile;
    capability = (struct Pad2Capability *)order;
    byteIndex = 0;
    capabilityIndex = 0;
    bitPosition = 0;
    bitIndex = 0;
    do {
        if (((s32)*profileByte >> bitPosition) & 1) {
            capability->byteIndex = byteIndex;
            capability->present = 1;
            capability->bitIndex = bitIndex;
            if ((u32)(capabilityIndex - 0x10) < 0x10U) {
                capability->width = 8;
                byteIndex += 1;
            } else if ((u32)(capabilityIndex - 0x23) < 4U) {
                capability->width = 8;
                byteIndex += 1;
            } else {
                bitIndex += 1;
                capability->width = 1;
                if (!(bitIndex & 7)) {
                    byteIndex += 1;
                    bitIndex = 0;
                }
            }
        } else {
            capability->present = 0;
            capability->byteIndex = 0;
            capability->bitIndex = 0;
        }
        bitPosition += 1;
        capabilityIndex += 1;
        if (!(bitPosition & 7)) {
            profileByte += 1;
            bitPosition = 0;
        }
        capability += 1;
    } while (capabilityIndex < 0x28);
    return 1;
}

#endif /* NON_MATCHING */
