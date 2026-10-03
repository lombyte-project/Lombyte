#include "rnc/preview_animation.h"

extern s32 preview_request_count __asm__("D_00160350");
extern PreviewAnimationRequest preview_animation_requests[] __asm__("D_001D5EC0");

s32 queue_preview_animation(s32 animation_id, s32 trigger_mode, s32 delay_frames, s32 item_index,
                           s32 item_animation, s32 attachment0_class, s32 attachment0_animation,
                           s32 attachment1_class, s32 attachment1_animation, s32 attachment2_class,
                           s32 attachment2_animation, s32 resource_first, s32 resource_count) __asm__("FUN_002265d8");

s32 queue_preview_animation(s32 animation_id, s32 trigger_mode, s32 delay_frames, s32 item_index,
                           s32 item_animation, s32 attachment0_class, s32 attachment0_animation,
                           s32 attachment1_class, s32 attachment1_animation, s32 attachment2_class,
                           s32 attachment2_animation, s32 resource_first, s32 resource_count)
{
    PreviewAnimationRequest *request;
    s32 request_index;

    request_index = preview_request_count;
    if (request_index >= 8) {
        return -1;
    }
    preview_request_count = request_index + 1;
    request = &preview_animation_requests[request_index];
    request->status = 0;
    request->animation_id = animation_id;
    request->trigger_mode = trigger_mode;
    request->delay_frames = delay_frames;
    request->item_index = item_index;
    request->item_animation = item_animation;
    request->attachment0_class = attachment0_class;
    request->attachment0_animation = attachment0_animation;
    request->attachment1_class = attachment1_class;
    request->attachment1_animation = attachment1_animation;
    request->attachment2_class = attachment2_class;
    request->attachment2_animation = attachment2_animation;
    request->resource_first = resource_first;
    request->resource_count = resource_count;
    return 0;
}

extern __typeof__(queue_preview_animation) func_002265D8 __attribute__((alias("FUN_002265d8")));
