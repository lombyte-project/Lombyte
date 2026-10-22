/* Ported from rac1-decomp (src/game/movie/audiodec.c, func_0023C390). */
#include "sda.h"
#include "rnc/audio/decoder/audio_dec.h"
extern unsigned char *D_001612BC MACRO_ADDR;
extern int snd_get_movie_nax(void) __asm__("FUN_0012f178");
extern void send_to_spu(struct AudioDec *, unsigned char *, int, int) __asm__("func_0023AF18");
/* sendADPCM(_AudioDec *): once enough is buffered (state 1: 4 KiB,
   filling the IOP buffer from iopLastPos; state 2: the free space behind
   the SPU position snd_get_movie_nax reports), sends 1 KiB per channel at a
   time: each channel's interleaved blocks are gathered from the ring
   buffer into D_001612BC, the ADPCM loop/start flags are patched at the
   buffer's start (spuPos 0) and end (spuPos 0xC00), and send_to_spu
   sends it to i * 0x1000 + spuPos. The space counter is decremented at
   the end of the body, so it is its own register at the loop test as
   in retail; the header bytes go through a local copy of the buffer
   pointer (one load, as retail). */
void send_adpcm(void *arg0) __asm__("FUN_0023afc0");

void send_adpcm(void *audio_decoder) {
    enum {
        ADPCM_IOP_BUFFER_FILL = 1,
        ADPCM_SPU_POSITION_REFILL = 2
    };
    struct AudioDec *ad = audio_decoder;
    int avail = 0;
    int i, j, copied_byte_count;
    unsigned char *interleaved_channel_read_cursor, *channel_staging_write_cursor;

    switch (ad->state) {
    case ADPCM_IOP_BUFFER_FILL:
        if (ad->count < 0x1000) {
            return;
        }
        avail = 0x1000 - ad->iopLastPos;
        break;
    case ADPCM_SPU_POSITION_REFILL:
        avail = (snd_get_movie_nax() - ad->spuPos) & 0xFFF;
        break;
    case 3:
        return;
    }
    while (avail >= 0x400 && ad->count >= ad->hdr.ch << 10) {
        for (i = 0; i < ad->hdr.ch; i++) {
            interleaved_channel_read_cursor = ad->data + (ad->put - ad->count + ad->size) % ad->size;
            interleaved_channel_read_cursor += i * ad->hdr.interSize;
            channel_staging_write_cursor = D_001612BC;
            copied_byte_count = 0;
            while (copied_byte_count < 0x400) {
                for (j = 0; j < ad->hdr.interSize; j++) {
                    *channel_staging_write_cursor++ = *interleaved_channel_read_cursor++;
                    copied_byte_count++;
                }
                interleaved_channel_read_cursor += ad->hdr.interSize * (ad->hdr.ch - 1);
            }
            if (ad->spuPos + 0x400 == 0x1000) {
                D_001612BC[0x3F1] = 3;
            }
            if (ad->spuPos == 0) {
                unsigned char *adpcm_staging_buffer = D_001612BC;
                adpcm_staging_buffer[1] = 6;
                adpcm_staging_buffer[0x11] = 2;
            }
            send_to_spu(ad, D_001612BC, 0x400, i * 0x1000 + ad->spuPos);
        }
        ad->spuPos = (ad->spuPos + 0x400) % 0x1000;
        ad->count -= ad->hdr.ch << 10;
        ad->iopLastPos += 0x400;
        avail -= 0x400;
    }
}

extern __typeof__(send_adpcm) func_0023AFC0 __attribute__((alias("FUN_0023afc0")));
