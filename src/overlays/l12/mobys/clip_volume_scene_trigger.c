/* Level 12 moby class clip_volume_scene_trigger (class 1404): starts a scene once when the hero enters its clip volume. */
#include "rnc/hero/hero.h"
#include "types.h"
#include "rnc/mobys/moby.h"
#include "asm.h"

#include "qcopy.h"
#include "sda.h"
#include "rnc/game/item_state.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"

typedef struct Moby Moby;
extern int          D_L12_0015F5CC;
extern void         start_scene(int) __asm__("FUN_L00_00298840");

extern char scene_trigger_saved_hero_data[] __asm__("D_L12_0016CCF0");
extern int  D_L12_0015F5C4;

void clip_volume_scene_trigger_update(struct Moby *moby) __asm__("FUN_L12_003093c8");
/* Vendor-counter moby update: once the shared block is ready, latch its data and copy two vectors. */
/* Ported from rac1-decomp (src/overlays/l12_hoven/vendor_002EDAA0.c: func_L12_0030A7A8), where it is exact; names translated to the US level program. */

/* Moby class 1404 */
void clip_volume_scene_trigger_update(struct Moby *moby) {
    char *scene_trigger_config = (char *)moby->pvars;
    char *scene_trigger_latch_base;
    char *hero_position;
    char *destination_base;
    if (D_L12_0015F5CC != 0 && D_L12_0015F5C4 == 0) {
        scene_trigger_latch_base = alternate_item_available;
        if (*(unsigned char *)(*(int *)(scene_trigger_config + 4) + scene_trigger_latch_base +
                               0x5C) != 0) {
            mark_moby_for_removal(moby);
            return;
        }
        hero_position = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(hero_position, *(int *)scene_trigger_config) != 0) {
            *(char *)(*(int *)(scene_trigger_config + 4) + scene_trigger_latch_base + 0x5C) = 1;
            qcopy(scene_trigger_saved_hero_data, hero_position);
            destination_base = scene_trigger_saved_hero_data - 0x10;
            *(float *)(destination_base + 0x18) = *(float *)(hero_position + 0x258);
            qcopy(scene_trigger_saved_hero_data + 0x10, hero_position + 0x10);
            *(short *)(destination_base + 0x46) = 1;
            start_scene(*(int *)(scene_trigger_config + 8));
        }
    }
}
