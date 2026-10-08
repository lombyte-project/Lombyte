#ifndef LOMBYTE_RNC_UI_MAP_MAP_ICON_H
#define LOMBYTE_RNC_UI_MAP_MAP_ICON_H

#include "types.h"

/* One entry of a level's map icon list (MapState.icons, D_001A2BC0[level]).
   The list ends at the first entry with flags bit 0x4 set. Layout from
   update_map_icons, format_menu_item_text and the draw_map_overlay draft. */
struct MapIcon {
    s16 id;            /* 0x00: moby index into D_00199478 (current level) or D_0013D5B0; -1..-9 are fixed positions */
    s16 link;          /* 0x02: -1 always active, else D_001A2C10.links[link].active decides */
    u16 flags;         /* 0x04: 0x4 ends the list, 0x10 label shown (update_map_icons) */
    u16 texture_id;    /* 0x06: draw_map_overlay skips the icon when zero */
    s16 frame_index;   /* 0x08: get_icon_frame argument in draw_map_overlay */
    s16 label_text_id; /* 0x0A: help text id of the label (get_help_message_text in format_menu_item_text); 0 = no label */
    s16 label_item;    /* 0x0C: D_001DFFB0 index whose name fills the label's "%b" */
    u16 label_width;   /* 0x0E: text window width measured by update_map_icons */
    u16 label_height;  /* 0x10: text window height measured by update_map_icons */
    s16 label_offset_x; /* 0x12: draw_map_overlay label placement */
    s16 label_offset_y; /* 0x14 */
    u8 pad16[0x2];
    f32 x;     /* 0x18: map position 0..1 (world_to_map_coords output) */
    f32 y;     /* 0x1C */
    f32 angle; /* 0x20: copied from the moby's z rotation (+0x48); draw_rotated_sprite angle */
    s32 active; /* 0x24: drawn only when nonzero */
}; /* size 0x28 */

/* Map marker list entry (MapState.markers), drawn by draw_map_markers. */
struct MapMarker {
    f32 x; /* 0x00: world x, through world_to_map_coords */
    f32 y; /* 0x04: world y */
    s32 texture_group;            /* 0x08: -1 draws a plain rect, else get_icon_frame group */
    s32 texture_variant_or_color; /* 0x0C: rect colour when group is -1, else frame variant */
}; /* size 0x10 */

#endif /* LOMBYTE_RNC_UI_MAP_MAP_ICON_H */
