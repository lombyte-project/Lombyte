#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/globals.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/ui/help/help_state.h"

extern s32 help_msgs __asm__("D_0015F6A0");

extern s32
start_audio_stream_read(s32 read_destination, s32 sector, s32 sector_count) __asm__("FUN_00216788");

static inline int help_text_count(void) {
    HelpState *state = &help_state;
    return state->count;
}

int update_menu_help_text_load(struct MenuScreen *menu) __asm__("FUN_0021d338");

int update_menu_help_text_load(struct MenuScreen *menu) {
    enum {
        HELP_TEXT_LOAD_START_READ = 0,
        HELP_TEXT_LOAD_WAIT_FOR_READ = 1,
        HELP_TEXT_LOAD_READY = 2,
        HELP_TEXT_LOAD_READ_NOT_STARTED = 3
    };
    switch (menu->data.help.load_state) {
    case HELP_TEXT_LOAD_START_READ:
        if (cd_read_active[0] == 0) {
            if (start_audio_stream_read(menu_system.help_text_buffer,
                                        disc_table.help_text.sector,
                                        disc_table.help_text.size) != 0) {
                menu->data.help.load_state = HELP_TEXT_LOAD_WAIT_FOR_READ;
            } else {
                menu->data.help.load_state = HELP_TEXT_LOAD_READ_NOT_STARTED;
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
            HelpState         *help_text_state;
            int               *text_entries;
            int                i;

            copy_memory_aligned(archive, language_entries, ((entry_bytes + 3) & ~3) - 8);
            help_text_state = &help_state;
            menu->data.help.saved_text_table = help_msgs;
            menu->data.help.saved_text_count = help_text_state->count;
            help_text_state->count = entry_count;
            text_entries = (int *)menu_system_state->help_text_buffer;
            help_msgs = (int)text_entries;
            for (i = 0; i < help_text_count(); i++) {
                int text_base = (int)text_entries - 8;
                *(int *)((char *)text_entries + i * 0x10) += text_base;
            }
            menu->flags &= ~MENU_SCREEN_HIDDEN;
            menu->data.help.load_state = HELP_TEXT_LOAD_READY;
        }
        break;
    case HELP_TEXT_LOAD_READY:
    case HELP_TEXT_LOAD_READ_NOT_STARTED:
        break;
    }
    return 0;
}

extern __typeof__(update_menu_help_text_load) func_0021D338 __attribute__((alias("FUN_0021d338")));
