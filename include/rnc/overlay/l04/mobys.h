#ifndef LOMBYTE_RNC_OVERLAY_L04_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L04_MOBYS_H

#include "types.h"

struct Moby;

void big_axebot_update() __asm__("FUN_L04_002ba520");
void blarg_trooper_update(struct Moby *) __asm__("FUN_L04_002c4850");
void butterfly_swarm_update(struct Moby *) __asm__("FUN_L04_0029ecf8");
void circular_saw_enemy_update() __asm__("FUN_L04_002c2270");
void get_circular_saw_enemy_suck_cannon_record() __asm__("FUN_L04_002c41b0");
int  launch_circular_saw_enemy_from_suck_cannon(struct Moby *,
                                                int,
                                                int,
                                                float) __asm__("FUN_L04_002c40d0");
void cutscene_camera_target_update(struct Moby *) __asm__("FUN_L04_002e53a0");
void eudora_bridge_update(struct Moby *) __asm__("FUN_L04_002c6858");
void floating_platform_update(char *) __asm__("FUN_L04_002cdda0");
void help_hints_update() __asm__("FUN_L04_002e4418");
void leaf_update(struct Moby *) __asm__("FUN_L04_002d7e90");
void lieutenant_update(struct Moby *) __asm__("FUN_L04_002e3078");
void path_progress_mover_update(char *) __asm__("FUN_L04_002d6f68");
void pipe_update(struct Moby *) __asm__("FUN_L04_002e79a8");
void platform_booster_update(struct Moby *) __asm__("FUN_L04_002c6bb8");
void random_interval_animation_update(struct Moby *) __asm__("FUN_L04_002a5a08");
void snow_plow_update(struct Moby *) __asm__("FUN_L04_002ca420");
void suck_cannon_update(struct Moby *) __asm__("FUN_L04_002e1a10");
void translation_delta_matrix_update(char *) __asm__("FUN_L04_002d3580");
void transport_ship_update(void *) __asm__("FUN_L04_002ce060");
void upper_door_update(struct Moby *) __asm__("FUN_L04_002e17d8");
void woodcut_bot_update() __asm__("FUN_L04_002d16b8");
void woodcut_bot_debris_update() __asm__("FUN_L04_002e5178");

#endif /* LOMBYTE_RNC_OVERLAY_L04_MOBYS_H */
