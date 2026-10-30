#ifndef LOMBYTE_RNC_OVERLAY_L06_MOBYS_ALIEN_QUEEN_H
#define LOMBYTE_RNC_OVERLAY_L06_MOBYS_ALIEN_QUEEN_H

#include "types.h"

/* Functions of src/overlays/l06/mobys/alien_queen.c that other level 06 mobys call. */
int  activate_state_15_moby_at_position(void *m_,
                                        void *activation_position) __asm__("FUN_L06_002e9d10");
int  check_all_listed_frog_states_fe_fd_or_10(int index) __asm__("FUN_L06_002e9ea0");
void set_moby_and_links_to_states_4_and_5(char *moby) __asm__("FUN_L06_002f5360");
void set_moby_and_links_to_states_8_and_9(char *moby) __asm__("FUN_L06_002f53e8");
void set_moby_list_state_18(int moby_list_index) __asm__("FUN_L06_002f9948");
void clear_moby_list_state_18(int index) __asm__("FUN_L06_002f99b0");

#endif /* LOMBYTE_RNC_OVERLAY_L06_MOBYS_ALIEN_QUEEN_H */
