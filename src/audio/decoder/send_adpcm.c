/* Ported from rac1-decomp (src/game/movie/audiodec.c, func_0023C390). */
#include "sda.h"
#include "rnc/audio/decoder/audio_dec.h"
extern unsigned char *D_001612BC MACRO_ADDR;
extern int snd_get_movie_nax(void) __asm__("FUN_0012f178");
extern void send_to_spu(struct AudioDec *, unsigned char *, int, int) __asm__("func_0023AF18");
/* sendADPCM(_AudioDec *): once enough is buffered (state 1: 4 KiB,
   filling the IOP buffer from iopLastPos; state 2: the free space behind
   the SPU position FUN_0012f178 reports), sends 1 KiB per channel at a
   time: each channel's interleaved blocks are gathered from the ring
   buffer into D_001612BC, the ADPCM loop/start flags are patched at the
   buffer's start (spuPos 0) and end (spuPos 0xC00), and func_0023AF18
   sends it to i * 0x1000 + spuPos. The space counter is decremented at
   the end of the body, so it is its own register at the loop test as
   in retail; the header bytes go through a local copy of the buffer
   pointer (one load, as retail). */
void send_adpcm(void *arg0) __asm__("FUN_0023afc0");

void send_adpcm(void *arg0) {
    struct AudioDec *ad = arg0;
    int avail = 0;
    int i, j, n;
    unsigned char *src, *dst;

    switch (ad->state) {
    case 1:
        if (ad->count < 0x1000) {
            return;
        }
        avail = 0x1000 - ad->iopLastPos;
        break;
    case 2:
        avail = (snd_get_movie_nax() - ad->spuPos) & 0xFFF;
        break;
    case 3:
        return;
    }
    while (avail >= 0x400 && ad->count >= ad->hdr.ch << 10) {
        for (i = 0; i < ad->hdr.ch; i++) {
            src = ad->data + (ad->put - ad->count + ad->size) % ad->size;
            src += i * ad->hdr.interSize;
            dst = D_001612BC;
            n = 0;
            while (n < 0x400) {
                for (j = 0; j < ad->hdr.interSize; j++) {
                    *dst++ = *src++;
                    n++;
                }
                src += ad->hdr.interSize * (ad->hdr.ch - 1);
            }
            if (ad->spuPos + 0x400 == 0x1000) {
                D_001612BC[0x3F1] = 3;
            }
            if (ad->spuPos == 0) {
                unsigned char *b = D_001612BC;
                b[1] = 6;
                b[0x11] = 2;
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
