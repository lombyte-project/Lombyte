/* Level 15 codebot slot moby class and its helpers. */
#include "types.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/game/item_state.h"

extern float probe_ground_height(void *, int, float) __asm__("func_00213508");

#include "rnc/overlay/l15/mobys/codebot_slot_globals.h"

extern void add_vectors(void *, void *, void *) __asm__("FUN_L00_001ff290");

extern float fast_add_rotations(float, float);

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

extern char *create_moby(int) __asm__("FUN_0020c4f8");

extern void func_L00_0025E210_u(void *) __asm__("FUN_L00_0025d1b8");

extern void upd_a(void *) __asm__("FUN_L00_00250df8");

char *spawn_codebot_slot_effect(char *spawn_position,
                                char *spawn_rotation) __asm__("FUN_L15_002eb9e0");

char *spawn_codebot_slot_effect(char *spawn_position, char *spawn_rotation) {
    char  saved_spawn_position_and_rotation[32] __attribute__((aligned(16)));
    char *saved_position = saved_spawn_position_and_rotation;
    char *spawned_moby;
    *(OvlQuad *)saved_spawn_position_and_rotation = *(OvlQuad *)spawn_position;
    *(OvlQuad *)(saved_spawn_position_and_rotation + 0x10) = *(OvlQuad *)spawn_rotation;
    spawned_moby = create_moby(0x594);
    if (spawned_moby != 0) {
        func_L00_0025E210_u(spawned_moby);
        MOBY(spawned_moby)->update_dist = 0xFF;
        MOBY(spawned_moby)->draw_dist = 0xFF;
        spawned_moby[0x31] = 1;
        qcopy(spawned_moby + 0x10, saved_position);
        *(OvlQuad *)&MOBY(spawned_moby)->rot =
            *(OvlQuad *)(saved_spawn_position_and_rotation + 0x10);
        upd_a(spawned_moby);
    }
    return spawned_moby;
}

extern int D_L15_001ABE40[];

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

void set_moby_list_visibility(int moby_list_index, int show) __asm__("FUN_L15_002ebf28");

void set_moby_list_visibility(int moby_list_index, int show) {
    short *moby_index_entry = (short *)D_L15_001ABE40[moby_list_index];
    if (moby_index_entry == 0)
        return;
    while (1) {
        unsigned char *moby =
            (unsigned char *)((int)l15_moby_array + ((*moby_index_entry & 0x7FFF) << 8));
        if (show) {
            MOBY(moby)->collision = *(int *)(((char *)MOBY(moby)->pclass) + 0x10);
            moby[0x31] = 1;
            MOBY(moby)->flags &= 0xFFFC;
        } else {
            MOBY(moby)->collision = 0;
            moby[0x31] = 0;
            MOBY(moby)->flags |= 3;
        }
        if (*moby_index_entry++ < 0)
            return;
    }
}

int count_missing_or_dying_mobys_in_list(int moby_list_index) __asm__("FUN_L15_002ebfb0");

int count_missing_or_dying_mobys_in_list(int moby_list_index) {
    unsigned short *moby_index_entry = (unsigned short *)D_L15_001ABE40[moby_list_index];
    int             missing_or_dying_count = 0;
    int             moby_array_base;
    if (moby_index_entry == 0)
        return 0;
    moby_array_base = (int)l15_moby_array;
    do {
        unsigned char *moby =
            (unsigned char *)(moby_array_base + ((moby_index_entry[0] & 0x7FFF) << 8));
        if (moby == 0 || moby[0x20] == 0xFE || moby[0x20] == 0xFD)
            missing_or_dying_count++;
    } while ((short)*moby_index_entry++ >= 0);
    return missing_or_dying_count;
}

extern unsigned int D_L15_001BAAD0[];

extern unsigned int l15_banked_moby_save_bits[] __asm__("D_L15_0014C190");

extern int D_L15_0015ED98;

extern volatile int D_L15_00184AAC NOT_SDA;

extern float D_0013F3D0[];

extern void FUN_L15_002f78b8(int);

int can_spawn_codebot_slot_effect(int slot) __asm__("FUN_L15_002f78e0");

extern int can_spawn_codebot_slot_effect(int) __asm__("FUN_L15_002f78e0");

extern int count_missing_or_dying_mobys_in_list(int) __asm__("FUN_L15_002ebfb0");

extern void set_moby_list_visibility(int, int) __asm__("FUN_L15_002ebf28");

extern char *spawn_codebot_slot_effect(char *, char *) __asm__("FUN_L15_002eb9e0");

extern float scale_time(float) __asm__("FUN_L15_001f8648");

void codebot_slot_update(struct Moby *moby) __asm__("FUN_L15_002ec030");

/* Moby class 1430 */
void codebot_slot_update(struct Moby *moby) {
    enum {
        CODEBOT_SLOT_INITIALIZE = 0,
        CODEBOT_SLOT_WAIT_FOR_PROXIMITY = 1,
        CODEBOT_SLOT_WAIT_TO_CREATE_EFFECT = 2,
        CODEBOT_SLOT_ANIMATE_EFFECT = 3,
        CODEBOT_SLOT_COMPLETED = 4
    };
    int    *codebot_slot_private_data = (int *)moby->pvars;
    float   position_work_vector[4] __attribute__((aligned(16)));
    float   position_rotation_work_vector[4] __attribute__((aligned(16)));
    OvlQuad position;
    float   angle;
    char   *effect;
    if (codebot_slot_private_data[3] != -1) {
        if (moby->state != 0) {
            fast_vec_sub(position_rotation_work_vector,
                         (char *)l15_moby_array + codebot_slot_private_data[3] * 0x100 + 0x10,
                         codebot_slot_private_data + 4);
            *(OvlQuad *)position_work_vector = *(OvlQuad *)position_rotation_work_vector;
            add_vectors(position_rotation_work_vector, &moby->pos, position_work_vector);
        }
        *(OvlQuad *)(codebot_slot_private_data + 4) =
            *(OvlQuad *)((char *)l15_moby_array + codebot_slot_private_data[3] * 0x100 + 0x10);
    }
    switch (moby->state) {
    case CODEBOT_SLOT_INITIALIZE: {
        int save_index;
        int i;
        save_index = ((int)((unsigned int)moby->save_id << 16)) >> 16;
        if (l15_save_state.cleared[save_index] != 0 ||
            ((*(int *)(level_save_bank_index * 0x100 + (save_index >> 5) * 4 +
                       (char *)l15_banked_moby_save_bits) >>
                  (moby->save_id & 31) &
              1) != 0)) {
            moby->state = 4;
            break;
        }
        D_L15_00184AAC = 0;
        moby->state = 1;
        for (i = 0; i < 24; i++) {
            if (codebot_slot_private_data[i + 8] != -1)
                set_moby_list_visibility(codebot_slot_private_data[i + 8], 0);
        }
        break;
    }
    case CODEBOT_SLOT_WAIT_FOR_PROXIMITY:
        if (alternate_item_available[2] != 0 && vector_distance_xy(D_0013F3D0, &moby->pos) < 3.0f &&
            *((unsigned char *)D_0013F3D0 + 0x2024) == 0) {
            FUN_L15_002f78b8(codebot_slot_private_data[0]);
            moby->state = 2;
        }
        break;
    case CODEBOT_SLOT_WAIT_TO_CREATE_EFFECT:
        if (can_spawn_codebot_slot_effect(codebot_slot_private_data[0])) {
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 2.0f;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 2.0f;
            position_work_vector[2] = 0.0f;
            add_vectors(&position, position_work_vector, &moby->pos);
            clear_u64_value(position_rotation_work_vector);
            position_rotation_work_vector[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            position_rotation_work_vector[1] = 1.5707964f;
            codebot_slot_private_data[1] = (int)spawn_codebot_slot_effect(
                (char *)position_work_vector, (char *)position_rotation_work_vector);
            codebot_slot_private_data[2] = 0;
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 3.0f;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 3.0f;
            position_work_vector[2] = 0.0f;
            add_vectors(&position, position_work_vector, &moby->pos);
            position_work_vector[2] = probe_ground_height(position_work_vector, 0, 0.5f);
            clear_u64_value(position_rotation_work_vector);
            position_rotation_work_vector[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            set_race_camera(position_work_vector, position_rotation_work_vector, 0x72, 0);
            if (codebot_slot_private_data[8] != -1)
                set_moby_list_visibility(codebot_slot_private_data[8], 1);
            moby->state = 3;
        }
        break;
    case CODEBOT_SLOT_ANIMATE_EFFECT:
        if (codebot_slot_private_data[1] != 0) {
            *(float *)&codebot_slot_private_data[2] =
                fast_add_rotations(*(float *)&codebot_slot_private_data[2],
                                   (360.0f / scale_time(360.0f)) * 0.017453292f);
            angle = fast_cos(*(float *)&codebot_slot_private_data[2]) + 1.0f;
            position_work_vector[0] =
                fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * angle;
            position_work_vector[1] =
                fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * angle;
            position_work_vector[2] = 0.0f;
            effect = (char *)codebot_slot_private_data[1];
            fast_vec_add(position_rotation_work_vector, &moby->pos, position_work_vector);
            *(OvlQuad *)(effect + 0x10) = *(OvlQuad *)position_rotation_work_vector;
            if (angle < 0.5f) {
                unsigned int *bits;
                bits = (unsigned int *)(((short)moby->save_id >> 5) * 4 +
                                        level_save_bank_index * 0x100 +
                                        (char *)l15_banked_moby_save_bits);
                *bits |= 1u << (moby->save_id & 31);
                {
                    unsigned int *bits = &D_L15_001BAAD0[(short)moby->save_id >> 5];
                    *bits |= 1u << (moby->save_id & 31);
                }
                moby->state = 4;
            }
        }
        break;
    case CODEBOT_SLOT_COMPLETED:
        D_L15_00184AAC = 1;
        if (*(volatile int *)(codebot_slot_private_data + 1) == 0) {
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            position_work_vector[2] = 0.0f;
            add_vectors(&position, position_work_vector, &moby->pos);
            clear_u64_value(position_rotation_work_vector);
            position_rotation_work_vector[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            position_rotation_work_vector[1] = 1.5707964f;
            *(OvlQuad *)&position = *(OvlQuad *)&moby->pos;
            codebot_slot_private_data[1] = (int)spawn_codebot_slot_effect(
                (char *)&position, (char *)position_rotation_work_vector);
        } else {
            position_work_vector[0] = fast_cos(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            position_work_vector[1] = fast_sin(fast_add_rotations(moby->rot.z, -1.5707964f)) * 0.5f;
            position_work_vector[2] = 0.0f;
            add_vectors(&position, position_work_vector, &moby->pos);
            clear_u64_value(position_rotation_work_vector);
            position_rotation_work_vector[2] = fast_add_rotations(moby->rot.z, 1.5707964f);
            position_rotation_work_vector[1] = 1.5707964f;
            *(OvlQuad *)((char *)codebot_slot_private_data[1] + 0x10) =
                *(OvlQuad *)position_work_vector;
            *(OvlQuad *)((char *)codebot_slot_private_data[1] + 0x40) =
                *(OvlQuad *)position_rotation_work_vector;
        }
        if (alternate_item_available[0x6D] == 0 &&
            count_missing_or_dying_mobys_in_list(codebot_slot_private_data[8]) > 9) {
            alternate_item_available[0x6D] = 1;
            D_L15_0015ED98 += 15000;
        }
        break;
    }
}

extern char *D_L15_00167400_lui __asm__("D_L15_00167400") NOT_SDA;

extern char *moby_slot_entry_table __asm__("D_L15_0015EF50");

int can_spawn_codebot_slot_effect(int slot) {
    char *slot_state_data;
    if (*(short *)(D_L15_00167400_lui + 0x86) != 0x13)
        return 1;
    slot_state_data =
        *(char **)(*(char **)(*(char **)(moby_slot_entry_table + slot * 32 + 0x1C) + 4) + 0x70) +
        0x40;
    if (*(short *)(slot_state_data + 0x14) == 1 && codebot_slot_effect_spawn_gate != 0.0f)
        return 1;
    return *(short *)(slot_state_data + 0x14) == 3;
}
