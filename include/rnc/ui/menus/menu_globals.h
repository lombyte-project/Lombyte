#ifndef LOMBYTE_RNC_UI_MENUS_MENU_GLOBALS_H
#define LOMBYTE_RNC_UI_MENUS_MENU_GLOBALS_H

#include "types.h"

extern unsigned char menu_item_enabled_0 __asm__("D_0013D394") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_9 __asm__("D_0013D39D") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_16 __asm__("D_0013D3A4") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_17 __asm__("D_0013D3A5") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_18 __asm__("D_0013D3A6") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_19 __asm__("D_0013D3A7") __attribute__((section(".data")));
extern u8 menu_item_enabled_41 __asm__("D_0013D3BD") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_68[] __asm__("D_0013D3D8");
extern s32 pause_action_list_mode __asm__("D_0015EE90");
extern s32 panel_clear_color __asm__("D_001601B0") __attribute__((sda));
extern s32 menu_fade_duration __asm__("D_001601B4") __attribute__((sda));
extern s32 *menu_level_indices __asm__("D_001601E0");
extern u8 requested_video_mode __asm__("D_0016034C");
extern s32 preview_request_count __asm__("D_00160350");
extern char preview_binding_table[] __asm__("D_001863D0");
extern char preview_camera[] __asm__("D_00186F40");
extern s32 menu_action_messages[] __asm__("D_00199478");
extern s32 help_msg_count[] __asm__("D_001996FC");
extern s32 selected_level_index[] __asm__("D_001A0314");
extern u8 preview_resource_bindings[] __asm__("D_001D59D8");

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_GLOBALS_H */
