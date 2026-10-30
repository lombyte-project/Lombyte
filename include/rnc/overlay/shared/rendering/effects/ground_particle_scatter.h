#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GROUND_PARTICLE_SCATTER_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GROUND_PARTICLE_SCATTER_H

/* The camera record 0x140 bytes before the camera target position (D_L08_001675C0):
   the ground particle scatter and the ground height voice read its heights. */
typedef struct {
    char  pad0[0x148];
    float reference_height; /* 0x148 */
    char  pad14C[8];
    float f154;
    float f158;
} GroundScatterCamera;

/* Spawn vector bias the scatter randomizes; the radial particle emitter reuses it. */
extern float ground_particle_scatter_spawn_vector_xy_bias[2] __asm__("D_L08_00162360");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_GROUND_PARTICLE_SCATTER_H */
