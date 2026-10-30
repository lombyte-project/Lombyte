/* Level 03 moby class cutscene_effects (0x60c): starts the cutscene, then emits two particle beams from the listed moby. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/rendering/effects/update_three_phase_effect_particle.h"

/* Cutscene state at D_L03_0016C960 (same layout as G_2da710 in l03/mobys/helga.c). */
struct L03Cutscene {
    char  pad0[0x30];
    int   mode;
    int   t;
    char  pad38[0x140];
    char *mobys[4];
};

extern struct L03Cutscene D_L03_0016C960;

/* Starts the cutscene, then during mode 3 (frames 900-10000) emits two particle beams from the listed moby. */
void update_kerwan_cutscene_effects(struct Moby *owner_moby) __asm__("FUN_L03_002e01d0");

void update_kerwan_cutscene_effects(struct Moby *owner_moby) {
    enum {
        CUTSCENE_EFFECTS_INITIALIZE = 0,
        CUTSCENE_EFFECTS_UPDATE = 1
    };
    char *moby;
    float from[4];
    float to[4];
    Vec4  zero;

    switch (owner_moby->state) {
    case CUTSCENE_EFFECTS_INITIALIZE:
        owner_moby->update_dist = 0xFF;
        owner_moby->state = 1;
        break;
    case CUTSCENE_EFFECTS_UPDATE:
        if (D_L03_0015F5C4 != 2)
            break;
        if (D_L03_0016C960.mode == 4 || D_L03_0016C960.mode == 10) {
            int cutscene_mode = D_L03_0016C960.mode;
            int i;
            i = 0;
            if (cutscene_mode == 4)
                i = 2;
            else if (cutscene_mode == 10)
                i = 0;
            cutscene_set_camera_target(D_L03_0016C960.mobys[i]);
        }
        if (D_L03_0015F5C4 != 2 || D_L03_0016C960.mode != 3)
            break;
        if (D_L03_0016C960.t < scale_game_frames(900))
            break;
        if (D_L03_0016C960.t > scale_game_frames(10000))
            break;
        moby = D_L03_0016C960.mobys[3];
        if (moby == 0)
            break;
        zero.q = 0;
        moby_point_position(moby, 0, from);
        moby_point_position(moby, 1, to);
        spawn_effect_particle_with_target_scalar(from,
                                                 &zero,
                                                 0x7F7F7F7F,
                                                 0x7F7F7F,
                                                 random_float_between(0.25f, 0.5f) * 210000.0f,
                                                 scale_game_frames(random_int_between(45, 60)));
        spawn_effect_particle_with_target_scalar(to,
                                                 &zero,
                                                 0x407F7F7F,
                                                 0x7F7F7F,
                                                 random_float_between(0.25f, 0.5f) * 210000.0f,
                                                 scale_game_frames(random_int_between(45, 60)));
        break;
    }
}
