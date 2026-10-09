#ifndef LOMBYTE_RNC_GAMEPLAY_GADGETS_HAND_GADGET_H
#define LOMBYTE_RNC_GAMEPLAY_GADGETS_HAND_GADGET_H

#include "types.h"

/* Bone manipulator the menu preview attaches to a preview moby. */
typedef struct HandGadgetManipulator {
    u8 pad0;
    u8 active;
    u8 pad2[0x1E];
    float rotation_x;
    float rotation_y;
    float rotation_z;
    u8 pad2C[0x14];
} HandGadgetManipulator; /* size 0x40 */

/* Preview animation and resource setup of one gadget (0x25 entries,
   indexed by gadget id). */
typedef struct HandGadgetAnimation {
    s32 resource_first;
    s32 resource_count;
    s32 primary_animation;
    s32 delay_frames;
    s32 item_animation;
    s32 secondary_animation;
    s32 attachment0_class;
    s32 attachment0_animation;
    s32 attachment1_class;
    s32 attachment1_animation;
    s32 attachment2_class;
    s32 attachment2_animation;
} HandGadgetAnimation; /* size 0x30 */

extern HandGadgetAnimation gadget_animations[0x25] __asm__("D_001D52E8");
extern HandGadgetManipulator class_pose_manipulator __asm__("D_001D5DD0");
extern HandGadgetManipulator first_attachment_manipulator __asm__("D_001D5E10");
extern HandGadgetManipulator second_attachment_manipulator __asm__("D_001D5E50");

#endif /* LOMBYTE_RNC_GAMEPLAY_GADGETS_HAND_GADGET_H */
