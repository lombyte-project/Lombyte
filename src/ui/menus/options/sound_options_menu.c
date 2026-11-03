#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/ui/menus/menu_system.h"
#include "sda.h"
#include "rnc/audio/mixer/snd_set_playback_mode.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/game/options.h"

struct SoundMenu {
    u8  pad_0[0x14];
    s32 sound_owner;
    u8  pad_18[0x18];
    s32 flags;
    u8  pad_34[0xC];
    s32 selected_option;
};

extern s32 D_001D5D14 NOT_SDA;
extern s32            music_volume __asm__("D_0015EDEC") MACRO_ADDR;
extern s32            sound_volume __asm__("D_0015EDF0") MACRO_ADDR;
extern s32            playback_mode __asm__("D_0015EDE8") MACRO_ADDR;
extern s32            selected_level_index_lui __asm__("D_001A0314") NOT_SDA;
extern s32            allocate_voice_for_target_entry(s32 flags,
                                                      s32 sound_index,
                                                      s32 sound_owner) __asm__("func_0022DA68");

s32 sound_options_menu(struct SoundMenu *menu) __asm__("FUN_0021cb30");

extern s32 *menu_level_indices __asm__("D_001601E0") __attribute__((sda));

s32 sound_options_menu(struct SoundMenu *menu) {
    s32  previous_selection;
    s32 *sound_volume_ptr;
    s32  previous_music_volume;
    s32  previous_sound_volume;
    s32  scaled_sound_volume;

    if ((((s32)controller_state.pressed_unmasked) & 0xD00) && (D_001D5D14 == 0)) {
        return 1;
    }
    if (((s32)controller_state.pressed_unmasked) & 0x10) {
        struct MenuPage *back_page = menu_system.current->back;

        if (back_page != 0) {
            menu_system.next = back_page;
        } else if (menu_system.close_locked == 0) {
            return -1;
        }
    }
    previous_selection = menu->selected_option;
    if (((s32)controller_state.pressed_unmasked) & 0x1000) {
        menu->selected_option = (previous_selection + 2) % 3;
    }
    if (((s32)controller_state.pressed_unmasked) & 0x4000) {
        menu->selected_option = (menu->selected_option + 1) % 3;
    }
    if ((menu->selected_option != previous_selection) ||
        (menu_system.current->pending_focus != 0)) {
        allocate_voice_for_target_entry(1, 0x11, menu->sound_owner);
        if (menu->flags & 0x20) {
            selected_level_index_lui = menu_level_indices[menu->selected_option];
        }
    }
    previous_music_volume = music_volume;
    previous_sound_volume = sound_volume;
    sound_volume_ptr = &sound_volume;
    if (((s32)controller_state.held_unmasked) & 0x2000) {
        if (menu->selected_option == 0) {
            sound_volume = (previous_sound_volume + 3 < 0x401) ? previous_sound_volume + 3 : 0x400;
        }
        if (menu->selected_option == 1) {
            music_volume = (previous_music_volume + 3 < 0x401) ? previous_music_volume + 3 : 0x400;
        }
    }
    if (((s32)controller_state.held_unmasked) & 0x8000) {
        if (menu->selected_option == 0) {
            sound_volume = (sound_volume - 3 <= 0) ? 0 : sound_volume - 3;
        }
        if (menu->selected_option == 1) {
            music_volume = (music_volume - 3 <= 0) ? 0 : music_volume - 3;
        }
    }
    if ((previous_music_volume != music_volume) || (previous_sound_volume != *sound_volume_ptr)) {
        voice_pool.group_0_volume = *sound_volume_ptr * 8 / 10;
        voice_pool.group_2_volume = voice_pool.group_1_volume = music_volume;
        voice_pool.group_3_volume = *sound_volume_ptr * 7 / 10;
        scaled_sound_volume = *sound_volume_ptr * 7 / 10;
        voice_pool.group_4_volume = scaled_sound_volume;
        voice_pool.group_5_volume = *sound_volume_ptr;
    }
    if (((s32)controller_state.pressed_unmasked) & 0x40) {
        if (menu->selected_option == 2) {
            playback_mode = !playback_mode;
        }
        snd_set_playback_mode(!playback_mode);
        allocate_voice_for_target_entry(0, 0x11, menu->sound_owner);
    }
    return 0;
}

s32 playback_mode = 1;

s32 music_volume = 0x2cc;

s32 sound_volume = 0x400;
