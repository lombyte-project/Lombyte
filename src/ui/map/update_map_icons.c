#include "types.h"
#include "rnc/ui/map/map_state.h"

/* Map screen icon refresh: places the fixed icons (-1..-9), projects the
   others through world_to_map_coords, then sizes each label box. */

typedef struct {
    short s[12];
} TextBox;

typedef struct MapIcon {
    s16 id;
    s16 link;
    u16 flags;
    u8 pad6[4];
    s16 label_enabled;
    u8 padC[2];
    u16 label_width;
    u16 label_height;
    u8 pad12[6];
    f32 x;
    f32 y;
    f32 z;
    s32 active;
} MapIcon;

typedef struct {
    u8 pad0[0x24];
    s16 active;
    u8 pad26[2];
} MapIconLink;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    s32 flag;
} MapPoint;

typedef struct {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    u8 pad18[0x30];
    f32 z;
} MapWorldObject;

struct MapCursor {
    u8 pad0[0x80];
    f32 x;
    f32 y;
    f32 active;
};

extern u8 D_0013DD58[];
extern MapIcon *D_001A2BC0[];
extern s32 D_001A01F4[];
extern struct MapCursor D_0013F350;
extern s32 D_0015ED84;
extern s32 D_0015FD60 __attribute__((sda));
extern MapWorldObject *D_00199478[];
extern MapPoint D_0013D5B0[];
extern s32 D_0013D5BC[];
extern struct {
    u8 pad0[0x10];
    MapIconLink *links;
} D_001A2C10;

extern void world_to_map_coords(f32 *outx, f32 *outy, s32 view, f32 x,
                                f32 y) __asm__("func_00208408");
extern void format_menu_item_text(s32 idx, char *dst) __asm__("func_00208280");
extern void font_print_window_small(TextBox *, long, char *, int) __asm__("func_001F75F0");

void update_map_icons(s32 level, s32 flag) __asm__("FUN_0020bf90");

void update_map_icons(s32 level, s32 flag) {
    char label_text[128];
    f32 map_x;
    f32 map_y;
    MapIcon *icon;
    s32 icon_index;

    if (level < 19 && D_0013DD58[level] != 0) {
        D_001A00F0.icons = D_001A2BC0[level];
    } else {
        D_001A00F0.icons = 0;
    }

    if (D_0013F350.active != 0.0f && level == D_0015ED84 && flag) {
        world_to_map_coords(&map_x, &map_y, D_0015FD60 ? level + 100 : level, D_0013F350.x,
                            D_0013F350.y);
        D_001A00F0.pan_x[level] = (s32)(map_x * 4096.0f) << 16;
        D_001A00F0.pan_y[level] = (s32)(map_y * 4096.0f) << 16;
    } else {
        /* D_001A01F4 is &D_001A00F0.posx: a null-guarded reset that can
           never run, but retail still emits it with p folded to 0. */
        s32 *p = D_001A01F4;
        if (p == 0) {
            p[level] = 0x8000000;
            p[level + 20] = 0x8000000;
        }
    }

    if (D_001A00F0.icons == 0) {
        return;
    }

    if (!(D_001A00F0.icons->flags & 4)) {
        icon_index = 0;
        do {
            icon = &D_001A00F0.icons[icon_index];
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
                if (level == D_0015ED84) {
                    if (D_00199478[icon->id] != 0) {
                        world_to_map_coords(&icon->x, &icon->y, level, D_00199478[icon->id]->x,
                                            D_00199478[icon->id]->y);
                        D_001A00F0.icons[icon_index].z = D_00199478[icon->id]->z;
                    }
                } else {
                    world_to_map_coords(&icon->x, &icon->y, level, D_0013D5B0[icon->id].x,
                                        D_0013D5B0[icon->id].y);
                    D_001A00F0.icons[icon_index].z = D_0013D5B0[icon->id].z;
                }
            }
            icon_index++;
        } while (!(D_001A00F0.icons[icon_index].flags & 4));
    }

    for (icon_index = 0; !(D_001A00F0.icons[icon_index].flags & 4); icon_index++) {
        icon = &D_001A00F0.icons[icon_index];
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
        if (D_001A00F0.icons[icon_index].label_enabled != 0) {
            s32 label_height_changed = 0;
            s16 previous_label_height;

            D_001A00F0.icons[icon_index].flags |= 0x10;
            format_menu_item_text(icon_index, label_text);
            {
                TextBox text_window = {
                    {0, icon->label_height, 0, icon->label_width, 4, 4, 0, 0, 0xF, 4}};
                font_print_window_small(&text_window, 0x80FFA888L, label_text, -1);
                previous_label_height = text_window.s[7];
                icon->label_height = text_window.s[7] + 8;
                do {
                    text_window.s[3] -= 4;
                    font_print_window_small(&text_window, 0x80FFA888L, label_text, -1);
                    if (text_window.s[7] != previous_label_height) {
                        label_height_changed = 1;
                    }
                } while (!label_height_changed);
                icon->label_width = text_window.s[3] + 4;
            }
        }
    }
}

extern __typeof__(update_map_icons) func_0020BF90 __attribute__((alias("FUN_0020bf90")));
