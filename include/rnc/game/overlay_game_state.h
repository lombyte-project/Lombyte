#ifndef LOMBYTE_RNC_GAME_OVERLAY_GAME_STATE_H
#define LOMBYTE_RNC_GAME_OVERLAY_GAME_STATE_H

#include "types.h"
#include "rnc/math/vector.h"

struct Mth;
struct NO;

/* A pause menu page; the level copy of struct MenuPage (rnc/ui/menus/menu_system.h). */
struct Own {
    s32         ids[14]; /* 0x00: animation per page moby */
    struct Own *back;    /* 0x38: page for the back button (pad 0x10); 0 = none */
    s32         state;   /* 0x3C */
    struct Mth *focus;   /* 0x40: object with input focus */
    struct NO  *objs[14];
    u8          pad7C[4];
    struct Mth *pending_focus; /* 0x80 */
};

/* Shared Level 00 state of the pause menu; the level copy of struct MenuSystem
   (rnc/ui/menus/menu_system.h), whose names it uses. */
struct GameState {
    s32         state;
    struct Own *owner;
    struct Own *next;   /* 0x08: requested page */
    s32  close_request; /* 0x0C: action on close: 2 level, 3..6 movies and scenes, 7 slideshow */
    s32  graphics_workspace; /* 0x10: compressed HUD data and draw buffer chunks */
    s32  timer;              /* 0x14: 12 on a page switch, 2 on close; counts down */
    s32  saved_texture_start;
    u8   pad1C[0x14];
    s32  saved_items[4];
    Vec4 saved_camera_position; /* 0x40: g_debug_camera.position while paused */
    Vec4 saved_camera_angles;   /* 0x50 */
    u8   pad60[0x6C];
    s32  saved_seq_count;
    struct Own *previous; /* 0xD0: page left by the last switch */
    u8          padD4[4];
    s32         goodies_tab_shown; /* 0xD8: page moby 6 is drawn only while set */
    u8          padDC[8];
    void       *travel_level;       /* 0xE4: level, movie or scene for close_request */
    f32         saved_tan_half_fov; /* 0xE8 */
    void       *unkEC;
    struct Own *pending_owner;
    s32         pending_state;
    u8          padF8[4];
    s32         first_base;         /* 0xFC: render archive allocation */
    s32         second_base;        /* 0x100 */
    s32         preview_workspace;  /* 0x104: decompression destination */
    s32         help_text_buffer;   /* 0x108 */
    s32         stream_buffer_base; /* 0x10C */
    s32         progress;
    u8          pad114[0x124 - 0x114];
    s32         close_locked; /* 0x124: nonzero blocks back from closing the menu */
    u8          pad128[0x130 - 0x128];
    s32         saved_decode_mode; /* 0x130: decode_mode before a menu movie */
    u8          pad134[0x13C - 0x134];
    s32         memcard_icon_hidden;        /* 0x13C: skips the busy card icon for one frame */
    s32         resource_table_class;       /* 0x140: oclass at the last resource table select */
    s32         last_resource_table_toggle; /* 0x144 */
    s32         stash_handles[20];          /* 0x148: FUN_L00_00276078 */
};

extern struct GameState D_L00_001B9CF0 __attribute__((section(".data")));

#endif
