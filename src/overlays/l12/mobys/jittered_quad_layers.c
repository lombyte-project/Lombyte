/* Level 12 moby class jittered_quad_layers (class 1345): draws jittered quad layers until its linked moby reaches state 7. */
#include "rnc/hero/hero.h"
#include "rnc/audio/voice_pool.h"
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"

typedef struct Moby Moby;

#include "rnc/game/game_globals.h"

extern int check_scaled_vector_bsphere(void *, float) __asm__("FUN_L01_00275690");
extern int FUN_0022da68(int, int, void *);
void       draw_hoven_jittered_quad_layers(struct Moby *moby) __asm__("FUN_L12_00308350");

extern void draw_hoven_jittered_quad_layers(struct Moby *) __asm__("FUN_L12_00308350");

/* Pvars: d[0] is the moby to watch, d[1] the voice slot held for this moby. */
void update_jittered_quad_layers(struct Moby *moby) __asm__("FUN_L12_003081b0");

void update_jittered_quad_layers(struct Moby *moby) {
    enum {
        CHECK_PERSISTENT_REMOVAL = 0,
        QUEUE_DRAW_AND_MONITOR_LINKED_MOBY = 1
    };
    int  moby_state_snapshot = moby->state;
    int  vis;
    int *moby_data = (int *)moby->pvars;

    switch (moby_state_snapshot) {
    case CHECK_PERSISTENT_REMOVAL:
        if (moby->mission != 0xFF && visited52[moby->mission + current_level_index * 16] == 0xFF) {
            mark_moby_for_removal(moby);
            break;
        }
        moby->state = 1;
        break;
    case QUEUE_DRAW_AND_MONITOR_LINKED_MOBY:
        vis = 1;
        if (check_scaled_vector_bsphere(moby, 48.0f) == -1) {
            vis = 0;
        }
        if (vis) {
            add_draw_callback(draw_hoven_jittered_quad_layers, moby);
        }
        if (moby_data[1] == -1) {
            moby_data[1] = FUN_0022da68(0, 4, moby);
        }
        if (moby_data[0] >= 0) {
            int i = moby_data[1];
            if (i != -1) {
                char *voice_pool_entry = ((char *)&voice_pool) + i * 0x70;
                if (*(struct Moby **)(voice_pool_entry + 0x88) == moby &&
                    *(unsigned char *)(voice_pool_entry + 0x74) != 0) {
                    release_voice_slot(i);
                }
            }
            moby_data[1] = -1;
            if (((unsigned char *)D_L12_0015FFD8_p)[(moby_data[0] << 8) + 0x20] == 7) {
                if (moby->mission != 0xFF) {
                    jet_clear_persistent(moby->mission);
                }
                FUN_0022da68(1, 0, moby);
                moby->collision = 0;
                moby->state = 2;
            }
        }
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_00309730.c: func_L12_00309730). */

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) VendorQuadCorner;

typedef struct {
    float        corner[4][4];
    unsigned int color[4];
    float        uv[4][2];
    long         tag;
    long         texture;
    long         texture_state;
    long         gs_state;
} VendorDrawPacket;

#include "rnc/overlay/l12/mobys/jittered_quad_layers_globals.h"

extern int hoven_jittered_first_packet_vertex_color_gp __asm__("D_L12_00161F90")
    __attribute__((sda));
extern int hoven_jittered_second_packet_vertex_color_gp __asm__("D_L12_00161F94")
    __attribute__((sda));
extern int hoven_jittered_first_packet_blend_minuend_selector_gp __asm__("D_L12_00161FBC")
    __attribute__((sda));
extern int hoven_jittered_first_packet_blend_subtrahend_selector_gp __asm__("D_L12_00161FC0")
    __attribute__((sda));
extern int hoven_jittered_first_packet_blend_alpha_factor_selector_gp __asm__("D_L12_00161FC4")
    __attribute__((sda));
extern int hoven_jittered_first_packet_blend_addend_selector_gp __asm__("D_L12_00161FC8")
    __attribute__((sda));
extern int hoven_jittered_second_packet_blend_minuend_selector_gp __asm__("D_L12_00161FCC")
    __attribute__((sda));
extern int hoven_jittered_second_packet_blend_subtrahend_selector_gp __asm__("D_L12_00161FD0")
    __attribute__((sda));
extern int hoven_jittered_second_packet_blend_alpha_factor_selector_gp __asm__("D_L12_00161FD4")
    __attribute__((sda));
extern int hoven_jittered_second_packet_blend_addend_selector_gp __asm__("D_L12_00161FD8")
    __attribute__((sda));
extern float hoven_jittered_quad_base_uvs[4][2] __asm__("D_L12_001FB6D0");

VendorQuadCorner D_L12_001FB6F0[4] = {
    {0.0f, -5.0f, -0.02f, 1.0f},
    {0.0f, -5.0f,  0.02f, 1.0f},
    {0.0f,  5.0f, -0.02f, 1.0f},
    {0.0f,  5.0f,  0.02f, 1.0f},
};

VendorQuadCorner D_L12_001FB730[4] = {
    {0.0f, -5.0f, -0.2f, 1.0f},
    {0.0f, -5.0f,  0.2f, 1.0f},
    {0.0f,  5.0f, -0.2f, 1.0f},
    {0.0f,  5.0f,  0.2f, 1.0f},
};

extern long get_effect_texture(int) __asm__("FUN_001f44b8");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");

void draw_hoven_jittered_quad_layers(struct Moby *moby) {
    VendorDrawPacket packets[2] __attribute__((aligned(16)));
    float            matrix[16] __attribute__((aligned(16)));
    long             texture;
    int              first_color, second_color;
    int              pass, row, vertex, corner;
    float            jitter;

    copy_matrix_rotation(matrix, &moby->rot_row0);
    qcopy(&matrix[12], &moby->pos);
    matrix[15] = 1.0f;
    texture = get_effect_texture(0xE);
    packets[0].texture = packets[1].texture = texture;
    first_color = hoven_jittered_first_packet_vertex_color_gp;
    second_color = hoven_jittered_second_packet_vertex_color_gp;
    packets[0].texture_state = packets[1].texture_state = 0xFF9000000260L;
    packets[0].gs_state = (long)hoven_jittered_first_packet_blend_minuend_selector_gp |
                          ((long)hoven_jittered_first_packet_blend_subtrahend_selector_gp << 2) |
                          ((long)hoven_jittered_first_packet_blend_alpha_factor_selector_gp << 4) |
                          ((long)hoven_jittered_first_packet_blend_addend_selector_gp << 6) |
                          (0x8000L << 24);
    packets[1].gs_state = (long)hoven_jittered_second_packet_blend_minuend_selector_gp |
                          ((long)hoven_jittered_second_packet_blend_subtrahend_selector_gp << 2) |
                          ((long)hoven_jittered_second_packet_blend_alpha_factor_selector_gp << 4) |
                          ((long)hoven_jittered_second_packet_blend_addend_selector_gp << 6) |
                          (0x8000L << 24);
    packets[0].color[0] = packets[0].color[1] = packets[0].color[2] = packets[0].color[3] =
        first_color;
    packets[0].tag = packets[1].tag = 0;
    packets[1].color[0] = packets[1].color[1] = packets[1].color[2] = packets[1].color[3] =
        second_color;
    for (vertex = 0; vertex < 4; vertex++) {
        float u, v;
        qcopy(packets[1].corner[vertex], &D_L12_001FB6F0[vertex]);
        qcopy(packets[0].corner[vertex], &D_L12_001FB730[vertex]);
        u = hoven_jittered_quad_base_uvs[vertex][0];
        v = hoven_jittered_quad_base_uvs[vertex][1];
        packets[0].uv[vertex][0] = u;
        packets[0].uv[vertex][1] = v;
        packets[1].uv[vertex][0] = u;
        packets[1].uv[vertex][1] = v;
    }
    for (pass = 0; pass < 5; pass++) {
        float u_offset = random_float_between(-0.5f, 0.5f);
        for (corner = 0; corner < 4; corner++) {
            float(*first)[4] = packets[0].corner;
            float(*second)[4] = packets[1].corner;
            first[corner][2] = first[corner][2] + 1.0f;
            second[corner][2] = second[corner][2] + 1.0f;
            packets[0].uv[corner][0] = hoven_jittered_quad_base_uvs[corner][0] + u_offset;
            packets[0].uv[corner][1] = hoven_jittered_quad_base_uvs[corner][1];
        }
        jitter = 0.0f;
        row = -3;
        do {
            int   next_row = row + 1;
            int   jittered = row < 4;
            float y = (float)row + jitter;
            jitter = 0.0f;
            packets[0].corner[0][1] = y;
            packets[0].corner[1][1] = y;
            if (jittered)
                jitter = random_float_between(-0.2f, 0.2f);
            y = (float)next_row + jitter;
            packets[0].corner[2][1] = y;
            packets[0].corner[3][1] = y;
            row = next_row;
            draw_geometry_quad(&packets[0], matrix, 0);
        } while (row < 3);
    }
}

float hoven_jittered_quad_base_uvs[4][2] __asm__("D_L12_001FB6D0") = {
    {0.0f, 0.0f},
    {0.0f, 1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
};
