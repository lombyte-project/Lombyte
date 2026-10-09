#ifndef LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_PREVIEW_ANIMATION_H
#define LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_PREVIEW_ANIMATION_H

#include "types.h"

typedef struct {
    s32 status;
    s32 animation_id;
    s32 trigger_mode;
    s32 delay_frames;
    s32 item_index;
    s32 item_animation;
    s32 attachment0_class;
    s32 attachment0_animation;
    s32 attachment1_class;
    s32 attachment1_animation;
    s32 attachment2_class;
    s32 attachment2_animation;
    s32 resource_first;
    s32 resource_count;
} PreviewAnimationRequest;

/* The stream state (buffers, loaded animations, resource range) lives in
   struct MenuSystem (rnc/ui/menus/menu_system.h) at 0xA0..0xCC. */

typedef struct {
    s32 class_id;
    s32 animation_index;
} PreviewResourceBinding;

extern f32 ammo_preview_offsets[6] __asm__("D_001D5E90");

extern f32 ammo_preview_velocities[6] __asm__("D_001D5EA8");

extern PreviewAnimationRequest preview_animation_requests[8] __asm__("D_001D5EC0");

extern PreviewAnimationRequest active_preview_animation __asm__("D_001D6080");

#endif /* LOMBYTE_RNC_UI_MENUS_ITEM_PREVIEW_PREVIEW_ANIMATION_H */
