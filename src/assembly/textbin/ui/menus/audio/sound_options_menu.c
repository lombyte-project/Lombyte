#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/audio/sound_options_menu/FUN_0021cb30.s", FUN_0021cb30);
#else
#include "types.h"
#include "sda.h"

struct SoundMenuInput {
    u8 pad_0[0x1C0];
    s32 held_buttons;
    s32 pressed_buttons;
};

struct SoundMenuMixer {
    u8 pad_0[0x48];
    s32 group_0_volume;
    s32 group_1_volume;
    s32 group_2_volume;
    s32 group_3_volume;
    s32 group_4_volume;
    s32 group_5_volume;
};

struct SoundMenuNavigation {
    u8 pad_0[0x4];
    struct SoundMenuNavigationEntry *page;
    s32 next_page;
    u8 pad_C[0x118];
    s32 busy;
};

struct SoundMenuNavigationEntry {
    u8 pad_0[0x38];
    s32 back_page;
};

struct SoundMenu {
    u8 pad_0[0x14];
    s32 sound_owner;
    u8 pad_18[0x18];
    s32 flags;
    u8 pad_34[0xC];
    s32 selected_option;
};

extern struct SoundMenuInput D_0013C940;
extern struct SoundMenuMixer D_0013E550;
extern struct SoundMenuNavigation D_001D5BF0;
extern s32 D_001D5D14 NOT_SDA;
extern s32 *D_001D5BF4 NOT_SDA;
extern s32 D_0015EDEC MACRO_ADDR;
extern s32 D_0015EDF0 MACRO_ADDR;
extern s32 D_0015EDE8 MACRO_ADDR;
extern s32 D_001A0314 NOT_SDA;
extern s32 *D_001601E0 __attribute__((sda));
extern s32 allocate_voice_for_target_entry(s32 flags, s32 sound_index, s32 sound_owner) __asm__("func_0022DA68");
void snd_set_playback_mode(s32 menu) __asm__("FUN_0012e240");

s32 sound_options_menu(struct SoundMenu *menu) __asm__("FUN_0021cb30");

s32 sound_options_menu(struct SoundMenu *menu) {
    s32 previous_selection;
    s32 *first_volume;
    s32 previous_playback_mode;
    s32 previous_first_volume;
    s32 previous_second_volume;
    s32 scaled_first_volume;

    if (D_0013C940.pressed_buttons & 0xD00) {
        if (D_001D5D14 == 0) {
            return 1;
        }
    }
    if (D_0013C940.pressed_buttons & 0x10) {
        s32 navigation_value = D_001D5BF0.page->back_page;

        if (navigation_value != 0) {
            D_001D5BF0.next_page = navigation_value;
        } else if (D_001D5BF0.busy == 0) {
            return -1;
        }
    }
    previous_selection = menu->selected_option;
    if (D_0013C940.pressed_buttons & 0x1000) {
        menu->selected_option = (previous_selection + 2) % 3;
    }
    if (D_0013C940.pressed_buttons & 0x4000) {
        menu->selected_option = (menu->selected_option + 1) % 3;
    }
    if ((menu->selected_option != previous_selection) || (D_001D5BF4[0x20] != 0)) {
        allocate_voice_for_target_entry(1, 0x11, menu->sound_owner);
        if (menu->flags & 0x20) {
            D_001A0314 = D_001601E0[menu->selected_option];
        }
    }
    previous_first_volume = D_0015EDF0;
    previous_second_volume = D_0015EDEC;
    first_volume = &D_0015EDF0;
    if (D_0013C940.held_buttons & 0x2000) {
        if (menu->selected_option == 0) {
            D_0015EDF0 = (0x400 < D_0015EDF0 + 3) ? 0x400 : D_0015EDF0 + 3;
        }
        if (menu->selected_option == 1) {
            D_0015EDEC = (0x400 < D_0015EDEC + 3) ? 0x400 : D_0015EDEC + 3;
        }
    }
    if (D_0013C940.held_buttons & 0x8000) {
        if (menu->selected_option == 0) {
            D_0015EDF0 = (D_0015EDF0 - 3 <= 0) ? 0 : D_0015EDF0 - 3;
        }
        if (menu->selected_option == 1) {
            D_0015EDEC = (D_0015EDEC - 3 <= 0) ? 0 : D_0015EDEC - 3;
        }
    }
    if ((previous_second_volume != D_0015EDEC) || (previous_first_volume != *first_volume)) {
        /* Retail assigns both groups 1 and 2 from the second slider here. */
        D_0013E550.group_0_volume = *first_volume * 8 / 10;
        D_0013E550.group_2_volume = D_0013E550.group_1_volume = D_0015EDEC;
        scaled_first_volume = *first_volume * 7 / 10;
        D_0013E550.group_4_volume = scaled_first_volume;
        D_0013E550.group_5_volume = *first_volume;
        D_0013E550.group_3_volume = scaled_first_volume;
    }
    if (D_0013C940.pressed_buttons & 0x40) {
        if (menu->selected_option == 2) {
            D_0015EDE8 = !D_0015EDE8;
        }
        snd_set_playback_mode(!D_0015EDE8);
        allocate_voice_for_target_entry(0, 0x11, menu->sound_owner);
    }
    return 0;
}
#endif /* NON_MATCHING */
