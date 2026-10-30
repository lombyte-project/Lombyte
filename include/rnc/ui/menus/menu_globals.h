#ifndef LOMBYTE_RNC_UI_MENUS_MENU_GLOBALS_H
#define LOMBYTE_RNC_UI_MENUS_MENU_GLOBALS_H

#include "types.h"
#include "rnc/math/vector.h"

extern unsigned char menu_item_enabled_0 __asm__("D_0013D394") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_9 __asm__("D_0013D39D") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_16 __asm__("D_0013D3A4") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_17 __asm__("D_0013D3A5") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_18 __asm__("D_0013D3A6") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_19 __asm__("D_0013D3A7") __attribute__((section(".data")));
extern u8            menu_item_enabled_41 __asm__("D_0013D3BD") __attribute__((section(".data")));
extern unsigned char menu_item_enabled_68[] __asm__("D_0013D3D8");
extern s32           pause_action_list_mode __asm__("D_0015EE90");
extern s32           panel_clear_color __asm__("D_001601B0") __attribute__((sda));
extern s32           menu_fade_duration __asm__("D_001601B4") __attribute__((sda));
extern s32          *menu_level_indices __asm__("D_001601E0");
extern u8            requested_video_mode __asm__("D_0016034C");
extern s32           preview_request_count __asm__("D_00160350");

/* One record per item id. */
typedef struct MenuItemInfo {
    s32 help_text_id; /* 0x00: message id for get_help_message_text */
    u8  pad4[4];
    s32 slot; /* 0x08: equip slot the item goes in */
    u8  padC[0x2C];
    u16 unk38;         /* 0x38: copied into the pause item list */
    s16 preview_class; /* 0x3A: preview moby class, -1 for none */
    u8  pad3C[4];
    s16 unk40; /* 0x40: pause list value for the plain text */
    s16 unk42; /* 0x42: pause list value when item_text_variant is set */
    s16 unk44; /* 0x44: copied into the filtered item list */
    u8  pad46[6];
} MenuItemInfo;

extern MenuItemInfo menu_item_info[] __asm__("D_001863D0");

/* The active camera (D_00186F40): the view matrices update_camera_view_matrices
   builds, the camera position and basis, and the camera-switch state. */
struct PreviewCamera {
    f32  view_matrices[0x50]; /* 0x000 */
    Vec4 position;            /* 0x140 */
    Vec4 rotation;            /* 0x150: basis angles while the camera rows are not published */
    u8   pad160[0x258 - 0x160];
    f32  timed_control_step; /* 0x258: advance_timed_camera_control subtracts it from sequence */
    u8   pad25C[0x350 - 0x25C];
    Vec4 forward; /* 0x350 */
    Vec4 right;   /* 0x360 */
    Vec4 up;      /* 0x370 */
};

extern struct PreviewCamera preview_camera __asm__("D_00186F40");
/* Level object mobys, indexed by map icon id (copy_level_object_positions, update_map_icons). */
struct Moby;
extern struct Moby *menu_action_messages[] __asm__("D_00199478");
extern s32          help_msg_count[] __asm__("D_001996FC");
extern s32          selected_level_index[] __asm__("D_001A0314");
extern u8           preview_resource_bindings[] __asm__("D_001D59D8");

#endif /* LOMBYTE_RNC_UI_MENUS_MENU_GLOBALS_H */
