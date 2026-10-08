#include "types.h"
#include "eetypes.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"

struct SkyTile {
    u128 bounds; /* 16-byte aligned: tile fields are addressed from the tile base */
    s32 address;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
};

struct Shell {
    s32 count;
    u8 pad4[0xC];
    struct SkyTile tiles[1];
};

extern s32 D_00160408[2] __attribute__((sda));
extern s32 D_00160410 __attribute__((sda));
extern u8 D_0013D160[];

extern void FUN_0022c4c8(struct SkyTile *tiles, s32 count, u8 *visibility) __asm__("FUN_0022c4c8");
extern void WriteDmaChannelRegisters(s32 address, s32 qwc, s32 destination);
extern void dma_to_spr_sync(void) __asm__("func_0020B3E0");
extern s32 FUN_0022bf94(s32, s32, s32, s32) __asm__("FUN_0022bf94");
extern void FUN_0022c208(s32, s32, s32, s32);

void sky_draw_shell_textured(struct Shell *shell) __asm__("FUN_0022b6e8");

void sky_draw_shell_textured(struct Shell *shell) {
    u8 visibility[shell->count];
    s32 i;
    s32 dest;
    s32 c_dest;
    s32 a_dest;
    s32 next;

    if (shell->count == 0) {
        return;
    }

    FUN_0022c4c8(shell->tiles, shell->count, visibility);

    render_packet_cursor.words[0] = 0x30000007;
    *(s32 *)((u32)render_packet_cursor.words + 4) = (s32)D_0013D160;
    *(s32 *)((u32)render_packet_cursor.words + 8) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 12) = 0x50000007;
    render_packet_cursor.words += 4;

    D_00160410 = 1 - D_00160410;
    if (visibility[0] == 1) {
        WriteDmaChannelRegisters(shell->tiles[0].address, shell->tiles[0].unkE >> 4,
                                 D_00160408[D_00160410]);
    }

    for (i = 0; i < shell->count; i++) {
        if (visibility[i] == 1) {
            dma_to_spr_sync();
        }

        next = i + 1;
        D_00160410 = 1 - D_00160410;
        if (next < shell->count && visibility[next] == 1) {
            WriteDmaChannelRegisters(shell->tiles[next].address, shell->tiles[next].unkE >> 4,
                                     D_00160408[D_00160410]);
        }

        if (visibility[i] == 1) {
            dest = D_00160408[1 - D_00160410];
            c_dest = dest + shell->tiles[i].unkC;
            a_dest = dest + shell->tiles[i].unkA;
            if (FUN_0022bf94(dest + shell->tiles[i].unk8, 0x70002000, shell->tiles[i].unk4,
                             (&shell->tiles[i])->unkC) == 0) {
                FUN_0022c208(shell->tiles[i].unk6, c_dest, a_dest, 0x70002000);
            }
        }
    }
}

extern __typeof__(sky_draw_shell_textured) func_0022B6E8 __attribute__((alias("FUN_0022b6e8")));
