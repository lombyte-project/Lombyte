#ifndef LOMBYTE_RNC_OVERLAY_L06_MOBYS_H
#define LOMBYTE_RNC_OVERLAY_L06_MOBYS_H

#include "types.h"

struct Moby;

void alien_snapper_update() __asm__("FUN_L06_002f7d78");
void alien_snapper_spawner_update(char *) __asm__("FUN_L06_002fda30");
void beam_barrier_update(struct Moby *) __asm__("FUN_L06_002f6470");
void breakable_prop_update(char *) __asm__("FUN_L06_00309e08");
void breakable_prop_variant_update(char *) __asm__("FUN_L06_00309fa8");
void breakable_window_update(char *) __asm__("FUN_L06_002ffcb0");
void breakaway_piece_update(char *) __asm__("FUN_L06_00300df0");
void countdown_sequence_update(unsigned char *) __asm__("FUN_L06_00301b90");
void cutscene_camera_target_update(struct Moby *) __asm__("FUN_L06_00309348");
void draw_pulsing_timer_display(char *) __asm__("FUN_L06_003021d8");
void drawbridge_update() __asm__("FUN_L06_002f55a0");
void dual_texture_mesh_update(char *) __asm__("FUN_L06_002fd088");
void elevator_update() __asm__("FUN_L06_002fc640");
void fred_update(struct Moby *) __asm__("FUN_L06_00301070");
void hangar_door_update(char *) __asm__("FUN_L06_002b4770");
void help_hints_update(struct Moby *) __asm__("FUN_L06_003083b0");
void red_button_update(struct Moby *) __asm__("FUN_L06_00304098");
void shuttle_update(struct Moby *) __asm__("FUN_L06_00302578");
void spark_emitter_update(struct Moby *) __asm__("FUN_L06_00308c68");
void activate_listed_spark_emitters(int) __asm__("FUN_L06_00309240");
void split_door_update(char *) __asm__("FUN_L06_002fbfb0");
void switchable_beam_barrier_update(char *) __asm__("FUN_L06_003049f8");
void toxic_gas_vent_update(char *) __asm__("FUN_L06_002f3ad8");

#endif /* LOMBYTE_RNC_OVERLAY_L06_MOBYS_H */
