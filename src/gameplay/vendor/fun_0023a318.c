#include "types.h"
extern u8 D_00160ED0[];
extern void func_0020CD48();
extern void func_001F9A28();
extern f32 func_001F9AF0();
struct Node { u8 pad[0x40]; f32 a; f32 b; };
struct Arg { u8 pad[0x78]; struct Node *node; };
struct __attribute__((packed)) Pair64 { u64 a; u64 b; };
void FUN_0023a318(struct Arg *arg) {
    u8 sp_slot[0x30];
    struct Node *node;
    node = arg->node;
    *(struct Pair64 *)(sp_slot + 0x20) = *(struct Pair64 *)D_00160ED0;
    func_0020CD48(arg, 4, sp_slot + 0x20, node);
    func_001F9A28(sp_slot, (u8 *)node + 0x10, node);
    func_001F9A28(sp_slot + 0x10, (u8 *)node + 0x20, node);
    node->a = func_001F9AF0(sp_slot);
    node->b = func_001F9AF0(sp_slot + 0x10);
}

extern __typeof__(FUN_0023a318) func_0023A318 __attribute__((alias("FUN_0023a318")));
