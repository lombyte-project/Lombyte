/* Mobys: the linked moby turn trigger test, and the oil tanker front texture batches. */
#include "types.h"
#include "asm.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/l11/mobys/distance_faded_scroll_effect_globals.h"
/* Recognize the active vendor object state. */

int test_linked_moby_turn_trigger(struct Moby *moby) __asm__("FUN_L11_0030f270");

int test_linked_moby_turn_trigger(struct Moby *moby) {
    if (moby->oclass == 0x49b && ((unsigned char *)moby)[0x20] == 0x2) {
        return 1;
    }
    return 0;
}

extern void       FUN_L00_001fde98(int, MeshPoint *, u32 *, void *, int);
extern void       FUN_L08_00258090(void *, MeshUv *, int, void *);
extern Pair       D_L11_00162628[] MACRO_ADDR;
extern MeshUv    *oil_tanker_front_texture_2b_batch_pair_sources[5] __asm__("D_L11_00217710");
extern MeshPoint *oil_tanker_front_texture_2b_batch_triplet_sources[5] __asm__("D_L11_002176F8");
void queue_oil_tanker_front_texture_2b_batches(int pair_index) __asm__("FUN_L11_0031e858");

void queue_oil_tanker_front_texture_2b_batches(int pair_index) {
    int i;
    for (i = 0; i < 0x5; i++) {
        FUN_L08_00258090(D_L11_00217770,
                         oil_tanker_front_texture_2b_batch_pair_sources[i],
                         oil_tanker_front_texture_2b_batch_vertex_counts[i],
                         &D_L11_00162628[pair_index]);
        FUN_L00_001fde98(oil_tanker_front_texture_2b_batch_vertex_counts[i],
                         oil_tanker_front_texture_2b_batch_triplet_sources[i],
                         oil_tanker_front_texture_2b_batch_color_tables[i],
                         D_L11_00217770,
                         1);
    }
}

Pair D_L12_001620F8 = {0};
