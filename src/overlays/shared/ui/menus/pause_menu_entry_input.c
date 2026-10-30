/* Pause menu: entry input. */
#include "types.h"
#include "rnc/game/overlay_game_state.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/globals.h"
#include "sda.h"

typedef struct {
    char         pad[0x1C4];
    volatile s32 f1C4;
} G_7c410;

extern G_7c410 D_0013C940_7c410 __asm__("D_0013C940");

extern s32 D_L00_00184414 NOT_SDA;
s32                       pause_menu_entry_input(s32 object_address) __asm__("FUN_L00_0027c410");

s32 pause_menu_entry_input(s32 object_address) {
    s32               flags_snapshot = D_0013C940_7c410.f1C4;
    s32               owner_unk38_address;
    struct GameState *game_state;
    if (flags_snapshot & 0xD00) {
        if (*(s32 *)(object_address + 0x30) & 0x20) {
            D_L00_00184414 = current_level_index;
        }
        return -1;
    }
    if (flags_snapshot & 0x10) {
        if (*(s32 *)(object_address + 0x30) & 0x20) {
            D_L00_00184414 = current_level_index;
        }
        game_state = &D_L00_001B9CF0;
        owner_unk38_address = (s32)game_state->owner->unk38;
        if (owner_unk38_address != 0) {
            game_state->unk8 = (struct Own *)owner_unk38_address;
        } else if (game_state->unk124 == 0) {
            return -1;
        }
    }
    if (D_0013C940_7c410.f1C4 & 0x20) {
        D_0013D384 = 0;
        D_L00_0015F5C0 = -1;
        D_L00_0015F570 = 1;
        D_L00_0015F5D8 = 1;
    }
    return 0;
}

s32 D_L01_00184894 NOT_SDA = 0;
s32 D_L02_00184A14 NOT_SDA = 0;
s32 D_L03_00184514 NOT_SDA = 0;
s32 D_L04_00184594 NOT_SDA = 0;
s32 D_L05_00184894 NOT_SDA = 0;
s32 D_L06_00184B94 NOT_SDA = 0;
s32 D_L07_00184494 NOT_SDA = 0;
s32 D_L08_00184C14 NOT_SDA = 0;
s32 D_L09_00184594 NOT_SDA = 0;
s32 D_L10_00184894 NOT_SDA = 0;
s32 D_L11_00184E14 NOT_SDA = 0;
s32 D_L12_00184894 NOT_SDA = 0;
s32 D_L13_00184714 NOT_SDA = 0;
s32 D_L14_00184B14 NOT_SDA = 0;
s32 D_L15_00184A14 NOT_SDA = 0;
s32 D_L16_00184894 NOT_SDA = 0;
s32 D_L17_00184D14 NOT_SDA = 0;
s32 D_L18_00184E14 NOT_SDA = 0;
