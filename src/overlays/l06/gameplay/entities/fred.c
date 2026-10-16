/* Level 06 moby classes: fred, shuttle, red_button. */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/gameplay/hero.h"
#include "rnc/globals.h"
#include "asm.h"

#include "eetypes.h"
#include "qcopy.h"
typedef struct {
    u128 v[4];
    int color[4];
    float uv[4][2];
    long unk70, tex, unk80, unk88;
} Packet_2fd1a0;

extern int D_L06_00161F7C __attribute__((sda));
extern int D_L06_00161F80 __attribute__((sda));
extern int D_L06_00161F84 __attribute__((sda));
extern int D_L06_00161F88 __attribute__((sda));
extern int D_L06_00161F8C __attribute__((sda));
extern int D_L06_00161F90 __attribute__((sda));
extern unsigned int D_L06_00161F94 __attribute__((sda));
extern int D_L06_00161F98 __attribute__((sda));
extern float D_L06_00161F9C __attribute__((sda));
extern float D_L06_00161FA0 __attribute__((sda));
extern float D_L06_00161FA4 __attribute__((sda));
extern float D_L06_00167554;
extern float D_L06_00167540[];
extern float D_L06_001F1540[][2];
extern short D_L06_001F1220[][4][2];
extern float D_L06_001F0900[][4] __attribute__((aligned(16)));
extern char D_L06_001F2610[][16];
extern char D_L06_001F19D0[][16];
extern long texture_2fd1a0(int) __asm__("FUN_001f44b8");
extern void initialize_matrix_2fd1a0(void *) __asm__("FUN_001f9fc8");
extern void transform_2fd1a0(void *, void *, float) __asm__("FUN_001f9a68");
extern void multiply_2fd1a0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void subtract_2fd1a0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_2fd1a0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void transform_other_2fd1a0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float normalize_other_2fd1a0(void *, void *) __asm__("FUN_001f9ab0");
extern void draw_2fd1a0(void *, void *, int) __asm__("FUN_001f7d30");

void FUN_L06_002fd1a0(void) {
    Packet_2fd1a0 first, second;
    float matrix[4][4] __attribute__((aligned(16)));
    float vec2[4] __attribute__((aligned(16)));
    float vec1[4] __attribute__((aligned(16)));
    float y_offset = D_L06_00161FA0 * D_L06_00167400[86];
    float x_offset = D_L06_00161FA4 * D_L06_00167400[85];
    void *matrix_ptr;
    long flags;
    int i, j;

    long tex = texture_2fd1a0(D_L06_00161F90);
    first.tex = tex;
    second.tex = texture_2fd1a0(0x2a);
    second.unk80 = 0xFF9000000260L;
    first.unk80 = 0xFF9000000260L;
    flags = (long)D_L06_00161F7C;
    flags |= (long)D_L06_00161F80 << 2;
    flags |= (long)D_L06_00161F84 << 4;
    flags |= (long)D_L06_00161F88 << 6;
    flags |= (long)D_L06_00161F8C << 32;
    second.unk88 = flags;
    first.unk88 = flags;
    second.unk70 = 0;
    first.unk70 = 0;
    matrix_ptr = matrix;
    initialize_matrix_2fd1a0(matrix);
    first.color[0] = first.color[1] = first.color[2] = first.color[3] = D_L06_00161F94;
    second.color[0] = second.color[1] = second.color[2] = second.color[3] = D_L06_00161F98;
    i = 0;
    do {
        transform_2fd1a0(vec1, D_L06_001F2610[i], D_L06_00161F9C);
        multiply_2fd1a0(vec1, vec1, matrix);
        subtract_2fd1a0(vec2, vec1, D_L06_00167540);
        normalize_2fd1a0(vec2, vec2, 1.0f);
        transform_other_2fd1a0(vec1, D_L06_001F19D0[i], matrix_ptr);
        normalize_other_2fd1a0(vec2, vec1);
        j = 0;
        do {
            int a = D_L06_001F1220[i][j][0];
            int b = D_L06_001F1220[i][j][1];
            float u, v;
            qcopy(&first.v[j], D_L06_001F0900[a]);
            qcopy(&second.v[j], &first.v[j]);
            u = D_L06_001F1540[b][0];
            v = D_L06_001F1540[b][1];
            first.uv[j][0] = u + y_offset;
            first.uv[j][1] = v + x_offset;
            second.uv[j][0] = u;
            second.uv[j][1] = v;
            j++;
        } while (j < 4);
        draw_2fd1a0(&first, matrix, 0);
        draw_2fd1a0(&second, matrix, 0);
        i++;
    } while (i < 50);
}
#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    u128 v[4];
    int color[4];
    float uv[4][2];
    long unk70, tex, unk80, unk88;
} Packet_2fd460;

extern float D_L06_00167540[];
extern short D_L06_001EF310[][4][2];
extern float D_L06_001EF6B0[][2];
extern float D_L06_001EED30[][4] __attribute__((aligned(16)));
extern char D_L06_001F0560[][16];
extern char D_L06_001EFBE0[][16];
extern long texture_2fd1a0(int) __asm__("FUN_001f44b8");
extern void initialize_matrix_2fd1a0(void *) __asm__("FUN_001f9fc8");
extern void transform_2fd1a0(void *, void *, float) __asm__("FUN_001f9a68");
extern void multiply_2fd1a0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void subtract_2fd1a0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_2fd1a0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void transform_other_2fd1a0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float normalize_other_2fd1a0(void *, void *) __asm__("FUN_001f9ab0");
extern void draw_2fd1a0(void *, void *, int) __asm__("FUN_001f7d30");

void FUN_L06_002fd460(void) {
    Packet_2fd460 first, second;
    float matrix[4][4] __attribute__((aligned(16)));
    float vec2[4] __attribute__((aligned(16)));
    float vec1[4] __attribute__((aligned(16)));
    void *matrix_ptr;
    float y_offset = D_L06_00161FA0 * D_L06_00167400[86];
    float x_offset = D_L06_00161FA4 * D_L06_00167400[85];
    long flags;
    int i, j;
    float facing;

    first.tex = texture_2fd1a0(D_L06_00161F90);
    second.tex = texture_2fd1a0(0x2a);
    matrix_ptr = matrix;
    flags = (long)D_L06_00161F7C | (long)D_L06_00161F80 << 2 |
            (long)D_L06_00161F84 << 4 | (long)D_L06_00161F88 << 6 |
            (long)D_L06_00161F8C << 32;
    second.unk88 = flags;
    first.unk88 = flags;
    second.unk80 = 0xFF9000000260L;
    first.unk80 = 0xFF9000000260L;
    second.unk70 = 0;
    first.unk70 = 0;
    initialize_matrix_2fd1a0(matrix_ptr);
    first.color[3] = D_L06_00161F94;
    first.color[2] = D_L06_00161F94;
    first.color[1] = D_L06_00161F94;
    first.color[0] = D_L06_00161F94;
    second.color[3] = D_L06_00161F98;
    second.color[2] = D_L06_00161F98;
    second.color[1] = D_L06_00161F98;
    second.color[0] = D_L06_00161F98;
    i = 0;
    do {
        transform_2fd1a0(vec1, D_L06_001F0560[i], D_L06_00161F9C);
        multiply_2fd1a0(vec1, vec1, matrix);
        subtract_2fd1a0(vec2, vec1, D_L06_00167540);
        normalize_2fd1a0(vec2, vec2, 1.0f);
        transform_other_2fd1a0(vec1, D_L06_001EFBE0[i], matrix_ptr);
        facing = normalize_other_2fd1a0(vec2, vec1);
        if (facing > 0.0f) goto next;
        {
            j = 0;
            do {
                int a = D_L06_001EF310[i][j][0];
                int b = D_L06_001EF310[i][j][1];
                float u, v;
                transform_2fd1a0(&first.v[j], D_L06_001EED30[a], D_L06_00161F9C);
                qcopy(&second.v[j], &first.v[j]);
                u = D_L06_001EF6B0[b][0];
                v = D_L06_001EF6B0[b][1];
                first.uv[j][0] = u + y_offset;
                first.uv[j][1] = v + x_offset;
                second.uv[j][0] = u;
                second.uv[j][1] = v;
                j++;
            } while (j < 4);
            draw_2fd1a0(&first, matrix, 0);
            draw_2fd1a0(&second, matrix, 0);
        }
next:
        i++;
    } while (i < 58);
}
#include "eetypes.h"
#include "qcopy.h"

typedef struct {
    u128 v[4];
    int color[4];
    float uv[4][2];
    long unk70, tex, unk80, unk88;
} Packet_2fd748;

extern float D_L06_00167540[];
extern short D_L06_001DF6F0[][4][2];
extern float D_L06_001E2CF0[][2];
extern float D_L06_001DBFF0[][4] __attribute__((aligned(16)));
extern char D_L06_001EB730[][16];
extern char D_L06_001E4A30[][16];
extern long texture_2fd1a0(int) __asm__("FUN_001f44b8");
extern void initialize_matrix_2fd1a0(void *) __asm__("FUN_001f9fc8");
extern void transform_2fd1a0(void *, void *, float) __asm__("FUN_001f9a68");
extern void multiply_2fd1a0(void *, void *, void *) __asm__("FUN_001f9d20");
extern void subtract_2fd1a0(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_2fd1a0(void *, void *, float) __asm__("FUN_001f9bf8");
extern void transform_other_2fd1a0(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float normalize_other_2fd1a0(void *, void *) __asm__("FUN_001f9ab0");
extern void draw_2fd1a0(void *, void *, int) __asm__("FUN_001f7d30");

void FUN_L06_002fd748(void) {
    Packet_2fd748 first, second;
    float matrix[4][4] __attribute__((aligned(16)));
    float vec2[4] __attribute__((aligned(16)));
    float vec1[4] __attribute__((aligned(16)));
    void *matrix_ptr;
    float y_offset = D_L06_00161FA0 * D_L06_00167400[86];
    float x_offset = D_L06_00161FA4 * D_L06_00167400[85];
    long flags;
    int i, j;
    float facing;

    first.tex = texture_2fd1a0(D_L06_00161F90);
    second.tex = texture_2fd1a0(0x2a);
    matrix_ptr = matrix;
    flags = (long)D_L06_00161F7C | (long)D_L06_00161F80 << 2 |
            (long)D_L06_00161F84 << 4 | (long)D_L06_00161F88 << 6 |
            (long)D_L06_00161F8C << 32;
    second.unk88 = flags;
    first.unk88 = flags;
    second.unk80 = 0xFF9000000260L;
    first.unk80 = 0xFF9000000260L;
    second.unk70 = 0;
    first.unk70 = 0;
    initialize_matrix_2fd1a0(matrix_ptr);
    first.color[3] = D_L06_00161F94;
    first.color[2] = D_L06_00161F94;
    first.color[1] = D_L06_00161F94;
    first.color[0] = D_L06_00161F94;
    second.color[3] = D_L06_00161F98;
    second.color[2] = D_L06_00161F98;
    second.color[1] = D_L06_00161F98;
    second.color[0] = D_L06_00161F98;
    i = 0;
    do {
        transform_2fd1a0(vec1, D_L06_001EB730[i], D_L06_00161F9C);
        multiply_2fd1a0(vec1, vec1, matrix);
        subtract_2fd1a0(vec2, vec1, D_L06_00167540);
        normalize_2fd1a0(vec2, vec2, 1.0f);
        transform_other_2fd1a0(vec1, D_L06_001E4A30[i], matrix_ptr);
        facing = normalize_other_2fd1a0(vec2, vec1);
        if (!(facing > 0.0f)) {
            j = 0;
            do {
                int a = D_L06_001DF6F0[i][j][0];
                int b = D_L06_001DF6F0[i][j][1];
                float u, v;
                transform_2fd1a0(&first.v[j], D_L06_001DBFF0[a], D_L06_00161F9C);
                qcopy(&second.v[j], &first.v[j]);
                u = D_L06_001E2CF0[b][0];
                v = D_L06_001E2CF0[b][1];
                first.uv[j][0] = u + y_offset;
                first.uv[j][1] = v + x_offset;
                second.uv[j][0] = u;
                second.uv[j][1] = v;
                j++;
            } while (j < 4);
            draw_2fd1a0(&first, matrix, 0);
            draw_2fd1a0(&second, matrix, 0);
        }
        i++;
    } while (i < 0x360);
}

#define NOT_SDA

#define MACRO_ADDR

/* Trigger moby: waits for the hero in its zone, plays its open animation, then launches a listed moby. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_002FEE60), where it is exact; names translated to the US level program. */

extern int D_L06_0015FFD8;
extern int FUN_L06_002e9d10(void *, void *);
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int tick_countdown_32_alt(void *) __asm__("FUN_001f9740");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern short *D_L06_001ABFC0[];
extern void blend_moby_animation(void *, int, int, int) __asm__("FUN_00212f90");

void FUN_L06_002fda30(char *m) {
    int *d = *(int **)(m + 0x78);
    switch (((unsigned char *)m)[0x20]) {
    case 0:
        if (d[1] >= 0 && is_point_inside_clip_volume(&hero.motion.pos, d[1])) {
            m[0x20] = 1;
            if (((unsigned char *)m)[0x53] != 1) {
                blend_moby_animation(m, 1, 0, 5);
            }
        }
        break;
    case 1:
        if (m[0x70] & 2) {
            if (d[0] >= 0) {
                m[0x20] = 2;
            } else {
                m[0x20] = 3;
            }
            if (((unsigned char *)m)[0x53] != 2) {
                blend_moby_animation(m, 2, 0, 5);
            }
        }
        break;
    case 2:
        if (tick_countdown_32_alt(d + 2)) {
            float found = 0.0f;
            short *list = D_L06_001ABFC0[d[0]];
            if (list != 0) {
                short *p = list;
                char *pos = m + 0x10;
                do {
                    if (FUN_L06_002e9d10(D_L06_0015FFD8 + ((*(unsigned short *)p & 0x7FFF) << 8),
                                         pos)) {
                        found = 1.0f;
                        break;
                    }
                } while (*p++ >= 0);
            }
            if (found != 0.0f) {
                d[2] = scale_game_frames(0x14);
            } else {
                m[0x20] = 3;
            }
        }
        break;
    }
}
extern int D_L06_001ABFC0_u[] __asm__("D_L06_001ABFC0");
extern int D_L06_0015FFD8_d __asm__("D_L06_0015FFD8");
/* 0x002ffc38, 116 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
void FUN_L06_002ffc38(int idx) {
    short *p = D_L06_001ABFC0_u[idx];
    if (p != 0) {
        do {
            char *moby = D_L06_0015FFD8_d + ((*p & 0x7FFF) << 8);
            if ((unsigned char)moby[0x20] < 0x7F) {
                unsigned short flags = *(unsigned short *)(moby + 0x34);
                moby[0x20] = 0x11;
                flags |= 1;
                *(int *)(moby + 0x94) = 0;
                flags &= 0xEFFF;
                *(unsigned short *)(moby + 0x34) = flags;
            }
        } while (*p++ >= 0);
    }
}

extern char D_0013F3D0[];
extern unsigned char *D_L06_0015FFD8_ffcb0 __asm__("D_L06_0015FFD8");
extern char *D_L06_001600EC_ffcb0 __asm__("D_L06_001600EC");
extern float D_L06_0015F3FC __asm__("D_L06_0015F3FC");
extern float vector_distance_ffcb0(void *, void *) __asm__("FUN_001f9b80");
extern int scale_frames_ffcb0(int) __asm__("FUN_001f96f8");
extern int count_down_ffcb0(int *) __asm__("FUN_001f9740");
extern float angle_atan2_ffcb0(float, float) __asm__("FUN_001f9e90");
extern float normalize_timer_ffcb0(int) __asm__("FUN_001fa6c0");
extern void approach_two_stage_ffcb0(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void lerp_vector_ffcb0(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void vec_add_ffcb0(void *, void *, void *) __asm__("FUN_001f9a10");
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void FUN_L00_002eaaa0(void *, void *, int, int, int);
extern void FUN_L00_002eaa30(void *);
extern void FUN_L06_00312b40(float, float, float, float, float, float);
extern void FUN_L06_00312a78(void *);
extern void FUN_L06_00312ad0(void *);
extern void FUN_L06_00312ce8(int);
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void FUN_L00_002eac18(int);
extern int hero_set_state(int, int) __asm__("FUN_L06_002356a0");
extern void FUN_L06_00300278(char *);
extern void enqueue_callback_list_1(int, char *) __asm__("FUN_L06_00210fc8");
extern void allocate_voice_for_target_entry_ffcb0(int, int, char *) __asm__("FUN_0022da68");

extern void FUN_L06_00300958(void);
extern void FUN_L06_00300720(void);
extern void FUN_L06_00300588(void);
extern void FUN_L06_003003f0(void);

extern int D_L06_0015F404;
extern int D_L06_0015F408;
extern float D_L06_00162048 __attribute__((sda));
extern int D_L06_00162044 __attribute__((sda));
extern float D_L06_00162050 __attribute__((sda));
void FUN_L06_002ffcb0(char *moby) {
    int *data = *(int **)(moby + 0x78);
    float pos[4], rot[4], from[4], target[4], offset[4];
    int base;
    int state;
    *(float *)(moby + 0x2c) = *(float *)(*(char **)(moby + 0x24) + 0x24) * D_L06_00162048;
    switch ((unsigned char)moby[0x20]) {
    case 0:
        data[1] = 1;
        moby[0x20] = 1;
        break;
    case 1:
        if ((data[2] < 0 || (unsigned char)D_L06_0015FFD8_ffcb0[(data[2] << 8) + 0xbc] == 0) && data[8] != -1) {
            break;
        }
        if (vector_distance_ffcb0(moby + 0x10, D_0013F3D0) < 32.0f) {
            moby[0x20] = 2;
            data[9] = scale_frames_ffcb0(30);
        } else {
            FUN_L06_00300278(moby);
            moby[0x20] = 4;
        }
        break;
    case 2:
        if (count_down_ffcb0(data + 9)) {
            {
                int state;
                int base;
                moby[0x20] = 3;
                D_L06_0015F404 = 1;
                D_L06_0015F408 = 0x12;
                state = data[5];
                state <<= 7;
                base = (int)D_L06_001600EC_ffcb0;
                qcopy(pos, (void *)(state + base + 0x30));
                qcopy(rot, (void *)(state + base + 0x70));
            }
            FUN_L00_002eaaa0(pos, rot, 1, scale_frames_ffcb0(300), 0);
            FUN_L06_00312b40(0.001f, 1.0f, 1.0f, 0.001f, 1.0f, 1.0f);
            FUN_L06_00312a78(pos);
            FUN_L06_00312ad0(rot);
            {
                int base;
                int state;
                float facing;
                data[6] = 0;
                {
                    int base;
                    base = data[4];
                    base <<= 7;
                    base += (int)D_L06_001600EC_ffcb0;
                    facing = angle_atan2_ffcb0(*(float *)(moby + 0x10) - *(float *)(base + 0x30), *(float *)(moby + 0x14) - *(float *)(base + 0x34));
                }
                {
                    int state;
                    state = data[4];
                    state <<= 7;
                    base = (int)D_L06_001600EC_ffcb0;
                    *(float *)(state + base + 0x78) = facing;
                }
                state = data[4];
                state <<= 7;
                FUN_L00_00216f90((void *)(base + state + 0x30), (void *)(base + state + 0x70), 0x72, 0);
            }
        }
        if (data[9] < D_L06_00162044)
            D_L06_0015F3FC = 1.0f - normalize_timer_ffcb0(data[9]) / (float)D_L06_00162044;
        break;
    case 3:
        if (data[9] < D_L06_00162044) {
            data[9]++;
            D_L06_0015F3FC = 1.0f - normalize_timer_ffcb0(data[9]) / (float)D_L06_00162044;
        }
        /* This field holds float bits; retail completes only on an ordered >= comparison. */
        if (*(float *)(data + 6) >= 0.99999f) {
            moby[0x20] = 5;
            hero_set_state(0, 0);
            FUN_L06_00312ce8(2);
            D_L06_0015F404 = 0;
        } else {
            if (*(float *)(data + 6) >= 0.05f && data[3] >= 0) {
                {
                    int state;
                    int base;
                    state = data[3];
                    state <<= 7;
                    base = (int)D_L06_001600EC_ffcb0;
                    qcopy((void *)(state + base + 0x30), D_0013F3D0);
                    *(float *)(state + base + 0x38) -= 0.5f;
                }
                data[3] = -1;
            }
            if (data[1] == 1 && vector_distance_ffcb0(moby + 0x10, D_L06_0015FFD8_ffcb0 + (data[8] << 8) + 0x10) > 0.1f) {
                allocate_voice_for_target_entry_ffcb0(0, 0, moby);
                FUN_L06_00300278(moby);
            }
            approach_two_stage_ffcb0((float *)(data + 6), (float *)(data + 7), 1.0f, frame_time_sq, frame_time_sq, frame_time * 1.5f);
            {
                int state;
                int base;
                state = data[5];
                state <<= 7;
                base = (int)D_L06_001600EC_ffcb0;
                qcopy(pos, (void *)(state + base + 0x30));
                qcopy(from, (void *)(state + base + 0x70));
            }
            from[0] = D_L06_00162050 * 0.017453292f;
            qcopy(rot, D_0013F3D0);
            qcopy(target, D_0013F3D0 + 16);
            /* Retail takes this component from the camera rotation, not the hero position. */
            target[1] = from[1];
            normalize_2fd1a0(offset, D_0013F3D0 - 0x80, -2.0f);
            vec_add_ffcb0(rot, rot, offset);
            rot[2] += 0.85f;
            lerp_vector_ffcb0(pos, pos, rot, *(float *)(data + 6));
            lerp_vector_ffcb0(from, from, target, *(float *)(data + 6));
            FUN_L00_002ea9d8(pos);
            FUN_L00_002eaa30(from);
        }
        break;
    case 4:
        if (vector_distance_ffcb0(moby + 0x10, D_0013F3D0) < 16.0f) {
            state = data[3];
            state <<= 7;
            base = (int)D_L06_001600EC_ffcb0;
            qcopy((void *)(state + base + 0x30), D_0013F3D0);
            *(float *)(state + base + 0x38) -= 0.5f;
            data[3] = -1;
            moby[0x20] = 5;
        }
        break;
    }
    if (data[1] != 0) {
        if (data[0] != 0) enqueue_callback_list_1((int)FUN_L06_00300720, moby);
        else enqueue_callback_list_1((int)FUN_L06_00300958, moby);
    } else {
        if (data[0] != 0) enqueue_callback_list_1((int)FUN_L06_003003f0, moby);
        else enqueue_callback_list_1((int)FUN_L06_00300588, moby);
    }
}

#include "qcopy.h"
extern char *create_moby_300278(int) __asm__("FUN_0020c4f8");
extern void moby_init_300278(char *) __asm__("FUN_L00_00250df8");
extern float rand_range_300278(float, float) __asm__("FUN_002132a8");
extern float rand_angle_300278(void) __asm__("FUN_00213308");
extern float cos_300278(float) __asm__("FUN_001f9dc8");
extern float sin_300278(float) __asm__("FUN_001f9de0");
extern void vadd_300278(void *, void *, void *) __asm__("FUN_001f9a10");
extern void vnorm_300278(void *, void *, float) __asm__("FUN_001f9bf8");
extern unsigned char *FUN_L06_00300c60(char *, float *, float *);

extern float D_L06_0016204C __attribute__((sda));
/* Breaks the moby apart: spawns a copy (class 0x43C) and throws 50
 * fragments out in random directions. */
void FUN_L06_00300278(char *m) {
    float p[4];
    float d[4];
    char *n;
    float s;
    float a;
    int i;

    *(int *)(*(char **)(m + 0x78) + 4) = 0;
    n = create_moby_300278(0x43C);
    *(short *)(n + 0x32) = 0x40;
    n[0x31] = 1;
    *(long long *)(n + 0x38) = *(long long *)(m + 0x38);
    *(unsigned short *)(n + 0x34) = *(unsigned short *)(m + 0x34);
    qcopy(n + 0x10, m + 0x10);
    qcopy(n + 0x40, m + 0x40);
    *(float *)(n + 0x2C) = *(float *)(m + 0x2C);
    moby_init_300278(n);
    *(unsigned short *)(m + 0x34) |= 1;
    for (i = 0; i < 50; i++) {
        s = rand_range_300278(1.0f, 6.0f) * frame_time;
        a = rand_angle_300278();
        d[0] = cos_300278(a) * 2.0f;
        d[1] = sin_300278(a) * 2.0f;
        d[2] = 0.0f;
        vadd_300278(p, m + 0x10, d);
        p[2] += rand_range_300278(D_L06_0016204C + 0.5f, D_L06_0016204C + 4.5f);
        vnorm_300278(d, d, s);
        FUN_L06_00300c60(m, p, d);
    }
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V3003f0;
typedef struct {
    float u, v;
} UV3003f0;
typedef struct {
    V3003f0 corner[4];
    unsigned int color[4];
    UV3003f0 uv[4];
    long unk70, tex, unk80, unk88;
} Q3003f0;
typedef struct {
    char pad[0x38];
    float f38;
    int pad3c;
} B3003f0;
typedef struct {
    short v, uv;
} I3003f0;
extern int D_L06_00162054 __attribute__((sda));
extern int D_L06_00162058 __attribute__((sda));
extern int D_L06_0016205C __attribute__((sda));
extern int D_L06_00162060 __attribute__((sda));
extern int D_L06_00162064 __attribute__((sda));
extern int D_L06_00162068 __attribute__((sda));
extern unsigned int D_L06_0016206C __attribute__((sda));
extern float D_L06_00162074 __attribute__((sda));
extern V3003f0 D_L06_001F2EB0[];
extern I3003f0 D_L06_001F34B0[];
extern UV3003f0 D_L06_001F38B0[];
extern long tex_3003f0(int) __asm__("FUN_001f44b8");
extern void init_3003f0(void *) __asm__("FUN_001f9fc8");
extern void vmul_3003f0(void *, void *, float) __asm__("FUN_001f9a68");
extern void draw_3003f0(void *, void *, int) __asm__("FUN_001f7d30");

/* Draws the 64 quads of the backdrop mesh, each corner scaled from the shared vertex table. */
void FUN_L06_003003f0(void) {
    Q3003f0 quad;
    B3003f0 buf;
    unsigned int c;
    int i, j, k;
    I3003f0 *e;

    quad.tex = tex_3003f0(D_L06_00162064);
    quad.unk88 = (long)D_L06_00162054 | ((long)D_L06_00162058 << 2) | ((long)D_L06_0016205C << 4) |
                 ((long)D_L06_00162060 << 6) | ((long)D_L06_00162068 << 32);
    quad.unk80 = 0xFF9000000260L;
    quad.unk70 = 0;
    init_3003f0(&buf);
    c = D_L06_0016206C;
    buf.f38 = D_L06_0016204C;
    quad.color[3] = c;
    quad.color[2] = c;
    quad.color[1] = c;
    quad.color[0] = c;
    for (i = 0; i < 64; i++) {
        e = &D_L06_001F34B0[i * 4];
        for (j = 0; j < 4; j++) {
            k = e->uv;
            vmul_3003f0(&quad.corner[j], &D_L06_001F2EB0[e->v], D_L06_00162074);
            e++;
            quad.uv[j].u = D_L06_001F38B0[k].u;
            quad.uv[j].v = D_L06_001F38B0[k].v;
        }
        draw_3003f0(&quad, &buf, 0);
    }
}

/* Draws the 96 quads of the second backdrop mesh, skipping those that face away from D_L06_00167540. */
typedef float FVec4_l06[4] __attribute__((aligned(16)));

typedef struct {
    FVec4_l06 v;
} FRow_l06;

extern FRow_l06 D_L06_001FAFF0[] __attribute__((section(".data")));
extern FRow_l06 D_L06_001FA2F0[] __attribute__((section(".data")));
extern FRow_l06 D_L06_001F77B0[] __attribute__((section(".data")));
extern float D_L06_001F84B0[][2] __attribute__((section(".data")));
extern short D_L06_001F7EB0[][4][2] __attribute__((section(".data")));
extern float D_L06_00167540[];
extern void transform_vector(void *, void *, void *) __asm__("FUN_001f9d20");
extern void subtract_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");

void FUN_L06_00300720(void) {
    FRow_l06 m[4];
    int colors[4];
    float uv[4][2];
    unsigned long pkt[4];
    float mat[4][4];
    FVec4_l06 dir;
    FVec4_l06 center;
    float *mp;
    int i;
    int j;

    pkt[1] = tex_3003f0(D_L06_00162064);
    mp = mat[0];
    pkt[2] = 0xFF9000000260;
    pkt[3] = D_L06_00162054 | (long)D_L06_00162058 << 2;
    pkt[3] |= (long)D_L06_0016205C << 4;
    pkt[3] |= (long)D_L06_00162060 << 6;
    pkt[3] |= (long)D_L06_00162068 << 32;
    pkt[0] = 0;
    init_3003f0(mp);
    mat[3][2] = D_L06_0016204C;
    colors[3] = D_L06_0016206C;
    colors[2] = D_L06_0016206C;
    colors[1] = D_L06_0016206C;
    colors[0] = D_L06_0016206C;
    for (i = 0; i < 96; i++) {
        vmul_3003f0(center, &D_L06_001FAFF0[i], D_L06_00162074);
        transform_vector(center, center, mat);
        subtract_vector_xyz(dir, center, D_L06_00167540);
        fast_vec_normalize(dir, dir, 1.0f);
        transform_vector_by_basis(center, &D_L06_001FA2F0[i], mp);
        if (fast_vec_dot(dir, center) > 0.0f)
            continue;
        for (j = 0; j < 4; j++) {
            int a = D_L06_001F7EB0[i][j][0];
            int b = D_L06_001F7EB0[i][j][1];
            vmul_3003f0(&m[j], &D_L06_001F77B0[a], D_L06_00162074);
            uv[j][0] = D_L06_001F84B0[b][0];
            uv[j][1] = D_L06_001F84B0[b][1];
        }
        draw_3003f0(m, mat, 0);
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00302248), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern void FUN_001f9a10(void *, void *, void *);
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

extern short D_L06_00162090 __attribute__((sda));
extern int GB_FRAME_gp __asm__("D_L06_0015F5CC") __attribute__((sda));
void FUN_L06_00300df0(char *moby) {
    char *data = *(char **)(moby + 0x78);
    int state;
    if (data == 0) {
        mark_moby_for_removal(moby);
        return;
    }
    state = (unsigned char)moby[0x20];
    switch (state) {
    case 0:
        *(short *)(moby + 0x32) = 0x200;
        ((unsigned char *)moby)[0x30] = 0xFF;
        *(float *)(data + 0xC) = *(float *)(moby + 0x18);
        moby[0x20] = 1;
        break;
    case 1:
        if (*(int *)(D_0013E533 + 0x2EA1) == 0x72) {
            *(float *)(moby + 0x18) = *(float *)(data + 0xC);
        } else {
            float f = convert_integer_to_float(GB_FRAME_gp % 600) / 600.0f;
            float g = fast_sin(f * 6.28318f);
            *(float *)(moby + 0x18) = *(float *)(data + 0xC) + *(float *)&D_L06_00162090 * g;
        }
        break;
    case 2:
        if (*(float *)(data + 0xC) > 1.0f)
            fast_vec_scale(data, data, 0.985f);
        FUN_001f9a10(moby + 0x10, moby + 0x10, data);
        FUN_001f9a10(moby + 0x40, moby + 0x40, data + 0x10);
        if (*(int *)(D_0013E533 + 0x2EA1) != 0x72)
            mark_moby_for_removal(moby);
        break;
    }
}
#include "qcopy.h"

#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_003023C8), where it is exact; names translated to the US level program. */

void FUN_L06_00300f70(char *arg, void *a, void *b) {
    char *dst = *(char **)(arg + 0x78);
    arg[0x20] = 2;
    qcopy(dst, a);
    qcopy(dst + 0x10, b);
}
#include "rnc/gameplay/entities/moby.h"
extern char *D_L06_0015FFD8_301070 __asm__("D_L06_0015FFD8");
extern int scale_game_frames_301070(int) __asm__("FUN_001f96f8");
extern int FUN_001f9740_301070(void *) __asm__("FUN_001f9740");
extern void add_vector_xyz_301070(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_xyz_301070(void *, void *, void *) __asm__("FUN_001f9a28");
extern void normalize_vector_xyz_301070(void *, void *, float) __asm__("FUN_001f9bf8");
extern void mark_moby_for_removal_301070(void *) __asm__("FUN_0020c828");
extern void blend_moby_animation_301070(void *, int, int, int) __asm__("FUN_00212f90");
extern float random_float_between_301070(float, float) __asm__("FUN_002132a8");
extern int is_point_inside_clip_volume_301070(void *, int) __asm__("FUN_00214720");
typedef float Vec_301070[4] __attribute__((aligned(16)));
typedef int u128q_301070 __attribute__((mode(TI)));
typedef union { u128q_301070 q; float f[4]; } VecQ_301070 __attribute__((aligned(16)));
struct Data_301070;
typedef struct Data_301070 {
    char pad00[4];
    short f4;
    char pad06[2];
    unsigned char f8;
    char pad09[3];
    float fC;
    char pad10[0x10];
    char *f20;
    char pad24[0x1C];
    char f40[0x64];
    float fA4;
    float fA8;
    char padAC[4];
    float fB0;
    char padB4[0xC];
    char fC0[0x68];
    float f128;
    char pad12C[0x14];
    float f140[4];
    char pad150[8];
    int f158, f15C;
    int f160, f164, f168, f16C, f170;
    short f174, f176;
    float f178, f17C;
} Data_301070;
typedef struct Obj_301070 {
    char pad00[0x86];
    short f86;
} Obj_301070;
typedef struct Busy_301070 {
    int f0;
    char pad4[0x20];
    int f24;
} Busy_301070;

extern void FUN_L03_002db480_301070(char *) __asm__("FUN_L03_002db480");
extern float distance_xyz_301070(void *, void *) __asm__("FUN_001f9b48");
extern void FUN_L00_0025a120_301070(void *) __asm__("FUN_L00_0025a120");
extern void FUN_L06_002f53e8_301070(char *) __asm__("FUN_L06_002f53e8");
extern void FUN_L06_002f5360_301070(char *) __asm__("FUN_L06_002f5360");
extern float probe_ground_height_301070(void *, int, float) __asm__("FUN_00213508");
extern int initialize_npc_dialogue_301070(void *, void *) __asm__("FUN_L00_002668a0");
extern int FUN_L06_002e9ea0_301070(int) __asm__("FUN_L06_002e9ea0");
extern int FUN_001f9770_301070(void *) __asm__("FUN_001f9770");
extern int update_npc_dialogue_301070(void *, void *) __asm__("FUN_L00_00266448");
extern void begin_dialogue_camera_transition_301070(float, void *) __asm__("FUN_L01_002783a8");
extern void FUN_L00_002502a0_301070(int) __asm__("FUN_L00_002502a0");
extern void FUN_L00_00260860_301070(int, int) __asm__("FUN_L00_00260860");
extern int memcard_save_data_301070(int, int) __asm__("FUN_0020b178");
extern int help_request_301070(int a, int b) __asm__("FUN_L00_00203908");
extern void cutscene_camera_start_301070(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L00_00216f90_301070(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern float advance_accelerated_scalar_301070(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void lerp_vector_301070(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern float fast_subtract_rotations_301070(float, float) __asm__("FUN_001fa5c8");
extern float fast_add_rotations_301070(float, float) __asm__("FUN_001fa580");
extern void FUN_L00_002ea9d8_301070(void *) __asm__("FUN_L00_002ea9d8");
extern void FUN_L00_002eaa30_301070(void *) __asm__("FUN_L00_002eaa30");
extern float approach_value_301070(float *, float, float) __asm__("FUN_00213ed8");
extern void cutscene_camera_stop_301070(int) __asm__("FUN_L00_002eac18");
extern float vector_distance_301070(void *, void *) __asm__("FUN_001f9b80");
extern float angle_atan2_301070(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations_301070(float, float) __asm__("FUN_001fa688");
extern float vector_length_xyz_301070(void *) __asm__("FUN_001f9af0");
extern float scale_time_301070(float) __asm__("FUN_001f96b0");
extern int truncate_float_to_s32_301070(float) __asm__("FUN_001fa6d0");
extern void build_spherical_offset_301070(void *, float, float, float) __asm__("FUN_00214db0");
extern float vector_length_xy_301070(void *) __asm__("FUN_001f9b20");
extern void update_joint_animation_301070(float, float, void *, void *, int) __asm__("FUN_L00_002628d8");

extern float D_L06_00167540[];
extern int D_L06_0015F5C4_macro __asm__("D_L06_0015F5C4") __attribute__((section(".sdata")));
extern float D_L06_0015F3FC_macro __asm__("D_L06_0015F3FC") __attribute__((section(".sdata")));
extern s32 D_L06_00179B90[] __asm__("D_L06_00179B90");
extern unsigned char big_head_cheat_enabled_301070[] __asm__("D_0015EDB0") __attribute__((section(".sdata")));
extern unsigned char cheats_301070[] __asm__("D_0015EDB0") __attribute__((section(".sdata")));
extern int D_0013D8AC_lui __asm__("D_0013D8AC") __attribute__((section(".data")));

char D_L06_00201880[] = "Scrawny Scientist";
extern Obj_301070 *D_L06_00167580 __attribute__((section(".data")));
extern unsigned char D_0013D505[];
extern f32 frame_time_macro __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern f32 frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
extern f32 frame_scale_sq_macro __asm__("D_0015ED64") __attribute__((section(".sdata")));
/* Update of moby class 1105 (fred): steps the conversation and camera states, then turns the head toward the hero. */
void FUN_L06_00301070(struct Moby *m) {
    Data_301070 *d = *(struct Data_301070 **)&m->pvars;
    float rate, head_rate;
    int tracking;
    FUN_L03_002db480_301070((char *)m);
    if (m->visible != 0 && distance_xyz_301070(((float *)&m->pos), ((char *)D_L06_00167540)) < 24.0f) {
        FUN_L00_0025a120_301070(m);
        m->shadow_range = 0x13;
    }
    switch (m->state) {
    case 0:
        if (D_0013D505[0] != 0) goto del;
        if (D_0013D8AC_lui == 0) {
            if (m->prev_seq != 1) {
                blend_moby_animation_301070(m, 1, 0, scale_game_frames_301070(10));
            }
            d->f176 = 1;
            FUN_L06_002f53e8_301070(D_L06_0015FFD8_301070 + (d->f164 << 8));
        } else {
            {
                Vec_301070 t;
                normalize_vector_xyz_301070(t, ((char *)&m->rot_row0), 2.0f);
                add_vector_xyz_301070(((float *)&m->pos), ((float *)&m->pos), t);
                m->pos.z = probe_ground_height_301070(((float *)&m->pos), 0, 0.5f);
                d->f176 = 0;
            }
        }
        d->f174 = scale_game_frames_301070(0x2D);
        FUN_L06_002f53e8_301070(D_L06_0015FFD8_301070 + (d->f160 << 8));
        d->f20 = D_L06_00201880;
        m->state = 1;
        initialize_npc_dialogue_301070(m, d);
        break;
    case 1:
        if (d->f8 == 1 && is_point_inside_clip_volume_301070(&hero.motion.pos, d->f168) != 0) {
            d->fC = 16.0f;
        } else {
            d->fC = 3.7f;
        }
        if (FUN_L06_002e9ea0_301070(d->f170) != 0) {
            if (FUN_001f9770_301070(&d->f174) != 0 && update_npc_dialogue_301070(m, d) != 0) {
                m->state = 2;
                if (d->f176 != 0) {
                    Vec_301070 t;
                    normalize_vector_xyz_301070(t, ((char *)&m->rot_row0), 2.0f);
                    add_vector_xyz_301070(((float *)&m->pos), ((float *)&m->pos), t);
                    m->pos.z = probe_ground_height_301070(((float *)&m->pos), 0, 0.5f);
                    d->f176 = 0;
                }
                begin_dialogue_camera_transition_301070(2.2f, m);
            }
        }
        if (hero.state.control_mode == 15) {
            FUN_L06_002f5360_301070(D_L06_0015FFD8_301070 + (d->f160 << 8));
        }
        break;
    case 2:
        if (D_L06_0015F5C4_macro != 2) {
            if (m->prev_seq != 0) {
                blend_moby_animation_301070(m, 0, 0, scale_game_frames_301070(10));
            }
            if (d->f4 < 2) {
                FUN_L00_002502a0_301070(m->mission);
                FUN_L06_002f5360_301070(D_L06_0015FFD8_301070 + (d->f164 << 8));
                m->state = 1;
            }
            if (d->f4 == 2) {
                FUN_L00_00260860_301070(0x1D, 1);
                memcard_save_data_301070(0, -1);
                help_request_301070(0x1775, 0x2C);
                D_L06_0015F3FC_macro = 1.0f;
                d->f178 = 0.0f;
                {
                    VecQ_301070 pos;
                    VecQ_301070 rot;
                    pos.q = 0;
                    rot.q = 0;
                    pos.f[0] = 178.47f;
                    pos.f[1] = 319.76f;
                    pos.f[2] = 146.31f;
                    rot.f[0] = 0.07f;
                    rot.f[1] = 0.18f;
                    rot.f[2] = -1.77f;
                    cutscene_camera_start_301070(pos.f, rot.f, 1, 0, 0);
                }
                FUN_L00_00216f90_301070(&hero.motion.pos, &hero.motion.rot, 0x72, 1);
                FUN_L06_002f53e8_301070(D_L06_0015FFD8_301070 + (d->f164 << 8));
                m->state = 3;
                m->visible = 0;
                m->flags |= 1;
                *(int *)&m->collision = 0;
            }
        }
        break;
    case 3: {
      {
        VecQ_301070 p0;
        VecQ_301070 p1;
        VecQ_301070 r0;
        VecQ_301070 r1;
        Vec_301070 pos;
        Vec_301070 rot;
        p0.q = 0;
        p1.q = 0;
        r0.q = 0;
        r1.q = 0;
        p0.f[0] = 178.47f;
        p0.f[1] = 319.76f;
        p0.f[2] = 146.31f;
        p1.f[0] = 172.55f;
        p1.f[1] = 316.01f;
        p1.f[2] = 145.27f;
        r0.f[0] = 0.07f;
        r0.f[1] = 0.18f;
        r0.f[2] = -1.77f;
        r1.f[0] = 0.07f;
        r1.f[1] = 0.13f;
        r1.f[2] = -1.21f;
        advance_accelerated_scalar_301070(&d->f178, &d->f17C, 1.0f, frame_time_sq_macro * 0.1f, frame_time_sq_macro * 0.1f, frame_time_macro * 0.1f);
        if (d->f178 > 0.1f) {
            FUN_L06_002f5360_301070(D_L06_0015FFD8_301070 + (d->f160 << 8));
        }
        lerp_vector_301070(pos, p0.f, p1.f, d->f178);
        rot[2] = fast_add_rotations_301070(fast_subtract_rotations_301070(r1.f[2], r0.f[2]) * d->f178, r0.f[2]);
        rot[1] = fast_add_rotations_301070(fast_subtract_rotations_301070(r1.f[1], r0.f[1]) * d->f178, r0.f[1]);
        rot[0] = fast_add_rotations_301070(fast_subtract_rotations_301070(r1.f[0], r0.f[0]) * d->f178, r0.f[0]);
        if (D_L06_00167580 != 0 && D_L06_00167580->f86 != 5) {
            cutscene_camera_start_301070(p0.f, r0.f, 1, 0, 0);
        }
        FUN_L00_002ea9d8_301070(pos);
        FUN_L00_002eaa30_301070(rot);
        if (d->f178 >= 1.0f
            || (((Busy_301070 *)D_L06_00179B90)->f0 == 0 && ((Busy_301070 *)D_L06_00179B90)->f24 == -1 && d->f178 > 0.2f)) {
            approach_value_301070(&D_L06_0015F3FC_macro, 1.0f, frame_time_macro * 4.0f);
        } else {
            approach_value_301070(&D_L06_0015F3FC_macro, 0.0f, frame_time_macro * 4.0f);
        }
        if (D_L06_0015F3FC_macro == 1.0f) {
            FUN_L00_00216f90_301070(&hero.motion.pos, &hero.motion.rot, 0, 0);
            cutscene_camera_stop_301070(0);
            m->state = 4;
        }
      }
        break;
    }
    case 4:
        approach_value_301070(&D_L06_0015F3FC_macro, 0.0f, frame_time_macro * 4.0f);
        if (D_L06_0015F3FC_macro == 0.0f) {
            m->state = 5;
        }
        break;
    case 5:
        if (hero.state.control_mode == 15) {
            FUN_L06_002f5360_301070(D_L06_0015FFD8_301070 + (d->f164 << 8));
        del:
            mark_moby_for_removal_301070(m);
            return;
        }
        break;
    }
    rate = 0.02f;
    head_rate = 0.3f;
    tracking = 0;
    {
        Vec_301070 target;
        Vec_301070 eye;
        Vec_301070 delta;
        if (m->prev_seq == 0) {
            char *player = &hero.motion.pos;
            tracking = 1;
            if (vector_distance_301070(((float *)&m->pos), player) < 8.0f &&
                fast_difference_between_rotations_301070(m->rot.z,
                    angle_atan2_301070(hero.motion.unkD0.f[0] - m->pos.x,
                        hero.motion.unkD0.f[1] - m->pos.y)) < 1.5707964f) {
                if (vector_length_xyz_301070(player + 0x80) > 0.01f) d->f158 = scale_game_frames_301070(120);
                else FUN_001f9740_301070(&d->f158);
            } else if (d->f158) {
                d->f158 = 0;
                qcopy(d->f140, &hero.motion.unkD0);
            }
            if (FUN_001f9740_301070(&d->f15C)) {
                float heading;
                d->f15C = truncate_float_to_s32_301070(scale_time_301070(random_float_between_301070(180.0f, 300.0f)));
                heading = fast_add_rotations_301070(m->rot.z, random_float_between_301070(-90.0f, 90.0f) * 0.017453292f);
                build_spherical_offset_301070(d->f140, 6.0f, heading, random_float_between_301070(0.0f, 30.0f) * 0.017453292f);
                add_vector_xyz_301070(d->f140, d->f140, ((float *)&m->pos));
            }
            if (d->f158) {
                qcopy(target, &hero.motion.unkD0);
                rate = 0.04f;
                head_rate = 0.3f;
            } else {
                qcopy(target, d->f140);
            }
        }
        if (tracking) {
            float yaw;
            float pitch;
            qcopy(eye, ((float *)&m->pos));
            eye[2] += 1.0f;
            subtract_vector_xyz_301070(delta, target, eye);
            yaw = fast_subtract_rotations_301070(angle_atan2_301070(delta[0], delta[1]), m->rot.z);
            pitch = -angle_atan2_301070(vector_length_xy_301070(delta), delta[2]);
            if (yaw > 1.5707964f) yaw = 1.5707964f;
            else if (yaw < -1.5707964f) yaw = -1.5707964f;
            if (pitch > 0.5235988f) pitch = 0.5235988f;
            else if (pitch < -0.5235988f) pitch = -0.5235988f;
            d->fA4 = pitch;
            d->fA8 = yaw * 0.6f;
            d->f128 = yaw * 0.4f;
        }
    }
    if (cheats_301070[0]) d->fB0 = 2.75f;
    update_joint_animation_301070(rate * frame_scale_sq_macro, head_rate * frame_scale_sq_macro, m, d->f40, 0);
    update_joint_animation_301070(rate * frame_scale_sq_macro, head_rate * frame_scale_sq_macro, m, d->fC0, 2);
}
extern unsigned char D_0014C050[];
extern char *D_L06_0015FFD8_301b90 __asm__("D_L06_0015FFD8");
extern char *D_L06_001600EC_301b90 __asm__("D_L06_001600EC");
extern int *D_L06_001B0C30_301b90[] __asm__("D_L06_001B0C30");
extern char D_0015172A[] __asm__("D_0015172A");
extern int D_L06_0015F5C4 __asm__("D_L06_0015F5C4");
extern float D_L06_00167560 __asm__("D_L06_00167560");
extern int D_L06_00167568 __asm__("D_L06_00167568");
extern float D_L06_00162098 __asm__("D_L06_00162098") __attribute__((sda));
extern float D_L06_0016209C __asm__("D_L06_0016209C") __attribute__((sda));
extern float D_L06_001620A0 __asm__("D_L06_001620A0") __attribute__((sda));
extern float D_L06_001620A4 __asm__("D_L06_001620A4") __attribute__((sda));
extern float D_L06_001620A8 __asm__("D_L06_001620A8") __attribute__((sda));
extern float D_L06_001620B0 __asm__("D_L06_001620B0") __attribute__((sda));
extern float D_L06_001620C4 __asm__("D_L06_001620C4") __attribute__((sda));
extern int D_L06_0015F880 __asm__("D_L06_0015F880");
extern void FUN_L06_002f45f0(int);
extern void FUN_L06_002f9948(int);
extern int FUN_L01_0026e008(int, int);
extern void FUN_L06_003047b8(char *);
extern int FUN_L06_003047f8(const u8 *);
extern void FUN_L06_00302400(struct Moby *, int);
extern void FUN_L06_002ffc38(int);
extern void FUN_L06_002e9e30(int);
extern void FUN_L06_002f45a0(int);
extern void FUN_L06_002f99b0(int);
extern void FUN_L06_002fc630(char *);
extern void FUN_L06_00309240(int);
extern void FUN_L06_00312a78(void *);
extern void FUN_L06_00312ad0(void *);
extern void FUN_L06_00312ce8(int);
extern void FUN_L06_00312b40(float, float, float, float, float, float);
extern void FUN_L00_002eaaa0(void *, void *, int, int, int);
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void FUN_L06_002356a0(int, int);
extern void FUN_L06_00304088(char *);
extern void FUN_001f99f8(void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float vector_distance_xy(void *, void *) __asm__("FUN_001f9b80");
extern int FUN_001f96f8(int);
extern float FUN_001f96b0(float) __asm__("FUN_L06_00216fa8");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern int FUN_001f9740(void *);
extern float FUN_002132a8(float, float);
extern int FUN_L01_0027a248(int, int);
extern void continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");
extern int FUN_L00_00211250(void);
extern void FUN_001f4600(int, char *) __asm__("FUN_L06_00210fc8");
extern void FUN_L06_003021d8(char *);
extern void allocate_voice_for_group_entry(int, int, int) __asm__("FUN_0022dba0");
extern void FUN_L06_002f45f0(int);
extern void FUN_L00_002eac18(int);
extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");
extern void FUN_L02_002f8aa0(float, float, float, float, float, float);

extern float D_L06_0015F400;
void FUN_L06_00301b90(unsigned char *m) {
    char *d = *(char **)(m + 0x78);
    float va[4] __attribute__((aligned(16)));
    float vc[4] __attribute__((aligned(16)));
    float vb[4] __attribute__((aligned(16)));
    switch (m[0x20]) {
    case 0:
        if (D_0014C050[m[0xB0] + (current_level_index << 4)] != 0) {
            mark_moby_for_removal((struct Obj *)m);
            return;
        }
        FUN_L06_002f45f0(*(int *)(d + 0));
        FUN_L06_002f9948(*(int *)(d + 4));
        FUN_L06_002f9948(*(int *)(d + 8));
        m[0x20] = 1;
        m[0x30] = 0xFF;
        return;
    case 1:
        if (FUN_L01_0026e008(*(int *)(d + 0x1C), -1) != 0) return;
        FUN_L06_003047b8(D_L06_0015FFD8_301b90 + (*(int *)(d + 0x2C) << 8));
        if (FUN_L06_003047f8((const u8 *)D_L06_0015FFD8_301b90 + (*(int *)(d + 0x2C) << 8)) != 1) return;
        m[0x20] = 2;
        FUN_L06_00302400((struct Moby *)m, 0);
        return;
    case 2: {
        int *p;
        float r;
        if (FUN_L06_003047f8((const u8 *)D_L06_0015FFD8_301b90 + (*(int *)(d + 0x2C) << 8)) != 2) return;
        m[0x20] = 3;
        *(int *)(d + 0xB0) = 1;
        FUN_L01_0027a248(6, 9);
        if (*(short *)D_0015172A == 3) continue_audio_stream_if_ready();
        FUN_L06_002ffc38(*(int *)(d + 0x10));
        FUN_L06_002e9e30(*(int *)(d + 0x14));
        FUN_L06_002e9e30(*(int *)(d + 0x18));
        FUN_L06_002f45a0(*(int *)(d + 0));
        FUN_L06_002f99b0(*(int *)(d + 4));
        FUN_L06_002f99b0(*(int *)(d + 8));
        FUN_L06_002fc630(D_L06_0015FFD8_301b90 + (*(int *)(d + 0x20) << 8));
        {
            *(float *)((char *)((*(int *)(d + 0xC) << 7) + (int)D_L06_001600EC_301b90) + 0x38) += 10.0f;
            p = D_L06_001B0C30_301b90[*(int *)(d + 0x24)];
            qcopy(va, (char *)p + 0x20);
            qcopy(vb, (char *)p + 0x10);
            FUN_001f99f8(vc);
            vc[2] = fast_arc_tan(vb[0] - va[0], vb[1] - va[1]);
            r = vector_distance_xy(va, vb);
            vc[1] = -fast_arc_tan(r, vb[2] - va[2]);
            FUN_L00_002eaaa0(va, vc, 2, FUN_001f96f8(300), 0);
            FUN_L06_00312b40(D_L06_00162098, D_L06_0016209C,
                               D_L06_001620A0, D_L06_001620A4,
                               D_L06_001620A8, D_L06_001620B0);
            FUN_L06_00312a78(va);
            FUN_L06_00312ad0(vc);
            {
                char *q = D_L06_001600EC_301b90 + (*(int *)(d + 0x28) << 7);
                FUN_L00_00216f90(q + 0x30, q + 0x70, 0x72, 0);
            }
        }
        return;
    }
    case 3: {
        int **p;
        int x, nb;
        float r;
        p = &D_L06_001B0C30_301b90[*(int *)(d + 0x24)];
        nb = *(int *)(d + 0xB0);
        qcopy(vb, (char *)((nb << 4) + (int)*p) + 0x10);
        nb++;
        *(int *)(d + 0xB0) = nb;
        if (nb == **p - 1) {
            m[0x20] = 4;
            FUN_L06_00302400((struct Moby *)m, 1);
            *(int *)(d + 0xB4) = truncate_float_to_s32(FUN_001f96b0(D_L06_001620C4));
            FUN_L06_00312ce8(2);
            FUN_L06_002356a0(0, 1);
            D_L06_0015F404 = 0;
            return;
        }
        FUN_L06_00309240(*(int *)(d + 0x34));
        x = *(int *)(d + 0xB0) - 30;
        if (x < 0) x = 0;
        qcopy(va, (char *)D_L06_001B0C30_301b90[*(int *)(d + 0x24)] + 0x10 + (x << 4));
        FUN_001f99f8(vc);
        vc[2] = fast_arc_tan(va[0] - vb[0], va[1] - vb[1]);
        r = vector_distance_xy(vb, va);
        vc[1] = -fast_arc_tan(r, va[2] - vb[2]);
        FUN_L06_00312a78(vb);
        FUN_L06_00312ad0(vc);
        return;
    }
    case 4: {
        int x;
        char *q;
        x = FUN_001f96f8(0x708);
        if (*(int *)(d + 0xB4) == x) {
            if (*(short *)D_0015172A == 3) continue_audio_stream_if_ready();
        } else {
            x = FUN_001f96f8(900);
            if (*(int *)(d + 0xB4) == x) FUN_L06_00302400((struct Moby *)m, 2);
            else {
                x = FUN_001f96f8(300);
                if (*(int *)(d + 0xB4) == x && *(short *)D_0015172A == 3) continue_audio_stream_if_ready();
            }
        }
        if (FUN_001f9740(d + 0xC0) != 0) {
            allocate_voice_for_group_entry(0, 0, (int)hero.moby);
            *(int *)(d + 0xC0) = truncate_float_to_s32(FUN_001f96b0(FUN_002132a8(480.0f, 600.0f)));
            D_L06_00167400[88] = 0.4f;
            ((int *)D_L06_00167400)[90] = FUN_001f96f8(60);
        }
        if (D_L06_0015F5C4 == 0) FUN_001f4600((int)FUN_L06_003021d8, (char *)m);
        if (hero.state.current == 0x72) { m[0x20] = 6; return; }
        if (D_L06_0015F5C4 != 0) return;
        if (FUN_001f9740(d + 0xB4) == 0) return;
        m[0x20] = 5;
        x = FUN_001f96f8(15);
        q = D_L06_0015FFD8_301b90 + (*(int *)(d + 0x30) << 8);
        *(float *)(d + 0xB8) = 0.0f;
        *(int *)(d + 0xBC) = x;
        FUN_L06_00304088(q);
        allocate_voice_for_group_entry(1, 0, (int)hero.moby);
        return;
    }
    case 5:
        if (*(float *)(d + 0xB8) == 1.0f) {
            FUN_L00_00211250();
            D_L06_0015F400 = 0.0f;
            return;
        }
        if (FUN_001f9740(d + 0xBC) == 0) return;
        approach_value((float *)(d + 0xB8), 1.0f, frame_time * 6.0f);
        D_L06_0015F400 = *(float *)(d + 0xB8);
        return;
    }
}
#include "rnc/gameplay/entities/moby.h"

struct TauntVoiceVars {
    u8 pad0[0x38];
    s32 greet[6];        /* 0x38: one per language */
    s32 leave[6];        /* 0x50 */
    s32 chatter[3][6];   /* 0x68 */
};
extern s32 D_L06_001620B8[1] __attribute__((sda));
extern s32 D_L06_001620BC;
extern s32 D_L06_001620C0;
extern s32 D_001516EC[];
extern s32 FUN_00213260(s32);

/* plays one of the vendor's greet, chatter or leave lines in the current language */
void FUN_L06_00302400(struct Moby *m, s32 kind) {
    struct TauntVoiceVars *v = (struct TauntVoiceVars *)m->pvars;
    s32 id = -1;
    s32 lang;
    s32 r;
    s32 i;

    switch (kind) {
    case 0:
        lang = game_language;
        if (lang != 0) lang--;
        id = v->greet[lang];
        break;
    case 1:
        r = FUN_00213260(3);
        for (i = 0; i < 4; i++) {
            if (D_L06_001620B8[r] == 0) {
                D_L06_001620B8[r] = 1;
                break;
            }
            r = (r + 1) % 3;
            if (i == 2) {
                D_L06_001620B8[0] = 0;
                D_L06_001620BC = 0;
                D_L06_001620C0 = 0;
            }
        }
        lang = game_language;
        if (lang != 0) lang--;
        id = v->chatter[r][lang];
        break;
    case 2:
        lang = game_language;
        if (lang != 0) lang--;
        id = v->leave[lang];
        break;
    }
    if (id != -1) {
        id += 20000;
        D_001516EC[0] = id;
    }
}
typedef int u128_302578 __attribute__((mode(TI)));
typedef struct {
    char pad0[0x24];
    float f24;
} Cls_302578;
typedef struct {
    char pad0[0x20];
    char area[0x40];
    int a[5];
    int b[5];
    char pad88[0x14];
    int f9C;
    int cam[3];
    int cut;
    int fB0;
    char padB4[8];
    int sel;
    int side;
    float fC4;
    float fC8;
    char padCC[0x24];
    int fF0;
    int fF4;
    int fF8;
    int fFC;
    int zone[3];
    int f10C;
    int go;
    int f114;
} Data_302578;
typedef struct {
    char pad0[0x2FC];
    struct Moby *f2FC;
    char pad300[0xE];
    short f30E;
} Hero_302578;
typedef struct {
    char pad0[0x21];
    unsigned char f21;
} Flags_302578;
typedef struct {
    int f0;
    char pad4[0x20];
    int f24;
} Busy_302578;
extern unsigned char D_0014C050[];
extern int D_0015EE1D;
extern int D_0015EE1C;
extern int D_L06_0015F5C4;
extern int D_L06_0015F594 __asm__("D_L06_0015F594");
extern char *D_L06_001600EC_302578 __asm__("D_L06_001600EC");
extern char *D_L06_0015FFD8_302578 __asm__("D_L06_0015FFD8");
extern int D_L06_00179B90[] __asm__("D_L06_00179B90");
extern char *D_L06_001B0C30_302578[] __asm__("D_L06_001B0C30");
extern char D_0013D3AC_302578[] __asm__("D_0013D3A7") __attribute__((section(".data")));
extern char D_0013D388_302578[] __asm__("D_0013D388") __attribute__((section(".data")));
extern char D_0013A4E0_302578[] __asm__("D_0013A4E0") __attribute__((section(".data")));
extern void FUN_L06_00302fd8(char *);
extern void FUN_L06_00304028(char *);
extern int FUN_L06_00303160(char *, int);
extern int scale_game_frames_302578(int) __asm__("FUN_001f96f8");
extern void FUN_L06_0030bef8_302578(void *, int, int, int) __asm__("FUN_L06_0030bef8");
extern void blend_moby_animation_302578(void *, int, int, int) __asm__("FUN_00212f90");
extern int is_point_inside_clip_volume(void *, int);
extern void play_sound_302578(int, int) __asm__("FUN_L01_0027a248");
extern int FUN_L00_002644e0_302578(int) __asm__("FUN_L00_002644e0");
extern void fade_to_black_302578(int) __asm__("FUN_001f4a58");
extern float angle_atan2_302578(float, float) __asm__("FUN_001f9e90");
extern float vector_distance_302578(void *, void *) __asm__("FUN_001f9b80");
extern void FUN_L00_002eaaa0_302578(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L02_002f8aa0_302578(float, float, float, float, float, float) __asm__("FUN_L02_002f8aa0");
extern void FUN_L00_002ea9d8_302578(void *) __asm__("FUN_L00_002ea9d8");
extern void FUN_L00_002eaa30_302578(void *) __asm__("FUN_L00_002eaa30");
extern void FUN_L00_00216f90_302578(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void vu_euler_rotation_basis_302578(void *, void *) __asm__("FUN_001fa030");
extern float fast_add_rotations_302578(float, float) __asm__("FUN_001fa580");
extern void FUN_L06_002e6148_302578(void *) __asm__("FUN_L06_002e6148");
extern void FUN_L00_002eac18_302578(int) __asm__("FUN_L00_002eac18");
extern void FUN_L00_00298840_302578(int) __asm__("FUN_L00_00298840");
extern void FUN_L00_002607d0_302578(int) __asm__("FUN_L00_002607d0");
extern void FUN_L00_00299460_302578(int) __asm__("FUN_L00_00299460");
extern void FUN_L00_00262d38_302578(int) __asm__("FUN_L00_00262d38");
extern void FUN_L00_002502a0_302578(int) __asm__("FUN_L00_002502a0");
extern void FUN_L00_00284e50_302578(void *, void *) __asm__("FUN_L00_00284e50");
extern void FUN_L06_00239528_302578(void) __asm__("FUN_L06_00239528");
extern int memcard_Save_302578(int, long) __asm__("FUN_0020b178");
extern void subtract_vector_xyz_302578(void *, void *, void *) __asm__("FUN_001f9a28");
extern void FUN_L00_00260738_302578(void *, void *, void *, void *) __asm__("FUN_L00_00260738");
extern void allocate_voice_302578(int, int, void *) __asm__("FUN_0022da68");
extern void clear_vector_302578(void *) __asm__("FUN_001f99f8");
extern void normalize_vector_xyz_302578(void *, void *, float) __asm__("FUN_001f9bf8");
extern void add_vector_xyz_302578(void *, void *, void *) __asm__("FUN_001f9a10");
extern void delete_moby_302578(void *) __asm__("FUN_0020c828");
extern int D_L06_0015FDF8;
extern float D_L06_001620F4 __attribute__((sda));
extern float D_L06_001620F8 __attribute__((sda));
extern int D_L06_00162100 __attribute__((sda));
extern int D_L06_00162104 __attribute__((sda));
extern int D_L06_00162108 __attribute__((sda));
extern int D_L06_0016210C __attribute__((sda));
extern int D_L06_00162110 __attribute__((sda));
extern unsigned char D_0013D4D5[] __attribute__((section(".data")));
/* Update of moby class 1109 (shuttle): waits at one of three camera points, takes the hero's choice of destination, flies there through a cutscene and hands over to the level's travel sequence. */
void FUN_L06_00302578(struct Moby *moby) {
    Data_302578 *d = *(Data_302578 **)&moby->pvars;
    float old_pos[4];
    float old_rot[4];
    float delta[4];
    float b[4];
    float rot[4];
    float a[4];
    int idx;
    int busy;
    int now;

    qcopy(old_pos, ((float *)&moby->pos));
    qcopy(old_rot, ((float *)&moby->rot));
    moby->scale = moby->pclass->scale * D_L06_001620F8;
    if (D_L06_00162104 != 0) {
        D_L06_00162104 = 0;
        *(u128_302578 *)b = *(u128_302578 *)((char *)&hero.motion.pos + 0x50);
        FUN_L06_0030bef8_302578((char *)b, scale_game_frames_302578(D_L06_00162108), D_L06_0016210C, D_L06_00162110);
    }
    switch (moby->state) {
    case 0:
        D_L06_0015FDF8 = 0;
        if (*(unsigned char *)(d->fF8 + (current_level_index << 4) + (int)D_0014C050) == 0xFF) {
            FUN_L06_00304028((char *)moby);
        } else {
            D_0013D3AC_302578[5] = 0;
        }
        moby->update_dist = 0xFF;
        moby->draw_dist = 0x80;
        moby->spawn_frame = hero.moby->spawn_frame;
        moby->state = 1;
        if (moby->prev_seq != 2) {
            blend_moby_animation_302578(moby, 2, 0, 0);
        }
        d->sel = 0;
        d->side = 0;
        break;
    case 1:
        busy = 0;
        if (*(unsigned char *)&D_0015EE1D != 0 || *(unsigned char *)&D_0015EE1C != 0) {
            if (((Busy_302578 *)D_L06_00179B90)->f0 != 0 || ((Busy_302578 *)D_L06_00179B90)->f24 != -1) {
                busy = 1;
            }
        }
        if ((busy == 0 && ((Hero_302578 *)((char *)&hero))->f2FC == moby && ((Hero_302578 *)((char *)&hero))->f30E == 0)
            || D_L06_00162100 != 0) {
            FUN_L06_00302fd8((char *)moby);
            now = D_L06_00162100;
            if (now != 0 || ((*(int *)(D_0013A4E0_302578 + 0x2604) & 0x10) && D_L06_0015F594 == 10 && d->f9C >= 0)) {
                d->go = 1;
                D_L06_0015FDF8 = 0;
                if (now != 0) {
                    d->f9C = 2;
                    D_L06_00162100 = 0;
                }
                switch (d->f9C) {
                case 0:
                    d->sel = 1;
                    d->fC4 = D_L06_001620F8;
                    d->fC8 = D_L06_001620F4;
                    d->side = 0;
                    play_sound_302578(4, 8);
                    break;
                case 1:
                    d->side = 1;
                    d->fC4 = D_L06_001620F4;
                    d->fC8 = D_L06_001620F8;
                    d->sel = 0;
                    play_sound_302578(0, 10);
                    break;
                case 2:
                    d->side = 2;
                    d->fC4 = D_L06_001620F4;
                    d->fC8 = D_L06_001620F8;
                    d->sel = 0;
                    break;
                case 3:
                    d->side = 3;
                    d->sel = 2;
                    d->fC4 = D_L06_001620F8;
                    d->fC8 = D_L06_001620F8;
                    break;
                case 4:
                    d->side = 4;
                    d->sel = 0;
                    d->fC4 = D_L06_001620F8;
                    d->fC8 = D_L06_001620F8;
                    break;
                }
            }
        } else if (d->go == 0) {
            if (d->sel == 0 && is_point_inside_clip_volume((char *)&hero.motion.pos, d->zone[0]) != 0) {
                int base = (int)D_L06_001600EC_302578;
                int ofs = d->cam[2] << 7;
                qcopy(((float *)&moby->pos), (char *)(ofs + base + 0x30));
                qcopy(((float *)&moby->rot), (char *)(ofs + base + 0x70));
                d->sel = 2;
                D_L06_0015FDF8 = 0;
            } else if (d->sel == 2 && is_point_inside_clip_volume((char *)&hero.motion.pos, d->zone[1]) != 0) {
                int ofs = d->cam[0] << 7;
                int base = (int)D_L06_001600EC_302578;
                qcopy(((float *)&moby->pos), (char *)(ofs + base + 0x30));
                qcopy(((float *)&moby->rot), (char *)(ofs + base + 0x70));
                d->sel = 0;
                D_L06_0015FDF8 = 0;
            } else if (d->sel != 1 && is_point_inside_clip_volume((char *)&hero.motion.pos, d->zone[2]) != 0) {
                int base = (int)D_L06_001600EC_302578;
                int ofs = d->cam[1] << 7;
                qcopy(((float *)&moby->pos), (char *)(ofs + base + 0x30));
                qcopy(((float *)&moby->rot), (char *)(ofs + base + 0x70));
                d->sel = 1;
                D_L06_0015FDF8 = 1;
                if (((Flags_302578 *)(D_0013D388_302578))->f21 == 0) {
                    ((Flags_302578 *)(D_0013D388_302578))->f21 = 1;
                }
            }
        }
        if (d->go != 0) {
            d->go = 0;
            moby->state = 3;
            FUN_L00_002644e0_302578(10);
            if (moby->prev_seq != 0) {
                blend_moby_animation_302578(moby, 0, 0, 0);
            }
            fade_to_black_302578(scale_game_frames_302578(10));
            d->fB0 = 0;
            idx = d->side;
            qcopy(a, ((char **)D_L06_001B0C30_302578)[d->a[idx]] + 0x10);
            qcopy(b, ((char **)D_L06_001B0C30_302578)[d->b[idx]] + 0x10);
            clear_vector_302578(rot);
            rot[2] = angle_atan2_302578(b[0] - a[0], b[1] - a[1]);
            rot[1] = -angle_atan2_302578(vector_distance_302578(a, b), b[2] - a[2]);
            FUN_L00_002eaaa0_302578(a, rot, 2, scale_game_frames_302578(300), 0);
            FUN_L02_002f8aa0_302578(0.001f, 1.0f, 1.0f, 0.001f, 1.0f, 1.0f);
            FUN_L00_002ea9d8_302578(a);
            FUN_L00_002eaa30_302578(rot);
            FUN_L00_00216f90_302578(D_L06_001600EC_302578 + (d->cut << 7) + 0x30, D_L06_001600EC_302578 + (d->cut << 7) + 0x70, 0x72, 0);
            allocate_voice_302578(2, 0, moby);
        }
        break;
    case 3:
        if (FUN_L06_00303160((char *)moby, d->side) != 0) {
            int ofs;
            int base;
            moby->state = 4;
            d->fF4 = 0;
            ofs = d->cam[d->sel] << 7;
            base = (int)D_L06_001600EC_302578;
            qcopy(((float *)&moby->pos), (char *)(ofs + base + 0x30));
            qcopy(((float *)&moby->rot), (char *)(ofs + base + 0x70));
            vu_euler_rotation_basis_302578(((float *)&moby->rot_row0), ((float *)&moby->rot));
            moby->scale = moby->pclass->scale;
            if (d->sel == 1) {
                D_L06_0015FDF8 = 1;
                if (((Flags_302578 *)(D_0013D388_302578))->f21 == 0) {
                    ((Flags_302578 *)(D_0013D388_302578))->f21 = 1;
                }
            } else {
                D_L06_0015FDF8 = 0;
            }
            qcopy(b, ((float *)&moby->pos));
            normalize_vector_xyz_302578(a, ((float *)&moby->rot_row0), -2.0f);
            add_vector_xyz_302578(a, a, ((float *)&moby->rot_row2));
            add_vector_xyz_302578(b, b, a);
            clear_vector_302578(rot);
            rot[2] = fast_add_rotations_302578(moby->rot.z, 3.1415927f);
            FUN_L00_00216f90_302578(b, rot, 0, 0);
            if (d->f114 != 0) {
                D_0013D3AC_302578[5] = 1;
                FUN_L06_002e6148_302578(D_L06_0015FFD8_302578 + (d->f10C << 8));
                d->f114 = 0;
                moby->state = 6;
                if (moby->prev_seq != 2) {
                    blend_moby_animation_302578(moby, 2, 0, scale_game_frames_302578(30));
                }
            }
            FUN_L00_002eac18_302578(0);
        }
        break;
    case 6:
        if (vector_distance_302578((char *)&hero.motion.pos, D_L06_0015FFD8_302578 + (d->f10C << 8) + 0x10) < 3.5f) {
            char *o = D_L06_0015FFD8_302578 + (d->f10C << 8);
            qcopy(b, o + 0x10);
            qcopy(rot, o + 0x40);
            rot[2] = fast_add_rotations_302578(rot[2], 3.14159f);
            FUN_L00_00216f90_302578(b, rot, 0x72, 1);
            play_sound_302578(0, 10);
            FUN_L00_00298840_302578(12);
            delete_moby_302578(o);
            FUN_L00_002607d0_302578(5);
            moby->state = 8;
            FUN_L00_002502a0_302578(d->fF8);
            FUN_L00_00284e50_302578(b, rot);
        }
        break;
    case 2:
        if (is_point_inside_clip_volume((char *)&hero.motion.pos, d->fF0) != 0) {
            d->side = 2;
            d->fC4 = D_L06_001620F4;
            d->fC8 = D_L06_001620F8;
            d->sel = 0;
            d->go = 1;
            d->f114 = 1;
            moby->state = 1;
        }
        break;
    case 4:
        moby->state = 1;
        if (moby->prev_seq != 2) {
            blend_moby_animation_302578(moby, 2, 0, scale_game_frames_302578(30));
        }
        break;
    case 7:
        if (D_L06_0015F5C4 != 2) {
            if (D_0013D4D5[9] != 0) {
                FUN_L00_00298840_302578(5);
                moby->state = 9;
            } else {
                FUN_L00_00262d38_302578(5);
                moby->state = 1;
                FUN_L06_00239528_302578();
                memcard_Save_302578(0, -1);
            }
        }
        break;
    case 9:
        if (D_L06_0015F5C4 != 2) {
            FUN_L00_00262d38_302578(5);
            memcard_Save_302578(0, -1);
            moby->state = 1;
        }
        break;
    case 8:
        if (D_L06_0015F5C4 != 2) {
            FUN_L00_00299460_302578(5);
            moby->state = 7;
        }
        break;
    }
    subtract_vector_xyz_302578(delta, ((float *)&moby->pos), old_pos);
    FUN_L00_00260738_302578(d->area, delta, old_rot, ((float *)&moby->rot));
}

#define NOT_SDA

#define MACRO_ADDR

/* Picks the next selection state from the menu mode, then queues the matching sound. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00304418), where it is exact; names translated to the US level program. */

extern int D_0015EE1C;
extern int D_0015EE1D;
extern int help_request(int a, int b) __asm__("FUN_L00_00203908");
extern unsigned char D_0014C050[];
extern unsigned short D_00141C00 __attribute__((section(".data")));
extern void try_set_help_message(int, int) __asm__("FUN_00215130");

void FUN_L06_00302fd8(char *m) {
    char *d = *(char **)(m + 0x78);
    switch (*(int *)(d + 0xBC)) {
    case 0: {
        int idx = current_level_index << 4;
        int a = *(unsigned char *)(*(int *)(d + 0xF8) + idx + (int)D_0014C050);
        if (a != 0xFF) {
            *(int *)(d + 0x9C) = 0;
        } else {
            int b = *(unsigned char *)(*(int *)(d + 0xFC) + idx + (int)D_0014C050);
            if (b == a) {
                *(int *)(d + 0x9C) = 3;
            } else {
                *(int *)(d + 0x9C) = -1;
            }
        }
        break;
    }
    case 1:
        *(int *)(d + 0x9C) = 1;
        break;
    case 2:
        *(int *)(d + 0x9C) = 4;
        break;
    }
    if (D_00141C00 == 0 && *(int *)(d + 0x9C) == 0 &&
        (*(unsigned char *)&D_0015EE1D != 0 || *(unsigned char *)&D_0015EE1C != 0)) {
        help_request(0x177A, 0x53);
        *(int *)(d + 0x9C) = -1;
        return;
    }
    switch (*(int *)(d + 0x9C)) {
    case 0:
        try_set_help_message(10, 0x1782);
        break;
    case 1:
    case 4:
        try_set_help_message(10, 0x1785);
        break;
    case 2:
        try_set_help_message(10, 0x1786);
        break;
    case 3:
        try_set_help_message(10, 0x1784);
        break;
    }
}
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) PathPoint_303160;
typedef struct {
    int n;
    int pad[3];
    PathPoint_303160 p[1];
} Path_303160;
struct PathRide_303160 {
    char pad0[0x60];
    int path_a[5];
    int path_b[5];
    int path_c[5];
    char pad1[0x14];
    float sway;
    float sway_speed;
    char pad2[0x0C];
    float scale_start;
    float scale_end;
    char pad3[4];
    int sound_slot[9];
    float progress;
};
extern float approach_value_303160(float *p, float target, float maxstep) __asm__("FUN_00213ed8");
extern int scale_frames_303160(int) __asm__("FUN_001f96f8");
extern int truncate_float_303160(float) __asm__("FUN_001fa6d0");
extern void lerp_vector_303160(void *, void *, void *, float) __asm__("FUN_001f9a40");
extern void set_position_303160(void *) __asm__("FUN_L00_002ea9d8");
extern float angle_atan2_303160(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations_303160(float, float) __asm__("FUN_001fa580");
extern float fast_subtract_rotations_303160(float, float) __asm__("FUN_001fa5c8");
extern float vector_distance_303160(void *, void *) __asm__("FUN_001f9b80");
extern float fast_cos_303160(float) __asm__("FUN_001f9dc8");
extern void clear_vector_303160(void *) __asm__("FUN_001f99f8");
extern void set_rotation_303160(void *) __asm__("FUN_L00_002eaa30");
extern float approach_angle_303160(float *p, float *v, float a, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern int allocate_voice_for_group_entry_303160(int, int, void *) __asm__("FUN_0022dba0");
extern void push_piece_303160(float, struct Moby *, void *, void *, int) __asm__("FUN_L06_00303748");
extern void FUN_L06_00303b68(struct Moby *);
extern char D_L06_001B0C30_303160[] __asm__("D_L06_001B0C30");
extern char *D_L06_001600EC_303160 __asm__("D_L06_001600EC");
/* Moves the moby along three paths and plays the sound marked on the path points. */
int FUN_L06_00303160(char *moby, int path) {
    float rotation[4], position[4];
    struct PathRide_303160 *ride = *(struct PathRide_303160 **)(moby + 0x78);
    Path_303160 *first_path = ((Path_303160 **)D_L06_001B0C30_303160)[ride->path_a[path]];
    Path_303160 *middle_path = ((Path_303160 **)D_L06_001B0C30_303160)[ride->path_b[path]];
    Path_303160 *third_path = ((Path_303160 **)D_L06_001B0C30_303160)[ride->path_c[path]];
    float time, first_index_f, first_fraction, third_fraction, angle, rate;
    int first_index, middle_index, third_index, marker, prev_marker;
    char *sound;

    approach_value_303160(&ride->progress, 1.0f, 1.0f / (float)scale_frames_303160(0x4B0));
    time = ride->progress * (float)(first_path->n - 1);
    first_index = truncate_float_303160(time);
    first_index_f = (float)first_index;
    first_fraction = time - first_index_f;
    middle_index = truncate_float_303160(ride->progress * (float)(middle_path->n - 1));
    time = ride->progress * (float)(third_path->n - 1);
    third_fraction = time - first_index_f;
    third_index = truncate_float_303160(time);
    lerp_vector_303160(position, &first_path->p[first_index], &first_path->p[first_index + 1], first_fraction);
    lerp_vector_303160(moby + 0x10, &third_path->p[third_index], &third_path->p[third_index + 1], third_fraction);
    set_position_303160(position);
    if (third_index > 0) {
        angle = angle_atan2_303160(third_path->p[third_index].x - third_path->p[third_index - 1].x, third_path->p[third_index].y - third_path->p[third_index - 1].y);
        *(float *)(moby + 0x48) = fast_add_rotations_303160(
            fast_subtract_rotations_303160(angle_atan2_303160(third_path->p[third_index + 1].x - third_path->p[third_index].x, third_path->p[third_index + 1].y - third_path->p[third_index].y), angle) * third_fraction,
            angle);
        angle = -angle_atan2_303160(vector_distance_303160(&third_path->p[third_index - 1], &third_path->p[third_index]), third_path->p[third_index].z - third_path->p[third_index - 1].z);
        *(float *)(moby + 0x44) = fast_add_rotations_303160(
            fast_subtract_rotations_303160(-angle_atan2_303160(vector_distance_303160(&third_path->p[third_index], &third_path->p[third_index + 1]), third_path->p[third_index + 1].z - third_path->p[third_index].z),
                           angle) * third_fraction,
            angle);
        *(float *)(moby + 0x44) = *(float *)(moby + 0x44) * fast_cos_303160(ride->sway);
        *(float *)(moby + 0x48) = fast_add_rotations_303160(*(float *)(moby + 0x48), ride->sway);
    } else {
        *(float *)(moby + 0x44) =
            -angle_atan2_303160(vector_distance_303160(&third_path->p[0], &third_path->p[1]), third_path->p[1].z - third_path->p[0].z);
        *(float *)(moby + 0x48) = angle_atan2_303160(third_path->p[1].x - third_path->p[0].x, third_path->p[1].y - third_path->p[0].y);
    }
    if (first_index > 0 && middle_index > 0) {
        clear_vector_303160(rotation);
        angle = angle_atan2_303160(middle_path->p[middle_index].x - position[0], middle_path->p[middle_index].y - position[1]);
        rotation[2] = fast_add_rotations_303160(
            fast_subtract_rotations_303160(angle_atan2_303160(middle_path->p[middle_index + 1].x - position[0], middle_path->p[middle_index + 1].y - position[1]), angle) * third_fraction, angle);
        angle = -angle_atan2_303160(vector_distance_303160(position, &middle_path->p[middle_index]), middle_path->p[middle_index].z - position[2]);
        rotation[1] = fast_add_rotations_303160(
            fast_subtract_rotations_303160(-angle_atan2_303160(vector_distance_303160(position, &middle_path->p[middle_index + 1]), middle_path->p[middle_index + 1].z - position[2]), angle) * third_fraction,
            angle);
        set_rotation_303160(rotation);
    }
    *(float *)(moby + 0x2C) =
        *(float *)(*(char **)(moby + 0x24) + 0x24) * (ride->scale_start - (ride->scale_start - ride->scale_end) * ride->progress);
    if (ride->progress > 0.8f) {
        rate = frame_time_sq * 12.566371f;
        approach_angle_303160(&ride->sway, &ride->sway_speed, 3.1415927f, rate, rate, frame_time * 6.2831855f);
    }
    marker = truncate_float_303160(first_path->p[first_index].w);
    if (first_index != 0)
        prev_marker = truncate_float_303160(first_path->p[first_index - 1].w);
    else
        prev_marker = -1;
    if (marker >= 0 || prev_marker >= 0) {
        first_path->p[first_index].w = -1.0f;
        first_path->p[first_index - 1].w = -1.0f;
        if (marker < 8 && prev_marker < 8) {
            allocate_voice_for_group_entry_303160(0, 0, moby);
            sound = D_L06_001600EC_303160 + (ride->sound_slot[marker] << 7);
            push_piece_303160(6.0f, (struct Moby *)moby, sound + 0x30, sound, 6);
        } else if (marker == 10 || prev_marker == 10) {
            allocate_voice_for_group_entry_303160(1, 0, moby);
            FUN_L06_00303b68((struct Moby *)moby);
        }
    }
    return ride->progress == 1.0f;
}
typedef struct { int v[6]; } Colors_303748;
Colors_303748 D_L06_00201960 = {{0x2f005f7f, 0x2f004f7f, 0x2f003f7f, 0x2f00004f, 0x2f000000, 0x3f000000}};
Colors_303748 D_L06_00201948 = {{0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}};
extern float D_L06_00162114 __attribute__((sda));
extern float D_L06_00162118 __attribute__((sda));
extern float D_L06_0016211C __attribute__((sda));
extern float D_L06_00162120 __attribute__((sda));
extern float D_L06_00162124 __attribute__((sda));
extern float D_L06_00162128 __attribute__((sda));
extern float D_L06_0016212C __attribute__((sda));
extern float D_L06_00162130 __attribute__((sda));
extern int D_L06_00162134 __attribute__((sda));
extern int D_L06_00162138 __attribute__((sda));
extern int D_L06_0016213C __attribute__((sda));
extern int D_L06_00162140 __attribute__((sda));
extern int D_L06_00162144 __attribute__((sda));
extern int D_L06_00162148 __attribute__((sda));
extern int D_L06_0016214C __attribute__((sda));
extern int D_L06_00162150 __attribute__((sda));
extern int D_L06_00162154 __attribute__((sda));
extern int D_L06_00162158 __attribute__((sda));
extern float frame_time_03b68 __asm__("D_0015ED6C");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern void random_vector_03b68(void *, float, float) __asm__("FUN_L00_00257d78");
extern float random_float_between_03b68(float, float) __asm__("FUN_002132a8");
extern float scale_time_03b68(float) __asm__("FUN_001f96b0");
extern int float_to_int_03b68(float) __asm__("FUN_001fa6d0");
extern int fast_tween_color_03b68(int, int, float) __asm__("FUN_001fa6e0");
extern u8 *emit_particle_03b68(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");
extern void prepare_piece_effect_303748(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);

void FUN_L06_00303748(float scale, char *moby, char *pos, char *push, int count) {
    float position[4], direction[4], velocity[4], acceleration[4];
    Colors_303748 first_colors, second_colors;
    int i;
    float strength;
    prepare_piece_effect_303748(moby, pos, -1, 2.0f, 13.0f);
    first_colors = D_L06_00201948;
    second_colors = D_L06_00201960;
    {
        int piece;
        for (piece = 0; piece < count; piece++) {
            int *first_color = first_colors.v + random_integer_below(6);
            int *second_color;
            int first_time, second_time;
            strength = scale * 400000.0f;
            second_color = second_colors.v + random_integer_below(6);
            {
                int low, high;
                low = scale_game_frames(15);
                high = scale_game_frames(20);
                first_time = random_int_between(low, high);
            }
            {
                int low, high;
                low = scale_game_frames(25);
                high = scale_game_frames(30);
                second_time = random_int_between(low, high);
            }
            FUN_L00_0026a9f0(pos, push, *first_color, *second_color, strength, first_time, second_time, 0, 0,
                             frame_time_03b68 * 10.0f * scale);
        }
    }
    for (i = 0; i < count * D_L06_00162150;) {
        int color1, color2, life, size, fade;
        i++;
        random_vector_03b68(position, 0.0f, 1.0f);
        FUN_001f9a10(position, position, pos);
        random_vector_03b68(direction, 0.0f, 0.9f);
        FUN_001f9a10(direction, direction, push);
        transform_2fd1a0(velocity, direction,
            random_float_between_03b68(D_L06_00162114, D_L06_00162118) * frame_time_03b68);
        random_vector_03b68(direction, 0.0f, 0.9f);
        FUN_001f9a10(direction, direction, push);
        transform_2fd1a0(acceleration, direction,
            random_float_between_03b68(D_L06_00162124, D_L06_00162128) * frame_time_03b68);
        velocity[3] = random_float_between_03b68(D_L06_0016211C, D_L06_00162120) * scale;
        acceleration[3] = random_float_between_03b68(D_L06_0016212C, D_L06_00162130) * scale;
        color1 = fast_tween_color_03b68(D_L06_00162134, D_L06_00162138, random_float_between_03b68(0.0f, 1.0f));
        color2 = fast_tween_color_03b68(D_L06_0016213C, D_L06_00162140, random_float_between_03b68(0.0f, 1.0f));
        life = float_to_int_03b68(scale_time_03b68(random_float_between_03b68((float)D_L06_00162144, (float)(D_L06_00162144 << 1))));
        size = float_to_int_03b68(scale_time_03b68(random_float_between_03b68((float)D_L06_00162148, (float)D_L06_00162148 * 1.2f)));
        fade = float_to_int_03b68(scale_time_03b68(random_float_between_03b68((float)D_L06_0016214C * 0.8f, (float)D_L06_0016214C)));
        emit_particle_03b68(position, velocity, acceleration, color1, color2, life, size, fade,
            D_L06_00162154 + (D_L06_00162158 << 16));
    }
}
extern char *D_L06_0015FFD8_03b68 __asm__("D_L06_0015FFD8");
extern short *D_L06_001ABFC0_03b68[] __asm__("D_L06_001ABFC0");
extern float D_L06_0016215C __attribute__((sda));
extern int D_L06_00162160 __attribute__((sda));
extern int D_L06_00162164 __attribute__((sda));
extern int D_L06_00162168 __attribute__((sda));
extern float D_L06_0016216C __attribute__((sda));
extern float D_L06_00162170 __attribute__((sda));
extern float D_L06_00162174 __attribute__((sda));
extern float D_L06_00162178 __attribute__((sda));
extern float D_L06_0016217C __attribute__((sda));
extern int D_L06_00162180 __attribute__((sda));
extern int D_L06_00162184 __attribute__((sda));
extern int D_L06_00162188 __attribute__((sda));
extern int D_L06_0016218C __attribute__((sda));
extern int D_L06_00162190 __attribute__((sda));
extern int D_L06_00162194 __attribute__((sda));
extern int scale_game_frames_03b68(int) __asm__("FUN_001f96f8");
extern void FUN_L06_0030bef8_03b68(void *, int, int, int) __asm__("FUN_L06_0030bef8");
extern void clear_vector_03b68(void *) __asm__("FUN_001f99f8");
extern void random_vector_03b68(void *, float, float) __asm__("FUN_L00_00257d78");
extern void push_piece_03b68(float, struct Moby *, void *, void *, int) __asm__("FUN_L06_00303748");
extern void add_vector_03b68(void *, void *, void *) __asm__("FUN_001f9a10");
extern void subtract_vector_03b68(void *, void *, void *) __asm__("FUN_001f9a28");
extern void scale_vector_03b68(void *, void *, float) __asm__("FUN_001f9a68");
extern void set_vector_length_03b68(void *, void *, float) __asm__("FUN_001f9bf8");
extern float int_to_float_03b68(int) __asm__("FUN_001fa6c0");
extern float random_float_between_03b68(float, float) __asm__("FUN_002132a8");
extern int fast_tween_color_03b68(int, int, float) __asm__("FUN_001fa6e0");
extern float scale_time_03b68(float) __asm__("FUN_001f96b0");
extern int float_to_int_03b68(float) __asm__("FUN_001fa6d0");
extern u8 *emit_particle_03b68(void *, void *, void *, int, int, int, int, int, int) __asm__("FUN_00218888");

/* Breaks the linked pieces apart: pushes each one out from their middle (smaller pieces faster), spins it and sheds dust. */
void FUN_L06_00303b68(struct Moby *moby) {
    char *vars = (char *)moby->pvars;
    float center[4];
    float middle[4];
    float push[4];
    float spin[4];
    float pos[4];
    float vel[4];
    float accel[4];
    struct Moby *piece;
    short *list;
    short count;
    int id;
    int i;
    float largest;
    u8 *particle;
    *(u128 *)center = 0;
    center[0] = 426.0f;
    center[1] = 189.75f;
    center[2] = 173.07f;
    FUN_L06_0030bef8_03b68(center, scale_game_frames_03b68(D_L06_00162108), D_L06_0016210C,
                           D_L06_00162110);
    largest = 0.0f;
    clear_vector_03b68(middle);
    list = D_L06_001ABFC0_03b68[*(int *)(vars + 0xCC)];
    count = 0;
    do {
        id = *list & 0x7FFF;
        random_vector_03b68(push, 1.0f, 2.0f);
        push_piece_03b68(30.0f, moby, &((struct Moby *)(D_L06_0015FFD8_03b68 + (id << 8)))->pos, push, 1);
        add_vector_03b68(middle, middle, &((struct Moby *)(D_L06_0015FFD8_03b68 + (id << 8)))->pos);
        count++;
        if (largest < ((struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68))->bsphere.w) {
            largest = ((struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68))->bsphere.w;
        }
    } while (*list++ >= 0);
    scale_vector_03b68(middle, middle, 1.0f / int_to_float_03b68(count));
    list = D_L06_001ABFC0_03b68[*(int *)(vars + 0xCC)];
    do {
        float speed;
        id = *list & 0x7FFF;
        subtract_vector_03b68(push, &((struct Moby *)(D_L06_0015FFD8_03b68 + (id << 8)))->pos, middle);
        piece = (struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68);
        speed = frame_time * 30.0f;
        set_vector_length_03b68(push, push, -((piece->bsphere.w - largest) / largest) * speed + speed);
        push[3] = 6.0f;
        random_vector_03b68(spin, frame_time * 0.2617994f, frame_time * 0.08726646f);
        FUN_L06_00300f70((char *)(D_L06_0015FFD8_03b68 + (id << 8)), push, spin);
        for (i = 0; i < D_L06_00162190; i++) {
            piece = (struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68);
            random_vector_03b68(pos, 0.0f, piece->bsphere.w * 0.00048828125f);
            add_vector_03b68(pos, pos, &((struct Moby *)(D_L06_0015FFD8_03b68 + (id << 8)))->pos);
            random_vector_03b68(vel, 0.0f, D_L06_0016215C * frame_time);
            clear_vector_03b68(accel);
            vel[3] = random_float_between_03b68(D_L06_00162170, D_L06_00162174);
            accel[3] = random_float_between_03b68(D_L06_00162178, D_L06_0016217C);
            particle = emit_particle_03b68(
                pos, vel, accel,
                fast_tween_color_03b68(D_L06_00162180, D_L06_00162184, random_float_between_03b68(0.0f, 1.0f)),
                fast_tween_color_03b68(D_L06_00162188, D_L06_0016218C, random_float_between_03b68(0.0f, 1.0f)),
                float_to_int_03b68(scale_time_03b68((float)D_L06_00162160 * random_float_between_03b68(0.0f, 1.0f) + 1.0f)),
                float_to_int_03b68(scale_time_03b68(
                    (float)D_L06_00162164 * (random_float_between_03b68(-D_L06_0016216C, D_L06_0016216C) + 1.0f))),
                float_to_int_03b68(scale_time_03b68(
                    (float)D_L06_00162168 * (random_float_between_03b68(-D_L06_0016216C, D_L06_0016216C) + 1.0f))),
                D_L06_00162194);
            if (particle != 0) {
                particle[9] = float_to_int_03b68(4.0f) - 0x20;
            }
        }
    } while (*list++ >= 0);
    ((struct Moby *)((id << 8) + (int)D_L06_0015FFD8_03b68))->command = 30;
}
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305468), where it is exact; names translated to the US level program. */

extern char *D_L06_0015FFD8_u __asm__("D_L06_0015FFD8");
extern short *D_L06_001ABFC0_c[] __asm__("D_L06_001ABFC0");
void mark_moby_for_removal_c(struct Obj *obj) __asm__("FUN_0020c828");

/* Marks every moby in the moby's list for removal; the list ends after its first negative entry. */
void FUN_L06_00304028(char *moby) {
    char *data = *(char **)(moby + 0x78);
    short *p = D_L06_001ABFC0_c[*(int *)(data + 0xCC)];
    do {
        mark_moby_for_removal_c(D_L06_0015FFD8_u + ((*p & 0x7FFF) << 8));
    } while (*p++ >= 0);
}
#include "rnc/gameplay/entities/moby.h"
typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vector_304098;

typedef struct Data_304098 {
    int cut;
    int cut2;
    int list[9];
    float t;
    float vel;
    int moby3;
    int moby1;
    int moby2;
    int timer;
    int timer2;
    int f48;
} Data_304098;

typedef struct {
    char pad0[0x30];
    Vector_304098 a;
    char pad40[0x30];
    Vector_304098 b;
} Cut_304098;

extern Cut_304098 *D_L06_001600EC_304098 __asm__("D_L06_001600EC");
extern struct Moby *D_L06_0015FFD8_304098 __asm__("D_L06_0015FFD8");
extern char D_0013F3D0[] __attribute__((section(".data")));
extern float D_L06_0015F3FC __asm__("D_L06_0015F3FC");
typedef struct { char pad0[0x160]; float f160; int pad164; int f168; } Fx_304098;
extern Fx_304098 D_L06_00167400_304098 __asm__("D_L06_00167400") __attribute__((section(".data")));

extern float fast_subtract_rotations_304098(float, float) __asm__("FUN_001fa5c8");
extern float fast_add_rotations_304098(float, float) __asm__("FUN_001fa580");
extern void approach_two_stage_304098(float *, float *, float, float, float, float) __asm__("FUN_00213f38");
extern void allocate_voice_for_target_entry_304098(int, int, void *) __asm__("FUN_0022da68");
extern void event_304098(int, int, void *) __asm__("FUN_0022dba0");
extern float vector_distance_304098(void *, void *) __asm__("FUN_001f9b80");
extern int try_set_help_message_304098(int, int) __asm__("FUN_00215130");
extern int scale_game_frames_304098(int) __asm__("FUN_001f96f8");
extern int dismiss_help_message_304098(int) __asm__("FUN_L00_002644e0");
extern int tick_countdown_304098(int *) __asm__("FUN_001f9740");
extern void fade_to_black_304098(int) __asm__("FUN_001f4a58");
extern void start_camera_move_304098(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void set_camera_position_304098(void *) __asm__("FUN_L00_002ea9d8");
extern void set_camera_rotation_304098(void *) __asm__("FUN_L00_002eaa30");
extern void set_camera_effect_304098(float, float, float, float, float, float) __asm__("FUN_L02_002f8aa0");
extern void copy_camera_304098(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void finish_camera_move_304098(int) __asm__("FUN_L00_002eac18");
extern void clear_vector_304098(void *) __asm__("FUN_001f99f8");
extern float float_from_int_304098(int) __asm__("FUN_001fa6c0");
extern void update_moby_304098(void *) __asm__("FUN_L00_00250df8");
extern void FUN_L06_00304818(int, void *);
extern void FUN_L06_00303150(unsigned char *);

extern char D_0013A5E0[] __attribute__((section(".data")));
extern char D_0013D3AA __attribute__((section(".data")));
extern float D_L06_00162198 __attribute__((sda));
extern float D_L06_0016219C __attribute__((sda));
extern float D_L06_001621A0 __attribute__((sda));
extern float D_L06_001621A4 __attribute__((sda));
extern float D_L06_001621A8 __attribute__((sda));
extern float D_L06_001621B0 __attribute__((sda));
extern char D_0013E633[], D_0014171B[], D_0013D24D[], D_L16_00167240[] __attribute__((section(".data")));
void FUN_L06_00304098(struct Moby *moby) {
    Data_304098 *data = *(struct Data_304098 **)&moby->pvars;
    Vector_304098 camera_position;
    Vector_304098 camera_rotation;
    Vector_304098 next_position;
    float previous_value;
    float current_angle;
    int i;
    int help_message_available;
    int *entry;
    int duration;
    int timer;

    switch (moby->state) {
    case 0:
        moby->update_dist = 0xFF;
        moby->rot.x = fast_subtract_rotations_304098(moby->rot.x, 0.78539819f);
        moby->state = 1;
        data->f48 = -1;
        break;
    case 1:
        break;
    case 2: {
        float angle;
        approach_two_stage_304098(&data->t, &data->vel, 1.0f, frame_time_sq, frame_time_sq, frame_time);
        angle = data->vel * 0.78539819f;
        moby->rot.x = fast_add_rotations_304098(moby->rot.x, angle);
        D_L06_0015FFD8_304098[data->moby1].rot.x = fast_subtract_rotations_304098(D_L06_0015FFD8_304098[data->moby1].rot.x, angle);
        D_L06_0015FFD8_304098[data->moby2].rot.x = fast_subtract_rotations_304098(D_L06_0015FFD8_304098[data->moby2].rot.x, angle);
        update_moby_304098(&D_L06_0015FFD8_304098[data->moby1]);
        update_moby_304098(&D_L06_0015FFD8_304098[data->moby2]);
        if (1.0f <= data->t) {
            moby->state = 3;
            allocate_voice_for_target_entry_304098(1, 0, moby);
            D_0013D3AA = 1;
        }
        break;
    }
    case 3:
        if (vector_distance_304098(D_0013F3D0, &moby->pos) < 3.7f) {
            help_message_available = try_set_help_message_304098(4, 0x1783) != 0;
            if ((*(int *)(D_0013A5E0 + 0x2504) & 0x10) && help_message_available) {
                allocate_voice_for_target_entry_304098(2, 0, moby);
                dismiss_help_message_304098(4);
                moby->state = 4;
                data->timer = scale_game_frames_304098(0x1E);
            }
        }
        break;
    case 4:
        if (tick_countdown_304098(&data->timer)) {
            qcopy_nc(&camera_position, &D_L06_001600EC_304098[data->cut].a);
            qcopy(&camera_rotation, &D_L06_001600EC_304098[data->cut].b);
            moby->state = 5;
            data->t = 0.0f;
            data->vel = 0.0f;
            fade_to_black_304098(scale_game_frames_304098(0xF));
            start_camera_move_304098(&camera_position, &camera_rotation, 2, scale_game_frames_304098(0x12C), 0);
            set_camera_effect_304098(D_L06_00162198, D_L06_0016219C, D_L06_001621A0,
                                     D_L06_001621A4, D_L06_001621A8, D_L06_001621B0);
            set_camera_position_304098(&camera_position);
            set_camera_rotation_304098(&camera_rotation);
            D_L06_0015F404 = 1;
        }
        if (data->timer < scale_game_frames_304098(10)) {
            previous_value = float_from_int_304098(data->timer);
            previous_value = previous_value / (float)scale_game_frames_304098(10);
            D_L06_0015F3FC = 1.0f - previous_value;
        }
        break;
    case 5:
        duration = scale_game_frames_304098(10);
        timer = data->timer;
        if (timer < duration) {
            data->timer = timer + 1;
            previous_value = float_from_int_304098(timer + 1);
            previous_value = previous_value / (float)scale_game_frames_304098(10);
            D_L06_0015F3FC = 1.0f - previous_value;
        }
        previous_value = data->t;
        approach_two_stage_304098(&data->t, &data->vel, 2.0f, frame_time_sq, frame_time_sq, frame_time * 0.5f);
        qcopy(&camera_position, &D_L06_001600EC_304098[data->cut].a);
        camera_position.z = camera_position.z + data->t * 18.0f;
        set_camera_position_304098(&camera_position);
        set_camera_rotation_304098(&D_L06_001600EC_304098[data->cut].b);
        previous_value = previous_value * 100.0f;
        current_angle = data->t * 100.0f * 0.0174532925f;
        previous_value = previous_value * 0.0174532925f;
        entry = data->list;
        for (i = 8; i >= 0; i--, entry++) {
            if (previous_value < D_L06_001600EC_304098[*entry].b.z && D_L06_001600EC_304098[*entry].b.z < current_angle) {
                FUN_L06_00304818((int)moby, &D_L06_001600EC_304098[*entry].a);
                event_304098(0, 0, moby);
            }
        }
        if (1.075f <= data->t) {
            copy_camera_304098(&D_L06_001600EC_304098[data->cut2].a, &D_L06_001600EC_304098[data->cut2].b, 0x72, 0);
            fade_to_black_304098(scale_game_frames_304098(10));
            moby->state = 6;
            finish_camera_move_304098(3);
            qcopy(&next_position, &D_L06_001600EC_304098[data->cut2].a);
            next_position.z = next_position.z + 1.0f;
            next_position.y = next_position.y - 2.5f;
            clear_vector_304098(&camera_rotation);
            camera_rotation.y = -0.2617993878f;
            camera_rotation.z = *(float *)(D_0013E633 + 0xDB5);
            start_camera_move_304098(&next_position, &camera_rotation, 2, scale_game_frames_304098(0x12C), 0);
            D_L06_00167400_304098.f160 = 0.3f;
            D_L06_00167400_304098.f168 = scale_game_frames_304098(0x2D);
            event_304098(0, 0, moby);
        }
        break;
    case 6:
        approach_two_stage_304098(&data->t, &data->vel, 2.0f, frame_time_sq, frame_time_sq, frame_time + frame_time);
        qcopy(&camera_rotation, &D_L06_001600EC_304098[data->cut2].a);
        camera_rotation.z = camera_rotation.z + 1.0f;
        camera_rotation.y = camera_rotation.y - 2.5f;
        clear_vector_304098(&camera_position);
        camera_position.y = -0.2617993878f;
        camera_position.z = *(float *)(D_0013E633 + 0xDB5);
        set_camera_position_304098(&camera_rotation);
        set_camera_rotation_304098(&camera_position);
        if (2.0f <= data->t) {
            moby->state = 7;
            data->timer2 = scale_game_frames_304098(0xE10);
            copy_camera_304098(&D_L06_001600EC_304098[data->cut2].a, &D_L06_001600EC_304098[data->cut2].b, 0, 0);
            finish_camera_move_304098(3);
            FUN_L06_00303150((unsigned char *)&D_L06_0015FFD8_304098[data->moby3]);
        }
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305BF8), where it is exact; names translated to the US level program. */

extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

/* Moves the moby from state 1 to 2 and gives it a voice. */
void FUN_L06_003047b8(char *arg) {
    if ((unsigned char)arg[0x20] == 1) {
        arg[0x20] = 2;
        allocate_voice_for_target_entry(0, 0, arg);
    }
}
s32 FUN_L06_003047f8(const u8 *moby) {
    u32 state = moby[0x20];

    if (state == 7) {
        return 2;
    }
    if (state > 3) {
        return 1;
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305C58), where it is exact; names translated to the US level program. */

typedef struct { int v[6]; } V6;

V6 D_L06_002019A0 = {{0x4f008fff, 0x4f008fff, 0x4f007fff, 0x4f006fff, 0x2fffffff, 0x2fffffff}};
extern V6 D_L06_002019B8;
extern f32 random_float_between(f32, f32) __asm__("func_002132A8");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern void FUN_L00_0026a9f0(void *, void *, int, int, float, int, int, int, int, float);
extern void clear_u64_value(void *) __asm__("func_001F99F8");

extern short D_L06_001621B4 __attribute__((sda));
extern short D_L06_001621B8 __attribute__((sda));
void FUN_L06_00304818(int unused, void *obj) {
    float pos[4];
    V6 a;
    V6 b;
    int i = 0;
    clear_u64_value(pos);
    if (*(int *)&D_L06_001621B8 > 0) {
        do {
            float s;
            float f;
            int *pa;
            int *pb;
            int r1;
            int r3;
            i++;
            s = random_float_between(8.0f, 10.0f);
            a = D_L06_002019A0;
            b = D_L06_002019B8;
            s = s * frame_time;
            pa = a.v + random_integer_below(6);
            pb = b.v + random_integer_below(6);
            f = *(float *)&D_L06_001621B4 * 400000.0f;
            r1 = random_int_between(scale_game_frames(0xF), scale_game_frames(0x14));
            FUN_L00_0026a9f0(obj, pos, *pa, *pb, f, r1, random_int_between(scale_game_frames(0x19), scale_game_frames(0x1E)), 0, 0, s * *(float *)&D_L06_001621B4);
        } while (i < *(int *)&D_L06_001621B8);
    }
}
/* Level 06 update for moby class 1123: waits for its partner, animates four ribbons, then bursts into particles. */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305E38), where it is exact; names translated to the US level program. */

typedef struct {
    float a[4][30][4];
    float b[4][30][4];
    int timer[4];
    int f10;
    int f14;
    int f18;
} Data_305E38;

extern char D_0013E550[];
extern float D_L06_00167540[];
extern float D_0015ED6C;
extern float D_L06_00162200 __attribute__((sda));
extern float D_L06_001FE890[];
extern float FUN_L00_00257c48(float lo, float hi);
extern float fast_vec_dist(void *, void *);
extern int D_L06_00162234;
extern int D_L06_00162238;
extern int FUN_L00_0028d8c0(void *, int);
extern s32 fast_tween_color(s32, s32, f32) __asm__("func_001FA6E0");
extern s32 truncate_float_to_s32_c(f32) __asm__("func_001FA6D0");
extern float D_L06_001621F8 __attribute__((sda));
extern short D_L06_001621FC __attribute__((sda));
extern short D_L06_00162204 __attribute__((sda));
extern float D_L06_00162208 __attribute__((sda));
extern short D_L06_00162210 __attribute__((sda));
extern short D_L06_00162214 __attribute__((sda));
extern short D_L06_00162218 __attribute__((sda));
extern short D_L06_0016221C __attribute__((sda));
extern short D_L06_00162220 __attribute__((sda));
extern short D_L06_00162224 __attribute__((sda));
extern short D_L06_00162228 __attribute__((sda));
extern short D_L06_0016222C __attribute__((sda));
extern short D_L06_00162230 __attribute__((sda));
extern void FUN_001f9ff8(float *, float);
extern void FUN_L00_00257d78(void *, float, float);
extern void FUN_L06_00217300(void *);
extern void add_vector_xyz_c(void *, void *, void *) __asm__("FUN_001f9a10");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void enqueue_callback_list_1(int, char *);
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void fast_vec_normalize_c(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void fast_vec_scale_c(void *, void *, f32) __asm__("func_001F9A68");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
unsigned char *FUN_L00_002730e0(float *pos, void *vel, int color, u8 life, u8 b, int mode, float scale);
void FUN_L06_00305000_c(char *m) __asm__("FUN_L06_00305000");
void FUN_L06_00305170_c(char *m) __asm__("FUN_L06_00305170");
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");
extern void func_L00_00273F80_r(void *, void *, int, float, int, int, int) __asm__("FUN_L00_002730e0");

void FUN_L06_003049f8(char *moby)
{
    Data_305E38 *d = *(Data_305E38 **)(moby + 0x78);
    union {
        float tmp[30][4];
        struct {
            float a[16];
            float b[16];
            float pos[4];
            float vel[4];
        } m;
    } u;

    switch ((unsigned char)moby[0x20]) {
    case 0:
        *(float *)(moby + 0x2C) = *(float *)(*(char **)(moby + 0x24) + 0x24) * *(float *)&D_L06_00162210;
        D_L06_00162238 = 0x16;
        D_L06_00162234 = 0x2D;
        d->timer[1] = 0xB;
        d->timer[2] = 0x17;
        d->timer[3] = 0x22;
        d->timer[0] = 0;
        d->f18 = -1;
        if (d->f10 != -1) {
            moby[0x20] = 1;
            *(int *)(moby + 0x94) = 0;
        } else {
            moby[0x20] = 2;
        }
        break;
    case 1: {
        char *e = D_L06_0015FFD8_301070 + (d->f10 << 8);
        if (*(short *)(e + 0xA6) == 0x463 && (unsigned char)e[0x20] < 3) {
            return;
        }
        moby[0x20] = 2;
        *(int *)(moby + 0x94) = *(int *)(*(char **)(moby + 0x24) + 0x10);
        break;
    }
    case 2: {
        float f20;
        float f21;
        float v;
        int i;
        int j;
        int k;
        int l;

        if (FUN_L00_0028d8c0(moby, d->f18) == 0) {
            d->f18 = allocate_voice_for_target_entry(0, 4, moby);
        }
        FUN_L06_00305000_c(moby);
        v = D_L06_001621F8 + *(float *)&D_L06_001621FC * D_0015ED6C;
        D_L06_001621F8 = v;
        if (1.0f < v) {
            D_L06_001621F8 = v - 1.0f;
        }
        f21 = *(float *)&D_L06_00162204 / 30.0f;
        f20 = -(*(float *)&D_L06_00162204 * 0.5f);
        for (i = 0; i < 4; i++) {
            if (d->timer[i] == 0) {
                for (j = 0; j < 30; j++) {
                    FUN_L06_00217300(d->a[i][j]);
                    d->a[i][j][0] = f20;
                    FUN_L00_00257d78(u.tmp[j], 0.0f, D_L06_00162200 * D_0015ED6C);
                    f20 += f21;
                }
                for (k = 1; k < 29; k++) {
                    add_vector_xyz_c(d->b[i][k], u.tmp[k - 1], u.tmp[k]);
                    add_vector_xyz_c(d->b[i][k], d->b[i][k], u.tmp[k + 1]);
                    fast_vec_scale_c(d->b[i][k], d->b[i][k], 0.333f);
                }
                d->timer[i] = D_L06_00162234;
            } else {
                for (l = 1; l < 29; l++) {
                    add_vector_xyz_c(d->a[i][l], d->a[i][l], d->b[i][l]);
                }
                d->timer[i]--;
            }
        }
        if (fast_vec_dist(moby + 0x10, ((char *)D_L06_00167540)) < 48.0f) {
            enqueue_callback_list_1((int)FUN_L06_00305170_c, moby);
        }
        if (d->f14 >= 0 && *(unsigned char *)(D_L06_0015FFD8_301070 + 0xBC + (d->f14 << 8)) != 0) {
            int h;
            *(int *)(moby + 0x94) = 0;
            moby[0x20] = 3;
            h = d->f18;
            if (h != -1) {
                char *e = D_0013E550 + h * 0x70;
                if (*(char **)(e + 0x88) == moby && *(unsigned char *)(e + 0x74) != 0) {
                    release_voice_slot(h);
                }
            }
            d->f18 = -1;
            allocate_voice_for_target_entry(1, 0, moby);
        }
        break;
    }
    case 3: {
        int i;
        int j;

        euler_rotation_matrix(u.m.a, (float *)(moby + 0x40));
        FUN_001f9ff8(u.m.b, D_L06_00162208);
        multiply_matrix(u.m.a, u.m.a, u.m.b);
        for (i = 0; i < 4; i++) {
            euler_rotation_matrix(u.m.b, &D_L06_001FE890[i * 4]);
            multiply_matrix(u.m.b, u.m.a, u.m.b);
            add_vector_xyz_c(u.m.b + 12, u.m.b + 12, moby + 0x10);
            u.m.b[14] += (float)i * 1.7f + 0.6f;
            u.m.b[15] = 1.0f;
            for (j = 0; j < 50; j++) {
                int color;
                float scale;
                int s;
                unsigned char life;
                fast_vec_normalize_c(u.m.pos, u.m.b, FUN_L00_00257c48(0.0f, *(float *)&D_L06_00162204 * 0.75f));
                add_vector_xyz_c(u.m.pos, u.m.pos, u.m.b + 12);
                color = fast_tween_color(*(int *)&D_L06_00162214, *(int *)&D_L06_00162218, random_float_between(0.0f, 1.0f));
                scale = random_float_between(*(float *)&D_L06_0016221C, *(float *)&D_L06_00162220);
                s = truncate_float_to_s32_c(random_float_between((float)*(int *)&D_L06_00162224, (float)*(int *)&D_L06_00162228));
                FUN_L00_00257d78(u.m.vel, *(float *)&D_L06_0016222C * D_0015ED6C, *(float *)&D_L06_00162230 * D_0015ED6C);
                life = scale_game_frames(s);
                func_L00_00273F80_r(u.m.pos, u.m.vel, color, scale, life, (unsigned char)random_integer_below(0xFF), 0);
            }
        }
        moby[0x20] = 4;
        break;
    }
    }
}
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00306440), where it is exact; names translated to the US level program. */

extern int coll_line_fix(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_L00_00259858(char *a, int b, int c, float d);

extern short D_L06_001621E8 __attribute__((sda));
extern short D_L06_001621EC __attribute__((sda));
extern short D_L06_001621F0 __attribute__((sda));
void FUN_L06_00305000(char *m) {
    float P[4];
    float Q[4];
    float A[12];
    FUN_L00_00259858((char *)A, (int)m, 0x10000, 6.0f);
    P[0] = *(float *)&D_L06_001621E8;
    P[1] = *(float *)&D_L06_001621EC;
    P[2] = *(float *)&D_L06_001621F0;
    Q[0] = -*(float *)&D_L06_001621E8;
    Q[1] = *(float *)&D_L06_001621EC;
    Q[2] = *(float *)&D_L06_001621F0;
    transform_vector_by_basis(P, P, m + 0xC0);
    FUN_001f9a10(P, P, m + 0x10);
    transform_vector_by_basis(Q, Q, m + 0xC0);
    FUN_001f9a10(Q, Q, m + 0x10);
    coll_line_fix(P, Q, 0, (int)m, (int)A);
    P[0] = *(float *)&D_L06_001621E8;
    P[1] = -*(float *)&D_L06_001621EC;
    P[2] = *(float *)&D_L06_001621F0;
    Q[0] = -*(float *)&D_L06_001621E8;
    Q[1] = -*(float *)&D_L06_001621EC;
    Q[2] = *(float *)&D_L06_001621F0;
    transform_vector_by_basis(P, P, m + 0xC0);
    FUN_001f9a10(P, P, m + 0x10);
    transform_vector_by_basis(Q, Q, m + 0xC0);
    FUN_001f9a10(Q, Q, m + 0x10);
    coll_line_fix(P, Q, 0, (int)m, (int)A);
}

#define NOT_SDA

#define MACRO_ADDR

/* draws four copies of a model transformed by a table, then spawns a part for each */
/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_003065B0), where it is exact; names translated to the US level program. */

extern float D_L06_001FE890[];
extern int D_L06_00162234;
extern int D_L06_00162238;
extern int get_effect_texture(int) __asm__("FUN_001f44b8");
extern short D_L06_001621C4 __attribute__((sda));
extern short D_L06_001621C8 __attribute__((sda));
extern short D_L06_001621C0 __attribute__((sda));
extern short D_L06_001621CC __attribute__((sda));
extern short D_L06_001621E0 __attribute__((sda));
extern short D_L06_001621E4 __attribute__((sda));
extern short D_L06_001621F4 __attribute__((sda));
extern f32 D_L06_001621F8_gp __asm__("D_L06_001621F8") __attribute__((sda));
extern f32 D_L06_00162208_gp __asm__("D_L06_00162208") __attribute__((sda));
extern void set_scale_matrix(float *, float) __asm__("FUN_001f9ff8");
extern void euler_rotation_matrix(float *, float *) __asm__("FUN_001fa050");
extern void multiply_matrix(void *, void *, void *) __asm__("FUN_001fa378");
extern void FUN_L06_00216b88(int *, float *);
extern void add_vector_xyz(void *, void *, void *);
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void vu1_add_g_sregister(int, long) __asm__("FUN_00233980");

void FUN_L06_00305170(char *m) {
    float a[16];
    float b[16];
    float mat[5][16];
    int s[10];
    char *data;
    float *m0;
    int *sp;
    float *pp;
    float *tp;
    float *mp;
    int i;
    int j;
    data = *(char **)(m + 0x78);
    euler_rotation_matrix(a, (float *)(m + 0x40));
    set_scale_matrix(b, *(float *)&D_L06_00162208_gp);
    multiply_matrix(a, a, b);
    vu1_add_g_sregister(6, get_effect_texture(0xE));
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x42, (long)*(int *)&D_L06_001621C0 | ((long)*(int *)&D_L06_001621C4 << 2) |
                                  ((long)*(int *)&D_L06_001621C8 << 4) |
                                  ((long)*(int *)&D_L06_001621CC << 6) | 0x8000000000L);
    font_queue_vu_state();
    m0 = mat[0];
    sp = s;
    pp = mat[0] + 12;
    tp = D_L06_001FE890;
    mp = m0;
    for (i = 0; i < 4; i++) {
        euler_rotation_matrix(mp, tp);
        tp += 4;
        multiply_matrix(mp, a, mp);
        mp += 16;
        add_vector_xyz(pp, pp, m + 0x10);
        pp[2] += (float)i * 1.7f + 0.6f;
        pp[3] = 1.0f;
        pp += 16;
    }
    s[0] = D_L06_00162234;
    s[2] = (int)(data + 0xF00);
    s[1] = D_L06_00162238;
    s[3] = *(int *)&D_L06_001621E0;
    s[4] = *(int *)&D_L06_001621E4;
    *(float *)&s[7] = *(float *)&D_L06_001621F4;
    *(float *)&s[8] = *(float *)&D_L06_001621F8_gp;
    s[5] = 0x1E;
    s[6] = (int)data;
    pp = m0;
    for (j = 3; j >= 0; j--) {
        FUN_L06_00216b88(sp, pp);
        pp += 16;
    }
}
