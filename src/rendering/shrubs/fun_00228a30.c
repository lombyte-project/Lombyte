#include "types.h"
struct TexRemap { s16 lo; s16 hi; };
struct GifPacket { s32 count; s32 skip; u8 pad8[8]; };
struct GifSlot { struct GifPacket *pkt; s32 pad; };
struct TieClass { u8 pad0[0x28]; s16 count; u8 pad2A[0x16]; struct GifSlot slots[1]; };
extern s32 D_001D8DB0[];
extern struct TieClass *D_001D7F30[];
extern struct TexRemap D_001D88B0[];
void FUN_00228a30(void) {
    s32 *id;
    struct TieClass *tc;
    s32 i;
    s32 j;
    struct GifPacket *pkt;
    u8 *gif;
    struct TexRemap *r;

    for (id = D_001D8DB0; *id >= 0; id++) {
        tc = D_001D7F30[*id];
        for (i = 0; i < tc->count; i++) {
            pkt = tc->slots[i].pkt + 1;
            gif = (u8 *)(pkt + pkt->skip) + 0x10;
            for (j = 0; j < pkt->count; j++) {
                r = &D_001D88B0[gif[0x13]];
                if (r->lo != 0) {
                    *(u32 *)(gif + 0x30) = (*(u32 *)(gif + 0x30) & 0xFFFFC000) | r->lo;
                }
                if (r->hi != 0) {
                    *(u32 *)(gif + 0x20) = (*(u32 *)(gif + 0x20) & 0xFFFFC000) | r->hi;
                }
                gif += 0x40;
            }
        }
    }
}

extern __typeof__(FUN_00228a30) func_00228A30 __attribute__((alias("FUN_00228a30")));
