/* Particles: creates a type 31 particle. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/rendering/effects/create_part.h"

/* Ported from rac1-decomp (src/overlays/shared/partupd_00260EE8.c: func_L03_00260EE8), where it is exact; names translated to the US level program. */

void create_type_31_part(int    part_word_3c,
                         int   *part_quadword_20_source,
                         int   *source_quadword,
                         float *source_floats,
                         int    part_halfword_0a) __asm__("FUN_L03_00260130");

void create_type_31_part(int    part_word_3c,
                         int   *part_quadword_20_source,
                         int   *source_quadword,
                         float *source_floats,

                         int part_halfword_0a) {
    unsigned char *created_part = create_part(0x31);
    if (created_part != 0) {
        float *destination_floats;
        *(int *)(created_part + 0x4) = 0x7FFFFFFF;
        *(int *)(created_part + 0xC) = 0xFFFFFFF;
        created_part[0x9] = truncate_float_to_s32(2.0f) + 0x70;
        created_part[0x3] = 0x44;
        destination_floats = (float *)(created_part + 0x30);
        created_part[0x1] = 2;
        qcopy(created_part + 0x10, source_quadword);
        qcopy(created_part + 0x20, part_quadword_20_source);
        destination_floats[0] = source_floats[0];
        destination_floats[1] = source_floats[1];
        destination_floats[2] = source_floats[2];
        *(int *)(destination_floats + 3) = part_word_3c;
        *(short *)(created_part + 0xA) = part_halfword_0a;
    }
}
