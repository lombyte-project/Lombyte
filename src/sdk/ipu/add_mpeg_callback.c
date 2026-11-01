#include "types.h"

s32 AddMpegCallback(u8 *decoder, s32 index, s32 callback, s32 callback_data) {
    u8 *callback_storage_base = *(u8 **)(decoder + 0x40);
    u8 *callback_slot = callback_storage_base + 0xC;
    s32 callback_entry_offset = index << 3;
    s32 previous_callback;

    callback_slot += callback_entry_offset;
    callback_storage_base += callback_entry_offset;
    ((u32 *)callback_storage_base)[4] = callback_data;
    previous_callback = ((u32 *)callback_slot)[0];
    ((u32 *)callback_slot)[0] = callback;
    return previous_callback;
}
