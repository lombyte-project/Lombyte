#include "types.h"
#include "rnc/rendering/image_clear_buffer.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/audio/sound.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/runtime/callbacks/count_vsync.h"
#include "rnc/runtime/iop/initialize_sif_rpc.h"
#include "rnc/sdk/debug.h"
#include "rnc/sdk/dma.h"
#include "rnc/sdk/graph.h"
#include "rnc/sdk/graph/sce_gs_sync_path.h"
#include "rnc/sdk/interrupts.h"
#include "rnc/sdk/rpc/iop.h"
#include "rnc/sdk/rpc/rpc.h"
#include "rnc/sdk/scf.h"
#include "rnc/sdk/storage/cdvd.h"
#include "rnc/sdk/storage/fileio.h"
#include "rnc/streaming.h"
#include "rnc/ui/text/font.h"

extern u8         vu0_micro_program[] __asm__("D_0010E4C0");
extern const char rebooted_iop_message[] __asm__("D_0015FA88");
extern s32        DIntr();
extern s32        EnableCache();
extern s32        fill_transfer_words() __asm__("func_001F97E8");
extern void       FlushCache();
extern s32        reset_vif1_gif() __asm__("FUN_001204b8");
extern s32        load_disc_sectors_into_global_buffer() __asm__("func_0012F2B8");
extern s32        init_view_context() __asm__("func_001F2C60");
extern s32        set_pal_mode() __asm__("func_001F34E8");
extern s32        initialize_alpha_lookup_table() __asm__("func_001F7A30");
extern s32        load_irx_module() __asm__("func_00201520");
extern s32        init_mem_slots() __asm__("func_002015D8");
extern s32        memcard_get_name() __asm__("func_00209030");
extern s32        memcard_initialize() __asm__("func_0020AC58");
extern s32        init_dma() __asm__("func_0020B418");
extern s32        decompress_wad() __asm__("FUN_0020b618");
extern s32        init_pads() __asm__("func_00217048");
extern s32        vu0_load_micro_program() __asm__("func_002334D8");
extern s32        vu1_init_chain() __asm__("func_002335D0");
void              init_once(void) __asm__("FUN_00201650");

const char iop_reboot_image_path[] __asm__("D_001E7AE0") = "cdrom0:\\IOPRP243.IMG;1";
const char loaded_irx_modules_message[] __asm__("D_001E7AF8") = "loaded irx modules\n";
const char unsupported_lang_message[] __asm__("D_001E7B10") = "Unsupported language %d\n";

/* The boot archive as decompress_wad unpacks it: the IRX modules to load,
   each as an offset from the archive start and a size. */
struct BootIrxModule {
    u32 offset;
    u32 size;
};

struct BootArchive {
    u8                   pad_00[0x70];
    struct BootIrxModule irx_modules[10]; /* 0x70 */
};

void init_once(void) {
    enum {
        SYSTEM_LANGUAGE_GAME_FALLBACK = 1,
        SYSTEM_LANGUAGE_GAME_COLUMN_TWO = 2,
        SYSTEM_LANGUAGE_GAME_COLUMN_FOUR = 3,
        SYSTEM_LANGUAGE_GAME_COLUMN_THREE = 4,
        SYSTEM_LANGUAGE_GAME_COLUMN_FIVE = 5
    };
    u8                  disc_sector_and_image_descriptor_buffer[0x800];
    s32                 lang;
    u32                 aligned_memory_base;
    struct BootArchive *boot_archive;
    s32                *compressed_archive_buffer;
    s32                 flag;

    reset_vif1_gif();
    sceDmaReset(1);
    sceCdInit(0);
    sceCdDiskReady(0);
    do {
    } while (sceSifRebootIop(iop_reboot_image_path) == 0);
    do {
    } while (sceSifSyncIop() == 0);
    DebugPrint(rebooted_iop_message);
    EnableCache(3);
    sceSifInitRpc(0);
    DIntr();
    sceSifInitIopHeap();
    EnableInterrupts();
    sceCdInit(0);
    sceCdDiskReady(0);
    sceCdMmode(2);
    SceFsReset();
    wad_get_sectors(0x121, 1, disc_sector_and_image_descriptor_buffer);
    FlushCache(0);
    flag = disc_sector_and_image_descriptor_buffer[0x33] != 0x4E;
    pal_mode = flag;
    pause_action_list_mode = flag;
    memcard_get_name(disc_sector_and_image_descriptor_buffer);
    sceGsResetGraph(0, 1, pal_mode ? 3 : 2, 0);
    init_dma();
    set_pal_mode();
    vu0_load_micro_program(vu0_micro_program);
    load_disc_sectors_into_global_buffer();
    aligned_memory_base = (u32)D_24135F & 0xFFFFC000;
    boot_archive = (struct BootArchive *)(aligned_memory_base + 0x2C0000);
    compressed_archive_buffer = (s32 *)(D_1FF8000 - (disc_table.unk12C0.size << 0xB));
    wad_get_sectors(disc_table.unk12C0.sector, disc_table.unk12C0.size, compressed_archive_buffer);
    FlushCache(0);
    decompress_wad(compressed_archive_buffer, boot_archive);
    FlushCache(0);
    load_irx_module(boot_archive->irx_modules[0].offset + (u32)boot_archive,
                    boot_archive->irx_modules[0].size);
    load_irx_module(boot_archive->irx_modules[1].offset + (u32)boot_archive,
                    boot_archive->irx_modules[1].size);
    load_irx_module(boot_archive->irx_modules[2].offset + (u32)boot_archive,
                    boot_archive->irx_modules[2].size);
    load_irx_module(boot_archive->irx_modules[3].offset + (u32)boot_archive,
                    boot_archive->irx_modules[3].size);
    load_irx_module(boot_archive->irx_modules[4].offset + (u32)boot_archive,
                    boot_archive->irx_modules[4].size);
    load_irx_module(boot_archive->irx_modules[5].offset + (u32)boot_archive,
                    boot_archive->irx_modules[5].size);
    load_irx_module(boot_archive->irx_modules[5].offset + (u32)boot_archive,
                    boot_archive->irx_modules[5].size);
    load_irx_module(boot_archive->irx_modules[7].offset + (u32)boot_archive,
                    boot_archive->irx_modules[7].size);
    load_irx_module(boot_archive->irx_modules[8].offset + (u32)boot_archive,
                    boot_archive->irx_modules[8].size);
    load_irx_module(boot_archive->irx_modules[6].offset + (u32)boot_archive,
                    boot_archive->irx_modules[6].size);
    DebugPrint(loaded_irx_modules_message);
    sceCdDiskReady(0);
    init_pads();
    sceGsSyncVCallback(count_vsync);
    current_vu_chain = 0x160000;
    init_mem_slots();
    sceCdDiskReady(0);
    memcard_initialize();
    init_view_context();
    update_view_context();
    vu1_init_chain();
    sceCdDiskReady(0);
    initialize_gameplay_sound_system();
    fill_transfer_words(image_clear_buffer, 0x80808080, 0x100);
    sceGsSetDefLoadImage(disc_sector_and_image_descriptor_buffer, 0x3FFB, 1, 0, 0, 0, 8, 8);
    FlushCache(0);
    sceGsExecLoadImage(disc_sector_and_image_descriptor_buffer, image_clear_buffer);
    sceGsSyncPath(0, 0);
    load_debug_font();
    *(volatile s32 *)0x10000810 = 0x82;
    *(volatile s32 *)0x10000800 = 0;
    initialize_sif_rpc();
    initialize_alpha_lookup_table();
    lang = sceScfGetLanguage();
    switch (lang) {
    case SYSTEM_LANGUAGE_GAME_COLUMN_TWO:
        game_language = 2;
        return;
    case SYSTEM_LANGUAGE_GAME_COLUMN_THREE:
        game_language = 3;
        return;
    case SYSTEM_LANGUAGE_GAME_COLUMN_FOUR:
        game_language = 4;
        return;
    case SYSTEM_LANGUAGE_GAME_COLUMN_FIVE:
        game_language = 5;
        return;
    default:
        DebugPrint(unsupported_lang_message, lang);
        /* fallthrough */
    case SYSTEM_LANGUAGE_GAME_FALLBACK:
        game_language = 0;
        return;
    }
}

extern __typeof__(init_once) func_00201650 __attribute__((alias("FUN_00201650")));

u8 image_clear_buffer[4096] = {0};

/* Retail keeps this message in small data; init_once reaches it with lui/addiu. */
const char rebooted_iop_message[] __asm__("D_0015FA88")
    __attribute__((section(".sdata"))) = "Rebooted IOP\n";
