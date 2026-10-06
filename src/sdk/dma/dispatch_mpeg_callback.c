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
    u8 pad0[0xC];
    struct MpegCallback entries[1];
};
struct MpegContext {
    u8 pad0[0x40];
    struct MpegCallbacks *callbacks;
};

s32 _dispatchMpegCallback(struct MpegContext *context, struct MpegEvent *event) {
    struct MpegCallbacks *callbacks;
    s32 (*callback)(struct MpegContext *, struct MpegEvent *, void *);
    s32 result;
    result = 0;
    if (context != 0) {
        callbacks = context->callbacks;
        if (callbacks != 0) {
            callback = callbacks->entries[event->type].function;
            if (callback != 0) {
                result = callback(context, event, callbacks->entries[event->type].data);
            }
        }
    }
    return result;
}
