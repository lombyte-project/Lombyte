#include "types.h"

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
    s16 unkA;
    u8 padC[2];
    u16 unkE;
    u16 unk10;
    u8 pad12[6];
    f32 x;
    f32 y;
    f32 z;
    s32 unk24;
} MapIcon;

typedef struct {
    u8 pad0[0x24];
    s16 unk24;
    u8 pad26[2];
} MapLink;

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
} MapObj;

struct MapState {
    u8 pad0[0x20];
    MapIcon *icons;
    u8 pad24[0xE0];
    s32 posx[20];
    s32 posy[20];
};

struct MapCursor {
    u8 pad0[0x80];
    f32 x;
    f32 y;
    f32 active;
};

extern u8 D_0013DD58[];
extern MapIcon *D_001A2BC0[];
extern struct MapState D_001A00F0;
extern s32 D_001A01F4[];
extern struct MapCursor D_0013F350;
extern s32 D_0015ED84;
extern s32 D_0015FD60 __attribute__((sda));
extern MapObj *D_00199478[];
extern MapPoint D_0013D5B0[];
extern s32 D_0013D5BC[];
extern struct { u8 pad0[0x10]; MapLink *links; } D_001A2C10;

extern void world_to_map_coords(f32 *outx, f32 *outy, s32 view, f32 x, f32 y) __asm__("func_00208408");
extern void format_menu_item_text(s32 idx, char *dst) __asm__("func_00208280");
extern void func_001F75F0(TextBox *, long, char *, int);

void FUN_0020bf90(s32 id, s32 flag) {
    char buf[128];
    f32 mx;
    f32 my;
    MapIcon *e;
    s32 i;

    if (id < 19 && D_0013DD58[id] != 0) {
        D_001A00F0.icons = D_001A2BC0[id];
    } else {
        D_001A00F0.icons = 0;
    }

    if (D_0013F350.active != 0.0f && id == D_0015ED84 && flag) {
        world_to_map_coords(&mx, &my, D_0015FD60 ? id + 100 : id, D_0013F350.x, D_0013F350.y);
        D_001A00F0.posx[id] = (s32)(mx * 4096.0f) << 16;
        D_001A00F0.posy[id] = (s32)(my * 4096.0f) << 16;
    } else {
        /* D_001A01F4 is &D_001A00F0.posx: a null-guarded reset that can
           never run, but retail still emits it with p folded to 0. */
        s32 *p = D_001A01F4;
        if (p == 0) {
            p[id] = 0x8000000;
            p[id + 20] = 0x8000000;
        }
    }

    if (D_001A00F0.icons == 0) {
        return;
    }

    if (!(D_001A00F0.icons->flags & 4)) {
        i = 0;
        do {
            e = &D_001A00F0.icons[i];
            if (e->id == -1) {
                e->x = 0.234375f;
                e->y = 0.30078125f;
            } else if (e->id == -2) {
                e->x = 0.5390625f;
                e->y = 0.365234375f;
            } else if (e->id == -3) {
                e->x = 0.58203125f;
                e->y = 0.6875f;
            } else if (e->id == -4) {
                e->x = 0.720703125f;
                e->y = 0.728515625f;
            } else if (e->id == -5) {
                e->x = 0.384765625f;
                e->y = 0.396484375f;
            } else if (e->id == -7) {
                e->x = 0.8671875f;
                e->y = 0.23046875f;
            } else if (e->id == -8) {
                e->x = 0.48046875f;
                e->y = 0.5703125f;
            } else if (e->id == -9) {
                e->x = 0.625f;
                e->y = 0.72265625f;
            } else {
                if (id == D_0015ED84) {
                    if (D_00199478[e->id] != 0) {
                        world_to_map_coords(&e->x, &e->y, id, D_00199478[e->id]->x, D_00199478[e->id]->y);
                        D_001A00F0.icons[i].z = D_00199478[e->id]->z;
                    }
                } else {
                    world_to_map_coords(&e->x, &e->y, id, D_0013D5B0[e->id].x, D_0013D5B0[e->id].y);
                    D_001A00F0.icons[i].z = D_0013D5B0[e->id].z;
                }
            }
            i++;
        } while (!(D_001A00F0.icons[i].flags & 4));
    }

    for (i = 0; !(D_001A00F0.icons[i].flags & 4); i++) {
        e = &D_001A00F0.icons[i];
        e->flags &= ~0x10;
        e->unk24 = 0;
        if (e->link == -1) {
            e->unk24 = 1;
        } else {
            e->unk24 = D_001A2C10.links[e->link].unk24 == 1;
        }
        if ((e->flags & 0x1000) && (D_0013D5BC[e->id * 4] ^ 1) & 1) {
            e->unk24 = 0;
        }
        if (D_001A00F0.icons[i].unkA != 0) {
            s32 done = 0;
            s16 h;

            D_001A00F0.icons[i].flags |= 0x10;
            format_menu_item_text(i, buf);
            {
                TextBox c = { { 0, e->unk10, 0, e->unkE, 4, 4, 0, 0, 0xF, 4 } };
                func_001F75F0(&c, 0x80FFA888L, buf, -1);
                h = c.s[7];
                e->unk10 = c.s[7] + 8;
                do {
                    c.s[3] -= 4;
                    func_001F75F0(&c, 0x80FFA888L, buf, -1);
                    if (c.s[7] != h) {
                        done = 1;
                    }
                } while (!done);
                e->unkE = c.s[3] + 4;
            }
        }
    }
}

extern __typeof__(FUN_0020bf90) func_0020BF90 __attribute__((alias("FUN_0020bf90")));
