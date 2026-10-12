#include "types.h"
#include "sda.h"
#include "rnc/gameplay/entities/moby.h"
extern u32 world_update_time __asm__("D_0015F60C");
extern s32 num_spawnable_mobys __asm__("D_0015FEFC") MACRO_ADDR;
extern struct Moby *moby_instance_end __asm__("D_0015FF1C");
extern struct Moby *moby_instance_perm_end __asm__("D_0015FF20");
extern u8 *moby_vars __asm__("D_0015FF28");
extern char s_tried_to_create_moby[] __asm__("D_001E83C0");
extern void DebugPrint(char *, ...);
extern void fill_transfer_words(void *, s32, s32) __asm__("func_001F97E8");
extern void init_moby_instance(struct Moby *, s32) __asm__("func_0020C5F0");
struct Moby *create_moby(s32 oclass) __asm__("FUN_0020c4f8");

struct Moby *create_moby(s32 oclass) {
    struct Moby *m;

    for (m = moby_instance_end; m < moby_instance_perm_end; m++) {
        if (m->state >= 0xFE && m->spawn_frame <= world_update_time) {
            if (m->state == 0xFF) {
                m[1].state = 0xFF;
            }
            init_moby_instance(m, oclass);
            m->pvars = moby_vars + (m - moby_instance_end) * 0x80;
            fill_transfer_words(m->pvars, 0, 0x80);
            if (num_spawnable_mobys != 0) {
                num_spawnable_mobys--;
            }
            return m;
        }
    }
    DebugPrint(s_tried_to_create_moby, world_update_time, oclass);
    return 0;
}

extern __typeof__(create_moby) func_0020C4F8 __attribute__((alias("FUN_0020c4f8")));

/* Defined below their only users, so retail reaches them with lui. */
s32 num_spawnable_mobys __asm__("D_0015FEFC") MACRO_ADDR = 0;
