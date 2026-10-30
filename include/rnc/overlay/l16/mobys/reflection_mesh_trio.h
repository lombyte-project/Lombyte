#ifndef LOMBYTE_RNC_OVERLAY_L16_MOBYS_REFLECTION_MESH_TRIO_H
#define LOMBYTE_RNC_OVERLAY_L16_MOBYS_REFLECTION_MESH_TRIO_H

#include "types.h"
#include "rnc/math/vector.h"

/* Declarations reflection_mesh_trio.c shares with other files of its level. */
void compute_level16_reflection_map_coordinates(int    point_count,
                                                Vec3  *direction_vectors,
                                                Vec3  *point_positions,
                                                int    unused,
                                                float *out) __asm__("FUN_L16_002e8b98");

void env_map_a_draw(void) __asm__("FUN_L16_002e8d40");

#endif /* LOMBYTE_RNC_OVERLAY_L16_MOBYS_REFLECTION_MESH_TRIO_H */
