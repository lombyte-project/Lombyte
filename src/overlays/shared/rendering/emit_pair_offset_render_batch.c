/* Rendering: emits a pair-offset render batch. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/level_labels.h"
#include "sda.h"

extern void FUN_L00_001fde98(int, int, int, void *, int);
extern void FUN_L08_00258090(void *, int, int, void *);
extern Pair D_L08_00161E20[] MACRO_ADDR;
extern int  pair_offset_batch_input_pair_streams[] __asm__("D_L08_001DDEE0");
extern int  pair_offset_batch_packet_triplet_streams[] __asm__("D_L08_001DDEC0");
void        emit_pair_offset_render_batch(int pair_index) __asm__("FUN_L08_002f0cf0");

void emit_pair_offset_render_batch(int pair_index) {
    int i;
    for (i = 0; i < 0x8; i++) {
        FUN_L08_00258090(D_L08_001E8360,
                         pair_offset_batch_input_pair_streams[i],
                         D_L08_001DB518[i],
                         &D_L08_00161E20[pair_index]);
        FUN_L00_001fde98(D_L08_001DB518[i],
                         pair_offset_batch_packet_triplet_streams[i],
                         D_L08_001DDF20[i],
                         D_L08_001E8360,
                         1);
    }
}
