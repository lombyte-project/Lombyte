#include "types.h"

struct VerticalSyncCallbackState {
    u8 reserved_00[8];
    s32 callback;
    s32 interrupt_handler_id;
};

extern struct VerticalSyncCallbackState *GetCoreDataTable();
extern s32 disable_intc() __asm__("func_00119028");
extern s32 enable_intc() __asm__("func_00119090");
extern s32 RemoveIntcHandler();
extern s32 AddIntcHandler();

s32 sceGsSyncVCallback(s32 callback) {
    s32 previous_callback;
    struct VerticalSyncCallbackState *callback_state;

    callback_state = GetCoreDataTable();
    previous_callback = callback_state->callback;
    if (callback == 0) {
        disable_intc(2);
        RemoveIntcHandler(2, callback_state->interrupt_handler_id);
        callback_state->callback = callback_state->interrupt_handler_id = 0;
    } else {
        if (previous_callback != 0) {
            disable_intc(2);
            RemoveIntcHandler(2, callback_state->interrupt_handler_id);
        }
        callback_state->callback = callback;
        callback_state->interrupt_handler_id = AddIntcHandler(2, callback, -1);
        enable_intc(2);
    }
    return previous_callback;
}
