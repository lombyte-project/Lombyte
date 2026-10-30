/* Level 04 moby class suck_cannon and its particles. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/pickups/infobot.h"

#include "rnc/overlay/l04/mobys/suck_cannon_globals.h"
#include "rnc/overlay/level_labels.h"

/* Warp portal: spins, opens once the hero is near, then warps to the exit moby. */
struct PortalVars {
    s32 unk0;
    s32 exitMoby; /* 0x04: moby index to warp to, -1 for none */
};

struct PortalMobyView {
    u8  pad0[0xB2];
    s16 uid; /* 0xB2 */
};

extern s32  D_L04_0015F5C4;
extern u8   suck_cannon_in_inventory[] __asm__("D_0013D4F1");
extern u8   suck_cannon_first_acquisition_recorded[] __asm__("D_0013D4C9");
extern char suck_cannon_missing_segment_switch_warning_format[] __asm__("D_L04_001DC760");
extern void FUN_L04_002e1d00_m(struct Moby *) __asm__("FUN_L04_002e1d00");
extern void start_scene(int) __asm__("FUN_L00_00298840");
extern void grant_inventory_item(int, int) __asm__("FUN_L00_00260860");
extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");
extern void debug_print(char *, int) __asm__("FUN_001e93b0");
extern void memcard_save_data(int, int) __asm__("FUN_0020b178");

void suck_cannon_update(struct Moby *moby) __asm__("FUN_L04_002e1a10");

void suck_cannon_update(struct Moby *moby) {
    enum {
        SUCK_CANNON_INITIALIZE = 0,
        SUCK_CANNON_WAIT_FOR_HERO = 1,
        SUCK_CANNON_WAIT_TO_RESET_CAMERA = 2,
        SUCK_CANNON_REMOVE = 3
    };
    struct PortalVars *vars = (struct PortalVars *)moby->pvars;
    struct Moby       *exit;

    moby->rot.z = fast_add_rotations(moby->rot.z, frame_time * 1.5707964f);
    if (D_L04_0015F5C4 == 2)
        moby->flags |= 0x41;
    else if (moby->flags & 1)
        moby->flags &= ~0x41;
    switch (moby->state) {
    case SUCK_CANNON_INITIALIZE:
        initialize_moby_particle_effect(moby);
        if (suck_cannon_in_inventory[0]) {
            mark_moby_for_removal(moby);
            break;
        }
        moby->state = 1;
        moby->pos.z += 1.0f;
        break;
    case SUCK_CANNON_WAIT_FOR_HERO:
        FUN_L04_002e1d00_m(moby);
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 3.0f) {
            moby->flags |= 0x41;
            start_scene(2);
            moby->state = 2;
        }
        break;
    case SUCK_CANNON_WAIT_TO_RESET_CAMERA:
        if (D_L04_0015F5C4 == 2)
            break;
        if (!suck_cannon_first_acquisition_recorded[0])
            queue_dialogue_message(0xFA7, -1);
        else
            queue_dialogue_message(0x53DF, -1);
        dialogue_message_duration_frames = 0xB4;
        grant_inventory_item(9, 1);
        if (vars->exitMoby != -1) {
            exit = &((struct Moby *)moby_array)[vars->exitMoby];
            jet_reset_camera(&exit->pos, &exit->rot);
        } else {
            debug_print(suck_cannon_missing_segment_switch_warning_format,
                        ((struct PortalMobyView *)moby)->uid);
        }
        moby->state = 3;
        memcard_save_data(0, -1);
        break;
    case SUCK_CANNON_REMOVE:
        mark_moby_for_removal(moby);
        break;
    }
}

/* Same source as the exact FUN_L10_00298940, with a 0.25 lift and the draw skipped in mode 2. */
typedef struct {
    float pad0[4];
    float a[4];
    float b[4];
    int   c[4];
    float d[4];
} L10SpriteData;

extern int            fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern unsigned char *func_L00_00273E08_298940(
    void *, int, unsigned char, int, int, int, int, float) __asm__("FUN_L00_00272f68");
extern char D_L10_00167240_298940[] __asm__("D_L04_00166F40");
extern int  D_L04_0015F5C4;
void        update_suck_cannon_particles(char *moby) __asm__("FUN_L04_002e1d00");

void update_suck_cannon_particles(char *moby) {
    L10SpriteData *sprite_data = ((L10SpriteData *)MOBY(moby)->pvars);
    float          d[4] __attribute__((aligned(16)));
    float          particle_position_step[4] __attribute__((aligned(16)));
    int            i, col;
    float          timer_progress;

    MOBY(moby)->pos.z += 0.25f;
    fast_vec_sub(d, D_L10_00167240_298940, moby + 0x10);
    fast_vec_normalize(d, d, -0.3f);
    fast_vec_normalize(particle_position_step, d, 0.1f);
    fast_vec_add(d, d, moby + 0x10);
    MOBY(moby)->pos.z -= 0.25f;
    for (i = 0; i < 4; i++) {
        float sprite_attribute_sum = sprite_data->a[i] + sprite_data->b[i];
        sprite_data->a[i] = sprite_attribute_sum;
        if (255.0f <= sprite_attribute_sum) {
            sprite_data->a[i] = sprite_attribute_sum - 255.0f;
        } else if (sprite_attribute_sum <= 0.0f) {
            sprite_data->a[i] = sprite_attribute_sum + 255.0f;
        }
        if (fast_dec_timer(&sprite_data->c[i])) {
            sprite_data->c[i] = scale_game_frames(0xFF);
        }
        timer_progress = convert_integer_to_float(scale_game_frames(0xFF) - sprite_data->c[i]);
        timer_progress = timer_progress / (float)scale_game_frames(0xFF);
        col = fast_tween_color(0x4040FFFF, 0x1040FFFF, absolute_float(0.5f - timer_progress));
        if (D_L04_0015F5C4 != 2)
            func_L00_00273E08_298940(d,
                                     col,
                                     truncate_float_to_s32(sprite_data->a[i]) & 0xFF,
                                     0x35,
                                     1,
                                     2,
                                     0,
                                     sprite_data->d[i]);
        fast_vec_add(d, d, particle_position_step);
    }
}
