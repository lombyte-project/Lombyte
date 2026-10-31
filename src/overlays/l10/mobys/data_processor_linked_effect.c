/* Level 10 moby class data_processor_linked_effect: the effect linked to the data processor and its drawing. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

#include "rnc/overlay/l10/mobys/data_processor_linked_effect_globals.h"
#include "rnc/overlay/quad.h"
#include "rnc/audio/voice_pool.h"

extern int   debug_print() __asm__("FUN_001e93b0");
extern float scale_time(float) __asm__("FUN_001f96b0");

extern struct Moby *D_L10_0015FFD8;
extern void        *memcpy(void *, const void *, u32);
extern OvlQuad      D_L10_00167240;

char l10_data_processor_linked_effect_missing_processor_link_message[63] __asm__("D_L10_001DD1D0") =

    "moby class 1067 REQUIRES a link to dataProcessor class 1073!!\n";

char l10_data_processor_linked_effect_deleted_instance_message[30] __asm__("D_L10_001DD210") =
    "\tMoby Maya Index %d deleted.\n";

extern unsigned char
    l10_data_processor_linked_effect_initial_strip_order[10] __asm__("D_L10_00161CA8");
extern unsigned char D_001413F4[];
void                 render_data_processor_linked_effect(u8 *moby) __asm__("FUN_L10_002da690");

extern float data_processor_linked_effect_fade_duration_seconds_gp __asm__("D_L10_00161C98")
    __attribute__((sda));
void data_processor_linked_effect_update(struct Moby *moby) __asm__("FUN_L10_002da2c8");

/* Moby class 1067 */
void data_processor_linked_effect_update(struct Moby *moby) {
    enum {
        MOBY_STATE_INITIALIZE = 0,
        MOBY_STATE_ACTIVE_WITH_SOUND = 1,
        MOBY_STATE_ACTIVE_WITHOUT_SOUND = 2
    };
    char *linked_effect_vars = (char *)moby->pvars;
    union {
        unsigned char order[10];
        float         position2[4];
    } scratch;
    float position1[4];
    int   pass, i, copy_index;

    if (*(int *)(linked_effect_vars + 0xC) == -1) {
        debug_print(l10_data_processor_linked_effect_missing_processor_link_message);
        debug_print(l10_data_processor_linked_effect_deleted_instance_message,
                    *(short *)&moby->save_id);
        mark_moby_for_removal(moby);
    }
    switch (moby->state) {
    case MOBY_STATE_INITIALIZE:
        if (current_level_index == 10 && D_001413F4[0] == 1) {
            mark_moby_for_removal(moby);
            break;
        }
        memcpy(scratch.order, l10_data_processor_linked_effect_initial_strip_order, 10);
        for (pass = 0; pass < 10; pass++) {
            for (i = 0; i < random_integer_below(9); i++) {
                unsigned char saved_next_order_byte = scratch.order[i + 1];
                scratch.order[i + 1] = scratch.order[i];
                scratch.order[i] = saved_next_order_byte;
            }
        }
        copy_index = 0;
        if (*(int *)(linked_effect_vars + 4) > 0) {
            char *dest = linked_effect_vars + 0x10;
            do {
                dest[copy_index] = scratch.order[copy_index];
                copy_index++;
            } while (copy_index < *(int *)(linked_effect_vars + 4));
        }
        {
            float timer;
            float new_rotation = fast_add_rotations(moby->rot.z, 1.5707964f);
            moby->update_dist = 0x60;
            moby->state = 1;
            timer = data_processor_linked_effect_fade_duration_seconds_gp * 60.0f;
            moby->rot.z = new_rotation;
            *(int *)(linked_effect_vars + 0x30) = -1;
            *(int *)(linked_effect_vars + 0x34) = scale_game_frames(truncate_float_to_s32(timer));
        }

        moby->flags |= 0x41;
        break;
    case MOBY_STATE_ACTIVE_WITH_SOUND: {
        float half_height, half_width, radius_squared;
        half_height =
            (float)*(int *)(linked_effect_vars + 4) * *(float *)(linked_effect_vars + 8) * 0.5f;
        half_width = *(float *)linked_effect_vars * 0.5f;
        *(OvlQuad *)position1 = *(OvlQuad *)&moby->pos;
        radius_squared = half_height * half_height;
        position1[2] += half_height;
        position1[3] = square_root(radius_squared + half_width * half_width);
        if (fast_vec_dist(&moby->pos, &D_L10_00167240) < 80.0f &&
            0.0f <= (float)fast_bsphere_check(position1, 80.0f))
            add_draw_callback((void *)render_data_processor_linked_effect, moby);
        if (voice_is_playing(moby, *(int *)(linked_effect_vars + 0x30)) == 0)
            *(int *)(linked_effect_vars + 0x30) = allocate_voice_for_target_entry(0, 4, (int)moby);
        break;
    }
    case MOBY_STATE_ACTIVE_WITHOUT_SOUND: {
        int   voice;
        float half_height, half_width, radius_squared;
        fast_dec_timer(linked_effect_vars + 0x34);
        voice = *(int *)(linked_effect_vars + 0x30);
        if (voice != -1) {
            unsigned char *voice_slot_entry = (unsigned char *)&voice_pool + voice * 0x70;
            if (*(struct Moby **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74] != 0)
                release_voice_slot(voice);
        }
        *(int *)(linked_effect_vars + 0x30) = -1;
        half_height =
            (float)*(int *)(linked_effect_vars + 4) * *(float *)(linked_effect_vars + 8) * 0.5f;
        half_width = *(float *)linked_effect_vars * 0.5f;
        *(OvlQuad *)scratch.position2 = *(OvlQuad *)&moby->pos;
        radius_squared = half_height * half_height;
        scratch.position2[2] += half_height;
        scratch.position2[3] = square_root(radius_squared + half_width * half_width);
        if (fast_vec_dist(&moby->pos, &D_L10_00167240) < 80.0f &&
            0.0f <= (float)fast_bsphere_check(scratch.position2, 80.0f))
            add_draw_callback((void *)render_data_processor_linked_effect, moby);
        break;
    }
    }
}
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DBA50), where it is exact; names translated to the US level program. */

typedef int u128_c __attribute__((mode(TI)));

/* Four (u, v) pairs: the render loop reads [k * 2] and [k * 2 + 1] for k < 4. */
f32 data_processor_linked_effect_base_uvs[8] __asm__("D_L10_001D9FF0") = {
    0.0f, 0.0f, 0.0f, 1.0f, 0.25f, 0.0f, 0.25f, 1.0f};

/* One 16-byte vector per quad vertex (vertex < 4, stride 0x10, added by fast_vec_add). */
f32 data_processor_linked_effect_quad_vertex_offsets[4][4] __asm__("D_L10_001DA010") = {
    {0.0f, 0.0f, -0.05f, 1.0f},
    {0.0f, 0.0f,  0.05f, 1.0f},
    {0.0f, 0.0f, -0.05f, 1.0f},
    {0.0f, 0.0f,  0.05f, 1.0f},
};

extern s32 D_L10_00161D64;
extern s32 D_L10_00161D68;
extern s32 fast_tween_color(s32, s32, f32) __asm__("FUN_001fa6e0");

#include "rnc/overlay/l10/mobys/data_processor_linked_effect_data/render_data_processor_linked_effect.h"

extern int data_processor_linked_effect_color_at_zero_progress_gp __asm__("D_L10_00161C90")
    __attribute__((sda));
extern int data_processor_linked_effect_color_at_full_progress_gp __asm__("D_L10_00161C94")
    __attribute__((sda));

extern void rotation_matrix(void *, void *) __asm__("FUN_001fa050");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern void draw_geometry_quad(void *, void *, s32) __asm__("FUN_001f7d30");

extern int D_L10_00161D64;
extern int D_L10_00161D68;

void render_data_processor_linked_effect(u8 *moby) {
    char *linked_effect_render_vars = ((char *)MOBY(moby)->pvars);
    char *linked_processor_vars =
        *(char **)((*(int *)(linked_effect_render_vars + 0xc) << 8) + (int)D_L10_0015FFD8 + 0x78);
    char strip_transforms[(unsigned int)*(int *)(linked_effect_render_vars + 4) * 0x40];
    struct {
        unsigned char vertices[0x50];
        float         matrix1[16];
        union {
            unsigned char quad[0x90];
            struct {
                unsigned char vertices[0x50];
                float         matrix2[16];
            } fields;
        } packet;
        float rotation[16];
    } buffers __attribute__((aligned(16)));
    int            i, j, group, row, vertex;
    float          fade, phase;
    unsigned char *order;
    int            group_age;
    char          *age_base;
    char          *ages;
    int            group_vertices;
    float         *matrix1;
    int            next_group, next_row;
    float         *matrix2, *uv1, *uv2;

    {
        float distance = fast_vec_dist(moby + 0x10, (u8 *)&D_L10_00167240);
        if (distance > 48.0f)
            fade = (80.0f - distance) * 0.03125f;
        else
            fade = 1.0f;
    }
    set_scale_matrix(buffers.rotation, *(float *)linked_effect_render_vars * 0.5f);
    matrix1 = buffers.matrix1;
    matrix2 = buffers.packet.fields.matrix2;
    uv1 = matrix1 + 1;
    uv2 = matrix2 + 1;
    {
        int transform_index;
        for (transform_index = 0; transform_index < *(int *)(linked_effect_render_vars + 4);
             transform_index++) {
            rotation_matrix(strip_transforms + transform_index * 0x40, moby + 0x40);
            multiply_matrix(strip_transforms + transform_index * 0x40,
                            strip_transforms + transform_index * 0x40,
                            buffers.rotation);
            *(u128_c *)(strip_transforms + transform_index * 0x40 + 0x30) =
                *(u128_c *)(moby + 0x10);
            *(float *)(strip_transforms + transform_index * 0x40 + 0x38) +=
                convert_integer_to_float(transform_index + 1) *
                *(float *)(linked_effect_render_vars + 8);
        }
    }
    {
        u64 texture = get_effect_texture(14);
        u64 packet_flags = ((u64)data_processor_linked_effect_blend_minuend_selector_gp) |
                           ((u64)data_processor_linked_effect_blend_subtrahend_selector_gp << 2) |
                           ((u64)data_processor_linked_effect_blend_alpha_factor_selector_gp << 4) |
                           ((u64)data_processor_linked_effect_blend_addend_selector_gp << 6) |
                           0x8000000000ULL;
        ((u64 *)buffers.packet.fields.matrix2)[5] = texture;
        ((u64 *)buffers.matrix1)[5] = texture;
        ((u64 *)buffers.packet.fields.matrix2)[6] = 0xff9000000260ULL;
        ((u64 *)buffers.matrix1)[6] = 0xff9000000260ULL;
        ((u64 *)buffers.packet.fields.matrix2)[4] = 0;
        ((u64 *)buffers.matrix1)[4] = 0;
        ((u64 *)buffers.packet.fields.matrix2)[7] = packet_flags;
        ((u64 *)buffers.matrix1)[7] = packet_flags;
    }
    {
        int k;
        for (k = 0; k < 4; k++) {
            uv1[k * 2] = uv2[k * 2] = data_processor_linked_effect_base_uvs[k * 2 + 1];
        }
    }
    group = 0;
    do {
        next_group = group + 1;
        phase = 0.0f;
        row = 0;
        do {
            int next_strip_index;
            phase += data_processor_linked_effect_row_uv_phase_step;
            if (phase > 1.0f)
                phase -= 1.0f;
            next_row = row + 1;
            for (j = 0; j < 4; j++) {
                matrix1[j * 2] = matrix2[j * 2] = data_processor_linked_effect_base_uvs[j * 2] +
                                                  phase +
                                                  *(float *)(linked_processor_vars + 0x12de0);
            }
            for (i = 0; i < *(int *)(linked_effect_render_vars + 4); i = next_strip_index) {
                int          offset;
                int          age;
                float        progress;
                unsigned int color;
                order = (unsigned char *)(linked_effect_render_vars + 0x10);
                group_age = group * 0x78;
                age_base = linked_processor_vars + 0x10000;
                ages = age_base + 0x2c00;
                age = *(int *)(ages + (order[i] * 4 + group_age));
                group_vertices = group * 0x2580;
                if (age > D_L10_00161D68) {
                    progress = convert_integer_to_float(age - D_L10_00161D68) /
                               convert_integer_to_float(D_L10_00161D64 - D_L10_00161D68);
                } else {
                    progress = 1.0f - convert_integer_to_float(age) /
                                          convert_integer_to_float(D_L10_00161D68);
                }
                color = fast_tween_color(data_processor_linked_effect_color_at_zero_progress_gp,
                                         data_processor_linked_effect_color_at_full_progress_gp,
                                         progress);
                color =
                    (color & 0xffffff) +
                    (truncate_float_to_s32(fade * fade * convert_integer_to_float((int)color >> 24))
                     << 24);
                if (moby[0x20] == 2) {
                    float alpha =
                        convert_integer_to_float(*(int *)(linked_effect_render_vars + 0x34));
                    float duration =
                        scale_time(data_processor_linked_effect_fade_duration_seconds_gp * 60.0f);
                    color = (truncate_float_to_s32(alpha * 255.0f / duration) << 24) | 0xffffff;
                }
                vertex = 0;
                offset = i * 0x40;
                next_strip_index = i + 1;
                *(unsigned int *)(buffers.packet.quad + 0x4c) = color;
                *(unsigned int *)(buffers.packet.quad + 0x48) = color;
                *(unsigned int *)(buffers.packet.quad + 0x44) = color;
                *(unsigned int *)(buffers.packet.quad + 0x40) = color;
                for (; vertex < 4; vertex++) {
                    fast_vec_add(buffers.packet.quad + vertex * 0x10,
                                 (char *)data_processor_linked_effect_quad_vertex_offsets +
                                     vertex * 0x10,
                                 linked_processor_vars + group_vertices +
                                     (row + (vertex >> 1)) * 0x140 + order[i] * 0x10);
                }
                draw_geometry_quad(buffers.packet.quad, strip_transforms + offset, 0);
            }
            row = next_row;
        } while (row < 19);
        group = next_group;
    } while (group < 4);
}

int D_L10_00161D64 = 0;
int D_L10_00161D68 = 0;

unsigned char l10_data_processor_linked_effect_initial_strip_order[10] __asm__("D_L10_00161CA8") = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
