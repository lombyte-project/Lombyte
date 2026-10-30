#ifndef LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_UPDATE_NPC_DIALOGUE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_UPDATE_NPC_DIALOGUE_H

#include "types.h"

s32  initialize_npc_dialogue(void *, void *) __asm__("FUN_L00_002668a0");
s32  get_dialogue_entry(void *) __asm__("FUN_L00_002667d0");
s32  update_npc_dialogue(void *, void *) __asm__("FUN_L00_00266448");
void set_moby_dialogue_record_value(void *, s32) __asm__("FUN_L00_00266858");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_CUTSCENES_UPDATE_NPC_DIALOGUE_H */
