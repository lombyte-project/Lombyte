#include "types.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/ui/hud/remove_hud_item.h"
#include "rnc/ui/menus/weapons/count_unspent_gold_bolts.h"
#include "rnc/ui/vendor/vendor_capture.h"
#include "rnc/sdk/libc/string.h"

typedef struct {
    s32 help_text;
    u16 action;
    s16 link;
    s16 condition;
    s16 condition_arg;
    s16 entry_if_met;
    s16 entry_if_not_met;
    u16 flags;
} MenuItem;

typedef struct {
    u8  pad0[0x4];
    u16 previous_entry;
    u8  pad6[0x2];
    u8  entry_flag;
    u8  pad9[0x2D];
    s16 current_entry;
    s32 entry_time;
    u8 *entries;
} Menu;

typedef struct {
    char *s[6];
} Suffixes;

#include "rnc/ui/menus/system/process_menu_entry_globals.h"

/* Small data, defined above the code so it is reached $gp-relative. */
s32         menu_help_hud_item __asm__("D_00160070") = -1;
extern s32  world_update_time_macro __asm__("D_0015F60C") MACRO_ADDR;
extern char thousands_separator_comma[] __asm__("D_00160078");
extern char thousands_separator_period[] __asm__("D_00160080");

/* The thousands separator of each language. */
const Suffixes menu_bolt_threshold_thousands_separators __asm__("D_001E8680") = {
    {
     thousands_separator_comma, thousands_separator_comma,
     thousands_separator_comma, thousands_separator_period,
     thousands_separator_comma, thousands_separator_period,
     }
};

extern char help_text_integer_placeholder[] __asm__("D_00160088");
extern char bolt_threshold_number_format[] __asm__("D_00160090");

#include "rnc/game/item_state.h"
#include "rnc/game/game_globals.h"

extern void  noop_callback_n(s32) __asm__("func_001E9458");
extern void  noop_callback_o(s32) __asm__("func_001E9460");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern char *strcpy(char *, const char *);
extern s32   sprintf(char *, const char *, ...);
extern void  copy_text_to_shared_buffer(char *) __asm__("func_001FF658");

#define SCRATCH ((char *)0x70000000)

void process_menu_entry(void *callback_context,
                        Menu *menu,
                        s32   follow_entry_link) __asm__("FUN_00216c48");

void process_menu_entry(void *callback_context, Menu *menu, s32 follow_entry_link) {
    enum {
        MENU_CONDITION_PURCHASE_PRICE_MET = 1,
        MENU_CONDITION_ITEM_AVAILABLE = 2,
        MENU_CONDITION_ITEM_AVAILABLE_NOT_ACQUIRED = 3,
        MENU_CONDITION_ALTERNATE_ITEM_AVAILABLE = 4,
        MENU_CONDITION_COUNT_DIFFERENCE_MET = 5,
        MENU_CONDITION_BOLT_THRESHOLD_AND_COUNT_DIFFERENCE_MET = 6
    };
    char      formatted_help_text[0x100];
    Suffixes  sfx;
    char      num[0x20];
    MenuItem *current_menu_entry;
    char     *substring_match;
    s32       ok;
    s32       condition_target_entry_index;
    u16       source_entry_flags;
    s16       linked_entry_index;
    s16       kind;
    s32       bolt_threshold;

    current_menu_entry = (MenuItem *)(menu->entries + menu->current_entry * 0x1C);
    if (menu->entries == 0) {
        return;
    }
    if (follow_entry_link != 0) {
        menu->previous_entry = menu->current_entry;
        menu->entry_time = world_update_time;
        linked_entry_index = current_menu_entry->link;
        if (menu->current_entry != linked_entry_index) {
            flags = current_menu_entry->flags;
            menu->current_entry = current_menu_entry->link;
            current_menu_entry = (MenuItem *)(menu->entries + linked_entry_index * 0x1C);
            menu->entry_flag = *(u8 *)&current_menu_entry->flags & 1;
            if (flags & 4) {
                menu_entry_context[2] = (s32)callback_context;
                menu_entry_context[3] = (s32)menu;
                if (current_menu_entry->action & 0x4000) {
                    noop_callback_n((s16)(current_menu_entry->action ^ 0x4000));
                } else {
                    remove_hud_item(menu_help_hud_item);
                    menu_help_hud_item = -1;
                    noop_callback_o((s16)current_menu_entry->action);
                }
            }
        }
    }
    if (menu->current_entry == -1) {
        return;
    }
    menu_entry_context[5] = current_menu_entry->help_text;
    kind = current_menu_entry->condition;
    if (kind == MENU_CONDITION_BOLT_THRESHOLD_AND_COUNT_DIFFERENCE_MET) {
        sfx = menu_bolt_threshold_thousands_separators;
        strcpy(SCRATCH, get_help_message_text(current_menu_entry->help_text));
        substring_match = FindSubstring(SCRATCH, help_text_integer_placeholder);
        if (substring_match != 0) {
            substring_match[1] = 's';
        }
        bolt_threshold = vendor_item_prices[current_menu_entry->condition_arg].gold_weapon_price;
        sprintf(num,
                bolt_threshold_number_format,
                bolt_threshold / 1000,
                sfx.s[game_language % 6],
                bolt_threshold % 1000);
        sprintf(formatted_help_text, SCRATCH, num);
    } else {
        sprintf(formatted_help_text, get_help_message_text(current_menu_entry->help_text));
    }
    if (menu_entry_context[5] != 0) {
        copy_text_to_shared_buffer(formatted_help_text);
    } else if (menu_help_hud_item != -1) {
        remove_hud_item(menu_help_hud_item);
        menu_help_hud_item = -1;
    }
    if (current_menu_entry->condition == 0) {
        return;
    }
    ok = 0;
    switch (current_menu_entry->condition) {
    case MENU_CONDITION_PURCHASE_PRICE_MET:
        ok = !(current_bolt_count <
               vendor_item_prices[current_menu_entry->condition_arg].purchase_price);
        break;
    case MENU_CONDITION_ITEM_AVAILABLE:
        ok = item_available[current_menu_entry->condition_arg] != 0;
        break;
    case MENU_CONDITION_ITEM_AVAILABLE_NOT_ACQUIRED:
        ok = 0;
        if (item_available[current_menu_entry->condition_arg] != 0) {
            ok = items_acquired[current_menu_entry->condition_arg] == 0;
        }
        break;
    case MENU_CONDITION_ALTERNATE_ITEM_AVAILABLE:
        ok = alternate_item_available[current_menu_entry->condition_arg] != 0;
        break;
    case MENU_CONDITION_COUNT_DIFFERENCE_MET:
        ok = !(count_unspent_gold_bolts() < current_menu_entry->condition_arg);
        break;
    case MENU_CONDITION_BOLT_THRESHOLD_AND_COUNT_DIFFERENCE_MET:
        ok = current_bolt_count >=
                 vendor_item_prices[current_menu_entry->condition_arg].gold_weapon_price &&
             count_unspent_gold_bolts() >= 4;
        break;
    default:
        ok = 0;
        break;
    }
    if (ok) {
        condition_target_entry_index = current_menu_entry->entry_if_met;
    } else {
        condition_target_entry_index = current_menu_entry->entry_if_not_met;
    }
    if (condition_target_entry_index != menu->current_entry) {
        menu->current_entry = condition_target_entry_index;
        menu->entry_time = world_update_time_macro;
        current_menu_entry = (MenuItem *)(menu->entries + condition_target_entry_index * 0x1C);
        menu->entry_flag = *(u8 *)&current_menu_entry->flags & 1;
        process_menu_entry(callback_context, menu, 0);
    }
}

extern __typeof__(process_menu_entry) func_00216C48 __attribute__((alias("FUN_00216c48")));

s32 menu_entry_context[6] = {0};

/* Small data reached with lui/addiu: the thousands separators that
 * menu_bolt_threshold_thousands_separators points to, and the formats. */
char thousands_separator_comma[] __asm__("D_00160078") __attribute__((section(".sdata"))) = ",";
char thousands_separator_period[] __asm__("D_00160080") __attribute__((section(".sdata"))) = ".";
char help_text_integer_placeholder[] __asm__("D_00160088")
    __attribute__((section(".sdata"))) = "%d";
char bolt_threshold_number_format[] __asm__("D_00160090")
    __attribute__((section(".sdata"))) = "%d%s%03d";
