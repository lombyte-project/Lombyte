#include "types.h"
#include "rnc/sdk/eetypes.h"
#include "sda.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
#include "rnc/data_labels.h"

/* A sky cluster header (0x20 bytes). */
struct SkyTile {
    u128 bounds;  /* 16-byte aligned: tile fields are addressed from the tile base */
    s32  address; /* cluster data */
    s16  vertex_count;
    s16  tri_count;
    s16  vertex_offset;
    s16  st_offset;
    s16  tri_offset;
    s16  data_size;
};

struct Shell {
    s32            count;
    u8             pad4[0xC];
    struct SkyTile tiles[1];
};

extern u8 sky_textured_gs_block[] __asm__("D_0013D160");

extern void
sky_bsphere_check(struct SkyTile *tiles, s32 count, u8 *visibility) __asm__("FUN_0022c4c8");
extern void WriteDmaChannelRegisters(s32 address, s32 qwc, s32 destination);
extern void dma_to_spr_sync(void) __asm__("func_0020B3E0");
extern s32  sky_transform_vertices(s32, s32, s32, s32) __asm__("FUN_0022bf94");
extern void FUN_0022c208(s32, s32, s32, s32);

void sky_draw_shell_textured(struct Shell *shell) __asm__("FUN_0022b6e8");

void sky_draw_shell_textured(struct Shell *shell) {
    u8  visibility[shell->count];
    s32 i;
    s32 dest;
    s32 tri_dest;
    s32 st_dest;
    s32 next_tile_index;

    if (shell->count == 0) {
        return;
    }

    sky_bsphere_check(shell->tiles, shell->count, visibility);

    render_packet_cursor.words[0] = 0x30000007;
    *(s32 *)((u32)render_packet_cursor.words + 4) = (s32)sky_textured_gs_block;
    *(s32 *)((u32)render_packet_cursor.words + 8) = 0;
    *(s32 *)((u32)render_packet_cursor.words + 12) = 0x50000007;
    render_packet_cursor.words += 4;

    D_00160410 = 1 - D_00160410;
    if (visibility[0] == 1) {
        WriteDmaChannelRegisters(
            shell->tiles[0].address, shell->tiles[0].data_size >> 4, D_00160408[D_00160410]);
    }

    for (i = 0; i < shell->count; i++) {
        if (visibility[i] == 1) {
            dma_to_spr_sync();
        }

        next_tile_index = i + 1;
        D_00160410 = 1 - D_00160410;
        if (next_tile_index < shell->count && visibility[next_tile_index] == 1) {
            WriteDmaChannelRegisters(shell->tiles[next_tile_index].address,
                                     shell->tiles[next_tile_index].data_size >> 4,
                                     D_00160408[D_00160410]);
        }

        if (visibility[i] == 1) {
            dest = D_00160408[1 - D_00160410];
            tri_dest = dest + shell->tiles[i].tri_offset;
            st_dest = dest + shell->tiles[i].st_offset;
            if (sky_transform_vertices(dest + shell->tiles[i].vertex_offset,
                                       0x70002000,
                                       shell->tiles[i].vertex_count,
                                       (&shell->tiles[i])->tri_offset) == 0) {
                FUN_0022c208(shell->tiles[i].tri_count, tri_dest, st_dest, 0x70002000);
            }
        }
    }
}

extern __typeof__(sky_draw_shell_textured) func_0022B6E8 __attribute__((alias("FUN_0022b6e8")));
