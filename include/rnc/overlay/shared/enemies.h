#ifndef LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_H
#define LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_H

#include "types.h"

struct Moby;

void  armed_transport_update(char *) __asm__("FUN_L16_002e22d8");
void  armed_transport_rocket_update(char *) __asm__("FUN_L16_002a0a98");
void  billboard_projectile_update(char *) __asm__("FUN_L01_002c7530");
void  blarg_commander_update(char *) __asm__("FUN_L12_00308918");
void  blarg_elite_commando_update(char *) __asm__("FUN_L16_002d2cf0");
void  blarg_gun_update(char *) __asm__("FUN_L05_003186f8");
void  helicopter_spark_update() __asm__("FUN_L03_002d47c0");
void  blarg_paratrooper_update(char *) __asm__("FUN_L08_00301158");
void  emit_moby_sprite_particles(char *) __asm__("FUN_L10_00298940");
void  blarg_ship_rocket_update(char *) __asm__("FUN_L10_002cc2e8");
char *spawn_blarg_ship_shot(int, float *, float *, float, float, float) __asm__("FUN_L11_00308f48");
void  blarg_ship_shot_update(char *) __asm__("FUN_L11_00309098");
void  blarg_space_commando_update(struct Moby *) __asm__("FUN_L06_002fdbd0");
void  blarg_trooper_projectile_update(struct Moby *) __asm__("FUN_L04_002af058");
void  bolt_thief_mine_update(struct Moby *) __asm__("FUN_L14_002ff640");
void  bomb_spawner_update(struct Moby *) __asm__("FUN_L03_002bef68");
void  bomb_spawner_bomb_update(struct Moby *) __asm__("FUN_L03_002c4718");
s32   cancel_moby_suction_and_save_state(unsigned char *) __asm__("FUN_L06_002ea7c8");
void  frog_update(unsigned char *) __asm__("FUN_L06_002e8678");
int   update_moby_grab_state(struct Moby *) __asm__("FUN_L06_002ea5e0");
int   update_moby_suck_cannon_storage_state(unsigned char *) __asm__("FUN_L06_002ea6a8");
void  get_moby_data_plus_0x60() __asm__("FUN_L01_002f1df8");
void  ground_melee_enemy_update() __asm__("FUN_L09_002e27d8");
int   moby_hold_state_if_grabbed(struct Moby *) __asm__("FUN_L01_002f1c78");
int   run_moby_helper_and_set_state_7(struct Moby *) __asm__("FUN_L01_002f1dc8");
void  hero_seeking_mine_update(char *) __asm__("FUN_L15_002a2488");
int   cancel_suck_cannon_suction_and_set_moby_state_9(char *) __asm__("FUN_L00_002d57d0");
void  get_moby_private_data_d0_address() __asm__("FUN_L00_002d5800");
void  horny_toad_update() __asm__("FUN_L00_002d4610");
int   process_suck_cannon_suction_and_update_moby_state(char *) __asm__("FUN_L00_002d56e0");
void  collect_moby_and_clear_path_keys(struct Moby *) __asm__("FUN_L00_002d5830");
int   cancel_suck_cannon_suction_and_set_state_14(struct Moby *) __asm__("FUN_L01_002efb98");
void  get_moby_data_plus_0xc0() __asm__("FUN_L01_002efbc8");
void  large_amoeboid_update(unsigned char *) __asm__("FUN_L01_002edca0");
int   moby_hold_state_if_found(struct Moby *) __asm__("FUN_L01_002efaf8");
int   process_amoeboid_suck_cannon_suction(struct Moby *) __asm__("FUN_L01_002efa88");
void  fire_bolt_update(char *) __asm__("FUN_L08_00307298");
void  sand_mouse_update(struct Moby *) __asm__("FUN_L01_002e3f20");
void  sandmouse_update() __asm__("FUN_L01_0030df40");
void  sandmouse_house_update(unsigned char *) __asm__("FUN_L01_002f2b68");
void  sandmouse_projectile_update(char *) __asm__("FUN_L01_0030c9a8");
void *enter_scoutbot_return_home_state(struct Moby *) __asm__("FUN_L08_002d4050");
void  scoutbot_update() __asm__("FUN_L08_002d2af0");
void  seeker_update(struct Moby *) __asm__("FUN_L09_002c5990");
void  sentrybot_update(unsigned char *) __asm__("FUN_L15_002979d8");
void  set_sentrybot_alarm_duration(char *, int) __asm__("FUN_L15_002cbac0");
void  mark_moby_killed_and_release_owned_voice(int) __asm__("FUN_L15_002a35d8");
void  sentrybot_alarm_update(unsigned char *) __asm__("FUN_L15_002cb4c8");
void  sentrybot_projectile_update(unsigned char *) __asm__("FUN_L15_002e4380");
void  smoke_trail_projectile_update(char *) __asm__("FUN_L01_002eda00");
void  spawner_update(char *) __asm__("FUN_L09_0030ab80");
void  spiderbot_update(unsigned char *) __asm__("FUN_L15_002a6e68");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_ENEMIES_H */
