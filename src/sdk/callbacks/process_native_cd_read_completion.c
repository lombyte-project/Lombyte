#include "types.h"
#include "asm.h"

#include "types.h"

extern s32 D_00131314[];
extern void handle_cd_read_callback(s32 *descriptor) __asm__("FUN_001206d8");

void process_native_cd_read_completion(void *descriptor) __asm__("FUN_00120788");

void process_native_cd_read_completion(void *descriptor) {
    u8 *packet = (u8 *)((u32)descriptor | 0x20000000);
    u8 *source;
    u8 *destination;
    s32 i;

    if (*(s32 *)(packet + 0) > 0) {
        destination = *(u8 **)(packet + 8);
        i = 0;
        if (i < *(s32 *)(packet + 0)) {
            source = packet + 0x10;
            do {
                destination[i] = source[i];
                i++;
            } while (i < *(s32 *)(packet + 0));
        }
    }
    if (*(s32 *)(packet + 4) > 0) {
        destination = *(u8 **)(packet + 0xc);
        i = 0;
        if (i < *(s32 *)(packet + 4)) {
            source = packet + 0x50;
            do {
                destination[i] = source[i];
                i++;
            } while (i < *(s32 *)(packet + 4));
        }
    }
    handle_cd_read_callback(D_00131314);
}
extern __typeof__(process_native_cd_read_completion) D_00120788
    __attribute__((alias("FUN_00120788")));
extern __typeof__(process_native_cd_read_completion) func_00120788
    __attribute__((alias("FUN_00120788")));
