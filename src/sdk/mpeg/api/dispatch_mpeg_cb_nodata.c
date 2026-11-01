#include "types.h"
#include "rnc/sdk/mpeg/api.h"

void DispatchMpegCallbackNoData(void *callback) __asm__("_dispatchMpegCbNodata");

void DispatchMpegCallbackNoData(void *callback) {
    u32 callback_data[8];
    callback_data[0] = 1;
    _dispatchMpegCallback(callback, callback_data);
}
