#include "types.h"
#include "rnc/ui/hud/hud_state.h"

#define ALIGN64(x) (((x) + 0x3F) & ~0x3FU)

struct Chunk {
    s32 offset;
    s32 size;
};

struct HudFile {
    u8 pad0[0x20];
    struct Chunk chunk[6];
};

struct Vram {
    u8 pad0[8];
    s32 base;
};

extern struct HudFile *D_0015EE4C;
extern char D_0015FBB0[];
extern char D_0015FBC0[];
extern char D_0015FBD0[];
extern char D_0015FBE0[];
extern char D_0015FBF0[];
extern struct Vram D_001940C0;
extern u32 D_0019A420[];
extern struct HudBank *hud_heap_alloc(u32, s32, char *, s32) __asm__("func_001FF288");
extern void FUN_001f98d0(void *, void *, u32);
extern void load_compressed_hud_bank(s32, s32) __asm__("FUN_00202d10");
extern s32 stash_send_data(void *, u32, u32, char *) __asm__("func_00232E40");
extern void hud_send_resident_bank(s32, s32, s32) __asm__("func_001FF128");
extern void link_hud_bank(s32, void *) __asm__("func_001FEFC0");
extern void FlushCache(s32);

void load_hud_banks(void) __asm__("FUN_00202a98");

void load_hud_banks(void) {
    struct HudFile *f = D_0015EE4C;
    struct HudBank *b;
    void *t;
    s32 i;
    u32 size;
    u32 n;
    s32 vram;
    u32 *dst;
    s32 *c;
    struct HudState *hb;
    struct Vram *v;

    dst = D_0019A420;
    c = &f->chunk[1].size;
    for (i = 0; i < 5; i++) {
        *dst++ = ALIGN64(*c);
        c += 2;
    }
    size = ALIGN64(f->chunk[0].size);
    v = &D_001940C0;
    b = hud_heap_alloc(size, 0, D_0015FBB0, 0x23A);
    hb = &hud_state;
    FUN_001f98d0(b, (void *)(f->chunk[0].offset + (s32)f), size);
    hb->header.bank = b;
    hb->anim_defs = (struct HudAnimDef *)((u8 *)b + b->off4);
    vram = v->base + 0x60000;
    hb->frame_refs = (struct HudFrameRef *)((u8 *)b + b->off8);
    hb->palette_pages = (struct HudTexPage *)((u8 *)b + b->offC);
    hb->image_pages = (struct HudTexPage *)((u8 *)b + b->off10);
    if (b->has54) {
        n = ALIGN64(f->chunk[1].size) >> 4;
        load_compressed_hud_bank(0, vram);
        hb->header.bank->unk94 = stash_send_data((void *)(f->chunk[1].offset + (s32)f), n, n, D_0015FBC0);
        hud_send_resident_bank(0, vram, 1);
    }
    if (hb->header.bank->size58) {
        t = hud_heap_alloc(hb->header.bank->size58, 0, D_0015FBB0, 0x261);
        load_compressed_hud_bank(1, (s32)t);
        FlushCache(0);
        link_hud_bank(1, t);
    }
    if (hb->header.bank->has5C) {
        u32 n3 = ALIGN64(f->chunk[3].size) >> 4;
        hb->header.bank->unk9C =
            stash_send_data((void *)(f->chunk[3].offset + (s32)f), n3, n3, D_0015FBD0);
    }
    if (hb->header.bank->has60) {
        u32 n4 = ALIGN64(f->chunk[4].size) >> 4;
        hb->header.bank->unkA0 =
            stash_send_data((void *)(f->chunk[4].offset + (s32)f), n4, n4, D_0015FBE0);
    }
    if (hb->header.bank->has64) {
        u32 n5 = ALIGN64(f->chunk[5].size) >> 4;
        hb->header.bank->unkA4 =
            stash_send_data((void *)(f->chunk[5].offset + (s32)f), n5, n5, D_0015FBF0);
    }
}

extern __typeof__(load_hud_banks) func_00202A98 __attribute__((alias("FUN_00202a98")));
