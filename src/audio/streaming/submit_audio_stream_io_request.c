#include "types.h"
#include "rnc/sdk/libcdvd.h"
#include "rnc/data_labels.h"

extern sceCdRMode cd_read_request_mode_template[] __asm__("D_00151700");
extern s32        sceCdRead(u32, u32, void *, sceCdRMode *);
s32 submit_audio_stream_io_request(void *buf, u32 lsn, u32 sectors) __asm__("FUN_00216728");

s32 submit_audio_stream_io_request(void *read_destination, u32 lsn, u32 sectors) {
    sceCdRMode mode = cd_read_request_mode_template[0];

    mode.spindlctrl = D_0015ED58;
    D_0015EEBC = 0;
    D_0015EEC0 = 0;
    sceCdRead(lsn, sectors, read_destination, &mode);
    return 1;
}

extern __typeof__(submit_audio_stream_io_request) func_00216728
    __attribute__((alias("FUN_00216728")));
