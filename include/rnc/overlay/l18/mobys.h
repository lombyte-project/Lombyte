#ifndef LOMBYTE_RNC_OVERLAY_L18_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L18_MOBYS_H

#include "types.h"

struct Moby;

void boss_camera_transition_update(struct Moby *) __asm__("FUN_L18_002df608");
void child_carrier_update(char *) __asm__("FUN_L18_002fa728");
void set_group_child_carrier_byte_bc(int, int) __asm__("FUN_L18_002fa888");
void deplanetizer_effects_update(struct Moby *) __asm__("FUN_L18_002f88e8");
void cutscene_joint_trail_update() __asm__("FUN_L18_002fad28");
void deplanetizer_gun_update() __asm__("FUN_L18_002f7ad8");
void destructible_scenery_update(struct Moby *) __asm__("FUN_L18_002e9768");
void drek_boss_damage_ring_update(struct Moby *) __asm__("FUN_L18_002dc260");
void drek_boss_electric_ball_update(struct Moby *) __asm__("FUN_L18_002e9e70");
void drek_boss_ring_projectile_update(struct Moby *) __asm__("FUN_L18_002db460");
void drek_final_boss_robot_update() __asm__("FUN_L18_002f2bf0");
void floating_platform_update(struct Moby *) __asm__("FUN_L18_002d82c0");
void gadgebot_update(unsigned char *) __asm__("FUN_L18_002fbb98");
void hide_moby_and_clear_collision(struct Moby *) __asm__("FUN_L18_002fcd00");
int  update_moby_suck_cannon_state(struct Moby *) __asm__("FUN_L18_002fcbb0");
void grind_rail_junction_update(struct Moby *) __asm__("FUN_L18_002d62e8");
void hidden_deplanetizer_gun_update(struct Moby *) __asm__("FUN_L18_002fa640");
void inert_moby_update() __asm__("FUN_L18_002f7ab0");
void lift_update(struct Moby *) __asm__("FUN_L18_002f2310");
void hide_moby_and_disable_collision(struct Moby *) __asm__("FUN_L18_002d6280");
void drifting_mine_update(struct Moby *) __asm__("FUN_L18_002d5918");
int  moby_hold_state_while_active(struct Moby *) __asm__("FUN_L18_002d6190");
int  update_moby_state_pause(struct Moby *) __asm__("FUN_L18_002d6108");
void rocket_moby_update(struct Moby *) __asm__("FUN_L18_002d5050");
void rocket_tank_update() __asm__("FUN_L18_002f7c40");
void rocket_tank_rocket_update(char *) __asm__("FUN_L18_002a7220");
void thruster_pack_lock_green_update(char *) __asm__("FUN_L18_002d6600");
void thruster_pack_lock_red_update(struct Moby *) __asm__("FUN_L18_002d7b20");
void trespasser_barrier_update(struct Moby *) __asm__("FUN_L18_002f1e68");

#endif /* LOMBYTE_RNC_OVERLAY_L18_MOBYS_H */
