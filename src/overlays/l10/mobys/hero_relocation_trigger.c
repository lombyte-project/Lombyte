/* Level 10 moby class hero_relocation_trigger. */
#include "types.h"
#include "rnc/overlay/cuboid.h"
#include "rnc/overlay/level_save_state.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/audio/sound_state.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/camera/follow_cam.h"
#include "rnc/game/game_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/rendering/effect_texture.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

#include "rnc/game/usage_stats.h"

typedef struct {
    int target;
    int vehicle_spot;
    int spot;
    int pad0C[2];
    int spot_alt;
    int switch_a;
    int switch_b;
} PickupVars;

int          D_L10_00161388 = 1;
extern char *D_L10_0015FFD8_98b68 __asm__("D_L10_0015FFD8");
extern int   D_L10_001BA950[];
extern u8    D_001413F4_98b68[] __asm__("D_001413F4");
extern int   try_set_help_message(int, int) __asm__("FUN_00215130");
extern void  set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void  enter_hero_vehicle_mode(int, int, struct Moby *) __asm__("FUN_L00_00210a08");
extern void  request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void  exit_hero_vehicle_mode(void) __asm__("FUN_L00_00210b30");

extern short D_00141948 NOT_SDA;
/* Level 10 pickup: saves its collected bit, flips the linked switch pair and moves the hero to the spot for the current state. */
void update_hero_relocation_trigger(struct Moby *moby) __asm__("FUN_L10_00298b68");

#include "sda.h"

void update_hero_relocation_trigger(struct Moby *moby) {
    PickupVars  *vars;
    struct Moby *target;
    moby->update_dist = 0xFF;
    vars = (PickupVars *)moby->pvars;
    if (D_L10_00161388 == 1) {
        D_L10_00161388 = 0;
        D_00141948 = 0;
    }
    if (vars->switch_a != -1 && vars->switch_b != -1) {
        struct Moby *switch_a_moby =
            (struct Moby *)((vars->switch_a << 8) + (int)D_L10_0015FFD8_98b68);
        if (switch_a_moby->oclass == 0x325) {
            struct Moby *switch_b_moby =
                (struct Moby *)((vars->switch_b << 8) + (int)D_L10_0015FFD8_98b68);
            if (switch_b_moby->oclass == switch_a_moby->oclass) {
                int *a_vars = (int *)switch_a_moby->pvars;
                int *b_vars = (int *)switch_b_moby->pvars;
                if (visited52[moby->mission + current_level_index * 16] == 0xFF) {
                    a_vars[2] |= 8;
                    b_vars[2] &= ~8;
                } else {
                    a_vars[2] &= ~8;
                    b_vars[2] |= 8;
                }
            }
        }
    }
    if (l10_save_state.cleared[(short)moby->save_id] != 0 ||
        ((D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] >>
          (moby->save_id & 31)) &
         1) != 0) {
        if (D_0013D4C6[0] != 0) {
            ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->flags |= 1;
            ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->flags |= 2;
            ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->collision = 0;
        } else {
            if (D_L10_0015F5C4_vendor == 0 && usage_stats.stat[32].count < scale_game_frames(300)) {
                if (usage_stats.stat[32].count <= 0xFFFE) {
                    usage_stats.stat[32].count++;
                }
                if (scale_game_frames(D_0015EEA4) / 600 > usage_stats.stat[32].unk2) {
                    usage_stats.stat[32].unk2 = scale_game_frames(D_0015EEA4) / 600;
                }
                usage_stats.stat[32].level_mask =
                    usage_stats.stat[32].level_mask | (1 << current_level_index) | 0x80000000;
                try_set_help_message(1, 0x53F1);
                if (D_001413F4_98b68[0] == 1) {
                    return;
                }
            } else if (D_001413F4_98b68[0] == 1) {
                return;
            }
        }
    }
    D_0014C190_257218[current_level_index][(short)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
    D_L10_001BA950[(short)moby->save_id >> 5] |= 1 << (moby->save_id & 31);
    if (vars->spot == -1 || vars->target == -1) {
        return;
    }
    if (D_0013D4C6[0] == 0) {
        target = (struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68);
        target->collision = *(u32 *)((char *)target->pclass + 0x10);
        if (visited52[moby->mission + current_level_index * 16] == 0xFF || vars->spot_alt == -1) {
            set_race_camera(
                &l10_cuboids[vars->spot_alt].pos, &l10_cuboids[vars->spot_alt].rot, 0, 1);
        } else {
            set_race_camera(&l10_cuboids[vars->spot].pos, &l10_cuboids[vars->spot].rot, 0, 1);
        }
        enter_hero_vehicle_mode(
            1, 0x43, (struct Moby *)(D_L10_0015FFD8_98b68 + (vars->target << 8)));
        request_music_track(2, 4);
        return;
    }
    if (hero.vehicle_mode == 1) {
        exit_hero_vehicle_mode();
        set_race_camera(
            &l10_cuboids[vars->vehicle_spot].pos, &l10_cuboids[vars->vehicle_spot].rot, 0, 1);
        request_music_track(0, 5);
        ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->flags |= 1;
        ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->flags |= 2;
        ((struct Moby *)((vars->target << 8) + (int)D_L10_0015FFD8_98b68))->collision = 0;
        hero.vent_updraft = 1;
        mark_moby_for_removal(moby);
    } else {
        hero.vent_updraft = 1;
        mark_moby_for_removal(moby);
    }
}
