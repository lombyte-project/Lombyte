#include "types.h"
#include "rnc/mobys/moby.h"

/* An animation sequence as loaded: its offsets are relative to the
   sequence itself until this turns them into addresses. */
struct LoadedAnimSeq {
    u8  pad_00[0x10];
    u8  frame_count; /* 0x10 */
    u8  pad_11[0x3];
    u32 optional_data; /* 0x14: offset, 0 = none */
    u8  pad_18[0x4];
    u32 frames[1]; /* 0x1C: one offset per frame */
};

void relocate_asset_entry_pointers(u8 *table_container,
                                   s32 animation_index,
                                   s32 on) __asm__("FUN_002032e0");

void relocate_asset_entry_pointers(u8 *table_container, s32 animation_index, s32 on) {
    struct LoadedAnimSeq *seq;
    u32                   optional_data_offset;

    seq = (struct LoadedAnimSeq *)((struct MobyClass *)table_container)->seqs[animation_index];
    optional_data_offset = seq->optional_data;
    if (optional_data_offset != 0) {
        seq->optional_data = (u32)((u8 *)seq + optional_data_offset);
    }
    if (seq->frame_count != 0) {
        s32  i = 0;
        u32 *relocation_slot = seq->frames;
        do {
            *relocation_slot = (u32)((u8 *)seq + *relocation_slot);
            i++;
            relocation_slot++;
        } while (i < seq->frame_count);
    }
}
