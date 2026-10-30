/* Shared moby class pilots_helmet: the pilot's helmet item, collected once with a cutscene. */

#include "types.h"
#include "rnc/overlay/point_light.h"
#include "rnc/mobys/moby.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "sda.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/pickups/infobot.h"

typedef struct {
    u8  pad[0xB0];
    u8  b0;
    u8  pad1;
    s16 h;
    u8  pad2[0x4C];
} E_2e03a0_2e03a0;

typedef struct {
    s32 fa_2e03a0;
    s32 b;
} P_2e03a0_2e03a0;

typedef struct {
    u8               pad[0x18];
    f32              z;
    u8               pad1[4];
    u8               state;
    u8               pad2[0x13];
    u16              fl;
    u8               pad3[0x12];
    f32              ang;
    u8               pad4[0x2C];
    P_2e03a0_2e03a0 *p;
    u8               pad5[0x34];
    u8               b0;
    u8               pad6;
    s16              h;
} M_2e03a0_2e03a0;

typedef struct {
    u8 pad[0x454];
    u8 arr[1];
} T_2e03a0_2e03a0;

typedef struct {
    char  pad[0x10];
    float a[4];
    float b[4];
    int   t[4];
    float c[4];
} M_2e0788;

extern f32  FUN_001fa580_2e03a0(f32, f32) __asm__("FUN_001fa580");
extern void debug_print() __asm__("FUN_001e93b0");
extern void FUN_L00_002e0788_2e03a0(void *) __asm__("FUN_L00_002e0788");
extern f32  vector_distance(void *, void *) __asm__("FUN_001f9b80");
extern void start_scene(s32) __asm__("FUN_L00_00298840");
extern void help_request(s32, s32) __asm__("FUN_L00_00203908");
extern void memcard_save_data(s32, s32) __asm__("FUN_0020b178");
extern void vscl_2e0788(void *, void *, float) __asm__("FUN_001f9bf8");
extern int  f9740_2e0788(void *) __asm__("FUN_001f9740");
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern unsigned char *
spr_2e0788(OvlQuad *, int, unsigned char, int, int, int, int, float) __asm__("FUN_L00_00272f68");

extern char            D_L00_00166DC0[] NOT_SDA;
extern s32             D_L00_0015F5C4_2e03a0 __asm__("D_L00_0015F5C4");
extern s32             D_L00_0015F640 __asm__("D_L00_0015F640");
extern u8              mission_state_macro[] __asm__("D_L00_0015FC88") MACRO_ADDR;
extern s32             D_00141410_2e03a0[] __asm__("D_00141410");
extern u8              pilots_helmet_missing_cutscene_format[] __asm__("D_L00_001EA1B0");
extern u8              pilots_helmet_already_owned_message[] __asm__("D_L00_001EA1D8");
extern u8              pilots_helmet_missing_segment_switch_format[] __asm__("D_L00_001EA200");
extern u8              pilots_helmet_acquired_message[] __asm__("D_L00_00161BC0");
extern u16             pilots_helmet_help_record[] __asm__("D_00141B08");
extern T_2e03a0_2e03a0 D_L00_001BA5D0;
extern T_2e03a0_2e03a0 D_L00_001BB230;
extern s32             current_level_index_gp __asm__("D_0015ED84") __attribute__((sda));

void pilots_helmet_update(M_2e03a0_2e03a0 *m) __asm__("FUN_L00_002e03a0");

void pilots_helmet_update(M_2e03a0_2e03a0 *moby) {
    enum {
        PILOTS_HELMET_INITIALIZE = 0,
        PILOTS_HELMET_WAIT_FOR_HERO_PROXIMITY = 1,
        PILOTS_HELMET_GRANT_REWARD = 2,
        PILOTS_HELMET_SAVE_AND_REMOVE = 3
    };
    P_2e03a0_2e03a0 *p = moby->p;
    char            *hero_data;
    E_2e03a0_2e03a0 *e;
    u32              k;
    s32              n;

    moby->ang = FUN_001fa580_2e03a0(moby->ang, frame_time * 1.5707964f);
    switch (moby->state) {
    case PILOTS_HELMET_INITIALIZE:
        initialize_moby_particle_effect(moby);
        if (p->fa_2e03a0 == -1)
            debug_print(pilots_helmet_missing_cutscene_format, moby->h);
        if (item_available[7]) {
            debug_print(pilots_helmet_already_owned_message);
            mark_moby_for_removal(moby);
            return;
        }
        moby->state = 1;
        moby->z += 1.0f;
        break;
    case PILOTS_HELMET_WAIT_FOR_HERO_PROXIMITY:
        FUN_L00_002e0788_2e03a0(moby);
        hero_data = (char *)&hero.motion.pos;
        if (vector_distance((char *)moby + 0x10, hero_data) < 3.0f) {
            hero_data -= 0x80;
            if (absolute_float(moby->z - *(f32 *)(hero_data + 0x88)) < 2.0f &&
                *(s32 *)(hero_data + 0x22A8)) {
                moby->fl |= 0x41;
                if (p->fa_2e03a0 != -1)
                    start_scene(p->fa_2e03a0);
                else
                    debug_print(pilots_helmet_missing_cutscene_format, moby->h);
                moby->state = 2;
            }
        }
        break;
    case PILOTS_HELMET_GRANT_REWARD:
        if (D_L00_0015F5C4_2e03a0 == 2)
            return;
        debug_print(pilots_helmet_acquired_message);
        queue_dialogue_message(0x232A, -1);
        D_L00_0015F640 = 0xB4;
        grant_inventory_item(7, 1);
        if (pilots_helmet_help_record[0] == 0)
            help_request(0x2328, 0x34);
        if (p->b != -1) {
            D_L00_001BA5D0.arr[((E_2e03a0_2e03a0 *)moby_table)[p->b].h] =
                ((E_2e03a0_2e03a0 *)moby_table)[p->b].b0 + 2;
            if (((E_2e03a0_2e03a0 *)moby_table)[p->b].b0 == 0xFF ||
                (mission_state_macro[((E_2e03a0_2e03a0 *)moby_table)[p->b].b0] != 0xFF &&
                 visited52[((E_2e03a0_2e03a0 *)moby_table)[p->b].b0 +
                           current_level_index_gp * 16] == 0xFF)) {
                D_L00_001BB230.arr[((E_2e03a0_2e03a0 *)moby_table)[p->b].h] =
                    ((E_2e03a0_2e03a0 *)moby_table)[p->b].b0 + 2;
            }
        } else {
            debug_print(pilots_helmet_missing_segment_switch_format, moby->h);
        }
        if (moby->b0 != 0xFF)
            jet_clear_persistent(moby->b0);
        D_00141410_2e03a0[0] = 7;
        moby->state = 3;
        break;
    case PILOTS_HELMET_SAVE_AND_REMOVE:
        memcard_save_data(0, -1);
        mark_moby_for_removal(moby);
        break;
    }
}

void emit_moby_color_cycling_particles(unsigned char *moby) __asm__("FUN_L00_002e0788");

void emit_moby_color_cycling_particles(unsigned char *moby) {
    OvlQuad   sprite_position_and_step[2];
    M_2e0788 *moby_data = ((M_2e0788 *)MOBY(moby)->pvars);
    float    *a;
    int      *t;
    int       i, j, c;
    float     pre_wrap_sum, color_cycle_progress;
    j = 0;
    moby += 0x10;
    fast_vec_sub(sprite_position_and_step, D_L00_00166DC0, moby);
    t = moby_data->t;
    a = moby_data->a;
    vscl_2e0788(sprite_position_and_step, sprite_position_and_step, -0.3f);
    vscl_2e0788(&sprite_position_and_step[1], sprite_position_and_step, 0.1f);
    fast_vec_add(sprite_position_and_step, sprite_position_and_step, moby);
    for (i = 3; i >= 0; i--) {
        pre_wrap_sum = *a + moby_data->b[j];
        *a = pre_wrap_sum;
        if (pre_wrap_sum >= 255.0f) {
            *a = pre_wrap_sum - 255.0f;
        } else if (pre_wrap_sum <= 0.0f) {
            *a = pre_wrap_sum + 255.0f;
        }
        if (f9740_2e0788(t)) {
            moby_data->t[j] = scale_game_frames(0xff);
        }
        t++;
        color_cycle_progress = convert_integer_to_float(scale_game_frames(0xff) - moby_data->t[j]);
        color_cycle_progress = color_cycle_progress / (float)scale_game_frames(0xff);
        c = fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - color_cycle_progress));
        spr_2e0788(sprite_position_and_step,
                   c,
                   truncate_float_to_s32(*a++),
                   0x35,
                   1,
                   2,
                   0,
                   moby_data->c[j]);
        j++;
        fast_vec_add(
            sprite_position_and_step, sprite_position_and_step, &sprite_position_and_step[1]);
    }
}
