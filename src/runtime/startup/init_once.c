#include "types.h"
#include "rnc/storage/disc_table.h"
#include "rnc/globals.h"
extern u8 D_0010E4C0[];
extern void count_vsync() __asm__("FUN_0012f1c8");
extern s32 D_0015EE90;
extern u8 D_0015FA88[];
extern s32 D_00160F0C;
extern u8 D_001941C0[];
extern u8 D_001E7AE0[];
extern u8 D_001E7AF8[];
extern u8 D_001E7B10[];
extern u8 D_1FF8000[];
extern u8 D_24135F[];
extern s32 DIntr();
extern s32 DebugPrint();
extern s32 EnableCache();
extern s32 EnableInterrupts();
extern s32 FillTransferWords();
extern void FlushCache();
extern s32 FUN_001204b8();
extern s32 FUN_00120558();
extern s32 FUN_00121190();
extern s32 wad_get_sectors() __asm__("FUN_0012f208");
extern s32 load_disc_sectors_into_global_buffer() __asm__("func_0012F2B8");
extern s32 load_debug_font() __asm__("FUN_001e9338");
extern s32 init_view_context() __asm__("func_001F2C60");
extern s32 update_view_context() __asm__("FUN_001f2d98");
extern s32 set_pal_mode() __asm__("func_001F34E8");
extern s32 initialize_alpha_lookup_table() __asm__("func_001F7A30");
extern s32 load_irx_module() __asm__("func_00201520");
extern s32 init_mem_slots() __asm__("func_002015D8");
extern s32 memcard_get_name() __asm__("func_00209030");
extern s32 memcard_initialize() __asm__("func_0020AC58");
extern s32 init_dma() __asm__("func_0020B418");
extern s32 FUN_0020b618();
extern s32 init_pads() __asm__("func_00217048");
extern s32 initialize_gameplay_sound_system() __asm__("FUN_0022c8d0");
extern s32 initialize_sif_rpc() __asm__("FUN_00232ce0");
extern s32 vu0_load_micro_program() __asm__("func_002334D8");
extern s32 vu1_init_chain() __asm__("func_002335D0");
extern s32 sceCdInit();
extern s32 sceCdMmode();
extern s32 sceDmaReset();
extern s32 sceFsReset();
extern s32 sceGsExecLoadImage();
extern s32 sceGsResetGraph();
extern s32 sceGsSetDefLoadImage();
extern s32 sceGsSyncVCallback();
extern s32 sceScfGetLanguage();
extern s32 sceSifInitIopHeap();
extern s32 sceSifInitRpc();
extern s32 sceSifRebootIop();
extern s32 sceSifSyncIop();
void init_once(void) __asm__("FUN_00201650");

void init_once(void) {
    u8 buf[0x800];
    s32 lang;
    u32 b;
    u32 base;
    s32 *dst;
    s32 flag;

    FUN_001204b8();
    sceDmaReset(1);
    sceCdInit(0);
    FUN_00121190(0);
    do {

    } while (sceSifRebootIop(D_001E7AE0) == 0);
    do {

    } while (sceSifSyncIop() == 0);
    DebugPrint(D_0015FA88);
    EnableCache(3);
    sceSifInitRpc(0);
    DIntr();
    sceSifInitIopHeap();
    EnableInterrupts();
    sceCdInit(0);
    FUN_00121190(0);
    sceCdMmode(2);
    sceFsReset();
    wad_get_sectors(0x121, 1, buf);
    FlushCache(0);
    flag = buf[0x33] != 0x4E;
    pal_mode = flag;
    D_0015EE90 = flag;
    memcard_get_name(buf);
    sceGsResetGraph(0, 1, pal_mode ? 3 : 2, 0);
    init_dma();
    set_pal_mode();
    vu0_load_micro_program(D_0010E4C0);
    load_disc_sectors_into_global_buffer();
    b = (u32)D_24135F & 0xFFFFC000;
    base = b + 0x2C0000;
    dst = (s32 *)(D_1FF8000 - (disc_table.unk12C0.size << 0xB));
    wad_get_sectors(disc_table.unk12C0.sector, disc_table.unk12C0.size, dst);
    FlushCache(0);
    FUN_0020b618(dst, base);
    FlushCache(0);
    load_irx_module(*(u32 *)(base + 0x70) + base, *(u32 *)(base + 0x74));
    load_irx_module(*(u32 *)(base + 0x78) + base, *(u32 *)(base + 0x7C));
    load_irx_module(*(u32 *)(base + 0x80) + base, *(u32 *)(base + 0x84));
    load_irx_module(*(u32 *)(base + 0x88) + base, *(u32 *)(base + 0x8C));
    load_irx_module(*(u32 *)(base + 0x90) + base, *(u32 *)(base + 0x94));
    load_irx_module(*(u32 *)(base + 0x98) + base, *(u32 *)(base + 0x9C));
    load_irx_module(*(u32 *)(base + 0x98) + base, *(u32 *)(base + 0x9C));
    load_irx_module(*(u32 *)(base + 0xA8) + base, *(u32 *)(base + 0xAC));
    load_irx_module(*(u32 *)(base + 0xB0) + base, *(u32 *)(base + 0xB4));
    load_irx_module(*(u32 *)(base + 0xA0) + base, *(u32 *)(base + 0xA4));
    DebugPrint(D_001E7AF8);
    FUN_00121190(0);
    init_pads();
    sceGsSyncVCallback(count_vsync);
    D_00160F0C = 0x160000;
    init_mem_slots();
    FUN_00121190(0);
    memcard_initialize();
    init_view_context();
    update_view_context();
    vu1_init_chain();
    FUN_00121190(0);
    initialize_gameplay_sound_system();
    FillTransferWords(D_001941C0, 0x80808080, 0x100);
    sceGsSetDefLoadImage(buf, 0x3FFB, 1, 0, 0, 0, 8, 8);
    FlushCache(0);
    sceGsExecLoadImage(buf, D_001941C0);
    FUN_00120558(0, 0);
    load_debug_font();
    *(volatile s32 *)0x10000810 = 0x82;
    *(volatile s32 *)0x10000800 = 0;
    initialize_sif_rpc();
    initialize_alpha_lookup_table();
    lang = sceScfGetLanguage();
    switch (lang) {
    case 2:
        game_language = 2;
        return;
    case 4:
        game_language = 3;
        return;
    case 3:
        game_language = 4;
        return;
    case 5:
        game_language = 5;
        return;
    default:
        DebugPrint(D_001E7B10, lang);
        /* fallthrough */
    case 1:
        game_language = 0;
        return;
    }
}

extern __typeof__(init_once) func_00201650 __attribute__((alias("FUN_00201650")));
