/* Shared moby class: grindrail_barrier. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/props/grindrail_barrier_data/draw_grindrail_barrier_effect.h"

typedef struct {
    float         position[4][4];
    int           color[4];
    float         uv[4][2];
    unsigned long flags, texture, giftag, primitive;
} RingQuad;

typedef struct {
    char  pad00[8];
    float yaw, phase;
    float cardinal_angles[4];
} RingData;

typedef struct {
    char          pad[0xBC];
    unsigned char act;
    char          pad2[0x43];
} M_5cd0;

extern char *D_L16_0015FFD8;
extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");

int check_grindrail_barrier_release_condition(int moby_list_index) __asm__("FUN_L16_002e5cd0");

int         check_grindrail_barrier_release_condition(int idx) __asm__("FUN_L16_002e5cd0");
extern void FUN_L16_002e58d8_u(void) __asm__("FUN_L16_002e58d8");
extern void group_trigger_nearest(int) __asm__("FUN_L16_002e5d30");

extern short grindrail_barrier_ring_rotation_speed __asm__("D_L16_00161E00") __attribute__((sda));
extern short grindrail_barrier_ring_phase_speed __asm__("D_L16_00161E04") __attribute__((sda));
extern short grindrail_barrier_accent_rotation_speeds __asm__("D_L16_00161E08")
    __attribute__((sda));

extern float grindrail_barrier_ring_uvs[4][2] __asm__("D_L16_001D9780");
extern float grindrail_barrier_ring_vertex_positions[4][4] __asm__("D_L16_001D97C0");
extern float grindrail_barrier_accent_uvs[4][2] __asm__("D_L16_001D97A0");
extern float grindrail_barrier_accent_vertex_positions[4][4] __asm__("D_L16_001D9800");

extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern s64  get_effect_texture(s32) __asm__("func_001F44B8");
extern void draw_geometry_quad(void *, int, int) __asm__("func_001F7D30");

extern short *D_L16_001ABCC0_5cd0[] __asm__("D_L16_001ABCC0");
extern short *D_L16_001ABCC0[];

extern float D_L16_00161D78;
void         group_trigger_nearest(int moby_list_index) __asm__("FUN_L16_002e5d30");

/* Updates a moby effect and advances it when its linked object is ready. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E6B70), where it is exact; names translated to the US level program. */

void grindrail_barrier_update(struct Moby *m) __asm__("FUN_L16_002e5708");

void grindrail_barrier_update(struct Moby *barrier_moby) {
    enum {
        GRINDRAIL_BARRIER_STATE_INITIALIZE = 0,
        GRINDRAIL_BARRIER_STATE_COLLISION_ENABLED = 1,
        GRINDRAIL_BARRIER_STATE_COLLISION_DISABLED = 2
    };
    char  *barrier_data = (char *)barrier_moby->pvars;
    float  cycle;
    int    i;
    float *cardinal_angles;
    switch (barrier_moby->state) {
    case GRINDRAIL_BARRIER_STATE_INITIALIZE:
        barrier_moby->state = 1;
        barrier_moby->update_dist = 0xFF;
        break;
    case GRINDRAIL_BARRIER_STATE_COLLISION_ENABLED:
        cycle = *(float *)(barrier_data + 0xC) +
                *(float *)&grindrail_barrier_ring_phase_speed * frame_time;
        *(float *)(barrier_data + 0xC) = cycle;
        if (cycle > 1.0f)
            *(float *)(barrier_data + 0xC) = cycle - 1.0f;
        if (voice_is_playing(barrier_moby, *(int *)(barrier_data + 0x24)) == 0) {
            *(int *)(barrier_data + 0x24) = allocate_voice_for_target_entry(0, 4, barrier_moby);
        }
        *(float *)(barrier_data + 8) = fast_add_rotations(
            *(float *)(barrier_data + 8),
            *(float *)&grindrail_barrier_ring_rotation_speed * DEG_TO_RAD * frame_time);
        cardinal_angles = (float *)(barrier_data + 0x10);
        for (i = 0; i < 4; i++) {
            cardinal_angles[i] = fast_add_rotations(
                cardinal_angles[i],
                ((float *)&grindrail_barrier_accent_rotation_speeds)[i] * DEG_TO_RAD * frame_time);
        }
        if (barrier_moby->visible)
            add_draw_callback(FUN_L16_002e58d8_u, barrier_moby);
        if (*(int *)(barrier_data + 0x20) != -1 &&
            check_grindrail_barrier_release_condition(*(int *)(barrier_data + 0x20))) {
            barrier_moby->collision = 0;
            barrier_moby->state = 2;
            group_trigger_nearest(*(int *)(barrier_data + 0x20));
        }
        break;
    case GRINDRAIL_BARRIER_STATE_COLLISION_DISABLED:
        if (!check_grindrail_barrier_release_condition(*(int *)(barrier_data + 0x20))) {
            barrier_moby->state = 1;
            barrier_moby->collision = barrier_moby->pclass->collision;
        }
        break;
    }
}

/* Draw the animated ring quads and four rotating accents around a moby. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E6D40), where it is exact; names translated to the US level program. */

void draw_grindrail_barrier_effect(char *barrier_moby) __asm__("FUN_L16_002e58d8");

void draw_grindrail_barrier_effect(char *barrier_moby) {
    RingQuad  quad;
    float     transform[4][4], rotation[4];
    RingData *data = ((RingData *)MOBY(barrier_moby)->pvars);
    float     scale =
        MOBY(barrier_moby)->scale / *(float *)(((char *)MOBY(barrier_moby)->pclass) + 0x24);
    float *u, *v;
    float *angles;
    int   *color_base;
    float  phase, angle_step, phase_step;
    int    i;
    qcopy(rotation, barrier_moby + 0x40);
    qcopy(transform[3], barrier_moby + 0x10);
    quad.texture = get_effect_texture(*(int *)&grindrail_barrier_ring_texture_id);
    u = &quad.uv[0][0];
    v = &quad.uv[0][1];
    quad.giftag = 0xFF9000000260UL;
    quad.primitive = *(int *)&grindrail_barrier_blend_minuend_selector;
    quad.primitive |= (unsigned long)*(int *)&grindrail_barrier_blend_subtrahend_selector << 2;
    quad.primitive |= (unsigned long)*(int *)&grindrail_barrier_blend_multiplier_selector << 4;
    quad.primitive |= (unsigned long)*(int *)&grindrail_barrier_blend_addend_selector << 6;
    quad.primitive |= 0x8000000000UL;
    quad.flags = 0;
    color_base = quad.color;
    {
        int k;
        for (k = 0; k < 4; ++k) {
            u[k * 2] = grindrail_barrier_ring_uvs[k][0];
            v[k * 2] = grindrail_barrier_ring_uvs[k][1];
            color_base[k] = *(int *)&grindrail_barrier_ring_base_color;
            qcopy(quad.position[k], grindrail_barrier_ring_vertex_positions[k]);
            fast_vec_scale(quad.position[k], quad.position[k], scale);
        }
    }
    angles = data->cardinal_angles;
    angle_step = 6.2831855f / convert_integer_to_float(*(int *)&grindrail_barrier_ring_quad_count);
    phase_step = 3.0f / convert_integer_to_float(*(int *)&grindrail_barrier_ring_quad_count);
    phase = data->phase;
    while (phase < 0.0f)
        phase += 1.0f;
    while (phase > 1.0f)
        phase -= 1.0f;
    for (i = 0; i < *(int *)&grindrail_barrier_ring_quad_count;) {
        float blend;
        int   color, j;
        rotation[0] = fast_add_rotations(data->yaw, (float)i * angle_step);
        euler_rotation_basis(transform, rotation);
        if (phase > 0.5f)
            blend = (-phase + 1.0f) + (-phase + 1.0f);
        else
            blend = phase + phase;
        ++i;
        color = fast_tween_color(*(int *)&grindrail_barrier_ring_blend_start_color,
                                 *(int *)&grindrail_barrier_ring_base_color,
                                 blend);
        for (j = 1; j >= 0; --j)
            color_base[j] = color;
        phase += phase_step;
        if (phase > 1.0f)
            phase -= 1.0f;
        draw_geometry_quad(&quad, transform, 0);
    }
    quad.texture = get_effect_texture(*(int *)&grindrail_barrier_accent_texture_id);
    {
        int k;
        int solid_color = *(int *)&grindrail_barrier_accent_color;
        for (k = 0; k < 4; ++k) {
            u[k * 2] = grindrail_barrier_accent_uvs[k][0];
            v[k * 2] = grindrail_barrier_accent_uvs[k][1];
            color_base[k] = solid_color;
            qcopy(quad.position[k], grindrail_barrier_accent_vertex_positions[k]);
        }
    }
    {
        int count;
        for (count = 3; count >= 0; --count) {
            rotation[0] = angles[3 - count];
            euler_rotation_basis(transform, rotation);
            draw_geometry_quad(&quad, transform, 0);
        }
    }
}

/* Returns 1 when every moby in the list is active, 0 if any is not or the list is empty. */

int check_grindrail_barrier_release_condition(int moby_list_index) {
    unsigned short *moby_index_entry = (unsigned short *)D_L16_001ABCC0_5cd0[moby_list_index];
    if (moby_index_entry == 0) {
        return 0;
    }
    do {
        if (((M_5cd0 *)D_L16_0015FFD8)[*moby_index_entry & 0x7FFF].act == 0) {
            return 0;
        }
    } while ((short)*moby_index_entry++ >= 0);
    return 1;
}

/* D_L16_0015FFD8 is declared sda above (FUN_L16_002e2b60 loads it off $gp), but
   here retail loads it absolute: reach it from a neighbour, past whose size the
   assembler no longer uses $gp. */
#define D_L16_0015FFD8 (((char **)&D_L16_00161D78)[-(0x161D78 - 0x15FFD8) / 4])

/* picks the nearest moby in a list (skipping class 0x5A3) and acts on it */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002A1B58.c: func_L16_002E7198), where it is exact; names translated to the US level program. */

void group_trigger_nearest(int moby_list_index) {
    short *moby_list_entry = D_L16_001ABCC0[moby_list_index];
    char  *best = 0;
    float  bestd = 50000.0f;
    if (moby_list_entry != 0) {
        do {
            char *candidate_moby =
                D_L16_0015FFD8 + ((*(unsigned short *)moby_list_entry & 0x7FFF) << 8);
            if (MOBY(candidate_moby)->oclass != 0x5A3) {
                float candidate_distance = fast_vec_dist(D_0013E533 + 0xE9D, candidate_moby + 0x10);
                if (candidate_distance < bestd) {
                    bestd = candidate_distance;
                    best = candidate_moby;
                }
            }
        } while (*moby_list_entry++ >= 0);
        if (best != 0) {
            allocate_voice_for_target_entry(0, 0, (int)best);
        }
    }
}

short D_L18_00162588 = 0;
short D_L18_0016258C = 0;
short D_L18_00162590 = 0;

float D_L18_001EFC10[4][2] = {
    {0.5f, 0.5f},
    {0.5f, 1.0f},
    {0.5f, 0.5f},
    {0.5f, 1.0f},
};

float D_L18_001EFC50[4][4] = {
    {0.0f, -4.0f, 0.9f, 1.0f},
    {0.0f, -4.0f, 0.4f, 1.0f},
    {0.0f,  4.0f, 0.9f, 1.0f},
    {0.0f,  4.0f, 0.4f, 1.0f},
};

float D_L18_001EFC30[4][2] = {
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
};

float D_L18_001EFC90[4][4] = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};

float grindrail_barrier_ring_uvs[4][2] __asm__("D_L16_001D9780") = {
    {0.5f, 0.5f},
    {0.5f, 1.0f},
    {0.5f, 0.5f},
    {0.5f, 1.0f},
};

float grindrail_barrier_ring_vertex_positions[4][4] __asm__("D_L16_001D97C0") = {
    {0.0f, -4.0f, 0.9f, 1.0f},
    {0.0f, -4.0f, 0.4f, 1.0f},
    {0.0f,  4.0f, 0.9f, 1.0f},
    {0.0f,  4.0f, 0.4f, 1.0f},
};

float grindrail_barrier_accent_uvs[4][2] __asm__("D_L16_001D97A0") = {
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 0.0f},
    {0.0f, 1.0f},
};

float grindrail_barrier_accent_vertex_positions[4][4] __asm__("D_L16_001D9800") = {
    {0.0f, -1.0f,  1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f, 1.0f},
    {0.0f,  1.0f,  1.0f, 1.0f},
    {0.0f,  1.0f, -1.0f, 1.0f},
};
