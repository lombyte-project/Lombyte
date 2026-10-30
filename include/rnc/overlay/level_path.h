#ifndef LOMBYTE_RNC_OVERLAY_LEVEL_PATH_H
#define LOMBYTE_RNC_OVERLAY_LEVEL_PATH_H

#include "types.h"
#include "rnc/math/vector.h"

/* A path from the level's path table: count 16-byte points after a 16-byte
   header. The taxi reads w of a point as the length of the segment to the next. */
typedef struct {
    s32  count;
    u8   pad4[0xC];
    Vec4 points[1];
} LevelPath;

/* Each level's path table, indexed by the path id a moby stores. */
extern LevelPath *l01_paths[] __asm__("D_L01_001B0930");
extern LevelPath *l03_paths[] __asm__("D_L03_001B05B0");
extern LevelPath *l04_paths[] __asm__("D_L04_001B0630");
extern LevelPath *l05_paths[] __asm__("D_L05_001B0930");
extern LevelPath *l08_paths[] __asm__("D_L08_001B0CB0");
extern LevelPath *l09_paths[] __asm__("D_L09_001B0630");
extern LevelPath *l10_paths[] __asm__("D_L10_001B0930");
extern LevelPath *l11_paths[] __asm__("D_L11_001B0EB0");
extern LevelPath *l12_paths[] __asm__("D_L12_001B0930");
extern LevelPath *l14_paths[] __asm__("D_L14_001B0BB0");

#endif /* LOMBYTE_RNC_OVERLAY_LEVEL_PATH_H */
