/* Level 00 moby classes: clank_cutscene_trigger. */
/* Mostly per-level sky drawing; also holds the per-level moby hooks and a door moby update. */
#include "types.h"
#include "rnc/rendering/sky_shell.h"
#include "rnc/rendering/sky_particles.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/game/item_state.h"
#include "rnc/math/random.h"
#include "rnc/math/spherical_offset.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/audio/streaming/initialize_npc_dialogue.h"
#include "rnc/overlay/shared/audio/streaming/update_npc_dialogue.h"
#include "rnc/overlay/shared/rendering/sky/update_sky_shells_animated.h"
#include "rnc/rendering/sky/do_sky_gif_paging.h"
#include "rnc/rendering/sky/setup_sky_gif_paging.h"
#include "rnc/rendering/sky/sky_sprite_proc.h"
#include "rnc/overlay/shared/rendering/sky/sky_particle_effects.h"
#include "rnc/overlay/shared/rendering/sky/sky_shells_rotate_two.h"
#include "qzero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

void draw_sky_shells_with_particles_and_rotating_shell(void) __asm__("FUN_L00_00289330");

extern void draw_sky_shells_with_particles_and_rotating_shell(void) __asm__("FUN_L00_00289330");
extern void update_sky_shells_animated(void) __asm__("FUN_L00_00289420");
extern void draw_sky_shells_with_fast_and_slow_rotation(void) __asm__("FUN_L00_00289a90");
extern void
draw_sky_shells_with_particles_and_modulo_50000_rotation(void) __asm__("FUN_L00_00289bd8");
extern void
draw_three_sky_shells_with_particles_and_modulo_50000_rotation(void) __asm__("FUN_L00_0028a208");
extern void draw_sky_shells_with_modulo_40000_rotation(void) __asm__("FUN_L00_0028a2f0");
extern void draw_sky_shells_with_modulo_25000_rotation(void) __asm__("FUN_L00_0028a3a8");
extern void draw_sky_shells_with_scaled_phase_rotation(void) __asm__("FUN_L00_0028a480");
extern void draw_sky_shells_with_slow_medium_and_fast_rotation(void) __asm__("FUN_L00_0028a620");
extern void FUN_L00_0028a7a0(void);
extern void FUN_L00_0028ab08(void);
extern void draw_three_sky_shells_with_particles(void) __asm__("FUN_L00_0028a920");
extern void
draw_three_sky_shells_with_modulo_50000_and_100000_rotation(void) __asm__("FUN_L00_0028a998");
extern void draw_two_sky_shells_with_particles(void) __asm__("FUN_L00_0028aa98");
extern void
draw_seven_sky_shells_with_modulo_40000_and_60000_rotation(void) __asm__("FUN_L00_0028b080");
extern void draw_sky_shells_with_rotation_and_scaling(void) __asm__("FUN_L00_0028b1a0");

void draw_level_sky(void) __asm__("FUN_L00_0023d968");

void draw_level_sky(void) {
    enum {
        LEVEL_WITH_SKY_PARTICLES_AND_ROTATING_SHELL = 0,
        LEVEL_WITH_THREE_SKY_SHELL_ROTATION_RATES = 1,
        LEVEL_WITH_ANIMATED_SKY_PARTICLE_POSITIONS = 2,
        LEVEL_WITH_TWO_ROTATING_SKY_SHELLS = 3,
        LEVEL_WITH_FAST_AND_SLOW_ROTATING_SKY_SHELLS = 4,
        LEVEL_WITH_MODULO_50000_SKY_SHELL_ROTATION = 5,
        LEVEL_WITH_THREE_SKY_SHELLS_AND_THREE_PARTICLE_TYPES = 6,
        LEVEL_WITH_SKY_PARTICLES_AND_MODULO_50000_SHELL_ROTATION = 7,
        LEVEL_WITH_MODULO_40000_SKY_SHELL_ROTATION = 8,
        LEVEL_WITH_MODULO_25000_SKY_SHELL_ROTATION = 9,
        LEVEL_WITH_FOUR_ROTATING_SKY_SHELLS = 10,
        LEVEL_WITH_THREE_ROTATING_SKY_SHELLS = 11,
        LEVEL_WITH_ROTATING_SKY_SHELLS_ONE_THROUGH_THREE = 12,
        LEVEL_WITH_THREE_SKY_SHELLS_AND_PARTICLES = 13,
        LEVEL_WITH_MODULO_50000_AND_100000_SKY_SHELL_ROTATIONS = 14,
        LEVEL_WITH_TWO_SKY_SHELLS_AND_PARTICLES = 15,
        LEVEL_WITH_SLOW_MEDIUM_AND_FAST_ROTATING_SKY_SHELLS = 16,
        LEVEL_WITH_THREE_SKY_SHELLS_AND_TWO_PARTICLE_TYPES = 17,
        LEVEL_WITH_MODULO_40000_AND_60000_SKY_SHELL_ROTATIONS = 18,
        LEVEL_WITH_MODULO_10000_SKY_SHELL_ROTATIONS = 19
    };
    setup_sky_gif_paging();
    switch (current_level_index) {
    case LEVEL_WITH_SKY_PARTICLES_AND_ROTATING_SHELL:
        draw_sky_shells_with_particles_and_rotating_shell();
        break;
    case LEVEL_WITH_THREE_SKY_SHELL_ROTATION_RATES:
        update_sky_shells_animated();
        break;
    case LEVEL_WITH_ANIMATED_SKY_PARTICLE_POSITIONS:
        draw_four_sky_shells_with_animated_particles();
        break;
    case LEVEL_WITH_TWO_ROTATING_SKY_SHELLS:
        sky_shells_rotate_two();
        break;
    case LEVEL_WITH_FAST_AND_SLOW_ROTATING_SKY_SHELLS:
        draw_sky_shells_with_fast_and_slow_rotation();
        break;
    case LEVEL_WITH_MODULO_50000_SKY_SHELL_ROTATION:
        draw_sky_shells_with_particles_and_modulo_50000_rotation();
        break;
    case LEVEL_WITH_THREE_SKY_SHELLS_AND_THREE_PARTICLE_TYPES:
        draw_three_sky_shells_with_three_particle_types();
        break;
    case LEVEL_WITH_SKY_PARTICLES_AND_MODULO_50000_SHELL_ROTATION:
        draw_three_sky_shells_with_particles_and_modulo_50000_rotation();
        break;
    case LEVEL_WITH_MODULO_40000_SKY_SHELL_ROTATION:
        draw_sky_shells_with_modulo_40000_rotation();
        break;
    case LEVEL_WITH_MODULO_25000_SKY_SHELL_ROTATION:
        draw_sky_shells_with_modulo_25000_rotation();
        break;
    case LEVEL_WITH_FOUR_ROTATING_SKY_SHELLS:
        draw_sky_shells_with_scaled_phase_rotation();
        break;
    case LEVEL_WITH_THREE_ROTATING_SKY_SHELLS:
        draw_sky_shells_with_slow_medium_and_fast_rotation();
        break;
    case LEVEL_WITH_ROTATING_SKY_SHELLS_ONE_THROUGH_THREE:
        FUN_L00_0028a7a0();
        break;
    case LEVEL_WITH_THREE_SKY_SHELLS_AND_PARTICLES:
        draw_three_sky_shells_with_particles();
        break;
    case LEVEL_WITH_MODULO_50000_AND_100000_SKY_SHELL_ROTATIONS:
        draw_three_sky_shells_with_modulo_50000_and_100000_rotation();
        break;
    case LEVEL_WITH_TWO_SKY_SHELLS_AND_PARTICLES:
        draw_two_sky_shells_with_particles();
        break;
    case LEVEL_WITH_SLOW_MEDIUM_AND_FAST_ROTATING_SKY_SHELLS:
        FUN_L00_0028ab08();
        break;
    case LEVEL_WITH_THREE_SKY_SHELLS_AND_TWO_PARTICLE_TYPES:
        render_three_shell_particle_sky();
        break;
    case LEVEL_WITH_MODULO_40000_AND_60000_SKY_SHELL_ROTATIONS:
        draw_seven_sky_shells_with_modulo_40000_and_60000_rotation();
        break;
    case LEVEL_WITH_MODULO_10000_SKY_SHELL_ROTATIONS:
        draw_sky_shells_with_rotation_and_scaling();
        break;
    default:
        update_sky_shells_static();
        break;
    }
    do_sky_gif_paging();
    vu1_add_g_sregister(0x47, 0x5360B);
    vu1_add_g_sregister(0x4E, 0x1000000 | (depth_buffer_address >> 13));
}

#include "eetypes.h"

typedef struct {
    u8  pad0[0x34];
    u16 flags;
    u8  pad36[0x70];
    s16 oClass;
} VeldinMoby;

typedef struct {
    u8          pad0[0x30];
    s32         state;
    s32         timer;
    u8          pad38[0xC];
    s16         count;
    u8          pad46[0x132];
    VeldinMoby *mobys[1];
    VeldinMoby *unk17C;
    u8          pad180[4];
    VeldinMoby *unk184;
    VeldinMoby *unk188;
} VeldinLevelState;

typedef union {
    u128 q;
    f32  f[4];
} VeldinVec;

extern void glow_quads_draw() __asm__("FUN_L00_0023db30");
extern void FUN_L00_002935b8();
extern void emit_moby_particle_pairs(VeldinMoby *) __asm__("FUN_L00_0023e008");
extern void FUN_L00_00257024(void *);
extern void spawn_effect_particle_with_target_scalar(void *, void *, s32, s32, s32, f32) __asm__("FUN_L00_0026ced0");
extern VeldinLevelState D_L00_0016C860;

void apply_level_specific_cutscene_moby_effects(void) __asm__("FUN_L00_0023e268");

void apply_level_specific_cutscene_moby_effects(void) {
    VeldinMoby *particle_source_moby;
    VeldinMoby *mb;
    char       *joint_position_source_moby;
    s32         i;
    s32         j;
    s32         k;
    s32         effect_spawn_attempt_index;
    f32         azimuth_angle;
    f32         elevation_angle;
    s32         r;
    VeldinVec   joint_world_position;
    VeldinVec   spherical_offset;

    if (current_level_index == 0) {
        for (i = 0; i < D_L00_0016C860.count; i++) {
            if (D_L00_0016C860.mobys[i]->oClass == 0x212) {
                add_draw_callback((s32)FUN_L00_002935b8, (s32)D_L00_0016C860.mobys[i]);
            }
        }
    }
    if (current_level_index == 2) {
        for (j = 0; j < D_L00_0016C860.count; j++) {
            mb = D_L00_0016C860.mobys[j];
            if (mb->oClass == 0x1B1) {
                mb->flags |= 0x800;
            }
        }
    }
    if (current_level_index == 3 && D_L00_0016C860.state == 5 &&
        scale_game_frames(900) < D_L00_0016C860.timer &&
        D_L00_0016C860.timer < scale_game_frames(1100)) {
        joint_position_source_moby = (char *)D_L00_0016C860.unk184;
        if (joint_position_source_moby != 0) {
            for (effect_spawn_attempt_index = 0; effect_spawn_attempt_index < 4;
                 effect_spawn_attempt_index++) {
                clear_u64_value(&spherical_offset);
                moby_point_position(joint_position_source_moby,
                                    effect_spawn_attempt_index % 2,
                                    &joint_world_position);
                azimuth_angle = random_angle_radians();
                elevation_angle = random_angle_radians();
                build_spherical_offset(spherical_offset.f,
                                       random_float_between(frame_time * 0.7f, frame_time * 2.2f),
                                       azimuth_angle,
                                       elevation_angle);
                r = scale_game_frames(0xC);
                spawn_effect_particle_with_target_scalar(
                    &joint_world_position,
                    &spherical_offset,
                    0x80808080,
                    0x808080,
                    random_int_between(r, scale_game_frames(0x23)),
                    147000.0f);
            }
        }
    }
    if (current_level_index == 7 && D_L00_0016C860.state == 4 && D_L00_0016C860.unk188 != 0 &&
        D_L00_0016C860.unk188->oClass == 0x214) {
        add_draw_callback((s32)FUN_L00_002935b8, (s32)D_L00_0016C860.unk188);
    }
    if (current_level_index == 14 && D_L00_0016C860.state == 0) {
        particle_source_moby = D_L00_0016C860.unk17C;
        if (particle_source_moby != 0 && (u32)(D_L00_0016C860.timer - 0x1C) < 6) {
            emit_moby_particle_pairs(particle_source_moby);
            add_draw_callback((s32)glow_quads_draw, (s32)particle_source_moby);
        }
    }
    if (current_level_index == 15 && (u32)(D_L00_0016C860.state - 3) < 2) {
        for (k = 0; k < D_L00_0016C860.count; k++) {
            FUN_L00_00257024(D_L00_0016C860.mobys[k]);
        }
    }
}

typedef union {
    u128 q;
    f32  f[4];
} SkyRotVec;

void draw_sky_shells_with_particles_and_rotating_shell(void) {
    SkyRotVec sky_shell_rotation_radians;

    *(short *)(D_L00_0016051C + 4) = 0;
    sky_draw_shell(0);
    sky_draw_shell(1);
    if (*(short *)(D_L00_0016051C + 8) == 0)
        sky_particles_init(0xF4, 0xC);
    sky_particles_update();
    sky_sprite_proc();
    vu1_add_g_sregister(0x42, 0x8000000044L);
    sky_draw_shell(2);
    qzero(&sky_shell_rotation_radians);
    sky_shell_rotation_radians.f[2] =
        (f32)(*(volatile int *)&D_L00_0015F5CC & 0x7FFF) * 1.9174760e-4f - 3.1415927f;
    euler_to_matrix_ee(D_L00_001BD7E0, &sky_shell_rotation_radians);
    sky_draw_shell(3);
    set_identity_matrix(D_L00_001BD7E0);
    sky_draw_shell(4);
}

typedef struct {
    u8  pad0[0x20];
    u8  unk20[4];
    s16 unk24;
    u8  pad26[6];
    f32 unk2C;
    u8  pad30[0x26];
    s16 unk56;
    u8  pad58[8];
    s32 unk60;
} DoorVars;

typedef struct {
    u8        pad0[0x20];
    u8        state;
    u8        pad21[0xF];
    u8        alpha;
    u8        unk31;
    u8        pad32[2];
    u16       flags;
    u8        pad36[0x42];
    DoorVars *vars;
} DoorMoby;

extern s16  D_00141628[];
extern void initialize_global_state_entry(s32) __asm__("FUN_0022e188");

void clank_cutscene_trigger_update(DoorMoby *m) __asm__("FUN_L00_002d9dc8");

void clank_cutscene_trigger_update(DoorMoby *moby) {
    enum {
        CLANK_TRIGGER_INITIALIZE_DIALOGUE = 0,
        CLANK_TRIGGER_POLL_DIALOGUE = 1,
        CLANK_TRIGGER_WAIT_TO_REPOLL_DIALOGUE = 2
    };
    DoorVars *vars;

    vars = moby->vars;
    if (D_L00_0015F5C4 == 2) {
        moby->unk31 = 0;
        moby->flags |= 1;
    } else {
        moby->unk31 = 1;
        moby->flags &= ~1;
    }
    switch (moby->state) {
    case CLANK_TRIGGER_INITIALIZE_DIALOGUE:
        D_00141628[0] = 1;
        initialize_npc_dialogue(moby, vars->unk20);
        if (alternate_item_available[8] != 0) {
            vars->unk56 = 1;
            vars->unk2C = 3.0f;
            vars->unk24 = 0;
        } else {
            vars->unk2C = 255.0f;
        }
        alternate_item_available[8] = 1;
        moby->alpha = 0xFF;
        moby->state = 1;
        break;
    case CLANK_TRIGGER_POLL_DIALOGUE:
        if (vars->unk56 != 0) {
            if (is_point_inside_clip_volume((s32)(&hero.motion.pos), vars->unk60) != 0) {
                vars->unk2C = 16.0f;
            } else {
                vars->unk2C = 3.0f;
            }
        }
        if (update_npc_dialogue(moby, vars->unk20) != 0) {
            moby->state = 2;
            vars->unk2C = 3.0f;
        }
        if (vars->unk24 >= 4 && D_L00_0015F5C4 != 6) {
            initialize_global_state_entry(1);
        }
        break;
    case CLANK_TRIGGER_WAIT_TO_REPOLL_DIALOGUE:
        if (D_L00_0015F5C4 != 2) {
            moby->state = 1;
        }
        break;
    }
}
