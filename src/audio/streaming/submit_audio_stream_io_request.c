#include "types.h"
#include "rnc/sdk/libcdvd.h"
extern sceCdRMode D_00151700[];
extern u8 D_0015ED58;
extern s32 D_0015EEBC;
extern s32 D_0015EEC0;
extern s32 sceCdRead(u32, u32, void *, sceCdRMode *);
s32 submit_audio_stream_io_request(void *buf, u32 lsn, u32 sectors) __asm__("FUN_00216728");

s32 submit_audio_stream_io_request(void *buf, u32 lsn, u32 sectors) {
    sceCdRMode mode = D_00151700[0];

    mode.spindlctrl = D_0015ED58;
    D_0015EEBC = 0;
    D_0015EEC0 = 0;
    sceCdRead(lsn, sectors, buf, &mode);
    return 1;
}

extern __typeof__(submit_audio_stream_io_request) func_00216728
    __attribute__((alias("FUN_00216728")));
