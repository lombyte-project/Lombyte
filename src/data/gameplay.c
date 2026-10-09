#include "types.h"
#include "sda.h"
#include "rnc/gameplay/camera/update_cam.h"
#include "rnc/gameplay/entities/moby_class_tables.h"
#include "rnc/gameplay/gadgets/hand_gadget.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/state/item_state.h"
#include "rnc/gameplay/state/level_state.h"
#include "rnc/gameplay/surface_height_grid.h"

u8 alternate_item_available[128] DATA_AT(0013D388) = {0};

u8 item_unlocked[32] DATA_AT(0013D408) = {0};

s32 weapon_ammo_counts[37] DATA_AT(0013D428) = {0};

u8 item_available[35] DATA_AT(0013D4C0) = {0};

u8 discount_purchase_pricing[5] DATA_AT(0013D4E3) = {0};

u8 level_available[20] DATA_AT(0013DD40) = {0};

u8 level_visit_state[20] DATA_AT(0013DD58) = {0};

u8 item_text_variant[40] DATA_AT(0013E520) = {0};

struct Hero hero DATA_AT(0013F350) = {0};

s32 camera_position_publication_suppressed[1] DATA_AT(0018C32C) = {0};

void * moby_class_resources[224] DATA_AT(001B3200) = {0};

u8 resident_class_slot_by_id[2048] DATA_AT(001B3AC0) = {0};

HandGadgetManipulator class_pose_manipulator DATA_AT(001D5DD0) = {0};

HandGadgetManipulator first_attachment_manipulator DATA_AT(001D5E10) = {0};

HandGadgetManipulator second_attachment_manipulator DATA_AT(001D5E50) = {0};

SurfaceHeightGrid surface_height_grid DATA_AT(001E66E0) = {{0, 0, 0, 0x42, 0, 0, 0, 0x42}, -16.0f, -16.0f, 2.0f, 2.0f};
