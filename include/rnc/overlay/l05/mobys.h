#ifndef LOMBYTE_RNC_OVERLAY_L05_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L05_MOBYS_H

#include "types.h"

struct Moby;

void amoeboid_update(struct Moby *) __asm__("FUN_L05_002dac80");
void box_update(char *) __asm__("FUN_L05_0030bf98");
void cutscene_camera_target_update(unsigned char *) __asm__("FUN_L05_0031d5c8");
void drain_cover_update() __asm__("FUN_L05_0030e7f8");
void electric_gate_update(char *) __asm__("FUN_L05_0030dc68");
void elevator_with_button_update(struct Moby *) __asm__("FUN_L05_003156d0");
void extermibot_update(unsigned char *) __asm__("FUN_L05_00301f48");
void extermitank_update(unsigned char *) __asm__("FUN_L05_00304320");
void floating_platform_update(unsigned char *) __asm__("FUN_L05_0030bdd8");
void fog_zone_update(struct Moby *) __asm__("FUN_L05_0030e1f8");
void help_hints_update(struct Moby *) __asm__("FUN_L05_0031bf40");
void hero_clip_volume_outputs_update(struct Moby *) __asm__("FUN_L05_00318a68");
void hoverboard_girl_update() __asm__("FUN_L05_00316ab8");
void ocean_rilgar_update(struct Moby *) __asm__("FUN_L05_00316110");
void pipe_mounted_grating_update(struct Moby *) __asm__("FUN_L05_0030f5c8");
void qwarks_bouncer_update() __asm__("FUN_L05_00317470");
void qwarks_trailer_update(struct Moby *) __asm__("FUN_L05_00319c28");
void rising_gate_update(struct Moby *) __asm__("FUN_L05_00314eb0");
void scenery_ship_update(char *) __asm__("FUN_L05_002d7140");
void shady_salesman_update() __asm__("FUN_L05_003180a0");
void sharkigator_update(char *) __asm__("FUN_L05_002d1688");
void skid_mcmarx_update() __asm__("FUN_L05_00307910");
void taxi_update(struct Moby *) __asm__("FUN_L05_00318c78");
void initialize_taxi_stop_state(void *) __asm__("FUN_L05_00319208");
void television_update(struct Moby *) __asm__("FUN_L05_00318b98");
void swinging_flap_update(struct Moby *) __asm__("FUN_L05_003166a0");
void hinged_hatch_update(char *) __asm__("FUN_L05_00316258");
void sliding_block_update(char *) __asm__("FUN_L05_0030e508");
void water_spout_update(struct Moby *) __asm__("FUN_L05_003150f0");
void water_update(struct Moby *) __asm__("FUN_L05_0030ca80");

#endif /* LOMBYTE_RNC_OVERLAY_L05_MOBYS_H */
