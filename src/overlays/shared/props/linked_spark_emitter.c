/* Linked spark emitter (class 0x4E): sprays sparks around itself and counts down, and stops once its linked moby reaches state 2. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/random.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "qzero.h"
#include "rnc/overlay/shared/props/linked_spark_emitter_data/draw_spark_emitter_effect_quads.h"

typedef struct {
    float              v[4][4];
    int                col[4];
    float              uv[4][2];
    long long          r70;
    long long          texture; /* get_effect_texture */
    unsigned long long r80;
    unsigned long long alpha; /* GS ALPHA register value (0x8000000044) */
} EffectQuad;

typedef struct {
    unsigned char pad0[0x74];
    unsigned char active;
    unsigned char pad75[0x13];
    char         *owner;
} Voice;

extern float          fast_sin(float);
extern struct Moby   *l15_moby_array __asm__("D_L15_0015FFD8");
extern int            random_integer_below(int);
extern int            scale_game_frames(int);
extern void           add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
extern char           D_0013F420[];
extern long long      get_effect_texture(int) __asm__("FUN_001f44b8");
extern void           draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern unsigned char  D_0014C190[];
extern int            D_0015ED84;
extern int            draw_spark_emitter_effect_quads() __asm__("FUN_L15_002a3138");
extern void           func_L15_002A47B8_2A3DD0(void *) __asm__("FUN_L15_002a35d8");
extern void           func_L15_002A4318_2A3DD0(void) __asm__("FUN_L15_002a3138");
extern unsigned char *func_L15_002655D0_2A3DD0(
    void *, void *, void *, int, int, int, int, int, void *) __asm__("FUN_L15_002647f0");

extern int fast_tween_color(float, int, int) __asm__("FUN_001fa6e0");

extern int   spark_emitter_pass_colors[] __asm__("D_L15_001CE230");
extern float spark_emitter_uv_phase_divisors_by_pass[][2] __asm__("D_L15_001CE250");
extern int   spark_emitter_uv_cosine_flags_by_pass[][2] __asm__("D_L15_001CE290");
extern float spark_emitter_base_uvs[][2] __asm__("D_L15_001CE2D0");
extern float spark_emitter_second_quad_vertex_template[4][4] __asm__("D_L15_001CE2F0");
extern float spark_emitter_first_quad_vertex_template[4][4] __asm__("D_L15_001CE330");

extern int spark_emitter_alpha_a_by_pass_parity[2] __asm__("D_L15_001614D8") __attribute__((sda));
extern int spark_emitter_alpha_b_by_pass_parity[2] __asm__("D_L15_001614E0") __attribute__((sda));
extern int spark_emitter_alpha_c_by_pass_parity[2] __asm__("D_L15_001614E8") __attribute__((sda));
extern int spark_emitter_alpha_d_by_pass_parity[2] __asm__("D_L15_001614F0") __attribute__((sda));

/* Ported from rac1-decomp (src/overlays/shared/vendor_00298BB8.c: func_L15_002A3DD0), where it is exact; names translated to the US level program. */

/* Level 15 moby update (class 78), three states: 0 picks the first state from the kill and save flags,
 * 1 sprays sixteen random sparks around the moby and parks its data vector, 2 counts down. */

void update_linked_spark_emitter(unsigned char *moby) __asm__("FUN_L15_002a2bf0");

void update_linked_spark_emitter(unsigned char *moby) {
    enum {
        INITIALIZE_EFFECT = 0,
        EMIT_SPARKS = 1,
        WAIT_FOR_EFFECT_TIMEOUT = 2
    };
    unsigned char *moby_data = ((unsigned char *)MOBY(moby)->pvars);
    float          V[4];
    float          W[16];
    float          A[4];
    float          B[4];
    float          C[4];

    switch (moby[0x20]) {
    case INITIALIZE_EFFECT: {
        unsigned short id;
        MOBY(moby)->flags |= 1;
        moby[0x31] = 0;
        id = MOBY(moby)->save_id;
        if (l15_save_state.cleared[(short)id] == 0 &&
            (((*(int *)(D_0014C190 + (((short)id >> 5) * 4) + (D_0015ED84 << 8)) >> (id & 0x1F)) ^
              1) &
             1)) {
            moby[0x20] = 1;
        } else {
            moby[0xBC] = 0xFF;
            MOBY(moby)->collision = 0;
            moby[0x20] = 3;
        }
        break;
    }
    case EMIT_SPARKS: {
        int linked_moby_index = *(int *)(moby_data + 0x10);
        if ((linked_moby_index != -1 && l15_moby_array[linked_moby_index].state == 2) ||
            visited52[(unsigned char)moby[0xB0] + (D_0015ED84 << 4)] == 0xFF) {
            func_L15_002A47B8_2A3DD0(moby);
        } else {
            Voice *e = (Voice *)((unsigned char *)&voice_pool + moby[0xBC] * 0x70);
            float  lo, hi, z, one;
            int    i;
            if (e->owner != (char *)moby || e->active == 0) {
                moby[0xBC] = allocate_voice_for_target_entry(0, 4, moby);
            }
            fast_vec_sub(V, D_0013F420, moby + 0x10);
            transpose_rotation_matrix(W, moby + 0xC0);
            transform_vector(V, V, W);
            V[0] = 0.0f;
            z = V[0];
            i = 15;
            do {
                int col, fa, fb, fc;
                lo = -5.0f;
                hi = 5.0f;
                one = 1.0f;
                i--;
                A[0] = random_float_between(-0.25f, 0.25f);
                A[1] = random_float_between(lo, hi);
                A[2] = random_float_between(z, hi);
                B[0] = z;
                B[1] = random_float_between(lo, hi);
                B[2] = random_float_between(lo, hi);
                C[0] = z;
                C[1] = random_float_between(lo, hi);
                C[2] = random_float_between(lo, hi);
                transform_vector_by_basis(A, A, moby + 0xC0);
                transform_vector_by_basis(B, B, moby + 0xC0);
                transform_vector_by_basis(C, C, moby + 0xC0);
                fast_vec_add(A, A, moby + 0x10);
                fast_vec_normalize(B, B, one / (float)scale_game_frames(30));
                fast_vec_normalize(C, C, one / (float)scale_game_frames(30));
                B[3] = 0.125f;
                {
                    int ra = random_integer_below(0x10);
                    int rb = random_integer_below(0x20);
                    int rc = random_integer_below(0x20);
                    int partial_spark_color = ((rb + 0x60) << 8) | 0x60000000;
                    col = ((ra + 0x20) << 16) | partial_spark_color;
                    col |= rc + 0x60;
                }
                fa = truncate_float_to_s32((float)(scale_game_frames(30) / 4));
                fb = truncate_float_to_s32((float)(scale_game_frames(30) / 2));
                fc = truncate_float_to_s32((float)(scale_game_frames(30) / 4));
                func_L15_002655D0_2A3DD0(A, B, C, col, 0x60808080, fa, fb, fc, moby);
            } while (i >= 0);
            if (absolute_float(V[1]) < hi && z < V[2] && V[2] < hi) {
                transform_vector_by_basis(moby_data, V, moby + 0xC0);
                fast_vec_add(moby_data, moby_data, moby + 0x10);
            } else {
                qzero(moby_data);
            }
            add_draw_callback(func_L15_002A4318_2A3DD0, moby);
        }
        break;
    }
    case WAIT_FOR_EFFECT_TIMEOUT:
        if ((*(int *)(moby_data + 0x14))++ >= 0x3D) {
            moby[0x20] = 3;
        } else {
            qzero(moby_data);
            add_draw_callback(func_L15_002A4318_2A3DD0, moby);
        }
        break;
    }
}

int draw_spark_emitter_effect_quads(unsigned char *moby) {
    EffectQuad first, second;
    float      transform[12];
    float      position[4];
    int       *data = ((int *)MOBY(moby)->pvars);
    int        pass, corner, axis, component;
    float     *transformPointer;
    float     *firstUv1, *firstUv3, *secondUv0, *secondUv1;
    int        nextPass;
    int        passOffset;
    float     *secondUv2, *secondUv3;
    float(*secondVertices)[4];
    float(*firstUv)[2];

    copy_matrix_rotation(transform, moby + 0xc0);
    qcopy(position, moby + 0x10);
    position[3] = 1.0f;
    first.texture = get_effect_texture(0x2c);
    first.r80 = 0xff9000000260ULL;
    first.r70 = 0;
    second.texture = get_effect_texture(0x2c);
    secondVertices = second.v;
    firstUv = first.uv;
    secondUv0 = second.uv[0];
    firstUv1 = first.uv[1];
    secondUv1 = second.uv[1];
    secondUv2 = second.uv[2];
    firstUv3 = first.uv[3];
    secondUv3 = second.uv[3];
    second.r70 = 0;
    second.r80 = 0xff9000000260ULL;
    second.col[3] = 0;
    second.col[1] = 0;
    for (corner = 0; corner < 4; corner++) {
        qcopy(first.v[corner], spark_emitter_first_quad_vertex_template[corner]);
        qcopy(secondVertices[corner], spark_emitter_second_quad_vertex_template[corner]);
    }
    for (pass = 0; pass < 6; pass = nextPass) {
        int parity = pass & 1;
        first.alpha = (unsigned long long)spark_emitter_alpha_a_by_pass_parity[parity] |
                      ((unsigned long long)spark_emitter_alpha_b_by_pass_parity[parity] << 2) |
                      ((unsigned long long)spark_emitter_alpha_c_by_pass_parity[parity] << 4) |
                      ((unsigned long long)spark_emitter_alpha_d_by_pass_parity[parity] << 6) |
                      0x8000000000ULL;
        second.alpha = (unsigned long long)spark_emitter_alpha_a_by_pass_parity[parity] |
                       ((unsigned long long)spark_emitter_alpha_b_by_pass_parity[parity] << 2) |
                       ((unsigned long long)spark_emitter_alpha_c_by_pass_parity[parity] << 4) |
                       ((unsigned long long)spark_emitter_alpha_d_by_pass_parity[parity] << 6) |
                       0x8000000000ULL;
        if (data[5] != 0) {
            second.col[2] =
                fast_tween_color((float)data[5] / 60.0f, spark_emitter_pass_colors[pass], 0);
            second.col[0] = second.col[2];
            first.col[3] = second.col[2];
            first.col[2] = second.col[2];
            first.col[1] = second.col[2];
            first.col[0] = second.col[2];
        } else {
            second.col[2] = spark_emitter_pass_colors[pass];
            second.col[0] = second.col[2];
            first.col[3] = second.col[2];
            first.col[2] = second.col[2];
            first.col[1] = second.col[2];
            first.col[0] = second.col[2];
        }
        corner = 0;
        passOffset = pass * 8;
        nextPass = pass + 1;
        for (; corner < 4; corner++) {
            float *uv = (float *)(corner * 8 + (unsigned int)firstUv);
            float *frequency =
                (float *)((char *)spark_emitter_uv_phase_divisors_by_pass + passOffset);
            int waveform = passOffset;
            int base = corner * 8;
            axis = 1;
            do {
                int   limit, value;
                float angle, delta;
                float step;
                float radians;
                axis--;
                limit = truncate_float_to_s32(*frequency * 360.0f);
                angle =
                    convert_integer_to_float(spark_emitter_uv_phase_input % limit) / *frequency -
                    180.0f;
                frequency++;
                value = *(int *)((char *)spark_emitter_uv_cosine_flags_by_pass + waveform);
                waveform += 4;
                radians = 0.017453292f;
                step = 0.125f;
                if (value != 0) {
                    angle = fast_cos(angle * radians);
                } else {
                    angle = fast_sin(angle * radians);
                }
                delta = angle * step;
                *uv++ = *(float *)((char *)spark_emitter_base_uvs + base) + delta;
                base += 4;
            } while (axis >= 0);
        }
        for (component = 0; component < 2; component++) {
            float step = (float)component * 0.125f;
            secondUv0[component] = firstUv1[component];
            secondUv1[component] = firstUv1[component] + step;
            secondUv2[component] = firstUv3[component];
            secondUv3[component] = firstUv3[component] + step;
        }
        transformPointer = transform;
        draw_geometry_quad(&first, transformPointer, 0);
        draw_geometry_quad(secondVertices, transformPointer, 0);
        for (corner = 0; corner < 4; corner++) {
            first.v[corner][0] += 0.1f;
            secondVertices[corner][0] += 0.1f;
        }
    }
}

float D_L17_001CE570[4][4] = {
    {-0.25f, -5.0f, 5.0f, 1.0f},
    {-0.25f, -5.0f, 6.0f, 1.0f},
    {-0.25f,  5.0f, 5.0f, 1.0f},
    {-0.25f,  5.0f, 6.0f, 1.0f},
};

float D_L17_001CE5B0[4][4] = {
    {-0.25f, -5.0f, 0.0f, 1.0f},
    {-0.25f, -5.0f, 5.0f, 1.0f},
    {-0.25f,  5.0f, 0.0f, 1.0f},
    {-0.25f,  5.0f, 5.0f, 1.0f},
};

int D_L17_00161478[2] = {0, 1};

int D_L17_00161480[2] = {2, 0};

int D_L17_00161488[2] = {2, 2};

int D_L17_00161490[2] = {1, 2};

float spark_emitter_second_quad_vertex_template[4][4] __asm__("D_L15_001CE2F0") = {
    {-0.25f, -5.0f, 5.0f, 1.0f},
    {-0.25f, -5.0f, 6.0f, 1.0f},
    {-0.25f,  5.0f, 5.0f, 1.0f},
    {-0.25f,  5.0f, 6.0f, 1.0f},
};

float spark_emitter_first_quad_vertex_template[4][4] __asm__("D_L15_001CE330") = {
    {-0.25f, -5.0f, 0.0f, 1.0f},
    {-0.25f, -5.0f, 5.0f, 1.0f},
    {-0.25f,  5.0f, 0.0f, 1.0f},
    {-0.25f,  5.0f, 5.0f, 1.0f},
};
