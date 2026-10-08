#ifndef LOMBYTE_RNC_AUDIO_DECODER_AUDIO_DEC_H
#define LOMBYTE_RNC_AUDIO_DECODER_AUDIO_DEC_H

#include "types.h"

/* _AudioDec (recovered symbols audioDecCreate/Start/Reset/BeginPut/EndPut,
 * sendToSPU, sendADPCM): movie ADPCM audio buffered on the EE and sent to
 * an IOP buffer.  The stream starts with a 0x28-byte header (hdr + body),
 * collected through BeginPut/EndPut before data goes to the ring buffer.
 * Offset checks: src/check/audio_dec_layout_check.c. */

struct SpuStreamHeader {
    char id[4];
    s32 size;
    s32 type;
    s32 rate;                   /* passed to snd_start_movie_sound */
    s32 ch;                     /* channel count; passed to snd_start_movie_sound */
    s32 interSize;              /* interleave block size (sendADPCM) */
    s32 loopStart;
    s32 loopEnd;
};

struct AudioDec {
    s32 state;                  /* 0 header, 1 header done, 2 started */
    s32 strType;                /* 3 at create; 4 = no header */
    struct SpuStreamHeader hdr;
    char body[8];               /* SpuStreamBody */
    s32 hdrCount;               /* header bytes received (of 0x28) */

    /* EE ring buffer */
    u8 *data;
    s32 put;                    /* write offset */
    s32 count;                  /* bytes buffered */
    s32 size;                   /* rounded down to 1 KiB by EndPut */
    s32 totalBytes;

    /* IOP side */
    s32 iopBuff;                /* snd_init_movie_sound result; sendToSPU DMA target */
    s32 iopBuffSize;            /* 0x400 at create */
    s32 iopLastPos;
    s32 iopPausePos;
    s32 totalBytesSent;
    s32 iopZero;                /* passed to snd_start_movie_sound */
    s32 spuPos;                 /* 0 or 0xC00 in sendADPCM */
};

#endif
