#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/sdk/input.h"

#include "rnc/input.h"

void poll_pad_device_state(struct PadState *p) __asm__("FUN_002170c8");

void poll_pad_device_state(struct PadState *pad_state) {
    enum {
        PAD_PROFILE_INITIALIZATION = 0,
        PAD_INPUT_POLLING = 1,
        PAD_BUTTON_PROFILE_TOO_LARGE = 2
    };
    enum {
        PAD_DEVICE_READY = 1
    };
    s32 saved_pad_value;
    s32 pad_device_state;
    s32 byte_count;
    s32 i;
    u8  pad_response_buffer[0x40];

    pad_state->held = 0;
    saved_pad_value = pad_state->raw_held;
    pad_state->raw_held = 0;
    pad_state->prev_held = pad_state->saved_held;
    pad_state->prev_raw_held = saved_pad_value;
    pad_device_state = scePad2GetState(pad_state->socket);
    pad_state->device_state = pad_device_state;
    if (pad_device_state == PAD_DEVICE_READY) {
        switch (pad_state->profile_state) {
        case PAD_PROFILE_INITIALIZATION: {
            byte_count = scePad2GetButtonProfile(pad_state->socket, pad_response_buffer);
            if (*(u32 *)pad_response_buffer == 0xFFFFFFFF) {
                pad_state->unk1DC = 0x79;
            } else {
                pad_state->unk1DC = 0;
            }
            if (byte_count < 5) {
                for (i = 0; i < byte_count; i++) {
                    pad_state->button_profile[i] = pad_response_buffer[i];
                }
                pad_state->profile_state = PAD_INPUT_POLLING;
            } else {
                pad_state->profile_state = PAD_BUTTON_PROFILE_TOO_LARGE;
                byte_count = 0;
            }
            for (i = byte_count; i < 4; i++) {
                pad_state->button_profile[i] = pad_response_buffer[i];
            }
            byte_count = sceVibGetProfile(pad_state->socket, pad_response_buffer);
            if (byte_count < 5) {
                for (i = 0; i < byte_count; i++) {
                    pad_state->vibration_profile[i] = pad_response_buffer[i];
                }
            } else {
                byte_count = 0;
            }
            for (i = byte_count; i < 4; i++) {
                pad_state->vibration_profile[i] = pad_response_buffer[i];
            }
            clear_pad_input(pad_state);
            break;
        }
        case PAD_INPUT_POLLING:
            byte_count = scePad2Read(pad_state->socket, pad_response_buffer);
            process_pad_input(pad_state, pad_response_buffer, byte_count);
            break;
        case PAD_BUTTON_PROFILE_TOO_LARGE:
            clear_pad_input(pad_state);
            break;
        }
    } else {
        clear_pad_input(pad_state);
        pad_state->profile_state = PAD_PROFILE_INITIALIZATION;
    }
    pad_state->unk188 = 0;
    pad_state->unk189 = 0;
}

extern __typeof__(poll_pad_device_state) func_002170C8 __attribute__((alias("FUN_002170c8")));
