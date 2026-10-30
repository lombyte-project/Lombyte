#ifndef LOMBYTE_RNC_MOBYS_HIT_RESPONSE_TABLES_H
#define LOMBYTE_RNC_MOBYS_HIT_RESPONSE_TABLES_H

#include "types.h"

/* A hit response code (resolve_moby_hit_response switches on it). */
typedef u8 HitResponseCode;

/* resolve_moby_hit_response's tables. The response code per hit response
   index (0..0x11, one row each) and target class (q->b8, 0..3). */
extern HitResponseCode hit_response_code_table[18][4] __asm__("D_001CC080");
/* Hit vector scale per row (0..3; row 4 means the target's class, q->b8)
   and hit state (0..7). */
extern f32 hit_vector_scale_table[4][8] __asm__("D_001CC0C8");

#endif /* LOMBYTE_RNC_MOBYS_HIT_RESPONSE_TABLES_H */
