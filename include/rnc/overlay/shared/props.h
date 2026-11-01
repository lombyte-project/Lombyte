#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_H

#include "types.h"

struct Moby;

void           bolt_crank_update(struct Moby *) __asm__("FUN_L01_002e0c68");
void           bot_door_lock_update(char *) __asm__("FUN_L06_00307d30");
void           breakable_lamp_update(struct Moby *) __asm__("FUN_L05_0031c8e0");
void           prop_breakup_update(char *) __asm__("FUN_L06_00309860");
void           directional_spark_emitter_update(char *) __asm__("FUN_L08_002f5830");
void           electric_beam_update(unsigned char *) __asm__("FUN_L14_003015d0");
void           electric_water_update(struct Moby *) __asm__("FUN_L15_002d6810");
void           draw_electric_water_bolt_effect(char *) __asm__("FUN_L15_002d8710");
void           energy_sphere_update(struct Moby *) __asm__("FUN_L06_002f7ab8");
void           energy_sphere_holder_update(char *) __asm__("FUN_L06_002f7288");
void           escalator_update(char *) __asm__("FUN_L02_002dc6b0");
void           flickering_point_light_update(u8 *) __asm__("FUN_L01_0030b618");
void           floor_button_update(struct Moby *) __asm__("FUN_L05_0030c5b0");
void           garage_door_update(struct Moby *) __asm__("FUN_L03_002dca30");
void           grindrail_barrier_update(struct Moby *) __asm__("FUN_L16_002e5708");
void           moby_rotation_group_update() __asm__("FUN_L06_002f4f00");
void           hoverboard_ring_update(char *) __asm__("FUN_L05_0031abe0");
void           hydrodisplacer_update() __asm__("FUN_L00_002df9a0");
void           hydrodisplacer_faucet_update(char *) __asm__("FUN_L05_002f8080");
void           set_moby_state_two(struct Moby *) __asm__("FUN_L06_00303150");
void           initialize_energy_sphere_holder_vars(char *) __asm__("FUN_L06_002f7548");
void           moby_wobble_update(unsigned char *) __asm__("FUN_L05_0030c0a8");
void           linked_spark_emitter_update(unsigned char *) __asm__("FUN_L15_002a2bf0");
void           moby_spatial_trigger_update(struct Moby *) __asm__("FUN_L05_002f5200");
void           novalis_elevator_update(unsigned char *) __asm__("FUN_L01_002b9eb0");
void           ocean_pokitaru_update() __asm__("FUN_L11_0030b358");
void           particle_spawner_smoke_fire_update() __asm__("FUN_L00_002d6f28");
void           red_plant_update(unsigned char *) __asm__("FUN_L00_002e3ca8");
void           pterodactyl_update(unsigned char *) __asm__("FUN_L00_002e3a88");
void           red_orb_update(struct Moby *) __asm__("FUN_L00_002df4f8");
void           respawn_volume_update(char *) __asm__("FUN_L06_002f7930");
void           ripple_strips_moby_update(struct Moby *) __asm__("FUN_L15_002eb0a0");
void           falling_spinning_moby_update(struct Moby *) __asm__("FUN_L06_00300b90");
unsigned char *spawn_spinning_moby_fragment(char *, float *, float *) __asm__("FUN_L06_00300c60");
void           statue_update(unsigned char *) __asm__("FUN_L03_002d3cd8");
void           swingshot_versatarget_update(char *) __asm__("FUN_L02_002e0450");
void           timed_activation_pads_update(unsigned char *) __asm__("FUN_L07_0030bf90");
void           trackdrive_left_update(char *) __asm__("FUN_L02_002cb978");
void           tresspasser_pad_update() __asm__("FUN_L02_002d8ad0");
void           tumbling_part_update(char *) __asm__("FUN_L00_002e3d88");
void           underwater_door_update(struct Moby *) __asm__("FUN_L15_002bddb0");
void           underwater_ring_update(struct Moby *) __asm__("FUN_L15_002e5958");
void           updraft_vent_update(unsigned char *) __asm__("FUN_L10_002e9028");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_H */
