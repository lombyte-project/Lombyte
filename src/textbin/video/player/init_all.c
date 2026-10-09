#include "types.h"
#include "asm.h"

#include "types.h"

extern u8 movie_open_error_text[18] __asm__("D_001E8AF0");

struct AudioDecoderState {
    u8 pad0[0xD90F8];
    s32 dmac_handler_id;
    s32 intc_handler_id;
};
struct AudioDecoderThreadArgs {
    u8 pad0[4];
    void (*entry)();
    void *stack;
    s32 stack_size;
    void *gp;
    s32 priority;
    u8 pad18[8];
    s32 gs_tag;
    u8 pad24[0xC];
};
extern s32 decoder_buffer_address __asm__("D_00161208");
extern struct AudioDecoderState *decoder_state __asm__("D_0016120C");
extern s32 decoder_thread_id __asm__("D_00161210");
extern u8 D_00166C00;
extern void video_callback() __asm__("func_0023B5F0");
extern void pcm_callback() __asm__("func_0023B728");
extern void func_0023B3D8();

extern void handler_end_image() __asm__("FUN_0023b540");
extern void video_dec_main() __asm__("func_0023CE28");
extern s32 func_0023B940(struct AudioDecoderState *);
extern void sceMpegInit(void);
extern s32 video_dec_create(void *, u32, u32, void *, u32, u32, void *,
                            u32) __asm__("FUN_0023cac8");
extern s32 audio_dec_create(void *, void *, u32, u32) __asm__("FUN_0023abd0");
extern s32 video_dec_set_stream(void *, u32, u32, void *,
                                struct AudioDecoderState *) __asm__("FUN_0023cbd0");
extern void vo_buf_create(void *, u32, u32, u32) __asm__("func_0023D190");
extern s32 CreateThread(struct AudioDecoderThreadArgs *);
extern s32 _StartThread(s32, void *);
extern s32 func_0023BA48(void *, s32, s32);
extern s32 DebugPrint(void *);
extern s32 AddIntcHandler(s32, void *, s32);
extern s32 enable_intc(s32) __asm__("func_00119090");
extern s32 AddDmacHandler(s32, void *, s32);
extern s32 enable_dmac(s32) __asm__("func_00119160");
s32 init_all(s32 stream_source, s32 source_mode,
                            s32 callback_context) __asm__("FUN_0023a7c0");
s32 init_all(s32 stream_source, s32 source_mode,
                            s32 callback_context) __asm__("FUN_0023a7c0");

s32 init_all(s32 stream_source, s32 source_mode, s32 callback_context) {
    struct AudioDecoderThreadArgs decoder_thread;
    s32 thread_id;
    s32 opened;
    void *thread_gp;
    *((volatile s32 *)0x1000E000) |= 3;
    *((volatile s32 *)0x1000E010) = 4;
    func_0023B940(decoder_state);
    sceMpegInit();
    video_dec_create(((u8 *)decoder_state) + 0xD9048, decoder_buffer_address + 0x1C85C0, 0xEB768,
                     ((u8 *)decoder_state) + 0x52040, decoder_buffer_address + 0x1C7180, 0x100,
                     ((u8 *)decoder_state) + 0xD6040, 0x200);
    audio_dec_create(((u8 *)decoder_state) + 0xD9100, ((u8 *)decoder_state) + 0x50040, 0x2000,
                     decoder_buffer_address + 0x1C8190);
    video_dec_set_stream(((u8 *)decoder_state) + 0xD9048, 0, 0, video_callback, decoder_state);
    video_dec_set_stream(((u8 *)decoder_state) + 0xD9048, 3, callback_context, pcm_callback,
                         decoder_state);
    vo_buf_create(((u8 *)decoder_state) + 0xD9168,
                  (decoder_buffer_address & 0x0FFFFFFF) | 0x20000000,
                  decoder_buffer_address + 0x1A0000, 2);
    /* Retail fills only these descriptor fields before CreateThread. */
    thread_gp = &D_00166C00;
    decoder_thread.stack = ((u8 *)decoder_state) + 0xD2040;
    decoder_thread.entry = video_dec_main;
    decoder_thread.stack_size = 0x4000;
    decoder_thread.priority = 1;
    decoder_thread.gp = thread_gp;
    decoder_thread.gs_tag = 0;
    thread_id = CreateThread(&decoder_thread);
    decoder_thread_id = thread_id;
    _StartThread(thread_id, ((u8 *)decoder_state) + 0xD9048);
    if (func_0023BA48(((u8 *)decoder_state) + 0xD9040, stream_source, source_mode) == 0) {
        opened = 0;
        DebugPrint(movie_open_error_text);
    } else {
        opened = 1;
    }
    decoder_state->intc_handler_id = AddIntcHandler(2, func_0023B3D8, 0);
    enable_intc(2);
    decoder_state->dmac_handler_id = AddDmacHandler(2, handler_end_image, 0);
    enable_dmac(2);
    return opened;
}
extern __typeof__(init_all) func_0023A7C0 __attribute__((alias("FUN_0023a7c0")));

u8 movie_open_error_text[18] = "Can't Open movie\n";
