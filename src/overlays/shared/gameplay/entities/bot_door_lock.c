/* Shared moby classes: bot_door_lock, mushroom. */
#include "types.h"
#include "asm.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/overlays/asm/FUN_L06_00307578.s", FUN_L06_00307578);
#else
#include "rnc/rendering/geometry_quad.h"
#include "rnc/overlay/quad.h"
extern u64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L06_00167400[];
extern OvlVec4 D_L06_001fe8d0[];
extern float D_L06_001fe960[][2];
extern u32 D_L06_001fe9a8[];
extern int D_L06_001fe9d0[];
void FUN_L06_00307578(char *moby) {
    struct GeometryQuad quad;
    OvlVec4 result[4];
    float rotation[3];
    float matrix[12];
    OvlVec4 pos[4];
    OvlVec4 corners[4];
    volatile int i;
    OvlVec4 *pos0, *pos1, *pos2, *pos3;
    OvlVec4 *result0, *result1, *result2, *result3;
    float *data = *(float **)(moby + 0x78);
    int *indices;
    int *index1, *index2;
    OvlVec4 *vertices;
    float (*uvs)[2];
    u32 *colors;
    rotation[0] = 0.0f; rotation[1] = 0.0f; rotation[2] = D_L06_00167400[86];
    euler_rotation_basis(matrix, rotation);
    data[7] += 0.01f;
    if (1.0f < data[7]) data[7] -= 1.0f;
    quad.texture_state = 1;
    *(volatile u64 *)&quad.primitive = 0x4000000044ULL;
    quad.reserved.value = 0;
    indices = D_L06_001fe9d0;
    i = 0;
    quad.texture = get_effect_texture(0x18);
    result0 = &result[0];
    result1 = &result[1];
    result2 = &result[2];
    result3 = &result[3];
    pos0 = &pos[0];
    pos1 = &pos[1];
    pos2 = &pos[2];
    pos3 = &pos[3];
    vertices = D_L06_001fe8d0;
    uvs = D_L06_001fe960;
    colors = D_L06_001fe9a8;
    do {
        volatile int offset = i * 4;
        i++;
        transform_vector_by_basis(result0, &vertices[indices[0]], matrix);
        index1 = D_L06_001fe9d0 + 1 + offset;
        transform_vector_by_basis(result1, &vertices[*index1], matrix);
        index2 = D_L06_001fe9d0 + 2 + offset;
        transform_vector_by_basis(result2, &vertices[*index2], matrix);
        transform_vector_by_basis(result3, &vertices[indices[3]], matrix);
        quad.positions[0].f[0] = *(float *)(moby + 16) + result[0].f[0] * data[9];
        quad.positions[0].f[1] = *(float *)(moby + 20) + result[0].f[1] * data[9];
        quad.positions[0].f[2] = *(float *)(moby + 24) + result[0].f[2];
        quad.positions[1].f[0] = *(float *)(moby + 16) + result[1].f[0] * data[9];
        quad.positions[1].f[1] = *(float *)(moby + 20) + result[1].f[1] * data[9];
        quad.positions[1].f[2] = *(float *)(moby + 24) + result[1].f[2];
        quad.positions[2].f[0] = *(float *)(moby + 16) + result[2].f[0] * data[9];
        quad.positions[2].f[1] = *(float *)(moby + 20) + result[2].f[1] * data[9];
        quad.positions[2].f[2] = *(float *)(moby + 24) + result[2].f[2];
        quad.positions[3].f[0] = *(float *)(moby + 16) + result[3].f[0] * data[9];
        quad.positions[3].f[1] = *(float *)(moby + 20) + result[3].f[1] * data[9];
        quad.positions[3].f[2] = *(float *)(moby + 24) + result[3].f[2];
        quad.colors[0] = colors[indices[0]];
        quad.colors[1] = colors[*index1];
        quad.colors[2] = colors[*index2];
        quad.colors[3] = colors[indices[3]];
        quad.texture_coordinates[0][0] = uvs[indices[0]][0];
        quad.texture_coordinates[0][1] = uvs[indices[0]][1] + data[7];
        quad.texture_coordinates[1][0] = uvs[*index1][0];
        quad.texture_coordinates[1][1] = uvs[*index1][1] + data[7];
        quad.texture_coordinates[2][0] = uvs[*index2][0];
        quad.texture_coordinates[2][1] = uvs[*index2][1] + data[7];
        quad.texture_coordinates[3][0] = uvs[indices[3]][0];
        quad.texture_coordinates[3][1] = uvs[indices[3]][1] + data[7];
        indices += 4;
        draw_geometry_quad(&quad, 0, 1);
    } while (i < 4);
    quad.texture = get_effect_texture(0x1b);
    data[8] += 0.01f;
    if (1.0f < data[8]) data[8] = 0.0f;
    corners[0].q = 0;
    corners[0].f[0] = -data[8] * 1.2f * data[9];
    corners[0].f[1] = -data[8] * 1.2f * data[9];
    corners[0].f[2] = data[8] * 1.4f + 1.1f;
    corners[1].q = 0;
    corners[1].f[0] = data[8] * 1.2f * data[9];
    corners[1].f[1] = -data[8] * 1.2f * data[9];
    corners[1].f[2] = data[8] * 1.4f + 1.1f - 0.2f;
    corners[2].q = 0;
    corners[2].f[0] = -data[8] * 1.2f * data[9];
    corners[2].f[1] = data[8] * 1.2f * data[9];
    corners[2].f[2] = data[8] * 1.4f + 1.1f;
    corners[3].q = 0;
    corners[3].f[0] = data[8] * 1.2f * data[9];
    corners[3].f[1] = data[8] * 1.2f * data[9];
    corners[3].f[2] = data[8] * 1.4f + 1.1f - 0.2f;
    pos[0].q = corners[0].q;
    pos[1].q = corners[1].q;
    pos[2].q = corners[2].q;
    pos[3].q = corners[3].q;
    rotation[0] = 0.0f; rotation[1] = 0.0f; rotation[2] = D_L06_00167400[86];
    euler_rotation_basis(matrix, rotation);
    transform_vector_by_basis(result0, pos0, matrix);
    transform_vector_by_basis(result1, pos1, matrix);
    transform_vector_by_basis(result2, pos2, matrix);
    transform_vector_by_basis(result3, pos3, matrix);
    quad.positions[0].f[0] = *(float *)(moby + 16) + result[0].f[0];
    quad.positions[0].f[1] = *(float *)(moby + 20) + result[0].f[1];
    quad.positions[0].f[2] = *(float *)(moby + 24) + result[0].f[2];
    quad.positions[1].f[0] = *(float *)(moby + 16) + result[1].f[0];
    quad.positions[1].f[1] = *(float *)(moby + 20) + result[1].f[1];
    quad.positions[1].f[2] = *(float *)(moby + 24) + result[1].f[2];
    quad.positions[2].f[0] = *(float *)(moby + 16) + result[2].f[0];
    quad.positions[2].f[1] = *(float *)(moby + 20) + result[2].f[1];
    quad.positions[2].f[2] = *(float *)(moby + 24) + result[2].f[2];
    quad.positions[3].f[0] = *(float *)(moby + 16) + result[3].f[0];
    quad.positions[3].f[1] = *(float *)(moby + 20) + result[3].f[1];
    quad.positions[3].f[2] = *(float *)(moby + 24) + result[3].f[2];
    quad.colors[0] = ((u32)(int)((1.0f - data[8]) * 128.0f) << 24) | 0x808080;
    quad.colors[3] = quad.colors[2] = quad.colors[1] = quad.colors[0];
    quad.texture_coordinates[0][0] = 0.0f; quad.texture_coordinates[0][1] = 0.0f;
    quad.texture_coordinates[1][0] = 1.0f; quad.texture_coordinates[2][1] = 1.0f;
    quad.texture_coordinates[3][0] = 1.0f; quad.texture_coordinates[3][1] = 1.0f;
    quad.texture_coordinates[1][1] = 0.0f; quad.texture_coordinates[2][0] = 0.0f;
    draw_geometry_quad(&quad, 0, 1);
}
#endif
#define NOT_SDA

#define MACRO_ADDR

/* state transitions of a moby depending on its id at 0xA6 */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00309088), where it is exact; names translated to the US level program. */

extern int trooper_play_sound(int i, int a1, int v, int k) __asm__("FUN_L00_0028dc90");
extern void allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");
extern void jet_clear_persistent(int idx) __asm__("FUN_L00_002502a0");

void FUN_L06_00307c48(unsigned char *a, unsigned char *m) {
    switch (*(short *)(m + 0xA6)) {
    case 0x3F7:
    case 0x502:
        if (m[0x20] == 2) {
            m[0x20] = 3;
            allocate_voice_for_target_entry(0, 0, (int)m);
        }
        break;
    case 0x404:
        if (m[0x20] == 1)
            m[0x20] = 2;
        break;
    case 0x542:
        m[0x20] = 2;
        trooper_play_sound(0, 0, (int)m, 0x434);
        m[0x30] = 0xFF;
        jet_clear_persistent(a[0xB0]);
        break;
    }
}
#include "rnc/overlay/quad.h"
extern void FUN_L06_00307578(char *moby);
extern float D_L06_001622DC __attribute__((sda));
extern float D_L06_001622E0 __attribute__((sda));
extern float D_L06_001622E4 __attribute__((sda));
extern float D_L06_001622E8 __attribute__((sda));
extern float D_L06_001622EC __attribute__((sda));
extern unsigned char D_001413F4 __attribute__((section(".data")));
extern unsigned char D_0015EDB4;
extern unsigned char D_0014C050[];
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern unsigned int fast_tween_color(float, int, int) __asm__("FUN_001fa6e0");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern float FUN_001f9b48(void *, void *);
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern void enqueue_callback_list_1(int, int) __asm__("FUN_001f4600");
extern void add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");
extern void draw_number(void *, void *, int, unsigned int, int, int, int, float) __asm__("FUN_L01_00285768");

extern int D_L06_0015FFD8_gp __asm__("D_L06_0015FFD8") __attribute__((sda));
void bot_door_lock_update(char *moby) __asm__("FUN_L06_00307d30");
void bot_door_lock_update(char *moby) {
    float *data = *(float **)(moby + 0x78);
    int *idata = (int *)data;
    float pos[4];
    float offset[3];
    float result[4];
    float angle;
    int value;
    unsigned int color;
    unsigned int color_channels;
    int i;

    if (*(int *)data == -1) return;
    data[10] = fast_add_rotations(data[10], (360.0f / D_L06_001622E4) * 0.017453292f * frame_time);
    *(unsigned int *)(moby + 0x90) = fast_tween_color((fast_cos(data[10]) + 1.0f) * 0.5f, 0x80464646, 0x80828282);
    switch ((unsigned char)moby[0x20]) {
    case 0:
        idata[5] = idata[4];
        data[6] = random_angle_radians();
        data[10] = random_angle_radians();
        moby[0x20] = 1;
        data[9] = D_L06_001622EC;
        return;
    case 1:
        *(OvlQuad *)pos = *(OvlQuad *)(moby + 0x10);
        pos[2] += 2.75f;
        if (FUN_001f9b48(D_L06_00167540, moby + 0x10) < 32.0f) {
            data[9] += frame_time + frame_time;
            if (D_L06_001622EC < data[9]) data[9] = D_L06_001622EC;
        } else if (36.0f < FUN_001f9b48(D_L06_00167540, moby + 0x10)) {
            data[9] -= frame_time + frame_time;
            if (data[9] < 0.0f) data[9] = 0.0f;
        }
        if (0.0f < data[9]) {
            enqueue_callback_list_1((int)FUN_L06_00307578, (int)moby);
            data[6] = fast_add_rotations(data[6], (360.0f / D_L06_001622E0) * 0.017453292f * frame_time);
            pos[2] += D_L06_001622DC * fast_cos(data[6]);
            value = truncate_float_to_s32(D_L06_001622E8 * fast_cos(data[10]));
            color_channels = (value / 2 + 120) * 0x100 | 0x20000000U;
            color = (value / 3 + 13) * 0x10000 | color_channels | (value + 150U);
            if (D_001413F4 == 1) {
                if (idata[5] < 10) {
                    draw_number(pos, moby, idata[5] + 2, color, 0x7f, 4, 0xff, 0.5f);
                } else {
                    if (D_0015EDB4) {
                        angle = fast_add_rotations(*(float *)(moby + 0x48), 1.5707964f);
                        offset[0] = fast_cos(angle) * 0.15f;
                        angle = fast_add_rotations(*(float *)(moby + 0x48), 1.5707964f);
                        offset[1] = fast_sin(angle) * 0.15f;
                        offset[2] = 0.0f;
                        add_vectors(result, offset, pos);
                        draw_number(offset, moby, idata[5] / 10 + 2, color, 0x7f, 4, 0xff, 0.5f);
                        angle = fast_add_rotations(*(float *)(moby + 0x48), 1.5707964f);
                        offset[0] = fast_cos(angle) * -0.4f;
                        angle = fast_add_rotations(*(float *)(moby + 0x48), 1.5707964f);
                        offset[1] = fast_sin(angle) * -0.4f;
                        offset[2] = 0.0f;
                        add_vectors(result, offset, pos);
                        draw_number(offset, moby, idata[5] % 10 + 2, color, 0x7f, 4, 0xff, 0.5f);
                    } else {
                        angle = fast_add_rotations(*(float *)(moby + 0x48), 1.5707964f);
                        offset[0] = fast_cos(angle) * -0.4f;
                        angle = fast_add_rotations(*(float *)(moby + 0x48), 1.5707964f);
                        offset[1] = fast_sin(angle) * -0.4f;
                        offset[2] = 0.0f;
                        add_vectors(result, offset, pos);
                        draw_number(offset, moby, idata[5] / 10 + 2, color, 0x7f, 4, 0xff, 0.5f);
                        angle = fast_add_rotations(*(float *)(moby + 0x48), 1.5707964f);
                        offset[0] = fast_cos(angle) * 0.15f;
                        angle = fast_add_rotations(*(float *)(moby + 0x48), 1.5707964f);
                        offset[1] = fast_sin(angle) * 0.15f;
                        offset[2] = 0.0f;
                        add_vectors(result, offset, pos);
                        draw_number(offset, moby, idata[5] % 10 + 2, color, 0x7f, 4, 0xff, 0.5f);
                    }
                }
            }
        }
        if (idata[5] < 1 || D_0014C050[(unsigned char)moby[0xb0] + current_level_index * 16] == 0xff) {
            for (i = 0; i < 4; i++) {
                if (idata[i] != -1) FUN_L06_00307c48((unsigned char *)moby, (unsigned char *)(D_L06_0015FFD8_gp + idata[i] * 0x100));
            }
            moby[0x20] = 2;
        }
        break;
    case 2:
        data[9] -= frame_time + frame_time;
        if (data[9] < 0.0f) data[9] = 0.0f;
        if (0.0f < data[9]) enqueue_callback_list_1((int)FUN_L06_00307578, (int)moby);
        return;
    default:
        return;
    }
}
#include "sda.h"
#include "rnc/overlay/entities.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"

/* Stage machine for a three-state effect moby: wait, activate, burst and delete. */
/* Ported from rac1-decomp (src/overlays/l07_umbris/vendor_0031BDB8.c: func_L07_00320980), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x10];
    char pos[0x10];
    unsigned char b20;
    char pad21[0xB];
    float f2C;
    unsigned char b30;
    unsigned char b31;
    short s32;
    unsigned short h34;
    char pad36[0x12];
    float f48;
    char pad4C[0x2C];
    Child *child;
    char pad7C[0x14];
    int f90;
} Obj;

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern char *func_0020D348_m(int);
extern int FUN_0022da68_c(int, int, int) __asm__("FUN_0022da68");
extern void FUN_L00_00250df8(void *);
extern void FUN_L00_00263fd8(void *, int, void *, void *, int, int, float, void *, void *, void *);
extern void FUN_L01_002787a0(void *);
void mark_moby_for_removal_c(struct Obj *obj) __asm__("FUN_0020c828");
extern char *mk_a(int) __asm__("FUN_0020c4f8");
extern void upd_a(void *) __asm__("FUN_L00_00250df8");

void FUN_L06_00309860(char *self) {
    int flag = 0;
    char *o = find_moby_hit(self, 0x10000, 0);
    switch (((unsigned char *)self)[0x20]) {
    case 0:
        self[0x20] = 1;
        break;
    case 1:
        if (o != 0 && *(float *)(o + 0x2C) > 0.0f) {
            flag = 1;
        }
        if (flag) {
            self[0x20] = 2;
        }
        break;
    case 2: {
        char *m;
        FUN_0022da68_c(0, 0, (int)self);
        FUN_L01_002787a0(self);
        FUN_L00_00263fd8(self, 0x679, self + 0x10, self + 0x40, 0, 0, 0.0f, D_L06_0015F580,
                         D_L06_0015F580, D_L06_0015F580);
        FUN_L00_00263fd8(self, 0x67a, self + 0x10, self + 0x40, 0, 0, 0.0f, D_L06_0015F580,
                         D_L06_0015F580, D_L06_0015F580);
        FUN_L00_00263fd8(self, 0x67b, self + 0x10, self + 0x40, 0, 0, 0.0f, D_L06_0015F580,
                         D_L06_0015F580, D_L06_0015F580);
        m = mk_a(0x678);
        if (m != 0) {
            m[0x31] = 1;
            *(short *)(m + 0x32) = 0xFF;
            qcopy(m + 0x10, self + 0x10);
            qcopy(m + 0x40, self + 0x40);
            *(long *)(m + 0x38) = *(long *)(self + 0x38);
            upd_a(m);
        }
        mark_moby_for_removal_c(self);
        break;
    }
    }
}
/* UpdateMoby: state 0 deletes, state 1 draws and times out */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_0030D2A0), where it is exact; names translated to the US level program. */

void FUN_L06_0030bfa8(char *moby);
void enqueue_callback_list_1(s32 arg0, s32 arg1) __asm__("FUN_001f4600");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void FUN_L06_0030be60(char *m) {
    float *d = *(float **)(m + 0x78);
    switch ((unsigned char)m[0x20]) {
    case 0:
        mark_moby_for_removal(m);
        break;
    case 1:
        enqueue_callback_list_1(FUN_L06_0030bfa8, m);
        d[0] = d[0] + d[1];
        if (2.0f < d[0]) {
            mark_moby_for_removal(m);
        }
        break;
    }
}
extern char *mk_b(int) __asm__("FUN_0020c4f8");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");


void FUN_L06_0030bef8(char *src, int duration_updates, int start_color, int end_color) {
    OvlQuad v;
    char *vp = (char *)&v;
    unsigned char *m;
    v = *(OvlQuad *)src;
    m = (unsigned char *)mk_b(0x76A);
    if (m) {
        char *e = *(char **)(m + 0x78);
        float f;
        qcopy(m + 0x10, vp);
        f = convert_integer_to_float(duration_updates);
        *(int *)(e + 0xC) = end_color;
        *(int *)(e + 0x8) = start_color;
        *(float *)(e + 0x4) = 2.0f / f;
        m[0x30] = 0xFF;
        m[0x20] = 1;
    }
}
extern void vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");
extern void project_to_screen(f32 *, void *) __asm__("FUN_001f2070");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern u32 fast_tween_color(f32, s32, s32) __asm__("FUN_001fa6e0");
extern u64 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void FUN_L02_0020bc88(void *, void *, void *, s64, s32);
extern s32 D_0013E500[];
extern s32 D_L06_00162414 __attribute__((sda));
extern s32 D_L06_00162418 __attribute__((sda));
extern s32 D_L06_0016241C __attribute__((sda));
extern s32 D_L06_00162420 __attribute__((sda));
extern s32 D_L06_00162424 __attribute__((sda));
extern s32 D_L06_00162428 __attribute__((sda));

/* Draws a screen-space sprite at the moby, grown and faded in by the first data float. */
void FUN_L06_0030bfa8(char *moby) {
    float *d = *(float **)(moby + 0x78);
    OvlVec4 pos;
    s64 corners[4];
    u32 colors[4];
    s32 uvs[4];
    float screen[4];
    float size;
    float fade;
    s32 y, x, radius;
    s32 raw_x;
    u64 texture;
    u32 color;
    s32 *screen_center;
    float screen_offset;

    qcopy(&pos, moby + 0x10);
    size = d[0];
    if (1.0f < size) size = 1.0f;
    fade = d[0] - 1.0f;
    if (1.0f < fade) fade = 1.0f;
    else if (fade < 0.0f) fade = 0.0f;
    vu1_add_g_sregister(0x47, 0x5380B);
    project_to_screen(screen, &pos);
    screen_center = D_0013E500;
    raw_x = truncate_float_to_s32(screen[0] - (float)screen_center[4]);
    screen_offset = (float)screen_center[4];
    y = truncate_float_to_s32(screen[1] - screen_offset);
    x = raw_x - 0x1000;
    y -= 0x1000;
    uvs[0] = D_L06_0016241C;
    uvs[1] = D_L06_00162420;
    uvs[2] = D_L06_00162424;
    uvs[3] = D_L06_00162428;
    color = fast_tween_color(fade, *(s32 *)&d[2], *(s32 *)&d[3]);
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    radius = truncate_float_to_s32((float)(D_L06_00162418 << 4) * size);
    corners[3] = 0xfffff080008000LL;
    corners[2] = 0xfffff080008000LL;
    corners[1] = 0xfffff080008000LL;
    corners[0] = 0xfffff080008000LL;
    corners[0] += (y - radius) << 16;
    corners[0] += x + radius;
    corners[1] += (y - radius) << 16;
    corners[1] += x - radius;
    corners[2] += (y + radius) << 16;
    corners[2] += x + radius;
    corners[3] += (y + radius) << 16;
    corners[3] += x - radius;
    texture = get_effect_texture(D_L06_00162414);
    FUN_L02_0020bc88(corners, uvs, colors, texture, 1);
    vu1_add_g_sregister(0x47, 0x5360B);
}
/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00317770), where it is exact; names translated to the US level program. */

extern char *D_L06_0015EF50;

void FUN_L06_00316330(char *moby) {
    if (*(short *)(moby + 0x86) == 0x12) {
        *(int *)(*(char **)(D_L06_0015EF50 + *(short *)(moby + 0x84) * 32 + 0x1C) + 0x50) = 1;
    }
}

s32 D_L06_001FE9D0[1] __attribute__((section(".data"))) = {0};
s32 D_L10_001DA210[1] __attribute__((section(".data"))) = {0};
