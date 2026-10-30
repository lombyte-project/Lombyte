#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_HYDRODISPLACER_FAUCET_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_HYDRODISPLACER_FAUCET_H

#include "types.h"

struct Moby;

void update_moby_spatial_trigger(struct Moby *) __asm__("FUN_L05_002f5200");
void hydrodisplacer_faucet_update(s8 *) __asm__("FUN_L05_002f8080");
void floor_button_update(struct Moby *) __asm__("FUN_L05_0030c5b0");
void update_moby_wobble(u8 *) __asm__("FUN_L05_0030c0a8");
s8  *find_moby_in_owner_list_by_id(void *, s32) __asm__("FUN_L05_00319598");
s32  advance_moby_path(void *) __asm__("FUN_L05_003195f8");
void reset_selected_moby_to_current_placement(void *) __asm__("FUN_L05_00319510");
void apply_moby_position_and_rotation_wobble(void *) __asm__("FUN_L05_00319740");
void transform_moby_vector_table_to_world_space(void *) __asm__("FUN_L05_003198e8");
void blarg_gun_update(s8 *) __asm__("FUN_L05_003186f8");
void hoverboard_update(u8 *) __asm__("FUN_L05_002f87a8");
void request_moby_variant_help_message(void *) __asm__("FUN_L05_00319690");
void add_moby_frames_for_group_proximity(s8 *) __asm__("FUN_L05_00307010");
void initialize_moby_launch_motion(void *, void *, void *, void *, s32) __asm__("FUN_L05_00318648");
void attach_big_head_cheat_manipulator(void *) __asm__("FUN_L05_00317398");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_HYDRODISPLACER_FAUCET_H */
