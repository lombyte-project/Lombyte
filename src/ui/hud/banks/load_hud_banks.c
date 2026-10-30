#include "types.h"
#include "rnc/ui/hud/hud_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/asm_helpers.h"

#define ALIGN64(x) (((x) + 0x3F) & ~0x3FU)

struct Chunk {
    s32 offset;
    s32 size;
};

struct HudFile {
    u8           pad0[0x20];
    struct Chunk chunk[6];
};

struct Vram {
    u8  pad0[8];
    s32 base;
};

extern char            hud_allocation_source_file[] __asm__("D_0015FBB0");
extern char            hud_bank_0_stash_label[] __asm__("D_0015FBC0");
extern char            hud_bank_2_stash_label[] __asm__("D_0015FBD0");
extern char            hud_bank_3_stash_label[] __asm__("D_0015FBE0");
extern char            hud_bank_4_stash_label[] __asm__("D_0015FBF0");
extern struct Vram     D_001940C0;
extern u32             D_0019A420[];
extern struct HudBank *hud_heap_alloc(u32, s32, char *, s32) __asm__("func_001FF288");
extern void            load_compressed_hud_bank(s32, s32) __asm__("FUN_00202d10");
extern s32             stash_send_data(void *, u32, u32, char *) __asm__("func_00232E40");
extern void            hud_send_resident_bank(s32, s32, s32) __asm__("func_001FF128");
extern void            link_hud_bank(s32, void *) __asm__("func_001FEFC0");
extern void            FlushCache(s32);

void load_hud_banks(void) __asm__("FUN_00202a98");

void load_hud_banks(void) {
    struct HudFile  *hud_file = level_root;
    struct HudBank  *hud_bank;
    void            *bank_1_base;
    s32              i;
    u32              size;
    u32              chunk_1_qword_count;
    s32              vram;
    u32             *aligned_chunk_size_cursor;
    s32             *chunk_size_cursor;
    struct HudState *hb;
    struct Vram     *vram_state_ptr;

    aligned_chunk_size_cursor = D_0019A420;
    chunk_size_cursor = &hud_file->chunk[1].size;
    for (i = 0; i < 5; i++) {
        *aligned_chunk_size_cursor++ = ALIGN64(*chunk_size_cursor);
        chunk_size_cursor += 2;
    }
    size = ALIGN64(hud_file->chunk[0].size);
    vram_state_ptr = &D_001940C0;
    hud_bank = hud_heap_alloc(size, 0, hud_allocation_source_file, 0x23A);
    hb = &hud_state;
    fast_mem_copy(hud_bank, (void *)(hud_file->chunk[0].offset + (s32)hud_file), size);
    hb->header.bank = hud_bank;
    hb->anim_defs = (struct HudAnimDef *)((u8 *)hud_bank + hud_bank->off4);
    vram = vram_state_ptr->base + 0x60000;
    hb->frame_refs = (struct HudFrameRef *)((u8 *)hud_bank + hud_bank->off8);
    hb->palette_pages = (struct HudTexPage *)((u8 *)hud_bank + hud_bank->offC);
    hb->image_pages = (struct HudTexPage *)((u8 *)hud_bank + hud_bank->off10);
    if (hud_bank->has54) {
        chunk_1_qword_count = ALIGN64(hud_file->chunk[1].size) >> 4;
        load_compressed_hud_bank(0, vram);
        hb->header.bank->unk94 =
            stash_send_data((void *)(hud_file->chunk[1].offset + (s32)hud_file),
                            chunk_1_qword_count,
                            chunk_1_qword_count,
                            hud_bank_0_stash_label);
        hud_send_resident_bank(0, vram, 1);
    }
    if (hb->header.bank->size58) {
        bank_1_base = hud_heap_alloc(hb->header.bank->size58, 0, hud_allocation_source_file, 0x261);
        load_compressed_hud_bank(1, (s32)bank_1_base);
        FlushCache(0);
        link_hud_bank(1, bank_1_base);
    }
    if (hb->header.bank->has5C) {
        u32 chunk_3_qword_count = ALIGN64(hud_file->chunk[3].size) >> 4;
        hb->header.bank->unk9C =
            stash_send_data((void *)(hud_file->chunk[3].offset + (s32)hud_file),
                            chunk_3_qword_count,
                            chunk_3_qword_count,
                            hud_bank_2_stash_label);
    }
    if (hb->header.bank->has60) {
        u32 chunk_4_qword_count = ALIGN64(hud_file->chunk[4].size) >> 4;
        hb->header.bank->unkA0 =
            stash_send_data((void *)(hud_file->chunk[4].offset + (s32)hud_file),
                            chunk_4_qword_count,
                            chunk_4_qword_count,
                            hud_bank_3_stash_label);
    }
    if (hb->header.bank->has64) {
        u32 chunk_5_qword_count = ALIGN64(hud_file->chunk[5].size) >> 4;
        hb->header.bank->unkA4 =
            stash_send_data((void *)(hud_file->chunk[5].offset + (s32)hud_file),
                            chunk_5_qword_count,
                            chunk_5_qword_count,
                            hud_bank_4_stash_label);
    }
}

extern __typeof__(load_hud_banks) func_00202A98 __attribute__((alias("FUN_00202a98")));
