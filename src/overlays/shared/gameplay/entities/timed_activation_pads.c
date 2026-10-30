/* Shared moby classes: timed_activation_pads, door, bridge. */
#include "types.h"
#include "asm.h"
#include "qcopy.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/game_globals.h"
#include "rnc/globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/gameplay/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/gameplay/entities/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/gameplay/entities/trackdrive_left.h"
#include "rnc/overlay/shared/gameplay/state/unlock_planet.h"



/* Ported from rac1-decomp (src/overlays/shared/vendor_002F9438.c: func_L07_002F9438), where it is exact; names translated to the US level program. */

extern char draw_registration_value_target[] __asm__("D_L07_001D3400");
extern void l07_moby_draw_callback(void) __asm__("FUN_L07_0030d420");

void update_moby_draw_registration(unsigned char *moby) __asm__("FUN_L07_002f8058");

void update_moby_draw_registration(unsigned char *moby) {
    enum {
        DRAW_REGISTRATION_INITIALIZE = 0,
        DRAW_REGISTRATION_ENQUEUE_CALLBACK = 1
    };
    switch (moby[0x20]) {
    case DRAW_REGISTRATION_INITIALIZE:
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        prepare_scaled_float_table_with_target_byte(draw_registration_value_target, 0x80, 1.0f);
        break;
    case DRAW_REGISTRATION_ENQUEUE_CALLBACK:
        add_draw_callback(l07_moby_draw_callback, moby);
        break;
    }
}
/* Ported from rac1-decomp src/overlays/shared/vendor_002F9438.c (func_L07_0030D370) */
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"

extern int set_moby_color() __asm__("FUN_L00_002502f0");
extern s32 allocate_voice_for_target_entry() __asm__("func_0022DA68");
extern s32 scale_game_frames() __asm__("func_001F96F8");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
float absolute_float(float input) __asm__("func_001F99C0");
void FUN_L01_0026e090(int i, int v);
void set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");
extern char D_0013F350_lui[] __asm__("D_0013F350") NOT_SDA;
extern void set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");
extern void func_L01_0026F040_30D370(int, int) __asm__("FUN_L01_0026e090");
extern int func_001F9850_30D370(int) __asm__("FUN_001f96f8");
extern void glow_n(void *, int, int, int) __asm__("FUN_L00_002502f0");

void timed_activation_pads_update(unsigned char *moby) __asm__("FUN_L07_0030bf90");
void timed_activation_pads_update(unsigned char *moby)
{
    enum {
        PAD_INITIALIZE = 0,
        PAD_WAIT_FOR_ACTIVATION = 1,
        PAD_ADVANCE_TO_FLASH_ACTIVATION_GATE = 2,
        PAD_WAIT_TO_ENTER_FLASH_ACTIVATION_WAIT = 3,
        PAD_ENTER_FLASH_ACTIVATION_WAIT = 4,
        PAD_WAIT_FOR_FLASH_ACTIVATION = 5,
        PAD_INITIALIZE_FLASHING = 0x6,
        PAD_UPDATE_FLASHING = 0x7,
        PAD_RESET_TO_ACTIVATION_WAIT = 0x8
    };
    enum {
        PAD_FLASH_APPLY_GREEN = 0x0,
        PAD_FLASH_APPLY_WHITE = 0x1
    };
    char *pad_data;
    char *activation_reference_data;
    float activation_offset[4];
    float transposed_pad_rotation[16];
    int flag;
    int on;
    int pad_state;

    if (moby == 0)
        return;
    pad_data = ((char *)MOBY(moby)->pvars);
    if (pad_data == 0)
        return;

    flag = 0;
    if (moby[0x20] < 6) {
        if (moby[0xB0] != 0xFF) {
            if (visited52[moby[0xB0] + (current_level_index << 4)] == 0xFF)
                set_moby_list_state(moby[0x21], 6);
        }
    }

    if (*(int *)(pad_data + 0x8) == 0) {
        activation_reference_data = (char *)D_0013F350_lui;
        if (*(short *)(activation_reference_data + 0x30E) == 0)
            flag = *(unsigned char **)(activation_reference_data + 0x2FC) == moby;
        if (0.7853982f < absolute_float(MOBY(moby)->rot.y)) {
            if (vector_distance_xy(moby + 0x10, activation_reference_data + 0x80) < 2.0f) {
                if (*(int *)(activation_reference_data + 0x2084) == 0x11) {
                    if (*(float *)(activation_reference_data + 0xAA8) < 7.0f) {
                        fast_vec_sub(activation_offset, moby + 0x10, activation_reference_data + 0x80);
                        set_identity_matrix(transposed_pad_rotation);
                        transpose_rotation_matrix(transposed_pad_rotation, moby + 0xC0);
                        transform_vector(activation_offset, activation_offset, transposed_pad_rotation);
                        if (absolute_float(activation_offset[0]) < 0.6f) {
                            if (absolute_float(activation_offset[1]) < 0.6f) {
                                if (absolute_float(activation_offset[2]) < 0.957f)
                                    flag = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    pad_state = moby[0x20];
    if ((unsigned int)(pad_state - 2) < 4) {
        if (moby[0xBC] == 1) {
            if (fast_dec_timer((int *)pad_data)) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                set_moby_list_state(moby[0x21], 8);
            } else if (fast_dec_timer((int *)(pad_data + 4))) {
                allocate_voice_for_target_entry(0, 0, (int)moby);
                *(int *)(pad_data + 4) = func_001F9850_30D370(0x2D) < func_001F9850_30D370(5) + *(int *)pad_data / func_001F9850_30D370(0x28)
                    ? func_001F9850_30D370(0x2D) : func_001F9850_30D370(5) + *(int *)pad_data / func_001F9850_30D370(0x28);
            }
        }
    }

    switch (moby[0x20]) {
    case PAD_INITIALIZE:
        glow_n(moby, 0x80, 0x80, 0x80);
        func_L01_0026F040_30D370(moby[0x21], 0);
        set_moby_list_state(moby[0x21], 1);
        break;
    case PAD_WAIT_FOR_ACTIVATION:
        *(int *)(pad_data + 0x8) = 0;
        glow_n(moby, 0x80, 0x80, 0x80);
        if (flag) {
            if (*(int *)(pad_data + 0x10) > 0)
                *(int *)pad_data = func_001F9850_30D370(*(int *)(pad_data + 0x10) * 60);
            else
                *(int *)pad_data = func_001F9850_30D370(900);
            on = 1;
            glow_n(moby, 0, 0xFF, 0);
            *(int *)(pad_data + 0x8) = on;
            func_L01_0026F040_30D370(moby[0x21], 0);
            moby[0xBC] = on;
            allocate_voice_for_target_entry(3, 0, (int)moby);
            set_moby_list_state(moby[0x21], 2);
        }
        break;
    case PAD_ADVANCE_TO_FLASH_ACTIVATION_GATE:
        moby[0x20] = 3;
        break;
    case PAD_WAIT_TO_ENTER_FLASH_ACTIVATION_WAIT:
        if (flag) {
            *(int *)(pad_data + 0x8) = 1;
            glow_n(moby, 0, 0xFF, 0);
            allocate_voice_for_target_entry(3, 0, (int)moby);
            set_moby_list_state(moby[0x21], 4);
        }
        break;
    case PAD_ENTER_FLASH_ACTIVATION_WAIT:
        moby[0x20] = 5;
        break;
    case PAD_WAIT_FOR_FLASH_ACTIVATION:
        if (flag) {
            *(int *)(pad_data + 0x8) = 1;
            glow_n(moby, 0, 0xFF, 0);
            set_moby_list_state(moby[0x21], 6);
            allocate_voice_for_target_entry(1, 0, (int)moby);
        }
        break;
    case PAD_INITIALIZE_FLASHING:
        *(short *)(pad_data + 0xC) = func_001F9850_30D370(0x14);
        *(short *)(pad_data + 0xE) = 0;
        glow_n(moby, 0x80, 0xFF, 0x80);
        moby[0x20] = 7;
        break;
    case PAD_UPDATE_FLASHING:
        if (moby[0xB0] != 0xFF) {
            if (visited52[moby[0xB0] + (current_level_index << 4)] != 0xFF)
                jet_clear_persistent(moby[0xB0]);
        }
        if (fast_dec_timer_s16(pad_data + 0xC)) {
            *(short *)(pad_data + 0xC) = func_001F9850_30D370(0x14);
            switch (*(short *)(pad_data + 0xE)) {
            case PAD_FLASH_APPLY_GREEN:
                glow_n(moby, 0, 0xFF, 0);
                *(short *)(pad_data + 0xE) = 1;
                break;
            case PAD_FLASH_APPLY_WHITE:
                glow_n(moby, 0xFF, 0xFF, 0xFF);
                *(short *)(pad_data + 0xE) = 0;
                break;
            case 2 ... 5:
                *(short *)(pad_data + 0xE) = 0;
                break;
            }
        }
        break;
    case PAD_RESET_TO_ACTIVATION_WAIT:
        glow_n(moby, 0x80, 0x80, 0x80);
        moby[0xBC] = 0;
        moby[0x20] = 1;
        break;
    case 9:
        break;
    }
}
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern f32 fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void fast_vec_normalize(void *, void *, f32) __asm__("FUN_001f9bf8");

void door_update(u8 *moby) __asm__("FUN_L07_00310df0");
void door_update(u8 *moby)
{
    enum {
        DOOR_INITIALIZE = 0,
        DOOR_WAIT_TO_MOVE_TO_OFFSET_POSITION = 1,
        DOOR_MOVE_TO_OFFSET_POSITION = 2,
        DOOR_WAIT_TO_RETURN_TO_INITIAL_POSITION = 3,
        DOOR_RETURN_TO_INITIAL_POSITION = 4
    };
    u8 *data = ((u8 *)MOBY(moby)->pvars);
    u8 *other;
    f32 vectors[8];
    f32 distance;
    u8 state;
    typedef u32 quadword __attribute__((mode(TI), aligned(16)));

    if (moby[0xb0] != 0xff &&
        visited52[moby[0xb0] + current_level_index * 16] == 0xff)
        *(s32 *)(data + 0x10) = -1;

    state = moby[0x20];
    switch (state) {
    case DOOR_INITIALIZE:
        qcopy(data, moby + 0x10);
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 0;
        moby[0x20] = 1;
        *(s32 *)(data + 0x0c) = 0;
        return;
    case DOOR_WAIT_TO_MOVE_TO_OFFSET_POSITION:
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 0;
        if (*(s32 *)(data + 0x10) != -1) {
            other = level_mobys + *(s32 *)(data + 0x10) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] != *(u32 *)(data + 0x14))
                return;
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = *(f32 *)(data + 0x1c);
        vectors[5] = *(f32 *)(data + 0x20);
        vectors[6] = *(f32 *)(data + 0x24);
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        *(f32 *)(data + 0x0c) = fast_vec_length(vectors);
        if (*(s32 *)(data + 0x30) != -1)
            allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
        moby[0x20] = 2;
        return;
    case DOOR_MOVE_TO_OFFSET_POSITION:
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 1;
        if (*(s32 *)(data + 0x10) != -1) {
            other = level_mobys + *(s32 *)(data + 0x10) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] == *(u32 *)(data + 0x18)) {
                if (*(s32 *)(data + 0x30) != -1)
                    allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
                moby[0x20] = 4;
                return;
            }
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = *(f32 *)(data + 0x1c);
        vectors[5] = *(f32 *)(data + 0x20);
        vectors[6] = *(f32 *)(data + 0x24);
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        transform_vector_by_basis(vectors, vectors, moby + 0xc0);
        fast_vec_normalize(vectors + 4, vectors, *(f32 *)(data + 0x28) * frame_time);
        fast_vec_add(moby + 0x10, moby + 0x10, vectors + 4);
        *(f32 *)(data + 0x0c) -= *(f32 *)(data + 0x28) * frame_time;
        if (!(*(f32 *)(data + 0x0c) < 0.0f))
            return;
        fast_vec_add(moby + 0x10, data, vectors);
        *(f32 *)(data + 0x0c) = 0.0f;
        moby[0x20] = 3;
        if (*(s32 *)(data + 0x34) != -1)
            allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
        return;
    case DOOR_WAIT_TO_RETURN_TO_INITIAL_POSITION:
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 1;
        if (*(s32 *)(data + 0x10) == -1)
            return;
        other = level_mobys + *(s32 *)(data + 0x10) * 0x100;
        if (other == 0 || other[0x20] == 0xfe || other[0x20] == 0xfd ||
            other[0x20] != *(u32 *)(data + 0x18))
            return;
        *(s32 *)(data + 0x0c) = 0;
        if (*(s32 *)(data + 0x30) != -1)
            allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
        moby[0x20] = 4;
        return;
    case DOOR_RETURN_TO_INITIAL_POSITION:
        if (*(s32 *)(data + 0x38) != -1)
            D_L07_00184528[*(s32 *)(data + 0x38)] = 0;
        if (*(s32 *)(data + 0x10) == -1 ||
            (other = level_mobys + *(s32 *)(data + 0x10) * 0x100) == 0 ||
            other[0x20] == 0xfe || other[0x20] == 0xfd ||
            other[0x20] == *(u32 *)(data + 0x14)) {
            if (*(s32 *)(data + 0x30) != -1)
                allocate_voice_for_target_entry(*(s32 *)(data + 0x30), 0, moby);
            moby[0x20] = 2;
        } else {
            *(quadword *)(vectors + 4) = 0;
            vectors[4] = *(f32 *)(data + 0x1c);
            vectors[5] = *(f32 *)(data + 0x20);
            vectors[6] = *(f32 *)(data + 0x24);
            *(quadword *)vectors = *(quadword *)(vectors + 4);
            distance = fast_vec_length(vectors);
            fast_vec_normalize(vectors, vectors, -(*(f32 *)(data + 0x2c) * frame_time));
            transform_vector_by_basis(vectors + 4, vectors, moby + 0xc0);
            fast_vec_add(moby + 0x10, moby + 0x10, vectors + 4);
            *(f32 *)(data + 0x0c) += *(f32 *)(data + 0x2c) * frame_time;
            if (!(distance < *(f32 *)(data + 0x0c)))
                return;
            qcopy(moby + 0x10, data);
            if (*(s32 *)(data + 0x30) != -1)
                allocate_voice_for_target_entry(*(s32 *)(data + 0x34), 0, moby);
            if (*(s32 *)(data + 0x38) != -1)
                D_L07_00184528[*(s32 *)(data + 0x38)] = 0;
            moby[0x20] = 1;
        }
        break;
    case 5:
    default:
        return;
    }
}
extern void fast_vec_scale(f32, void *, void *) __asm__("FUN_001f9a68");
extern void scale_vector_xyz_alt_31aee0(f32, void *, void *) __asm__("FUN_001f9bf8");

void bridge_update(u8 *moby) __asm__("FUN_L07_0031aee0");
void bridge_update(u8 *moby)
{
    enum {
        BRIDGE_INITIALIZE = 0,
        BRIDGE_WAIT_TO_MOVE_TO_OFFSET_POSITION = 1,
        BRIDGE_MOVE_TO_OFFSET_POSITION = 2,
        BRIDGE_WAIT_TO_RETURN_TO_INITIAL_POSITION = 3,
        BRIDGE_RETURN_TO_INITIAL_POSITION = 4
    };
    u8 *data = ((u8 *)MOBY(moby)->pvars);
    u8 *other;
    f32 delta[4];
    f32 old_rotation[4];
    f32 vectors[8];
    f32 distance;
    u8 state;
    typedef u32 quadword __attribute__((mode(TI), aligned(16)));

    fast_vec_scale(-1.0f, delta, moby + 0x10);
    qcopy(old_rotation, moby + 0x40);

    if (moby[0xb0] != 0xff &&
        visited52[moby[0xb0] + current_level_index * 16] == 0xff)
        *(s32 *)(data + 0xb0) = -1;

    state = moby[0x20];
    switch (state) {
    case BRIDGE_INITIALIZE:
        qcopy(data + 0xa0, moby + 0x10);
        if (*(s32 *)(data + 0xd8) != -1)
            D_L07_00184528[*(s32 *)(data + 0xd8)] = 0;
        moby[0x20] = 1;
        *(s32 *)(data + 0xac) = 0;
        break;
    case BRIDGE_WAIT_TO_MOVE_TO_OFFSET_POSITION:
        if (*(s32 *)(data + 0xb0) != -1) {
            other = level_mobys + *(s32 *)(data + 0xb0) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] != *(u32 *)(data + 0xb4))
                break;
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = *(f32 *)(data + 0xbc);
        vectors[5] = *(f32 *)(data + 0xc0);
        vectors[6] = *(f32 *)(data + 0xc4);
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        *(f32 *)(data + 0xac) = fast_vec_length(vectors);
        if (*(s32 *)(data + 0xd0) != -1)
            allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
        moby[0x20] = 2;
        break;
    case BRIDGE_MOVE_TO_OFFSET_POSITION:
        if (*(s32 *)(data + 0xb0) != -1) {
            other = level_mobys + *(s32 *)(data + 0xb0) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] == *(u32 *)(data + 0xb8)) {
                if (*(s32 *)(data + 0xd0) != -1)
                    allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
                moby[0x20] = 4;
                break;
            }
        }
        *(quadword *)(vectors + 4) = 0;
        vectors[4] = *(f32 *)(data + 0xbc);
        vectors[5] = *(f32 *)(data + 0xc0);
        vectors[6] = *(f32 *)(data + 0xc4);
        *(quadword *)vectors = *(quadword *)(vectors + 4);
        transform_vector_by_basis(vectors, vectors, moby + 0xc0);
        scale_vector_xyz_alt_31aee0(*(f32 *)(data + 0xc8) * frame_time, vectors + 4, vectors);
        fast_vec_add(moby + 0x10, moby + 0x10, vectors + 4);
        *(f32 *)(data + 0xac) -= *(f32 *)(data + 0xc8) * frame_time;
        if (*(f32 *)(data + 0xac) < 0.0f) {
            fast_vec_add(moby + 0x10, data + 0xa0, vectors);
            *(f32 *)(data + 0xac) = 0.0f;
            if (*(s32 *)(data + 0xd8) != -1)
                D_L07_00184528[*(s32 *)(data + 0xd8)] = 1;
            moby[0x20] = 3;
            if (*(s32 *)(data + 0xd4) != -1)
                allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
        }
        break;
    case BRIDGE_WAIT_TO_RETURN_TO_INITIAL_POSITION:
        if (*(s32 *)(data + 0xb0) != -1) {
            other = level_mobys + *(s32 *)(data + 0xb0) * 0x100;
            if (other != 0 && other[0x20] != 0xfe && other[0x20] != 0xfd &&
                other[0x20] == *(u32 *)(data + 0xb8)) {
                *(s32 *)(data + 0xac) = 0;
                if (*(s32 *)(data + 0xd0) != -1)
                    allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
                moby[0x20] = 4;
            }
        }
        break;
    case BRIDGE_RETURN_TO_INITIAL_POSITION:
        if (*(s32 *)(data + 0xb0) == -1 ||
            (other = level_mobys + *(s32 *)(data + 0xb0) * 0x100) == 0 ||
            other[0x20] == 0xfe || other[0x20] == 0xfd ||
            other[0x20] == *(u32 *)(data + 0xb4)) {
            if (*(s32 *)(data + 0xd0) != -1)
                allocate_voice_for_target_entry(*(s32 *)(data + 0xd0), 0, moby);
            moby[0x20] = 2;
        } else {
            *(quadword *)(vectors + 4) = 0;
            vectors[4] = *(f32 *)(data + 0xbc);
            vectors[5] = *(f32 *)(data + 0xc0);
            vectors[6] = *(f32 *)(data + 0xc4);
            *(quadword *)vectors = *(quadword *)(vectors + 4);
                distance = fast_vec_length(vectors);
            scale_vector_xyz_alt_31aee0(-(*(f32 *)(data + 0xcc) * frame_time), vectors, vectors);
            transform_vector_by_basis(vectors + 4, vectors, moby + 0xc0);
            fast_vec_add(moby + 0x10, moby + 0x10, vectors + 4);
            *(f32 *)(data + 0xac) += *(f32 *)(data + 0xcc) * frame_time;
            if (distance < *(f32 *)(data + 0xac)) {
                qcopy(moby + 0x10, data + 0xa0);
                if (*(s32 *)(data + 0xd0) != -1)
                    allocate_voice_for_target_entry(*(s32 *)(data + 0xd4), 0, moby);
                if (*(s32 *)(data + 0xd8) != -1)
                    D_L07_00184528[*(s32 *)(data + 0xd8)] = 0;
                moby[0x20] = 1;
            }
        }
        break;
    case 5:
        break;
    }
    fast_vec_add(delta, delta, moby + 0x10);
    update_entity_matrix(data + 0x60, delta, old_rotation, moby + 0x40);
}
