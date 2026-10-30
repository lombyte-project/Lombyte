/* Effects: draws the textured effect list, the NPC big head cheat and the level 12 effects. */
#include "types.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/game/options.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"
#include "rnc/mobys/moby.h"

/* draw a list of effect entries into the scratchpad */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BD3D0.c: func_L12_002BD3D0), where it is exact; names translated to the US level program. */

extern char textured_effect_gs_state_packet[] __asm__("D_L12_001CB7A0");

struct Pkt611c0 {
    int *p;
};

extern struct Pkt611c0 l12_render_packet_cursor __asm__("D_L12_001611C0");
extern void            FUN_L01_0021fa98(int, int, int, int, int, int);
extern void            FUN_L12_002667fc(int, int, int, int);

void draw_textured_effect_list(void *list_,
                               int   count,
                               int   context_one_texture_state,
                               int   context_two_texture_state) __asm__("FUN_L12_002bc210");

void draw_textured_effect_list(void *list_,
                               int   count,
                               int   context_one_texture_state,

                               int context_two_texture_state) {
    char *list = list_;
    int   i;
    int  *sp;
    int   sphere_check_result;
    vu1_add_g_sregister(6, context_one_texture_state);
    vu1_add_g_sregister(7, context_two_texture_state);
    l12_render_packet_cursor.p[0] = 0x30000009;
    l12_render_packet_cursor.p[1] = (int)textured_effect_gs_state_packet;
    l12_render_packet_cursor.p[2] = 0;
    l12_render_packet_cursor.p[3] = 0x50000009;
    l12_render_packet_cursor.p += 4;
    font_queue_vu_state();
    sp = (int *)0x70000000;
    for (i = 0xB3; i >= 0; i--) {
        *sp++ = 0x80808080;
    }
    if (count > 0) {
        for (i = count; i != 0; i--) {
            sphere_check_result = fast_bsphere_check(list, 256.0f);
            if (sphere_check_result != -1) {
                FUN_L12_002667fc(*(int *)(list + 0x1C),
                                 *(int *)(list + 0x10),
                                 *(int *)(list + 0x18),
                                 0x70001000);
                FUN_L01_0021fa98(*(int *)(list + 0x1C),
                                 *(int *)(list + 0x10),
                                 0x70000000,
                                 *(int *)(list + 0x14),
                                 0x70001000,
                                 sphere_check_result == 0);
            }
            list += 0x20;
        }
    }
}

typedef struct {
    char  pad0[0x44];
    short count;
    char  pad46[0x132];
    char *mobys[1];
} L12MobyList;

extern L12MobyList   D_L12_0016CCE0;
extern unsigned char l12_npc_big_head_manipulator[] __asm__("D_L12_0017C8C0");

void apply_npc_big_head_cheat(char *moby) __asm__("FUN_L12_002e6b60");

void apply_npc_big_head_cheat(char *moby) {
    int i;

    if (D_L12_0015F5C4 == 2 && big_head_cheat_enabled != 0) {
        for (i = 0; i < D_L12_0016CCE0.count; i++) {
            char *candidate_moby = D_L12_0016CCE0.mobys[i];

            if (MOBY(candidate_moby)->oclass == MOBY(moby)->oclass) {
                unsigned char *manipulator = l12_npc_big_head_manipulator;

                if (manipulator[1] == 0) {
                    attach_manipulator(candidate_moby, 1, manipulator);
                    *(float *)(manipulator + 0x20) = 2.75f;
                    *(float *)(manipulator + 0x24) = 2.75f;
                    *(float *)(manipulator + 0x28) = 2.75f;
                }
            }
        }
    }
}

/* calls func_L12_002BD3D0 with two looked-up ids */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002BD3D0.c: func_L12_002E8530), where it is exact; names translated to the US level program. */

extern char level_12_effect_entries[] __asm__("D_L12_001F5480");
extern s32  get_effect_texture() __asm__("FUN_001f44b8");
extern void draw_textured_effect_list(void *, int, int, int) __asm__("FUN_L12_002bc210");

void draw_level_12_effects(void) __asm__("FUN_L12_002e71b0");

void draw_level_12_effects(void) {
    draw_textured_effect_list(
        level_12_effect_entries, 0x2F, get_effect_texture(0x2C), get_effect_texture(0x2D));
}

f32 D_L15_0017CA60[1] __attribute__((section(".data"))) = {0};

f32 D_L15_0017CA64[1] __attribute__((section(".data"))) = {0};
