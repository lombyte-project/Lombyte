/* Level 12 moby classes: hoven_turret, mine, planetbuster_maximus_part, homing_mine_spawner. */
#include "rnc/gameplay/hero.h"
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/globals.h"
#include "rnc/gameplay/entities/moby.h"
#include "asm.h"
#include "rnc/overlay/entities.h"

#include "rnc/math/vector.h"

#include "qcopy.h"

/* Hoven vendor draw: builds the two strip matrices, then draws the four rings as quads lit by their phase. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00303CA8), where it is exact; names translated to the US level program. */

typedef struct Moby Moby;

typedef struct Quad_303CA8 {
    float v[16];
    int c[4];
    float uv[8];
    long g[4];
} Quad_303CA8;

typedef struct Data_303CA8 {
    float ring[8][70][4];
    int phase[4]; /* 0x2300 */
    int row; /* 0x2310 */
    int target; /* 0x2314 */
    float angle; /* 0x2318 */
    int f231C;
    int f2320;
    float dist; /* 0x2324 */
    float turn; /* 0x2328 */
    int count; /* 0x232C */
    int f2330;
} Data_303CA8;

extern Moby *D_L12_0015FFD8;
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern float D_L12_001FB530[][4];
extern float D_L12_001FB5B0[][4];
extern int D_L12_00161DCC __attribute__((sda));
extern int D_L12_00161DD0 __attribute__((sda));
extern int D_L12_00161DD4 __attribute__((sda));
extern int D_L12_00161DD8 __attribute__((sda));
extern int D_L12_00161DEC __attribute__((sda));
extern int D_L12_00161DF0 __attribute__((sda));
extern float D_L12_00161DF4 __attribute__((sda));
extern float D_L12_00161DF8 __attribute__((sda));
extern int D_L12_00161E04;
extern int D_L12_00161E08;
extern int euler_rotation_matrix() __asm__("FUN_001fa050");
extern int multiply_matrix() __asm__("FUN_001fa378");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");
extern void func_001FA218_303CA8(void *, void *) __asm__("FUN_001fa050");
extern void func_001FA540_303CA8(void *, void *, void *) __asm__("FUN_001fa378");
extern void func_001F9BD8_303CA8(void *, void *, void *) __asm__("FUN_001f9a10");
extern int func_001F4868_303CA8(int) __asm__("FUN_001f44b8");
extern float func_001FA888_303CA8(int) __asm__("FUN_001fa6c0");
extern int func_001FA8A8_303CA8(int, int, float) __asm__("FUN_001fa6e0");
extern void func_L00_001FD1D8_303CA8(void *, void *, int) __asm__("FUN_001f7d30");
extern float D_L12_001FB7D0_303CA8[] __asm__("D_L12_001FB510");

void FUN_L12_003028c8(Moby *moby) {
    Quad_303CA8 quad[2];
    float rotation_matrix[16];
    float matrices[5][16];
    float anchor[4];
    Data_303CA8 *data;
    float *pos;
    float *translation;
    float fade;
    float scroll;
    int i, segment, uv_index, vertex, pass, ring;

    data = *(Data_303CA8 **)&moby->pvars;
    func_001FA218_303CA8(rotation_matrix, ((float *)&moby->rot));
    pos = ((float *)&moby->pos);
    translation = &matrices[0][12];
    for (i = 0; i < 2; i++) {
        float *matrix = matrices[i];

        func_001FA218_303CA8(matrix, D_L12_001FB5B0[i]);
        func_001FA540_303CA8(matrix, rotation_matrix, matrix);
        if (!(i & 1) && data->target != -1) {
            qcopy(anchor, ((float *)&D_L12_0015FFD8[data->target].pos));
        } else {
            qcopy(anchor, pos);
        }
        func_001F9BD8_303CA8(translation, translation, anchor);
        translation += 16;
        matrix[15] = 1.0f;
    }
    quad[1].g[1] = func_001F4868_303CA8(0xE);
    quad[1].g[3] = (long)D_L12_00161DCC | ((long)D_L12_00161DD0 << 2) | ((long)D_L12_00161DD4 << 4) | ((long)D_L12_00161DD8 << 6) | (0x8000L << 24);
    quad[1].g[2] = 0xFF9000000260UL;
    quad[1].g[0] = 0;
    {
        float *uv = &quad[1].uv[1];
        int corner;

        for (corner = 0; corner < 4; corner++) {
            *uv = D_L12_001FB7D0_303CA8[corner * 2 + 1];
            uv += 2;
        }
    }
    for (ring = 0; ring < 4; ring++) {
        if (D_L12_00161E08 < data->phase[ring]) {
            fade = func_001FA888_303CA8(data->phase[ring] - D_L12_00161E08) / func_001FA888_303CA8(D_L12_00161E04 - D_L12_00161E08);
        } else {
            fade = 1.0f - func_001FA888_303CA8(data->phase[ring]) / func_001FA888_303CA8(D_L12_00161E08);
        }
        quad[1].c[0] = quad[1].c[1] = quad[1].c[2] = quad[1].c[3] = func_001FA8A8_303CA8(D_L12_00161DEC, D_L12_00161DF0, fade);
        scroll = 0.0f;
        for (segment = 0; segment < data->count - 1; segment++) {
            scroll = scroll + D_L12_00161DF4;
            if (1.0f < scroll) {
                scroll = scroll - 1.0f;
            }
            for (uv_index = 0; uv_index < 4; uv_index++) {
                quad[1].uv[uv_index * 2] = D_L12_001FB7D0_303CA8[uv_index * 2] + scroll + D_L12_00161DF8;
            }
            for (vertex = 0; vertex < 4; vertex++) {
                func_001F9BD8_303CA8(&quad[1].v[vertex * 4], D_L12_001FB530[vertex], data->ring[ring][segment + (vertex >> 1)]);
            }
            for (pass = 0; pass < 2; pass++) {
                func_L00_001FD1D8_303CA8(&quad[1], matrices[pass], 0);
            }
        }
    }
}
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern void clear_u64_value(void *) __asm__("FUN_001f99f8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
extern float distance_xyz(void *, void *) __asm__("FUN_001f9b48");
extern void random_vector(float *, float, float) __asm__("FUN_L00_00257d78");
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern float D_L12_00161DFC __attribute__((sda));
extern float D_L12_00161E00 __attribute__((sda));
extern void FUN_L12_003028c8_u(struct Moby *) __asm__("FUN_L12_003028c8");

void FUN_L12_00302c58(struct Moby *moby) {
    char *data = (char *)moby->pvars;
    float cycle = D_L12_00161DF8 + D_L12_00161DFC * frame_time;
    int strip;
    D_L12_00161DF8 = cycle;
    if (cycle > 1.0f) D_L12_00161DF8 = cycle - 1.0f;
    for (strip = 0; strip < 4; strip++) {
        int count;
        int i;
        float *points;
        float *intermediate;
        if (tick_countdown_32_alt((int *)(data + 0x2300 + strip * 4))) {
            int vector_count = *(int *)(data + 0x232c);
            float intermediate_storage[vector_count][4];
            char *timers = data + 0x2300;
            intermediate = &intermediate_storage[0][0];
            i = 0;
            if (0 < *(int *)(data + 0x232c)) {
                points = (float *)(data + strip * 0x460);
                do {
                    float vector_count_float;
                    float length;
                    float index;
                    float rate;
                    clear_u64_value(points);
                    vector_count_float = (float)*(int *)(data + 0x232c);
                    length = *(float *)(data + 0x2324);
                    index = (float)i;
                    rate = D_L12_00161E00 * frame_time;
                    points[3] = 1.0f;
                    points[0] = index * (length / vector_count_float);
                    random_vector(intermediate + i * 4, 0.0f, rate);
                    points += 4;
                    i++;
                } while (i < *(int *)(data + 0x232c));
            }
            count = 1;
            if (count < *(int *)(data + 0x232c) - 1) {
                points = (float *)(data + strip * 0x460 + 0x1190);
                do {
                    fast_vec_add(points, intermediate_storage[count - 1], intermediate_storage[count]);
                    fast_vec_add(points, points, intermediate_storage[count + 1]);
                    scale_vector_xyz(points, points, 0.333f);
                    count++;
                    points += 4;
                } while (count < *(int *)(data + 0x232c) - 1);
            }
            *(int *)(timers + (strip << 2)) = D_L12_00161E04;
        } else {
            for (i = 1; i < *(int *)(data + 0x232c) - 1; i++) {
                float *point = (float *)(data + strip * 0x460 + i * 16);
                float *delta = (float *)(data + strip * 0x460 + 0x1190 + i * 16 - 16);
                fast_vec_add(point, point, delta);
            }
        }
    }
    if (distance_xyz(&moby->pos, D_L12_00167240) < 48.0f)
        enqueue_callback_list_1(FUN_L12_003028c8_u, moby);
}
#include "rnc/math/vector.h"

#include "qcopy.h"

/* Level 12 vendor moby update: steers toward its target, then calls the vendor helpers. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00304310), where it is exact; names translated to the US level program. */


typedef struct Data_304310 {
    char pad0[0x2300];
    int f2300;
    int f2304;
    int f2308;
    int f230C;
    int row; /* 0x2310 */
    int target; /* 0x2314 */
    float angle; /* 0x2318 */
    int f231C;
    int f2320;
    float dist; /* 0x2324 */
    float turn; /* 0x2328 */
    int f232C;
    int f2330;
} Data_304310;

typedef struct Row_304310 {
    u128 v0;
    u128 v10;
    u128 v20;
} Row_304310;

extern Moby *D_L12_0015FFD8;
extern char D_L12_00167240[];
extern f32 distance_xyz(void *, void *) __asm__("func_001F9B48");
extern f32 fast_add_rotations_c(f32, f32) __asm__("func_001FA580");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern float D_0015ED6C;
extern float D_L12_0015F580[] __attribute__((section(".sdata")));
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern int D_0013F3D0; /* no foreign declaration */
extern int D_L12_001B0930; /* no foreign declaration */
extern int collision_line() __asm__("FUN_001efa68");
extern int fast_bsphere_check() __asm__("FUN_001fa728");
extern int FUN_L00_00259888();
extern int is_point_inside_clip_volume_c(void *arg0, int arg1) __asm__("func_00214720");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void scale_vector_xyz(void *, void *, float) __asm__("FUN_001f9a68");
float absolute_float(float input) __asm__("func_001F99C0");
void FUN_L00_0025b6b8(f32 t, f32 *out, f32 *a, f32 *b);
void FUN_L12_00302c58(struct Moby *moby);
extern float func_L00_001FF860_304310(float, float) __asm__("FUN_001f9e90");
extern float func_001F9D10_304310(void *, void *) __asm__("FUN_001f9b48");
extern float func_001FA748_304310(float, float) __asm__("FUN_001fa580");
extern float func_001F9FA8_304310(float) __asm__("FUN_001f9de0");
extern float absolute_float(float) __asm__("FUN_001f99c0");
extern void func_L00_0025C710_304310(void *, void *, void *, float) __asm__("FUN_L00_0025b6b8");
extern float func_001F9D48_304310(void *, void *) __asm__("FUN_001f9b80");
extern void func_001F9BD8_304310(void *, void *, void *) __asm__("FUN_001f9a10");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern int func_L00_00200290_304310(void *, float) __asm__("FUN_001fa728");
extern int func_00215570_304310(void *, int) __asm__("FUN_00214720");
extern void func_L12_00304038_304310(void *) __asm__("FUN_L12_00302c58");
extern void func_L00_0025A8C0_304310(void *, void *, int, float, void *) __asm__("FUN_L00_00259888");
extern int func_L00_001EFFF0_304310(void *, void *, int, void *, void *) __asm__("FUN_001efa68");
extern Row_304310 *D_L12_001B0C30_304310[] __asm__("D_L12_001B0930");
extern char D_L12_00167240[];
extern float D_0013F4D0_304310[] __asm__("D_0013F3D0");

void FUN_L12_00302f30(Moby *moby) {
    enum {
        TARGET_TRACKING_STATE_INITIALIZE = 0,
        TARGET_TRACKING_STATE_ACTIVE = 1
    };
    Vec4 start;
    Vec4 end;
    char hit[0x30];
    Data_304310 *data = *(Data_304310 **)&moby->pvars;
    Row_304310 *row;
    float swing;
    float half;
    int target;

    if (data->target == -1) {
        return;
    }
    switch (moby->state) {
    case TARGET_TRACKING_STATE_INITIALIZE:
        D_L12_00161E08 = 0x16;
        D_L12_00161E04 = 0x2D;
        data->f231C = -1;
        data->f2304 = 0xB;
        data->f2308 = 0x17;
        data->f230C = 0x22;
        data->f2300 = 0;
        moby->state = 1;
        break;
    case TARGET_TRACKING_STATE_ACTIVE:
        moby->rot.z = func_L00_001FF860_304310(D_L12_0015FFD8[data->target].pos.x - moby->pos.x,
                                             D_L12_0015FFD8[data->target].pos.y - moby->pos.y);
        data->dist = func_001F9D10_304310(((float *)&moby->pos), (char *)D_L12_0015FFD8 + (data->target << 8) + 0x10);
        data->angle = func_001FA748_304310(data->angle, data->turn * 0.017453292f * D_0015ED6C);
        row = D_L12_001B0C30_304310[data->row];
        swing = func_001F9FA8_304310(data->angle);
        if (1.5707964f < data->angle && data->angle < 3.1415927f) {
            swing = 2.0f - swing;
        }
        if (-3.1415927f < data->angle && data->angle < -1.5707964f) {
            swing = 2.0f - swing;
        }
        half = 0.5f;
        swing = swing * half;
        swing = swing + half;
        if (1.0f < swing) {
            swing = swing - 2.0f;
        }
        swing = absolute_float(swing);
        start.q = row->v10;
        end.q = row->v20;
        func_L00_0025C710_304310(((float *)&moby->pos), &start, &end, swing);
        if (func_001F9D48_304310(((float *)&moby->pos), D_0013F4D0_304310) > 64.0f) {
            return;
        }
        if (moby->visible == 0 && D_L12_0015FFD8[data->target].visible == 0) {
            func_001F9BD8_304310(&start, ((float *)&moby->pos), ((float *)&D_L12_0015FFD8[data->target].pos));
            fast_vec_scale(&start, &start, half);
            start.f[3] = 16.0f;
            if (func_L00_00200290_304310(&start, 32.0f) == -1) {
                return;
            }
        }
        if (data->f2330 != -1 && func_00215570_304310(D_L12_00167240, data->f2330) != 0) {
            return;
        }
        if (data->f2320 == 0) {
            return;
        }
        func_L12_00304038_304310(moby);
        target = data->target;
        if (target == -1) {
            return;
        }
        qcopy(&start, &moby->pos.z);
        qcopy(&end, ((float *)&D_L12_0015FFD8[target].pos));
        func_L00_0025A8C0_304310(hit, moby, 0x10001, 1.0f, D_L12_0015F580);
        func_L00_001EFFF0_304310(&start, &end, 0, 0, hit);
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_003046C8), where it is exact; names translated to the US level program. */

extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("FUN_001f9cf8");
extern void FUN_L00_002ea9d8(void *);
extern void FUN_L00_002eaa30(void *);
extern void FUN_L08_00313ba8(void *);

extern char D_L12_00161EF0 __attribute__((sda));
void FUN_L12_003032e8(struct Moby *moby) {
    float position_and_rotation[8];
    char *data = (char *)moby->pvars;
    FUN_L08_00313ba8(moby);
    transform_vector_by_basis(position_and_rotation, &D_L12_00161EF0, &moby->rot_row0);
    fast_vec_add(position_and_rotation, position_and_rotation, &moby->pos);
    FUN_L00_002ea9d8(position_and_rotation);
    position_and_rotation[4] = 0;
    position_and_rotation[5] = -*(float *)(data + 0x74);
    position_and_rotation[6] = fast_add_rotations(1.5707964f, moby->rot.z);
    FUN_L00_002eaa30(position_and_rotation + 4);
}
extern float vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern int tick_countdown_32_alt(int *) __asm__("FUN_001f9740");
extern int scale_ticks(int) __asm__("FUN_001f96f8");

/* The view the targets are checked against: position, pitch and yaw. */
typedef struct {
    u8 pad0[0x140];
    Vec4 position;
    f32 unk150;
    f32 pitch;
    f32 yaw;
} View_303370;
extern View_303370 view_303370 __asm__("D_L12_00167100");

typedef struct {
    u8 pad0[0x40];
    s32 targets[10];
    u8 pad68[0x20];
    s32 timer;
} Data_303370;

/* Raises the timer while one of the ten targets is in the view cone, otherwise counts it down. */
void FUN_L12_00303370(void *moby) {
    Data_303370 *data = (Data_303370 *)((struct Moby *)moby)->pvars;
    int found = 0;
    int i;
    Vec4 position;

    for (i = 0; i < 10; i++) {
        if (data->targets[i] != -1) {
            Moby *target = &D_L12_0015FFD8[data->targets[i]];
            float yaw_limit;
            float pitch_limit;
            float yaw;
            float pitch;

            qcopy(&position, &target->pos);
            yaw_limit = 0.12217305f;
            pitch_limit = yaw_limit;
            if (target->oclass == 0x146)
                position.f[2] += 1.5f;
            if (target->oclass == 0x4FE) {
                yaw_limit = 0.034906585f;
                pitch_limit = 0.05235988f;
                position.f[2] += 1.5f;
            }
            yaw = fast_arc_tan(position.f[0] - view_303370.position.f[0], position.f[1] - view_303370.position.f[1]);
            pitch = fast_arc_tan(vector_distance(&view_303370.position, &position), position.f[2] - view_303370.position.f[2]);
            if (fast_difference_between_rotations(yaw, view_303370.yaw) < yaw_limit &&
                fast_difference_between_rotations(pitch, -view_303370.pitch) < pitch_limit)
                found = 1;
        }
    }
    if (!found) {
        tick_countdown_32_alt(&data->timer);
    } else {
        data->timer += 2;
        if (scale_ticks(30) < data->timer)
            data->timer = scale_ticks(30);
    }
}
#include "sda.h"

/* Update of moby class 1267 (Hoven turret): waits for the hero, is manned and aimed with the stick, fires, and burns out when destroyed. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00304920), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef float Vec_304920[4] __attribute__((aligned(16)));

typedef struct {
    char pad0;
    unsigned char b1;
    char pad2[0xE];
    char f10[0x60];
    short f70;
    short f72;
    float f74;
    float f78;
    float f7C;
    int f80;
    int f84;
    int f88;
    float f8C;
    int f90;
    int f94;
    int f98;
    int f9C;
    float fA0;
    float fA4;
    int fA8;
    short fAC;
    short fAE;
} Data_304920;

typedef struct {
    char pad0[0x140];
    float f140[4];
    char pad150[4];
    float f154;
    float f158;
    char pad15C[4];
    float f160;
    char pad164[4];
    int f168;
    char pad16C[4];
    float f170;
    char pad174[4];
    int f178;
} Cam_304920;

typedef struct {
    char pad0[0x6C];
    struct Moby *f6C;
} Snd_304920;

typedef struct {
    char pad0[0x2C];
    float f2C;
} Hit_304920;

extern char *FUN_L00_0025a420(void *, int, int);
extern char *FUN_L00_0026dd20(void *, void *, int, int, float);
extern char voice_pool[] __asm__("D_0013E550") __attribute__((section(".data")));
extern f32 fast_add_rotations_c3(f32, f32) __asm__("func_001FA580");
extern f32 fast_subtract_rotations(f32, f32) __asm__("func_001FA5C8");
extern f32 random_float_between_c(f32, f32) __asm__("func_002132A8");
extern f32 vector_length_xyz(void *) __asm__("FUN_001f9af0");
extern float D_0015ED6C_gp __asm__("D_0015ED6C") __attribute__((section(".sdata")));
extern float D_L12_00161E60 __attribute__((sda));
extern float D_L12_00161E64 __attribute__((sda));
extern float D_L12_00161EA8 __attribute__((sda));
extern float D_L12_00161EAC __attribute__((sda));
extern float D_L12_00161ED0 __attribute__((sda));
extern float D_L12_00161ED4 __attribute__((sda));
extern float D_L12_00161ED8 __attribute__((sda));
extern float D_L12_00161EDC __attribute__((sda));
extern float D_L12_00161EE0 __attribute__((sda));
extern float D_L12_00161EE4 __attribute__((sda));
extern float D_L12_00161EE8 __attribute__((sda));
extern float randf_sym(float, float) __asm__("FUN_L00_00257c48");
extern float approach_value(float *, float, float);
extern float D_L12_0015F580_f[] __asm__("D_L12_0015F580") __attribute__((section(".sdata")));
extern int D_L12_0015F5C4_macro __asm__("D_L12_0015F5C4") __attribute__((section(".sdata")));
extern int D_L12_001619A0 __attribute__((section(".sdata")));
extern int D_L12_00161F00 __attribute__((sda));
extern int D_L12_00161F04 __attribute__((sda));
extern int D_L12_00161F20 __attribute__((section(".sdata")));
extern int D_L12_00161F24 __attribute__((section(".sdata")));
extern int fast_dec_timer_s16(void *) __asm__("FUN_001f9770");
extern int random_int_between(int, int) __asm__("FUN_L00_00257b90");
extern int FUN_L12_002400d0(int, int);
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern void FUN_L00_001fff28(void *, int, float);
extern void FUN_L00_00211250(void);
extern void FUN_L00_00216f90(void *, void *, int, int);
extern void FUN_L00_0024f7c8(void *, int, void *);
extern void jet_clear_persistent(int) __asm__("FUN_L00_002502a0");
extern void spawn_explosion(void *, void *, void *, float, float, int, int, int, float, float, float, int, float, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_002607d0(int);
extern void FUN_L00_00262cf0(void *, void *, float, float, float);
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void FUN_L00_00299460(int);
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L08_002e7eb0(void *, void *, void *, int);
extern void FUN_L12_00303370(void *);
extern void FUN_L12_00304218_c() __asm__("FUN_L12_00304218");
extern void FUN_L12_00304d98_c() __asm__("FUN_L12_00304d98");
extern void allocate_voice_for_target_entry_alt(int, int, int) __asm__("FUN_0022da68");
extern void attach_manipulator(void *, int, void *);
extern void blend_moby_animation_c(void *, s32, s32, s32) __asm__("FUN_00212f90");
extern void build_spherical_offset(void *, f32, f32, f32) __asm__("func_00214DB0");
extern void detach_manipulator(void *, void *);
extern void enqueue_callback_list_1(void *, void *) __asm__("FUN_001f4600");
extern void memcard_save_data(s32, s32) __asm__("func_0020B178");
extern void normalize_vector_xyz(void *, void *, f32) __asm__("FUN_001f9bf8");
extern void set_moby_animation(void *, s32, s32) __asm__("func_00212ED8");
extern void tick_countdown_32(s32 *) __asm__("func_001F9740");
void FUN_L12_003032e8_c(struct Moby *moby) __asm__("FUN_L12_003032e8");
extern int D_0013A5E0_e[] __asm__("D_0013CAE0") __attribute__((section(".data")));
extern int current_level_index __asm__("D_0015ED84") __attribute__((section(".sdata")));
extern char D_0013E533[];
extern unsigned char D_0014171B_304920[] __asm__("D_0014C050");
extern int D_0015EED0_304920[] __asm__("D_0015EDDC") __attribute__((section(".sdata")));
extern Moby *D_L12_0015FFD8_gp __asm__("D_L12_0015FFD8") __attribute__((section(".sdata")));
extern char D_L12_00167180_304920[] __asm__("D_L12_00167100");
extern int fast_dec_timer(void *) __asm__("FUN_001f9740");
extern int func_0020BFC8_304920(int, int) __asm__("FUN_0020b178");

extern f32 frame_scale_macro __asm__("D_0015ED60") __attribute__((section(".sdata")));
extern f32 frame_time_sq_macro __asm__("D_0015ED70") __attribute__((section(".sdata")));
void hoven_turret_update(struct Moby *moby) __asm__("FUN_L12_00303540");
void hoven_turret_update(struct Moby *moby) {
    enum {
        HOVEN_TURRET_STATE_INITIALIZE = 0,
        HOVEN_TURRET_STATE_WAIT_FOR_BOARDING = 3,
        HOVEN_TURRET_STATE_MANNED = 4,
        HOVEN_TURRET_STATE_START_SCENE = 5,
        HOVEN_TURRET_STATE_INACTIVE = 6,
        HOVEN_TURRET_STATE_DESTRUCTION_EFFECTS = 7,
        HOVEN_TURRET_STATE_SAVE_DATA = 8
    };
    Data_304920 *data = *(Data_304920 **)&moby->pvars;

    ((Snd_304920 *)(D_0013E533 + 0x1D))->f6C = moby;
    switch (moby->state) {
    case HOVEN_TURRET_STATE_INITIALIZE:
        D_L12_00161F00 = 0;
        D_L12_00161F04 = 0;
        if (D_0014171B_304920[moby->mission + (current_level_index << 4)] == 0xFF) {
            moby->state = 6;
        } else {
            attach_manipulator(moby, 0, data);
            moby->state = 3;
            data->f84 = 0xC8;
            moby->rot.y = 0.0f;
            data->f70 = scale_game_frames_alt(0x1E);
            data->f72 = 0;
            data->f78 = 0.0f;
            data->f7C = 0.0f;
            data->fA0 = moby->pos.z;
            data->f98 = -1;
            moby->rot.z = 3.1415927f;
            ((Snd_304920 *)(D_0013E533 + 0x1D))->f6C = moby;
        }
        break;
    case HOVEN_TURRET_STATE_WAIT_FOR_BOARDING: {
        {
            Vec_304920 out;

            FUN_L00_0024f7c8(moby, 3, out);
        }
        if (vector_distance(((float *)&moby->pos), ((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f) < 7.0f
            || ((Hero *)(D_0013E533 + 0xE1D))->ground_moby == moby) {
            moby->pos.z = moby->pos.z + ((data->fA0 - 1.2f) - moby->pos.z) * 0.11f;
            data->fA4 = fast_add_rotations_c3(data->fA4, fast_add_rotations_c3(fast_subtract_rotations(fast_arc_tan(((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f[0] - moby->pos.x, ((Hero *)(D_0013E533 + 0xE1D))->motion.pos.f[1] - moby->pos.y), moby->rot.z), 1.5707964f) * 0.01f) * 0.9f;
            moby->rot.z = fast_add_rotations_c3(moby->rot.z, data->fA4);
        } else {
            if (moby->pos.z < data->fA0) {
                moby->pos.z = moby->pos.z + D_0015ED6C_gp;
            } else {
                moby->pos.z = data->fA0;
            }
            if (((Hero *)(D_0013E533 + 0xE1D))->ground_moby != moby && data->f98 != -1) {
                allocate_voice_for_target_entry_alt(2, 0, (int)moby);
            }
        }
        if (vector_distance(((float *)&moby->pos), ((float *)&((Hero *)(D_0013E533 + 0xE1D))->moby->pos)) < 1.3f) {
            if (scale_game_frames_alt(0xF) < ((Hero *)(D_0013E533 + 0xE1D))->grounded_frames) {
                jet_mount(((float *)&moby->pos), ((float *)&moby->rot), 1, 0, 0);
                if (moby->prev_seq != 5) {
                    set_moby_animation(moby, 5, 0);
                }
                FUN_L12_002400d0(0x32, 1);
                FUN_L12_003032e8_c((char *)moby);
                data->f8C = 200.0f;
                moby->hit_slot = 0xFF;
                D_L12_00161F20++;
                moby->state = 4;
            }
        }
        break;
    }
    case HOVEN_TURRET_STATE_MANNED:
        D_L12_00161F00 = 1;
        ((Hero *)(D_0013E533 + 0xE1D))->unk20A5 = 1;
        ((Hero *)(D_0013E533 + 0xE1D))->unk1CC = scale_game_frames_alt(0x78);
        ((Hero *)(D_0013E533 + 0xE1D))->invulnerable_frames = scale_game_frames_alt(0x78);
        if (D_L12_0015F5C4_macro != 2 && D_L12_001619A0 == 0) {
            enqueue_callback_list_1(FUN_L12_00304218_c, moby);
        }
        FUN_L12_00303370(moby);
        if (fast_dec_timer(&data->f90)) {
            Hit_304920 *hit = (Hit_304920 *)FUN_L00_0025a420(moby, 0x10000, 0);
            if (hit != 0) {
                data->f90 = scale_game_frames_alt(4);
                data->f8C = data->f8C - hit->f2C;
            }
            moby->hit_slot = 0xFF;
        }
        if (D_L12_00161F24 == 0) {
            if (((Hero *)(D_0013E533 + 0xE1D))->ship_flags & 1) {
                if (data->f8C > 100.0f) {
                    D_L12_00161F20--;
                }
                ((Hero *)(D_0013E533 + 0xE1D))->ship_flags = 0;
                FUN_L00_00211250();
            }
            if (data->f8C <= 0.0f) {
                D_L12_00161F04 = 1;
                data->fAC = scale_game_frames_alt(0x78);
                moby->state = 7;
                break;
            }
        }
        if (data->f94 != -1 && (signed char)((struct Moby *)((data->f94 << 8) + (*(int *)&D_L12_0015FFD8_gp)))->state < 0) {
            if (D_L12_0015F5C4_macro == 2) {
                if (data->b1 != 0) {
                    detach_manipulator(moby, data);
                }
                if (moby->prev_seq != 0) {
                    set_moby_animation(moby, 0, 0);
                }
                moby->pos.z = data->fA0;
                moby->rot.z = 3.1415927f;
            } else {
                jet_clear_persistent(moby->mission);
                FUN_L00_00299460(0xD);
                FUN_L00_002607d0(0xD);
                {
                    Vec_304920 cpos;
                    Vec_304920 crot;

                    *(u128 *)&cpos = 0;
                    *(u128 *)&crot = 0;
                    cpos[0] = 328.4f;
                    cpos[1] = 338.7f;
                    cpos[2] = 60.98f;
                    crot[2] = -1.7f;

                    D_L12_00161F00 = 0;
                    FUN_L00_00216f90(cpos, crot, 0, 1);
                }
                moby->state = 5;
                break;
            }
        }
        if (D_L12_001619A0 != 0) {
            break;
        }
        {
            float joy[2];
            float y;
            float tgt;
            float tgt2;

            joy[0] = ((Hero *)(D_0013E533 + 0xE1D))->stick_x;
            y = ((Hero *)(D_0013E533 + 0xE1D))->stick_y;
            if (D_0015EED0_304920[0] == 0) {
                y = -y;
            }
            joy[1] = y;
            tgt = D_L12_00161ED0 * 0.017453292f * D_0015ED6C_gp * -joy[0];
            if ((data->f7C >= 0.0f && data->f7C < tgt) || (data->f7C <= 0.0f && tgt < data->f7C)) {
                approach_value(&data->f7C, tgt, D_L12_00161ED8 * 0.017453292f * frame_time_sq_macro);
            } else {
                approach_value(&data->f7C, tgt, D_L12_00161EE0 * 0.017453292f * frame_time_sq_macro);
            }
            moby->rot.z = fast_add_rotations_c3(moby->rot.z, data->f7C);
            tgt2 = -(D_L12_00161ED4 * 0.017453292f * D_0015ED6C_gp) * -joy[1];
            if ((data->f78 >= 0.0f && data->f78 < tgt2) || (data->f78 <= 0.0f && tgt2 < data->f78)) {
                approach_value(&data->f78, tgt2, D_L12_00161EDC * 0.017453292f * frame_time_sq_macro);
            } else {
                approach_value(&data->f78, tgt2, D_L12_00161EE4 * 0.017453292f * frame_time_sq_macro);
            }
            data->f74 = fast_add_rotations_c3(data->f74, data->f78);
            if (data->f74 > D_L12_00161EAC) {
                data->f74 = D_L12_00161EAC;
            }
            if (data->f74 < D_L12_00161EA8) {
                data->f74 = D_L12_00161EA8;
            }
            FUN_L00_001fff28(data->f10, 1, -data->f74);
            if (moby->anim_flags & 2) {
                blend_moby_animation_c(moby, 5, 0, 1);
            }
            fast_dec_timer_s16(&data->f70);
            if ((D_0013A5E0_e[0] & 0xA0) && data->f70 == 0) {
                Vec_304920 dir;
                Vec_304920 muz;
                float yaw;
                int i;

                blend_moby_animation_c(moby, data->f80 == 1 ? 4 : 3, 0, 1);
                yaw = fast_add_rotations_c3(moby->rot.z, 1.5707964f);
                build_spherical_offset(dir, D_L12_00161EE8 * D_0015ED6C_gp, yaw, data->f74);
                if (data->f80 == 1) {
                    data->f80 = 0;
                    FUN_L00_0024f7c8(moby, 2, muz);
                } else {
                    FUN_L00_0024f7c8(moby, 1, muz);
                    data->f80 = 1;
                }
                add_vector_xyz(muz, muz, dir);
                add_vector_xyz(muz, muz, dir);
                add_vector_xyz(muz, muz, dir);
                FUN_L08_002e7eb0(moby, muz, dir, 0);
                allocate_voice_for_target_entry_alt(0, 0, (int)moby);
                ((Cam_304920 *)D_L12_00167180_304920)->f160 = 0.03f;
                ((Cam_304920 *)D_L12_00167180_304920)->f168 = scale_game_frames_alt(0xA);
                ((Cam_304920 *)D_L12_00167180_304920)->f170 = 0.03f;
                ((Cam_304920 *)D_L12_00167180_304920)->f178 = scale_game_frames_alt(7);
                data->f70 = scale_game_frames_alt(0x14);
                for (i = 0; i < 5; i++) {
                    Vec_304920 vel;
                    Vec_304920 rnd;

                    *(u128 *)rnd = 0;
                    rnd[0] = random_float_between_c(-1.0f, 1.0f);
                    rnd[1] = random_float_between_c(-1.0f, 1.0f);
                    rnd[2] = random_float_between_c(-1.0f, 1.0f);
                    *(u128 *)vel = *(u128 *)rnd;
                    normalize_vector_xyz(vel, vel, vector_length_xyz(dir) * (frame_scale_macro * (0.65f - 1.0f) + 1.0f));
                    add_vector_xyz(vel, dir, vel);
                    normalize_vector_xyz(vel, vel, random_float_between_c(D_0015ED6C_gp * 3.0f, D_0015ED6C_gp * 6.0f));
                    FUN_L00_0026dd20(muz, vel, 0x5F2F4F6F, random_int_between(scale_game_frames_alt(5), scale_game_frames_alt(0xA)), 50000.0f);
                }
                normalize_vector_xyz(dir, dir, D_0015ED6C_gp * 0.5f);
                FUN_L00_0026dd20(muz, dir, 0x2F4F7F7F, random_int_between(scale_game_frames_alt(4), scale_game_frames_alt(7)), 500000.0f);
            }
            if (D_L12_001619A0 == 0) {
                FUN_L12_003032e8_c((char *)moby);
            }
        }
        break;
    case HOVEN_TURRET_STATE_DESTRUCTION_EFFECTS:
        enqueue_callback_list_1(FUN_L12_00304d98_c, moby);
        if (fast_dec_timer_s16(&data->fAE)) {
            Vec_304920 v;
            float a;
            float b;
            float c;
            float t;

            a = random_float_between_c(D_L12_00161E60, D_L12_00161E64);
            b = fast_add_rotations_c3(((Cam_304920 *)D_L12_00167180_304920)->f158, randf_sym(0.0f, 0.5235988f));
            c = fast_add_rotations_c3(((Cam_304920 *)D_L12_00167180_304920)->f154, randf_sym(0.0f, 0.5235988f));
            qcopy(v, ((Cam_304920 *)D_L12_00167180_304920)->f140);
            FUN_L00_00262cf0(v, v, a, b, c);
            t = random_float_between_c(0.5f, 1.3f);
            spawn_explosion(moby, D_L12_0015F580_f, v, 0.0f, 0.0f, 7, 0xF, 0x19, t, t * 0.57f, 10.0f, 3, 0.7f, 0.0f, 0, 0, -1, 0);
            data->fAE = scale_game_frames_alt(random_int_between(5, 0xD));
        }
        if (fast_dec_timer_s16(&data->fAC)) {
            FUN_L00_00211250();
        }
        break;
    case HOVEN_TURRET_STATE_START_SCENE:
        if (D_L12_0015F5C4_macro != 2) {
            start_scene(1);
            moby->state = 8;
        }
        break;
    case HOVEN_TURRET_STATE_SAVE_DATA:
        if (D_L12_0015F5C4_macro != 2) {
            func_0020BFC8_304920(0, -1);
            moby->state = 6;
        }
        break;
    case HOVEN_TURRET_STATE_INACTIVE:
        if (moby->prev_seq != 0) {
            set_moby_animation(moby, 0, 0);
        }
        break;
    }
}
/* Draws the HUD gauge: a four-part frame, a 32-segment dial that fades with the data's charge, and two rotating needles. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_003055F8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0x74];
    float f74;
    char pad78[0x10];
    int f88;
    float f8C;
} Data_3055F8;

extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern f32 fast_add_rotations_c2(f32, f32) __asm__("func_001FA580");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_normalize_angle(f32) __asm__("func_001FA610");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int scale_game_frames_alt(int) __asm__("FUN_001f96f8");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern short D_L12_00161E14 __attribute__((sda));
extern short D_L12_00161E18 __attribute__((sda));
extern short D_L12_00161E1C __attribute__((sda));
extern short D_L12_00161E20 __attribute__((sda));
extern short D_L12_00161E24 __attribute__((sda));
extern short D_L12_00161E28 __attribute__((sda));
extern int D_L12_00161E2C __attribute__((sda));
extern int D_L12_00161E30 __attribute__((sda));
extern int D_L12_00161E34 __attribute__((sda));
extern int D_L12_00161E38 __attribute__((sda));
extern int D_L12_00161E3C __attribute__((sda));
extern int D_L12_00161E40 __attribute__((sda));
extern short D_L12_00161E44 __attribute__((sda));
extern short D_L12_00161E48 __attribute__((sda));
extern int D_L12_00161E4C __attribute__((sda));
extern int D_L12_00161E50 __attribute__((sda));
extern int D_L12_00161E54 __attribute__((sda));
extern int D_L12_00161E58 __attribute__((sda));
extern int D_L12_00161E5C __attribute__((sda));
extern int D_L12_00161EB0 __attribute__((sda));
extern int D_L12_00161F08 __attribute__((sda));
extern int D_L12_00161F0C __attribute__((sda));
extern int D_L12_00161F10 __attribute__((sda));
extern int D_L12_00161F14 __attribute__((sda));
extern void vu1_add_g_sregister_c(s32, u64) __asm__("func_00233980");
void FUN_L02_0020bc88(long *m, int *b, int *a, long c, int flag);
extern long func_001F4868_3055F8(int) __asm__("FUN_001f44b8");
extern int truncate_float_to_s32(float) __asm__("FUN_001fa6d0");
extern float func_001FA7D8_3055F8(float) __asm__("FUN_001fa610");
extern float convert_integer_to_float(int) __asm__("FUN_001fa6c0");
extern void func_L02_0020BF88_3055F8(void *, void *, void *, long, int) __asm__("FUN_L02_0020bc88");

void FUN_L12_00304218(Moby *moby) {
    int uv[4];
    long xy[4];
    int colors[4];
    Data_3055F8 *data = *(Data_3055F8 **)&moby->pvars;
    long texture;
    int filled;
    int color;
    int dim_color;
    int i;
    int j;
    float angle;

    vu1_add_g_sregister_c(0x42, 0x8000000044L);
    uv[0] = D_L12_00161F08;
    uv[1] = D_L12_00161F0C;
    uv[2] = D_L12_00161F10;
    uv[3] = D_L12_00161F14;
    colors[3] = D_L12_00161EB0;
    colors[2] = D_L12_00161EB0;
    colors[1] = D_L12_00161EB0;
    colors[0] = D_L12_00161EB0;
    texture = func_001F4868_3055F8(D_L12_00161E5C + 0x28);
    for (i = 0; i < 4; i++) {
        float sign_x;
        float sign_y;
        if (i & 1) {
            sign_x = -1.0f;
        } else {
            sign_x = 1.0f;
        }
        if (i >= 2) {
            sign_y = -1.0f;
        } else {
            sign_y = 1.0f;
        }
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (u32)((truncate_float_to_s32((*(float *)&D_L12_00161E18 + *(float *)&D_L12_00161E20 * sign_y) * 16.0f) << 16) - (int)0x80000000);
        xy[0] += (truncate_float_to_s32(*(float *)&D_L12_00161E14 + *(float *)&D_L12_00161E1C * sign_x) << 4) + 0x8000;
        xy[1] += (u32)((truncate_float_to_s32(*(float *)&D_L12_00161E18 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (truncate_float_to_s32(*(float *)&D_L12_00161E14 + *(float *)&D_L12_00161E1C * sign_x) << 4) + 0x8000;
        xy[2] += (u32)((truncate_float_to_s32((*(float *)&D_L12_00161E18 + *(float *)&D_L12_00161E20 * sign_y) * 16.0f) << 16) - (int)0x80000000);
        xy[2] += (truncate_float_to_s32(*(float *)&D_L12_00161E14) << 4) + 0x8000;
        xy[3] += (u32)((truncate_float_to_s32(*(float *)&D_L12_00161E18 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (truncate_float_to_s32(*(float *)&D_L12_00161E14) << 4) + 0x8000;
        func_L02_0020BF88_3055F8(xy, uv, colors, texture, 1);
    }
    texture = func_001F4868_3055F8(6);
    uv[0] = D_L12_00161E2C;
    uv[1] = D_L12_00161E30;
    uv[2] = D_L12_00161E34;
    uv[3] = D_L12_00161E38;
    if (data->f8C > 100.0f) {
        color = fast_tween_color(0x6000FFFF, 0x6000FF00, (data->f8C - 100.0f) / 100.0f);
    } else {
        color = fast_tween_color(0x600000FF, 0x6000FFFF, data->f8C / 100.0f);
    }
    filled = truncate_float_to_s32((1.0f - data->f8C / 200.0f) * 32.0f);
    dim_color = (color & 0xFFFFFF) + 0x20000000;
    for (j = 0; j < 32; j++) {
        float start_angle, end_angle;
        int x0, y0, x1, y1;
        if (j >= filled) {
            colors[3] = color;
            colors[2] = color;
            colors[1] = color;
            colors[0] = color;
        } else {
            colors[3] = dim_color;
            colors[2] = dim_color;
            colors[1] = dim_color;
            colors[0] = dim_color;
        }
        start_angle = func_001FA7D8_3055F8((float)j * 0.19634955f);
        end_angle = fast_add_rotations_c2(start_angle, 0.19634955f);
        x0 = truncate_float_to_s32(*(float *)&D_L12_00161E24 * fast_sin(start_angle));
        y0 = truncate_float_to_s32(*(float *)&D_L12_00161E28 * -fast_cos(start_angle));
        x1 = truncate_float_to_s32(*(float *)&D_L12_00161E24 * fast_sin(end_angle));
        y1 = truncate_float_to_s32(*(float *)&D_L12_00161E28 * -fast_cos(end_angle));
        xy[3] = 0xFFFFF000000000L;
        xy[2] = 0xFFFFF000000000L;
        xy[1] = 0xFFFFF000000000L;
        xy[0] = 0xFFFFF000000000L;
        xy[0] += (u32)((truncate_float_to_s32((*(float *)&D_L12_00161E18 + (float)y1) * 16.0f) << 16) - (int)0x80000000);
        xy[0] += (truncate_float_to_s32(*(float *)&D_L12_00161E14 + (float)x1) << 4) + 0x8000;
        xy[1] += (u32)((truncate_float_to_s32(*(float *)&D_L12_00161E18 * 16.0f) << 16) - (int)0x80000000);
        xy[1] += (truncate_float_to_s32(*(float *)&D_L12_00161E14) << 4) + 0x8000;
        xy[2] += (u32)((truncate_float_to_s32((*(float *)&D_L12_00161E18 + (float)y0) * 16.0f) << 16) - (int)0x80000000);
        xy[2] += (truncate_float_to_s32(*(float *)&D_L12_00161E14 + (float)x0) << 4) + 0x8000;
        xy[3] += (u32)((truncate_float_to_s32(*(float *)&D_L12_00161E18 * 16.0f) << 16) - (int)0x80000000);
        xy[3] += (truncate_float_to_s32(*(float *)&D_L12_00161E14) << 4) + 0x8000;
        func_L02_0020BF88_3055F8(xy, uv, colors, texture, 1);
    }
    if (scale_game_frames_alt(0xF) < data->f88) {
        color = fast_tween_color(0x5000FFFF, 0x500000FF, convert_integer_to_float(data->f88 - scale_game_frames_alt(0xF)) / (float)scale_game_frames_alt(0xF));
    } else {
        color = fast_tween_color(0x5000FF00, 0x5000FFFF, convert_integer_to_float(data->f88) / (float)scale_game_frames_alt(0xF));
    }
    uv[0] = D_L12_00161E4C;
    uv[1] = D_L12_00161E50;
    uv[2] = D_L12_00161E54;
    uv[3] = D_L12_00161E58;
    colors[3] = color;
    colors[2] = color;
    colors[1] = color;
    colors[0] = color;
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = func_001FA7D8_3055F8(fast_add_rotations_c2(moby->rot.z * 4.0f, 0.0f));
    xy[0] += (u32)((truncate_float_to_s32(fast_sin(angle) * *(float *)&D_L12_00161E48 * 16.0f) << 16) - (int)0x80000000);
    xy[0] += truncate_float_to_s32(fast_cos(angle) * *(float *)&D_L12_00161E44 * 16.0f) + 0x8000;
    angle = func_001FA7D8_3055F8(fast_add_rotations_c2(moby->rot.z * 4.0f, 1.5707964f));
    xy[1] += (u32)((truncate_float_to_s32(fast_sin(angle) * *(float *)&D_L12_00161E48 * 16.0f) << 16) - (int)0x80000000);
    xy[1] += truncate_float_to_s32(fast_cos(angle) * *(float *)&D_L12_00161E44 * 16.0f) + 0x8000;
    angle = func_001FA7D8_3055F8(fast_add_rotations_c2(moby->rot.z * 4.0f, -1.5707964f));
    xy[2] += (u32)((truncate_float_to_s32(fast_sin(angle) * *(float *)&D_L12_00161E48 * 16.0f) << 16) - (int)0x80000000);
    xy[2] += truncate_float_to_s32(fast_cos(angle) * *(float *)&D_L12_00161E44 * 16.0f) + 0x8000;
    angle = func_001FA7D8_3055F8(fast_add_rotations_c2(moby->rot.z * 4.0f, 3.1415927f));
    xy[3] += (u32)((truncate_float_to_s32(fast_sin(angle) * *(float *)&D_L12_00161E48 * 16.0f) << 16) - (int)0x80000000);
    xy[3] += truncate_float_to_s32(fast_cos(angle) * *(float *)&D_L12_00161E44 * 16.0f) + 0x8000;
    func_L02_0020BF88_3055F8(xy, uv, colors, get_effect_texture(D_L12_00161E3C), 1);
    xy[3] = 0xFFFFF000000000L;
    xy[2] = 0xFFFFF000000000L;
    xy[1] = 0xFFFFF000000000L;
    xy[0] = 0xFFFFF000000000L;
    angle = func_001FA7D8_3055F8(fast_add_rotations_c2(data->f74 * 8.0f, 0.0f));
    xy[0] += (u32)((truncate_float_to_s32(fast_sin(angle) * *(float *)&D_L12_00161E48 * 16.0f) << 16) - (int)0x80000000);
    xy[0] += truncate_float_to_s32(fast_cos(angle) * *(float *)&D_L12_00161E44 * 16.0f) + 0x8000;
    angle = func_001FA7D8_3055F8(fast_add_rotations_c2(data->f74 * 8.0f, 1.5707964f));
    xy[1] += (u32)((truncate_float_to_s32(fast_sin(angle) * *(float *)&D_L12_00161E48 * 16.0f) << 16) - (int)0x80000000);
    xy[1] += truncate_float_to_s32(fast_cos(angle) * *(float *)&D_L12_00161E44 * 16.0f) + 0x8000;
    angle = func_001FA7D8_3055F8(fast_add_rotations_c2(data->f74 * 8.0f, -1.5707964f));
    xy[2] += (u32)((truncate_float_to_s32(fast_sin(angle) * *(float *)&D_L12_00161E48 * 16.0f) << 16) - (int)0x80000000);
    xy[2] += truncate_float_to_s32(fast_cos(angle) * *(float *)&D_L12_00161E44 * 16.0f) + 0x8000;
    angle = func_001FA7D8_3055F8(fast_add_rotations_c2(data->f74 * 8.0f, 3.1415927f));
    xy[3] += (u32)((truncate_float_to_s32(fast_sin(angle) * *(float *)&D_L12_00161E48 * 16.0f) << 16) - (int)0x80000000);
    xy[3] += truncate_float_to_s32(fast_cos(angle) * *(float *)&D_L12_00161E44 * 16.0f) + 0x8000;
    func_L02_0020BF88_3055F8(xy, uv, colors, get_effect_texture(D_L12_00161E40), 1);
}
extern int scale_ticks(int) __asm__("FUN_001f96f8");

#define NOT_SDA

#define MACRO_ADDR

/* Scale and play the vendor cue while its counter is below the limit. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_00306178), where it is exact; names translated to the US level program. */

typedef struct {
    char pad0[0xAC];
    short value;
} Level12VendorCounterData;

typedef struct {
    char pad0[0x78];
    Level12VendorCounterData *data;
} Level12VendorCounterMoby;

void FUN_L12_00304d98(Level12VendorCounterMoby *moby) {
    Level12VendorCounterData *data = moby->data;
    int count = scale_ticks(0x50);
    int value = data->value;
    if (value < count) {
        emit_rgba_draw_packet(0xAA, 0, 0, (count - value) * 0x50 / count);
    }
}
extern void find_point_ahead(struct Moby *, void *, float) __asm__("FUN_L01_00274b78");
extern void *find_moby_hit_304e00(struct Moby *, int, int) __asm__("FUN_L00_0025a420");
extern int FUN_00213928_304e00(struct Moby *, void *, void *, int, int *, int, int, int) __asm__("FUN_00213928");
extern float vector_distance_304e00(void *, void *) __asm__("FUN_001f9b80");
extern float fast_sin_304e00(float) __asm__("FUN_001f9de0");
extern float fast_cos_304e00(float) __asm__("FUN_001f9dc8");
extern float angle_atan2_304e00(float, float) __asm__("FUN_001f9e90");
extern float fast_add_rotations_304e00(float, float) __asm__("FUN_001fa580");
extern void add_vector_xyz_304e00(void *, void *, void *) __asm__("FUN_001f9a10");
extern void blend_moby_animation_304e00(struct Moby *, int, int, int) __asm__("FUN_00212f90");
extern float probe_ground_height(void *, int, float) __asm__("FUN_00213508");
extern int collect_mobies_in_sphere(void *, int, struct Moby *, void *, float) __asm__("FUN_L00_001f2868");
extern int coll_sphere(void *, int, struct Moby *, float) __asm__("FUN_L00_001f0d60");
extern void FUN_L00_0025b8c0_304e00(float *, float *, float, float, float, float) __asm__("FUN_L00_0025b8c0");
extern void spawn_explosion_304e00(struct Moby *, void *, void *, float, float, int, int, int, float, float, float, float, int, float, int, int, int, int) __asm__("FUN_L00_0025e450");
extern void FUN_L00_00257470(struct Moby *, int, int);
extern void jet_animate(float, struct Moby *) __asm__("FUN_L00_00259fe8");
extern void mark_moby_for_removal_304e00(struct Moby *) __asm__("FUN_0020c828");

typedef struct {
    u8 pad0[0x20];
    s32 damage[0x10];
    Vec4 start;
    f32 speed;
    f32 bob;
    f32 bob_angle;
    f32 climb;
    f32 turn;
    s32 timer;
} Data_304e00;

typedef struct {
    u8 pad0[0x20];
    struct Moby *moby;
} HitSource_304e00;

extern struct Moby *hit_moby __asm__("D_L12_001742D8") __attribute__((section(".data")));
/* A homing mine: rises, then steers toward the point ahead and blows up near it or on contact. */
void FUN_L12_00304e00(struct Moby *moby) {
    enum {
        HOMING_MINE_STATE_INITIALIZE = 0,
        HOMING_MINE_STATE_HOMING = 1,
        HOMING_MINE_STATE_DETONATE = 2
    };
    Data_304e00 *data = (Data_304e00 *)moby->pvars;
    s32 *damage = data->damage;
    f32 target[20];
    f32 velocity[3];
    u8 hit_info[0x30];
    Vec4 direction;
    Vec4 probe;
    int hits;
    HitSource_304e00 *source;
    float angle;
    float turn;
    float height;
    float difference;

    find_point_ahead(moby, target, 8.0f);
    source = find_moby_hit_304e00(moby, 0x230000, 0);
    FUN_00213928_304e00(moby, source, damage, 0, &hits, 0, 0, 4);
    if (vector_distance_304e00(&moby->pos, target) < 0.25f ||
        (hits >= 2 && source != NULL && source->moby->oclass != 0x4F5)) {
        spawn_explosion_304e00(moby, D_L12_0015F580, 0, 0.0f, 0.0f, 3, 3, 5,
            1.0f, 0.5f, 4.0f, 0.7f, 2, 7.0f, 0, 0, -1, 0);
        FUN_L00_00257470(moby, 0, -1);
        mark_moby_for_removal_304e00(moby);
        return;
    }
    switch (moby->state) {
    case HOMING_MINE_STATE_INITIALIZE:
        moby->flags |= 0x1000;
        qcopy(&data->start, &moby->pos);
        moby->state = 1;
        data->turn = 0.6981317f;
        if (moby->parent != NULL && ((struct Moby *)moby->parent)->command != 0)
            data->turn = -data->turn;
        data->timer = 0;
        break;
    case HOMING_MINE_STATE_HOMING:
        data->timer++;
        if (data->timer < scale_ticks(30))
            moby->pos.z += fast_sin_304e00((f32)data->timer * 3.1415927f / (f32)scale_ticks(30)) * 0.07f;
        if (scale_ticks(15) < data->timer)
            data->speed += frame_time_sq * 30.0f;
        if (scale_ticks(30) < data->timer) {
            if (moby->prev_seq == 0)
                blend_moby_animation_304e00(moby, 1, 0, 2);
            if (moby->prev_seq == 1 && (moby->anim_flags & 2))
                blend_moby_animation_304e00(moby, 2, 0, 2);
        }
        if (frame_time * 5.75f < data->speed)
            data->speed = frame_time * 5.75f;
        angle = angle_atan2_304e00(target[0] - moby->pos.x, target[1] - moby->pos.y);
        turn = angle;
        if (vector_distance_304e00(&moby->pos, target) < 7.0f)
            turn = fast_add_rotations_304e00(angle, data->turn);
        velocity[0] = fast_cos_304e00(turn) * data->speed;
        velocity[1] = fast_sin_304e00(turn) * data->speed;
        velocity[2] = 0.0f;
        add_vector_xyz_304e00(&moby->pos, &moby->pos, velocity);
        if (scale_ticks(30) < data->timer) {
            data->bob_angle = fast_add_rotations_304e00(data->bob_angle, frame_time * 5.2359877f);
            moby->pos.z -= data->bob;
            data->bob = fast_sin_304e00(data->bob_angle) * 0.1f;
            moby->pos.z += data->bob;
        }
        if (scale_ticks(30) < data->timer) {
            height = probe_ground_height(&moby->pos, 0, 0.5f);
            if (height < hero.ground_z.f)
                height = hero.ground_z.f;
            difference = height - moby->pos.z;
            if (difference < 0.5f && -3.0f < difference)
                FUN_L00_0025b8c0_304e00(&moby->pos.z, &data->climb, height + 0.58f,
                    frame_scale_sq * 0.02f, frame_scale_sq * 0.3f, frame_time * 4.0f);
        }
        if (scale_ticks(30) < data->timer) {
            direction.f[0] = fast_cos_304e00(angle) * 0.3f;
            direction.f[1] = fast_sin_304e00(angle) * 0.3f;
            direction.f[3] = 5627.925f;
            direction.f[2] = 1.2f;
            func_L00_0025A8C0_304310(hit_info, moby, 0x10001, 1.0f, &direction);
            qcopy(&probe, &moby->pos);
            probe.f[2] += 0.4f;
            if (!collect_mobies_in_sphere(&probe, 0x10, moby, hit_info, 0.4f) || hit_moby == NULL ||
                hit_moby->oclass == 0x4F5 || hit_moby == moby->parent) {
                qcopy(&probe, &moby->pos);
                probe.f[2] += 0.35f;
                if (coll_sphere(&probe, 0, moby, 0.35f))
                    moby->state = 2;
            } else {
                moby->state = 2;
            }
        }
        break;
    case HOMING_MINE_STATE_DETONATE:
        spawn_explosion_304e00(moby, D_L12_0015F580, 0, 0.0f, 0.0f, 5, 15, 25,
            2.0f, 1.0f, 4.0f, 1.0f, 2, 7.0f, 0, 9, -1, 0);
        mark_moby_for_removal_304e00(moby);
        return;
    }
    jet_animate(1.0f, moby);
}
