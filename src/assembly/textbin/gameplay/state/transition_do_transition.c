#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/state/transition_do_transition/"
            "FUN_001eb798.s",
            FUN_001eb798);
#else
#include "types.h"
#include "sda.h"

typedef struct {
    s32 off;
    s32 size;
} Chunk;

typedef struct {
    char pad0[0x20];
    Chunk chunk[6];
} WadHeader;

typedef struct {
    s32 a;
    s32 b;
} ChunkSrc;

typedef struct {
    char pad0[0x14F8];
    ChunkSrc src[6];
} Globals137B80;

typedef struct {
    s32 start;
    s32 end;
    s32 sound;
    s32 voice;
} SoundCue;

typedef struct {
    char pad0[0x34];
    s32 time;
    s32 pad38;
    s32 f3C;
} TransferState;

extern Globals137B80 D_00137B80;
extern s32 D_0015ED80 MACRO_ADDR;
extern s32 D_0015ED84;
extern s32 D_0015ED88 __attribute__((sda));
extern WadHeader *D_0015EE4C;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern volatile s32 D_0015EE8C;
extern s32 D_0015EF50;
extern s32 D_0015EF54;
extern s32 D_0015EF58 MACRO_ADDR;
extern f32 D_0015F43C MACRO_ADDR;
extern s32 D_0015F438 MACRO_ADDR;
extern s32 D_0015F5B0 MACRO_ADDR;
extern s32 D_0015F604 MACRO_ADDR;
extern s32 D_0015F618;
extern u8 D_0016034C MACRO_ADDR;
extern SoundCue D_001862B0[];
extern TransferState D_0018CB20;
extern WadHeader *volatile D_001940C8[];

extern void InitializeResourceEntry(void);
extern void QueueDmaTransfer(s32 index);
extern void ReadGlobalTableEntry(void);
extern s32 rand(void);
extern void sceGsResetGraph(s16 mode, s16 inter, s16 out, s16 ff);
extern s32 sceGsSyncV(s32 mode);
extern s32 func_001204B8();
extern s32 snd_flush_sound_commands() __asm__("func_0012DC80");
extern s32 snd_reset_state_and_flush_commands() __asm__("func_0012EB00");
extern void start_level(int level) __asm__("FUN_001e9488");
extern void transition_load_wad(void) __asm__("FUN_001ea830");
extern void update_gameplay_frame(void) __asm__("func_001EB0A8");
extern void transition_default_draw() __asm__("FUN_001eb410");
extern s32 is_active_state_entry(s32 arg0, s32 arg1) __asm__("FUN_001eb740");
extern s32 set_pal_mode() __asm__("func_001F34E8");
extern void fade_to_black(s32) __asm__("func_001F4A58");
extern s32 scale_game_frames() __asm__("func_001F96F8");
extern void put_draw_buffer_large(void) __asm__("func_001FB2D0");
extern void put_draw_buffer_small(void) __asm__("func_001FB3D0");
extern void append_palette_transfer_packet() __asm__("func_001FB6E0");
extern void init_hud(void) __asm__("FUN_001fee88");
extern void load_hud_banks(void) __asm__("FUN_00202a98");
extern void parse_space_scene_chunk(s32 index) __asm__("FUN_002049f0");
extern s32 run_state_handler() __asm__("func_00208840");
extern s32 load_and_initialize_level_chunk() __asm__("func_00209370");
extern s32 memcard_update_state() __asm__("func_002093D8");
extern s32 load(u8 *dst, s32 a, s32 b) __asm__("func_00216828");
extern void update_primary_pad_state(void) __asm__("func_00217A10");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern s32 allocate_voice_for_group_entry(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_0022dba0");
extern void initialize_sif_rpc(void) __asm__("FUN_00232ce0");
extern void FUN_00232d00(void);
extern void vu1_init_chain(void) __asm__("func_002335D0");
extern void swap_render_buffer_chain(void) __asm__("func_00233630");
extern void vu1_send_chain(void) __asm__("func_002336A0");
extern void vu1_sync_chain(s32) __asm__("func_002337B0");

void transition_do_transition(void) __asm__("FUN_001eb798");

void transition_do_transition(void) {
    WadHeader *hdr;
    u8 *p;
    s32 n;
    s32 level;
    s32 wait;
    s32 i;
    s32 old;
    Globals137B80 *tbl;
    u32 align_mask = 0xFFFFFFF0;

    D_0015ED84 = 0;
    *(volatile u32 *)0x10000010 = 0x83;
    *(volatile u32 *)0x10000000 = 0;
    load_and_initialize_level_chunk();
    tbl = &D_00137B80;
    transition_load_wad();
    p = (u8 *)D_001940C8[0];
    hdr = (WadHeader *)p;
    /* The first 0x60 bytes are reserved; six descriptors start at offset 0x20. */
    /* Load chunks 1..5 before chunk 0, advancing each destination to 16-byte alignment. */
    p = (u8 *)hdr + 0x60;
    D_0015EE4C = hdr;
    hdr->chunk[1].size =
        load(p, ((volatile ChunkSrc *)&tbl->src[1])->a, ((volatile ChunkSrc *)&tbl->src[1])->b);
    hdr->chunk[1].off = p - (u8 *)hdr;
    p += (hdr->chunk[1].size + 0xF) & align_mask;
    hdr->chunk[2].size = load(p, tbl->src[2].a, tbl->src[2].b);
    hdr->chunk[2].off = p - (u8 *)hdr;
    p += (hdr->chunk[2].size + 0xF) & align_mask;
    hdr->chunk[3].size = load(p, tbl->src[3].a, tbl->src[3].b);
    hdr->chunk[3].off = p - (u8 *)hdr;
    p += (hdr->chunk[3].size + 0xF) & align_mask;
    hdr->chunk[4].size = load(p, tbl->src[4].a, tbl->src[4].b);
    hdr->chunk[4].off = p - (u8 *)hdr;
    p += (hdr->chunk[4].size + 0xF) & align_mask;
    hdr->chunk[5].size = load(p, tbl->src[5].a, tbl->src[5].b);
    hdr->chunk[5].off = p - (u8 *)hdr;
    p += (hdr->chunk[5].size + 0xF) & align_mask;
    hdr->chunk[0].size = load(p, tbl->src[0].a, tbl->src[0].b);
    hdr->chunk[0].off = p - (u8 *)hdr;
    InitializeResourceEntry();
    initialize_sif_rpc();
    FUN_00232d00();
    init_hud();
    load_hud_banks();
    for (n = *(volatile u32 *)0x10000000 / 0x109; n < scale_game_frames(0xB4); n++) {
        sceGsSyncV(0);
    }
    snd_reset_state_and_flush_commands();
    while (snd_flush_sound_commands() != 0) {
    }
    wait = 0;
    level = rand() % 4;
    ReadGlobalTableEntry();
    while (D_0015F5B0 == 0) {
        vu1_send_chain();
        swap_render_buffer_chain();
        put_draw_buffer_small();
        append_palette_transfer_packet();
        put_draw_buffer_large();
        memcard_update_state();
        run_state_handler();
        update_primary_pad_state();
        QueueDmaTransfer(D_0015ED88);
        D_0015F618 = 0;
        for (i = 0; D_001862B0[i].start != -1; i++) {
            if (D_001862B0[i].end == -1) {
                if (D_0018CB20.time < D_001862B0[i].start) {
                    D_001862B0[i].voice = -1;
                    continue;
                }
                if (D_001862B0[i].voice != -1) {
                    continue;
                }
            } else if (D_0018CB20.time < D_001862B0[i].start ||
                       D_001862B0[i].end < D_0018CB20.time) {
                goto stop;
            } else if (is_active_state_entry(D_001862B0[i].voice, D_001862B0[i].sound) != 0) {
                continue;
            }
            D_001862B0[i].voice = allocate_voice_for_group_entry(D_001862B0[i].sound, 0, 0);
            continue;
        stop:
            if (is_active_state_entry(D_001862B0[i].voice, D_001862B0[i].sound) != 0) {
                release_voice_slot(D_001862B0[i].voice);
            }
            D_001862B0[i].voice = -1;
        }
        update_gameplay_frame();
        transition_default_draw();
        if (D_0015F604 != 0) {
            wait = 0;
        } else if (++wait >= scale_game_frames(0x5DC)) {
            start_level(level);
            wait = 0;
            level = (level + 1) % 4;
            D_0015F43C = 1.0f;
            D_0018CB20.time = 0;
            D_0018CB20.f3C = 0;
            parse_space_scene_chunk(0);
            D_0015EF50 = 0;
            D_0015EF54 = 0;
            D_0015EF58 = 0;
            vu1_init_chain();
            continue;
        }
        if (D_0016034C != D_0015ED80) {
            D_0015ED80 = D_0015ED80 == 0;
            fade_to_black(4);
            func_001204B8();
            sceGsResetGraph(0, 1, D_0015ED80 != 0 ? 3 : 2, 0);
            old = D_0015EE78;
            D_0015F43C = 1.0f;
            set_pal_mode();
            D_0015EE78 = old;
        }
        vu1_sync_chain(1);
        sceGsSyncV(0);
        D_0015F438++;
    }
    if (D_0015ED80 == 0) {
        D_0015EE8C = 0x280000;
    }
    D_0015EE74 = D_0015EE78 = D_0015EE8C;
}
#endif /* NON_MATCHING */
