#include "types.h"
#include "rnc/game/level_object_states.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/hero/hero.h"
#include "rnc/ui/map/map_icon.h"
#include "rnc/ui/map/map_state.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/game/level_state.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/game/missions/mission_item.h"
#include "rnc/world/level_id.h"

/* Map screen icon refresh: places the fixed icons (-1..-9), projects the
   others through world_to_map_coords, then sizes each label box. */

typedef struct {
    u8  pad0[0x24];
    s16 active;
    u8  pad26[2];
} MapIconLink;

/* The icon list of each level (update_map_icons only reads it for level < 19). */
extern struct MapIcon *map_icon_lists_by_level[19] __asm__("D_001A2BC0");
extern s32             D_001A01F4[];
extern s32             D_0015FD60 __attribute__((sda));

extern s32 D_0013D5BC[];

extern struct {
    u8           pad0[0x10];
    MapIconLink *links;
} D_001A2C10;

extern void
world_to_map_coords(f32 x, f32 y, f32 *map_x, f32 *map_y, s32 level) __asm__("func_00208408");
extern void format_menu_item_text(s32 idx, char *dst) __asm__("func_00208280");
extern void
font_print_window_small(struct TextRegion *, long, char *, int) __asm__("func_001F75F0");

void update_map_icons(s32 level, s32 flag) __asm__("FUN_0020bf90");

void update_map_icons(s32 level, s32 flag) {
    char            label_text[128];
    f32             map_x;
    f32             map_y;
    struct MapIcon *icon;
    s32             icon_index;

    if (level < 19 && level_visit_state[level] != 0) {
        level_map_selection.icons = map_icon_lists_by_level[level];
    } else {
        level_map_selection.icons = 0;
    }

    if (hero.motion.pos.f[2] != 0.0f && level == current_level_index && flag) {
        world_to_map_coords(hero.motion.pos.f[0],
                            hero.motion.pos.f[1],
                            &map_x,
                            &map_y,
                            D_0015FD60 ? level + 100 : level);
        level_map_selection.pan_x[level] = (s32)(map_x * 4096.0f) << 16;
        level_map_selection.pan_y[level] = (s32)(map_y * 4096.0f) << 16;
    } else {
        /* D_001A01F4 is &level_map_selection.posx: a null-guarded reset that can
           never run, but retail still emits it with p folded to 0. */
        s32 *pan_reset_base = D_001A01F4;
        if (pan_reset_base == 0) {
            pan_reset_base[level] = 0x8000000;
            pan_reset_base[level + 20] = 0x8000000;
        }
    }

    if (level_map_selection.icons == 0) {
        return;
    }

    if (!(level_map_selection.icons->flags & 4)) {
        icon_index = 0;
        do {
            icon = &level_map_selection.icons[icon_index];
            if (icon->id == -1) {
                icon->x = 0.234375f;
                icon->y = 0.30078125f;
            } else if (icon->id == -2) {
                icon->x = 0.5390625f;
                icon->y = 0.365234375f;
            } else if (icon->id == -3) {
                icon->x = 0.58203125f;
                icon->y = 0.6875f;
            } else if (icon->id == -4) {
                icon->x = 0.720703125f;
                icon->y = 0.728515625f;
            } else if (icon->id == -5) {
                icon->x = 0.384765625f;
                icon->y = 0.396484375f;
            } else if (icon->id == -7) {
                icon->x = 0.8671875f;
                icon->y = 0.23046875f;
            } else if (icon->id == -8) {
                icon->x = 0.48046875f;
                icon->y = 0.5703125f;
            } else if (icon->id == -9) {
                icon->x = 0.625f;
                icon->y = 0.72265625f;
            } else {
                if (level == current_level_index) {
                    if (menu_action_messages[icon->id] != 0) {
                        world_to_map_coords(menu_action_messages[icon->id]->pos.x,
                                            menu_action_messages[icon->id]->pos.y,
                                            &icon->x,
                                            &icon->y,
                                            level);
                        level_map_selection.icons[icon_index].angle =
                            menu_action_messages[icon->id]->rot.z;
                    }
                } else {
                    world_to_map_coords(level_object_states[icon->id].x,
                                        level_object_states[icon->id].y,
                                        &icon->x,
                                        &icon->y,
                                        level);
                    level_map_selection.icons[icon_index].angle = level_object_states[icon->id].z;
                }
            }
            icon_index++;
        } while (!(level_map_selection.icons[icon_index].flags & 4));
    }

    for (icon_index = 0; !(level_map_selection.icons[icon_index].flags & 4); icon_index++) {
        icon = &level_map_selection.icons[icon_index];
        icon->flags &= ~0x10;
        icon->active = 0;
        if (icon->link == -1) {
            icon->active = 1;
        } else {
            icon->active = D_001A2C10.links[icon->link].active == 1;
        }
        if ((icon->flags & 0x1000) && (D_0013D5BC[icon->id * 4] ^ 1) & 1) {
            icon->active = 0;
        }
        if (level_map_selection.icons[icon_index].label_text_id != 0) {
            s32 label_height_changed = 0;
            s16 previous_label_height;

            level_map_selection.icons[icon_index].flags |= 0x10;
            format_menu_item_text(icon_index, label_text);
            {
                struct TextRegion text_window = {
                    0, icon->label_height, 0, icon->label_width, 4, 4, 0, 0, 0xF, 4};
                font_print_window_small(&text_window, 0x80FFA888L, label_text, -1);
                previous_label_height = text_window.rendered_height;
                icon->label_height = text_window.rendered_height + 8;
                do {
                    text_window.right -= 4;
                    font_print_window_small(&text_window, 0x80FFA888L, label_text, -1);
                    if (text_window.rendered_height != previous_label_height) {
                        label_height_changed = 1;
                    }
                } while (!label_height_changed);
                icon->label_width = text_window.right + 4;
            }
        }
    }
}

extern __typeof__(update_map_icons) func_0020BF90 __attribute__((alias("FUN_0020bf90")));

/* The missions' callbacks and kind-7 conditions (src/game/missions). */
extern int FUN_0020bd30(void);
extern int FUN_0020bd58(void);
extern int FUN_0020bd90(void);
extern int FUN_0020bdc0(void);
extern int FUN_0020bde8(void);
extern int FUN_0020be10(void);
extern int FUN_0020be38(void);
extern int FUN_0020bed8(void);
extern int FUN_0020bf08(void);
extern int is_item_available(int item_index) __asm__("FUN_0020be68");
extern int get_map_point_mission_status(void) __asm__("FUN_0020be80");
extern int get_morph_o_ray_status(void) __asm__("FUN_0020bf30");
extern int has_hologuise(void) __asm__("FUN_0020bf58");
extern int mission_get_hologuise_or_map_status(void) __asm__("FUN_0020bf68");

/* Each level's mission list, then its map icons, level by level. */
struct MissionItem veldin1_missions[2] __asm__("D_001A08C0") = {
    {0x53b2,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_KERWAN, 0,
     0x50, {0x53b1, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon veldin1_map_icons[2] __asm__("D_001A0910") = {
    {-1, 0, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem novalis_missions[5] __asm__("D_001A0960") = {
    {0x5320,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_KERWAN, MISSION_FLAG_NEEDS_VISIT,
     0, {0x531d, 0, 0, 0},
     0, 0},
    {0x5321,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     3, MISSION_FLAG_NEEDS_VISIT,
     1, {0x531e, 0, 0, 0},
     0, 0},
    {0x5322,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 3,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_ARIDIA, 0,
     2, {0x531f, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_LEVEL_AVAILABLE, LEVEL_KERWAN,
     MISSION_CONDITION_ALWAYS_FALSE, {0},
     0, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon novalis_map_icons[5] __asm__("D_001A0A28") = {
    {2, 3, 0, 0xe99a},
    {-2, 0, 0, 0xe99a, 2},
    {3, 1, 0, 0xe99a, 2},
    {3, 2, 0, 0xe99a, 4, 0x4ea0, 0x25, 0x84, 0x26, 0x4a},
    {0, 0, 4},
};

struct MissionItem aridia_missions[6] __asm__("D_001A0AF0") = {
    {0x5329,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x10, 0,
     3, {0x5323, 0, 0, 0},
     0, 0},
    {0x532a,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x10,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0x1e, 0,
     4, {0x5324, 0, 0, 0},
     0, 0},
    {0x532b,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x11, 0,
     5, {0x5325, 0, 0, 0},
     0, 0},
    {0x532c,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x11,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     5, 0,
     6, {0x5326, 0, 0, 0},
     0, 0},
    {0x532d,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x14,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0x1a, 0,
     7, {0x5327, 0x5328, 0, 0},
     &is_item_available,
     (void *)12},
    {0},
};

struct MapIcon aridia_map_icons[7] __asm__("D_001A0BE0") = {
    {0xf, -1, 0, 0xe99a},
    {0x10, 0, 0, 0xe99a, 2},
    {0x10, 1, 0x40, 0xea7e, 0, 0x4ea1, 0, 0x7d, 0x36, -0x50},
    {0x11, 2, 0, 0xe99a, 2},
    {0x11, 3, 0x40, 0xea65, 0, 0x4ea2, 0, 0x88, 0x36, 0x50},
    {0x12, 4, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem kerwan_missions[9] __asm__("D_001A0CF8") = {
    {0x5336,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x14, 0,
     9, {0x532e, 0, 0, 0},
     0, 0},
    {0x5337,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x14,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     2, 0,
     0xa, {0x532f, 0, 0, 0},
     0, 0},
    {0x5338,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_CALLBACK_NONZERO, {0},
     (s32)&FUN_0020bd30,
     MISSION_FLAG_NEEDS_VISIT, 0xb,
     {0x5330, 0x5331, 0x5332, 0},
     &get_map_point_mission_status,
     0},
    {0x5339,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x18,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_EUDORA, 0,
     0xe, {0x5333, 0, 0, 0},
     0, 0},
    {0x533a,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x18,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x15, 0,
     0xf, {0x5334, 0, 0, 0},
     0, 0},
    {0x533b,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x15,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0xc, 0,
     0x10, {0x5335, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x15, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_EUDORA, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon kerwan_map_icons[13] __asm__("D_001A0E60") = {
    {0x16, -1, 0xb02, 0xe99a, 8},
    {0x1b, -1, 0x702, 0xe99a, 0xa},
    {0x1c, -1, 0x702, 0xe99a, 0xa},
    {0x1d, -1, 0x702, 0xe99a, 0xa},
    {0x1e, -1, 0x702, 0xe99a, 0xa},
    {0x1f, -1, 0x702, 0xe99a, 0xa},
    {0x13, -1, 0, 0xe99a},
    {0x14, 0, 0, 0xe99a, 2},
    {0x14, 1, 0x40, 0xea62, 0, 0x4ea3, 2, 0x84, 0x26, 0x50},
    {0x15, 6, 0, 0xe99a, 2},
    {0x15, 5, 0x40, 0xea6c, 0, 0x4ea4, 0xc, 0x80, 0x26, 0x50},
    {0x17, 7, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem eudora_missions[3] __asm__("D_001A1068") = {
    {0x533e,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_NEBULA_G34, 0,
     0x11, {0x533c, 0, 0, 0},
     0, 0},
    {0x533f,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ITEM_ACQUIRED, {0},
     9, MISSION_FLAG_NEEDS_VISIT,
     0x12, {0x533d, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon eudora_map_icons[4] __asm__("D_001A10E0") = {
    {0x22, -1, 0, 0xe99a},
    {0x23, 1, 0, 0xe99a, 2},
    {-3, 0, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem rilgar_missions[6] __asm__("D_001A1180") = {
    {0x5344,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x26, MISSION_FLAG_HIDE_WHEN_DONE,
     0x13, {0x5340, 0, 0, 0},
     0, 0},
    {0x5344,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x26,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0, 0,
     0x13, {0x5340, 0, 0, 0},
     0, 0},
    {0x5345,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x27, 0,
     0x14, {0x5341, 0, 0, 0},
     0, 0},
    {0x5346,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x27,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_UMBRIS, 0,
     0x15, {0x5342, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x28,
     MISSION_CONDITION_ITEM_ACQUIRED, {0},
     0x17, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon rilgar_map_icons[7] __asm__("D_001A1270") = {
    {0x25, -1, 0, 0xe99a},
    {0x26, 0, 0, 0xe99a, 2},
    {0x26, 1, 0x40, 0xeac8, 0, 0x4ea5, 0, 0x73, 0x26, 0x44},
    {0x27, 2, 0, 0xe99a, 2},
    {0x27, 3, 0x80, 0xe99a, 3, 0x4ea6, 0x26, 0x82, 0x26, -0x46},
    {0x28, 4, 0x40, 0xea77, 0, 0x4ea7, 0x17, 0x78, 0x26, 0x46},
    {0, 0, 4},
};

struct MissionItem nebula_g34_missions[8] __asm__("D_001A1388") = {
    {0x534d,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_CALLBACK_NONZERO, {0},
     (s32)&FUN_0020bd58,
     0, 0x17,
     {0x5348, 0x5348, 0x5348, 0},
     &FUN_0020bed8,
     0},
    {0x534e,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x2f,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0x1d, 0,
     0x1a, {0x5349, 0, 0, 0},
     0, 0},
    {0x534f,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x21,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0x22, 0,
     0x1b, {0x534a, 0, 0, 0},
     0, 0},
    {0x5350,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x22,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0x24, 0,
     0x1c, {0x534b, 0, 0, 0},
     0, 0},
    {0x5351,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x20,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0x16, 0,
     0x1d, {0x534c, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0x22, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x2f, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon nebula_g34_map_icons[7] __asm__("D_001A14C8") = {
    {0x2e, -1, 0, 0xe99a},
    {0x2f, 6, 0, 0xe99a, 2},
    {0x2f, 1, 0x40, 0xea7d, 0, 0x4ea8, 0x1d, 0x8c, 0x28, -0x50},
    {-4, 5, 0, 0xe99a, 2},
    {-4, 3, 0, 0xe99a, 6, 0x4ea9, 0, 0x64, 0x26, 0, 0x1e},
    {0x31, 4, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem umbris_missions[4] __asm__("D_001A15E0") = {
    {0x5355,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0x29, 0,
     0x1e, {0x5352, 0, 0, 0},
     0, 0},
    {0x5356,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x29,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x34, 0,
     0x1f, {0x5353, 0, 0, 0},
     0, 0},
    {0x5357,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x34,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_BATALIA, 0,
     0x20, {0x5354, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon umbris_map_icons[4] __asm__("D_001A1680") = {
    {0x32, -1, 0, 0xe99a},
    {0x33, 0, 0, 0xe99a, 2},
    {0x34, 1, 0, 0xe99a, 6, 0x4eaa, 0, 0x8c, 0x28, -0x50},
    {0, 0, 4},
};

struct MissionItem batalia_missions[8] __asm__("D_001A1720") = {
    {0x535f,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0x2f, 0,
     0x21, {0x5358, 0, 0, 0},
     0, 0},
    {0x5360,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x2f,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x38, 0,
     0x22, {0x5359, 0, 0, 0},
     0, 0},
    {0x5361,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x38,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_GASPAR, 0,
     0x23, {0x535a, 0, 0, 0},
     0, 0},
    {0x5362,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x37, 0,
     0x24, {0x535b, 0, 0, 0},
     0, 0},
    {0x5363,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x37,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_ORXON, 0,
     0x25, {0x535c, 0, 0, 0},
     0, 0},
    {0x5364,
     MISSION_CONDITION_LEVEL_AVAILABLE, LEVEL_ORXON,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x39, 0,
     0x26, {0x535d, 0, 0, 0},
     0, 0},
    {0x5365,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x39,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0x1b, 0,
     0x27, {0x535e, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon batalia_map_icons[9] __asm__("D_001A1860") = {
    {0x36, -1, 0, 0xe99a},
    {0x37, 3, 0, 0xe99a, 2},
    {0x37, 4, 0, 0xe99a, 2},
    {0x38, 0, 0, 0xe99a, 2},
    {0x38, 1, 0, 0xe99a, 2},
    {0x38, 2, 0, 0xe99a, 4, 0x4eab, 0, 0x82, 0x22, 0x28, 0x4b},
    {0x39, 5, 0, 0xe99a, 2},
    {0x39, 6, 0x40, 0xea7b, 0, 0x4eac, 0, 0x74, 0x26, 0, 0x1e},
    {0, 0, 4},
};

struct MissionItem gaspar_missions[2] __asm__("D_001A19C8") = {
    {0x5368,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     7, 0,
     0x28, {0x5366, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon gaspar_map_icons[3] __asm__("D_001A1A18") = {
    {0x3a, -1, 0, 0xe99a},
    {0x3b, 0, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem orxon_missions[10] __asm__("D_001A1A90") = {
    {0x5370,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0x1c, 0,
     0x2a, {0x536a, 0, 0, 0},
     0, 0},
    {0x5371,
     MISSION_CONDITION_ITEM_AVAILABLE, 0x1c,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_POKITARU, 0,
     0x2b, {0x536b, 0, 0, 0},
     0, 0},
    {0x5373,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_CALLBACK_NONZERO, {0},
     (s32)&FUN_0020bd90,
     MISSION_FLAG_NEEDS_VISIT, 0x2c,
     {0x536c, 0, 0, 0},
     0, 0},
    {0x5372,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x3f,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_HOVEN, 0,
     0x2d, {0x536d, 0, 0, 0},
     0, 0},
    {0x5374,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x40,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     4, 0,
     0x2e, {0x536e, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 4,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     5, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_POKITARU, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ITEM_AVAILABLE, 6,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_HOVEN, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ITEM_AVAILABLE, 6,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x40, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon orxon_map_icons[8] __asm__("D_001A1C20") = {
    {0x3c, -1, 0, 0xe99a},
    {0x3d, 0, 0, 0xe99a, 2},
    {0x3e, 6, 0, 0xe99a, 2},
    {0x3f, 7, 0, 0xe99a, 2},
    {0x40, 8, 0, 0xe99a, 2},
    {0x40, 4, 0x40, 0xeac9, 3, 0x4ead, 0x28, 0x70, 0x37, 0x50},
    {0x40, 5, 0x40, 0xeac9, 4, 0x4eae, 0x29, 0x70, 0x37, 0x50},
    {0, 0, 4},
};

struct MissionItem pokitaru_missions[9] __asm__("D_001A1D60") = {
    {0x537c,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_CALLBACK_NONZERO, {0},
     (s32)&FUN_0020bdc0,
     0, 0x30,
     {0x5376, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x42, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0x537d,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x42,
     MISSION_CONDITION_UPGRADE_LEVEL_AT_LEAST_TWO, {0},
     0x42, 0,
     0x31, {0x5377, 0, 0, 0},
     0, 0},
    {0x537e,
     MISSION_CONDITION_UPGRADE_LEVEL_AT_LEAST_TWO, 0x42,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     3, 0,
     0x32, {0x5378, 0, 0, 0},
     0, 0},
    {0x537f,
     MISSION_CONDITION_ITEM_AVAILABLE, 3,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     6, 0,
     0x33, {0x5379, 0, 0, 0},
     0, 0},
    {0x5380,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x55,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x44, 0,
     0x34, {0x537a, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x44, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0x5381,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x44,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0x23, 0,
     0x35, {0x537b, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon pokitaru_map_icons[10] __asm__("D_001A1EC8") = {
    {0x41, -1, 0, 0xe99a},
    {0x42, 1, 0, 0xe99a, 2},
    {0x42, 2, 0x40, 0xea66, 0, 0x4eaf, 0, 0x84, 0x26, 0x50},
    {0x42, 4, 0x40, 0xea66, 0, 0x4eb0, 0, 0x84, 0x26, 0x50},
    {0x43, 1, 0, 0xe99a, 2},
    {0x43, 2, 0, 0xe99a, 2},
    {0x43, 3, 0x40, 0xea63, 0, 0x4eb1, 0, 0xa0, 0x26, 0x5a},
    {0x44, 6, 0, 0xe99a, 2},
    {0x44, 7, 0x40, 0xeac9, 0, 0x4eb2, 0, 0x8c, 0x26, 0x50},
    {0, 0, 4},
};

struct MissionItem hoven_missions[6] __asm__("D_001A2058") = {
    {0x5385,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_OLTANIS_ORBIT, 0,
     0x36, {0x5382, 0, 0, 0},
     0, 0},
    {0x5386,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_CALLBACK_NONZERO, {0},
     (s32)&FUN_0020bde8,
     MISSION_FLAG_NEEDS_VISIT, 0x37,
     {0x5383, 0x5383, 0, 0},
     &FUN_0020bf08,
     0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x46, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0x5387,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x46,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     4, 0,
     0x39, {0x5384, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0x61, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon hoven_map_icons[6] __asm__("D_001A2148") = {
    {0x45, -1, 0, 0xe99a},
    {0x46, 2, 0, 0xe99a, 2},
    {0x46, 3, 0x40, 0xea62, 0, 0x4eb3, 4, 0x96, 0x26, 0x55},
    {0x49, 0, 0, 0xe99a, 2},
    {0x48, 4, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem oltanis_orbit_missions[3] __asm__("D_001A2238") = {
    {0x538a,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x4b, 0,
     0x3a, {0x5388, 0, 0, 0},
     0, 0},
    {0x538b,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x4b,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_OLTANIS, 0,
     0x3b, {0x5389, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon oltanis_orbit_map_icons[4] __asm__("D_001A22B0") = {
    {0x4a, -1, 0, 0xe99a},
    {0x4b, 0, 0, 0xe99a, 2},
    {-5, 1, 0, 0xe99a, 6, 0x538b, 0, 0x96, 0x26, 0x55},
    {0, 0, 4},
};

struct MissionItem oltanis_missions[8] __asm__("D_001A2350") = {
    {0x5390,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x58, 0,
     0x3c, {0x538c, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x58, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0x5391,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x58,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_QUARTU, 0,
     0x3d, {0x538d, 0, 0, 0},
     0, 0},
    {0x5392,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_CALLBACK_NONZERO, {0},
     (s32)&FUN_0020be10,
     0, 0x3e,
     {0x538e, 0x538e, 0x538e, 0},
     &get_morph_o_ray_status,
     0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x57, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0x5393,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x57,
     MISSION_CONDITION_ITEM_ACQUIRED, {0},
     0x20, 0,
     0x41, {0x538f, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ITEM_ACQUIRED, {0},
     0x15, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon oltanis_map_icons[7] __asm__("D_001A2490") = {
    {0x56, -1, 0, 0xe99a},
    {0x58, 1, 0, 0xe99a, 2},
    {0x58, 2, 0, 0xe99a, 4, 0x4eb4, 0, 0x96, 0x26, 0x55},
    {0x57, 4, 0, 0xe99a, 2},
    {0x57, 5, 0, 0xe99a, 4, 0x4eb5, 0, 0x96, 0x26, 0x55},
    {0x59, 6, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem quartu_missions[7] __asm__("D_001A25A8") = {
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_VELDIN_ORBIT, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0x539b,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x5e, 0,
     0x42, {0x5394, 0, 0, 0},
     0, 0},
    {0x539c,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x5e,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_KALEBO3, 0,
     0x43, {0x5395, 0, 0, 0},
     0, 0},
    {0x539e,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ITEM_ACQUIRED, {0},
     0x22, MISSION_FLAG_NEEDS_VISIT,
     0x44, {0x5397, 0, 0, 0},
     0, 0},
    {0x539f,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x6e,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_VELDIN_ORBIT, 0,
     0x45, {0x5398, 0x5398, 0, 0},
     &has_hologuise,
     0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ITEM_ACQUIRED, {0},
     0x22, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon quartu_map_icons[6] __asm__("D_001A26C0") = {
    {0x5d, -1, 0, 0xe99a},
    {0x5f, 0, 0, 0xe99a, 2},
    {0x5e, 1, 0, 0xe99a, 2},
    {0x5e, 2, 0, 0xe99a, 2, 0x4eb6, 0, 0x96, 0x26, 0x55},
    {0x60, 5, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem kalebo3_missions[7] __asm__("D_001A27B0") = {
    {0x53a7,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_CALLBACK_NONZERO, {0},
     (s32)&FUN_0020be38,
     0, 0x47,
     {0x53a2, 0x53a2, 0x53a3, 0},
     &mission_get_hologuise_or_map_status,
     0},
    {0x53a8,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x70,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x66, 0,
     0x4a, {0x53a4, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, {0},
     0x66, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0x53a9,
     MISSION_CONDITION_UPGRADE_LEVEL_NONZERO, 0x66,
     MISSION_CONDITION_ITEM_AVAILABLE, {0},
     0x1f, 0,
     0x4b, {0x53a5, 0, 0, 0},
     0, 0},
    {0x53aa,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, 0x71,
     MISSION_CONDITION_ITEM_ACQUIRED, {0},
     0x21, 0,
     0x4c, {0x53a6, 0, 0, 0},
     0, 0},
    {-1,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ITEM_ACQUIRED, {0},
     0x21, MISSION_FLAG_UNCOUNTED,
     0, {0, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon kalebo3_map_icons[5] __asm__("D_001A28C8") = {
    {0x64, -1, 0, 0xe99a},
    {0x66, 2, 0, 0xe99a, 2},
    {0x66, 3, 0, 0xea7f, 0, 0x4eb8, 0, 0x96, 0x26, 0x55},
    {0x65, 5, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem veldin_orbit_missions[3] __asm__("D_001A2990") = {
    {0x53ad,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_LEVEL_AVAILABLE, {0},
     LEVEL_VELDIN2, 0,
     0x4d, {0x53ab, 0, 0, 0},
     0, 0},
    {0x53ae,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0x78, MISSION_FLAG_NEEDS_VISIT,
     0x4e, {0x53ac, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon veldin_orbit_map_icons[4] __asm__("D_001A2A08") = {
    {0x6b, -1, 0, 0xe99a},
    {-7, 0, 0, 0xe99a, 2},
    {-8, 1, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem veldin2_missions[2] __asm__("D_001A2AA8") = {
    {0x53b0,
     MISSION_CONDITION_ALWAYS_TRUE, 0,
     MISSION_CONDITION_ALTERNATE_ITEM_AVAILABLE, {0},
     0x7d, 0,
     0x4f, {0x53af, 0, 0, 0},
     0, 0},
    {0},
};

struct MapIcon veldin2_map_icons[3] __asm__("D_001A2AF8") = {
    {0x72, -1, 0, 0xe99a},
    {-9, 0, 0, 0xe99a, 2},
    {0, 0, 4},
};

struct MissionItem *mission_lists_by_level[20] __asm__("D_001A2B70") = {
    veldin1_missions, novalis_missions,       aridia_missions,     kerwan_missions,
    eudora_missions,  rilgar_missions,        nebula_g34_missions, umbris_missions,
    batalia_missions, gaspar_missions,        orxon_missions,      pokitaru_missions,
    hoven_missions,   oltanis_orbit_missions, oltanis_missions,    quartu_missions,
    kalebo3_missions, veldin_orbit_missions,  veldin2_missions,
};

struct MapIcon *map_icon_lists_by_level[19] __asm__("D_001A2BC0") = {
    veldin1_map_icons, novalis_map_icons,       aridia_map_icons,     kerwan_map_icons,
    eudora_map_icons,  rilgar_map_icons,        nebula_g34_map_icons, umbris_map_icons,
    batalia_map_icons, gaspar_map_icons,        orxon_map_icons,      pokitaru_map_icons,
    hoven_map_icons,   oltanis_orbit_map_icons, oltanis_map_icons,    quartu_map_icons,
    kalebo3_map_icons, veldin_orbit_map_icons,  veldin2_map_icons,
};
