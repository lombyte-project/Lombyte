#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_H

#include "types.h"

struct Moby;
struct RailSpawnerMoby;

void big_al_update(struct Moby *) __asm__("FUN_L03_002db558");
void kerwan_cutscene_effects_update(struct Moby *) __asm__("FUN_L03_002e01d0");
void helga_update(struct Moby *) __asm__("FUN_L03_002da870");
void help_hints_update(struct Moby *) __asm__("FUN_L03_002df520");
void kerwan_path_enemy_update() __asm__("FUN_L03_002c6fd0");
void lightpost_update(struct Moby *) __asm__("FUN_L03_002e2948");
void metropolis_train_carriage_update(struct Moby *) __asm__("FUN_L03_002953f8");
void metropolis_train_locomotive_update(char *) __asm__("FUN_L03_00292e98");
void mine_layer_mine_update(struct Moby *) __asm__("FUN_L03_002cbea8");
void mine_layer_update(struct Moby *) __asm__("FUN_L03_002c9eb8");
void moving_platform_small_update(char *) __asm__("FUN_L03_00294c08");
void path_mover_update(struct Moby *) __asm__("FUN_L03_002db020");
void path_mover_spawner_update(struct RailSpawnerMoby *) __asm__("FUN_L03_002db280");
void rail_lift_update(struct Moby *) __asm__("FUN_L03_002dccc0");
void red_ship_1_update(char *) __asm__("FUN_L03_0029dba8");
void robo_shack_sign_update(struct Moby *) __asm__("FUN_L03_002d3e58");
void robot_qwark_update(struct Moby *) __asm__("FUN_L03_002dbd90");

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_H */
