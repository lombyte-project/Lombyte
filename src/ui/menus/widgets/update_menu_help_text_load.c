#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/globals.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/asm_helpers.h"

extern u8 D_001996D0[];

extern s32 help_msgs __asm__("D_0015F6A0");

extern s32
start_audio_stream_read(s32 read_destination, s32 sector, s32 sector_count) __asm__("FUN_00216788");

static inline int pauseSlotCount(void) {
    char *b = D_001996D0;
    return *(int *)(b + 0x2C);
}

int update_menu_help_text_load(char *menu) __asm__("FUN_0021d338");

int update_menu_help_text_load(char *menu) {
    enum {
        HELP_TEXT_LOAD_START_READ = 0,
        HELP_TEXT_LOAD_WAIT_FOR_READ = 1,
        HELP_TEXT_LOAD_READY = 2,
        HELP_TEXT_LOAD_READ_NOT_STARTED = 3
    };
    switch (*(int *)(menu + 0x50)) {
    case HELP_TEXT_LOAD_START_READ:
        if (cd_read_active[0] == 0) {
            if (start_audio_stream_read(menu_system.help_text_buffer,
                                        disc_table.help_text.sector,
                                        disc_table.help_text.size) != 0) {
                *(int *)(menu + 0x50) = 1;
            } else {
                *(int *)(menu + 0x50) = 3;
            }
        }
        break;
    case HELP_TEXT_LOAD_WAIT_FOR_READ:
        if (cd_read_active[0] == 0) {
            struct MenuSystem *menu_system_state = &menu_system;
            int               *archive = (int *)menu_system_state->help_text_buffer;
            int               *language_entries = (int *)((char *)archive + archive[game_language]);
            int                entry_count = *language_entries++;
            int                entry_bytes = *language_entries++;
            char              *entry_count_base;
            int               *text_entries;
            int                i;

            copy_memory_aligned(archive, language_entries, ((entry_bytes + 3) & ~3) - 8);
            entry_count_base = D_001996D0;
            *(int *)(menu + 0x54) = help_msgs;
            *(int *)(menu + 0x38) = *(int *)(entry_count_base + 0x2C);
            *(int *)(entry_count_base + 0x2C) = entry_count;
            text_entries = (int *)menu_system_state->help_text_buffer;
            help_msgs = (int)text_entries;
            for (i = 0; i < pauseSlotCount(); i++) {
                int text_base = (int)text_entries - 8;
                *(int *)((char *)text_entries + i * 0x10) += text_base;
            }
            *(int *)(menu + 0x10) &= ~4;
            *(int *)(menu + 0x50) = 2;
        }
        break;
    case HELP_TEXT_LOAD_READY:
    case HELP_TEXT_LOAD_READ_NOT_STARTED:
        break;
    }
    return 0;
}

extern __typeof__(update_menu_help_text_load) func_0021D338 __attribute__((alias("FUN_0021d338")));
