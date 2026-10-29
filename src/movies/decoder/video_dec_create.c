#include "types.h"
#include "rnc/movies/decoder/video_dec.h"

extern u8   D_0023D080[];
extern u8   mpeg_decoder_callback[] __asm__("D_0023D0A8");
extern void mpeg_stop_dma() __asm__("FUN_0023d0e0");
extern void mpeg_restart_video_dma() __asm__("FUN_0023d110");
extern u8   D_0023D140[];
extern s32  sceMpegCreate();
extern s32  AddMpegCallback();
extern s32  vi_buf_create() __asm__("func_0023BC48");
s32         video_dec_create(struct VideoDec *vd,
                             s32              mpeg_work_area,
                             s32              mpeg_work_area_size,
                             s32              data,
                             s32              tag,
                             s32              size,
                             s32              ts,
                             s32              ts_count) __asm__("FUN_0023cac8");

s32 video_dec_create(struct VideoDec *vd,
                     s32              mpeg_work_area,
                     s32              mpeg_work_area_size,
                     s32              video_input_buffer,
                     s32              tag,
                     s32              size,
                     s32              ts,

                     s32 ts_count) {
    sceMpegCreate(vd);
    AddMpegCallback(vd, 0, D_0023D080, 0);
    AddMpegCallback(vd, 1, mpeg_decoder_callback, 0);
    AddMpegCallback(vd, 2, mpeg_stop_dma, 0);
    AddMpegCallback(vd, 3, mpeg_restart_video_dma, 0);
    AddMpegCallback(vd, 5, D_0023D140, 0);
    video_dec_reset(vd);
    vi_buf_create(&vd->vi_buf, video_input_buffer, tag, size, ts, ts_count);
    return 1;
}
