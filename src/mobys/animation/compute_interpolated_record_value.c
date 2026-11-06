#include "types.h"

/* The animation fields of struct Moby (0x50..0x6F). */
struct InterpolatedStateEntry {
    u8                      pad_0[0x50];
    u8                      frame;
    u8                      prev_frame;
    u8                      seq;
    u8                      prev_seq;
    f32                     anim_time;
    u8                      pad_58[0x10];
    struct InterpKeyframeA *cur_frame_data;
    struct InterpKeyframeB *prev_frame_data;
};

struct InterpKeyframeB {
    u8  pad_0[0x4];
    s16 time; /* key time in 1/16 of a 30 Hz frame (ReRAC moby_animation.md frame header +0x4) */
};

struct InterpKeyframeA {
    u8  pad_0[0x4];
    s16 time; /* key time in 1/16 of a 30 Hz frame (ReRAC moby_animation.md frame header +0x4) */
};

extern float convert_integer_to_float() __asm__("func_001FA6C0");
f32 compute_interpolated_record_value(struct InterpolatedStateEntry *anim) __asm__("FUN_0020c9e0");

f32 compute_interpolated_record_value(struct InterpolatedStateEntry *anim) {
    struct InterpKeyframeB *next_record;
    struct InterpKeyframeA *cur_record;
    f32                     weighted_current_record_value;
    u8                      primary_binding_index, secondary_binding_index;

    if (anim->seq != 0xFF) {
        cur_record = anim->cur_frame_data;
    } else {
        cur_record = anim->prev_frame_data;
    }
    if (anim->anim_time == 0.0f) {
        return convert_integer_to_float(cur_record->time) * 0.0625f;
    }
    if (anim->seq != anim->prev_seq) {
        return (convert_integer_to_float(cur_record->time) * 0.0625f) + anim->anim_time;
    }
    primary_binding_index = anim->frame;
    secondary_binding_index = anim->prev_frame;
    if ((u8)secondary_binding_index >= (u8)primary_binding_index) {
        next_record = anim->prev_frame_data;
        weighted_current_record_value =
            convert_integer_to_float(cur_record->time) * (1.0f - anim->anim_time);
        return (weighted_current_record_value +
                (convert_integer_to_float(next_record->time) * anim->anim_time)) *
               0.0625f;
    }
    return (convert_integer_to_float(cur_record->time) * 0.0625f) + anim->anim_time;
}

extern f32 func_0020C9E0(struct InterpolatedStateEntry *) __attribute__((alias("FUN_0020c9e0")));
