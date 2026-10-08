#include "types.h"
#include "rnc/ui/menus/menu_system.h"
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
extern s32 D_001D5D14 NOT_SDA;
extern s32 music_volume __asm__("D_0015EDEC") MACRO_ADDR;
extern s32 sound_volume __asm__("D_0015EDF0") MACRO_ADDR;
extern s32 playback_mode __asm__("D_0015EDE8") MACRO_ADDR;
extern s32 D_001A0314 NOT_SDA;
extern s32 *D_001601E0 __attribute__((sda));
extern s32 allocate_voice_for_target_entry(s32 flags, s32 sound_index,
                                           s32 sound_owner) __asm__("func_0022DA68");
void snd_set_playback_mode(s32 menu) __asm__("FUN_0012e240");

s32 sound_options_menu(struct SoundMenu *menu) __asm__("FUN_0021cb30");

s32 sound_options_menu(struct SoundMenu *menu) {
    s32 previous_selection;
    s32 *sound_volume_ptr;
    s32 previous_music_volume;
    s32 previous_sound_volume;
    s32 scaled_sound_volume;

    if ((D_0013C940.pressed_buttons & 0xD00) && (D_001D5D14 == 0)) {
        return 1;
    }
    if (D_0013C940.pressed_buttons & 0x10) {
        struct MenuPage *back_page = menu_system.current->back;

        if (back_page != 0) {
            menu_system.next = back_page;
        } else if (menu_system.close_locked == 0) {
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
    if ((menu->selected_option != previous_selection) || (menu_system.current->pending_focus != 0)) {
        allocate_voice_for_target_entry(1, 0x11, menu->sound_owner);
        if (menu->flags & 0x20) {
            D_001A0314 = D_001601E0[menu->selected_option];
        }
    }
    previous_music_volume = music_volume;
    previous_sound_volume = sound_volume;
    sound_volume_ptr = &sound_volume;
    if (D_0013C940.held_buttons & 0x2000) {
        if (menu->selected_option == 0) {
            sound_volume = (previous_sound_volume + 3 < 0x401) ? previous_sound_volume + 3 : 0x400;
        }
        if (menu->selected_option == 1) {
            music_volume = (previous_music_volume + 3 < 0x401) ? previous_music_volume + 3 : 0x400;
        }
    }
    if (D_0013C940.held_buttons & 0x8000) {
        if (menu->selected_option == 0) {
            sound_volume = (sound_volume - 3 <= 0) ? 0 : sound_volume - 3;
        }
        if (menu->selected_option == 1) {
            music_volume = (music_volume - 3 <= 0) ? 0 : music_volume - 3;
        }
    }
    if ((previous_music_volume != music_volume) || (previous_sound_volume != *sound_volume_ptr)) {
        D_0013E550.group_0_volume = *sound_volume_ptr * 8 / 10;
        D_0013E550.group_2_volume = D_0013E550.group_1_volume = music_volume;
        D_0013E550.group_3_volume = *sound_volume_ptr * 7 / 10;
        scaled_sound_volume = *sound_volume_ptr * 7 / 10;
        D_0013E550.group_4_volume = scaled_sound_volume;
        D_0013E550.group_5_volume = *sound_volume_ptr;
    }
    if (D_0013C940.pressed_buttons & 0x40) {
        if (menu->selected_option == 2) {
            playback_mode = !playback_mode;
        }
        snd_set_playback_mode(!playback_mode);
        allocate_voice_for_target_entry(0, 0x11, menu->sound_owner);
    }
    return 0;
}
