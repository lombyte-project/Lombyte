#include "types.h"
struct TransferState {
    u8 pad[0x5C];
    u32 current;
    u32 entries[1];
};
extern struct TransferState D_0018CB20;
#define D_00160F0C ((u32 *)0x00160F0C)
extern u32 D_001940C8[4];
extern void update_world_object_animation(void *) __asm__("func_00204790");

void parse_space_scene_chunk(s32 index) __asm__("FUN_002049f0");

void parse_space_scene_chunk(s32 index) {
    struct TransferState *slot;
    slot = (struct TransferState *)((u8 *)&D_0018CB20 - (-(index * 4)));
    D_0018CB20.current = slot->entries[0];
    update_world_object_animation(slot);
    D_0018CB20.current = D_001940C8[0] + D_00160F0C[0];
}
