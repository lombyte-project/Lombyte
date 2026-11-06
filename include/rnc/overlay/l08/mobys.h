#ifndef LOMBYTE_RNC_OVERLAY_L08_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L08_MOBYS_H

#include "rnc/sdk/eetypes.h"
#include "types.h"

struct Moby;

void           batalia_cannon_update() __asm__("FUN_L08_002e0328");
void           batalia_deserter_update() __asm__("FUN_L08_00305270");
void           batalia_textured_effect_update(struct Moby *) __asm__("FUN_L08_003097b0");
void           blarg_bombthrower_update(struct Moby *) __asm__("FUN_L08_002dabf0");
void           bolt_pickup_update() __asm__("FUN_L08_002b82a0");
void           bolt_thief_update(struct Moby *) __asm__("FUN_L08_002e2df0");
void           blarg_bombthrower_bomb_update(char *) __asm__("FUN_L08_002d23f0");
void           bomber_tank_update() __asm__("FUN_L08_002d42d8");
void           commando_update() __asm__("FUN_L08_00302ce8");
void           cutscene_camera_target_update(struct Moby *) __asm__("FUN_L08_003084d8");
void           effect_emitter_update(struct Moby *) __asm__("FUN_L08_00308c90");
void           falling_debris_update() __asm__("FUN_L08_002e16c0");
void           flickering_mesh_effect_update(struct Moby *) __asm__("FUN_L08_002f70a0");
void           gunship_update(struct Moby *) __asm__("FUN_L08_002e24e8");
void           heavy_gunship_update(struct Moby *) __asm__("FUN_L08_002e9b70");
void           help_hints_update(struct Moby *) __asm__("FUN_L08_00307540");
void           landmine_update(struct Moby *) __asm__("FUN_L08_002f44c0");
void           layered_mesh_effect_update(struct Moby *) __asm__("FUN_L08_002f1548");
void           lift_update(char *) __asm__("FUN_L08_002ea398");
void           path_following_moby_update(char *) __asm__("FUN_L08_002de848");
void           shattering_prop_update(struct Moby *) __asm__("FUN_L08_0030bc00");
void           shrub_update(struct Moby *) __asm__("FUN_L08_002e8cd0");
void           shuttle_update(struct Moby *) __asm__("FUN_L08_002dbdb0");
unsigned char *spawn_moby_offset_effect(char *) __asm__("FUN_L08_00279f00");
void spawn_effect_with_initial_motion(u128 *, u128 *, int, float) __asm__("FUN_L08_0027fe00");
void swing_gate_update(struct Moby *) __asm__("FUN_L08_002ea4c0");
void the_plumber_batalia_update() __asm__("FUN_L08_003065d8");
void water_surface_update(struct Moby *) __asm__("FUN_L08_002da0f0");

#endif /* LOMBYTE_RNC_OVERLAY_L08_MOBYS_H */
