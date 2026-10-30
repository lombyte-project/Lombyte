/* Level 17 moby class floating_mine (0x343): bobs in place and explodes when it is shot or touched. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/mobys/find_moby_hit.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"

extern int
hit_reaction(void *, void *, void *, int, void *, void *, int, int) __asm__("FUN_L00_0025a478");

/* Updates moby 835: bobs in place and explodes when it is shot or touched. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002AA068.c: func_L17_002DD8D0), where it is exact; names translated to the US level program. */

extern char *D_L17_00174758;
extern char *FUN_L00_0026f080(char *vec, float *pos, float sc, float vy);
extern int   collect_mobies_in_sphere() __asm__("FUN_L00_001f2868");
extern void  spawn_explosion(void *,
                             void *,
                             void *,
                             float,
                             float,
                             int,
                             int,
                             int,
                             float,
                             float,
                             float,
                             float,
                             int,
                             float,
                             int,
                             int,
                             int,
                             int) __asm__("FUN_L00_0025e450");
extern short D_L17_00161D2C __attribute__((sda));
extern short fleet_bobbing_phase_rate_degrees __asm__("D_L17_00161D30") __attribute__((sda));
extern void  update_moby_vertical_oscillation(float, float, char *, float *, float *) __asm__("FUN_L00_00262b00");
f32        resolve_camera_surface_height(struct CameraProbePosition *position,
                                         s32 optional_output) __asm__("FUN_002135f0");
extern int func_L00_001F2BE8_alt(float, void *, int, void *, void *) __asm__("FUN_L00_001f2868");

void update_floating_mine(struct Moby *moby) __asm__("FUN_L17_002dc3f8");

void update_floating_mine(struct Moby *moby) {
    enum {
        BOBBING_INITIALIZE = 0,
        BOBBING_UPDATE_AND_CHECK_CONTACT = 1
    };
    float        zero_vector[4];
    float        hit_descriptor[12];
    float        pos[4];
    float        surface_query_scratch[4];
    float        scaled_particle_vector[4];
    float        random_direction[4];
    int          out;
    char        *data = (char *)moby->pvars;
    char        *hit_record;
    struct Hero *hero_contact_context;
    float        yy;
    int          i;
    clear_u64_value(zero_vector);
    hit_record = find_moby_hit(moby, 0x230000, 0);
    hit_reaction(moby, hit_record, data + 0x20, 0, &out, 0, 0, 4);
    if (out >= 2 && hit_record != 0 && *(short *)(*(char **)(hit_record + 0x20) + 0xA6) != 0x343) {
        spawn_explosion(moby,
                        zero_vector,
                        0,
                        0.0f,
                        0.0f,
                        3,
                        3,
                        5,
                        2.0f,
                        1.0f,
                        4.0f,
                        1.0f,
                        1,
                        7.0f,
                        0,
                        1,
                        -1,
                        0);
        mark_moby_for_removal(moby);
        return;
    }
    switch (moby->state) {
    case BOBBING_INITIALIZE:
        *(float *)(data + 0x60) = random_angle_radians();
        *(int *)(data + 0x64) = 0;
        moby->state = 1;
        break;
    case BOBBING_UPDATE_AND_CHECK_CONTACT:
        update_moby_vertical_oscillation(*(float *)&D_L17_00161D2C,
                                         *(float *)&fleet_bobbing_phase_rate_degrees * DEG_TO_RAD *
                                             frame_time,
                                         moby,
                                         (float *)(data + 0x60),
                                         (float *)(data + 0x64));
        make_hit_descriptor(hit_descriptor, (int)moby, 0x10001, 1.0f, zero_vector);
        qcopy(pos, &moby->pos);
        pos[2] = pos[2] + 0.75f;
        hero_contact_context = &hero;
        if (((char *)hero_contact_context->coll_hit_moby) == moby ||
            ((char *)hero_contact_context->unk240) == moby ||
            func_L00_001F2BE8_alt(1.0f, pos, 0x10, moby, hit_descriptor)) {
            if (D_L17_00174758 != 0 && *(short *)(D_L17_00174758 + 0xA6) != 0x343) {
                spawn_explosion(moby,
                                zero_vector,
                                0,
                                0.0f,
                                0.0f,
                                3,
                                3,
                                5,
                                2.0f,
                                1.0f,
                                4.0f,
                                1.0f,
                                1,
                                7.0f,
                                0,
                                1,
                                -1,
                                0);
                moby->pos.z = moby->pos.z + 1.0f;
                yy = resolve_camera_surface_height(&moby->pos.x, surface_query_scratch);
                moby->pos.z = moby->pos.z - 1.0f;
                i = 0x95;
                do {
                    *(OvlQuad *)random_direction = 0;
                    random_direction[0] = random_float_between(-1.0f, 1.0f);
                    random_direction[1] = random_float_between(-1.0f, 1.0f);
                    random_direction[2] = random_float_between(-0.5f, 2.0f);
                    *(OvlQuad *)scaled_particle_vector = *(OvlQuad *)random_direction;
                    fast_vec_normalize(scaled_particle_vector,
                                       scaled_particle_vector,
                                       random_float_between(frame_time * 4.0f, frame_time * 8.0f));
                    FUN_L00_0026f080(&moby->pos,
                                     scaled_particle_vector,
                                     random_float_between(0.05f, 0.1f) * 210000.0f,
                                     yy);
                } while (--i >= 0);
                mark_moby_for_removal(moby);
            }
        }
        break;
    }
}
