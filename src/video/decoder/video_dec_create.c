#include "types.h"
extern u8 D_0023D080[];
extern u8 D_0023D0A8[];
extern void mpeg_stop_dma() __asm__("FUN_0023d0e0");
extern void FUN_0023d110();
extern u8 D_0023D140[];
extern s32 sceMpegCreate();
extern s32 AddMpegCallback();
extern s32 vi_buf_create() __asm__("func_0023BC48");
extern s32 func_0023CC30();
s32 video_dec_create(s32 video_dec, s32 arg1, s32 arg2, s32 data, s32 tag, s32 size, s32 ts,
                     s32 ts_count) __asm__("FUN_0023cac8");

s32 video_dec_create(s32 video_dec, s32 arg1, s32 arg2, s32 data, s32 tag, s32 size, s32 ts,
                     s32 ts_count) {
    sceMpegCreate(video_dec);
    AddMpegCallback(video_dec, 0, D_0023D080, 0);
    AddMpegCallback(video_dec, 1, D_0023D0A8, 0);
    AddMpegCallback(video_dec, 2, mpeg_stop_dma, 0);
    AddMpegCallback(video_dec, 3, FUN_0023d110, 0);
    AddMpegCallback(video_dec, 5, D_0023D140, 0);
    func_0023CC30(video_dec);
    vi_buf_create(video_dec + 0x48, data, tag, size, ts, ts_count);
    return 1;
}
