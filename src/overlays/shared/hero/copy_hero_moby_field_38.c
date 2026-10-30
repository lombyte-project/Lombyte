/* Hero: copies the 64-bit field at 0x38 of the hero moby. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "asm.h"

typedef struct {
    u8  pad[0x38];
    s64 v;
} S;

/* Copies the 64-bit value from the record D_001413D0[0] points to into p. */
void copy_hero_moby_field_38(S *p, s64 v) __asm__("FUN_L00_0025d1b8");

void copy_hero_moby_field_38(S *destination, s64 v) {
    v = (((S *)hero.moby))->v;
    destination->v = v;
}
