#include "types.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/ui/hud/remove_hud_item.h"
#include "rnc/ui/menus/weapons/count_unspent_gold_bolts.h"

typedef struct {
    s32 unk0;
    u16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    u16 unk10;
} MenuItem;

typedef struct {
    u8  pad0[0x4];
    u16 unk4;
    u8  pad6[0x2];
    u8  unk8;
    u8  pad9[0x2D];
    s16 unk36;
    s32 unk38;
    u8 *unk3C;
} Menu;

typedef struct {
    s32 unk0;
    u8  pad4[0x10];
    u16 unk14;
    u8  pad16[0x2];
} Weapon;

typedef struct {
    char *s[6];
} Suffixes;

#include "rnc/ui/menus/system/process_menu_entry_globals.h"

extern s32      menu_help_hud_item_gp __asm__("D_00160070") __attribute__((sda));
extern s32      world_update_time_macro __asm__("D_0015F60C") MACRO_ADDR;
extern s32      D_001993C0[];
extern Suffixes menu_bolt_threshold_thousands_separators __asm__("D_001E8680");
extern char     help_text_integer_placeholder[] __asm__("D_00160088");
extern char     bolt_threshold_number_format[] __asm__("D_00160090");
extern Weapon   D_001DFFB0[];

#include "rnc/game/item_state.h"
#include "rnc/game/game_globals.h"

extern void  noop_callback_n(s32) __asm__("func_001E9458");
extern void  noop_callback_o(s32) __asm__("func_001E9460");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern char *strcpy(char *, const char *);
extern char *FindSubstring(char *, char *);
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
    MenuItem *entry;
    char     *substring_match;
    s32       ok;
    s32       next;
    u16       flags;
    s16       linked_entry_index;
    s16       kind;
    s32       bolt_threshold;

    entry = (MenuItem *)(menu->unk3C + menu->unk36 * 0x1C);
    if (menu->unk3C == 0) {
        return;
    }
    if (follow_entry_link != 0) {
        menu->unk4 = menu->unk36;
        menu->unk38 = world_update_time;
        linked_entry_index = entry->unk6;
        if (menu->unk36 != linked_entry_index) {
            flags = entry->unk10;
            menu->unk36 = entry->unk6;
            entry = (MenuItem *)(menu->unk3C + linked_entry_index * 0x1C);
            menu->unk8 = *(u8 *)&entry->unk10 & 1;
            if (flags & 4) {
                D_001993C0[2] = (s32)callback_context;
                D_001993C0[3] = (s32)menu;
                if (entry->unk4 & 0x4000) {
                    noop_callback_n((s16)(entry->unk4 ^ 0x4000));
                } else {
                    remove_hud_item(menu_help_hud_item_gp);
                    menu_help_hud_item_gp = -1;
                    noop_callback_o((s16)entry->unk4);
                }
            }
        }
    }
    if (menu->unk36 == -1) {
        return;
    }
    D_001993C0[5] = entry->unk0;
    kind = entry->unk8;
    if (kind == 6) {
        sfx = menu_bolt_threshold_thousands_separators;
        strcpy(SCRATCH, get_help_message_text(entry->unk0));
        substring_match = FindSubstring(SCRATCH, help_text_integer_placeholder);
        if (substring_match != 0) {
            substring_match[1] = 's';
        }
        bolt_threshold = D_001DFFB0[entry->unkA].unk14;
        sprintf(num,
                bolt_threshold_number_format,
                bolt_threshold / 1000,
                sfx.s[game_language % 6],
                bolt_threshold % 1000);
        sprintf(formatted_help_text, SCRATCH, num);
    } else {
        sprintf(formatted_help_text, get_help_message_text(entry->unk0));
    }
    if (D_001993C0[5] != 0) {
        copy_text_to_shared_buffer(formatted_help_text);
    } else if (menu_help_hud_item_gp != -1) {
        remove_hud_item(menu_help_hud_item_gp);
        menu_help_hud_item_gp = -1;
    }
    if (entry->unk8 == 0) {
        return;
    }
    ok = 0;
    switch (entry->unk8) {
    case MENU_CONDITION_PURCHASE_PRICE_MET:
        ok = !(current_bolt_count < D_001DFFB0[entry->unkA].unk0);
        break;
    case MENU_CONDITION_ITEM_AVAILABLE:
        ok = item_available[entry->unkA] != 0;
        break;
    case MENU_CONDITION_ITEM_AVAILABLE_NOT_ACQUIRED:
        ok = 0;
        if (item_available[entry->unkA] != 0) {
            ok = items_acquired[entry->unkA] == 0;
        }
        break;
    case MENU_CONDITION_ALTERNATE_ITEM_AVAILABLE:
        ok = alternate_item_available[entry->unkA] != 0;
        break;
    case MENU_CONDITION_COUNT_DIFFERENCE_MET:
        ok = !(count_unspent_gold_bolts() < entry->unkA);
        break;
    case MENU_CONDITION_BOLT_THRESHOLD_AND_COUNT_DIFFERENCE_MET:
        ok = current_bolt_count >= D_001DFFB0[entry->unkA].unk14 && count_unspent_gold_bolts() >= 4;
        break;
    default:
        ok = 0;
        break;
    }
    if (ok) {
        next = entry->unkC;
    } else {
        next = entry->unkE;
    }
    if (next != menu->unk36) {
        menu->unk36 = next;
        menu->unk38 = world_update_time_macro;
        entry = (MenuItem *)(menu->unk3C + next * 0x1C);
        menu->unk8 = *(u8 *)&entry->unk10 & 1;
        process_menu_entry(callback_context, menu, 0);
    }
}

extern __typeof__(process_menu_entry) func_00216C48 __attribute__((alias("FUN_00216c48")));
