#include "types.h"
#include "sda.h"
#include "rnc/rendering/draw_config.h"
#include "rnc/rendering/draw_environment.h"
#include "rnc/rendering/fs_aa_buffer.h"
#include "rnc/rendering/fs_aa_packets.h"
#include "rnc/rendering/graphics_buffer.h"
#include "rnc/rendering/image_clear_buffer.h"
#include "rnc/rendering/material_templates.h"
#include "rnc/rendering/object_render_class.h"
#include "rnc/rendering/resident_class.h"
#include "rnc/rendering/screen.h"
#include "rnc/rendering/shrub_render_class.h"
#include "rnc/rendering/texture_upload.h"
#include "rnc/rendering/view.h"

u64 first_clear_packet[40] DATA_AT(0013CC90) = {0x2000000000000001, 0xee, 0x30000, 0x47, 0x146, 0, 0x2400000000008010, 0x44, 0x73007000, 0x8d007200, 0x73007200, 0x8d007400, 0x73007400, 0x8d007600, 0x73007600, 0x8d007800, 0x73007800, 0x8d007a00, 0x73007a00, 0x8d007c00, 0x73007c00, 0x8d007e00, 0x73007e00, 0x8d008000, 0x73008000, 0x8d008200, 0x73008200, 0x8d008400, 0x73008400, 0x8d008600, 0x73008600, 0x8d008800, 0x73008800, 0x8d008a00, 0x73008a00, 0x8d008c00, 0x73008c00, 0x8d008e00, 0x73008e00, 0x8d009000};

u64 second_clear_packet[40] DATA_AT(0013CDD0) = {0x2000000000000001, 0xee, 0x30000, 0x47, 0x146, 0, 0x2400000000008010, 0x44, 0x72007000, 0x8e007200, 0x72007200, 0x8e007400, 0x72007400, 0x8e007600, 0x72007600, 0x8e007800, 0x72007800, 0x8e007a00, 0x72007a00, 0x8e007c00, 0x72007c00, 0x8e007e00, 0x72007e00, 0x8e008000, 0x72008000, 0x8e008200, 0x72008200, 0x8e008400, 0x72008400, 0x8e008600, 0x72008600, 0x8e008800, 0x72008800, 0x8e008a00, 0x72008a00, 0x8e008c00, 0x72008c00, 0x8e008e00, 0x72008e00, 0x8e009000};

GraphicsDrawEnvironment draw_environment DATA_AT(0013CF10) = {{0xa000000000008001, 0xeeeeeeeeee}, 0x80070, 0x4c, 0x80070, 0x4d, 0x10000d8, 0x4e, 0x10000d8, 0x4f, 0x730000007000, 0x18, 0x730000007000, 0x19, 0x19f000001ff0000, 0x40, 0x19f000001ff0000};

struct Screen screen_extent DATA_AT(0013E500) = {0};

struct FsAaBuf fs_aa_buffer DATA_AT(00151780) = {0};

u64 fs_aa_draw_packet[76] DATA_AT(00151900) = {0};

u64 fs_aa_resample_packet[74] DATA_AT(00151DF0) = {0};

u64 fs_aa_clear_packet[42] DATA_AT(00152040) = {0};

struct DrawConfig draw_config DATA_AT(0018A2B0) = {0};

struct View view_context DATA_AT(0018CD00) = {0};

struct TextureUpload pending_texture_uploads[64] DATA_AT(0018D040) = {0};

u8 image_clear_buffer[4096] DATA_AT(001941C0) = {0};

u64 resident_material_templates[48] DATA_AT(0019E540) = {0};

u64 special_material_template[3] DATA_AT(0019E6C0) = {0};

u64 alternate_special_material_template[3] DATA_AT(0019E6D8) = {0};

MaterialMap resident_class_material_maps[224] DATA_AT(001B6880) = {0};

struct GraphicsBufferDescriptor graphics_buffer_descriptors[5] DATA_AT(001D60B8) = {0};

ShrubRenderClass *shrub_render_classes[64] DATA_AT(001D7F30) = {0};

u8 shrub_render_class_slot_by_id[1024] DATA_AT(001D80B0) = {0};

s32 shrub_render_class_fixed_thresholds[64] DATA_AT(001D8CB0) = {0};

MaterialMap shrub_render_class_material_maps[64] DATA_AT(001D92B0) = {0};

ObjectRenderClass *object_render_classes[128] DATA_AT(001E1700) = {0};

s32 object_render_class_fixed_thresholds[128] DATA_AT(001E2E00) = {0};

MaterialMap object_render_class_material_maps[128] DATA_AT(001E3600) = {0};
