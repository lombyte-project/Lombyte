/* Menus: builds the reversed list of active messages. */
#include "types.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/ui/help/message_state.h"
#include "rnc/ui/menus/menu_screen.h"

extern u16 message_list_value_lookup[] __asm__("D_L00_00179452");
/* The level's help history list: one row per help entry shown so far, newest
   first, then a 0 text that ends it. */
extern struct MenuTextItem level_help_history_items[151] __asm__("D_L00_001B3D40");
/* Fills level_help_history_items from the D_00141E08 table in reverse order, then ends it with 0. */
s32 build_reversed_active_message_list(void) __asm__("FUN_L00_0027adb0");

s32 build_reversed_active_message_list(void) {
    s32 i, j;
    for (i = 0; i < active_message_record_count; i = j) {
        j = i + 1;
        level_help_history_items[i].action = MENU_ACTION_NO_OP;
        level_help_history_items[i].text =
            *(u16 *)((char *)message_list_value_lookup +
                     active_message_records[active_message_record_count - j] * 4);
    }
    level_help_history_items[*(volatile s32 *)&active_message_record_count].text = 0;
    return 0;
}

struct MenuTextItem level_help_history_items[151] __asm__("D_L00_001B3D40") = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L01_001B41C0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L02_001B4340[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L03_001B3E40[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L04_001B3EC0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L05_001B41C0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L06_001B44C0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L07_001B3DC0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L08_001B4540[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L09_001B3EC0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L10_001B41C0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L11_001B4740[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L12_001B41C0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L13_001B4040[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L14_001B4440[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L15_001B4340[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L16_001B41C0[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L17_001B4640[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};

struct MenuTextItem D_L18_001B4740[151] = {
    {0x5277, MENU_ACTION_NO_OP},
    {0x5278, MENU_ACTION_NO_OP},
    {0x52b9, MENU_ACTION_NO_OP},
    {0x52c0, MENU_ACTION_NO_OP},
    /* Filled at run time from the help log. */
};
