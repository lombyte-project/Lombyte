#include "types.h"

struct TransferState {
    u8  pad[0x5C];
    u32 current;
    u32 entries[1];
};

extern struct TransferState render_sequence __asm__("D_0018CB20");
#define current_vu_chain ((u32 *)0x00160F0C)
extern u32  D_001940C8[4];
extern void update_world_object_animation(void *) __asm__("func_00204790");

void parse_space_scene_chunk(s32 index) __asm__("FUN_002049f0");

void parse_space_scene_chunk(s32 index) {
    struct TransferState *slot;
    slot = (struct TransferState *)((u8 *)&render_sequence - (-(index * 4)));
    render_sequence.current = slot->entries[0];
    update_world_object_animation(slot);
    render_sequence.current = D_001940C8[0] + current_vu_chain[0];
}
