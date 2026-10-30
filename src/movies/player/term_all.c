#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/movies/decoder/decoder_globals.h"
#include "rnc/movies/player/movie_player.h"

extern s32 DeleteThread();
extern s32 RemoveDmacHandler();
extern s32 RemoveIntcHandler();
extern s32 TerminateThread();
extern s32 disable_intc() __asm__("func_00119028");
extern s32 disable_dmac() __asm__("func_001190F8");
extern s32 audio_dec_delete() __asm__("func_0023AC90");
extern s32 mpeg_cd_stream_close(struct MpegCdStream *stream) __asm__("func_0023BA58");
extern s32 video_dec_delete() __asm__("func_0023CC38");
extern s32 sceCdSync();
void       term_all(void) __asm__("FUN_0023aa68");

void term_all(void) {
    sceCdSync(0);
    read_buf_delete(&movie_player->read_buf);
    vo_buf_delete(&movie_player->vo_buf);

    TerminateThread(decoder_thread);
    DeleteThread(decoder_thread);
    disable_dmac(2);
    RemoveDmacHandler(2, movie_player->video_dec.hid_endimage);
    disable_intc(2);
    RemoveIntcHandler(2, movie_player->video_dec.hid_vblank);
    video_dec_delete(&movie_player->video_dec);
    audio_dec_delete(&movie_player->audio_dec);
    mpeg_cd_stream_close(&movie_player->cd_stream);
    sceCdSync(0);
    *(s32 *)0x1000E000 &= 0xFFFFFFFD;
}

extern __typeof__(term_all) func_0023AA68 __attribute__((alias("FUN_0023aa68")));
