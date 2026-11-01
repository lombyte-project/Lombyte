#ifndef LOMBYTE_RNC_OVERLAY_L17_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L17_MOBYS_H

#include "types.h"

struct Moby;

void  blarg_ship_update(struct Moby *) __asm__("FUN_L17_002f40d8");
char *try_activate_blarg_ship_from_list(int) __asm__("FUN_L17_002f4fb8");
void  breakaway_prop_update(struct Moby *) __asm__("FUN_L17_002f5390");
void  fleet_camera_box_watcher_update(struct Moby *) __asm__("FUN_L17_002f26d0");
void  cutscene_camera_target_update(struct Moby *) __asm__("FUN_L17_002f2e78");
void  door_update(struct Moby *) __asm__("FUN_L17_002e87d8");
void  dreks_fleet_turret_update(struct Moby *) __asm__("FUN_L17_002cb310");
void  fleet_turret_shot_update(struct Moby *) __asm__("FUN_L17_002e8e08");
void  fighter_jet_update() __asm__("FUN_L17_002ed018");
void  fleet_backdrop_animation_update(struct Moby *) __asm__("FUN_L17_002f3848");
void  fleet_beam_update(struct Moby *) __asm__("FUN_L17_002a8da0");
void  fleet_rotating_radial_effect_update(char *) __asm__("FUN_L17_002d77f0");
void  floating_mine_update(struct Moby *) __asm__("FUN_L17_002dc3f8");
void  magnet_elevator_update(struct Moby *) __asm__("FUN_L17_002ee2b0");
int   cancel_suck_cannon_suction_and_set_state_12(struct Moby *) __asm__("FUN_L17_002f0040");
int   enemy_stun_update(struct Moby *) __asm__("FUN_L17_002efea8");
void  robot_dog_update(unsigned char *) __asm__("FUN_L17_002eeb68");
void  shattering_prop_update(struct Moby *) __asm__("FUN_L17_002f5260");
void  shuttle_update(struct Moby *) __asm__("FUN_L17_002f1940");

#endif /* LOMBYTE_RNC_OVERLAY_L17_MOBYS_H */
