#ifndef LOMBYTE_RNC_OVERLAY_CUBOID_H
#define LOMBYTE_RNC_OVERLAY_CUBOID_H

#include "types.h"
#include "rnc/math/vector.h"

/* A 0x80-byte cuboid of the level's cuboid table: three axes scaled to its
   size, its position, the basis that takes a point into its space, and its
   rotation. A point offset from pos and taken through to_local lies in
   [-1, 1] on each axis inside the cuboid. Cameras, lifts and taxi stops take
   their position and rotation from one. */
typedef struct {
    Vec4f axes[3];     /* 0x00 */
    Vec4f pos;         /* 0x30 */
    Vec4f to_local[3]; /* 0x40 */
    Vec4f rot;         /* 0x70: z is the yaw */
} LevelCuboid;

/* Each level's cuboid table, indexed by the cuboid id a moby stores. */
extern LevelCuboid *l01_cuboids __asm__("D_L01_001600EC");
extern LevelCuboid *l02_cuboids __asm__("D_L02_001600EC");
extern LevelCuboid *l03_cuboids __asm__("D_L03_001600EC");
extern LevelCuboid *l04_cuboids __asm__("D_L04_001600EC");
extern LevelCuboid *l05_cuboids __asm__("D_L05_001600EC");
extern LevelCuboid *l06_cuboids __asm__("D_L06_001600EC");
extern LevelCuboid *l07_cuboids __asm__("D_L07_001600EC");
extern LevelCuboid *l10_cuboids __asm__("D_L10_001600EC");
extern LevelCuboid *l11_cuboids __asm__("D_L11_001600EC");
extern LevelCuboid *l13_cuboids __asm__("D_L13_001600EC");
extern LevelCuboid *l14_cuboids __asm__("D_L14_001600EC");

#endif /* LOMBYTE_RNC_OVERLAY_CUBOID_H */
