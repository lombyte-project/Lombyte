#include "types.h"
#include "rnc/input/pad_state.h"
#include "rnc/sdk/input/dbc.h"

typedef struct {
    s32 port;
    s32 slot;
    s32 number;
    u8  reserve[4];
} scePad2SocketParam;

scePad2SocketParam controller_socket_params __asm__("D_001CD760") = {0};

extern s32 scePad2Init(s32);
extern s32 scePad2CreateSocket(scePad2SocketParam *, void *);
void       init_pads(void) __asm__("FUN_00217048");

void init_pads(void) {
    enum {
        CONTROLLER_PROFILE_INITIAL_STATE = 0,
        CONTROLLER_DEVICE_INITIAL_STATE = 0
    };
    sceDbcInit();
    scePad2Init(0);
    controller_socket_params.port = 2;
    controller_socket_params.slot = 0;
    controller_state.socket = scePad2CreateSocket(&controller_socket_params, &controller_state);
    controller_socket_params.port = 2;
    controller_socket_params.slot = 1;
    controller_state.profile_state = CONTROLLER_PROFILE_INITIAL_STATE;
    controller_state.device_state = CONTROLLER_DEVICE_INITIAL_STATE;
}

extern __typeof__(init_pads) func_00217048 __attribute__((alias("FUN_00217048")));
