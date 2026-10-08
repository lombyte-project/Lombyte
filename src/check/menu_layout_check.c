/* Offset checks for menu_system.h and menu_screen.h; `make check` compiles
   this file with the game compiler. gcc 2.95 has no _Static_assert: a
   negative array size fails the build. */
#include "rnc/ui/menus/menu_system.h"
#include "rnc/ui/menus/menu_screen.h"

#define CHECK(type, name, field, off) \
    typedef char check_##name[ \
        ((unsigned long)&((struct type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(type, size) \
    typedef char size_check_##type[(sizeof(struct type) == (size)) ? 1 : -1]

CHECK(MenuPage, page_back, back, 0x38);
CHECK(MenuPage, page_state, state, 0x3C);
CHECK(MenuPage, page_focus, focus, 0x40);
CHECK(MenuPage, page_screens, screens, 0x44);
CHECK(MenuPage, page_pending_focus, pending_focus, 0x80);
CHECK(MenuPage, page_confirmed, confirmed, 0x84);
SIZE_CHECK(MenuPage, 0x88);

CHECK(MenuSystem, current, current, 0x4);
CHECK(MenuSystem, next, next, 0x8);
CHECK(MenuSystem, close_request, close_request, 0xC);
CHECK(MenuSystem, timer, timer, 0x14);
CHECK(MenuSystem, saved_texture_start, saved_texture_start, 0x18);
CHECK(MenuSystem, current_gadget, current_gadget, 0x1C);
CHECK(MenuSystem, equipped, equipped, 0x30);
CHECK(MenuSystem, stream_buffer, stream_buffer, 0xA0);
CHECK(MenuSystem, resource_buffer_address, resource_buffer_address, 0xB0);
CHECK(MenuSystem, read_offset, read_offset, 0xBC);
CHECK(MenuSystem, loaded_animation, loaded_animation, 0xC8);
CHECK(MenuSystem, pending_buffer, pending_buffer, 0xCB);
CHECK(MenuSystem, streamed_animation_base, streamed_animation_base, 0xCC);
CHECK(MenuSystem, previous, previous, 0xD0);
CHECK(MenuSystem, confirm_kind, confirm_kind, 0xD4);
CHECK(MenuSystem, special_slot_enabled, special_slot_enabled, 0xD8);
CHECK(MenuSystem, action_value, action.value, 0xE4);
CHECK(MenuSystem, action_message, action.message, 0xEC);
CHECK(MenuSystem, action_return_page, action.return_page, 0xF0);
CHECK(MenuSystem, action_mode, action.mode, 0xF4);
CHECK(MenuSystem, unkF8, unkF8, 0xF8);
CHECK(MenuSystem, unkFC, unkFC, 0xFC);
CHECK(MenuSystem, help_text_buffer, help_text_buffer, 0x108);
CHECK(MenuSystem, update_count, update_count, 0x110);
CHECK(MenuSystem, resource_table_toggle, resource_table_toggle, 0x118);
CHECK(MenuSystem, close_blocked, close_blocked, 0x124);
CHECK(MenuSystem, save_pending, save_pending, 0x128);
CHECK(MenuSystem, message_id, message_id, 0x12C);
CHECK(MenuSystem, unk138, unk138, 0x138);
CHECK(MenuSystem, last_resource_table_toggle, last_resource_table_toggle, 0x144);
SIZE_CHECK(MenuSystem, 0x148);

CHECK(MenuScreen, update, update, 0x0);
CHECK(MenuScreen, enter, enter, 0x8);
CHECK(MenuScreen, leave, leave, 0xC);
CHECK(MenuScreen, moby, moby, 0x14);
CHECK(MenuScreen, x, x, 0x18);
CHECK(MenuScreen, y, y, 0x1C);
CHECK(MenuScreen, width, width, 0x20);
CHECK(MenuScreen, height, height, 0x24);
CHECK(MenuScreen, raw_unk34, data.raw.unk34, 0x34);
CHECK(MenuScreen, raw_unk50, data.raw.unk50, 0x50);
CHECK(MenuScreen, raw_end, data.raw.pad_58[7], 0x5F);
CHECK(MenuScreen, options_selection, data.options.selection, 0x38);
CHECK(MenuScreen, choices_selection, data.choices.selection, 0x38);
CHECK(MenuScreen, missions_count, data.missions.count, 0x7C);
CHECK(MenuScreen, grid_cells, data.grid.cells, 0x48);
CHECK(MenuScreen, grid_right, data.grid.right, 0x58);
CHECK(MenuScreen, icons_list, data.icons.list, 0x48);
CHECK(MenuScreen, icons_first_y, data.icons.first_y, 0x5C);
CHECK(MenuScreen, save_slot, data.save.slot, 0x40);
CHECK(MenuScreen, save_step, data.save.step, 0x4C);
CHECK(MenuScreen, prompt_buffer, data.prompt.buffer, 0x54);
CHECK(MenuScreen, slots_cursor, data.slots.cursor, 0x50);
CHECK(MenuScreen, cycle_selection, data.cycle.selection, 0x54);
CHECK(MenuScreen, list_scroll, data.list.scroll, 0x44);
CHECK(MenuScreen, screen_stream_buffer, data.stream.buffer, 0x48);
CHECK(MenuScreen, stream_read_offset, data.stream.read_offset, 0x60);
CHECK(MenuScreen, preview_second_moby, data.preview.second_moby, 0x48);
CHECK(MenuScreen, label_fade_timer, data.label.fade_timer, 0x44);
CHECK(MenuScreen, label_value_variant, data.label.value_variant, 0x4C);
SIZE_CHECK(MenuOption, 0x14);
SIZE_CHECK(MenuChoice, 0x18);
SIZE_CHECK(MenuTextItem, 0xC);
