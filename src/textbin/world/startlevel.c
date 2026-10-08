#include "types.h"
#include "rnc/input/pad_state.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/storage/disc_table.h"

typedef struct {
    s32 off;
    s32 size;
} LevelChunk;

typedef struct {
    s32 gfx;
    s32 pad4;
    s32 gfx_alt;
    s32 padC;
    LevelChunk intro[6];
    LevelChunk loading[6];
    s32 code;
} LevelHeader;

typedef struct {
    s32 pad[7];
    s32 bank;
} SoundSlot;

extern s32 D_0015F5E8 MACRO_ADDR;
extern u8 D_00161280[];
extern u8 D_00165430[];
extern s32 D_0015F438 MACRO_ADDR;
extern s32 D_0015F604 MACRO_ADDR;
extern s32 D_0015EF5C MACRO_ADDR;
extern s32 D_0015EED8 MACRO_ADDR;
extern s32 D_0015ED80 MACRO_ADDR;
extern s32 D_0015ED84 MACRO_ADDR;
extern u8 D_24135F[];
extern struct PadState D_0013C940;
extern s32 D_00139378[];
extern s32 D_00139380[];
extern char D_001E76C0[];
extern volatile SoundSlot D_00186100[];
extern volatile SoundSlot D_001861E0[];
extern volatile SoundSlot * volatile D_0015F634;
extern volatile s32 D_0015F630;
extern u8 D_0016034C;
extern s32 D_0015F600;
extern u8 D_0013E030[];

extern void init_once(void) __asm__("func_00201650");
extern void InitializeStreamingState(void);
extern void vu1_init_chain(void) __asm__("func_002335D0");
extern void dmac_vif1_enable(void) __asm__("func_00233D00");
extern void ClearDmaQueueEntry(void);
extern s32 sceGsSyncV(s32);
extern void put_disp_buffer(void) __asm__("func_001FB2A8");
extern void PackDmaTag(s32, u64, u64);
extern void FlushCache(s32);
extern void func_0020B618(s32, s32);
extern void put_draw_buffer_large(void) __asm__("func_001FB2D0");
extern void append_gif_transfer_packet(void) __asm__("func_001FB368");
extern void draw_boot_image(s32) __asm__("func_002012B8");
extern void put_draw_buffer_small(void) __asm__("func_001FB3D0");
extern void append_palette_transfer_packet(void) __asm__("func_001FB6E0");
extern void vu1_send_chain(void) __asm__("func_002336A0");
extern void swap_render_buffer_chain(void) __asm__("func_00233630");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");
extern s32 check_memory_card(void) __asm__("FUN_00209168");
extern void update_primary_pad_state(void) __asm__("func_00217A10");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern void play_mpeg_movie(s32, s32, s32, s32, s32) __asm__("func_0023A3B8");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern void DebugPrint(char *, ...);
extern s32 load_audio_bank_by_location(s32) __asm__("func_0022D708");
extern void snd_resolve_bank_xrefs(void) __asm__("func_0012E1A8");
extern s32 transition_do_transition(void) __asm__("FUN_001eb798");
extern s32 func_001204B8(void);
extern void sceGsResetGraph(s16, s16, s16, s16);
extern void set_pal_mode(void) __asm__("func_001F34E8");
extern void do_space_transition(void) __asm__("FUN_00231ff0");

static inline void clear_bytes(u8 *p, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        p[i] = 0;
    }
}

void startlevel(void) __asm__("FUN_001e9658");

void startlevel(void) {
    LevelHeader *hdr;
    LevelChunk *tbl;
    s32 n;
    s32 i;
    s32 cur;
    s32 prev;
    s32 frames;
    s32 code;
    s32 bank;
    u8 *p;

    D_0015F5E8 = 1;
    init_once();
    InitializeStreamingState();
    for (i = 0; i < D_00165430 - D_00161280; i++) {
        D_00161280[i] = 0;
    }
    prev = 0;
    frames = 0;
    vu1_init_chain();
    dmac_vif1_enable();
    ClearDmaQueueEntry();
    D_0015F438 = 0;
    sceGsSyncV(0);
    D_0015F604 = 0;
    put_disp_buffer();
    PackDmaTag(0, 0, 0);
    hdr = (LevelHeader *)(((u32)D_24135F & 0xFFFFC000) + 0x2C0000);
    while ((cur = check_memory_card()) != 0 && (frames < 11 || D_0013C940.pressed == 0)) {
        if (cur != prev) {
            if (cur == 1) {
                tbl = hdr->intro;
            } else {
                tbl = hdr->loading;
            }
            FlushCache(0);
            func_0020B618(tbl[game_language].off + (s32)hdr, hdr->code + (s32)hdr);
            FlushCache(0);
            vu1_init_chain();
            PackDmaTag(0, 0, 0);
            put_draw_buffer_large();
            append_gif_transfer_packet();
            draw_boot_image(hdr->code + (s32)hdr);
            put_draw_buffer_small();
            append_palette_transfer_packet();
            vu1_send_chain();
            swap_render_buffer_chain();
            vu1_sync_chain(1);
        }
        sceGsSyncV(0);
        prev = cur;
        update_primary_pad_state();
        frames++;
    }
    if (prev != 0) {
        fade_to_black(10);
    }
    D_0015EED8 = -1;
    code = hdr->code + (s32)hdr;
    D_0015EF5C = code;
    if (D_0015ED80 == 0) {
        play_mpeg_movie(D_00139378[0], D_00139378[1], (code + 0x3F) & ~0x3F,
                        (code + 0x2C003F) & ~0x3F, 0);
    } else {
        play_mpeg_movie(D_00139380[0], D_00139380[1], (code + 0x3F) & ~0x3F,
                        (code + 0x2C003F) & ~0x3F, 0);
    }
    D_0015EED8 = 0;
    fade_to_black(scale_game_frames(0x12));
    FlushCache(0);
    if (D_0015ED80 != 0) {
        func_0020B618(hdr->gfx_alt + (s32)hdr, hdr->code + (s32)hdr);
    } else {
        func_0020B618(hdr->gfx + (s32)hdr, hdr->code + (s32)hdr);
    }
    FlushCache(0);
    vu1_init_chain();
    PackDmaTag(0, 0, 0);
    put_draw_buffer_large();
    append_gif_transfer_packet();
    draw_boot_image(hdr->code + (s32)hdr);
    put_draw_buffer_small();
    append_palette_transfer_packet();
    vu1_send_chain();
    swap_render_buffer_chain();
    vu1_sync_chain(1);
    sceGsSyncV(0);
    D_0015F438++;
    DebugPrint(D_001E76C0);
    bank = load_audio_bank_by_location(disc_table.sound_bank);
    snd_resolve_bank_xrefs();
    /* Publish the slot tables and metadata in retail order. */
    D_001861E0[4].bank = bank;
    D_00186100[6].bank = bank;
    D_0015F634 = D_00186100;
    D_0015F630 = 7;
    D_00186100[0].bank = bank;
    D_00186100[1].bank = bank;
    D_00186100[2].bank = bank;
    D_00186100[3].bank = bank;
    D_00186100[4].bank = bank;
    D_00186100[5].bank = bank;
    D_001861E0[0].bank = bank;
    /* The ordinary view lets the last bank store fill the call delay slot. */
    ((SoundSlot *)D_001861E0)[2].bank = bank;
    ((SoundSlot *)D_001861E0)[3].bank = bank;
    D_001861E0[1].bank = bank;
    transition_do_transition();
    if (D_0015ED80 != D_0016034C) {
        D_0015ED80 = D_0016034C;
        func_001204B8();
        sceGsResetGraph(0, 1, D_0015ED80 != 0 ? 3 : 2, 0);
        set_pal_mode();
        put_disp_buffer();
    }
    p = D_0013E030;
    D_0015F600 = D_0015ED84;
    *(s16 *)(p + 0x2A) = 1;
    D_0015ED84 = -1;
    do_space_transition();
}

extern __typeof__(startlevel) func_001E9658 __attribute__((alias("FUN_001e9658")));
