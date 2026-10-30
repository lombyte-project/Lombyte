#ifndef LOMBYTE_RNC_OVERLAY_L15_MOBYS_CONVEYOR_BELT_GLOBALS_H
#define LOMBYTE_RNC_OVERLAY_L15_MOBYS_CONVEYOR_BELT_GLOBALS_H

#include "types.h"

/* Scalar data read by src/overlays/l15/mobys/conveyor_belt.c. */
/* Explicit _gp views stay in the source at their assembler declaration sites. */
extern float poison_vessel_gravity __asm__("D_L15_00161FBC");
extern float ripple_mesh_buffers_z_amplitude __asm__("D_L15_00162100");
extern float ripple_mesh_buffers_uv_scroll_rate __asm__("D_L15_00162104");
extern float ripple_mesh_uv_scroll_rate __asm__("D_L15_0016216C");
extern float ripple_mesh_radial_wavelength __asm__("D_L15_00162160");
extern float ripple_mesh_z_amplitude __asm__("D_L15_00162168");
extern int   level_save_bank_index __asm__("D_L15_0015ED84");
extern float scaled_ripple_radial_wavelength __asm__("D_L15_001621F0");
extern float scaled_ripple_z_amplitude __asm__("D_L15_001621F8");
extern float scaled_ripple_uv_scroll_rate __asm__("D_L15_001621FC");
extern float scaled_ripple_template_vertex_scale __asm__("D_L15_00162200");
extern float ripple_88_radial_wavelength __asm__("D_L15_00162260");
extern float ripple_88_z_amplitude __asm__("D_L15_00162268");
extern float ripple_88_uv_scroll_rate __asm__("D_L15_0016226C");
extern float ripple_88_template_vertex_scale __asm__("D_L15_00162270");
extern float codebot_slot_effect_spawn_gate __asm__("D_L15_0015F3FC");

#endif /* LOMBYTE_RNC_OVERLAY_L15_MOBYS_CONVEYOR_BELT_GLOBALS_H */
