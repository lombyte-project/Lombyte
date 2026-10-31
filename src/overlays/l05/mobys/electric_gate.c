/* Level 05 moby class electric_gate and its beams. */
#include "types.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/rendering/geometry_quad.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/rendering/effect_texture.h"

/* Ported from rac1-decomp (src/overlays/l05_rilgar/vendor_0030EB68.c: func_L05_0030F130), where it is exact; names translated to the US level program. */

extern char  D_0013E550[];
extern char  D_L05_001671C0[];
extern int   D_L05_0015F5CC_e[] __asm__("D_L05_0015F5C4") MACRO_ADDR;
extern int   allocate_voice_for_target_entry() __asm__("FUN_0022da68");
extern short D_L05_0015FFD8_d __asm__("D_L05_0015FFD8");
extern void  draw_electric_gate_beams(struct Moby *moby) __asm__("FUN_L05_0030de90");
void         add_draw_callback(int arg0, int arg1) __asm__("FUN_001f4600");

extern short electric_gate_beam_callback_frame_gp __asm__("D_L05_00161D38") __attribute__((sda));
void         electric_gate_update(char *m) __asm__("FUN_L05_0030dc68");

/* Moby class 838 */
void electric_gate_update(char *gate_moby) {
    enum {
        ELECTRIC_GATE_INITIALIZE = 0,
        ELECTRIC_GATE_WAIT_FOR_TRIGGER = 1,
        ELECTRIC_GATE_TRIGGERED = 2
    };
    char *electric_gate_vars = ((char *)MOBY(gate_moby)->pvars);
    int   state = MOBY(gate_moby)->state;
    char *voice_pool_base;
    char *tab;
    char *gate_position;
    char *voice_position_base;
    char *voice_owner;
    float gate_reference_distance;
    int   voice_slot_index;
    switch (state) {
    case ELECTRIC_GATE_INITIALIZE:
        MOBY(gate_moby)->flags |= 1;
        MOBY(gate_moby)->state = 1;
        MOBY(gate_moby)->visible = 0;
        **(int **)(electric_gate_vars + 4) = -1;
        break;
    case ELECTRIC_GATE_WAIT_FOR_TRIGGER:
        if (*(int *)&electric_gate_beam_callback_frame_gp != D_L05_0015F5CC_e[2]) {
            *(int *)&electric_gate_beam_callback_frame_gp = D_L05_0015F5CC_e[2];
            add_draw_callback(draw_electric_gate_beams, gate_moby);
        }
        gate_position = gate_moby + 0x10;
        tab = D_L05_001671C0;
        if (fast_vec_dist(gate_position, tab) < 64.0f) {
            voice_slot_index = **(int **)(electric_gate_vars + 4);
            if (voice_slot_index >= 0) {
                voice_pool_base = D_0013E550;
                voice_owner = *(char **)(voice_pool_base + voice_slot_index * 0x70 + 0x88);
                if (voice_owner != 0) {
                    if (*(short *)(voice_owner + 0xA6) == 0x346) {
                        if (voice_owner != gate_moby) {
                            if (*(unsigned char *)(voice_owner + 0x20) == state) {
                                gate_reference_distance = fast_vec_dist(gate_position, tab);
                                if (!(gate_reference_distance <
                                      fast_vec_dist(voice_owner + 0x10, tab)))
                                    goto after;
                            }
                            voice_position_base = voice_pool_base + 0x90;
                            *(char **)(voice_pool_base + **(int **)(electric_gate_vars + 4) * 0x70 +
                                       0x88) = gate_moby;
                            qcopy(**(int **)(electric_gate_vars + 4) * 0x70 + voice_position_base,
                                  gate_position);
                        }
                        goto after;
                    }
                }
            }
            **(int **)(electric_gate_vars + 4) = allocate_voice_for_target_entry(0, 4, gate_moby);
        }
    after:
        if (*(int *)electric_gate_vars >= 0) {
            if ((*(unsigned char **)&D_L05_0015FFD8_d)[*(int *)electric_gate_vars * 256 + 0xBC] !=
                0) {
                allocate_voice_for_target_entry(1, 0, gate_moby);
                MOBY(gate_moby)->collision = 0;
                MOBY(gate_moby)->state = 2;
            }
        }
        break;
    case ELECTRIC_GATE_TRIGGERED:
        if (((int)MOBY(gate_moby)->collision) != 0)
            MOBY(gate_moby)->collision = 0;
        break;
    }
}

extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
extern int l05_electric_gate_outer_color_blend_start __asm__("D_L05_00161D3C") __attribute__((sda));
extern int l05_electric_gate_outer_color_blend_end __asm__("D_L05_00161D40") __attribute__((sda));
extern int l05_electric_gate_inner_color_blend_start __asm__("D_L05_00161D44") __attribute__((sda));
extern int l05_electric_gate_inner_color_blend_end __asm__("D_L05_00161D48") __attribute__((sda));
extern int l05_electric_gate_blend_minuend_selector __asm__("D_L05_00161D70") __attribute__((sda));
extern int l05_electric_gate_blend_subtrahend_selector __asm__("D_L05_00161D74")
    __attribute__((sda));
extern int l05_electric_gate_blend_alpha_factor_selector __asm__("D_L05_00161D78")
    __attribute__((sda));
extern int l05_electric_gate_blend_addend_selector __asm__("D_L05_00161D7C") __attribute__((sda));
extern int l05_electric_gate_beam_effect_texture_index __asm__("D_L05_00161D98")
    __attribute__((sda));
extern float  D_L05_0020B740[4][2];
extern Vec4   l05_electric_gate_outer_beam_base_positions[4] __asm__("D_L05_0020B760");
extern Vec4   l05_electric_gate_inner_beam_base_positions[4] __asm__("D_L05_0020B7A0");
extern short *D_L05_001ABCC0[];
extern int    frame_number __asm__("D_L05_0015F5CC");
extern float  fast_add_rotations(float, float);

void draw_electric_gate_beams(struct Moby *moby) {
    struct GeometryQuad quads[2][5];
    char                matrix[0x40];
    u64                 texture;
    int                 i, j, k, vertex_index, outer_color, inner_color, offset;
    float               angle, blend;
    unsigned short     *list;
    unsigned short     *next_moby_index_ptr;
    short               id;

    angle = convert_integer_to_float(frame_number & 0x3F) / 63.0f * 6.28318f;
    for (i = 0; i < 5; i++) {
        blend = (fast_sin(angle) + 1.0f) * 0.5f;
        outer_color = fast_tween_color(l05_electric_gate_outer_color_blend_start,
                                       l05_electric_gate_outer_color_blend_end,
                                       blend);
        inner_color = fast_tween_color(l05_electric_gate_inner_color_blend_start,
                                       l05_electric_gate_inner_color_blend_end,
                                       blend);
        angle = fast_add_rotations(angle, 1.2566371f);
        for (j = 0; j < 2; j++) {
            texture = get_effect_texture(l05_electric_gate_beam_effect_texture_index);
            quads[j][i].primitive = (s64)l05_electric_gate_blend_minuend_selector |
                                    ((s64)l05_electric_gate_blend_subtrahend_selector << 2) |
                                    ((s64)l05_electric_gate_blend_alpha_factor_selector << 4) |
                                    ((s64)l05_electric_gate_blend_addend_selector << 6) |
                                    0x8000000000LL;
            quads[j][i].texture = texture;
            quads[j][i].texture_state = 0xFF9000000260LL;
            quads[j][i].reserved.value = 0;
            for (vertex_index = 0; vertex_index < 4; vertex_index++) {
                quads[j][i].texture_coordinates[vertex_index][0] = D_L05_0020B740[vertex_index][0];
                quads[j][i].texture_coordinates[vertex_index][1] = D_L05_0020B740[vertex_index][1];
                if (j != 0) {
                    quads[j][i].colors[vertex_index] = outer_color;
                    qcopy(&quads[j][i].positions[vertex_index],
                          &l05_electric_gate_outer_beam_base_positions[vertex_index]);
                    quads[j][i].positions[vertex_index].f[2] += (float)(i + 1);
                } else {
                    quads[0][i].colors[vertex_index] = inner_color;
                    qcopy(&quads[0][i].positions[vertex_index],
                          &l05_electric_gate_inner_beam_base_positions[vertex_index]);
                    quads[0][i].positions[vertex_index].f[2] += (float)(i + 1);
                }
            }
        }
    }

    list = (unsigned short *)D_L05_001ABCC0[moby->group];
    do {
        char *listed_moby;
        offset = (*list & 0x7FFF) << 8;
        listed_moby = (char *)(offset + (int)*(char **)&D_L05_0015FFD8_d);
        next_moby_index_ptr = list + 1;
        if (*(short *)(listed_moby + 0xA6) == 0x346 &&
            *(unsigned char *)(listed_moby + 0x20) == 1) {
            copy_matrix_rotation(matrix, listed_moby + 0xC0);
            qcopy(matrix + 0x30, (char *)(offset + (int)*(char **)&D_L05_0015FFD8_d) + 0x10);
            *(float *)(matrix + 0x3C) = 1.0f;
            for (k = 0; k < 5; k++) {
                draw_geometry_quad(&quads[0][k], matrix, 0);
                draw_geometry_quad(&quads[1][k], matrix, 0);
            }
        }
        id = *(short *)list;
        list = next_moby_index_ptr;
    } while (id >= 0);
}

float D_L05_0020B740[4][2] = {
    {0.5f, 0.0f},
    {0.5f, 1.0f},
    {0.5f, 0.0f},
    {0.5f, 1.0f},
};

int l05_electric_gate_beam_effect_texture_index __asm__("D_L05_00161D98") = 0x13;
