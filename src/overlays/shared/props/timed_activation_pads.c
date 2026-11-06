/* Shared moby class: timed_activation_pads. */
#include "types.h"
#include "qcopy.h"
#include "rnc/game/game_globals.h"
#include "rnc/globals.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "sda.h"
#include "rnc/mobys/moby.h"

typedef struct TimedActivationPadVars {
    u8  pad0[0x4];
    s32 tick_sound_timer;
    s32 pressed;
    s16 glow_timer;
    s16 glow_phase;
    s32 duration_seconds;
} TimedActivationPadVars;

extern s32  allocate_voice_for_target_entry() __asm__("func_0022DA68");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
float       absolute_float(float input) __asm__("func_001F99C0");
void        set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");
extern char D_0013F350_lui[] __asm__("D_0013F350") NOT_SDA;
extern void set_moby_list_state(int list, int state) __asm__("FUN_L01_0026e0e0");
extern void func_L01_0026F040_30D370(int, int) __asm__("FUN_L01_0026e090");
extern int  func_001F9850_30D370(int) __asm__("FUN_001f96f8");
extern void glow_n(void *, int, int, int) __asm__("FUN_L00_002502f0");

/* Ported from rac1-decomp src/overlays/shared/vendor_002F9438.c (func_L07_0030D370) */

void timed_activation_pads_update(unsigned char *moby) __asm__("FUN_L07_0030bf90");

/* Moby class 886 */
void timed_activation_pads_update(unsigned char *moby) {
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
    TimedActivationPadVars *pad_data;
    char                   *activation_reference_data;
    float                   activation_offset[4];
    float                   transposed_pad_rotation[16];
    int                     flag;
    int                     on;
    int                     pad_state;

    if (moby == 0)
        return;
    pad_data = (TimedActivationPadVars *)MOBY(moby)->pvars;
    if (pad_data == 0)
        return;

    flag = 0;
    if (MOBY(moby)->state < PAD_INITIALIZE_FLASHING) {
        if (MOBY(moby)->mission != 0xFF) {
            if (visited52[MOBY(moby)->mission + (current_level_index << 4)] == 0xFF)
                set_moby_list_state(MOBY(moby)->group, 6);
        }
    }

    if (pad_data->pressed == 0) {
        activation_reference_data = (char *)D_0013F350_lui;
        if (*(short *)(activation_reference_data + 0x30E) == 0)
            flag = *(unsigned char **)(activation_reference_data + 0x2FC) == moby;
        if (0.7853982f < absolute_float(MOBY(moby)->rot.y)) {
            if (vector_distance_xy(&MOBY(moby)->pos, activation_reference_data + 0x80) < 2.0f) {
                if (*(int *)(activation_reference_data + 0x2084) == 0x11) {
                    if (*(float *)(activation_reference_data + 0xAA8) < 7.0f) {
                        fast_vec_sub(
                            activation_offset, &MOBY(moby)->pos, activation_reference_data + 0x80);
                        set_identity_matrix(transposed_pad_rotation);
                        transpose_rotation_matrix(transposed_pad_rotation, moby + 0xC0);
                        transform_vector(
                            activation_offset, activation_offset, transposed_pad_rotation);
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

    pad_state = MOBY(moby)->state;
    if ((unsigned int)(pad_state - 2) < 4) {
        if (MOBY(moby)->command == 1) {
            if (fast_dec_timer((int *)pad_data)) {
                allocate_voice_for_target_entry(2, 0, (int)moby);
                set_moby_list_state(MOBY(moby)->group, 8);
            } else if (fast_dec_timer((int *)(&pad_data->tick_sound_timer))) {
                allocate_voice_for_target_entry(0, 0, (int)moby);
                pad_data->tick_sound_timer =
                    func_001F9850_30D370(0x2D) <
                            func_001F9850_30D370(5) + *(int *)pad_data / func_001F9850_30D370(0x28)
                        ? func_001F9850_30D370(0x2D)
                        : func_001F9850_30D370(5) + *(int *)pad_data / func_001F9850_30D370(0x28);
            }
        }
    }

    switch (MOBY(moby)->state) {
    case PAD_INITIALIZE:
        glow_n(moby, 0x80, 0x80, 0x80);
        func_L01_0026F040_30D370(MOBY(moby)->group, 0);
        set_moby_list_state(MOBY(moby)->group, 1);
        break;
    case PAD_WAIT_FOR_ACTIVATION:
        pad_data->pressed = 0;
        glow_n(moby, 0x80, 0x80, 0x80);
        if (flag) {
            if (pad_data->duration_seconds > 0)
                *(int *)pad_data = func_001F9850_30D370(pad_data->duration_seconds * 60);
            else
                *(int *)pad_data = func_001F9850_30D370(900);
            on = 1;
            glow_n(moby, 0, 0xFF, 0);
            pad_data->pressed = on;
            func_L01_0026F040_30D370(MOBY(moby)->group, 0);
            MOBY(moby)->command = on;
            allocate_voice_for_target_entry(3, 0, (int)moby);
            set_moby_list_state(MOBY(moby)->group, 2);
        }
        break;
    case PAD_ADVANCE_TO_FLASH_ACTIVATION_GATE:
        MOBY(moby)->state = PAD_WAIT_TO_ENTER_FLASH_ACTIVATION_WAIT;
        break;
    case PAD_WAIT_TO_ENTER_FLASH_ACTIVATION_WAIT:
        if (flag) {
            pad_data->pressed = 1;
            glow_n(moby, 0, 0xFF, 0);
            allocate_voice_for_target_entry(3, 0, (int)moby);
            set_moby_list_state(MOBY(moby)->group, 4);
        }
        break;
    case PAD_ENTER_FLASH_ACTIVATION_WAIT:
        MOBY(moby)->state = PAD_WAIT_FOR_FLASH_ACTIVATION;
        break;
    case PAD_WAIT_FOR_FLASH_ACTIVATION:
        if (flag) {
            pad_data->pressed = 1;
            glow_n(moby, 0, 0xFF, 0);
            set_moby_list_state(MOBY(moby)->group, 6);
            allocate_voice_for_target_entry(1, 0, (int)moby);
        }
        break;
    case PAD_INITIALIZE_FLASHING:
        pad_data->glow_timer = func_001F9850_30D370(0x14);
        pad_data->glow_phase = 0;
        glow_n(moby, 0x80, 0xFF, 0x80);
        MOBY(moby)->state = PAD_UPDATE_FLASHING;
        break;
    case PAD_UPDATE_FLASHING:
        if (MOBY(moby)->mission != 0xFF) {
            if (visited52[MOBY(moby)->mission + (current_level_index << 4)] != 0xFF)
                jet_clear_persistent(MOBY(moby)->mission);
        }
        if (fast_dec_timer_s16(&pad_data->glow_timer)) {
            pad_data->glow_timer = func_001F9850_30D370(0x14);
            switch (pad_data->glow_phase) {
            case PAD_FLASH_APPLY_GREEN:
                glow_n(moby, 0, 0xFF, 0);
                pad_data->glow_phase = 1;
                break;
            case PAD_FLASH_APPLY_WHITE:
                glow_n(moby, 0xFF, 0xFF, 0xFF);
                pad_data->glow_phase = 0;
                break;
            case 2 ... 5:
                pad_data->glow_phase = 0;
                break;
            }
        }
        break;
    case PAD_RESET_TO_ACTIVATION_WAIT:
        glow_n(moby, 0x80, 0x80, 0x80);
        MOBY(moby)->command = 0;
        MOBY(moby)->state = PAD_WAIT_FOR_ACTIVATION;
        break;
    case 9:
        break;
    }
}
