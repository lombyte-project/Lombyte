/* Prototypes exported by shared/vehicles/taxi_path.c. */
#ifndef LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_TAXI_PATH_H
#define LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_TAXI_PATH_H

#include "types.h"

void reset_selected_moby_to_current_placement(void *) __asm__("FUN_L05_00319510");
s8  *find_moby_in_owner_list_by_id(void *, s32) __asm__("FUN_L05_00319598");
s32  advance_moby_path(void *) __asm__("FUN_L05_003195f8");
void request_moby_variant_help_message(void *) __asm__("FUN_L05_00319690");
void apply_moby_position_and_rotation_wobble(void *) __asm__("FUN_L05_00319740");
void transform_moby_vector_table_to_world_space(void *) __asm__("FUN_L05_003198e8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_VEHICLES_TAXI_PATH_H */
