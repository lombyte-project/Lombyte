#ifndef LOMBYTE_RNC_MOBYS_H
#define LOMBYTE_RNC_MOBYS_H

#include "types.h"

struct MobyObject;

void assign_moby_class_data(s32) __asm__("FUN_00212d68");
s32  get_moby_hit_state(struct MobyObject *);
void quad_moby_update(char *) __asm__("FUN_0023a2c0");

#endif /* LOMBYTE_RNC_MOBYS_H */
