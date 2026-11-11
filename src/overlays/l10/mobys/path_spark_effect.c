/* Level 10 moby class path_spark_effect. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/math/quad.h"
#include "rnc/overlay/shared/mobys/test_path_segment_strict_crossing.h"

extern struct Moby *nearby_mobys[] __asm__("D_L10_00178380");
extern int collect_mobies_in_sphere(void *, float, int, void *, void *) __asm__("FUN_L00_001f2868");
/* Advances an effect, scales its size over its lifetime and emits hits. */
/* Ported from rac1-decomp (src/overlays/l10_orxon/vendor_00296BD8.c: func_L10_002DE798), where it is exact; names translated to the US level program. */

typedef struct {
    int   owner;
    float phase, speed, size;
    int   source, timer;
    float growth;
} EffectData;

extern void
send_blast_hits(void *, void *, void *, int, int, int, int, int, float, float, float) __asm__(
    "FUN_L00_0025a9f8");

void path_spark_effect_update(char *moby) __asm__("FUN_L10_002dd3d8");

typedef struct PathSparkEffectVars {
    u8    pad_0[0x10];
    void *blast_source;
    s32   timer_14;
} PathSparkEffectVars;

/* Moby class 1090 */
void path_spark_effect_update(char *moby) {
    float spark_position_before_path_update[4], blast_hit_position[4], path_segment_index,
        path_segment_remainder;
    PathSparkEffectVars *effect_data = (PathSparkEffectVars *)MOBY(moby)->pvars;
    *(OvlQuad *)spark_position_before_path_update = *(OvlQuad *)&MOBY(moby)->pos;
    ((EffectData *)effect_data)->phase += ((EffectData *)effect_data)->speed;
    ((EffectData *)effect_data)->speed *=
        (((EffectData *)effect_data)->growth - 1.0f) * frame_scale + 1.0f;
    path_sample_at_distance(((EffectData *)effect_data)->owner,
                            &path_segment_index,
                            &path_segment_remainder,
                            moby + 0x10,
                            ((EffectData *)effect_data)->phase,
                            ((EffectData *)effect_data)->size);
    MOBY(moby)->rot.x = random_angle_radians();
    MOBY(moby)->rot.y = random_angle_radians();
    MOBY(moby)->rot.z = random_angle_radians();
    if (fast_dec_timer(&effect_data->timer_14)) {
        mark_moby_for_removal(moby);
        return;
    }
    if (effect_data->timer_14 < truncate_float_to_s32(10.0f / (frame_time * 5.0f)) / 2) {
        float numerator = convert_integer_to_float(effect_data->timer_14);
        MOBY(moby)->scale =
            MOBY(moby)->pclass->scale * numerator /
            convert_integer_to_float(truncate_float_to_s32(10.0f / (frame_time * 5.0f)) / 2) * 0.5f;
    } else if ((truncate_float_to_s32(10.0f / (frame_time * 5.0f)) / 4) * 3 <
               effect_data->timer_14) {
        float numerator = convert_integer_to_float(
            truncate_float_to_s32(10.0f / (frame_time * 5.0f)) - effect_data->timer_14);
        MOBY(moby)->scale =
            MOBY(moby)->pclass->scale * numerator /
            convert_integer_to_float(truncate_float_to_s32(10.0f / (frame_time * 5.0f)) / 4) * 0.5f;
    }
    if (MOBY(moby)->pclass->scale * 0.25f < MOBY(moby)->scale) {
        int nearby_moby_count =
            collect_mobies_in_sphere(moby + 0x10, 0.15f, 16, effect_data->blast_source, 0);
        *(OvlQuad *)blast_hit_position = *(OvlQuad *)&MOBY(moby)->pos;
        send_blast_hits(effect_data->blast_source,
                        blast_hit_position,
                        nearby_mobys,
                        nearby_moby_count,
                        0,
                        0x810001,
                        2,
                        1,
                        2.0f,
                        1.0f,
                        1.0f);
    }
}
