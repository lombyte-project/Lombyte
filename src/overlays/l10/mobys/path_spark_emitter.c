/* Level 10 moby class path_spark_emitter: the electric spark running along its wire path. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/audio/sound_state.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"

#include "qcopy.h"
#include "rnc/overlay/quad.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/level_path.h"

extern int D_L10_0015F5CC;
/* Electric spark: runs along its wire path with a light and a buzzing sound, then rests for a random time. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DA0F0), where it is exact; names translated to the US level program. */

extern char point_light_parameter_entries[] __asm__("D_L10_00180740");

char path_spark_missing_path_message[62] __asm__("D_L10_001DD120") =
    "Moby947 maya instance %d HAS NO PATH!!!\n\n\nMoron.\nMobyDeleted\n";

extern int allocate_colored_effect_slot(float *, float, float, float, float, float) __asm__("FUN_L00_0023e738");
extern void    release_point_light_slot(int) __asm__("FUN_L00_0023e838");
unsigned char *spawn_path_spark_effect(int   source_moby,
                                       void *initial_position,
                                       int   path_address,
                                       float x,
                                       float y,
                                       float z) __asm__("FUN_L10_002dd270");

unsigned char *spawn_path_spark_effect(int a, void *pos, int b, float x, float y, float z) __asm__("FUN_L10_002dd270");
extern int func_001E9730_2da0f0(void *, int) __asm__("FUN_001e93b0");
extern int func_001F9908_v(void *) __asm__("FUN_001f9740");
extern void
func_L00_002633D8_f(float, float, void *, int *, float *, void *) __asm__("FUN_L00_00262360");
extern int trooper_play_sound(int, int, void *, int) __asm__("FUN_L00_0028dc90");

void path_spark_emitter_update(struct Moby *moby) __asm__("FUN_L10_002d8d30");

/* Moby class 947 */
void path_spark_emitter_update(struct Moby *moby) {
    enum {
        SPARK_STATE_WAIT_FOR_TIMER = 0,
        SPARK_STATE_TRAVERSE_PATH = 1
    };
    char *spark_vars = ((char *)moby->pvars);
    int  *path;
    float spark_path_position[4];
    int   seg;
    float frac;
    float first_path_segment_length;
    if (*(int *)spark_vars == -1) {
        func_001E9730_2da0f0(path_spark_missing_path_message, ((short)moby->save_id));
        mark_moby_for_removal(moby);
        return;
    }
    path = ((int *)l10_paths[*(int *)spark_vars]);
    ((unsigned char *)moby)[0x30] = 0xFF;
    switch (((unsigned char *)moby)[0x20]) {
    case SPARK_STATE_WAIT_FOR_TIMER:
        if (*(int *)(spark_vars + 0xC) != -1) {
            release_point_light_slot(*(int *)(spark_vars + 0xC));
            *(int *)(spark_vars + 0xC) = -1;
        }
        if (*(int *)(spark_vars + 0x10) != -1) {
            char *voice_entry = ((char *)&voice_pool) + *(int *)(spark_vars + 0x10) * 0x70;
            if (*(char **)(voice_entry + 0x88) == moby &&
                ((unsigned char *)voice_entry)[0x74] != 0) {
                release_voice_slot(*(int *)(spark_vars + 0x10));
            }
        }
        *(int *)(spark_vars + 0x10) = -1;
        if (func_001F9908_v(spark_vars + 8) != 0) {
            moby->state = 1;
            *(int *)(spark_vars + 4) = 0;
        }
        break;
    case SPARK_STATE_TRAVERSE_PATH:
        first_path_segment_length = fast_vec_dist((char *)path + 0x10, (char *)path + 0x20);
        *(float *)(spark_vars + 4) += frame_time * 5.0f;
        func_L00_002633D8_f(*(float *)(spark_vars + 4),
                            first_path_segment_length,
                            path,
                            &seg,
                            &frac,
                            spark_path_position);
        if (*(int *)(spark_vars + 0xC) == -1) {
            *(int *)(spark_vars + 0xC) =
                allocate_colored_effect_slot(spark_path_position, 3.0f, 0.0f, 1.0f, 1.0f, 0.5f);
        } else {
            char *light_parameter_entry =
                point_light_parameter_entries + *(int *)(spark_vars + 0xC) * 32;
            qcopy(light_parameter_entry + 0x10, spark_path_position);
            *(float *)(light_parameter_entry + 0) = 1.0f;
            *(float *)(light_parameter_entry + 0x1C) = 3.0f;
            *(float *)(light_parameter_entry + 4) = 1.0f;
            *(float *)(light_parameter_entry + 8) = 0.5f;
            *(int *)(light_parameter_entry + 0xC) = 0;
        }
        if (random_integer_below(2) != 0)
            return;
        if (D_L10_0015F5CC & 1) {
            spawn_path_spark_effect(moby,
                                    spark_path_position,
                                    path,
                                    *(float *)(spark_vars + 4),
                                    frame_time * 5.0f,
                                    first_path_segment_length);
        }
        if (voice_is_playing(moby, *(int *)(spark_vars + 0x10)) == 0) {
            int voice_slot_index = trooper_play_sound(0, 0xC, moby, 0x442);
            *(int *)(spark_vars + 0x10) = voice_slot_index;
            if (voice_slot_index != -1) {
                *(OvlQuad *)(((char *)&voice_positions) + voice_slot_index * 0x70) =
                    *(OvlQuad *)spark_path_position;
            }
        } else {
            *(OvlQuad *)(((char *)&voice_positions) + *(int *)(spark_vars + 0x10) * 0x70) =
                *(OvlQuad *)spark_path_position;
        }
        if (seg == *path - 1) {
            moby->state = 0;
            *(int *)(spark_vars + 8) = scale_game_frames(0x78);
        }
        break;
    }
}

/* Spawns a moby at a spot, fills its data block and starts its fade and sound. */
extern void glow_n(unsigned char *, int, int, int) __asm__("FUN_L00_002502f0");
extern int  FUN_001fa6d0_dd(float) __asm__("truncate_float_to_s32");
extern void spawn_source_attached_fading_effect_particle(
    unsigned char *, int, int, int, float) __asm__("FUN_L00_0026daa0");

unsigned char *spawn_path_spark_effect(

    int source_moby, void *initial_position, int path_address, float x, float y, float z) {
    unsigned char *effect_moby = create_moby(0x442);
    int           *effect_data;
    float          lifetime_frames;
    if (effect_moby != 0) {
        effect_moby[0x30] = 0xFF;
        MOBY(effect_moby)->draw_dist = 0xFF;
        effect_moby[0x31] = 1;
        effect_moby[0x23] = 0x40;
        effect_moby[0x20] = 0;
        effect_data = ((int *)MOBY(effect_moby)->pvars);
        *(OvlQuad *)&MOBY(effect_moby)->pos = *(OvlQuad *)initial_position;
        glow_n(effect_moby, 0x7F, 0x40, 0);
        lifetime_frames = 10.0f / (frame_time * 5.0f);
        effect_data[0] = path_address;
        ((float *)effect_data)[1] = x;
        ((float *)effect_data)[2] = y;
        ((float *)effect_data)[3] = z;
        effect_data[4] = source_moby;
        effect_data[5] = FUN_001fa6d0_dd(lifetime_frames);
        ((float *)effect_data)[6] = random_float_between(0.999f, 0.97f);
        spawn_source_attached_fading_effect_particle(
            effect_moby, 0x1F4F7F7F, FUN_001fa6d0_dd(10.0f / (frame_time * 5.0f)), -1, 126000.008f);
    }
    return effect_moby;
}
