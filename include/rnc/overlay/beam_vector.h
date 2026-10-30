#ifndef LOMBYTE_RNC_OVERLAY_BEAM_VECTOR_H
#define LOMBYTE_RNC_OVERLAY_BEAM_VECTOR_H

/* Shared beam point layout; preserve the initializer's float-only representation. */
typedef struct {
    float f[4];
} __attribute__((aligned(16))) V2b9310;

extern float   D_L00_001615AC;
extern float   D_L00_001615B4;
extern float   D_L00_001615C8;
extern int     D_L00_001DB840[];
extern V2b9310 D_L00_001DB890[];
extern int     D_L00_001DB9D0[];

#endif
