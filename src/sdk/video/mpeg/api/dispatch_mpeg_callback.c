#include "types.h"

struct MpegContext;

struct MpegEvent {
    s32 type;
};

struct MpegCallback {
    s32 (*function)(struct MpegContext *, struct MpegEvent *, void *);
    void *data;
};

struct MpegCallbacks {
    u8                  pad0[0xC];
    struct MpegCallback entries[1];
};

struct MpegContext {
    u8                    pad0[0x40];
    struct MpegCallbacks *callbacks;
};

s32 _dispatchMpegCallback(struct MpegContext *callback_context, struct MpegEvent *event) {
    struct MpegCallbacks *callbacks;
    s32 (*callback)(struct MpegContext *, struct MpegEvent *, void *);
    s32 callback_result;
    callback_result = 0;
    if (callback_context != 0) {
        callbacks = callback_context->callbacks;
        if (callbacks != 0) {
            callback = callbacks->entries[event->type].function;
            if (callback != 0) {
                callback_result =
                    callback(callback_context, event, callbacks->entries[event->type].data);
            }
        }
    }
    return callback_result;
}
