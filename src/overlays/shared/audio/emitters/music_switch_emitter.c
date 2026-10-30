/* Sound emitter type 6: switches the music track when the hero leaves the trigger box, by the side it leaves through. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"

/* sound instance update: fade/stop test against a listener distance */
/* Ported from rac1-decomp (src/overlays/shared/vendor_0031AD00.c: func_L01_0031B500), where it is exact; names translated to the US level program. */

extern void request_music_track(int, int) __asm__("FUN_L01_0027a248");
extern void transform_vector_by_basis(void *, void *, void *);

void update_music_switch_emitter(int *moby) __asm__("FUN_L01_0031a128");

void update_music_switch_emitter(int *moby) {
    float hero_offset_vectors[8];
    int  *music_trigger_data = (int *)moby[2];
    if (((unsigned *)music_trigger_data)[3] != 0xFFFFFFFF) {
        fast_vec_sub(hero_offset_vectors, &hero.motion.pos, (char *)moby + 0x40);
        hero_offset_vectors[3] = 0;
        transform_vector_by_basis(
            hero_offset_vectors + 4, hero_offset_vectors, (char *)moby + 0x50);
        if (absolute_float(hero_offset_vectors[4]) <= 1.0f &&
            absolute_float(hero_offset_vectors[5]) <= 1.0f &&
            absolute_float(hero_offset_vectors[6]) <= 1.0f) {
            music_trigger_data[2] = 1;
            return;
        }
        if (music_trigger_data[2] != 0) {
            short *music_trigger_halfwords = (short *)music_trigger_data;
            if (music_trigger_halfwords[7] == -1)
                music_trigger_halfwords[7] = music_trigger_halfwords[6];
            if (music_trigger_halfwords[6] == -1)
                music_trigger_halfwords[6] = music_trigger_halfwords[7];
            if (hero_offset_vectors[4] > 0) {
                request_music_track(music_trigger_data[1], music_trigger_halfwords[7]);
            } else {
                request_music_track(music_trigger_data[0], music_trigger_halfwords[6]);
            }
            music_trigger_data[2] = 0;
        }
    }
}
