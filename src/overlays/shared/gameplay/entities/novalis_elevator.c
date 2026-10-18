/* Shared moby classes: novalis_elevator, bolt_crank, gold_weapon_case, sand_mouse, large_amoeboid, sandmouse_house. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/overlay/collision.h"
#include "rnc/data_labels.h"
#include "rnc/ui/text/font_state.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/overlay/level_labels.h"


void FUN_L01_002b8c08(f32 *pos) {
    volatile f32 *dst = (volatile f32 *)0x70002000;
    f32 y = pos[1] + pool_light[3];
    s32 i, j;
    for (i = 0; i < 17; i++) {
        f32 x = pos[0] + pool_light[2];
        f32 *row = (f32 *)0x70000010 + (i + 1) * 24;
        for (j = 0; j < 17; j++) {
            *dst = x;
            dst += 1;
            *dst = y;
            dst += 1;
            *dst = row[j] + pos[2];
            dst += 1;
            x += pool_light[4];
        }
        y += pool_light[5];
    }
}
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern u32 is_vector_outside_clip(void *) __asm__("FUN_001f9958");
extern void FUN_L01_00262388(s32, s32);
extern char D_L01_00167240[];

void FUN_L01_002b8cb0(f32 *pos, s32 index, s32 mode) {
    f32 corners[4][4];
    f32 near_clip[4];
    f32 far_clip[4];
    f32 offsets[4][4];
    f32 x_offset, y_offset, x_end, y_end;
    f32 half_x, half_y;
    f32 x, y;
    u32 near_bits, far_bits;
    s32 i, value, row, column;

    if (mode != 0) {
        row = index >> 2;
        column = index & 3;
        half_x = 0.5f * -pool_light[2];
        half_y = 0.5f * -pool_light[3];
        x_offset = half_x * (f32)column;
        y_offset = half_y * (f32)row;
        y_end = y_offset + half_y;
        x_end = x_offset + half_x;
        offsets[0][1] = y_offset;
        offsets[0][0] = x_offset;
        offsets[1][1] = y_offset;
        offsets[1][0] = x_end;
        offsets[2][0] = x_offset;
        offsets[2][1] = y_end;
        offsets[3][0] = x_end;
        offsets[3][1] = y_end;
        x = pos[0] + pool_light[2];
        y = pos[1] + pool_light[3];
        corners[0][0] = x + offsets[2][0];
        corners[0][1] = y + offsets[0][1];
        corners[0][2] = pos[2];
        corners[0][3] = 1.0f;
        corners[1][0] = x + offsets[1][0];
        corners[1][1] = y + offsets[1][1];
        corners[1][2] = pos[2];
        corners[1][3] = 1.0f;
        corners[2][0] = corners[0][0];
        corners[2][1] = y + offsets[2][1];
        corners[2][2] = pos[2];
        corners[2][3] = 1.0f;
        corners[3][0] = corners[1][0];
        corners[3][1] = corners[2][1];
        corners[3][2] = pos[2];
        corners[3][3] = 1.0f;
        near_bits = 0;
        far_bits = 0;
        for (i = 0; i < 4; i++) {
            subtract_vector_xyz(near_clip, corners[i], D_L01_00167240);
            scale_vector_xyz(far_clip, near_clip, 1024.0f);
            scale_vector_xyz(near_clip, near_clip, 1024.0f);
            transform_vector(far_clip, far_clip, D_L01_00167240 - 0x80);
            transform_vector(near_clip, near_clip, D_L01_00167240 - 0x40);
            far_bits = (far_bits << 8) | is_vector_outside_clip(far_clip);
            near_bits = (near_bits << 8) | is_vector_outside_clip(near_clip);
        }
        if (mode == 1) {
            if ((near_bits & 0x01010101) == 0x01010101) return;
            if ((near_bits & 0x02020202) == 0x02020202) return;
            if ((near_bits & 0x04040404) == 0x04040404) return;
            if ((near_bits & 0x08080808) == 0x08080808) return;
            if (far_bits == 0) mode = 0;
            if ((near_bits & 0x20202020) == 0x20202020) return;
        } else {
            if ((near_bits & 0x01010101) == 0x01010101) return;
            if ((near_bits & 0x02020202) == 0x02020202) return;
            if ((near_bits & 0x04040404) == 0x04040404) return;
            if ((near_bits & 0x08080808) == 0x08080808) return;
            if ((near_bits & 0x20202020) == 0x20202020) return;
            mode = 0;
        }
    }
    value = (index & 3) * 4 + (index >> 2) * 0x44;
    if (mode != 0) {
        mode = 0xe;
    } else {
        mode = 0xc;
    }
    FUN_L01_00262388(mode, value);
}
#include "qcopy.h"
#include "rnc/rendering/dma_tag.h"
typedef struct {
    u8 pad0[0x190];
    u8 v190[0x10];
    u8 v1A0[0x10];
    u8 pad1B0[0x60];
    f32 f210;
    u8 pad214[0x14];
    f32 f228;
    f32 f22C;
} L01Camera;

extern s32 D_L01_0015F5E0 __attribute__((section(".sdata")));
extern u32 D_L01_00161228 __attribute__((sda));
extern u32 D_L01_0016122C __attribute__((sda));
extern u32 D_L01_00161230 __attribute__((sda));
extern u32 D_L01_00161234 __attribute__((sda));
extern union PacketCursor l01_packet __asm__("D_L01_001611C0") __attribute__((section(".sdata")));
extern char D_L01_00167240[];
extern void set_scale_matrix(f32 (*)[4], f32) __asm__("FUN_001f9ff8");
extern void scale_vector_xyz(void *, void *, f32) __asm__("FUN_001f9a68");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void vu1_add_data_ref(void *, s32) __asm__("FUN_00233830");
extern void vu1_gs_regs_font(void) __asm__("FUN_00233c90");
extern void FUN_L01_002b4c28(void);

extern L01Camera D_L01_0016CEC0;
void FUN_L01_002b8f98(void) {
    f32 m[4][4];
    struct DmaTag *base;
    u8 *p;
    L01Camera *camera;

    set_scale_matrix(m, 1024.0f);
    scale_vector_xyz(m[3], D_L01_00167240, -1024.0f);
    m[3][3] = 1.0f;
    if (D_L01_0015F5E0 != 7) {
        vu1_add_data_ref(font_vu_program, font_vu_program_tag[0]);
        D_L01_0015F5E0 = 7;
    }
    l01_packet.tag->tag = 0x10000000;
    l01_packet.tag->addr = 0;
    l01_packet.tag->vif0 = 0x11000000;
    l01_packet.tag->vif1 = 0x1000404;
    base = l01_packet.tag;
    base[1].tag = 0;
    base[1].addr = 0;
    base[1].vif0 = 0;
    base[1].vif1 = 0x6C0C43A4;
    multiply_matrix(base + 2, D_L01_00167240 - 0x100, m);
    multiply_matrix(base + 6, D_L01_00167240 - 0x80, m);
    base[10].tag = 0x8000;
    base[10].addr = 0x303EC000;
    base[10].vif0 = 0x412;
    camera = &D_L01_0016CEC0;
    *(f32 *)&base[10].vif1 = camera->f210;
    p = (u8 *)(base + 11);
    qcopy(p, camera->v190);
    p = (u8 *)(base + 12);
    qcopy(p, camera->v1A0);
    *(f32 *)&base[13].tag = camera->f22C;
    *(f32 *)&base[13].addr = camera->f228;
    base[13].vif0 = 0;
    base[13].vif1 = 0;
    base[14].addr = 0x20001D2;
    base[14].vif0 = 0x15000000;
    base[14].tag = 0x3000000;
    base[14].vif1 = 0;
    p = (u8 *)(base + 15);
    l01_packet.tag->tag |= ((p - (u8 *)l01_packet.tag) >> 4) - 1;
    l01_packet.bytes = p;
    FUN_L01_002b4c28();
    l01_packet.tag->tag = 0x30000007;
    l01_packet.tag->addr = (u32)D_L01_001CAC80;
    l01_packet.tag->vif0 = 0;
    l01_packet.tag->vif1 = 0x50000007;
    D_L01_00161228 = 0xFFFFFFFF;
    D_L01_0016122C = 0xFFFFFFFF;
    D_L01_00161230 = 0xFFFFFFFF;
    D_L01_00161234 = 0xFFFFFFFF;
    l01_packet.tag++;
}
#include "qcopy.h"
#define F(p, o) (*(float *)((p) + (o)))

/* Path mover gated by an activation trigger and its collectable flag; plays the arrival cue on the way back. */
extern char *D_L01_001B0930[];
typedef struct {
    u8 pad0[0x454];
    u8 collected[1];
} L01PathLevelState;
extern L01PathLevelState D_L01_001BB6B0;
extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float multiply_global_factor_ed64(float) __asm__("FUN_001f96b0");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float FUN_001f9b48(void *, void *);
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
extern int FUN_L01_00277fb8(void *);
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("func_0022DA68");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern int hero_set_state(int, int) __asm__("FUN_L00_002223f8");
extern void FUN_L00_00227638(void);
extern void update_entity_matrix(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern void scale_vector_xyz(void *out, void *a, f32 s) __asm__("FUN_001f9a68");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
extern char D_0013E550[];
extern int func_001FA898_r(float) __asm__("FUN_001fa6d0");

#define F(p, o) (*(float *)((p) + (o)))

extern s32 D_0014C190_257218[][64] __asm__("D_0014C190");
void novalis_elevator_update(unsigned char *moby) __asm__("FUN_L01_002b9eb0");
void novalis_elevator_update(unsigned char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *path;
    float orig[4], delta[4], tmp[4];
    int on = 0;
    if (*(int *)(data + 0xB4) == -1) {
        return;
    }
    path = D_L01_001B0930[*(int *)(data + 0xB4)];
    if (*(int *)(data + 0xCC) != -1) {
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xCC)) == 0) {
            moby[0x30] = 0xFF;
            *(int *)(moby + 0x94) = 0;
            *(unsigned short *)(moby + 0x34) |= 0x41;
            return;
        }
        if (*(int *)(data + 0xCC) != -1 &&
            is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xCC))) {
            *(unsigned short *)(moby + 0x34) &= 0xFFBE;
            *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
            *(int *)(data + 0xCC) = -1;
        }
    }
    if (*(int *)(data + 0xC0) != -1) {
        unsigned short id;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(data + 0xC0)) ||
            D_L01_001BB6B0.collected[(short)(id = *(unsigned short *)(moby + 0xB2))] != 0 ||
            (D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 0x1F)) & 1) {
            if (*(int *)(data + 0xC0) != -1) {
                D_0014C190_257218[current_level_index][(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
                moby_killed[(short)*(unsigned short *)(moby + 0xB2) >> 5] |=
                    1 << (*(unsigned short *)(moby + 0xB2) & 0x1F);
                *(int *)(data + 0xC0) = -1;
            }
            on = 1;
        }
    } else {
        on = 1;
    }
    qcopy(orig, moby + 0x10);
    fast_dec_timer_s16(data + 0xBA);
    if (moby[0x20] == 0) {
        float f;
        char *pts;
        data[0x28] = 4;
        *(short *)(data + 0x3E) = 5;
        *(int *)(data + 0x20) = 0;
        *(short *)(data + 0x24) = 0;
        f = 0.0f;
        if ((moby[0xBC] = data[0xA0]) == 0) {
            f = 1.0f;
        }
        F(data, 0xA4) = f;
        pts = path + 0x10;
        qcopy((moby + 0x10), func_001FA898_r(f) ? pts + (*(int *)path - 1) * 16 : pts);
        *(short *)(data + 0xBA) = 0;
        *(int *)(data + 0xA8) = 0;
        *(int *)(data + 0xBC) = 0;
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        *(short *)(moby + 0x32) = 0xFF;
        *(int *)(data + 0xC4) = -1;
    }
    switch (moby[0xBC]) {
    case 0:
        if (on) {
            float far;
            if ((*(short *)(data + 0xB8) != 0 && FUN_L01_00277fb8(moby)) ||
                (far = FUN_001f9b48(&hero.motion.pos, path + *(int *)path * 16),
                 FUN_001f9b48(&hero.motion.pos, path + 0x10) < far)) {
                moby[0xBC] = 2;
                F(data, 0xAC) = -1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case 1:
        if (on) {
            float near;
            if ((*(short *)(data + 0xB8) != 0 && FUN_L01_00277fb8(moby)) ||
                (near = FUN_001f9b48(&hero.motion.pos, path + 0x10),
                 FUN_001f9b48(&hero.motion.pos, path + *(int *)path * 16) < near)) {
                moby[0xBC] = 2;
                F(data, 0xAC) = 1.0f / multiply_global_factor_ed64(F(data, 0xB0) * 60.0f);
                *(short *)(data + 0xB8) = 0;
            }
        }
        break;
    case 2:
        if (*(int *)(data + 0xC8) == 0 && FUN_L01_00277fb8(moby)) {
            struct Hero *g = &hero;
            g->unk1F2 = 4;
            g->unk1F4 = 4;
        }
        if (*(short *)(data + 0xBA) != 0 ||
            (F(data, 0xBC) < 0.0f &&
             vector_distance((moby + 0x10), &hero.motion.pos) < 2.83f &&
             absolute_float(hero.motion.pos.f[2] - F(moby, 0x18)) < 4.0f &&
             F(moby, 0x18) - hero.motion.pos.f[2] > 0.9f)) {
            int r;
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xBA) = scale_game_frames(30);
            }
            r = *(int *)(data + 0xC4);
            *(int *)(data + 0xA8) = 0;
            if (r != -1) {
                unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                    release_voice_slot(r);
                }
            }
            *(int *)(data + 0xC4) = -1;
        } else {
            int i;
            float f20, t;
            char *q;
            if (!voice_is_playing(moby, *(int *)(data + 0xC4))) {
                *(int *)(data + 0xC4) = allocate_voice_for_target_entry(0, 4, (int)moby);
            }
            F(data, 0xA8) += F(data, 0xAC) * frame_time;
            if (absolute_float(F(data, 0xA8)) > absolute_float(F(data, 0xAC))) {
                F(data, 0xA8) = F(data, 0xAC);
            }
            F(data, 0xA4) = F(data, 0xA4) + F(data, 0xA8);
            if (0.5f < absolute_float(F(data, 0xA4) - 0.5f)) {
                int r;
                if (0.0f < F(data, 0xAC)) {
                    moby[0xBC] = 0;
                    F(data, 0xA4) = 1.0f;
                } else {
                    moby[0xBC] = 1;
                    F(data, 0xA4) = 0.0f;
                }
                *(int *)(data + 0xA8) = 0;
                *(short *)(data + 0xBA) = scale_game_frames(15);
                r = *(int *)(data + 0xC4);
                if (r != -1) {
                    unsigned char *s = (unsigned char *)((char *)&D_0013E550) + r * 0x70;
                    if (*(unsigned char **)(s + 0x88) == moby && s[0x74] != 0) {
                        release_voice_slot(r);
                    }
                }
                *(int *)(data + 0xC4) = -1;
            }
            f20 = convert_integer_to_float(*(int *)path - 1);
            i = func_001FA898_r(f20 * F(data, 0xA4));
            f20 = convert_integer_to_float(*(int *)path - 1);
            t = convert_integer_to_float(i);
            f20 *= F(data, 0xA4);
            f20 -= t;
            if (i == *(int *)path - 1) {
                qcopy((moby + 0x10), path + 0x10 + i * 16);
            } else {
                q = path + 0x10 + i * 16;
                subtract_vector_xyz(tmp, path + 0x20 + i * 16, q);
                scale_vector_xyz(tmp, tmp, f20);
                add_vector_xyz((moby + 0x10), tmp, q);
            }
            F(data, 0xBC) = F(moby, 0x18) - orig[2];
        }
        break;
    }
    subtract_vector_xyz(delta, (moby + 0x10), orig);
    update_entity_matrix(data + 0x60, delta, moby + 0x40, moby + 0x40);
    if (!FUN_L01_00277fb8(moby)) {
        if (2.0f < vector_distance((moby + 0x10), &hero.motion.pos)) {
            if (*(short *)(data + 0xBA) == 0) {
                *(short *)(data + 0xB8) = 1;
            }
        }
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* spawns two bursts of particles around a point */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002C84D8), where it is exact; names translated to the US level program. */

extern char *FUN_L00_0026cbb0(void *, void *, int, int, int, int, float);
extern char D_L01_00167240[];
extern char D_L01_001742E0[];
extern float D_L01_001742C0[];
extern float vector_length_xy(void *) __asm__("FUN_001f9b20");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_L00_00257b90(int, int);
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_L00_0026c6e8(char *, char *);
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

void FUN_L01_002c72c8(void) {
    float v[4];
    float p[4];
    float a, b;
    int i;
    fast_vec_sub(v, D_L01_00167240, D_L01_001742E0);
    a = -fast_arc_tan(vector_length_xy(v), v[2]);
    fast_arc_tan(v[0], v[1]);
    for (i = 19; i >= 0; i--) {
        float x, y;
        x = fast_add_rotations(randf_sym(0.0f, 3.1415927f), a);
        y = fast_add_rotations(randf_sym(0.0f, 3.1415927f), a);
        build_spherical_offset(v, 0.2f, y, x);
        fast_vec_scale(p, v, random_float_between_alt(1.0f, 3.0f));
        fast_vec_add(p, p, D_L01_001742E0);
        FUN_L00_0026c6e8((char *)p, (char *)v);
    }
    for (i = 9; i >= 0; i--) {
        float x, y;
        p[0] = D_L01_001742C0[8] + randf_sym(0.0f, 0.1f);
        p[1] = D_L01_001742C0[9] + randf_sym(0.0f, 0.1f);
        p[2] = D_L01_001742C0[10] + randf_sym(0.0f, 0.1f);
        x = fast_add_rotations(randf_sym(0.0f, 3.1415927f), a);
        y = fast_add_rotations(randf_sym(0.0f, 3.1415927f), a);
        build_spherical_offset(v, 0.05f, y, x);
        FUN_L00_0026cbb0(p, v, 0x4F007FFF, 0x1FFFFFFF,
                         FUN_L00_00257b90(scale_game_frames(10), scale_game_frames(20)), 1, 20000.0f);
    }
}
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern int FUN_001e9448(void *);
extern int collect_mobies_in_sphere(float, void *, int, int, void *) __asm__("FUN_L00_001f2868");
extern void FUN_L00_00259bc8_2c7530(float, void *, void *, int, void *, void *) __asm__("FUN_L00_00259bc8");
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern void FUN_L01_00280970(int, void *);
extern void FUN_0020c828(void *);

extern s32 D_L01_00161494 __attribute__((sda));
extern f32 D_L01_0016149C __attribute__((sda));
void FUN_L01_002c7530(char *moby) {
    int *data = *(int **)(moby + 0x78);
    char *camera = D_L01_00167240;
    char hit[0x30];
    float normal[4];
    float distance;

    distance = vector_distance(camera, moby + 0x10);
    camera -= 0x140;
    *(float *)(moby + 0x44) = -fast_arc_tan(distance, *(float *)(camera + 0x148) - *(float *)(moby + 0x18));
    *(float *)(moby + 0x48) = fast_arc_tan(*(float *)(camera + 0x140) - *(float *)(moby + 0x10),
                                             *(float *)(camera + 0x144) - *(float *)(moby + 0x14));
    *(float *)(moby + 0x40) = fast_add_rotations(*(float *)(moby + 0x40), *(float *)(data + 3));
    fast_vec_add(moby + 0x10, moby + 0x10, data + 8);
    switch ((u8)moby[0x20]) {
    case 1:
        FUN_L01_00280970(D_L01_00161494, moby);
        fast_vec_normalize(normal, data + 8, 1.0f);
        make_hit_descriptor(hit, moby, 0, 0.0f, normal);
        if (collect_mobies_in_sphere(D_L01_0016149C, moby + 0x10, 0x10, data[6], hit) &&
            !FUN_001e9448(moby)) {
            CollisionHit *contact = (CollisionHit *)D_L01_001742C0;
            if (contact->moby != 0)
                FUN_L00_00259bc8_2c7530(1.0f, contact->moby, moby, 0x10001, &contact->point, normal);
            ((void (*)(void *))FUN_L01_002c72c8)(moby);
            goto remove;
        }
        if (fast_dec_timer(data)) {
            data[0] = scale_game_frames(0x1e);
            moby[0x20] = 2;
            if (data[4] != 0)
                data[4] = 0;
        }
        break;
    case 0:
        break;
    case 2:
        if (fast_dec_timer(data)) {
        remove:
            FUN_0020c828(moby);
        } else {
            int duration = scale_game_frames(0x1e);
            moby[0x23] = truncate_float_to_s32((float)data[0] * (127.0f / (float)duration));
        }
        break;
    }
}
extern int D_0015ED84;
extern unsigned char menu_item_enabled_1_lui __asm__("D_0013D395") __attribute__((section(".data")));
/* On level 1, when a state-5 moby of type 0x118 has sub-id 0x34 or 0x35, sets the matching flag byte (D_0013D394 / D_0013D395). */
void FUN_L01_002e0bd8(char *moby)
{
    int level = current_level_index;
    if (level != 1) return;
    if (*(short *)(moby + 0xB2) == 0x34 &&
            *(short *)(moby + 0xA6) == 0x118 &&
            (unsigned char)moby[0x20] == 5) {
            menu_item_enabled_0 = level;
            level = D_0015ED84;
    }
    if (level == 1 && *(short *)(moby + 0xB2) == 0x35 &&
            *(short *)(moby + 0xA6) == 0x118 &&
            (unsigned char)moby[0x20] == 5) {
            menu_item_enabled_1_lui = level;
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L01_002e0c68.s", FUN_L01_002e0c68);
#else
#include "rnc/gameplay/entities/moby.h"
extern void FUN_L00_002eac18(int);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_002eaa30(void *);
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L01_00316e88(float, float);
extern int FUN_L01_0023cf98(int, int);
extern int pad_pressed_within(int, int, int *) __asm__("FUN_L00_00266d60");
extern float approach_value(float, float, float *) __asm__("FUN_00213ed8");
extern float vendor_spring(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern void FUN_L00_0025bc00(float *, void *, float, float);
extern void set_hero_animation(int, int, float) __asm__("FUN_L00_002323b8");
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void FUN_L01_0029b0a0(void *, int, void *, int, void *);
extern void divide_vector_xyz(void *, void *, float) __asm__("FUN_L00_001ff378");
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern void euler_rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_subtract_rotations(float, float) __asm__("FUN_001fa5c8");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern void fast_mem_zero16(void *, int) __asm__("FUN_001f9810");
extern char *D_L01_00167280_lui __asm__("D_L01_00167280") __attribute__((section(".data")));
extern char *D_L01_001600EC;
extern char D_L01_001BAAA0[];
#include "rnc/gameplay/state/game_globals.h"
extern float D_L01_001618A0 __attribute__((sda));
extern unsigned char D_0015EDB5;

/* Six-state interaction: turn around the moby to advance its saved progress. */
void bolt_crank_update(char *moby) __asm__("FUN_L01_002e0c68");
void bolt_crank_update(char *moby) {
    float matrix[4][4];
    float direction[4], movement[4], facing[4], rotation[4];
    float target[4] __attribute__((aligned(16)));
    float offset[4], distance;
    char *data = *(char **)(moby + 0x78);
    char *other;
    float angle, old_angle, height, ground;
    int available;

    if (data == 0) {
        F(moby, 0x48) = fast_add_rotations(F(moby, 0x48), frame_time * 6.2831855f);
        return;
    }
    if (hero.state.current != 0x3b && (unsigned char)moby[0x20] == 3) {
        moby[0x20] = 1;
        if (D_L01_00167280_lui != 0) FUN_L00_002eac18(4);
        return;
    }
    switch ((unsigned char)moby[0x20]) {
    case 0:
        F(data, 0x3c) = F(moby, 0x18);
        moby[0x20] = 1;
        break;
    case 1: {
        unsigned short id;
        struct Hero *g;
        id = *(unsigned short *)(moby + 0xb2);
        if (D_L01_001BB6B0.collected[(short)id] ||
            ((D_0014C190_257218[current_level_index][(short)id >> 5] >> (id & 31)) & 1)) {
            F(moby, 0x18) -= 0.5f;
            moby[0x20] = 5;
            F(data, 0) = 1.0f;
            break;
        }
        g = (struct Hero *)&hero.motion.pos;
        if (vector_distance(moby + 0x10, g) < 1.75f &&
            (g = (struct Hero *)((char *)g - 0x80), g->vehicle_mode) == 0 &&
            fast_difference_between_rotations(g->motion.rot.f[2], fast_arc_tan(F(moby,0x10) - g->motion.pos.f[0], F(moby,0x14) - g->motion.pos.f[1])) < 1.5707964f &&
            absolute_float(g->motion.pos.f[2] - 0.3f - F(moby,0x18)) < 0.2f &&
            g->items[0].item_id == 8 && g->items[0].moby != 0 &&
            pad_pressed_within(0x80, scale_game_frames(7), 0) &&
            (unsigned int)scale_game_frames(30) < *(unsigned int *)(data + 0x14) &&
            F(moby,0x18) == F(data,0x3c) && F(data,0) < 1.0f) {
            *(int *)(data+0x14) = 0;
            available = 1;
            D_L01_001618A0 = 0;
            for (other = D_L01_0015FFE4; other != 0; other = *(char **)(other+0x28)) {
                if ((*(unsigned short *)(other+0x34) & 0x1000) && vector_distance(moby+0x10, other+0x10) < 3.25f) {
                    available = 0;
                    break;
                }
            }
            if (!available) break;
            FUN_L01_0023cf98(0x3b,1);
            jet_mount(D_L01_00167240,D_L01_00167240+0x10,3,scale_game_frames(180),0);
            FUN_L00_002eaa30(D_L01_001600EC + (*(int *)(data+0x20)<<7) + 0x70);
            FUN_L00_002ea9d8(D_L01_001600EC + (*(int *)(data+0x20)<<7) + 0x30);
            FUN_L01_00316e88(F(data,0x28),F(data,0x2c));
            moby[0x20] = 3;
            *(int *)(data+0x24) = 0;
            F(data,0x18) = F(data,0) * (F(data,0x30) * 6.2831855f);
            break;
        }
        if (F(data,0) < 1.0f) {
            ++*(int *)(data+0x14);
            approach_value(0.0f, 1.0f / (float)scale_game_frames(60), (float *)data);
            if (0.0f < F(data,0)) {
                F(moby,0x48) = fast_subtract_rotations(F(moby,0x48), (F(data,0x30) * 6.2831855f) / (float)scale_game_frames(60));
            }
            F(moby,0x18) += frame_time * 0.5f;
            if (F(data,0x3c) <= F(moby,0x18)) F(moby,0x18) = F(data,0x3c);
        } else {
            if (*(int *)(data+0x38) == 0) break;
            if (*(int *)(data+0x34) != -1 && visited52[(unsigned char)moby[0xb0] + (current_level_index << 4)] != 255) {
                FUN_L01_0029b0a0(data,4,moby,2,D_L01_001BAAA0);
                jet_clear_persistent((unsigned char)moby[0xb0]);
                if (*(int *)(data+0x34) != -1) {
                    jet_reset_camera(D_L01_001600EC + (*(int *)(data+0x34)<<7) + 0x30,
                                      D_L01_001600EC + (*(int *)(data+0x34)<<7) + 0x70);
                }
            }
            F(moby,0x18) -= frame_time * 0.5f;
            if (F(moby,0x18) <= F(data,0x3c)-0.5f) F(moby,0x18) = F(data,0x3c)-0.5f;
        }
        break;
    }
    case 3: {
        *(u128 *)target = 0;
        target[2] = F(D_L01_00167240,0x18);
        *(u128 *)rotation = *(u128 *)target;
        old_angle = fast_arc_tan(hero.motion.pos.f[0]-F(moby,0x10),hero.motion.pos.f[1]-F(moby,0x14));
        if ((pad_pressed_within(0x80,scale_game_frames(7),0) && scale_game_frames(60) < hero.state_timer) || F(data,0) == 1.0f) {
            FUN_L01_0023cf98(0,1);
            if (*(int *)(data+0x38)) {
                moby[0x20] = 1;
            } else if (F(data,0) == 1.0f) {
                unsigned short id;
                FUN_L01_0029b0a0(data,4,moby,2,D_L01_001BAAA0);
                moby[0x20] = 4;
                id = *(unsigned short *)(moby+0xb2);
                D_0014C190_257218[current_level_index][(short)id >> 5] |= 1 << (id & 31);
                id = *(unsigned short *)(moby+0xb2);
                moby_killed[(short)id >> 5] |= 1 << (id & 31);
                allocate_voice_for_target_entry(0,0,moby);
                if (*(int *)(data+0x34) != -1 && visited52[(unsigned char)moby[0xb0] + (current_level_index << 4)] != 255) {
                    FUN_L01_0029b0a0(data,4,moby,2,D_L01_001BAAA0);
                    jet_clear_persistent((unsigned char)moby[0xb0]);
                    if (*(int *)(data+0x34) != -1) {
                        jet_reset_camera(D_L01_001600EC + (*(int *)(data+0x34)<<7) + 0x30,
                                          D_L01_001600EC + (*(int *)(data+0x34)<<7) + 0x70);
                    }
                }
            }
            FUN_L00_002eac18(4);
            return;
        }
        direction[1] = -hero.stick_x;
        direction[0] = -hero.stick_y;
        { float length = vector_length_xy(direction);
          if (1.0f < length) divide_vector_xyz(direction,direction,length); }
        euler_rotation_matrix(matrix,rotation);
        transform_vector(direction,direction,matrix);
        facing[0] = fast_cos(hero.moby->rot.z);
        facing[1] = fast_sin(hero.moby->rot.z);
        facing[2] = 0;
        {
            float turn, speed;
            {
                float limit;
                turn = fast_vec_dot(facing,direction) - F(data,0x1c);
                limit = frame_time_sq * 5.5f;
                if (limit < turn) turn = limit;
                if (turn < -limit) turn = -limit;
            }
            speed = F(data,0x1c) + turn;
            {
                float limit = frame_time * 3.7f;
                F(data,0x1c) = speed;
                if (limit < speed) F(data,0x1c) = limit;
            }
            if (F(data,0x1c) < 0.0f) F(data,0x1c) = 0.0f;
            if (absolute_float(F(data,0x1c)) < (frame_time_sq * 5.5f) * 0.25f) F(data,0x1c) = 0.0f;
        }
        fast_vec_normalize(movement,facing,F(data,0x1c));
        if ((hero.unkA98 & 2) && hero.moby->prev_seq == 0x44) set_hero_animation(0x3f,0,(float)scale_game_frames(7));
        if (hero.moby->prev_seq != 0x44 && hero.unkA9C == 0) {
            if (hero.moby->seq == 0x40 && absolute_float(F(data,0x1c)) == 0.0f) {
                set_hero_animation(0x3f,0,(float)scale_game_frames(14));
            } else if (hero.moby->seq == 0x3f) {
                if (0.0f < F(data,0x1c)) set_hero_animation(0x40,0,(float)scale_game_frames(14));
            } else if (hero.moby->seq == 0x40 && ((frame_time * 3.7f) * 0.6f) < F(data,0x1c)) {
                set_hero_animation(0x41,0,(float)scale_game_frames(14));
            } else if (hero.moby->seq == 0x41 && F(data,0x1c) < ((frame_time * 3.7f) * 0.4f)) {
                set_hero_animation(0x40,0,(float)scale_game_frames(16));
            }
        }
        height = hero.motion.pos.f[2];
        add_vector_xyz(&hero.motion.pos,&hero.motion.pos,movement);
        angle = fast_arc_tan(hero.motion.pos.f[0]-F(moby,0x10),hero.motion.pos.f[1]-F(moby,0x14));
        movement[0] = fast_cos(angle) * 1.05f;
        movement[1] = fast_sin(angle) * 1.05f;
        movement[2] = 0;
        add_vector_xyz(target,moby+0x10,movement);
        subtract_vector_xyz(offset,target,&hero.motion.pos);
        offset[2] = 0;
        distance = -vector_length_xy(offset);
        vendor_spring(&distance,&D_L01_001618A0,0.0f,frame_scale_sq*0.015f,frame_scale_sq*0.3f,frame_time*7.0f);
        fast_vec_normalize(offset,offset,D_L01_001618A0);
        add_vector_xyz(&hero.motion.pos,&hero.motion.pos,offset);
        hero.motion.pos.f[2] = height;
        {
            char *g;
            *(volatile float *)&hero.motion.move_yaw = fast_add_rotations(D_0015EDB5 ? -1.5707964f : 1.5707964f,angle);
            /* Existing data-label anchor reconstructs hero yaw at 0x0013f3e8. */
            other = D_0013E533 + 0xeb5;
            g = other - 0x98;
            FUN_L00_0025bc00((float *)other,0,F(g,0x180),*(volatile float *)&frame_time*6.2831855f);
            /* Keep retail's two separate height subtractions around the ground probe. */
            F(data,0x24) = frame_time_sq*9.8f + F(data,0x24);
            F(g,0x88) -= F(data,0x24)-2.0f;
            ground = probe_ground_height(other-0x18,0,0.5f);
            F(g,0x88) -= 2.0f;
            if (F(g,0x88) < ground) {
                F(g,0x88) = ground;
                *(int *)(data+0x24) = 0;
            }
            if (((struct Hero *)g)->moby->prev_seq != 0x44) {
                float turn;
                turn = fast_add_rotations(0.2617994f,angle);
                FUN_L00_0025bc00((float *)(moby+0x48),0,turn,frame_time*3.1415927f);
                if (1.0471976f < fast_subtract_rotations(F(moby,0x48),turn)) {
                    F(moby,0x48) = fast_subtract_rotations(F(moby,0x48),1.0471976f);
                } else if (fast_subtract_rotations(F(moby,0x48),turn) < -1.0471976f) {
                    F(moby,0x48) = fast_add_rotations(F(moby,0x48),1.0471976f);
                }
            }
            F(data,0x18) += fast_difference_between_rotations(angle,old_angle);
            F(data,0) = F(data,0x18) / (F(data,0x30)*6.2831855f);
            if (1.0f < F(data,0)) F(data,0) = 1.0f;
            if (F(data,0) < 0.0f) F(data,0) = 0.0f;
            fast_mem_zero16(&hero.motion.velocity,0x90);
        }
        break;
    }
    case 4:
        F(moby,0x18) -= frame_time*0.5f;
        if (F(moby,0x18) <= F(data,0x3c)-0.5f) {
            F(moby,0x18) = F(data,0x3c)-0.5f;
            moby[0x20] = 5;
            F(data,0) = 1.0f;
        }
        break;
    case 5:
        F(data,0) = 1.0f;
        break;
    }
    FUN_L01_002e0bd8(moby);
}
#endif
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"

/* Bought-item price entry (0x18 bytes). */
typedef struct {
    u8 pad_00[0x14];
    u16 bolts; /* 0x14 */
    u8 pad_16[2];
} ItemPrice_2e1ac0;

/* pvars of the moby run by gold_weapon_case_update: a scene trigger
   (update_npc_dialogue) followed by the item it sells. */
typedef struct {
    u8 pad_00[4];
    s16 result;   /* 0x04: 2 once the scene finished with a sale */
    u8 pad_06[6];
    f32 radius;   /* 0x0C */
    u8 pad_10[0x30];
    s32 slot;     /* 0x40: -1 none, else index into D_L01_001DEB10 */
} SaleVars_2e1ac0;

extern s32 cutscene_mode __asm__("D_L01_0015F5C4");
extern u8 D_0013E520_2e1ac0[] __asm__("D_0013E520");
extern void FUN_0020c828_2e1ac0(struct Moby *) __asm__("FUN_0020c828");
extern s32 FUN_L00_002668a0_2e1ac0(struct Moby *, SaleVars_2e1ac0 *) __asm__("FUN_L00_002668a0");
extern s32 FUN_L00_00266448_2e1ac0(struct Moby *, SaleVars_2e1ac0 *) __asm__("FUN_L00_00266448");
extern void FUN_L01_002aea70_2e1ac0(struct Moby *, s32, ItemPrice_2e1ac0 *) __asm__("FUN_L01_002aea70");
extern s32 FUN_0020b178_2e1ac0(s32, s32) __asm__("FUN_0020b178");

extern s32 D_L01_001DEB10[];
extern ItemPrice_2e1ac0 D_L01_001C4530[];
extern s32 alternate_animation_sequence_gp __asm__("D_0015EE20") __attribute__((sda));
void gold_weapon_case_update(struct Moby *m) __asm__("FUN_L01_002e1ac0");
void gold_weapon_case_update(struct Moby *m) {
    SaleVars_2e1ac0 *v = (SaleVars_2e1ac0 *)m->pvars;
    s32 item;
    ItemPrice_2e1ac0 *price;

    if (v != 0 && v->slot != -1 && ((v->slot ^ 1) & 1) && alternate_animation_sequence_gp == 0) {
        FUN_0020c828_2e1ac0(m);
        return;
    }
    switch (m->state) {
    case 0:
        if (m->oclass != 0x130) {
            m->state = 2;
        } else if (D_0013E520_2e1ac0[D_L01_001DEB10[v->slot]] != 0 ||
                   FUN_L00_002668a0_2e1ac0(m, v) == -1) {
            m->state = 3;
        } else {
            m->state = 1;
        }
        break;
    case 1:
        v->radius = 1.9f;
        FUN_L00_00266448_2e1ac0(m, v);
        if (v->result == 2) {
            D_0013E520_2e1ac0[D_L01_001DEB10[v->slot]] = 1;
            m->state = 4;
            item = D_L01_001DEB10[v->slot];
            price = &D_L01_001C4530[item];
            current_bolt_count -= price->bolts;
            FUN_L01_002aea70_2e1ac0(m, item, price);
        }
        break;
    case 3: /* sold out or done: idle */
        break;
    case 4:
        if (cutscene_mode == 0) {
            FUN_0020b178_2e1ac0(0, -1);
            m->state = 3;
        }
        break;
    case 2:
        if (D_0013E520_2e1ac0[D_L01_001DEB10[v->slot]] != 0) {
            FUN_0020c828_2e1ac0(m);
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4430), where it is exact; names translated to the US level program. */

extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void FUN_L01_002e30b8(int unused, void *out) {
    char scratch[16];
    char *base = D_0013E533 + 0xE1D;
    fast_vec_normalize(scratch, *(char **)(base + 0x2080) + 0xD0, 1.0f);
    fast_vec_add(out, scratch, base + 0x80);
}
#include "sda.h"

/* Builds a camera-relative transform for a moby and steps its height. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4488), where it is exact; names translated to the US level program. */

typedef struct {
    float a[4];
} Vs __attribute__((aligned(16)));

extern int FUN_L00_002583f0(Vs *p, int x, int y, float f, float g);
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");

extern float D_L01_001DEB40[];
void FUN_L01_002e3110(char *a, char *m) {
    Vs t0;
    Vs t1;
    struct Hero *g = &hero;
    normalize_vector_xyz(&t0, ((char *)g->moby) + 0xD0, 1.1f);
    normalize_vector_xyz(&t1, ((char *)g->moby) + 0xE0, 1.0f);
    add_vector_xyz(m, &t0, &t1);
    add_vector_xyz(m, m, &g->motion.pos);
    if (*(int *)(m + 0x68) == 0) {
        if (FUN_L00_002583f0((Vs *)m, (int)a, 0, D_L01_001DEB40[0], 2.0f)) {
            *(float *)(m + 8) = D_L01_001742E8;
        }
    } else {
        *(float *)(m + 8) = *(float *)(m + 8) + 1.0f;
    }
}
struct DriftEmitter {
    s32 count;          /* 0x00 */
    u8 pad4[0x2C];
    f32 spread;         /* 0x30 */
};
extern struct DriftEmitter D_L01_001DEB38;
extern s32 FUN_L00_00258ad0(void *, void *, s32, s32, f32, f32);

/* shrinks and sinks the moby, then hands it to the drift emitter */
s32 FUN_L01_002e3208(void *ctx, struct Moby *m) {
    f32 shrink;

    m->pos.z -= frame_time_sq * 29.7f;
    shrink = frame_scale * -0.050000012f + 1.0f;
    m->pos.x *= shrink;
    m->pos.y *= shrink;
    return FUN_L00_00258ad0(ctx, &m->pos, D_L01_001DEB38.count, 0, 0.3f, D_L01_001DEB38.spread);
}
/* Picks the best lock-on target for moby from the visible-moby list: a living, drawn class-5 enemy within range whose direction from `from`
 * is inside the yaw/pitch cone (or the wider near cone when close), scored by angle error and distance,
 * with a clear line of sight (or the blocker is the same kind of enemy). */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002E4620), where it is exact; names translated to the US level program. */

extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern float square_root(float) __asm__("FUN_001f9988");
extern float fast_difference_between_rotations_c(float, float) __asm__("func_001FA688");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern char D_L01_00174340_h[] __asm__("D_L01_001742C0");
extern float *func_L00_0025D390_t(void *) __asm__("FUN_002141f8");
extern int func_L00_001EFFF0_t(void *, void *, int, void *, void *) __asm__("FUN_001efa68");

void *FUN_L01_002e32a8(void *moby, void *from, void *ang, float yawMax, float pitchMax, float range, float near,
                        float nearYaw, float nearPitch) {
    float tp[4];
    char *best = 0;
    float bestScore = 1.0e9f;
    char *t;
    for (t = D_L01_0015FFE4; t != 0; t = *(char **)(t + 0x28)) {
        char *hs = D_L01_00174340_h;
        struct Hero *pl = &hero;
        float *hp;
        int cls;
        float dist, score, p2;
        if (*(short *)(t + 0x32) == 0) continue;
        hp = func_L00_0025D390_t(t);
        if (!(*(unsigned short *)(t + 0x34) & 0x1000) || t == 0 || *(char **)(t + 0x24) == 0) continue;
        cls = *(short *)(*(char **)(t + 0x24) + 0x46);
        if (cls != 5 || hp == 0 || !(0.0f < *hp)) continue;
        dist = distance_xyz(from, t + 0x10);
        if (!(dist < range)) continue;
        qcopy(tp, t + 0x10);
        tp[2] += 0.4f;
        score = fast_difference_between_rotations_c(((float *)ang)[2], fast_arc_tan(tp[0] - ((float *)from)[0], tp[1] - ((float *)from)[1]));
        score = score * score;
        if (!(score < yawMax * yawMax)) {
            if (!(dist < near) || !(score < nearYaw * nearYaw)) continue;
        }
        p2 = fast_difference_between_rotations_c(((float *)ang)[1], fast_arc_tan(dist, tp[2] - ((float *)from)[2]));
        p2 = p2 * p2;
        if (!(p2 < pitchMax * pitchMax)) {
            if (!(dist < near) || !(p2 < nearPitch * nearPitch)) continue;
        }
        score *= p2;
        score *= square_root(distance_xyz(from, tp));
        if (!(score < bestScore)) continue;
        if (func_L00_001EFFF0_t(from, tp, 0, moby, 0)) {
            char *hit = *(char **)(hs + 0x18);
            if (hit == 0 || hit == ((char *)pl->moby) || hit == 0 || *(char **)(hit + 0x24) == 0
                || *(short *)(*(char **)(hit + 0x24) + 0x46) != cls) {
                continue;
            }
        }
        bestScore = score;
        best = t;
    }
    return best;
}
#include "qcopy.h"
#include "qzero.h"
#include "rnc/overlay/quad.h"


extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void *suck_get_aim_height(void *) __asm__("FUN_002141f8");
extern void *FUN_L01_002e32a8_u(void *, void *, void *, float, float, float, float, float, float) __asm__("FUN_L01_002e32a8");
extern void FUN_L01_0030c898(void *, void *, float, float, float, float);

extern float D_L01_00161904 __attribute__((sda));
extern float D_L01_00161908 __attribute__((sda));
extern float D_L01_0016190C __attribute__((sda));
extern float D_L01_00161910 __attribute__((sda));
extern float D_L01_00161914 __attribute__((sda));
extern int D_L01_00161900 __attribute__((sda));
extern int cutscene_mode __asm__("D_L01_0015F5C4");

typedef union {
    OvlQuad q;
    float f[4];
} V4_w22c;

void FUN_L01_002e35a8(char *moby, char *state) {
    V4_w22c v;
    V4_w22c q;
    V4_w22c p;
    char *e;
    float a, b, c, d, t, dist;

    if (fast_dec_timer_s16(state + 0x4E) == 0)
        return;
    qzero(&v);
    fast_vec_add(&v, &v, moby + 0x10);
    v.f[2] = v.f[2] + D_L01_00161914;
    e = FUN_L01_002e32a8_u(moby, &v, moby + 0x40, D_L01_00161908 * DEG_TO_RAD,
                         D_L01_0016190C * DEG_TO_RAD, D_L01_00161904, 2.0f, 3.1415927f,
                         3.1415927f);
    if (e != 0) {
        *(char **)(state + 0x68) = e;
        if (hero.motion.pos.f[2] + 1.1f < *(float *)(moby + 0x18)) {
            t = fast_arc_tan(*(float *)(e + 0x10) - *(float *)(moby + 0x10),
                             *(float *)(e + 0x14) - *(float *)(moby + 0x14));
            if (fast_difference_between_rotations(*(float *)(moby + 0x48), t) < 0.5235988f) {
                if (cutscene_mode != 2) {
                    char *r;
                    qcopy(&q, e + 0x10);
                    r = suck_get_aim_height(e);
                    if (r != 0)
                        q.f[2] = q.f[2] + *(float *)(r + 0x10);
                    else
                        q.f[2] = q.f[2] + D_L01_00161914;
                    fast_vec_sub(&p, &q, &v);
                    fast_vec_normalize(&p, &p, D_L01_00161910);
                    fast_vec_add(&v, &v, &p);
                    a = fast_arc_tan(q.f[0] - v.f[0], q.f[1] - v.f[1]);
                    dist = vector_distance(&v, &q);
                    b = -fast_arc_tan(dist, q.f[2] - v.f[2]);
                    c = fast_arc_tan(q.f[0] - v.f[0], q.f[1] - v.f[1]);
                    dist = vector_distance(&v, &q);
                    d = -fast_arc_tan(dist, q.f[2] - v.f[2]);
                    FUN_L01_0030c898(moby, &v, a, b, c, d);
                    *(short *)(state + 0x4E) = scale_game_frames(D_L01_00161900);
                    *(int *)(state + 0x64) = *(int *)(state + 0x64) + 1;
                }
            }
        }
    } else {
        *(int *)(state + 0x68) = 0;
    }
}
extern f32 D_L01_00161944 __attribute__((sda));
extern f32 D_L01_00161948 __attribute__((sda));
extern f32 D_L01_0016194C __attribute__((sda));
extern f32 D_L01_00161920 __attribute__((sda));
extern void FUN_L00_00258278(struct Moby *, f32, f32 *, f32, f32, f32);
extern f32 vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern f32 fast_cos(f32) __asm__("FUN_001f9dc8");
extern f32 fast_sin(f32) __asm__("FUN_001f9de0");
extern void clamp_vector_length(void *, void *, f32) __asm__("FUN_001f9c90");
extern f32 probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern void blend_moby_animation_3830(void *, int, int, int) __asm__("FUN_00212f90");
extern int random_integer_3830(void) __asm__("FUN_001160d8");

void FUN_L01_002e3830(struct Moby *m, char *p) {
    Vec4 v;
    f32 d, ang, len, ground, near;
    s32 hit;
    s16 n;
    struct Moby *target;

    subtract_vector_xyz(&v, p, &m->pos);
    if (m->state == 6)
        d = vector_distance(&m->pos, p + 0x30);
    else
        d = vector_distance(&m->pos, &hero.motion.pos);
    vector_distance(&m->pos, p);

    switch (m->command) {
    case 0:
        ang = fast_arc_tan(*(f32 *)(p + 0) - m->pos.x, *(f32 *)(p + 4) - m->pos.y);
        FUN_L00_00258278(m, ang, &D_L01_001DEB4C, frame_time_sq * 11.519173f,
                         frame_time_sq * 3.1415927f, frame_time * 11.519173f);
        if (m->anim_flags & 2) {
            m->anim_speed = 1.0f;
            if (m->prev_seq != random_integer_3830() % 3) {
                int seq = random_integer_3830() % 3;
                blend_moby_animation_3830(m, seq, 0, scale_game_frames(20));
            }
        }
        if (1.3f < d) {
            if (m->prev_seq != 5)
                blend_moby_animation_3830(m, 5, 0, scale_game_frames(10));
            m->command = 2;
        }
        return;
    case 2:
    case 3:
        break;
    default:
        goto inactive;
    }

    target = *(struct Moby **)(p + 0x68);
    if (target == 0) {
        ang = fast_arc_tan(*(f32 *)(p + 0) - m->pos.x, *(f32 *)(p + 4) - m->pos.y);
        FUN_L00_00258278(m, ang, &D_L01_001DEB4C, frame_time_sq * 11.519173f,
                         frame_time_sq * 3.1415927f, frame_time * 11.519173f);
    } else {
        ang = fast_arc_tan(target->pos.x - m->pos.x, target->pos.y - m->pos.y);
        FUN_L00_00258278(m, ang, &D_L01_001DEB4C,
                         D_L01_00161944 * DEG_TO_RAD * frame_time_sq,
                         D_L01_00161948 * DEG_TO_RAD * frame_time_sq,
                         D_L01_0016194C * DEG_TO_RAD * frame_time);
    }
    scale_vector_xyz(&v, &v, frame_scale * 0.04f);
    subtract_vector_xyz(&v, &v, ((f32 *)(p + 0x10)));
    if (1.3f < d) {
        scale_vector_xyz(&v, &v, frame_scale_sq * 0.13f);
    } else {
        fast_vec_scale(&v, &v, frame_scale_sq * 0.25f);
    }
    v.f[2] *= frame_scale * -0.100000024f + 1.0f;
    add_vector_xyz(((f32 *)(p + 0x10)), ((f32 *)(p + 0x10)), &v);
    if (m->state == 6)
        clamp_vector_length(((f32 *)(p + 0x10)), ((f32 *)(p + 0x10)), frame_time * 2.7f);
    hit = FUN_L00_00258ad0(m, ((f32 *)(p + 0x10)), D_L01_001DEB38.count, 0,
                            1.0f, D_L01_001DEB38.spread);
    len = fast_vec_length(((f32 *)(p + 0x10)));
    if (hit != 0) {
        if (len < 0.005f) {
            n = ++*(s16 *)(p + 0x4a);
            if (scale_game_frames(20) < n && m->visible == 0) {
                qcopy(&m->pos, p);
                *(s16 *)(p + 0x4a) = 0;
            }
        }
        ground = probe_ground_height(&m->pos, 0, 0.5f);
        if ((hit & 2) || m->pos.z - ground < D_L01_00161920) {
            if (len < 0.005f) {
                if (m->prev_seq >= 3 && m->prev_seq != random_integer_3830() % 3) {
                    int seq = random_integer_3830() % 3;
                    blend_moby_animation_3830(m, seq, 0, scale_game_frames(10));
                }
            } else {
                goto animation3;
            }
        } else {
            if (m->prev_seq != 5)
                blend_moby_animation_3830(m, 5, 0, scale_game_frames(7));
        }
    } else {
        ground = probe_ground_height(&m->pos, 0, 0.5f);
        if (m->pos.z - ground < D_L01_00161920) {
 animation3:
            if (m->prev_seq != 3)
                blend_moby_animation_3830(m, 3, 0, scale_game_frames(11));
        } else {
            if (m->prev_seq != 5)
                blend_moby_animation_3830(m, 5, 0, scale_game_frames(7));
            *(s16 *)(p + 0x4a) = 0;
        }
    }
collision_done:
    if (m->state == 6 || !(d < 1.1f)) return;
    near = 1.1f - d;
    ang = fast_arc_tan(hero.motion.pos.f[0] - m->pos.x, hero.motion.pos.f[1] - m->pos.y);
    ((f32 *)(p + 0x10))[0] -= near * fast_cos(ang) * (frame_scale_sq * 0.1f);
    ((f32 *)(p + 0x10))[1] -= near * fast_sin(ang) * (frame_scale_sq * 0.1f);
    return;
inactive:
    m->command = 0;
    if (1.3f < d) m->command = 2;
}
/* State for the parented moby.  The parent index is -1 when unattached. */
typedef int u128_2e3f20 __attribute__((mode(TI)));
typedef struct {
    float target[4], vel[4], start[4], home[4];
    int vol;
    char pad44[4];
    unsigned char b48;
    char pad49;
    short s4A, timer, s4E;
    int parent;
    char pad54[0xC];
    int t60, count;
    void *tgt;
} Data_2e3f20;
typedef struct {
    int i0;
    float f4, f8, fC, f10, f14, f18, f1C, f20, f24, f28, f2C, f30;
    int i34, i38;
    float f3C, f40, f44;
    int i48, i4C;
    void *p50;
} Mover_2e3f20;
typedef struct {
    char pad0[0x80];
    float pos[4];
    char pad90[0x10A0];
    struct Moby *helper;
    char pad1134[0x24];
    int i1158;
    char pad115C[0xF24];
    void *p2080;
    char pad2084[8];
    int i208C;
    char pad2090[0x14];
    unsigned char b20A4;
} Hero_2e3f20;
#define hero_2e3f20 ((Hero_2e3f20 *)&hero)
#define mover_2e3f20 (*(Mover_2e3f20 *)&D_L01_001DEB38)
extern struct Moby *ents_2e3f20 __asm__("D_L01_0015FFD8");
extern int D_0015ED84;
extern int D_L01_00161918 __attribute__((sda));
extern int D_L01_0016191C __attribute__((sda));
extern int D_L01_00161924 __attribute__((sda));
extern int D_L01_00161928 __attribute__((sda));
extern int D_L01_0016192C __attribute__((sda));
extern float D_L01_00161930 __attribute__((sda));
extern float D_L01_00161934 __attribute__((sda));
extern float D_L01_00161938 __attribute__((sda));
extern int D_L01_0016193C __attribute__((sda));
extern float D_L01_00161940 __attribute__((sda));
extern char D_L01_0020AE80[];
extern char D_L01_0020AEA0[];
extern void debug_print_2e3f20(char *) __asm__("FUN_001e93b0");
extern void initialize_mover_2e3f20(void *) __asm__("FUN_L01_0026d930");
extern void rig_probe_setup(void *, int, int *, float *, float *, int *) __asm__("FUN_L01_00263a28");
extern void face_target_2e3f20(struct Moby *, float, float *, float, float, float) __asm__("FUN_L00_00258278");
extern int FUN_L01_0023cf98(int, int);
extern void play_sound_2e3f20(int, int, float) __asm__("FUN_L00_002323b8");
extern int step_mover_2e3f20(void *, void *, float *, float *) __asm__("FUN_L00_00259028");
extern void FUN_L00_0025f8e0(void *, float);
extern void spawn_particle_2e3f20(float *, int, int, int, int, float *, float, float, float) __asm__("FUN_L00_00272060");
extern void finish_moby_2e3f20(void *, float) __asm__("FUN_L00_00259fe8");
extern int random_integer_3830(void) __asm__("FUN_001160d8");
extern void blend_moby_animation_3830(void *, int, int, int) __asm__("FUN_00212f90");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int FUN_L00_00257b90(int, int);
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern void FUN_L01_002e30b8(int, void *);
extern void FUN_L01_002e3110(char *, char *);
extern s32 FUN_L01_002e3208(void *, struct Moby *);
extern void FUN_L01_002e35a8(char *, char *);
extern void FUN_L01_002e3830(struct Moby *, char *);
extern unsigned char D_0013D4C5 __attribute__((section(".data")));
void sand_mouse_update(struct Moby *moby) __asm__("FUN_L01_002e3f20");
void sand_mouse_update(struct Moby *moby) {
    Data_2e3f20 *d;
    float va[4];
    float vb[4];
    float ang;

    if (moby == 0) {
        return;
    }
    d = *(Data_2e3f20 **)&moby->pvars;
    if (d == 0) {
        return;
    }
    if (d->parent != -1) {
        if (moby->state >= 5) {
            ((Data_2e3f20 *)ents_2e3f20[d->parent].pvars)->target[0] =
                fast_arc_tan(moby->pos.x - ents_2e3f20[d->parent].pos.x,
                                  moby->pos.y - ents_2e3f20[d->parent].pos.y);
        } else {
            ((Data_2e3f20 *)ents_2e3f20[d->parent].pvars)->target[0] =
                fast_arc_tan(hero_2e3f20->pos[0] - ents_2e3f20[d->parent].pos.x,
                                  hero_2e3f20->pos[1] - ents_2e3f20[d->parent].pos.y);
        }
        if (d->parent != -1) {
            *(int *)&ents_2e3f20[d->parent].collision = 0;
        }
    }
    switch (moby->state) {
    case 0:
        qzero(d->target);
        qzero(d->vel);
        d->s4A = 0;
        d->timer = 0;
        if (moby->prev_seq != 0) {
            blend_moby_animation_3830(moby, 0, 0, scale_game_frames(10));
        }
        moby->update_dist = 0xFF;
        moby->draw_dist = 0xFF;
        if (d->parent != -1) {
            unsigned short id;
            qcopy(((float *)&moby->pos), ((float *)&ents_2e3f20[d->parent].pos));
            moby->pos.z = moby->pos.z + 0.02f;
            if (D_L01_001BB6B0.collected[(short)(id = moby->save_id)] != 0
                || (*(int *)((char *)D_0014C190_257218 + (((short)id >> 5) * 4 + (D_0015ED84 << 8))) >> (id & 0x1F)) & 1) {
                debug_print_2e3f20(D_L01_0020AE80);
                mark_moby_for_removal(moby);
                return;
            }
        } else {
            debug_print_2e3f20(D_L01_0020AEA0);
            mark_moby_for_removal(moby);
            return;
        }
        qcopy(d->start, ((float *)&moby->pos));
        qcopy(d->home, ((float *)&moby->pos));
        initialize_mover_2e3f20((char *)&mover_2e3f20);
        {
            int kind;
            float len;
            int flag;
            rig_probe_setup(moby, 0, &kind, va, &len, &flag);
            mover_2e3f20.f14 = frame_scale * 0.4f;
            mover_2e3f20.f8 = 0.15f;
            mover_2e3f20.fC = 0.15f;
            mover_2e3f20.f10 = frame_scale * 0.01f;
            mover_2e3f20.f24 = frame_time * 2.7f;
            mover_2e3f20.f1C = frame_scale * 0.01f;
            mover_2e3f20.i0 = truncate_float_to_s32(len * 1024.0f) + 1;
            mover_2e3f20.i38 |= 8;
            mover_2e3f20.f4 = 0.3f;
            mover_2e3f20.f30 = 0.005f;
            mover_2e3f20.f3C = frame_time_sq * 11.519173f;
            mover_2e3f20.f40 = frame_time_sq * 3.1415927f;
            mover_2e3f20.f44 = frame_time * 11.519173f;
            mover_2e3f20.i48 = scale_game_frames(4);
            mover_2e3f20.f28 = 3.1415927f;
        }
        moby->state = 1;
        moby->draw_dist = 0;
        break;
    case 1:
        if (D_0013D4C5 != 0 && hero_2e3f20->i1158 == 5) {
            d->count = 0;
            d->t60 = scale_game_frames(D_L01_00161918);
            if (moby->visible != 0 && (moby->anim_flags & 2)) {
                if (moby->prev_seq != random_integer_3830() % 3) {
                    blend_moby_animation_3830(moby, random_integer_3830() % 3, 0, scale_game_frames(10));
                }
            }
            if (d->parent != -1) {
                if (is_point_inside_clip_volume(hero_2e3f20->pos, d->vol)) {
                    ents_2e3f20[d->parent].command = 3;
                } else {
                    ents_2e3f20[d->parent].command = 1;
                }
            }
            if (hero_2e3f20->helper != 0 && hero_2e3f20->i1158 == 5) {
                ang = fast_arc_tan(hero_2e3f20->pos[0] - moby->pos.x,
                                        hero_2e3f20->pos[1] - moby->pos.y);
                face_target_2e3f20(moby, ang, &mover_2e3f20.f14,
                                        frame_time_sq * 11.519173f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
                qcopy(va, ((float *)&ents_2e3f20[d->parent].pos));
                va[2] = va[2] + 0.5f;
                qcopy(vb, hero_2e3f20->pos);
                vb[2] = vb[2] + 0.2f;
                if (is_point_inside_clip_volume(hero_2e3f20->pos, d->vol)) {
                    if (!collision_line(va, vb, 0, moby, 0)
                        || D_L01_001742D8 == hero_2e3f20->p2080) {
                        FUN_L01_002e30b8((int)moby, d);
                        if (hero_2e3f20->helper != 0
                            && hero_2e3f20->helper->oclass == 0x1B1
                            && hero_2e3f20->helper->state != 0xFE
                            && hero_2e3f20->helper->state != 0xFD) {
                            hero_2e3f20->helper->state = 1;
                        }
                        *(void **)((char *)&mover_2e3f20 + 0x50) = moby;
                        FUN_L01_0023cf98(0x1F, 0);
                        play_sound_2e3f20(0, 0, (float)scale_game_frames(6));
                        if (d->parent != -1) {
                            ents_2e3f20[d->parent].command = 5;
                            moby->state = 2;
                        } else {
                            moby->draw_dist = 0xFF;
                            moby->state = 3;
                        }
                    }
                }
            }
        }
        break;
    case 2:
        if (d->parent != -1) {
            if (ents_2e3f20[d->parent].state == 5) {
                moby->state = 3;
                d->timer = 0;
                moby->draw_dist = 0xFF;
            }
        } else {
            d->timer = 0;
            moby->draw_dist = 0xFF;
            moby->state = 3;
        }
        break;
    case 3:
        FUN_L01_002e30b8((int)moby, d);
        ang = fast_arc_tan(d->target[0] - moby->pos.x, d->target[1] - moby->pos.y);
        face_target_2e3f20(moby, ang, &mover_2e3f20.f14,
                                frame_time_sq * 11.519173f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
        if ((step_mover_2e3f20(moby, &mover_2e3f20, d->target, d->vel) & 6)
            || vector_distance(((float *)&moby->pos), hero_2e3f20->pos) < 1.0f) {
            qzero(d->vel);
            moby->state = 4;
            d->timer = scale_game_frames(0x78);
        }
        if (vector_distance(((float *)&moby->pos), d->home) > 0.4f && d->timer == 0) {
            *(float *)((char *)&mover_2e3f20 + 4) = 0.7f;
            if (moby->prev_seq != 3) {
                blend_moby_animation_3830(moby, 3, 0, scale_game_frames(3));
            }
        }
        break;
    case 4:
        FUN_L01_0023cf98(0, 1);
        moby->flags &= 0xFFBE;
        if (d->parent != -1) {
            ents_2e3f20[d->parent].flags &= 0xFFBE;
        }
        moby->state = 5;
        if (hero_2e3f20->helper != 0
            && hero_2e3f20->helper->oclass == 0x1B1
            && hero_2e3f20->helper->state != 0xFE
            && hero_2e3f20->helper->state != 0xFD) {
            d->b48 = 0xFF;
            hero_2e3f20->helper->state = 2;
        }
        break;
    case 5:
        if (hero_2e3f20->helper != 0
            && hero_2e3f20->i1158 == 5
            && hero_2e3f20->i208C != 0x16
            && hero_2e3f20->i208C != 0x14
            && hero_2e3f20->b20A4 == 0
            && !fast_dec_timer(&d->t60)
            && d->count < D_L01_0016191C) {
            FUN_L01_002e3110((char *)moby, (char *)d);
            FUN_L01_002e3830(moby, d);
        } else {
            if (moby->prev_seq != 5) {
                blend_moby_animation_3830(moby, 5, 0, scale_game_frames(10));
            }
            qzero(d->vel);
            d->timer = D_L01_00161924;
            moby->state = 6;
            if (d->parent != -1) {
                ents_2e3f20[d->parent].command = 5;
            }
        }
        FUN_L01_002e35a8((char *)moby, (char *)d);
        break;
    case 6:
        if (fast_dec_timer_s16(&d->timer)) {
            moby->alpha = 0x80;
            qcopy(((float *)&moby->pos), d->home);
            qcopy(d->target, d->home);
            qzero(d->vel);
            moby->state = 0x13;
            moby->flags &= 0xFDFF;
        } else {
            int cr;
            int cg;
            int cb;
            int col;
            int dir;
            int n;
            float f;
            *(u128_2e3f20 *)vb = 0;
            qcopy(va, ((float *)&moby->pos));
            FUN_L00_0025f8e0(va, D_L01_00161930);
            cr = (random_integer_3830() + 0x30) & 0x3F;
            cg = (random_integer_3830() + 0x20) & 0x3F;
            cb = random_integer_3830() & 0x2F;
            col = cr | (cg << 8) | (cb << 16) | (D_L01_0016193C << 24);
            dir = random_integer_3830() % 2;
            if (d->timer & 1) {
                dir = -dir;
            }
            n = FUN_L00_00257b90(D_L01_00161928, D_L01_0016192C);
            f = random_float_between_alt(D_L01_00161934, D_L01_00161938);
            spawn_particle_2e3f20(va, n, col, 2, dir, vb, f, 0.001f, D_L01_00161940 * frame_time_sq);
            moby->flags |= 0x200;
            moby->alpha -= 0x80 / D_L01_00161924;
            moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 25.132742f);
        }
        break;
    case 19:
        {
            float t = frame_scale * -0.050000012f + 1.0f;
            d->vel[0] = d->vel[0] * t;
            d->vel[1] = d->vel[1] * t;
        }
        if (FUN_L01_002e3208(moby, d) & 2) {
            qzero(d->vel);
            qcopy(d->target, ((float *)&ents_2e3f20[d->parent].pos));
            moby->state = 0x14;
        }
        break;
    case 20:
        {
            float z;
            int r;
            mover_2e3f20.p50 = 0;
            ang = fast_arc_tan(d->target[0] - moby->pos.x, d->target[1] - moby->pos.y);
            face_target_2e3f20(moby, ang, &mover_2e3f20.f14,
                                    frame_time_sq * 11.519173f, frame_time_sq * 3.1415927f, frame_time * 11.519173f);
            z = moby->pos.z;
            r = step_mover_2e3f20(moby, &mover_2e3f20, d->target, d->vel);
            moby->pos.z = z;
            if (r & 4) {
                if (moby->prev_seq != 0) {
                    blend_moby_animation_3830(moby, 0, 0, scale_game_frames(6));
                }
                ents_2e3f20[d->parent].command = 1;
                moby->state = 1;
                moby->draw_dist = 0;
            }
        }
        break;
    case 21:
        break;
    }
    finish_moby_2e3f20(moby, 0.3f);
    if (d->parent != -1) {
        *(int *)&ents_2e3f20[d->parent].collision = ents_2e3f20[d->parent].pclass->collision;
    }
}
/* Burning debris: flies along its velocity leaving two smoke trails, sticks to the ground, and hurts what it hits. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002EEDD8), where it is exact; names translated to the US level program. */

typedef struct {
    float v[2];
    float f8;
    float fC;
    char pad10[8];
    char b18;
    char b19;
    short s1A;
    char pad1C[0x14];
} Hit_2eedd8;

extern float probe_ground_height(void *, int, float) __asm__("func_00213508");
extern int collision_line(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern void set_vector_length_xy(void *, void *, float) __asm__("FUN_001f9c48");
extern void FUN_L00_00269958(void *, void *, int, int, int, int, int, int);
extern void make_hit_descriptor(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern void FUN_L00_00259a88(void *, void *);
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void clear_vector(void *) __asm__("func_001F99F8");
void mark_moby_for_removal(void *obj) __asm__("FUN_0020c828");
extern char D_L01_00174340_c[] __asm__("D_L01_001742C0");

void FUN_L01_002eda00(char *m) {
    char *d = *(char **)(m + 0x78);
    float prev[4];
    float z[4];
    Hit_2eedd8 h;
    float gz;
    switch (((unsigned char *)m)[0x20]) {
    case 1:
        qcopy(prev, m + 0x10);
        add_vector_xyz(m + 0x10, m + 0x10, d);
        *(float *)(m + 0x48) = fast_arc_tan(*(float *)(d + 0), *(float *)(d + 4));
        clear_vector(z);
        FUN_L00_00269958(m + 0x10, z, 0x6F00AFFF, 0xFF,
                         scale_game_frames(FUN_L00_00257b90(0xF, 0x16)), 0x28,
                         FUN_L00_00257b90(0x14, 0x23), 1);
        FUN_L00_00269958(m + 0x10, z, 0x1FFFFFFF, 0x4F4F4F,
                         scale_game_frames(FUN_L00_00257b90(0x1E, 0x3C)), 0x28,
                         FUN_L00_00257b90(0x32, 0x4B), 0);
        gz = *(float *)(m + 0x18) - probe_ground_height(m + 0x10, 0, 0.5f);
        if (gz < *(float *)(d + 0x18)) {
            *(float *)(m + 0x18) = *(float *)(m + 0x18) + (*(float *)(d + 0x18) - gz);
            *(int *)(d + 8) = 0;
        } else if (*(float *)(m + 0x18) < hero.ground_point.f[2] + *(float *)(d + 0x18)) {
            *(int *)(d + 8) = 0;
        } else {
            *(float *)(d + 8) -= frame_time_sq * 10.8f;
        }
        if (collision_line(m + 0x10, prev, 0, *(int *)(d + 0x10), 0) != 0) {
            char *p = D_L01_00174340_c;
            if (*(int *)(p + 0x18) != 0) {
                make_hit_descriptor(&h, m, 0x10001, 1.0f, d);
                set_vector_length_xy(&h, &h, 1.0f);
                h.f8 = 1.0f;
                h.fC = 5627.9248f;
                h.b19 = 1;
                h.s1A = *(unsigned short *)(m + 0xA6);
                h.b18 = 0;
                FUN_L00_00259a88(*(void **)(p + 0x18), &h);
            }
            m[0x20] = 2;
        }
        if (tick_countdown_32_alt(d + 0x14) != 0)
            m[0x20] = 2;
        break;
    case 2:
        cart_burst(m, m + 0x10, -1, 0.25f, 13.0f);
        mark_moby_for_removal(m);
        break;
    }
}
extern void FUN_L00_00263ac8(float, void *, int, void *);
extern void FUN_L00_0025a120(void *);
extern void FUN_L01_002eec68_u(char *) __asm__("FUN_L01_002eec68");
extern void FUN_L01_002ef560(unsigned char *);
extern void FUN_L01_002ef770(unsigned char *, float *);
extern int follow_path(int *, int, int, float, void *, void *, void *) __asm__("FUN_L01_00276fe8");
extern void FUN_L01_0026d930(void *);
extern void FUN_L00_00258b50(void *, void *, void *, void *, float);
extern float approach_angle(float *, float *, float, float, float, float) __asm__("FUN_L00_0025be00");
extern int mine_fall(void *, void *) __asm__("FUN_L00_0025c698");
extern int FUN_L00_00261b48(int, void *, void *, void *);
extern void spawn_nearby_effect(void *, void *, int, void *, void *, float) __asm__("FUN_L00_00259bc8");
extern int FUN_L00_002dc9f0(void *, void *);
extern int FUN_L00_00257470(void *, int, int);
extern int FUN_00214cc8(float, void *);
extern int FUN_00214720(void *, int);
extern void FUN_0022da68(int, int, char *);
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");
extern int FUN_00213260(int);
extern float FUN_00213308(void);
extern float random_float_between(float, float) __asm__("FUN_002132a8");
extern float FUN_001f9b48(void *, void *);
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float FUN_001f99c0(float);
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern int FUN_001fa6d0(float);
extern float compute_interpolated_record_value(void *) __asm__("FUN_0020c9e0");
extern void FUN_0020c828(void *);
extern float frame_time_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float D_0015ED70 __attribute__((section(".sdata")));
extern float D_0013F420[4];
extern float D_L01_0015F580[] __attribute__((section(".sdata")));

extern char D_L01_00161A40 __attribute__((sda));
extern char D_L01_00161A50 __attribute__((sda));
extern float D_L01_001619AC __attribute__((sda));

void large_amoeboid_update(unsigned char *moby) __asm__("FUN_L01_002edca0");
void large_amoeboid_update(unsigned char *moby) {
    char *data = *(char **)(moby + 0x78);
    char *target;
    char *target_position;
    float move[4];
    float hit[4];
    float angle;
    float distance;
    int path_hit;
    int blocked;
    int result;

    if (moby[0x20] == 12) {
        *(u16 *)(moby + 0x34) |= 1;
        return;
    }
    FUN_L00_00263ac8(2.9f, moby, 0, data + 0x270);
    if (moby[0x31] != 0 && FUN_001f9b48(moby + 0x10, D_L01_00167240) < 28.0f) {
        FUN_L00_0025a120(moby);
        moby[0x7f] = 0x16;
    }
    FUN_L01_002eec68_u((char *)moby);
    target = *(char **)(data + 0x200);
    switch (moby[0x20]) {
    case 0:
        qcopy(data + 0x210, moby + 0x10);
        switch (*(short *)(moby + 0xa6)) {
        case 0x23c:
            data[0x58] = FUN_001fa6d0(14.0f);
            data[0x5a] = FUN_001fa6d0(10.4f);
            *(float *)(data + 0x250) = 1.0f;
            break;
        case 0x361:
            data[0x58] = FUN_001fa6d0(14.0f);
            data[0x5a] = FUN_001fa6d0(5.6f);
            *(float *)(data + 0x250) = 0.7f;
            break;
        case 0x362:
            data[0x58] = FUN_001fa6d0(14.0f);
            data[0x5a] = FUN_001fa6d0(4.0f);
            *(float *)(data + 0x250) = 0.49f;
            break;
        }
        *(float *)(data + 0x30) = *(float *)(data + 0x250);
        *(float *)(data + 0x254) = FUN_00213308();
        if (*(short *)(moby + 0xa6) == 0x362)
            *(void **)(data + 0x130) = &D_L01_00161A50;
        else
            *(void **)(data + 0x130) = &D_L01_00161A40;
        data[0x29] = 1;
        if (*(int *)(data + 0x258) != 0) {
            *(float *)(data + 0x20) = 1.0f;
            *(int *)(data + 0x230) = 1;
        } else {
            *(float *)(data + 0x20) = 3.0f;
        }
        if (*(float *)(data + 0x224) == 0.0f)
            *(float *)(data + 0x224) = 15.0f;
        FUN_L01_0026d930(data + 0x170);
        *(float *)(data + 0x17c) = 2.0f;
        *(float *)(data + 0x178) = 2.0f;
        result = FUN_001fa6d0(*(float *)(data + 0x250) * 0.8f * 1024.0f);
        angle = *(float *)(data + 0x23c) * frame_time_gp;
        *(int *)(data + 0x170) = result;
        *(float *)(data + 0x194) = angle;
        if (*(int *)(data + 0x230) != 0) {
            moby[0x20] = 1;
            *(u16 *)(moby + 0x34) |= 0x1000;
        } else {
            if (*(int *)(data + 0x25c) == -1) moby[0x20] = 12;
            else moby[0x20] = 13;
            *(int *)(moby + 0x94) = 0;
            *(int *)(moby + 0x58) = 0;
            moby[0x31] = 0;
            distance = *(float *)(moby + 0x18) - 20.0f;
            *(u16 *)(moby + 0x34) = (*(u16 *)(moby + 0x34) & 0xefff) | 1;
            *(float *)(moby + 0x18) = distance;
        }
        break;
    case 1:
        if (*(int *)(data + 0x204) != 2) {
            if (FUN_00213260(4) == 0) {
                path_hit = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                            *(float *)(data + 0x250) * 0.8f,
                                            moby + 0x10, target + 0x10, move);
                if (path_hit != 0) {
                    moby[0x20] = 4;
                    if (moby[0x53] != 1)
                        blend_moby_animation(moby, 1, 0, scale_game_frames(10));
                    clear_u64_value(data + 0x40);
                    *(float *)(data + 0x234) = random_float_between(-0.34906584f, 0.34906584f);
                } else {
                    *(int *)(data + 0x204) = 2;
                }
            }
        }
        if (*(int *)(data + 0x22c) != 0 && *(int *)(data + 0x204) == 2) {
            moby[0x20] = 15;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, 5);
            return;
        }
        *(float *)(data + 0x194) = *(float *)(data + 0x23c) * 0.5f * frame_time_gp;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, 5);
        moby[0x20] = 2;
        break;
    case 2:
        angle = fast_arc_tan(*(float *)(data + 0x210) - *(float *)(moby + 0x10),
                              *(float *)(data + 0x214) - *(float *)(moby + 0x14));
        approach_angle((float *)(moby + 0x48), (float *)(data + 0x220), angle,
                          D_0015ED70 * 1.0471976f, D_0015ED70 * 1.0471976f,
                          frame_time_gp * 3.1415927f);
        move[0] = fast_cos(*(float *)(moby + 0x48));
        move[1] = fast_sin(*(float *)(moby + 0x48));
        move[2] = 0.0f;
        FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
        result = *(int *)(data + 0x204);
        if (result != 2 && FUN_00213260(4) == 0) {
            path_hit = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                        *(float *)(data + 0x250) * 0.8f,
                                        moby + 0x10, target + 0x10, hit);
            if (path_hit != 0) {
                moby[0x20] = 3;
                if (moby[0x53] != 0)
                    blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                clear_u64_value(data + 0x40);
                angle = random_float_between(-0.34906584f, 0.34906584f);
                distance = *(float *)(data + 0x23c) * frame_time_gp;
                *(float *)(data + 0x234) = angle;
                *(float *)(data + 0x194) = distance;
            } else {
                *(int *)(data + 0x204) = 2;
            }
        }
        if (*(int *)(data + 0x22c) != 0 && *(int *)(data + 0x204) == 2) {
            moby[0x20] = 15;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, 5);
        }
        FUN_L01_002ef560(moby);
        return;
    case 15:
        blocked = 0;
        angle = fast_arc_tan(*(float *)(target + 0x10) - *(float *)(moby + 0x10),
                              *(float *)(target + 0x14) - *(float *)(moby + 0x14));
        approach_angle((float *)(moby + 0x48), (float *)(data + 0x220), angle,
                          D_0015ED70 * 1.0471976f, D_0015ED70 * 1.0471976f,
                          frame_time_gp * 3.1415927f);
        move[0] = fast_cos(*(float *)(moby + 0x48));
        move[1] = fast_sin(*(float *)(moby + 0x48));
        move[2] = 0.0f;
        FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
        if (FUN_L00_00261b48(*(int *)(data + 0x240), moby + 0x10, target + 0x10, hit) != 0 &&
            FUN_001f9b48(moby + 0x10, hit) < 1.5f) blocked = 1;
        if (*(int *)(data + 0x22c) == 0) {
            moby[0x20] = 5;
        } else if (blocked) {
            moby[0x20] = 16;
            if (moby[0x53] != 0)
                blend_moby_animation(moby, 0, 0, scale_game_frames(10));
        } else if (vector_distance(moby + 0x10, target + 0x10) < 2.0f) {
            moby[0x20] = 6;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 5, scale_game_frames(10));
            return;
        }
        break;
    case 16:
        if (!(moby[0x70] & 2) || *(int *)(data + 0x22c) != 0) return;
        moby[0x20] = 5;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(20));
        return;
    case 4: {
        char *target_position;
        unsigned char path_hit;
        target_position = target + 0x10;
        distance = vector_distance(moby + 0x10, target_position);
        if (distance < D_L01_001619AC) {
            moby[0x20] = 6;
            *(float *)(data + 0x194) = (*(float *)(data + 0x23c) + *(float *)(data + 0x23c)) * frame_time_gp;
            if (moby[0x53] != 2)
                blend_moby_animation(moby, 2, 0, scale_game_frames(10));
            return;
        } else {
            if (follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                        *(float *)(data + 0x250) * 0.8f, moby + 0x10, target_position, hit) != 0)
                path_hit = 1;
            else
                path_hit = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                            0.0f, moby + 0x10, target_position, hit) != 0;
            if (path_hit != 0) {
                angle = fast_arc_tan(hit[0] - *(float *)(moby + 0x10), hit[1] - *(float *)(moby + 0x14));
                angle = fast_add_rotations(angle, *(float *)(data + 0x234));
                approach_angle((float *)(moby + 0x48), (float *)(data + 0x220), angle,
                                  D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f,
                                  frame_time_gp * 25.132742f);
                move[0] = fast_cos(*(float *)(moby + 0x48));
                move[0] += move[0];
                move[1] = fast_sin(*(float *)(moby + 0x48));
                move[1] += move[1];
                move[2] = 0.0f;
                FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
            } else {
                *(int *)(data + 0x204) = 2;
            }
            if (*(int *)(data + 0x204) == 2) moby[0x20] = 5;
            FUN_L01_002ef560(moby);
            return;
        }
    }
    case 6: {
        char *target_position;
        float *rotation = (float *)(moby + 0x48);
        angle = fast_arc_tan(*(float *)(target + 0x10) - *(float *)(moby + 0x10),
                              *(float *)(target + 0x14) - *(float *)(moby + 0x14));
        approach_angle(rotation, (float *)(data + 0x220), angle,
                          D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f,
                          frame_time_gp * 25.132742f);
        distance = compute_interpolated_record_value(moby);
        if (moby[0x52] == moby[0x53] && 10.0f <= distance && distance <= 16.0f) {
            move[0] = fast_cos(*rotation);
            move[0] += move[0];
            move[1] = fast_sin(*rotation);
            move[1] += move[1];
            move[2] = 0.0f;
            FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
        }
        if (FUN_00214cc8(15.5f, moby) != 0 && vector_distance(moby + 0x10, (target_position = target + 0x10)) < 2.0f &&
            FUN_001f99c0(*(float *)(target + 0x18) - *(float *)(moby + 0x18)) <
                *(float *)(data + 0x250) + *(float *)(data + 0x250)) {
            move[0] = fast_cos(*(float *)(moby + 0x48));
            move[1] = fast_sin(*(float *)(moby + 0x48));
            move[2] = 0.0f;
            move[3] = 5627.9248f;
            qcopy(hit, target_position);
            hit[2] += 0.75f;
            spawn_nearby_effect(target, moby, 0x10001, hit, move, 1.0f);
            return;
        }
        if (moby[0x70] & 2) {
            moby[0x20] = 7;
            if (moby[0x53] != 3)
                blend_moby_animation(moby, 3, 0, scale_game_frames(10));
            *(float *)(data + 0x194) = *(float *)(data + 0x23c) * frame_time_gp;
        }
        break;
    }
    case 3:
    case 7:
        if (!(moby[0x70] & 2)) return;
        moby[0x20] = 4;
        if (moby[0x53] != 1)
            blend_moby_animation(moby, 1, 0, scale_game_frames(10));
        return;
    case 5: {
        int blocked = 0;
        char *target_position;
        distance = vector_distance(moby + 0x10, (target_position = data + 0x210));
        if (distance < 1.0f) {
            moby[0x20] = 1;
        } else {
            if (follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                        *(float *)(data + 0x250) * 0.8f, moby + 0x10, target_position, hit) != 0)
                blocked = 1;
            else
                blocked = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                            0.0f, target_position, target_position, hit) != 0;
        }
        if (blocked != 0) {
            angle = fast_arc_tan(hit[0] - *(float *)(moby + 0x10), hit[1] - *(float *)(moby + 0x14));
            angle = fast_add_rotations(angle, *(float *)(data + 0x234));
            approach_angle((float *)(moby + 0x48), (float *)(data + 0x220), angle,
                              D_0015ED70 * 12.566371f, D_0015ED70 * 12.566371f,
                              frame_time_gp * 25.132742f);
            move[0] = fast_cos(*(float *)(moby + 0x48));
            move[0] += move[0];
            move[1] = fast_sin(*(float *)(moby + 0x48));
            move[1] += move[1];
            move[2] = 0.0f;
            FUN_L00_00258b50(moby, data + 0x170, move, data + 0x40, 1.0f);
        }
        if (*(int *)(data + 0x204) != 2 && (moby[0x20] = 4, moby[0x53] != 1))
            blend_moby_animation(moby, 1, 0, scale_game_frames(3));
        FUN_L01_002ef560(moby);
        return;
    }
    case 8:
        result = mine_fall(moby, data + 0x60);
        if (result & 1) {
            path_hit = follow_path((int *)(data + 0x240), 1, *(int *)(data + 0x244),
                                        *(float *)(data + 0x250) * 0.8f,
                                        moby + 0x10, data + 0x210, move);
            if (path_hit != 0) {
                FUN_0022da68(4, 0, (char *)moby);
                *(float *)(data + 0x194) = *(float *)(data + 0x23c) * frame_time_gp;
                moby[0x20] = 9;
                if (moby[0x53] != 5)
                    blend_moby_animation(moby, 5, 4, scale_game_frames(10));
                qcopy(data + 0x210, moby + 0x10);
                return;
            }
            FUN_L01_002ef770(moby, D_L01_0015F580);
            FUN_L00_00257470(moby, 0, -1);
        } else if (!(result & 0x120)) {
            return;
        }
        FUN_0020c828(moby);
        return;
    case 9:
        if (moby[0x70] & 2) {
            moby[0x20] = 4;
            if (moby[0x53] != 1)
                blend_moby_animation(moby, 1, 0, scale_game_frames(10));
            clear_u64_value(data + 0x40);
            angle = random_float_between(-0.34906584f, 0.34906584f);
            distance = *(float *)(data + 0x23c) * frame_time_gp;
            *(float *)(data + 0x234) = angle;
            *(float *)(data + 0x194) = distance;
        }
        break;
    case 13:
        *(u16 *)(moby + 0x34) |= 1;
        if (FUN_00214720(D_0013F420, *(int *)(data + 0x25c)) != 0) {
            u16 flags;
            float distance;
            int result;
            distance = *(float *)(moby + 0x18) + 20.0f;
            flags = *(u16 *)(moby + 0x34) | 0x1000;
            moby[0x20] = 1;
            *(u16 *)(moby + 0x34) = flags;
            *(float *)(moby + 0x58) = 1.0f;
            *(float *)(moby + 0x18) = distance;
            result = *(int *)(*(char **)(moby + 0x24) + 0x10);
            *(int *)(moby + 0x94) = result;
            *(u16 *)(moby + 0x34) = flags & 0xfffe;
            moby[0x31] = 1;
        }
        return;
    case 14:
        if (FUN_L00_002dc9f0(moby, data + 0x60)) {
            moby[0x20] = 1;
            *(u16 *)(data + 0x128) = 0;
        }
        break;
    }
}
/* Moby update: hit test against a target, hurt/debris on a hit, else path search and bob. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F0040), where it is exact; names translated to the US level program. */

typedef int u128_2F0040 __attribute__((mode(TI)));

typedef union { u128_2F0040 q; float f[4]; } V_2F0040;

extern char *FUN_L00_0025a420(void *, int, int);
extern char *FUN_L01_002ef350(void *);
extern char *FUN_L01_002ef478_c(char *, int) __asm__("FUN_L01_002ef478");
extern char D_0013E533[];
extern f32 fast_sin_c(f32) __asm__("func_001F9DE0");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");

extern float fast_add_rotations_c(float, float) __asm__("func_001FA580");
extern float fast_vec_dist(void *, void *);
extern float vector_distance_xy_alt(void *, void *) __asm__("FUN_001f9b80");
extern char *D_L01_0015FFD8;
extern int D_L01_001612D0;
extern float D_L01_001619A0;
extern float D_L01_001619A4;
extern int FUN_L00_0025ff38(float, char *, void *, int, int, void *, int);
extern int fast_tween_color(float, int, int);
extern int fun_00213928(void *, void *, void *, int, int *, float *, int, int);
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern short D_L01_00161998 __attribute__((sda));
extern short D_L01_0016199C __attribute__((sda));
extern void trooper_knockback_arc(void *, float *, void *, void *) __asm__("FUN_L00_0025ab48");
extern void trooper_start_knockback(void *, void *, float, int, int, int) __asm__("FUN_L00_0025c558");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("func_0022DA68");
extern void clear_u64_value_c(void *) __asm__("func_001F99F8");
extern void mark_moby_for_removal_c(void *) __asm__("func_0020C828");
float absolute_float(float input) __asm__("func_001F99C0");
extern float D_0013F420[];
extern int D_001413D0[];
extern char *D_L01_0015FFD8;
extern const float D_L01_001619A0_gp __asm__("D_L01_001619A0") __attribute__((sda));
extern const float D_L01_001619A4_gp __asm__("D_L01_001619A4") __attribute__((sda));
extern const float frame_time_macro __asm__("D_0015ED6C") MACRO_ADDR;

void FUN_L01_002eec68(char *m) {
    V_2F0040 v;
    V_2F0040 w;
    int flag;
    float amount;
    float first_spawn_angle;
    float second_spawn_angle;
    char *d;
    char *t;
    char *p;
    char *x;
    char *e;
    char *spawned;
    char *q;
    char *cand;
    u128_2F0040 *wp;
    int c;
    char *p2;
    char *x2;
    char *b;
    int c258;
    int c260;
    float f20;
    float rate;
    float fixed_scale;
    float f24;
    float dist;
    float ang;
    float k;
    float seven;

    amount = 0.0f;
    d = *(char **)(m + 0x78);
    t = FUN_L00_0025a420(m, 0x330000, 0);
    if (t != 0) {
        qcopy(&v, m + 0x10);
        v.f[2] = v.f[2] + 0.5f;
        p = *(char **)(t + 0x20);
        if (p != 0) {
            short cls = *(short *)(p + 0xA6);
            if (cls == 0x47) {
                qcopy(&w, ((char *)D_0013F420));
                wp = &w.q;
            } else {
                x = *(char **)(p + 0x24);
                if ((x != 0 && *(short *)(x + 0x46) == 5) || cls == 0xBA) {
                    qcopy(&w, &v);
                    wp = &w.q;
                } else {
                    qcopy(&w, p + 0x10);
                    wp = &w.q;
                }
            }
        } else {
            qcopy(&w, t);
            wp = &w.q;
        }
        if (fast_vec_dist(&v, wp) < 24.0f) {
            if (func_L00_001EFFF0_t(&v, wp, 2, m, 0) != 0) {
                t = 0;
            }
        }
    }
    fun_00213928(m, t, d + 0x20, 0, &flag, &amount, 0, 4);
    if (flag != 1 && amount != 0.0f && *(unsigned char *)(m + 0x20) != 0xB && *(unsigned char *)(m + 0x20) != 0xA && *(unsigned char *)(m + 0x20) != 8) {
        f20 = 0.8f;
        allocate_voice_for_target_entry(0, 0, m);
        f24 = 1.0f;
        fixed_scale = 1024.0f;
        rate = 0.008f;
        *(float *)(d + 0x20) = *(float *)(d + 0x20) - f24;
        d[0x9D] = 0;
        *(float *)(d + 0x70) = rate;
        *(int *)(d + 0x84) = 8;
        *(int *)(d + 0x80) = truncate_float_to_s32_c(*(float *)(d + 0x250) * f20 * fixed_scale);
        *(float *)(d + 0x88) = *(float *)(d + 0x250) * f20;
        FUN_L01_002ef770((unsigned char *)m, (float *)(t + 0x10));
        if (0.0f < *(float *)(d + 0x20)) {
            spawned = FUN_L01_002ef350(m);
            if (spawned != 0) {
                q = *(char **)(spawned + 0x78);
                q[0x9D] = 3;
                *(float *)(q + 0x70) = rate;
                *(int *)(q + 0x84) = 8;
                *(int *)(q + 0x80) = truncate_float_to_s32_c(*(float *)(d + 0x250) * f20 * fixed_scale);
                k = frame_time_macro;
                seven = 7.0f;
                *(float *)(q + 0x88) = *(float *)(d + 0x250) * f20;
                *(float *)(q + 0x78) = D_L01_001619A0_gp * k;
                *(float *)(q + 0x7C) = D_L01_001619A4_gp * k;
                *(int *)(q + 0x84) = *(int *)(q + 0x84) | 4;
                *(float *)(q + 0xB0) = seven;
                *(float *)(q + 0xB4) = 13.0f;
                v.q = *(u128_2F0040 *)(t + 0x10);
                trooper_knockback_arc(&v, &first_spawn_angle, q + 0x78, q + 0x7C);
                first_spawn_angle = fast_subtract_rotations(first_spawn_angle, 0.7853982f);
                trooper_start_knockback(spawned, q + 0x60, first_spawn_angle, 4, 8, 0);
                spawned[0x20] = 8;
                *(float *)(spawned + 0x58) = f24;
            }
            spawned = FUN_L01_002ef350(m);
            if (spawned != 0) {
                q = *(char **)(spawned + 0x78);
                *(float *)(q + 0x70) = rate;
                q[0x9D] = 3;
                *(int *)(q + 0x84) = 8;
                *(int *)(q + 0x80) = truncate_float_to_s32_c(*(float *)(d + 0x250) * f20 * fixed_scale);
                k = frame_time_macro;
                seven = 7.0f;
                *(float *)(q + 0x88) = *(float *)(d + 0x250) * f20;
                *(float *)(q + 0x78) = D_L01_001619A0_gp * k;
                *(float *)(q + 0x7C) = D_L01_001619A4_gp * k;
                *(int *)(q + 0x84) = *(int *)(q + 0x84) | 4;
                *(float *)(q + 0xB0) = seven;
                *(float *)(q + 0xB4) = 13.0f;
                v.q = *(u128_2F0040 *)(t + 0x10);
                trooper_knockback_arc(&v, &second_spawn_angle, q + 0x78, q + 0x7C);
                second_spawn_angle = fast_add_rotations_c(second_spawn_angle, 0.7853982f);
                trooper_start_knockback(spawned, q + 0x60, second_spawn_angle, 4, 8, 0);
                spawned[0x20] = 8;
                *(float *)(spawned + 0x58) = f24;
            }
            p2 = *(char **)(t + 0x20);
            if (p2 != 0 && (x2 = *(char **)(p2 + 0x24)) != 0 && *(short *)(x2 + 0x46) == 5) {
                FUN_L00_00257470(m, 0x800, -1);
            } else {
                FUN_L00_00257470(m, 0, -1);
            }
            mark_moby_for_removal_c(m);
            return;
        }
        *(unsigned short *)(m + 0x34) = *(unsigned short *)(m + 0x34) & 0xEFFF;
        mark_moby_for_removal_c(m);
        return;
    }
    *(unsigned char *)(m + 0xA4) = 0xFF;
    c258 = *(int *)(d + 0x258);
    if (c258 != 0 && *(unsigned char *)(m + 0x20) != 0
        && *(float *)(m + 0x18) < *(float *)(c258 * 0x1190 + D_L01_001612D0 + 8)) {
        clear_u64_value_c(&v);
        FUN_L01_002ef770((unsigned char *)m, (float *)&v);
        mark_moby_for_removal_c(m);
        return;
    }
    if (*(int *)(d + 0x38) != 0) {
        *(int *)(d + 0x22C) = scale_game_frames(0xF0);
        *(int *)(d + 0x38) = 0;
    }
    if (tick_countdown_32_alt(d + 0x22C) != 0) {
        *(float *)(d + 0x228) = *(float *)(d + 0x224);
    } else {
        *(float *)(d + 0x228) = *(float *)(d + 0x224) + 20.0f;
    }
    c260 = *(int *)(d + 0x260);
    if (c260 <= 0 || *(unsigned char *)((c260 << 8) + (*(int *)&D_L01_0015FFD8) + 0xBC) != 0) {
        e = D_L01_001B0930[*(int *)(d + 0x24C)];
        b = d + 0x1C0;
        if (FUN_L00_0025ff38(*(float *)(d + 0x228), m, b, 0, 0, e + 0x10, *(int *)e) != 2) {
            if (*(float *)(d + 0x228) < vector_distance_xy_alt(d + 0x210, b)
                || 3.0f < absolute_float(*(float *)(m + 0x18) - *(float *)(d + 0x1C8))) {
                *(int *)(d + 0x204) = 2;
            }
        }
    }
    c = *(int *)(d + 0x248);
    if (c >= 0) {
        cand = FUN_L01_002ef478_c(m, c);
        if (cand != 0) {
            dist = vector_distance_xy_alt(m + 0x10, cand + 0x10);
            if (dist < *(float *)(d + 0x228)) {
                if (*(int *)(d + 0x204) == 2
                    || dist < vector_distance_xy_alt(m + 0x10, *(char **)(d + 0x200) + 0x10)) {
                    *(char **)(d + 0x200) = cand;
                    *(int *)(d + 0x204) = 1;
                }
            }
        }
    }
    if (*(int *)(d + 0x200) == 0) {
        *(int *)(d + 0x200) = *(int *)(((char *)((char (*))D_001413D0)));
    }
    ang = fast_add_rotations_c(*(float *)(d + 0x254), frame_time * 9.424778f);
    *(float *)(d + 0x254) = ang;
    *(int *)(m + 0x90) = fast_tween_color((fast_sin_c(ang) + 1.0f) * 0.5f,
                                           *(int *)&D_L01_00161998, *(int *)&D_L01_0016199C);
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L01_002ef350.s", FUN_L01_002ef350);
#else
extern short *lists_2ef478[] __asm__("D_L01_001ABCC0");
extern char *D_L01_0015FFD8;
extern void FUN_L00_00250df8(void *);

char *FUN_L01_002ef350(void *arg) {
    char *self = arg;
    short *list;
    char *entities;
    int entry;
    int type = -1;
    if (*(short *)(self + 0xA6) == 0x23C) {
        type = 0x361;
    } else if (*(short *)(self + 0xA6) == 0x361) {
        type = 0x362;
    }
    entry = *(volatile unsigned char *)(self + 0x21);
    entities = (char *)(lists_2ef478 + entry);
    list = *(short **)entities;
    entities = D_L01_0015FFD8;
    if (list == 0) return 0;
    goto scan;
    do {
        char *other;
        int index;
advance:
        if ((short)entry < 0) return 0;
        list++;
scan:
        index = *(unsigned short *)list;
        entry = index;
        index &= 0x7FFF;
        other = entities + (index << 8);
        if ((unsigned char)other[0x20] != 12 || *(short *)(other + 0xA6) != type) goto advance;
        {
            /* Retail reads both pvars pointers and unk30 before writing the entity. */
            char *other_data = *(char * volatile *)(other + 0x78);
            unsigned char unk30 = *(volatile unsigned char *)(self + 0x30);
            char *self_data = *(char * volatile *)(self + 0x78);
            other[0x30] = unk30;
            *(unsigned short *)(other + 0x32) = *(unsigned short *)(self + 0x32);
            {
                unsigned long long spawn_frame = *(unsigned long long *)(self + 0x38);
                other[0x31] = 1;
                *(unsigned long long *)(other + 0x38) = spawn_frame;
            }
            *(int *)(other + 0x94) = *(int *)(*(char **)(other + 0x24) + 0x10);
            *(unsigned short *)(other + 0x34) = *(unsigned short *)(self + 0x34);
            qcopy(other + 0x10, self + 0x10);
            qcopy(other + 0x40, self + 0x40);
            *(float *)(other_data + 0x20) = *(float *)(self_data + 0x20);
            *(float *)(other_data + 0x224) = *(float *)(self_data + 0x224);
            FUN_L00_00250df8(other);
            return other;
        }
    } while (1);
    return 0;
}
#endif /* NON_MATCHING */

typedef struct {
    char p0[0x10];
    char pos[0x10];
    unsigned char state;
    char p21[0x13];
    unsigned short flags;
    char p36[0x70];
    short type;
    char pA8[0x58];
} Ent_2ef478;

extern float dist_2ef478(void *, void *) __asm__("FUN_001f9b48");
extern short *lists_2ef478[] __asm__("D_L01_001ABCC0");
extern char *D_L01_0015FFD8;
extern char *D_L01_0015FFD8;

/* Nearest live entity of type 0x26F in list idx within 120 units of self, or 0. */
char *FUN_L01_002ef478(char *self, int idx) {
    short *list = lists_2ef478[idx];
    float best = 120.0f;
    char *res = 0;
    if (list == 0) {
        return 0;
    }
    while (1) {
        int i = *(unsigned short *)list & 0x7FFF;
        if ((*(Ent_2ef478 * *)&D_L01_0015FFD8)[i].flags & 0x1000) {
            if ((*(Ent_2ef478 * *)&D_L01_0015FFD8)[i].type == 0x26F) {
                if ((*(Ent_2ef478 * *)&D_L01_0015FFD8)[i].state < 0x7F) {
                    float d = dist_2ef478((*(Ent_2ef478 * *)&D_L01_0015FFD8)[i].pos, self + 0x10);
                    if (d < best) {
                        best = d;
                        res = D_L01_0015FFD8 + (i << 8);
                    }
                }
            }
        }
        if (*list++ < 0) {
            return res;
        }
    }
}
#include "rnc/gameplay/entities/moby.h"
#define MOBY(p) ((struct Moby *)(p))
extern int D_L01_001619E0 __attribute__((sda));
extern int D_L01_001619E4 __attribute__((sda));
extern float D_L01_001619E8 __attribute__((sda));
extern float D_L01_001619B8 __attribute__((sda));
extern float D_L01_001619BC __attribute__((sda));
extern int D_L01_001619C0 __attribute__((sda));
extern int D_L01_001619C4 __attribute__((sda));
extern int D_L01_001619B0 __attribute__((sda));
extern int D_L01_001619B4 __attribute__((sda));
extern int D_L01_001619C8 __attribute__((sda));
extern int D_L01_001619CC __attribute__((sda));
extern float D_L01_001619D0 __attribute__((sda));
extern float D_L01_001619D4 __attribute__((sda));
extern int D_L01_001619D8 __attribute__((sda));
extern int D_L01_001619DC __attribute__((sda));
extern int random_integer_below(int) __asm__("FUN_00213260");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void FUN_L01_00287158(float *, float, float, int, int, int);

void FUN_L01_002ef560(unsigned char *moby) {
    char *data = (char *)MOBY(moby)->pvars;
    float pos[3];
    float angle, radius, lower, upper;
    int time;
    if (MOBY(moby)->visible == 0)
        return;
    if (random_integer_below(D_L01_001619E0 - 1) == 0) {
        angle = random_angle_radians();
        radius = random_float_between_alt(0.0f, D_L01_001619E8);
        lower = random_float_between_alt(0.0f, D_L01_001619B8) * *(float *)(data + 0x250);
        upper = random_float_between_alt(D_L01_001619B8, D_L01_001619BC) * *(float *)(data + 0x250);
        time = truncate_float_to_s32(random_float_between_alt((float)D_L01_001619C0, (float)D_L01_001619C4));
        pos[0] = fast_cos(angle) * radius;
        pos[1] = fast_sin(angle) * radius;
        pos[2] = 0.0f;
        add_vector_xyz(pos, pos, &MOBY(moby)->pos);
        FUN_L01_00287158(pos, lower, upper, D_L01_001619B0, D_L01_001619B4,
                         scale_game_frames(time));
    }
    if (random_integer_below(D_L01_001619E4 - 1) == 0) {
        angle = random_angle_radians();
        radius = random_float_between_alt(0.0f, D_L01_001619E8);
        lower = random_float_between_alt(0.0f, D_L01_001619D0) * *(float *)(data + 0x250);
        upper = random_float_between_alt(D_L01_001619D0, D_L01_001619D4) * *(float *)(data + 0x250);
        time = truncate_float_to_s32(random_float_between_alt((float)D_L01_001619D8, (float)D_L01_001619DC));
        pos[0] = fast_cos(angle) * radius;
        pos[1] = fast_sin(angle) * radius;
        pos[2] = 0.0f;
        add_vector_xyz(pos, pos, &MOBY(moby)->pos);
        FUN_L01_00287158(pos, lower, upper, D_L01_001619C8, D_L01_001619CC,
                         scale_game_frames(time));
    }
}
extern s32 D_L01_001619EC __attribute__((sda));
extern s32 D_L01_001619F0 __attribute__((sda));
extern f32 D_L01_001619F4 __attribute__((sda));
extern f32 D_L01_001619F8 __attribute__((sda));
extern f32 D_L01_001619FC __attribute__((sda));
extern f32 D_L01_00161A00 __attribute__((sda));
extern f32 D_L01_00161A04 __attribute__((sda));
extern f32 D_L01_00161A08 __attribute__((sda));
extern f32 D_L01_00161A0C __attribute__((sda));
extern f32 D_L01_00161A10 __attribute__((sda));
extern f32 D_L01_00161A14 __attribute__((sda));
extern f32 D_L01_00161A18 __attribute__((sda));
extern s32 D_L01_00161A1C __attribute__((sda));
extern s32 D_L01_00161A20 __attribute__((sda));
extern s32 D_L01_00161A24 __attribute__((sda));
extern s32 D_L01_00161A28 __attribute__((sda));
extern s32 D_L01_00161A2C __attribute__((sda));
extern s32 D_L01_00161A30 __attribute__((sda));
extern s32 D_L01_00161A34 __attribute__((sda));
extern f32 D_L01_00161A38 __attribute__((sda));

extern f32 D_0015ED70;
extern void FUN_L00_0025f8e0(void *, f32);
extern int fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern f32 FUN_00220e20(f32) __asm__("FUN_001f96b0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern char *spawn_spark(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

void FUN_L01_002ef770(unsigned char *moby, f32 *mode) {
    char *data = (char *)MOBY(moby)->pvars;
    f32 pos[4], origin[4], velocity[4];
    u128 *dest;
    f32 *origin_copy;
    u128 *source;
    f32 angle, radius, vertical, lateral, scale, scaled_radius, scaled_vertical;
    s32 i, next, j, lower, upper, first, second, duration;

    i = 0;
    if (D_L01_001619EC > 0) {
      source = (u128 *)(moby + 0x10);
      dest = (u128 *)pos;
      origin_copy = origin;
      do {
        qcopy_nc(dest, source);
        FUN_L00_0025f8e0(dest, D_L01_001619F4 * *(f32 *)(data + 0x250));
        pos[2] += 1.0f;
        angle = random_angle_radians();
        radius = random_float_between_alt(D_L01_00161A00, D_L01_00161A04);
        lateral = random_float_between_alt(D_L01_00161A08, D_L01_00161A0C);
        vertical = random_float_between_alt(0.0f, D_L01_00161A38);
        scale_vector_xyz(origin_copy, mode, vertical * frame_time_gp);
        scale = fast_cos(angle);
        origin[0] += scale * (radius * frame_time_gp);
        scale = fast_sin(angle);
        scaled_radius = radius * frame_time_gp;
        origin[1] += scale * scaled_radius;
        scaled_vertical = lateral * frame_time_gp;
        origin[2] += scaled_vertical;
        qcopy(velocity, origin_copy);
        next = i + 1;
        velocity[2] -= D_L01_00161A10 * D_0015ED70 * (f32)scale_game_frames(D_L01_00161A30);
        for (j = 0; j < D_L01_001619F0; j++) {
            scale = random_float_between_alt(0.5f, 1.5f);
            origin[3] = scale * D_L01_00161A14;
            velocity[3] = scale * D_L01_00161A18;
            FUN_L00_0025f8e0(pos, D_L01_001619F8);
            FUN_L00_0025f8e0(velocity, D_L01_001619FC * frame_time_gp);
            lower = fast_tween_color(random_float_between_alt(0.0f, 1.0f), D_L01_00161A1C, D_L01_00161A24);
            upper = fast_tween_color(random_float_between_alt(0.0f, 1.0f), D_L01_00161A20, D_L01_00161A28);
            first = scale_game_frames(D_L01_00161A2C);
            second = scale_game_frames(D_L01_00161A30);
            duration = truncate_float_to_s32(FUN_00220e20(random_float_between_alt((f32)D_L01_00161A34 * 0.5f, (f32)D_L01_00161A34 * 2.5f)));
            spawn_spark(pos, origin_copy, velocity, lower, upper, first, second, duration, -1);
        }
        i = next;
      } while (i < D_L01_001619EC);
    }
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F0E60), where it is exact; names translated to the US level program. */

extern int FUN_L00_002db8f8(void *);

int FUN_L01_002efa88(unsigned char *moby) {
    int value = FUN_L00_002db8f8(moby);
    if (value != 0) {
        if (*(short *)(moby + 0xA6) != 0x362) {
            return 0;
        } else if (moby[0x20] == 8) {
            return 0;
        } else {
            moby[0x20] = 0xE;
        }
    } else if (moby[0x20] == 0xE) {
        moby[0x20] = 1;
    }
    return value;
}
extern s32 level_frame_count __asm__("D_L01_0015F5CC");

/* Search backward for the first entry below the loaded threshold. */
s32 FUN_L01_002efbf8(s32 *values, s32 count, s32 offset) {
    s32 index = (s32)((u32)count - 1);
    if (index >= 0) {
        for (; index >= 0; index--) {
            if (values[index] < level_frame_count) {
                values[index] = (s32)((u32)level_frame_count + (u32)offset);
                break;
            }
        }
    }
    return index;
}

#define NOT_SDA

#define MACRO_ADDR

/* runs the moby update and sets its state byte */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B90A8.c: func_L01_002F30F0), where it is exact; names translated to the US level program. */

extern char *FUN_L00_002db890(char *);
extern int FUN_L00_002dbc30(char *, int, int, float);
extern void FUN_0022da68(int, int, char *);

int FUN_L01_002f1d18(char *m, int a, int b, float f) {
    char *h = FUN_L00_002db890(m);
    int r = FUN_L00_002dbc30(m, a, b, f);
    if (r != 0) {
        m[0x20] = 7;
    } else if ((unsigned char)m[0x20] == 7) {
        m[0x20] = 1;
    }
    if (h != 0) {
        if (*(short *)(h + 0x68) == 6) {
            FUN_0022da68(2, 0, m);
        }
    }
    return r;
}
extern float FUN_L00_00258110_w7(float *vel, float cur, float target, float k, float d,
                                 float max) __asm__("FUN_L00_00258110");
extern int FUN_001f96f8_w7(int) __asm__("FUN_001f96f8");
extern void blend_moby_animation_w7(void *m, int a, int b, int c) __asm__("FUN_00212f90");
extern float D_0015ED70_lui __asm__("D_0015ED70");

void sandmouse_house_update(unsigned char *m) __asm__("FUN_L01_002f2b68");
void sandmouse_house_update(unsigned char *m) {
    char *d;
    if (m == 0)
        return;
    d = *(char **)(m + 0x78);
    if (d != 0) {
        if (m[0x20] != 0) {
            if (D_0013D4C5 != 0) {
                if (hero.items[2].item_id == 5) {
                    float s = D_0015ED70_lui;
                    *(float *)(m + 0x48) =
                        FUN_L00_00258110_w7((float *)(d + 4), *(float *)(m + 0x48), *(float *)d,
                                            s * 6.2831855f, s * 5.7595863f, 0.0f);
                }
            }
        }
    }
    switch (m[0x20]) {
    case 0:
        if (m[0x53] != 0) {
            blend_moby_animation_w7(m, 0, 0, FUN_001f96f8_w7(0));
        }
        m[0x20] = 1;
        *(float *)d = *(float *)(m + 0x48);
        break;
    case 1:
        if ((unsigned char)(m[0xBC] - 3) < 3) {
            if (m[0x53] != 1) {
                blend_moby_animation_w7(m, 1, 0, FUN_001f96f8_w7(10));
            }
            m[0x20] = 2;
        }
        break;
    case 2:
        if (m[0x70] & 2) {
            if (m[0x53] != 2) {
                blend_moby_animation_w7(m, 2, 0, FUN_001f96f8_w7(3));
            }
            m[0x20] = 3;
        }
        break;
    case 3:
        if ((unsigned char)(m[0xBC] - 4) < 2) {
            if (m[0x53] != 3) {
                blend_moby_animation_w7(m, 3, 0, FUN_001f96f8_w7(3));
            }
            m[0x20] = 4;
        } else if (m[0xBC] == 7 || m[0xBC] == 1) {
            if (m[0x53] != 6) {
                blend_moby_animation_w7(m, 6, 0, FUN_001f96f8_w7(3));
            }
            m[0x20] = 7;
        }
        break;
    case 4:
        if (m[0x70] & 2) {
            if (m[0x53] != 4) {
                blend_moby_animation_w7(m, 4, 0, FUN_001f96f8_w7(3));
            }
            m[0x20] = 5;
        }
        break;
    case 5:
        if (m[0xBC] == 3 || m[0xBC] == 6 || m[0xBC] == 7 || m[0xBC] == 1) {
            if (m[0x53] != 5) {
                blend_moby_animation_w7(m, 5, 0, FUN_001f96f8_w7(3));
            }
            m[0x20] = 6;
        }
        break;
    case 6:
        if (m[0x70] & 2) {
            if (m[0xBC] == 7 || m[0xBC] == 1) {
                if (m[0x53] != 6) {
                    blend_moby_animation_w7(m, 6, 0, FUN_001f96f8_w7(3));
                }
                m[0x20] = 7;
            } else {
                if (m[0x53] != 2) {
                    blend_moby_animation_w7(m, 2, 0, FUN_001f96f8_w7(3));
                }
                m[0x20] = 3;
            }
        }
        break;
    case 7:
        if (m[0x70] & 2) {
            if (m[0x53] != 0) {
                blend_moby_animation_w7(m, 0, 0, FUN_001f96f8_w7(3));
            }
            m[0x20] = 1;
        }
        break;
    case 8:
        break;
    }
}
#include "sda.h"
#include "qcopy.h"
#include "rnc/gameplay/entities/moby.h"

/* pvars of the moby run by FUN_L01_002f2eb8 */
typedef struct {
    Vec4 home;         /* position at spawn */
    union {
        struct {
            s32 path;   /* -1, or an index into D_L01_001B0930 */
            s32 volume; /* -1, or a clip volume is_point_inside_clip_volume tests */
        } s;
        s64 both;       /* -1 when neither is set */
    } area;
    s32 count;         /* spawn budget; the moby is deleted when it runs out */
} AreaVars_2f2eb8;

extern int D_0015ED84;
extern s32 *D_L01_001B0930_2f2eb8[] __asm__("D_L01_001B0930");
extern void FUN_0020c828_2f2eb8(struct Moby *) __asm__("FUN_0020c828");
extern s32 FUN_L00_00259740_2f2eb8(void *, void *, s32) __asm__("FUN_L00_00259740");
extern s32 FUN_00214720_2f2eb8(void *, s32) __asm__("FUN_00214720");
extern f32 FUN_001f9b80_2f2eb8(void *, void *) __asm__("FUN_001f9b80");

extern s32 D_L01_00161AF0 __attribute__((sda));
void FUN_L01_002f2eb8(struct Moby *m) {
    AreaVars_2f2eb8 *v = (AreaVars_2f2eb8 *)m->pvars;
    Vec4 target;
    s32 k;
    s32 *path;
    f32 d;

    if (m->state == 0) {
        if (v == 0) {
            FUN_0020c828_2f2eb8(m);
            return;
        }
        m->update_dist = 0xFF;
        qcopy(&v->home, &m->pos);
        m->flags |= 0x41;
        m->state = 1;
        if (m->command == 0 && D_0015ED84 < 20) {
            k = D_0014BF10[D_L01_00161AF0 / 2 + D_0015ED84 * 16];
            if (D_L01_00161AF0 & 1) {
                k &= 0xF;
            } else {
                k = (k >> 4) & 0xF;
            }
            D_L01_00161AF0++;
            m->command = D_L01_00161AF0;
            k *= 5;
            v->count -= k;
            if (v->count <= 0) {
                FUN_0020c828_2f2eb8(m);
                return;
            }
        }
    } else {
        D_L01_00161AF0 = 0;
    }
    if (hero.items[0].moby != 0 && hero.items[0].item_id == 0x1B) {
        qcopy(&target, hero.items[0].moby->pvars + 0x10);
    } else {
        qcopy(&target, &hero.motion.pos);
    }
    if (v->area.s.path != -1) {
        path = D_L01_001B0930_2f2eb8[v->area.s.path];
        k = FUN_L00_00259740_2f2eb8(&target, path + 4, path[0]);
        if (k) {
            goto in_range;
        }
    }
    if (v->area.s.volume != -1 && FUN_00214720_2f2eb8(&target, v->area.s.volume)) {
        goto in_range;
    }
    if (v->area.both != -1) {
        return;
    }
in_range:
    d = FUN_001f9b80_2f2eb8(&m->pos, &target);
    if (d < hero.unk2044) {
        hero.unk2040 = m;
        hero.unk2044 = d;
        if (d < 20.0f) {
            hero.unk2048 = 1;
        }
    }
}
/* Builds two scaled offset vectors from the moby's matrix and adds them to out. */
/* Ported from rac1-decomp (src/overlays/l03_kerwan/vendor_00293720.c: func_L03_0029DA60), where it is exact; names translated to the US level program. */


extern void FUN_001f9d20_q(void *, void *, void *) __asm__("FUN_001f9d20");
extern void FUN_001fa050_q(float *, float *) __asm__("FUN_001fa050");
extern void add_vector_xyz_q(void *, void *, void *) __asm__("FUN_001f9a10");
extern void cross_vectors_xyz_q(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void scale_vector_xyz_q(void *out, void *a, f32 s) __asm__("FUN_001f9a68");

void FUN_L01_002f5040(char *moby, float *out) {
    float v[4];
    float a[4];
    float c[4];
    float d[4];
    float b[4];
    float m[16];
    char *data;
    *(OvlQuad *)v = 0;
    v[0] = 1.0f;
    data = *(char **)(moby + 0x78);
    if (*(float *)(data + 0x130) != 0.0f || *(float *)(data + 0x134) != 0.0f) {
        FUN_001fa050_q(m, (float *)(moby + 0x40));
        FUN_001f9d20_q(a, v, m);
        *(OvlQuad *)b = *(OvlQuad *)a;
        b[2] = b[2] - 1.0f;
        cross_vectors_xyz_q(c, a, b);
        cross_vectors_xyz_q(d, a, c);
        scale_vector_xyz_q(c, c, *(float *)(data + 0x130));
        scale_vector_xyz_q(d, d, *(float *)(data + 0x134));
        add_vector_xyz_q(out, out, c);
        add_vector_xyz_q(out, out, d);
    }
}
#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L01_002f5168.s", FUN_L01_002f5168);
#else
extern float FUN_00222150_2f5168(short) __asm__("FUN_001fa6c0");
extern int FUN_L01_00275690(float, char *);
extern void FUN_L00_0026e150_2f5168(void *, int, int, int) __asm__("FUN_L00_002591d0");
extern void FUN_L00_0026e238_2f5168(void *, char *, int, int) __asm__("FUN_L00_002592b8");
extern short FUN_00220e30_2f5168(int) __asm__("FUN_001f96f8");
extern int FUN_00220ea8_2f5168(void *) __asm__("FUN_001f9770");
extern float FUN_00221360_2f5168(void *, void *) __asm__("FUN_001f9b48");
extern float FUN_002217c0_2f5168(float, float) __asm__("FUN_001f9e90");
extern float FUN_00221710_2f5168(float) __asm__("FUN_001f9de0");
extern float FUN_002216f8_2f5168(float) __asm__("FUN_001f9dc8");
extern float FUN_00222040_2f5168(float, float) __asm__("FUN_001fa5c8");
extern float FUN_00221ff8_2f5168(float, float) __asm__("FUN_001fa580");
extern float FUN_002731d0_2f5168(float) __asm__("FUN_L00_0025e310");
extern float FUN_0026cef0_2f5168(float, float, float, float, float, float *) __asm__("FUN_L00_00258110");
extern float FUN_0026c9c8_2f5168(float, float) __asm__("FUN_002132a8");
extern float FUN_0026ca90_2f5168(void) __asm__("FUN_00213308");
extern int FUN_0026c930_2f5168(int) __asm__("FUN_00213260");
extern int FUN_0026c970_2f5168(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_002a12f0_2f5168(char *, short) __asm__("FUN_L00_0028d8c0");
extern short FUN_002a1618_2f5168(int, int, char *) __asm__("FUN_0022da68");
extern void FUN_002a1348_2f5168(short) __asm__("FUN_0022d798");
extern void FUN_002211b8_2f5168(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_00221188_2f5168(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_00221170_2f5168(void *) __asm__("FUN_001f99f8");
extern void FUN_00221210_2f5168(void *, void *, float) __asm__("FUN_001f9a68");
extern void FUN_00221410_2f5168(void *, void *, float) __asm__("FUN_001f9bf8");
extern float FUN_00221318_2f5168(void *) __asm__("FUN_001f9b20");
extern void FUN_L01_0028b410(char *);
extern int FUN_L01_0028b510(void *, void *, float);
extern int FUN_L01_0028b878(char *);
extern int FUN_L01_0028b828(void *, int);
extern void FUN_L01_0028b8c8(void *, void *, int, float, float);
extern void FUN_L01_0028ba80(float, void *, void *, void *, void *, void *);
extern float FUN_L01_0028bb90(void *, int, void *, float, float);
extern void FUN_L01_0027f660(void *, void *);
extern void FUN_002645a8_2f5168(char *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void *FUN_0026f320_2f5168(char *, int, int) __asm__("FUN_L00_0025a420");
extern void FUN_00273310_2f5168(void *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern int FUN_00222160_2f5168(float) __asm__("FUN_001fa6d0");
extern void FUN_0020cc78_2f5168(void *, short, int) __asm__("FUN_001e93b0");
extern int D_L01_00161B00 __attribute__((sda));
extern int level_frame_count __asm__("D_L01_0015F5CC");
#define D_L01_0015ED84 (*(int *)0x0015ed84)
extern char *D_L01_0015FFD8 __asm__("D_L01_0015FFD8");
extern char *D_L01_001B0930[];
extern void (*D_L01_00160770)(char *);

#define F2(p, o) (*(float *)((p) + (o)))
#define I2(p, o) (*(int *)((p) + (o)))
#define H2(p, o) (*(short *)((p) + (o)))
#define UH2(p, o) (*(unsigned short *)((p) + (o)))
#define B2(p, o) (*(unsigned char *)((p) + (o)))
#define Q2(p, o) (*(OvlQuad *)((p) + (o)))

void gunship_follow_path(char *moby) __asm__("FUN_L01_002f5168");
void gunship_follow_path(char *moby) {
    char *data;
    char *path;
    char *child;
    char *child_data;
    char *current;
    char *node;
    char *entry;
    struct { int index; signed char direction; unsigned char state; short pad; float progress, distance; char *path; int path_id; float angle; int unused; } *path_state;
    int *link;
    int i, j, count, index, state;
    int link_offset;
    int hit;
    int found;
    short type;
    float temp[4];
    float zero[4];
    float rotation[4], position[4];
    union { float point[16]; struct { float first[4], work[4], offset[4], last[4]; } parts; } vectors;
    float work_init[4];
    char iterator[16];
#define point vectors.point
#define work vectors.parts.work
#define offset vectors.parts.offset
    float speed, fraction, distance, angle, scale;

    Q2(zero, 0) = 0;
    found = 0;
    data = *(char **)(moby + 0x78);
    if (H2(moby, 0xa6) != 0x336) {
        found = FUN_L01_00275690(FUN_00222150_2f5168(H2(moby, 0x32)), moby) == -1;
    }
    if (data == 0 || D_L01_00161B00 != 0) return;
    path_state = (void *)(data + 0x60);
    if (B2(moby, 0x21) != 0xff && B2(moby, 0x20) != 0 && B2(moby, 0x20) != 3) {
        int valid = 0;
        FUN_L00_0026e150_2f5168(iterator, B2(moby, 0x21), 0, 0);
        if (*(char **)iterator == moby) {
            while ((current = *(char **)iterator) != 0) {
                if ((unsigned int)FUN_L01_00275690(FUN_00222150_2f5168(H2(current, 0x32)), current) + 1 != 0) {
                    valid = 1;
                    break;
                }
                FUN_L00_0026e238_2f5168(iterator, *(char **)iterator, 0, 0);
            }
            if (!valid) {
                FUN_L00_0026e150_2f5168(iterator, B2(moby, 0x21), 0, 0);
                while (*(char **)iterator != 0) {
                    child_data = *(char **)(*(char **)iterator + 0x78);
                    B2(*(char **)iterator, 0x20) = 3;
                    H2(child_data, 0x11c) = FUN_00220e30_2f5168(5);
                    FUN_L00_0026e238_2f5168(iterator, *(char **)iterator, 0, 0);
                }
            }
        }
    }
    type = H2(moby, 0xa6);
    if ((type == 0x33 || type == 0x46a) || (type == 0x473 || type == 0x474)) {
        if ((level_frame_count & 7) == (((int)moby >> 8) & 7) && H2(data, 0x13a) <= 0 && B2(moby, 0x20) != 0) {
            if (FUN_002a12f0_2f5168(moby, H2(data, 0x11e)) == 0)
                H2(data, 0x11e) = FUN_002a1618_2f5168(0, 4, moby);
        }
    }
    switch ((short)(UH2(moby, 0xa6) - 0x33)) {
    case 0: case 9: case 0x17:
        if (H2(data, 0x138) != -1) {
            if (FUN_00220ea8_2f5168(data + 0x138) == 0) break;
            if (FUN_00221360_2f5168(moby + 0x10, D_L01_00167240) < 30.0f)
                FUN_002a1618_2f5168(1, 0, moby);
        }
        goto reset_timer;
    case 0xd: case 0x11:
        if (H2(data, 0x138) == -1) goto reset_timer;
        if (FUN_00220ea8_2f5168(data + 0x138) == 0) break;
        FUN_002a1618_2f5168((FUN_0026c930_2f5168(100) & 1) ? 1 : 2, 0, moby);
reset_timer:
        H2(data, 0x138) = FUN_00220e30_2f5168(FUN_0026c970_2f5168(H2(data, 0x13c), H2(data, 0x13c) + H2(data, 0x13e)));
        break;
    default: break;
    }
    switch (B2(moby, 0x20)) {
    case 0:
        if (path_state->path_id != -1) {
            path = D_L01_001B0930[path_state->path_id];
            if (FUN_00221360_2f5168(path + 0x10, path + (*(int *)path << 4)) < 0.5f) --*(int *)path;
        } else {
            path = (char *)0x0020b3e0;
            /* Retail uses an absolute load of D_L01_0015FFD8 here, despite its
             * small-data declaration used by the other accesses in this file. */
            index = FUN_00222160_2f5168((float)(((int)moby - I2((char *)0x0015ffd8, 0)) >> 8));
            type = H2(moby, 0xa6);
            FUN_0020cc78_2f5168(path, type, index);
        }
        B2(moby, 0x30) = 0xff;
        H2(data, 0x11e) = -1;
        if (F2(data, 0x130) > 500.0f) F2(data, 0x130) = FUN_0026c9c8_2f5168(-1.0f, 1.0f) * F2(data, 0x130) / 1000.0f;
        if (F2(data, 0x134) > 500.0f) F2(data, 0x134) = FUN_0026c9c8_2f5168(-1.0f, 1.0f) * F2(data, 0x134) / 1000.0f;
        if (path_state->path_id == -1) { B2(moby, 0x20) = 2; return; }
        B2(data, 0x121) = (((((int)moby - (int)(*(int *)&D_L01_0015FFD8)) >> 8) + (int)((unsigned int)((int)moby - (int)(*(int *)&D_L01_0015FFD8)) >> 31)) >> 1) % 7;
        FUN_L01_0028b410((char *)path_state);
        if (path_state->state == 0) {
            if (path_state->index <= 0) {
                path_state->index = FUN_L01_0028b510(moby + 0x10, path_state->path, 0.0f);
                if (path_state->direction < 0) {
                    if (path_state->index == 0) path_state->index = 0;
                } else if (path_state->index == *(int *)path_state->path - 1) path_state->index = 0;
            }
            path_state->distance = FUN_00221360_2f5168(moby + 0x10, path_state->path + (path_state->index << 4) + 0x10);
            node = (char *)((path_state->index << 4) + (int)path_state->path);
            path_state->angle = FUN_002217c0_2f5168(F2(node, 0x10) - F2(moby, 0x10), F2(node, 0x14) - F2(moby, 0x14));
            path_state->state = 2;
            path_state->progress = 0;
            path_state->unused = 0;
        }
        node = (char *)((path_state->index << 4) + (int)path_state->path);
        Q2(data, 0xe0) = Q2(node, 0x10);
        F2(data, 0xf0) = 1.0f;
        FUN_L01_0028b8c8(data + 0xa0, path_state, 0, F2(data, 0xf4), F2(data, 0xf8));
        node = (char *)((path_state->index << 4) + (int)path_state->path);
        Q2(temp, 0) = Q2(node, 0x10);
        Q2(data, 0xb0) = Q2(temp, 0);
        Q2(data, 0xc0) = Q2(temp, 0);
        F2(moby, 0x40) = 0.0f;
        F2(moby, 0x44) = -FUN_002217c0_2f5168(FUN_00221318_2f5168(data + 0xa0), F2(data, 0xa8));
        F2(moby, 0x48) = FUN_002217c0_2f5168(F2(data, 0xa0) - zero[0], F2(data, 0xa4) - zero[1]);
        I2(data, 0x104) = 0;
        I2(data, 0x10c) = 0;
        Q2(moby, 0x10) = Q2(data, 0xe0);
        FUN_L01_002f5040(moby, (float *)(moby + 0x10));
        B2(data, 0x121) = 7;
        if (F2(data, 0x12c) > 6.2831855f) F2(data, 0x12c) *= 0.017453292f;
        else if (F2(data, 0x124) != 0.0f && F2(data, 0x12c) == -1.0f) F2(data, 0x12c) = FUN_0026ca90_2f5168();
        Q2(data, 0xd0) = Q2(data, 0xc0);
        FUN_L01_0028bb90(path_state, B2(data, 0x120) & 0x20, moby, F2(data, 0xf4), F2(data, 0xf8));
        if ((B2(data, 0x120) & 0x1e) && !(B2(data, 0x120) & 1)) {
            i = 0;
            node = (char *)point;
            link = (int *)(data + 0x140);
            for (; i < 4; ++i, node += 0x10, ++link) {
                if ((B2(data, 0x120) & (2 << i)) && *link != -1) {
                    child = (*(int *)&D_L01_0015FFD8) + (*link << 8);
                    if (child && (child_data = *(char **)(child + 0x78)) != 0) {
                        I2(child_data, 0x70) |= 0x1000;
                        FUN_002645a8_2f5168(moby, i, node);
                        FUN_002211b8_2f5168(work_init, node, moby + 0x10);
                        FUN_00221410_2f5168(work_init, work_init, 1.0f);
                        Q2(child, 0x10) = *(OvlQuad *)node;
                        Q2(child_data, 0x80) = *(OvlQuad *)work_init;
                    }
                }
            }
        }
        if (H2(data, 0x11c) > 0) {
            B2(moby, 0x20) = 3;
            return;
        }
        if (D_L01_0015ED84 == 3 || D_L01_0015ED84 == 9) H2(data, 0x13a) = 0;
        B2(moby, 0x20) = 1;
        return;
    case 2:
        if (path_state->path_id == -1) return;
        B2(moby, 0x20) = 0;
        {
            short index = H2(data, 0x11e);
            if (index != -1 && (entry = D_0013E550 + index * 0x70, *(char **)(entry + 0x88) == moby) && B2(entry, 0x74))
                FUN_002a1348_2f5168(index);
        }
        H2(data, 0x11e) = -1;
        return;
    case 3:
        if (!FUN_00220ea8_2f5168(data + 0x11c)) return;
        H2(data, 0x11c) = 0;
        B2(moby, 0x20) = 1;
        return;
    case 1:
        break;
    default: return;
    }
    Q2(rotation, 0) = Q2(moby, 0x40);
    Q2(position, 0) = Q2(moby, 0x10);
    if (F2(data, 0xf0) >= 1.0f) {
        F2(data, 0xf0) -= 1.0f;
        index = FUN_L01_0028b878((char *)path_state);
        Q2(data, 0x90) = Q2(data, 0xa0);
        FUN_L01_0028b8c8(data + 0xa0, path_state, 1, F2(data, 0xf4), F2(data, 0xf8));
        Q2(data, 0xb0) = Q2(data, 0xc0);
        index = FUN_L01_0028b828(path_state, index + 1);
        Q2(data, 0xc0) = Q2(path_state->path + (index << 4), 0x10);
    }
    speed = F2(data, 0xfc) + F2(data, 0x124) * frame_scale * FUN_00221710_2f5168(F2(data, 0x12c)) * frame_scale;
    fraction = speed / F2(data, 0xbc);
    F2(data, 0x12c) = FUN_00221ff8_2f5168(F2(data, 0x12c), F2(data, 0x128) * frame_scale * 0.017453292f);
    Q2(data, 0xe0) = Q2(data, 0xd0);
    FUN_L01_0028ba80(F2(data, 0xf0) + fraction, data + 0xd0, data + 0xb0, data + 0xc0, data + 0x90, data + 0xa0);
    if (!(B2(data, 0x120) & 0x40)) {
        distance = FUN_00221360_2f5168(data + 0xe0, data + 0xd0);
        if (distance != 0.0f) {
            fraction *= speed / distance;
            FUN_L01_0028ba80(F2(data, 0xf0) + fraction, data + 0xd0, data + 0xb0, data + 0xc0, data + 0x90, data + 0xa0);
        }
    }
    if (!found) {
        angle = (float)H2(data, 0x122) * 0.017453292f;
        FUN_00222040_2f5168(F2(moby, 0x48), angle);
        scale = FUN_00222040_2f5168(FUN_00221ff8_2f5168(FUN_002217c0_2f5168(F2(data, 0xd0) - F2(data, 0xe0), F2(data, 0xd4) - F2(data, 0xe4)), angle), F2(moby, 0x48)) * ((1.0f / F2(data, 0x110)) * frame_scale);
        F2(moby, 0x48) = FUN_00221ff8_2f5168(scale, F2(moby, 0x48));
        FUN_002211b8_2f5168(temp, data + 0xd0, data + 0xe0);
        angle = FUN_002217c0_2f5168(FUN_00221318_2f5168(temp), temp[2]);
        F2(moby, 0x44) = FUN_0026cef0_2f5168(F2(moby, 0x44), -angle, F2(data, 0x108) * 10.0f * 0.017453292f * frame_time_sq, F2(data, 0x108) * 5.0f * 0.017453292f * frame_time_sq, F2(data, 0x108) * 0.017453292f * frame_time, (float *)(data + 0x10c));
        angle = FUN_002731d0_2f5168(scale * F2(data, 0x114));
        angle = FUN_00222040_2f5168(F2(moby, 0x40), angle);
        angle = FUN_002731d0_2f5168(angle * F2(data, 0x118));
        F2(moby, 0x40) = FUN_0026cef0_2f5168(F2(moby, 0x40), angle, F2(data, 0x100) * 10.0f * 0.017453292f * frame_time_sq, F2(data, 0x100) * 5.0f * 0.017453292f * frame_time_sq, F2(data, 0x100) * 0.017453292f * frame_time, (float *)(data + 0x104));
    } else {
        FUN_00221170_2f5168(moby + 0x40);
        F2(moby, 0x48) = FUN_002217c0_2f5168(F2(data, 0xd0) - F2(data, 0xe0), F2(data, 0xd4) - F2(data, 0xe4));
    }
    F2(data, 0xf0) += fraction;
    Q2(moby, 0x10) = Q2(data, 0xe0);
    if (!found) FUN_L01_002f5040(moby, (float *)(moby + 0x10));
    if (B2(data, 0x120) & 0x1f) {
        count = 0;
        i = 0;
        link_offset = -4;
        do {
            if (B2(data, 0x120) & (1 << i)) {
                FUN_002645a8_2f5168(moby, count++, point);
                FUN_002211b8_2f5168(work, point, moby + 0x10);
                if (!(B2(data, 0x120) & 1)) {
                    j = *(int *)(data + link_offset + 0x140);
                    if (j != -1) {
                        child = (*(int *)&D_L01_0015FFD8) + (j << 8);
                        if (child && (child_data = *(char **)(child + 0x78)) != 0) {
                            FUN_00221410_2f5168(work, work, 1.0f);
                            I2(child_data, 0x70) |= 0x1000;
                            Q2(child, 0x10) = *(OvlQuad *)point;
                            Q2(child_data, 0x80) = *(OvlQuad *)work;
                        }
                    }
                } else if (B2(data, 0x121) == 0) {
                    scale = FUN_0026c9c8_2f5168(0.016f, 0.031f) * frame_scale;
                    FUN_00221410_2f5168(work, work, scale);
                    FUN_L01_0027f660(point, work);
                    scale = FUN_0026c9c8_2f5168(0.016f, 0.031f) * frame_scale;
                    FUN_00221410_2f5168(work, work, scale);
                    FUN_L01_0027f660(point, work);
                }
            }
            ++i;
            link_offset += 4;
        } while (i < 5);
        if (B2(data, 0x121) == 0) B2(data, 0x121) = FUN_00220e30_2f5168(7);
        B2(data, 0x121)--;
    }
    if (D_L01_0015ED84 != 3 && D_L01_0015ED84 != 9) return;
    {
        char *current = FUN_0026f320_2f5168(moby, 0x210000, 0);
        if (current && UH2(current, 0x28) != 0x102) {
            FUN_00221170_2f5168(point);
            if (H2(moby, 0xa6) == 0x33) {
                point[0] = FUN_002216f8_2f5168(F2(moby, 0x48)) * -2.747f;
                point[1] = FUN_00221710_2f5168(F2(moby, 0x48)) * -2.747f;
                point[2] = 0.0f;
            }
            FUN_002211b8_2f5168(offset, moby + 0x10, position);
            *(OvlQuad *)work = *(OvlQuad *)offset;
            FUN_00221210_2f5168(work, work, 0.5f);
            FUN_00221188_2f5168(point, point, moby + 0x10);
            H2(data, 0x13a) = FUN_00220e30_2f5168(600);
            FUN_00273310_2f5168(moby, work, point, 0.0f, 0.0f, 0x14, 3, 4, 4.0f, 2.0f, 100000.0f, 3.0f, (H2(moby, 0xa6) == 0x532 || H2(moby, 0xa6) == 0x53b || H2(moby, 0xa6) == 0x53c) ? 1 : -1, 15.0f, 1, 1, -1, 0);
        }
    }
    D_L01_00160770 = gunship_follow_path;
    B2(moby, 0xa4) = 0xff;
    if (H2(data, 0x13a) == 0) return;
    hit = FUN_00220ea8_2f5168(data + 0x13a);
    if (hit && found) {
        I2(moby, 0x94) = I2(*(char **)(moby + 0x24), 0x10);
        UH2(moby, 0x34) = (UH2(moby, 0x34) & 0xffbe) | 0x1000;
        return;
    }
    I2(moby, 0x94) = 0;
    UH2(moby, 0x34) |= 0x41;
    {
        short index = H2(data, 0x11e);
        if (index != -1 && (entry = D_0013E550 + index * 0x70, *(char **)(entry + 0x88) == moby) && B2(entry, 0x74))
            FUN_002a1348_2f5168(index);
    }
    H2(data, 0x11e) = -1;
    UH2(moby, 0x34) &= 0xefff;

}
#undef point
#undef work
#undef offset

#undef F2
#undef I2
#undef H2
#undef UH2
#undef B2
#undef Q2
#endif

#include "rnc/rendering/dma_tag.h"
extern void FUN_L01_00261c10(void *, void *) __asm__("FUN_L01_00261c10");
extern void FUN_L01_00261df8(f32 *) __asm__("FUN_L01_00261df8");
extern void FUN_L01_002b80d0(f32 *) __asm__("FUN_L01_002b80d0");
extern u64 get_effect_texture(s32) __asm__("FUN_001f44b8");

void FUN_L01_002b91c8(f32 *pos, s32 count) {
    f32 corners[4][4];
    f32 near_clip[4];
    f32 far_clip[4];
    f32 x_offset, y_offset, x_end, y_end, z;
    u32 near_bits, far_bits;
    s32 mask;
    s32 i, j, mode;
    s32 height_index, index;
    f32 *height;
    u32 *packet;
    u64 *data;

    FUN_L01_002b8f98();
    i = 0;
    if (count > 0) do {
        f32 *next_pos = pos + 0x464;
        u16 active = *(u16 *)((u8 *)pos + 0x1e);
        if (active == 0) goto next_entity;
        x_offset = pos[0] + pool_light[2];
        x_end = pos[0] - pool_light[2];
        corners[0][0] = x_offset;
        y_offset = pos[1] + pool_light[3];
        y_end = pos[1] - pool_light[3];
        corners[0][1] = y_offset;
        z = pos[2];
        corners[0][2] = z;
        corners[0][3] = 1.0f;
        corners[1][0] = x_end;
        corners[1][1] = y_offset;
        corners[1][2] = z;
        corners[1][3] = 1.0f;
        corners[2][0] = x_offset;
        corners[2][1] = y_end;
        corners[2][2] = z;
        corners[2][3] = 1.0f;
        corners[3][0] = x_end;
        corners[3][1] = y_end;
        corners[3][2] = z;
        corners[3][3] = 1.0f;
        near_bits = 0;
        far_bits = 0;
        for (j = 0; j < 4; j++) {
            subtract_vector_xyz(near_clip, corners[j], D_L01_00167240);
            scale_vector_xyz(far_clip, near_clip, 1024.0f);
            scale_vector_xyz(near_clip, near_clip, 1024.0f);
            transform_vector(far_clip, far_clip, D_L01_00167240 - 0x80);
            transform_vector(near_clip, near_clip, D_L01_00167240 - 0x40);
            far_bits = (far_bits << 8) | is_vector_outside_clip(far_clip);
            near_bits = (near_bits << 8) | is_vector_outside_clip(near_clip);
        }
        if ((near_bits & 0x01010101) == 0x01010101) goto next_entity;
        if ((near_bits & 0x02020202) == 0x02020202) goto next_entity;
        if ((near_bits & 0x04040404) == 0x04040404) goto next_entity;
        if ((near_bits & 0x08080808) == 0x08080808) goto next_entity;
        if ((near_bits & 0x20202020) == 0x20202020) goto next_entity;

        FUN_L01_00261c10((void *)0x70000000, (u8 *)pos + (D_L01_00161220 * 0x5c0 + 0x50));
        FUN_L01_00261df8(pos);
        FUN_L01_002b8c08(pos);
        FUN_L01_002b80d0(pos);
        if (*(u32 *)(pos + 0x11) & 1) {
            height = (f32 *)0x70002008;
            for (height_index = 0x10; height_index >= 0; height_index--, height += 3) *height = pos[2];
        }
        if (*(u32 *)(pos + 0x11) & 2) {
            height = (f32 *)0x700020c8;
            for (height_index = 0x10; height_index >= 0; height_index--, height += 0x33) *height = pos[2];
        }
        if (*(u32 *)(pos + 0x11) & 4) {
            height = (f32 *)0x70002cc8;
            for (height_index = 0x10; height_index >= 0; height_index--, height += 3) *height = pos[2];
        }
        if (*(u32 *)(pos + 0x11) & 8) {
            height = (f32 *)0x70002008;
            for (height_index = 0x10; height_index >= 0; height_index--, height += 0x33) *height = pos[2];
        }
        mode = (far_bits & 0x2f2f2f2f) != 0;
        if (!mode) {
            mode = 2;
            if (!(near_bits & 0x2f2f2f2f)) mode = 0;
        }
        mask = *(u16 *)((u8 *)pos + 0x1e);
        if (*(u32 *)(pos + 5) != D_L01_00161228 || *(u32 *)(pos + 6) != D_L01_0016122C ||
            *(u8 *)(pos + 7) != D_L01_00161230 || *(u8 *)((u8 *)pos + 0x1d) != D_L01_00161234) {
            l01_packet.words[0] = 0x10000005;
            l01_packet.words[1] = 0;
            l01_packet.words[2] = 0x11000000;
            l01_packet.words[3] = 0x50000005;
            packet = l01_packet.words;
            l01_packet.words += 4;
            data = (u64 *)(packet + 4);
            data[0] = 0x4000000000008001ULL;
            data[1] = 0xeeee;
            data[2] = ((u64)*(u8 *)((u8 *)pos + 0x1d) << 32) | 100;
            data[3] = 0x42;
            data[4] = ((u64)*(u8 *)(pos + 7) << 32) | 100;
            data[5] = 0x43;
            data[6] = get_effect_texture(*(s32 *)(pos + 6));
            data[7] = 6;
            data[8] = get_effect_texture(*(s32 *)(pos + 5));
            data[9] = 7;
            l01_packet.words = packet + 0x18;
            {
                u32 texture_id = *(u32 *)(pos + 5);
                u32 second_texture_id = *(u32 *)(pos + 6);
                D_L01_00161228 = texture_id;
                D_L01_0016122C = second_texture_id;
            }
            D_L01_00161230 = *(u8 *)(pos + 7);
            D_L01_00161234 = *(u8 *)((u8 *)pos + 0x1d);
        }
        for (index = 0; index < 16; index++, mask = (s32)mask >> 1) {
            if (mask & 1) FUN_L01_002b8cb0(pos, index, mode);
        }
next_entity:
        i++;
        pos = next_pos;
    } while (i < count);
    l01_packet.words[0] = 0x10000000;
    l01_packet.words[1] = 0;
    l01_packet.words[2] = 0x13000000;
    l01_packet.words[3] = 0;
    l01_packet.words += 4;
}
