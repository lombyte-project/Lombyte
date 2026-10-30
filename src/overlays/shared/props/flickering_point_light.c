/* Shared moby class 0x5E0: a point light that blends between random colours of its keyframe list and fades out with the camera distance. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/quad.h"
#include "qcopy.h"
#include "qzero.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/level_labels.h"

extern void interpolate_vector(void *, void *, void *, f32) __asm__("FUN_L00_0025b6b8");
extern s32
allocate_colored_effect_slot(void *, f32, f32, f32, f32, f32) __asm__("FUN_L00_0023e738");
extern void release_point_light_slot(s32) __asm__("FUN_L00_0023e838");
extern void mark_moby_for_removal(void *) __asm__("FUN_0020c828");
extern char camera_pos[] __asm__("D_L01_00167240");

extern char point_light_slot_parameters[] __asm__("D_L01_00180740");
void        update_moby_interpolated_point_light(u8 *moby) __asm__("FUN_L01_0030b618");

void update_moby_interpolated_point_light(u8 *moby) {
    enum {
        EFFECT_STATE_INITIALIZE = 0,
        EFFECT_STATE_UPDATE = 1
    };
    s32     state = moby[0x20];
    char   *effect_vars = ((char *)MOBY(moby)->pvars);
    s32     i;
    s32     current;
    s32     next;
    s32     slot;
    f32     elapsed;
    f32     active;
    f32     phase;
    f32     blend;
    f32     cutoff;
    OvlQuad first;
    OvlQuad second;

    switch (state) {
    case EFFECT_STATE_INITIALIZE:
        if (*(s32 *)(effect_vars + 0x58) < 2) {
            mark_moby_for_removal(moby);
            return;
        }
        {
            s32 i;
            for (i = 0; i < *(s32 *)(effect_vars + 0x58); i++) {
                f32 *point = ((f32 *)effect_vars) + i * 4;
                point[0] *= 0.00390625f;
                point[1] *= 0.00390625f;
                point[2] *= 0.00390625f;
                point[3] *= 0.00390625f;
            }
        }
        *(s32 *)(effect_vars + 0x68) = -1;
        if (moby[0x30] < truncate_float_to_s32(*(f32 *)(effect_vars + 0x5c)) + 4)
            moby[0x30] = truncate_float_to_s32(*(f32 *)(effect_vars + 0x5c)) + 4;
        moby[0x20] = 1;
        return;
    case EFFECT_STATE_UPDATE:
        break;
    default:
        return;
    }

    elapsed = vector_distance_xy(moby + 0x10, camera_pos);
    active = elapsed < *(f32 *)(effect_vars + 0x5c) ? 1.0f : 0.0f;
    if (active != 0.0f && *(s32 *)(effect_vars + 0x80) != -1) {
        if (!is_point_inside_clip_volume(camera_pos, *(s32 *)(effect_vars + 0x80)))
            active = 0.0f;
    }
    i = *(s32 *)(effect_vars + 0x84);
    if (i != -1 && *(s8 *)((ents + 0x20) + i * 0x100) < 0) {
        slot = *(s32 *)(effect_vars + 0x68);
        if (slot != -1)
            release_point_light_slot(slot);
        mark_moby_for_removal(moby);
        return;
    }
    if (active != 0.0f) {
        if (fast_dec_timer(effect_vars + 0x6c)) {
            current =
                random_int_between(*(s32 *)(effect_vars + 0x50), *(s32 *)(effect_vars + 0x54));
            *(s32 *)(effect_vars + 0x74) = *(s32 *)(effect_vars + 0x78);
            *(s32 *)(effect_vars + 0x70) = current;
            *(s32 *)(effect_vars + 0x6c) = current;
            next = random_int_between(1, *(s32 *)(effect_vars + 0x58) - 1);
            i = (*(s32 *)(effect_vars + 0x78) + next) % *(s32 *)(effect_vars + 0x58);
            *(s32 *)(effect_vars + 0x7c) = *(s32 *)(effect_vars + 0x7c) == 0;
            *(s32 *)(effect_vars + 0x78) = i;
        }
        phase =
            convert_integer_to_float(*(s32 *)(effect_vars + 0x70) - *(s32 *)(effect_vars + 0x6c));
        phase = phase / convert_integer_to_float(*(s32 *)(effect_vars + 0x70));
        phase *= 3.1415927f;
        blend = phase * 0.5f;
        if (*(s32 *)(effect_vars + 0x7c) != 0)
            blend += 1.5707964f;
        blend = fast_sin(blend);

        if (*(s32 *)(effect_vars + 0x7c) != 0) {
            first = ((OvlQuad *)effect_vars)[*(s32 *)(effect_vars + 0x78)];
            second = ((OvlQuad *)effect_vars)[*(s32 *)(effect_vars + 0x74)];
            interpolate_vector(effect_vars + 0x40, &first, &second, blend);
        } else {
            first = ((OvlQuad *)effect_vars)[*(s32 *)(effect_vars + 0x74)];
            second = ((OvlQuad *)effect_vars)[*(s32 *)(effect_vars + 0x78)];
            interpolate_vector(effect_vars + 0x40, &first, &second, blend);
        }
        cutoff = *(f32 *)(effect_vars + 0x5c) * 0.75f;
        {
            f32 denominator = *(f32 *)(effect_vars + 0x5c) - cutoff;
            if (cutoff < elapsed) {
                f32 fade = (elapsed - cutoff) / denominator;
                qzero(&first);
                second = *(OvlQuad *)(effect_vars + 0x40);
                interpolate_vector(effect_vars + 0x40, &second, &first, fade);
            }
        }
        slot = *(s32 *)(effect_vars + 0x68);
        if (slot == -1) {
            slot = allocate_colored_effect_slot(moby + 0x10,
                                                *(f32 *)(effect_vars + 0x60),
                                                *(f32 *)(effect_vars + 0x4c),
                                                *(f32 *)(effect_vars + 0x40),
                                                *(f32 *)(effect_vars + 0x44),
                                                *(f32 *)(effect_vars + 0x48));
            *(s32 *)(effect_vars + 0x68) = slot;
            return;
        }
        {
            char *effect_entry = point_light_slot_parameters + slot * 0x20;
            qcopy(effect_entry + 0x10, moby + 0x10);
            *(f32 *)(effect_entry + 0x1c) = *(f32 *)(effect_vars + 0x60);
            qcopy(effect_entry, effect_vars + 0x40);
        }
    } else {
        slot = *(s32 *)(effect_vars + 0x68);
        if (slot != -1) {
            release_point_light_slot(slot);
            *(s32 *)(effect_vars + 0x68) = -1;
        }
    }
}
