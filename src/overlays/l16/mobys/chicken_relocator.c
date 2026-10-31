/* Level 16 moby class chicken_relocator (0x783): triggers its target moby, then moves a morphoray chicken of its group there. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/overlay/shared/mobys/is_hero_on_moby.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_line.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/audio/voice_alloc.h"

#include "qcopy.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/overlay/watch.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/interpolation/compute_directed_angle_delta.h"
#include "rnc/overlay/shared/vendor/vendor_spring.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/mobys/step_moby_motion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/overlay/shared/game/restore_or_clear_level_state.h"
#include "rnc/ui/text/font/font_queue_vu_state.h"

extern float D_0015ED6C;

#include "rnc/overlay/quad.h"

extern float D_0015ED6C, frame_time;

/* Spawns effects around a moby when a nearby position is valid. */
/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002C57E8), where it is exact; names translated to the US level program. */

extern float D_0015ED6C, D_0015EE70;
extern int   FUN_L00_001f0d60_q(float, void *, int, void *) __asm__("FUN_L00_001f0d60");
extern void *spawn_feather_moby(void *, void *, void *) __asm__("FUN_L00_002cbf68");
extern void  build_spherical_offset(float, float, float, void *) __asm__("FUN_00214db0");

#include "rnc/overlay/l16/mobys/chicken_relocator_globals.h"

extern float reposition_effect_velocity_elevation_degrees_gp __asm__("D_L16_001618E8")
    __attribute__((sda));
extern float reposition_effect_velocity_scale_range_start_gp __asm__("D_L16_001618EC")
    __attribute__((sda));
extern float reposition_effect_velocity_scale_range_end_gp __asm__("D_L16_001618F0")
    __attribute__((sda));
extern float reposition_effect_spawn_z_offset_gp __asm__("D_L16_001618F4") __attribute__((sda));
extern float reposition_effect_radius_range_start_gp __asm__("D_L16_001618F8") __attribute__((sda));
extern float reposition_effect_radius_range_end_gp __asm__("D_L16_001618FC") __attribute__((sda));
void         reposition_moby_with_collision_check(struct Moby *moby,
                                                  void        *new_position) __asm__("FUN_L16_002c44f0");

void reposition_moby_with_collision_check(struct Moby *moby, void *new_position) {
    float origin[4];
    float pos[4];
    float effect_velocity[4];
    float effect_spawn_position[4];
    float effect_position_angle, random_effect_angle, radius, speed;
    int   i;
    char *moby_data;
    *(OvlQuad *)origin = *(OvlQuad *)new_position;
    {
        float *start = origin;
        qcopy(&moby->pos, start);
        qcopy(pos, start);
    }
    origin[2] += 0.3f;
    if (FUN_L00_001f0d60_q(0.2f, pos, 0, moby)) {
        float *effectPosition = effect_spawn_position;
        float *effectVelocity = effect_velocity;
        i = 3;
        do {
            radius = random_float_between(reposition_effect_radius_range_start_gp,
                                          reposition_effect_radius_range_end_gp);
            effect_position_angle = random_angle_radians();
            random_effect_angle = random_angle_radians();
            build_spherical_offset(
                radius, effect_position_angle, random_effect_angle, effectPosition);
            effect_spawn_position[2] += reposition_effect_spawn_z_offset_gp;
            fast_vec_add(effectPosition, effectPosition, &moby->pos);
            speed =
                random_float_between(reposition_effect_velocity_scale_range_start_gp * D_0015ED6C,
                                     reposition_effect_velocity_scale_range_end_gp * D_0015ED6C);
            random_effect_angle = random_angle_radians();
            build_spherical_offset(speed,
                                   random_effect_angle,
                                   reposition_effect_velocity_elevation_degrees_gp * DEG_TO_RAD,
                                   effectVelocity);
            speed = random_float_between(3.5f, 5.5f);
            effect_velocity[2] = speed * D_0015ED6C;
            spawn_feather_moby(moby, effectPosition, effectVelocity);
        } while (--i >= 0);
        allocate_voice_for_target_entry(1, 0, moby);
    } else {
        moby_data = (char *)moby->pvars;
        *(float *)(moby_data + 0x78) = 1.0f;
        if (moby->prev_seq != 2)
            blend_moby_animation(moby, 2, 0, 1);
        moby->flags = (moby->flags | 0x1000) & 0xFFBE;
        moby->collision = moby->pclass->collision;
        moby->state = 4;
        *(short *)(moby_data + 0x66) = scale_game_frames(0xB4);
    }
}

/* Check whether this vendor moby is in state six. */
/* Ported from rac1-decomp (src/overlays/l16/vendor_002A50F0.c: func_L16_002C5A08), where it is exact; names translated to the US level program. */

typedef struct {
    char          pad0[0x20];
    unsigned char state;
} Level16VendorMoby;

int is_moby_in_state_6(Level16VendorMoby *moby) __asm__("FUN_L16_002c4710");

int is_moby_in_state_6(Level16VendorMoby *moby) {
    return moby->state == 6;
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E2248), where it is exact; names translated to the US level program. */

void request_target_reposition(struct Moby *moby) __asm__("FUN_L16_002e0de0");

void request_target_reposition(struct Moby *moby) {
    int state = 1;
    if (moby->state == state) {
        allocate_voice_for_target_entry(0, 0, moby);
        state = 7;
        moby->state = state;
    }
}

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002A50F0.c: func_L16_002E2290), where it is exact; names translated to the US level program. */

int get_target_reposition_status(char *moby_or_private_data) __asm__("FUN_L16_002e0e28");

int get_target_reposition_status(char *moby_or_private_data) {
    unsigned char moby_state = moby_or_private_data[0x20];
    moby_or_private_data = *(char **)(moby_or_private_data + 0x78);
    if (moby_state == 7 || moby_state == 8) {
        if (moby_state == 8 && *(float *)(moby_or_private_data + 0x34) > 0.2f) {
            return 2;
        }
        return 1;
    }
    return 0;
}

extern int scale_game_frames(int);

extern char *D_L16_0015FFD8;

/* Ported from rac1-decomp (src/overlays/l16_kalebo3/vendor_002E7C70.c: func_L16_002E9DE8), where it is exact; names translated to the US level program. */

char *find_group_chicken_in_state_6(int moby_group_index) __asm__("FUN_L16_002e8b08");

char *find_group_chicken_in_state_6(int idx) __asm__("FUN_L16_002e8b08");
f32   probe_ground_height(void *arg0, s32 arg1, f32 arg2) __asm__("FUN_00213508");
s32   is_point_inside_clip_volume(s32 arg0, s32 arg1) __asm__("FUN_00214720");

void chicken_relocator_update(struct Moby *moby) __asm__("FUN_L16_002e8970");

/* Moby class 1923 */
void chicken_relocator_update(struct Moby *moby) {
    enum {
        TARGET_CYCLE_INITIALIZE_DELAY = 0,
        TARGET_CYCLE_WAIT_TO_TRIGGER = 1,
        TARGET_CYCLE_POLL_STATUS = 2
    };
    enum {
        TARGET_STATUS_INACTIVE = 0,
        TARGET_STATUS_REPOSITION_READY = 2
    };
    char *hero_position = (char *)&hero.motion.pos;
    char *target_cycle_data = (char *)moby->pvars;
    char *target;
    int   target_status;
    float pos[4];
    if (!is_point_inside_clip_volume(hero_position, *(int *)(target_cycle_data + 8)))
        return;
    target = D_L16_0015FFD8 + (*(int *)(target_cycle_data + 4) << 8);
    switch (moby->state) {
    case TARGET_CYCLE_INITIALIZE_DELAY:
        moby->state = 1;
        *(int *)(target_cycle_data + 0xC) = scale_game_frames(random_int_between(0xB4, 0x168));
        break;
    case TARGET_CYCLE_WAIT_TO_TRIGGER:
        if (vector_distance_xy(hero_position, target + 0x10) > 3.0f) {
            if (find_group_chicken_in_state_6(*(int *)target_cycle_data) != 0) {
                if (fast_dec_timer(target_cycle_data + 0xC)) {
                    moby->state = 2;
                    request_target_reposition(target);
                    *(int *)(target_cycle_data + 0xC) =
                        scale_game_frames(random_int_between(0xB4, 0x168));
                }
            }
        }
        break;
    case TARGET_CYCLE_POLL_STATUS:
        target_status = get_target_reposition_status(target);
        switch (target_status) {
        case TARGET_STATUS_INACTIVE:
            moby->state = 1;
            break;
        case TARGET_STATUS_REPOSITION_READY:
            moby->state = 1;
            {
                char *moby_to_reposition = find_group_chicken_in_state_6(*(int *)target_cycle_data);
                if (moby_to_reposition != 0) {
                    qcopy(pos, target + 0x10);
                    pos[2] += 1.0f;
                    pos[2] = probe_ground_height(pos, 0, 0.5f);
                    reposition_moby_with_collision_check(moby_to_reposition, pos);
                }
            }
            break;
        }
        break;
    }
}

extern short *D_L16_001ABCC0[];

char *find_group_chicken_in_state_6(int moby_group_index) {
    short *moby_index_entry = D_L16_001ABCC0[moby_group_index];
    char  *moby;
    if (moby_index_entry == 0)
        return 0;
    for (;;) {
        moby = D_L16_0015FFD8 + ((*(unsigned short *)moby_index_entry & 0x7FFF) << 8);
        if (MOBY(moby)->oclass == 0x10E && is_moby_in_state_6(moby))
            return moby;
        if (*moby_index_entry++ < 0)
            return 0;
    }
}
