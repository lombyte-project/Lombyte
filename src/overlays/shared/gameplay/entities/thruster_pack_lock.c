/* Shared moby classes: thruster_pack_lock. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0030FB58), where it is exact; names translated to the US level program. */

extern int D_L11_0015FFD8; /* no foreign declaration */
extern short *D_L11_001AC240[];

void FUN_L11_0030e690(char *moby) {
    short *p = D_L11_001AC240[*(unsigned char *)(moby + 0x21)];
    int base = D_L11_0015FFD8;
    do {
        int i = (*p & 0x7FFF) * 256;
        char *e = (char *)(i + base);
        char *s;
        *(float *)(e + 0x18) = *(float *)(moby + 0x18);
        s = D_L11_001DA880 + *(int *)(*(char **)(e + 0x78) + 4) * 0x1190;
        *(float *)(s + 8) = *(float *)(moby + 0x18);
    } while (*p++ >= 0);
}
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math/vector.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_003102C8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct { unsigned char pad0[0x454]; unsigned char collected[1]; } LvState_3102C8;

typedef struct {
    char pad000[0x360];
    unsigned short count;   /* 0x360 */
    unsigned short best;    /* 0x362 */
    unsigned int levels;    /* 0x364 */
    char pad368[0x22];
    unsigned short best2;   /* 0x38A */
} Stats_3102C8;

typedef struct {
    char pad00[0x10];
    float pos[2];           /* 0x10 */
    float z;                /* 0x18 */
    char pad1C[0x4];
    unsigned char state;    /* 0x20 */
    char pad21[0x57];
    char *vars;             /* 0x78 */
    char pad7C[0x34];
    unsigned char bB0;      /* 0xB0 */
    char padB1;
    unsigned short uid;     /* 0xB2 */
} Lock_3102C8;

extern unsigned char D_00141968_r[] __asm__("D_00141968");
extern unsigned char D_0014C190_r[] __asm__("D_0014C190");
extern unsigned char D_0013F350_r[] __asm__("D_0013F350");
extern unsigned char D_0014C050_r[] __asm__("D_0014C050");
extern unsigned char D_0013F3D0_r[] __asm__("D_0013F3D0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int D_0014C050; /* no foreign declaration */
extern int D_L11_0015FC88; /* no foreign declaration */
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
extern unsigned char D_0013F350[];
s32 FUN_L00_00203908(s32 a, s32 b);
extern u8 D_0013D4C0[];
extern unsigned char D_0014171B_3102C8[] __asm__("D_0013F350");
extern LvState_3102C8 D_L11_001BBF40_3102C8 __asm__("D_L11_001BBC30");
extern LvState_3102C8 D_L11_001BB2E0_3102C8 __asm__("D_L11_001BAFD0");
extern unsigned char D_L11_0015FD08_3102C8[] __asm__("D_L11_0015FC88");
extern void func_0022ED80_3102C8(int, int, void *) __asm__("FUN_0022da68");
extern int func_L00_00203F20_3102C8(int a, int b) __asm__("FUN_L00_00203908");
extern float func_001F9D48_3102C8(void *, void *) __asm__("FUN_001f9b80");
extern int func_001F9850_3102C8(int) __asm__("FUN_001f96f8");

void thruster_pack_lock_update(Lock_3102C8 *moby) __asm__("FUN_L11_0030ee00");
void thruster_pack_lock_update(Lock_3102C8 *moby)
{
    char *vars = moby->vars;

    switch (moby->state) {
    case 0: {
        int level = current_level_index;
        ((Stats_3102C8 *)(D_00141968_r))->count = 0;
        ((Stats_3102C8 *)(D_00141968_r))->best = 0;
        *(unsigned short *)(vars + 0x3E) |= 8;
        if (level == 0xB && moby->bB0 != 0xFF) {
            if (D_L11_001BBF40_3102C8.collected[(short)moby->uid] != 0 ||
                ((*(int *)(D_0014C190_r + (((short)moby->uid >> 5) * 4 + (level << 8))) >> (moby->uid & 0x1F)) & 1)) {
                moby->state = 2;
                moby->z = moby->z - 1.5f;
                func_0022ED80_3102C8(0, 0, moby);
                return;
            }
        }
        moby->state = 1;
        break;
    }
    case 1:
        if (((void *)((Hero *)(D_0013F350_r))->ground_moby) == moby && D_0013D4C0[3] != 0 &&
            ((Hero *)(D_0013F350_r))->air_frames.s == 0 && ((Hero *)(D_0013F350_r))->state.current == 0x22) {
            if (current_level_index == 0xB && moby->bB0 != 0xFF) {
                int group;
                int group_index;
                D_L11_001BB2E0_3102C8.collected[(short)moby->uid] = moby->bB0 + 2;
                group = moby->bB0;
                group_index = group & 0xFF;
                if (group == 0xFF || (D_L11_0015FD08_3102C8[group_index] != 0xFF &&
                                  (D_0014C050_r)[group_index + (current_level_index << 4)] == 0xFF)) {
                    D_L11_001BBF40_3102C8.collected[(short)moby->uid] = group + 2;
                }
            }
            moby->state = 2;
            moby->z = moby->z - 1.5f;
            func_0022ED80_3102C8(0, 0, moby);
        }
        if (moby->bB0 == 0xFF) {
            return;
        }
        if (!(func_001F9D48_3102C8(moby->pos, D_0013F3D0_r) < 4.0f)) {
            return;
        }
        if (!((unsigned int)*(int *)(D_0013F3D0_r + 0x200C) < 2)) {
            return;
        }
        if (D_0013D4C0[3] == 0) {
            int elapsed = func_001F9850_3102C8(D_0015EEA4) - ((Stats_3102C8 *)(D_00141968_r))->best2 * 600;
            if ((int)((float)func_001F9850_3102C8(0x12) * 60.0f) < elapsed ||
                ((Stats_3102C8 *)(D_00141968_r))->best2 * 600 == 0) {
                func_L00_00203F20_3102C8(0x2B02, 0x71);
                return;
            }
            {
                int now = func_001F9850_3102C8(D_0015EEA4) / 600;
                if (((Stats_3102C8 *)(D_00141968_r))->best2 < now) {
                    ((Stats_3102C8 *)(D_00141968_r))->best2 = func_001F9850_3102C8(D_0015EEA4) / 600;
                }
            }
        } else if (((Stats_3102C8 *)(D_00141968_r))->count != 0) {
            int elapsed = func_001F9850_3102C8(D_0015EEA4) - ((Stats_3102C8 *)(D_00141968_r))->best * 600;
            if ((int)((float)func_001F9850_3102C8(0x12) * 60.0f) < elapsed ||
                ((Stats_3102C8 *)(D_00141968_r))->best * 600 == 0) {
                func_L00_00203F20_3102C8(0x2B01, 0x6C);
                return;
            }
            {
                int now = func_001F9850_3102C8(D_0015EEA4) / 600;
                if (((Stats_3102C8 *)(D_00141968_r))->best < now) {
                    ((Stats_3102C8 *)(D_00141968_r))->best = func_001F9850_3102C8(D_0015EEA4) / 600;
                }
            }
        } else {
            ((Stats_3102C8 *)(D_00141968_r))->count += 1;
            {
                int now = func_001F9850_3102C8(D_0015EEA4) / 600;
                if (((Stats_3102C8 *)(D_00141968_r))->best < now) {
                    ((Stats_3102C8 *)(D_00141968_r))->best = func_001F9850_3102C8(D_0015EEA4) / 600;
                }
            }
            ((Stats_3102C8 *)(D_00141968_r))->levels =
                ((Stats_3102C8 *)(D_00141968_r))->levels | (1 << current_level_index) | 0x80000000;
        }
        break;
    }
}
#include "qcopy.h"

/* Builds one GIF packet for a rotated textured sprite and advances the packet pointer. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_003121C0), where it is exact; names translated to the US level program. */

extern int D_L11_001611C0 __attribute__((section(".sdata")));
extern unsigned char screen_extent_c[] __asm__("D_0013E500");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern int ftoi(float) __asm__("FUN_001fa6d0");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_cos(float) __asm__("FUN_001f9dc8");

void FUN_L11_00310cf8(float x, float y, float width, float height, float angle, int texture_width,
                       int texture_height, long texture, int z_and_fog, int red, int green, int blue,
                       int alpha) {
    float vertical[4];
    float horizontal[4];
    float center[4];
    float top_left[4];
    float bottom_left[4];
    float top_right[4];
    float bottom_right[4];
    unsigned long rgba;
    unsigned long green_bits;
    unsigned long blue_bits;
    unsigned long alpha_bits;
    unsigned long tex0;
    int depth;
    unsigned long packed_depth;
    unsigned long gif_tag;
    unsigned long gif_regs;
    unsigned long prim;
    unsigned long tex_size;
    int gif_dma_tag;
    int u_end;
    int v_end;
    char *tag;
    unsigned long *packet;
    int *extent;
    float scale;

    rgba = red & 0xFF;
    green_bits = green & 0xFF;
    blue_bits = blue & 0xFF;
    alpha_bits = alpha & 0xFF;
    tex0 = texture;
    depth = z_and_fog;

    center[0] = x;
    center[1] = y;
    vertical[0] = height * fast_sin(angle);
    vertical[1] = height * fast_cos(angle);
    horizontal[0] = width * fast_cos(angle);
    horizontal[1] = -width * fast_sin(angle);
    add_vector_xyz(top_left, center, vertical);
    subtract_vector_xyz(top_left, top_left, horizontal);
    add_vector_xyz(bottom_left, center, vertical);
    subtract_vector_xyz(top_right, center, horizontal);
    qcopy(bottom_right, center);

    *(int *)(D_L11_001611C0 + 0) = 0x10000007;
    green_bits = green_bits << 8;
    rgba = rgba | green_bits;
    gif_dma_tag = 0x50000007;
    gif_regs = 0x53535353106;
    *(int *)(D_L11_001611C0 + 4) = 0;
    blue_bits = blue_bits << 16;
    rgba = rgba | blue_bits;
    alpha_bits = alpha_bits << 24;
    prim = 0x154;
    tex_size = 0x100010;
    gif_tag = 0xB400000000008001;
    rgba = rgba | alpha_bits;
    *(int *)(D_L11_001611C0 + 8) = 0;
    scale = 16.0f;
    u_end = texture_width << 4;
    v_end = texture_height << 20;
    packed_depth = (unsigned long)depth << 32;
    *(int *)(D_L11_001611C0 + 12) = gif_dma_tag;
    tag = (char *)D_L11_001611C0;
    packet = (unsigned long *)(tag + 0x10);
    D_L11_001611C0 = (int)packet;
    *(unsigned long *)(tag + 0x10) = gif_tag;
    packet[1] = gif_regs;
    packet[2] = tex0;
    packet[3] = prim;
    packet[4] = rgba;
    packet[5] = tex_size;

    {
        int a = ftoi(top_left[0] * scale);
        int b = ftoi(top_left[1] * scale);
        extent = (int *)(screen_extent_c);
        packet[6] = (unsigned long)(a + extent[4] - 8) | ((unsigned long)(b + extent[5] - 8) << 16) | packed_depth;
        packet[7] = u_end;
    }
    {
        int a = ftoi(bottom_left[0] * scale);
        int b = ftoi(bottom_left[1] * scale);
        packet[8] = (unsigned long)(a + extent[4] - 8) | ((unsigned long)(b + extent[5] - 8) << 16) | packed_depth;
        packet[9] = v_end;
    }
    {
        int a = ftoi(top_right[0] * scale);
        int b = ftoi(top_right[1] * scale);
        v_end = v_end + u_end;
        packet[10] = (unsigned long)(a + extent[4] - 8) | ((unsigned long)(b + extent[5] - 8) << 16) | packed_depth;
        packet[11] = v_end;
    }
    {
        int a = ftoi(bottom_right[0] * scale);
        int b = ftoi(bottom_right[1] * scale);
        packet[12] = (unsigned long)(a + extent[4] - 8) | ((unsigned long)(b + extent[5] - 8) << 16) | packed_depth;
        packet[13] = 0;
    }

    /* Fakematch: the store through the address keeps the reload after the last packet store. */
    *(char **)&D_L11_001611C0 += 0x70;
}
#define NOT_SDA

#define MACRO_ADDR

/* Draws four quads with the angle advanced by a quarter turn each time. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00312510), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern s32 get_effect_texture() __asm__("FUN_001f44b8");

void FUN_L11_00311048(unsigned char a, unsigned char b, unsigned char c, unsigned char d, float x,
                      float y, float s, float ang) {
    int h;
    float w;
    h = get_effect_texture(0x42);
    w = s * 20.0f;
    FUN_L11_00310cf8(x, y, w, w, ang, 0x3F, 0x3F, h, 0xFFFFF3, a, b, c, d);
    ang = fast_add_rotations(ang, 1.5707964f);
    FUN_L11_00310cf8(x, y, w, w, ang, 0x3F, 0x3F, h, 0xFFFFF3, a, b, c, d);
    ang = fast_add_rotations(ang, 1.5707964f);
    FUN_L11_00310cf8(x, y, w, w, ang, 0x3F, 0x3F, h, 0xFFFFF3, a, b, c, d);
    ang = fast_add_rotations(ang, 1.5707964f);
    FUN_L11_00310cf8(x, y, w, w, ang, 0x3F, 0x3F, h, 0xFFFFF3, a, b, c, d);
}
/* Projects a world point to screen and stores its x and y in 1/16 units, y plus an offset. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_003126D8), where it is exact; names translated to the US level program. */

extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void project_to_screen(f32 *, void *) __asm__("FUN_001f2070");
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

void FUN_L11_00311210(void *pt, int *outx, int *outy, int yoff) {
    float p[4];
    int *g;
    project_to_screen(p, pt);
    g = (int *)(D_0013E05A + 0x4A6);
    *outx = func_001FA898_r((p[0] - (float)g[4]) * 0.0625f);
    *outy = func_001FA898_r((p[1] - (float)g[5]) * 0.0625f) + yoff;
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_00312780), where it is exact; names translated to the US level program. */


extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern f32 D_L11_00161FF4_gp __asm__("D_L11_00161FF4") __attribute__((sda));
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void FUN_L00_0025f8e0(float *v, float s);
extern void FUN_L01_0028a7a8(void *, float *, float *, void *, int);

extern short D_L11_001621C8 __attribute__((sda));
extern short D_L11_001621D8 __attribute__((sda));
extern short D_L11_001621E8 __attribute__((sda));
extern short D_L11_001621F8 __attribute__((sda));
extern short D_L11_00162208 __attribute__((sda));
void FUN_L11_003112b8(void *m, char *t) {
    int i;
    for (i = 1; i < 3; i++) {
        float vel[4];
        float pos[4];
        float a[4];
        float b[4];
        float c[4];
        float scale;

        *(OvlQuad *)vel = 0;
        scale = *(float *)(t + 0x64) / *(float *)&D_L11_00161FF4_gp;
        if (scale > 1.0f) {
            scale = 1.0f;
        } else if (scale < 0.35f) {
            scale = 0.35f;
        }
        moby_point_position(m, i, pos);
        pos[3] = 0.0f;
        qcopy(a, pos);
        qcopy(b, pos);
        qcopy(c, pos);
        FUN_L00_0025f8e0(a, *(float *)(t + 0xC4) * 0.02f + 0.02f);
        FUN_L00_0025f8e0(b, *(float *)(t + 0xC4) * 0.02f + 0.02f);
        FUN_L00_0025f8e0(c, *(float *)(t + 0xC4) * 0.01f + 0.02f);
        vel[0] =
            -((random_float_between_alt(6.0f, 7.0f) + *(float *)(t + 0xC4) * 2.0f) * frame_time) *
            scale;
        FUN_L01_0028a7a8(m, a, vel, &D_L11_001621C8, 1);
        vel[0] =
            -((random_float_between_alt(6.0f, 7.0f) + *(float *)(t + 0xC4) * 3.0f) * frame_time) *
            scale;
        FUN_L01_0028a7a8(m, b, vel, &D_L11_001621D8, 1);
        vel[0] =
            -((random_float_between_alt(6.0f, 7.0f) + *(float *)(t + 0xC4) * 4.0f) * frame_time) *
            scale;
        FUN_L01_0028a7a8(m, c, vel, &D_L11_001621E8, 1);
        if (*(float *)(t + 0xC4) > 0.5f) {
            FUN_L00_0025f8e0(a, *(float *)(t + 0xC4) * 0.02f + 0.01f);
            FUN_L00_0025f8e0(b, *(float *)(t + 0xC4) * 0.02f + 0.01f);
            FUN_L00_0025f8e0(c, *(float *)(t + 0xC4) * 0.01f + 0.01f);
            vel[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(t + 0xC4) * 2.0f) *
                       frame_time) *
                     scale;
            FUN_L01_0028a7a8(m, a, vel, &D_L11_001621C8, 1);
            vel[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(t + 0xC4) * 3.0f) *
                       frame_time) *
                     scale;
            FUN_L01_0028a7a8(m, b, vel, &D_L11_001621D8, 1);
            vel[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(t + 0xC4) * 4.0f) *
                       frame_time) *
                     scale;
            FUN_L01_0028a7a8(m, c, vel, &D_L11_001621E8, 1);
            vel[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(t + 0xC4) * 2.0f) *
                       frame_time) *
                     scale;
            FUN_L01_0028a7a8(m, a, vel, &D_L11_001621F8, 1);
            vel[0] = -((random_float_between_alt(6.0f, 7.0f) + *(float *)(t + 0xC4) * 3.0f) *
                       frame_time) *
                     scale;
            FUN_L01_0028a7a8(m, pos, vel, &D_L11_00162208, 1);
        }
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L11_003192c8.s", FUN_L11_003192c8);
#else
extern unsigned char D_0015EDB2;
extern float D_L11_001F1490[];
extern short D_L11_001623F0_d __asm__("D_L11_001623F0") __attribute__((sda));
extern long long trail_effect_texture(int) __asm__("FUN_001f44b8");
extern float integer_to_float(int) __asm__("FUN_001fa6c0");
extern int tween_color(float, int, int) __asm__("FUN_001fa6e0");
extern void memset(void *, int, int) __asm__("FUN_001153fc");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void draw_packet(void *, int, int) __asm__("FUN_001f7d30");
extern int level_frame __asm__("D_L11_0015F5CC");
extern int D_L11_0015FFD8;
extern short *D_L11_001AC240[];

int FUN_L11_003192c8(char *moby) {
    char stack_data[0x900] __attribute__((aligned(16)));
    float offset[8] __attribute__((aligned(16)));
    short *list;
    int i;
    short *next_list;
    float phase;
    float *position_base;
    float *position_y_base;
    int *colors_base;

    if ((unsigned char)moby[0x21] == 0xff) {
        return;
    }
    if (D_0015EDB2 != 0) {
        *(float *)&D_L11_001623F0_d = 16.0f;
    } else {
        *(float *)&D_L11_001623F0_d = 5.0f;
    }
    position_y_base = (float *)(stack_data + 0x54);
    position_base = (float *)(stack_data + 0x50);
    colors_base = (int *)(stack_data + 0x40);
    for (i = 0; i < 16; i++) {
        long long texture;
        long long *packet = (long long *)(stack_data + 0x70);
        float *source;
        float *position;
        float *position_y;
        int *colors;
        float span;
        int j;
        j = 0;
        texture = trail_effect_texture(0x13);
        packet[i * 18 + 1] = texture;
        packet[i * 18 + 3] = 0x8000000048LL;
        packet[i * 18 + 2] = 0xff9000000260LL;
        packet[i * 18] = 0;
        span = *(float *)&D_L11_001623F0_d * 4.0f;
        phase = integer_to_float((level_frame + 3) & 3) / span;
        source = D_L11_001F1490;
        position = position_base + i * 0x24;
        position_y = position_y_base + i * 0x24;
        colors = colors_base + i * 0x24;
        for (; j < 4; j++) {
            float t;
            position[j * 2] = source[j * 2];
            position_y[j * 2] = source[j * 2 + 1];
            t = integer_to_float(i - ((j >> 1) - 1)) / *(float *)&D_L11_001623F0_d + phase;
            if (t > 1.0f) t = 1.0f;
            else if (t < 0.0f) t = 0.0f;
            colors[j] = tween_color(t, 0x8c28aa28, 0x144646);
        }
    }
    memset(offset, 0, 0x20);
    offset[2] = 0.25f;
    offset[6] = -0.25f;
    list = D_L11_001AC240[(unsigned char)moby[0x21]];
    do {
        unsigned short entry = *(unsigned short *)list;
        char *linked = (char *)(((entry & 0x7fff) << 8) + D_L11_0015FFD8);
        int skip = *(short *)(linked + 0xa6) != *(short *)(moby + 0xa6);
        if (*(unsigned char *)(linked + 0x31) == 0) skip = 1;
        next_list = list + 1;
        if (skip == 0) {
            char *trail = *(char **)(linked + 0x78);
            long limit = -1;
            for (i = 0; i < *(int *)(trail + 0x2c0); i++) {
                int next = *(int *)(trail + 0x2c4) - i;
                int a = next + 15;
                int b = next + 30;
                int start;
                int j;
                if (limit < a) b = a;
                start = b >> 4;
                next = start << 4;
                start = a - next;
                for (j = 0; j < 2; j++) {
                    int k;
                    char *packet = stack_data + i * 0x90;
                    char *points = trail + 0xc0 + j * 0x100;
                    for (k = 0; k < 4; k++) {
                        int n = start + (k >> 1);
                        int q = n + 15;
                        if (limit < n) q = n;
                        q = (q >> 4) << 4;
                        n = n - q;
                        add_vector_xyz((float *)stack_data + i * 0x24 + k * 4, points + n * 16,
                                       (char *)offset + (k & 1) * 16);
                    }
                    draw_packet(packet, 0, 0);
                }
            }
        }
        if (*list < 0) break;
        list = next_list;
    } while (1);
}
#endif
/* Advances a trail ring buffer and records two offset points each fourth frame. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0031AAE0), where it is exact; names translated to the US level program. */


extern int level_frame __asm__("D_L11_0015F5CC");
extern int FUN_L11_003192c8();
extern short D_L11_001623F0_d __asm__("D_L11_001623F0") __attribute__((sda));
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
void enqueue_callback_list_1(int arg0, int arg1) __asm__("FUN_001f4600");
extern void f49(void *, void *) __asm__("FUN_001f4600");
extern void FUN_L11_003192c8_c(void) __asm__("FUN_L11_003192c8");

void FUN_L11_00319670(char *moby) {
    char *d = *(char **)(moby + 0x78);
    float v[4];
    float w[4];
    int t;
    if ((level_frame & 3) == 0) {
        int cnt;
        float lim = *(float *)&D_L11_001623F0_d;
        cnt = *(int *)(d + 0x2C0);
        *(int *)(d + 0x2C4) = (*(int *)(d + 0x2C4) + 1) % 16;
        if ((float)cnt < lim) {
            *(int *)(d + 0x2C0) = cnt + 1;
        }
    }
    *(OvlQuad *)v = 0;
    v[1] = 0.7f;
    v[0] = -1.2f;
    v[2] = 0.3f;
    transform_vector_by_basis(v, v, moby + 0xC0);
    add_vector_xyz(d + ((*(int *)(d + 0x2C4) << 4) + 0xC0), v, moby + 0x10);
    *(float *)(d + (*(int *)(d + 0x2C4) << 4) + 0xCC) = 1.0f;
    *(OvlQuad *)w = 0;
    w[0] = -1.2f;
    w[1] = -0.7f;
    w[2] = 0.3f;
    transform_vector_by_basis(w, w, moby + 0xC0);
    add_vector_xyz(d + ((*(int *)(d + 0x2C4) << 4) + 0x1C0), w, moby + 0x10);
    t = level_frame;
    *(float *)(d + (*(int *)(d + 0x2C4) << 4) + 0x1CC) = 1.0f;
    if (t != 0) {
        int *p = *(int **)(d + 0x2C8);
        if (t != *p) {
            *p = t;
            f49(FUN_L11_003192c8_c, moby);
        }
    }
}

#define NOT_SDA

#define MACRO_ADDR

/* Collectable crate: when hit, updates the totals (count, best time, levels mask, a level-11 bonus) and breaks open. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002C99E0.c: func_L11_0031B630), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int D_0015EE00;
extern int coll_sphere(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern unsigned char D_0013D408[];
extern unsigned char D_00141968[];
extern void queue_dialogue_message(int, int) __asm__("FUN_L00_00263d40");
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
s32 allocate_voice_for_bank_entry(s32 arg1, s32 arg0, s32 arg2) __asm__("FUN_0022db10");

void FUN_L11_0031a1c0(char *m) {
    char *d = *(char **)(m + 0x78);
    char *info = find_moby_hit(m, 0x330000, 0);
    struct Hero *g;
    ((unsigned char *)m)[0xA4] = 0xFF;
    g = &hero;
    if (g->state.current != 0x32) {
        *(unsigned short *)(m + 0x34) &= 0xEFFF;
    } else {
        *(unsigned short *)(m + 0x34) |= 0x1000;
    }
    if (info != 0) {
        char *st = (char *)D_00141968;
        int st2;
        int cur;
        if (*(unsigned short *)(st + 0x157E8) < 0xFFFF) {
            *(unsigned short *)(st + 0x157E8) += 1;
        }
        if (scale_game_frames(D_0015EEA4) / 600 > *(unsigned short *)(st + 0x157EA)) {
            *(unsigned short *)(st + 0x157EA) = scale_game_frames(D_0015EEA4) / 600;
        }
        *(int *)(st + 0x157EC) = *(int *)(st + 0x157EC) | (1 << current_level_index) | 0x80000000;
        if (current_level_index == 0xB && *(char **)(info + 0x20) != 0 &&
            *(short *)(*(char **)(info + 0x20) + 0xA6) == 0xAC) {
            if (++D_0015EE00 >= 3 && D_0013D408[0x12] == 0) {
                D_0013D408[0x12] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        if (*(int *)(d + 0x88) != 0) {
            st2 = 6;
            cur = ((unsigned char *)m)[0x20];
        } else {
            cur = ((unsigned char *)m)[0x20];
            st2 = 2;
        }
        if (cur != st2) {
            m[0x20] = st2;
            if (((unsigned char *)m)[0x53] != 1) {
                blend_moby_animation(m, 1, 0, scale_game_frames(10));
            }
        }
    } else if (((unsigned char *)m)[0x20] == 5) {
        struct Hero *g2 = &hero;
        if (g2->state.current != 0x32) {
            m[0x20] = 4;
            *(int *)(m + 0x94) = 0;
            *(unsigned short *)(m + 0x34) |= 1;
        }
        if (coll_sphere(1.0f, m + 0x10, 0, m)) {
            m[0x20] = 6;
            if (((unsigned char *)m)[0x53] != 1) {
                blend_moby_animation(m, 1, 0, scale_game_frames(10));
            }
        }
    }
}

/* Queues a GIF packet that draws a line strip through the given points, scaled and offset to the screen. */
typedef unsigned int u128_ad0 __attribute__((mode(TI)));
extern int D_L11_001611C0_lui __asm__("D_L11_001611C0");
extern int screen_extent[] __asm__("D_0013E500");
extern float itof_g(int) __asm__("FUN_001fa6c0");
extern int ftoi_g(float) __asm__("FUN_001fa6d0");

extern u128_ad0 D_L11_001608E0;
extern u128_ad0 D_L11_001608F0;
void FUN_L11_00310ad0(short *pts, int n, float x0, float y0, unsigned int col, unsigned long long prim, float scale) {
    int q = (n + 1) / 2 + 3;
    unsigned long long *o;
    int x, y;
    int i;
    *(int *)(D_L11_001611C0 + 0) = q | 0x10000000;
    *(int *)(D_L11_001611C0 + 4) = 0;
    *(int *)(D_L11_001611C0 + 8) = 0;
    *(int *)(D_L11_001611C0 + 12) = q | 0x50000000;
    {
        char *dma_header = (char *)D_L11_001611C0;
        D_L11_001611C0 = (int)(dma_header + 0x10);
        qcopy((void *)D_L11_001611C0, &D_L11_001608E0);
        *(short *)(dma_header + 0x10) = -0x7FFF;
    }
    {
        unsigned long long *primitive_payload = (unsigned long long *)((char *)D_L11_001611C0 + 0x10);
        D_L11_001611C0 = (int)primitive_payload;
        primitive_payload[0] = 0x144;
        primitive_payload[1] = prim;
    }
    {
        char *saved_packet_cursor = (char *)D_L11_001611C0;
        D_L11_001611C0 = (int)(saved_packet_cursor + 0x10);
        qcopy((void *)D_L11_001611C0, &D_L11_001608F0);
        *(short *)(saved_packet_cursor + 0x10) = n - 0x8000;
    }
    D_L11_001611C0 += 0x10;
    o = (unsigned long long *)D_L11_001611C0;
    for (i = 0; i < n; i++) {
        x = ftoi_g((itof_g(pts[i * 2]) * scale + x0) * 16.0f);
        y = ftoi_g((itof_g(pts[i * 2 + 1]) * scale + y0) * 16.0f);
        o[i] = (unsigned long long)(x + screen_extent[4] - 8) | ((unsigned long long)(y + screen_extent[5] - 8) << 16) | ((unsigned long long)col << 32);
    }
    D_L11_001611C0 += ((n + 1) / 2) * 16;
}
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void interpolate_vector_xyz(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern float vector_distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern void normalize_vector_xyz(void *, void *, float) __asm__("FUN_001f9bf8");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_atan2(float, float) __asm__("FUN_001f9e90");
extern void rotation_matrix_from_euler(void *, void *) __asm__("FUN_001fa050");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");

#include "rnc/gameplay/entities/moby.h"
#define MOBY(p) ((struct Moby *)(p))

void FUN_L11_0031a438(char *moby) {
    char *d = (char *)MOBY(moby)->pvars;
    int i = *(int *)(d + 0x64);
    char *table = *(char **)(path_table + *(int *)(d + 0x60) * 4);
    int count;
    float matrix[16];
    float a[4], b[4], c[4];
    float point[4], turn[4], direction[4];
    float angle_a, angle_b, slope_a, slope_b;
    float distance;
    float speed;

    qcopy(a, table + i * 16 + 0x10);
    count = *(int *)table;
    qcopy(b, table + (i + 1) % count * 16 + 0x10);
    qcopy(c, table + (i + 2) % count * 16 + 0x10);
    clear_u64_value(turn);
    angle_a = fast_atan2(a[0] - b[0], a[1] - b[1]);
    angle_b = fast_atan2(b[0] - c[0], b[1] - c[1]);
    slope_a = fast_atan2(vector_distance_xy(a, b), b[2] - a[2]);
    slope_b = fast_atan2(vector_distance_xy(b, c), c[2] - b[2]);
    turn[1] = fast_add_rotations(fast_subtract_rotations(slope_b, slope_a) * *(float *)(d + 0x68), slope_a);
    turn[2] = fast_add_rotations(fast_subtract_rotations(angle_b, angle_a) * *(float *)(d + 0x68), angle_a);
    qcopy(&MOBY(moby)->rot, turn);
    if (*(float *)(d + 0x6C) > 0.0f) {
        MOBY(moby)->rot.z = fast_add_rotations(MOBY(moby)->rot.z, 3.14159f);
        MOBY(moby)->rot.y = -MOBY(moby)->rot.y;
    }
    interpolate_vector_xyz(point, a, b, *(float *)(d + 0x68));
    rotation_matrix_from_euler(matrix, turn);
    normalize_vector_xyz(direction, matrix + 4, *(float *)(d + 0x78) * fast_sin(*(float *)(d + 0x70)));
    add_vector_xyz(point, point, direction);
    normalize_vector_xyz(direction, matrix + 8, *(float *)(d + 0x78) * fast_cos(*(float *)(d + 0x70)));
    add_vector_xyz(point, point, direction);
    speed = 0.0f;
    distance = vector_distance_xyz(&MOBY(moby)->pos, point);
    advance_accelerated_scalar(&speed, (float *)(d + 0x80), distance, frame_time_sq * 10.0f, frame_time_sq * 10.0f, frame_time * 20.0f);
    subtract_vector_xyz(direction, point, &MOBY(moby)->pos);
    normalize_vector_xyz(direction, direction, *(float *)(d + 0x80));
    add_vector_xyz(&MOBY(moby)->pos, &MOBY(moby)->pos, direction);
}

