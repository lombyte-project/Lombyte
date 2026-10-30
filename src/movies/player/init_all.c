#include "types.h"
#include "asm.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

extern u8 movie_open_error_text[18] __asm__("D_001E8AF0");

struct AudioDecoderThreadArgs {
    u8 pad0[4];
    void (*entry)();
    void *stack;
    s32   stack_size;
    void *gp;
    s32   priority;
    u8    pad18[8];
    s32   gs_tag;
    u8    pad24[0xC];
};

extern u8   D_00166C00;
extern void video_callback() __asm__("func_0023B5F0");
extern void pcm_callback() __asm__("func_0023B728");
extern void vblank_handler() __asm__("func_0023B3D8");

extern void handler_end_image() __asm__("FUN_0023b540");
extern void video_dec_main() __asm__("func_0023CE28");
extern void sceMpegInit(void);
extern s32
video_dec_create(void *, u32, u32, void *, u32, u32, void *, u32) __asm__("FUN_0023cac8");
extern s32 audio_dec_create(void *, void *, u32, u32) __asm__("FUN_0023abd0");
extern s32 video_dec_set_stream(void *, u32, u32, void *, struct ReadBuf *) __asm__("FUN_0023cbd0");
extern s32 CreateThread(struct AudioDecoderThreadArgs *);
extern s32 _StartThread(s32, void *);
extern s32 mpeg_cd_stream_open(struct MpegCdStream *stream,
                               s32                  first_sector,
                               s32                  byte_count) __asm__("func_0023BA48");
extern s32 DebugPrint(void *);
extern s32 AddIntcHandler(s32, void *, s32);
extern s32 enable_intc(s32) __asm__("func_00119090");
extern s32 AddDmacHandler(s32, void *, s32);
extern s32 enable_dmac(s32) __asm__("func_00119160");
s32        init_all(s32 movie_sector, s32 movie_size, s32 callback_context) __asm__("FUN_0023a7c0");
s32        init_all(s32 movie_sector, s32 movie_size, s32 callback_context) __asm__("FUN_0023a7c0");

s32 init_all(s32 movie_sector, s32 movie_size, s32 callback_context) {
    struct AudioDecoderThreadArgs thread_params;
    s32                           thread_id;
    s32                           opened;
    void                         *thread_gp;
    *((volatile s32 *)0x1000E000) |= 3;
    *((volatile s32 *)0x1000E010) = 4;
    read_buf_create(&movie_player->read_buf);
    sceMpegInit();
    video_dec_create(&movie_player->video_dec,
                     decoder_buffer_address + 0x1C85C0,
                     0xEB768,
                     movie_player->vi_buf_data,
                     decoder_buffer_address + 0x1C7180,
                     MOVIE_VI_BUF_BLOCKS,
                     movie_player->vi_buf_ts,
                     MOVIE_VI_BUF_TIMESTAMPS);
    audio_dec_create(&movie_player->audio_dec,
                     movie_player->audio_buf,
                     MOVIE_AUDIO_BUF_SIZE,
                     decoder_buffer_address + 0x1C8190);
    video_dec_set_stream(&movie_player->video_dec, 0, 0, video_callback, &movie_player->read_buf);
    video_dec_set_stream(
        &movie_player->video_dec, 3, callback_context, pcm_callback, &movie_player->read_buf);
    vo_buf_create(&movie_player->vo_buf,
                  (struct VoData *)((decoder_buffer_address & 0x0FFFFFFF) | 0x20000000),
                  (struct VoTag *)(decoder_buffer_address + 0x1A0000),
                  2);
    /* Retail fills only these descriptor fields before CreateThread. */
    thread_gp = &D_00166C00;
    thread_params.stack = movie_player->decoder_stack;
    thread_params.entry = video_dec_main;
    thread_params.stack_size = MOVIE_DECODER_STACK_SIZE;
    thread_params.priority = 1;
    thread_params.gp = thread_gp;
    thread_params.gs_tag = 0;
    thread_id = CreateThread(&thread_params);
    decoder_thread = thread_id;
    _StartThread(thread_id, &movie_player->video_dec);
    if (mpeg_cd_stream_open(&movie_player->cd_stream, movie_sector, movie_size) == 0) {
        opened = 0;
        DebugPrint(movie_open_error_text);
    } else {
        opened = 1;
    }
    movie_player->video_dec.hid_vblank = AddIntcHandler(2, vblank_handler, 0);
    enable_intc(2);
    movie_player->video_dec.hid_endimage = AddDmacHandler(2, handler_end_image, 0);
    enable_dmac(2);
    return opened;
}

extern __typeof__(init_all) func_0023A7C0 __attribute__((alias("FUN_0023a7c0")));

u8 movie_open_error_text[18] = "Can't Open movie\n";
