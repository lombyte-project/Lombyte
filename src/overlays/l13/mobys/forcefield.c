/* Level 13 forcefield moby class and its contact effects. */
#include "types.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/rendering/set_moby_color.h"
#include "rnc/overlay/shared/rendering/effects/glow_quads_draw.h"
#include "qzero.h"

extern void mark_moby_for_removal(void *) __asm__("func_0020C828");

extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");

extern void spawn_nearby_effect_without_direction(float, void *, void *, int, void *) __asm__("FUN_L10_0024c7f0");

extern void *memcpy(void *, const void *, unsigned int);

extern void *sphere_hits_ed158[] __asm__("D_L13_00178200");

void spawn_forcefield_contact_effects(void *moby, void *data) __asm__("FUN_L13_002ed158");

void spawn_forcefield_contact_effects(void *moby, void *data) {
    float  matrix[12];
    float  query[4];
    float  candidate_pos[4];
    float  local_pos[4];
    float  rotation_matrix[16];
    float  probe[4];
    void **candidates;
    int    count, hit_count, i, j;
    char  *candidate;
    short  half_width, half_height, max_size;
    float  radius;
    float  limit, tolerance, height_limit, height_tolerance;

    euler_rotation_basis(matrix, moby + 0x40);
    max_size =
        *(short *)(data + 10) < *(short *)(data + 8) ? *(short *)(data + 8) : *(short *)(data + 10);
    radius = (float)max_size * 0.5f;
    qzero(query);
    query[2] = radius;
    transform_vector_by_basis(query, query, matrix);
    fast_vec_add(query, query, moby + 0x10);
    count = collect_mobies_in_sphere(query, radius, 0x10, moby, 0);
    if (count == 0)
        return;
    candidates = __builtin_alloca(((unsigned)count << 5) >> 3);
    memcpy(candidates, sphere_hits_ed158, count * 4);
    for (i = 0; i < count; i++) {
        candidate = *(void **)((u32)candidates + (i << 2));
        if (candidate == 0 || (unsigned char)candidate[0x20] == 0xfe ||
            (unsigned char)candidate[0x20] == 0xfd)
            continue;
        fast_vec_sub(candidate_pos, candidate + 0x10, moby + 0x10);
        transpose_rotation_matrix(rotation_matrix, matrix);
        transform_vector(local_pos, candidate_pos, rotation_matrix);
        qcopy(probe, local_pos);
        half_width = *(short *)(data + 8);
        limit = (float)half_width * 0.5f;
        tolerance = limit + 0.7f;
        if (probe[0] < -tolerance)
            probe[0] = (float)-half_width * 0.5f;
        else if (tolerance < probe[0])
            probe[0] = limit;
        half_height = *(short *)(data + 10);
        height_limit = (float)half_height * 0.5f;
        height_tolerance = height_limit + 1.5f;
        if (probe[2] < -height_tolerance)
            probe[2] = (float)-half_height * 0.5f;
        else if (height_tolerance < probe[2])
            probe[2] = height_limit;
        probe[1] = 0.0f;
        transform_vector_by_basis(probe, probe, matrix);
        fast_vec_add(probe, probe, moby + 0x10);
        hit_count = collect_mobies_in_sphere(probe, 0.7f, 0x10, moby, 0);
        for (j = 0; j < hit_count; j++) {
            if (sphere_hits_ed158[j] == candidate)
                spawn_nearby_effect_without_direction(1.0f, candidate, moby, 0x10001, probe);
        }
    }
}

extern void stub_printf(char *, int, int) __asm__("FUN_001e93b0");

extern void spawn_forcefield_contact_effects(void *, void *) __asm__("FUN_L13_002ed158");

extern void offset_moby_class_point_pairs(void *, int, int) __asm__("FUN_L00_0024efb0");

extern char forcefield_missing_trigger_format[] __asm__("D_L13_001F4FB0");

void forcefield_update(struct Moby *m) __asm__("FUN_L13_002ed4a8");

void forcefield_update(struct Moby *moby) {
    enum {
        FORCEFIELD_STATE_INITIALIZE = 0,
        FORCEFIELD_STATE_MONITOR_LINKED_MOBY = 1,
        FORCEFIELD_STATE_FADE_OUT = 2,
        FORCEFIELD_STATE_REMOVE = 3
    };
    char *forcefield_data = ((char *)moby->pvars);
    int   wrap_adjusted_step;
    switch (moby->state) {
    case FORCEFIELD_STATE_INITIALIZE:
        moby->state = 1;
        moby->alpha = 0x14;
        moby->flags |= 0xA08;
        *(short *)(forcefield_data + 0xE) = -1;
        *(float *)(forcefield_data + 4) = 1.0f;
        set_moby_color(moby, 0x80, 0x80, 0x80);
        break;
    case FORCEFIELD_STATE_MONITOR_LINKED_MOBY:
        if (*(int *)forcefield_data != -1) {
            unsigned char *linked_moby =
                (unsigned char *)(moby_base + (*(int *)forcefield_data << 8));
            if (linked_moby == 0 || linked_moby[0x20] == 0xFE || linked_moby[0x20] == 0xFD ||
                (D_0014C190_257218[current_level_index][((short)MOBY(linked_moby)->save_id) >> 5] >>
                 (MOBY(linked_moby)->save_id & 0x1F)) &
                    1) {
                *(float *)(forcefield_data + 4) = 1.0f;
                moby->state = 2;
            }
        } else {
            *(float *)(forcefield_data + 4) = 1.0f;
            moby->state = 2;
            stub_printf(forcefield_missing_trigger_format, moby->oclass, ((short)moby->save_id));
        }
        spawn_forcefield_contact_effects(moby, forcefield_data);
        break;
    case FORCEFIELD_STATE_FADE_OUT:
        *(float *)(forcefield_data + 4) -= frame_scale * 0.05f;
        if (*(float *)(forcefield_data + 4) < 0.0f)
            moby->state = 3;
        else
            moby->alpha = truncate_float_to_s32(*(float *)(forcefield_data + 4) * 20.0f);
        break;
    case FORCEFIELD_STATE_REMOVE:
        if (*(short *)(forcefield_data + 0xE) != -1) {
            release_point_light_slot(*(short *)(forcefield_data + 0xE));
            *(short *)(forcefield_data + 0xE) = -1;
        }
        mark_moby_for_removal(moby);
        return;
    }
    wrap_adjusted_step = 0xC0;
    *(short *)(forcefield_data + 0xC) += 0xC0;
    if (*(short *)(forcefield_data + 0xC) > 0x1000) {
        *(short *)(forcefield_data + 0xC) -= 0x1000;
        wrap_adjusted_step = -0xF40;
    } else if (*(short *)(forcefield_data + 0xC) < 0) {
        *(short *)(forcefield_data + 0xC) += 0x1000;
        wrap_adjusted_step = 0x10C0;
    }
    offset_moby_class_point_pairs(((void *)moby->pclass), wrap_adjusted_step, 0);
}
