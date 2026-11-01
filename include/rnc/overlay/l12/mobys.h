#ifndef LOMBYTE_RNC_OVERLAY_L12_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L12_MOBYS_H

#include "rnc/overlay/l12/mobys/watcher.h"
#include "types.h"

struct Moby;

void blarg_helicopter_update(struct Moby *) __asm__("FUN_L12_002e9f68");
void blarg_trooper_update(struct Moby *) __asm__("FUN_L12_002e72c0");
void clip_volume_scene_trigger_update(struct Moby *) __asm__("FUN_L12_003093c8");
void crane_arm_update(unsigned char *) __asm__("FUN_L12_002ec1d0");
void cutscene_camera_target_update(struct Moby *) __asm__("FUN_L12_003094a0");
void edwina_update(L16WatchMoby *) __asm__("FUN_L12_002eb570");
void effects_draw_trigger_update(struct Moby *) __asm__("FUN_L12_002e7208");
void helicopter_update(char *) __asm__("FUN_L12_002ebea8");
void help_hints_update(struct Moby *) __asm__("FUN_L12_002ed280");
void homing_mine_update(struct Moby *) __asm__("FUN_L12_00304e00");
void homing_mine_spawner_update(struct Moby *) __asm__("FUN_L12_00307840");
void hoven_turret_update(struct Moby *) __asm__("FUN_L12_00303540");
void jittered_quad_layers_update(struct Moby *) __asm__("FUN_L12_003081b0");
void lamp_update(struct Moby *) __asm__("FUN_L12_0030b6c8");
void oscillating_target_link_update() __asm__("FUN_L12_00302f30");
void planetbuster_maximus_part_update(char *) __asm__("FUN_L12_003069d0");
void platform_update() __asm__("FUN_L12_002e3ed8");
void rocket_update(struct Moby *) __asm__("FUN_L12_002ece00");
void scrolling_texture_layers_update() __asm__("FUN_L12_0030bff0");
void water_reservoir_update(struct Moby *) __asm__("FUN_L12_002ec720");
void level_12_water_tile_update(struct Moby *) __asm__("FUN_L12_002bf140");

#endif /* LOMBYTE_RNC_OVERLAY_L12_MOBYS_H */
