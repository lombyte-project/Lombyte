#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/save_data/loading_data_menu/FUN_002232d8.s", FUN_002232d8);
#else
#include "types.h"

struct LoadMenuControllerState
{
  u8 pad_0[0x1B4];
  s32 held_buttons;
  u8 pad_1B8[0xC];
  s32 pressed_buttons;
};
struct LoadMenuMemoryCardState
{
  u8 pad_0[0x8];
  s32 phase;
  u8 pad_C[0x8];
  s32 selected_save_slot;
  u8 pad_18[0xA8];
  s32 load_state;
  u8 pad_C4[0x10];
  s32 card_operation_state;
  u8 pad_D8[0x4];
  s32 pending_card_operation;
  s32 card_operation_progress;
  s32 load_failed;
  u8 pad_E8[0xC];
  s32 loaded_save_ready;
};
struct LoadMenuMixerState
{
  u8 pad_0[0x48];
  s32 group_0_volume;
  s32 group_1_volume;
  s32 group_2_volume;
  s32 group_3_volume;
  s32 group_4_volume;
  s32 group_5_volume;
};
struct LoadMenuState
{
  u8 pad_0[0x4];
  struct LoadMenuPage *page;
  s32 next_page;
  u8 pad_C[0x118];
  s32 busy;
  s32 load_pending;
  s32 status_text_id;
};
struct LoadMenuPage
{
  u8 pad_0[0x38];
  s32 back_page;
};
struct LoadMenuDescriptor
{
  u8 pad_0[0x14];
  s32 sound_owner;
  u8 pad_18[0x18];
  s32 flags;
  u8 pad_34[0xC];
  s32 selected_save_slot;
};
extern struct LoadMenuControllerState controller_state __asm__("D_0013C940");
extern struct LoadMenuMemoryCardState memory_card_state __asm__("D_0013D290");
extern s16 D_0013E05A;
extern struct LoadMenuMixerState mixer_state __asm__("D_0013E550");
extern s32 current_level_index __asm__("D_0015ED84");
extern s32 music_volume __asm__("D_0015EDEC");
extern s32 sound_volume __asm__("D_0015EDF0");
extern s32 selected_save_slot __asm__("D_0015EE34");
extern s32 mode_freeze_state __asm__("D_0015EEB0");
extern s32 mode_freeze_flags __asm__("D_0015EEB4");
extern struct LoadMenuState menu_state __asm__("D_001D5BF0");
extern void InitializeGlobalStateEntry(s32);
extern s32 mode_freeze_init() __asm__("func_001FBAB8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
s32 loading_data_menu(struct LoadMenuDescriptor *menu) __asm__("FUN_002232d8");

s32 loading_data_menu(struct LoadMenuDescriptor *menu)
{
  s32 previous_save_slot;
  s32 back_page;
  s32 next_save_slot;
  s32 buttons;
  s32 scaled_volume_80;
  s32 scaled_volume_70;
  previous_save_slot = menu->selected_save_slot;
  if (menu_state.load_pending != 0)
  {
    if ((memory_card_state.card_operation_state < 3) && (memory_card_state.pending_card_operation < 0))
    {
      menu_state.load_pending = 0;
      if (memory_card_state.load_failed != 0)
      {
        mode_freeze_flags |= 0x100;
        mode_freeze_init(3, menu_state.page);
        return 0;
      }
      memory_card_state.loaded_save_ready = 1;
      scaled_volume_80 = ((s32) (sound_volume * 8)) / 10;
      mixer_state.group_1_volume = music_volume;
      scaled_volume_70 = ((s32) (sound_volume * 7)) / 10;
      mixer_state.group_0_volume = scaled_volume_80;
      mixer_state.group_2_volume = scaled_volume_80;
      mixer_state.group_3_volume = ((s32) (sound_volume * 7)) / 10;
      scaled_volume_70 = ((s32) (sound_volume * 7)) / 10;
      mixer_state.group_5_volume = sound_volume;
      mixer_state.group_4_volume = scaled_volume_70;
      InitializeGlobalStateEntry(current_level_index);
      D_0013E05A = 0;
    }
    else
    {
      return 0;
    }
  }
  if (controller_state.pressed_buttons & 0xD00)
  {
    if (menu_state.busy == 0)
    {
      return 1;
    }
  }
  if (controller_state.pressed_buttons & 0x10)
  {
    back_page = menu_state.page->back_page;
    if (back_page != 0)
    {
      menu_state.next_page = back_page;
    }
    else
      if (menu_state.busy == 0)
    {
      return -1;
    }
  }
  if ((mode_freeze_state != 0x10) && (mode_freeze_state != 1))
  {
    menu_state.next_page = menu_state.page->back_page;
    return 0;
  }
  if (((memory_card_state.card_operation_state < 3) && (memory_card_state.pending_card_operation < 0)) && (memory_card_state.phase == 2))
  {
    if (menu->flags & 1)
    {
      buttons = controller_state.held_buttons;
    }
    else
    {
      buttons = controller_state.pressed_buttons;
    }
    menu->selected_save_slot = (s32) selected_save_slot;
    if ((buttons & 0x1000) && (selected_save_slot != 0))
    {
      menu->selected_save_slot = (s32) (selected_save_slot - 1);
    }
    next_save_slot = menu->selected_save_slot;
    if ((buttons & 0x4000) && (next_save_slot < 4))
    {
      menu->selected_save_slot = (s32) (next_save_slot + 1);
    }
    selected_save_slot = menu->selected_save_slot;
    if (((buttons & 0x40) && (memory_card_state.phase == 2)) && ((*((s32 *) ((((u8 *) (&memory_card_state)) + (menu->selected_save_slot * 0x1C)) + 0x20))) >= 0))
    {
      allocate_voice_for_target_entry(0, 0x11, menu->sound_owner);
      memory_card_state.load_state = 0;
      memory_card_state.selected_save_slot = (s32) menu->selected_save_slot;
      if (memory_card_state.pending_card_operation < 0)
      {
        memory_card_state.card_operation_progress = 0;
        memory_card_state.pending_card_operation = 0xD;
      }
      menu_state.load_pending = 1;
      menu_state.status_text_id = 0x4FB6;
    }
    if (menu->selected_save_slot != previous_save_slot)
    {
      allocate_voice_for_target_entry(1, 0x11, menu->sound_owner);
    }
  }
  return 0;
}
#endif /* NON_MATCHING */
