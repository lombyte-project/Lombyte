#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/map/draw_map_overlay/FUN_00205640.s", FUN_00205640);
#else
#include "types.h"
#include "sda.h"

struct DmaTag {
    u32 w0;
    u32 addr;
    u32 w2;
    u32 w3;
};

struct TagPtr {
    struct DmaTag *p;
};

struct ScreenOfs {
    u8 pad0[0x10];
    s32 x;
    s32 y;
};

typedef struct {
    s16 id;      /* 0x00 */
    u16 pad02;
    u16 flags;   /* 0x04 */
    u16 tex;     /* 0x06 */
    s16 frame;   /* 0x08 */
    u16 pad0A[2];
    u16 w;       /* 0x0E */
    s16 h;       /* 0x10 */
    s16 xoff;    /* 0x12 */
    s16 yoff;    /* 0x14 */
    u16 pad16;
    f32 x;       /* 0x18 */
    f32 y;       /* 0x1C */
    f32 angle;   /* 0x20 */
    s32 active;  /* 0x24 */
} MapIcon;

typedef struct {
    s32 cell;
    s32 pad[3];
} MapMark;

typedef struct {
    u8 pad0[0x8];
    s32 marks_on;      /* 0x08 */
    u8 padC[0xC];
    s32 z;             /* 0x18 */
    s32 grid;          /* 0x1C */
    MapIcon *icons;    /* 0x20 */
    s32 enabled;       /* 0x24 */
    u8 pad28[0x4];
    s32 show_marks;    /* 0x2C */
    MapMark marks[8];  /* 0x30 */
    u8 padB0[0x4];
    f32 zoom[20];      /* 0xB4 */
    s32 ofs_x[20];     /* 0x104 */
    s32 ofs_y[20];     /* 0x154 */
    u8 pad1A4[0x84];
    s32 cur;           /* 0x228 */
    u8 pad22C[0x14];
    s32 tbp;           /* 0x240 */
} MapState;

typedef struct {
    u8 pad0[0x80];
    f32 x;        /* 0x80 */
    f32 y;        /* 0x84 */
    u8 pad88[0x10];
    f32 angle;    /* 0x98 */
    u8 pad9C[0x1FF0];
    s32 mode;     /* 0x208C */
} Player;

typedef struct {
    s16 pad0;
    s16 img;
} TexRef;

typedef struct {
    u8 pad0[6];
    u8 lw;
    u8 lh;
} TexInfo;

typedef struct {
    u8 pad0[0x20];
    TexRef *refs;     /* 0x20 */
    TexInfo *infos;   /* 0x24 */
} TexBank;

typedef struct {
    s16 s[12];
} TextBox;

typedef struct {
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} Rect;

#define SPR ((Rect *)0x70000000)

extern struct TagPtr D_00160F00;
extern struct ScreenOfs D_0013E500;
extern MapState D_001A00F0;
extern Player D_0013F350;
extern TexBank D_0019A3E8;
extern u8 D_0013D5BC[];
extern u16 D_001518D2[];
extern s32 D_0015ED84;
extern u8 D_0015EDB4;
extern s32 D_0015FD60 __attribute__((sda));
extern f32 D_0015FD80 __attribute__((sda));
extern f32 D_0015FD84 __attribute__((sda));
extern f32 D_0015FD88 __attribute__((sda));
extern f32 D_0015FD8C __attribute__((sda));
extern f32 D_0015FD90 __attribute__((sda));
extern f32 D_0015FD94 __attribute__((sda));
extern u8 D_001E8068[];

extern void func_001F0C50(s32, s32, s32, u8 *);
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern void func_001F5F18(s32, s32, s32, s32, s32);
extern void func_001F75F0(void *, u64, void *, s32);
extern f32 func_001FA580(f32, f32);
extern f32 func_001FA5C8(f32, f32);
extern s32 func_001FF960(s32, s32);
extern u64 func_001FFA10(s32);
extern void func_00200080(s32, s32, s32, s32, s32, s32);
extern void func_00200600(f32, f32, f32, f32, f32, s32, s32, u64);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);
extern void func_00208280(s32, void *);
extern void func_00208408(f32 *, f32 *, s32, f32, f32);
extern void func_00208508(s32, s32, s32, s32);
extern void vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");
extern void *memset(void *, s32, u32);

void draw_map_overlay(void) __asm__("FUN_00205640");

extern MapState D_001A00F0_far __asm__("D_001A00F0") __attribute__((section(".data")));
void draw_map_overlay(void) {
    union {
        u8 b[0x80];
        f32 f[2];
    } buf;
    TextBox tb;
    s32 rx0, ry0, rx1, ry1;
    struct DmaTag *tag;
    u64 *q;
    u64 tex;
    u64 tex2;
    s32 cbp;
    s32 sign;
    s32 tile;
    s32 nx0, ny0, nx1, ny1;
    s32 x0, y0, x1, y1;
    s32 u, v;
    s32 dx, dy;
    s32 i, j;
    s32 cell, col, row;
    MapMark *mk;
    Rect *r;
    Rect *spr;
    MapIcon *ic;
    TexInfo *ti;
    f32 zoom;
    f32 scale;
    f32 k;
    f32 s;
    f32 fx, fy;
    s32 ox0, ox1, oy0, oy1;
    s32 ax, ay, bx, by;
    s32 id, frame, size;
    f32 ang, sx, sy, cx, cy;
    s16 w;
    s32 h, xo, yo;
    s32 lx, ly;
    s32 img;
    s32 flip;
    s32 tid;
    s32 lim;

    if (D_001A00F0.enabled == 0) {
        u8 *font = D_001E8068;

        func_001F4280(0);
        func_001F0C50(0x100, (s16)D_001518D2[0] >> 1, 0x80909090, font);
        func_001F4398();
        return;
    }
    if (D_001A00F0.cur < 0) {
        return;
    }

    rx0 = 0x1000;
    func_001F4280(0);
    ry0 = 0x800;
    sign = D_0015EDB4 ? -1 : 1;
    zoom = D_001A00F0.zoom[D_001A00F0.cur];
    oy0 = zoom * (f32)(D_001A00F0.ofs_y[D_001A00F0.cur] >> 15);
    ox1 = zoom * 8192.0f;
    ox0 = zoom * (f32)(D_001A00F0.ofs_x[D_001A00F0.cur] >> 15);
    tile = zoom * 512.0f;
    lim = tile + 0x2000;
    rx0 -= ox0 * sign;
    ry0 -= oy0;
    rx1 = rx0 + ox1 * sign;
    ry1 = ry0 + ox1;
    nx0 = (rx0 + tile - 1) / tile;
    ny0 = (ry0 + tile - 1) / tile;
    nx1 = (lim - rx1 - 1) / tile;
    ny1 = (lim - ry1 - 1) / tile;
    x0 = rx0 - tile * nx0;
    y0 = ry0 - tile * ny0;
    x1 = rx1 + tile * nx1;
    y1 = ry1 + tile * ny1;
    u = ((nx0 + nx1) << 9) + 0x2000;
    v = ((ny0 + ny1) << 9) + 0x2000;

    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x47, 0);
    D_00160F00.p->w0 = 0x10000005;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000005;
    D_00160F00.p++;
    tex = func_001FFA10(func_001FF960(0xE999, D_001A00F0.cur));
    q = (u64 *)D_00160F00.p;
    q[0] = 0x7400000000008001;
    q[1] = 0x5353106;
    q[2] = tex;
    q[3] = 0x156;
    q[4] = 0x80808080;
    q[5] = 0;
    q[6] = ((x0 + D_0013E500.x) - 8) | ((u64)((y0 + D_0013E500.y) - 8) << 16) |
           ((u64)D_001A00F0.z << 32);
    q[7] = u | ((u64)v << 16);
    q[8] = ((x1 + D_0013E500.x) - 8) | ((u64)((y1 + D_0013E500.y) - 8) << 16) |
           ((u64)D_001A00F0.z << 32);
    q[9] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x50);
    vu1_add_g_sregister(8, 5);
    vu1_add_g_sregister(0x47, 0x60B);

    cbp = (tex >> 37) & 0x3FFF;
    D_00160F00.p->w0 = 0x10000005;
    D_00160F00.p->addr = 0;
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0x50000005;
    tex2 = (u64)((D_001A00F0.tbp >> 8) | (8 << 14) | (0x13 << 20) | (9 << 26)) | ((u64)9 << 30) | ((u64)1 << 34) | ((u64)cbp << 37) | ((long)4 << 61);
    tag = D_00160F00.p;
    D_00160F00.p = tag + 1;
    q = (u64 *)(tag + 1);
    q[0] = 0x7400000000008001;
    q[1] = 0x5353106;
    q[2] = tex2;
    q[3] = 0x156;
    q[4] = 0x80808080;
    q[5] = 0;
    q[6] = ((rx0 + D_0013E500.x) - 8) | ((u64)((ry0 + D_0013E500.y) - 8) << 16) |
           ((u64)D_001A00F0.z << 32);
    q[7] = 0x20002000;
    q[8] = ((rx1 + D_0013E500.x) - 8) | ((u64)((ry1 + D_0013E500.y) - 8) << 16) |
           ((u64)D_001A00F0.z << 32);
    q[9] = 0;
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x50);
    vu1_add_g_sregister(0x47, 0x360B);

    if (D_001A00F0.show_marks != 0 && D_001A00F0.marks_on != 0) {
        dx = rx1 - rx0;
        dy = ry1 - ry0;
        for (i = 0; i < 8; i++) {
            cell = D_001A00F0.marks[i].cell;
            if (cell >= 0) {
                row = cell % 16;
                col = cell / 16;
                func_00200E08(rx0 + row * dx / 16, ry0 + col * dy / 16,
                              rx0 + (row + 1) * dx / 16, ry0 + (col + 1) * dy / 16,
                              0x20000000, 1);
            }
        }
    }

    if (D_001A00F0.icons != 0) {
        spr = SPR;
        scale = (D_001A00F0.zoom[D_001A00F0.cur] * 2.0f + 5.0f) / 13.0f;
        if (!(D_001A00F0.icons[0].flags & 4)) {
            Rect *r;
            s32 i;
            MapIcon *ic;
            f32 k;
            s32 img;
            f32 fx, fy;
            TexInfo *ti;
            f32 s;
            s32 tid;
            

            i = 0;
            do {
                if (D_001A00F0.icons[i].active != 0 && (tid = D_001A00F0.icons[i].tex) != 0 && !(D_001A00F0.icons[i].flags & 1)) {
                    k = 1.0f;
                    if (D_001A00F0.icons[i].flags & 0x80) {
                        k = 1.5f;
                    }
                    img = func_001FF960(tid, D_001A00F0.icons[i].frame);
                    fx = (f32)rx0 + D_001A00F0.icons[i].x * (f32)(rx1 - rx0);
                    fy = (f32)ry0 + D_001A00F0.icons[i].y * (f32)(ry1 - ry0);
                    ti = &D_0019A3E8.infos[D_0019A3E8.refs[img].img];
                    if (D_001A00F0.icons[i].flags & 0x200) {
                        s = D_001A00F0.zoom[D_001A00F0.cur];
                    } else {
                        s = scale;
                    }
                    spr[i].x0 = fx - k * s * (f32)(1 << (ti->lw + 3));
                    spr[i].x1 = (f32)spr[i].x0 + k * s * (f32)(1 << (ti->lw + 4));
                    spr[i].y0 = fy - k * s * (f32)(1 << (ti->lh + 3));
                    spr[i].y1 = (f32)spr[i].y0 + k * s * (f32)(1 << (ti->lh + 4));
                }
                i++;
            } while (!(D_001A00F0.icons[i].flags & 4));
        }

        {
            s32 i, j;
            s32 ox0, ox1, oy0, oy1;
            s32 ax, ay, bx, by;
            MapIcon *ic;

            for (i = 0; !(D_001A00F0.icons[i + 1].flags & 4); i++) {
                if (D_001A00F0.icons[i].active == 0 || D_001A00F0.icons[i].tex == 0 || (D_001A00F0.icons[i].flags & 3)) {
                    continue;
                }
                for (j = i + 1; !(D_001A00F0.icons[j].flags & 4); j++) {
                    ox0 = spr[j].x1 - spr[i].x0;
                    if (ox0 <= 0) continue;
                    ox1 = spr[i].x1 - spr[j].x0;
                    if (ox1 <= 0) continue;
                    oy0 = spr[j].y1 - spr[i].y0;
                    if (oy0 <= 0) continue;
                    oy1 = spr[i].y1 - spr[j].y0;
                    if (oy1 <= 0) continue;
                    if (D_001A00F0.icons[j].active == 0 || D_001A00F0.icons[j].tex == 0 || (D_001A00F0.icons[j].flags & 3)) {
                        continue;
                    }
                    ax = 0;
                    ay = 0;
                    bx = 0;
                    by = 0;
                    if (ox0 <= ox1 && ox0 <= oy0 && ox0 <= oy1) {
                        ax = ox0 >> 1;
                        bx = ax - ox0;
                    } else if (ox1 <= oy0 && ox1 <= oy1) {
                        bx = ox1 >> 1;
                        ax = bx - ox1;
                    } else if (oy0 <= oy1) {
                        ay = oy0 >> 1;
                        by = ay - oy0;
                    } else {
                        by = oy1 >> 1;
                        ay = by - oy1;
                    }
                    spr[i].x0 += ax;
                    spr[i].x1 += ax;
                    spr[i].y0 += ay;
                    spr[i].y1 += ay;
                    spr[j].x0 += bx;
                    spr[j].x1 += bx;
                    spr[j].y0 += by;
                    spr[j].y1 += by;
                }
            }
        }

        {
            s32 i;

        i = 0;
        if (!(D_001A00F0.icons[0].flags & 4)) {
            do {
                if (D_001A00F0.icons[i].active != 0 && !(D_001A00F0.icons[i].flags & 1) && (id = D_001A00F0.icons[i].tex) != 0) {
                    frame = D_001A00F0.icons[i].frame;
                    if (D_001A00F0.icons[i].flags & 0x40) {
                        func_00200E08(spr[i].x0 - 0x20, spr[i].y0 - 0x20, spr[i].x1 + 0x20, spr[i].y1 + 0x20,
                                      0x80000000, 1);
                    }
                    if (D_001A00F0.icons[i].flags & 0x200) {
                        s = D_001A00F0.zoom[D_001A00F0.cur];
                    } else {
                        s = scale;
                    }
                        if (D_001A00F0.icons[i].flags & 0x100) {
                        ang = D_001A00F0.icons[i].angle;
                        size = 0x20;
                        sy = s * 256.0f;
                        sx = sy;
                        if (D_001A00F0.icons[i].flags & 0x400) {
                            ang = func_001FA580(ang, 1.5707964f);
                            size = 0x40;
                            sx = s * D_0015FD88;
                            sy = s * D_0015FD8C;
                            if (*(s32 *)(D_0013D5BC + D_001A00F0.icons[i].id * 16) & 2) {
                                frame++;
                            }
                        }
                        if (D_001A00F0.icons[i].flags & 0x800) {
                            ang = func_001FA580(ang, 1.5707964f);
                            size = 0x40;
                            sx = s * D_0015FD80;
                            sy = s * D_0015FD84;
                            if (*(s32 *)(D_0013D5BC + D_001A00F0.icons[i].id * 16) & 2) {
                                frame++;
                            }
                        }
                        if (D_001A00F0.icons[i].flags & 0x1000) {
                            ang = func_001FA580(ang, 1.5707964f);
                            sx = s * D_0015FD90;
                            sy = s * D_0015FD94;
                        }
                        cx = (f32)(spr[i].x0 + spr[i].x1) * 0.5f;
                        cy = (f32)(spr[i].y0 + spr[i].y1) * 0.5f;
                        func_00200600(cx, cy, sx, sy, ang, size, 0x20, func_001FFA10(func_001FF960(id, frame)));
                    } else {
                        func_00200080(func_001FF960(id, frame), spr[i].x0, spr[i].y0, spr[i].x1 - spr[i].x0,
                                      spr[i].y1 - spr[i].y0, 0x80);
                    }
                        if (D_001A00F0.icons[i].flags & 0x10) {
                        w = D_001A00F0.icons[i].w;
                        xo = D_001A00F0.icons[i].xoff;
                        h = D_001A00F0.icons[i].h;
                        yo = D_001A00F0.icons[i].yoff;
                        if (xo == 0) {
                            lx = ((spr[i].x0 + spr[i].x1) >> 5) - w / 2;
                        } else {
                            lx = ((xo > 0 ? spr[i].x1 : spr[i].x0) >> 4) - w / 2 + xo;
                        }
                        if (yo == 0) {
                            ly = ((spr[i].y0 + spr[i].y1) >> 5) - h / 2;
                        } else {
                            ly = ((yo > 0 ? spr[i].y1 : spr[i].y0) >> 4) - h / 2 + yo;
                        }
                        func_001F5F18(ly, ly + h, lx, lx + w, 0x40);
                        func_00208280(i, &buf);
                        memset(&tb, 0, sizeof(tb));
                        tb.s[8] = 0xF;
                        tb.s[1] = ly + h;
                        tb.s[3] = lx + w;
                        tb.s[4] = lx + w / 2;
                        tb.s[5] = ly + 4;
                        tb.s[9] = 1;
                        tb.s[0] = ly;
                        tb.s[2] = lx;
                        func_001F75F0(&tb, 0x80FFA888, &buf, -1);
                    }
                }
                i++;
            } while (!(D_001A00F0.icons[i].flags & 4));
        }
        }
    }

    {
        MapState *m = &D_001A00F0;

    if (D_0015ED84 == m->cur) {
        s32 img;
        f32 s;
        f32 ang;
        s32 flip;
        

        img = func_001FF960(0xE99A, 5);
        s = ((m->zoom[m->cur] * 4.0f + 10.0f) * 0.75f) / 13.0f;
        ang = D_0013F350.angle;
        flip = D_0013F350.mode == 0xF;
        if (flip) {
            ang = func_001FA580(ang, 1.5707964f);
        }
        if (D_0015FD60 != 0) {
            func_00208408(&buf.f[0], &buf.f[1], D_0015ED84 + 100, D_0013F350.x, D_0013F350.y);
            ang = func_001FA580(ang, 1.5707964f);
        } else {
            func_00208408(&buf.f[0], &buf.f[1], D_0015ED84, D_0013F350.x, D_0013F350.y);
        }
        buf.f[0] = (f32)rx0 + buf.f[0] * (f32)(rx1 - rx0);
        buf.f[1] = (f32)ry0 + buf.f[1] * (f32)(ry1 - ry0);
        if (D_0015EDB4 != 0) {
            ang = func_001FA5C8(-func_001FA580(ang, 1.5707964f), 1.5707964f);
        }
        {
            f32 sz = s * 256.0f;

            func_00200600(buf.f[0], buf.f[1], sz, sz, ang, 0x40, 0x40, func_001FFA10(img));
        }
    }
    }
    func_001F4398();
    if (D_001A00F0.grid != 0) {
        func_001F4280(0);
        func_00208508(rx0, ry0, rx1, ry1);
        func_001F4398();
    }
}
#endif /* NON_MATCHING */
