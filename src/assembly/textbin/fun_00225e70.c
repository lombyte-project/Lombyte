#include "rnc/gameplay/entities/moby_class_tables.h"
#include "sda.h"
#include "types.h"
#include "asm.h"
#include "rnc/ui/menus/item_preview/preview_animation.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00225e70/FUN_00225e70.s", FUN_00225e70);
#else
#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/storage/disc_table.h"
#include "eetypes.h"
#include "qcopy.h"
#include "sda.h"

#include "rnc/ui/menus/item_preview/preview_animation.h"

typedef struct {
    u8 pad0[0x10];
    u128 position;
    u8 pad20[0xC];
    f32 scale;
    u8 pad30[4];
    s16 flags;
    u8 pad36[0xA];
    u128 rotation;
    u8 pad50[0x20];
    u8 transition_flags;
    u8 pad71[3];
    void *update;
    u8 pad78[0x2E];
    s16 oclass;
} Moby;

typedef struct {
    u8 pad0[8];
    s16 active;
} CdReadState;

extern s32 preview_request_count __asm__("D_00160350");
extern s32 preview_request_count_address __asm__("D_00160350") MACRO_ADDR;
extern CdReadState cd_read_state __asm__("D_001516D0");
typedef struct {
    u8 pad0[0x48];
    s32 animation_tables[1];
} PreviewAnimationClassResource;

typedef struct {
    u8 pad0[8];
    s32 item_type;
    u8 pad0C[0x40];
} PreviewItemDefinition;

extern PreviewItemDefinition preview_item_definitions[] __asm__("D_001863D0");
extern u8 attachment_update_callback[] __asm__("FUN_00224b68");

extern void decompress_wad() __asm__("func_0020B618");
extern s32 clear_record_flag_by_key(s32) __asm__("FUN_00225e20");
extern void relocate_asset_entry_pointers() __asm__("FUN_002032e0");
extern s32 get_stream_buffer_size(s32) __asm__("FUN_00225d88");
extern s32 start_audio_stream_read(s32, s32, s32) __asm__("FUN_00216788");
extern void RaiseKernelTrap(void);
extern s32 mark_stream_buffer_read_active(s32) __asm__("FUN_00225dd8");
extern s32 advance_preview_animation_queue(void) __asm__("FUN_00226670");
extern void clear_preview_resource_bindings(void) __asm__("FUN_002267b8");
extern void load_preview_resource_bindings(s32, s32) __asm__("FUN_00226848");
extern void blend_moby_animation_ex(Moby *, s32, s32, s32, s32) __asm__("FUN_002130d8");
extern void blend_moby_animation(Moby *, s32, s32, s32) __asm__("FUN_00212f90");
extern void set_moby_animation(Moby *, s32, s32) __asm__("FUN_00212ed8");
extern Moby *create_menu_preview_moby(s32) __asm__("FUN_00225490");
extern Moby *delete_moby(Moby *) __asm__("FUN_00225530");
extern void refresh_moby_spatial_bounds() __asm__("func_0020DEF8");

s32 update_preview_animation_and_attachments(Moby *source_moby, Moby *primary_item_moby,
                                             Moby *secondary_item_moby, Moby **attachment0,
                                             Moby **attachment1,
                                             Moby **attachment2) __asm__("FUN_00225e70");

s32 update_preview_animation_and_attachments(Moby *source_moby, Moby *primary_item_moby,
                                             Moby *secondary_item_moby, Moby **attachment0,
                                             Moby **attachment1, Moby **attachment2) {
    s32 animation_complete;
    s32 delay_expired;
    s32 activate_request;

    animation_complete = 0;
    delay_expired = 0;
    activate_request = 0;
    if (preview_request_count_address == 0) {
        return 0;
    }
    if (source_moby->transition_flags & 2) {
        animation_complete = 1;
        if (active_preview_animation.delay_frames == 0 ||
            --active_preview_animation.delay_frames == 0) {
            delay_expired = 1;
        }
    }
    if (menu_system.pending_buffer != 0 && cd_read_state.active == 0) {
        s32 buffer_index = menu_system.pending_buffer - 1;
        s32 *buffer_slot = &menu_system.stream_buffer[buffer_index];
        s32 buffer_address = *buffer_slot;

        decompress_wad(buffer_address + menu_system.read_offset, buffer_address);
        menu_system.read_offset = 0;
        clear_record_flag_by_key(buffer_address);
        menu_system.loaded_animation[buffer_index] =
            preview_animation_requests[0].animation_id;
        preview_animation_requests[0].status = 2;
        ((PreviewAnimationClassResource *)moby_class_resources[0])
            ->animation_tables[preview_animation_requests[0].animation_id] = *buffer_slot;
        relocate_asset_entry_pointers(moby_class_resources[0],
                                      preview_animation_requests[0].animation_id);
        menu_system.pending_buffer = 0;
    }
    if (menu_system.pending_buffer == 0 && cd_read_state.active == 0 &&
        preview_request_count > 0) {
        if (preview_animation_requests[0].status == 0) {
            u32 animation_id = preview_animation_requests[0].animation_id;
            struct MenuSystem *stream = &menu_system;
            u8 *loaded_animation = stream->loaded_animation;

            /* Retail uses the Boolean opposite-buffer index here. */
            if (loaded_animation[stream->read_buffer_index] == animation_id ||
                loaded_animation[stream->read_buffer_index == 0] == animation_id) {
                preview_animation_requests[0].status = 2;
            } else if (animation_id < stream->streamed_animation_base) {
                preview_animation_requests[0].status = 3;
            } else {
                u32 archive_index = animation_id - stream->streamed_animation_base;
                s32 buffer_address = stream->stream_buffer[stream->read_buffer_index];
                s32 read_size = disc_table.animation_streams[archive_index].size << 11;
                s32 read_address =
                    buffer_address + get_stream_buffer_size(buffer_address) - read_size;

                stream->read_offset = read_address - buffer_address;
                if (start_audio_stream_read(
                        read_address, disc_table.animation_streams[archive_index].sector,
                        disc_table.animation_streams[archive_index].size) == 0) {
                    RaiseKernelTrap();
                }
                mark_stream_buffer_read_active(buffer_address);
                preview_animation_requests[0].status = 1;
                stream->pending_buffer = stream->read_buffer_index + 1;
                loaded_animation[stream->read_buffer_index] = 0xFF;
            }
        }
    }
    if (preview_request_count > 0 &&
        (preview_animation_requests[0].status == 2 || preview_animation_requests[0].status == 3)) {
        s32 item_index = preview_animation_requests[0].item_index;

        if (item_index != 0 && menu_system.equipped[0] != item_index &&
            menu_system.equipped[2] != item_index &&
            menu_system.equipped[1] != item_index) {
            activate_request = 0;
            advance_preview_animation_queue();
        } else if (preview_animation_requests[0].trigger_mode == 0) {
            activate_request = 1;
        } else if (preview_animation_requests[0].trigger_mode == 1 && animation_complete) {
            activate_request = 1;
        } else if (preview_animation_requests[0].trigger_mode == 2 && delay_expired) {
            activate_request = 1;
        } else if (preview_animation_requests[0].trigger_mode == 3) {
            activate_request = 0;
            advance_preview_animation_queue();
        }
    }
    if (activate_request) {
        active_preview_animation = preview_animation_requests[0];
        advance_preview_animation_queue();
        clear_preview_resource_bindings();
        load_preview_resource_bindings(active_preview_animation.resource_first,
                                       active_preview_animation.resource_count);
        blend_moby_animation_ex(source_moby, active_preview_animation.animation_id, 0, 10, 5);
        if (active_preview_animation.animation_id == menu_system.loaded_animation[0]) {
            menu_system.read_buffer_index = 1;
        } else if (active_preview_animation.animation_id ==
                   menu_system.loaded_animation[1]) {
            menu_system.read_buffer_index = 0;
        }
        if (preview_item_definitions[active_preview_animation.item_index].item_type == 2) {
            if (secondary_item_moby != 0) {
                blend_moby_animation_ex(secondary_item_moby,
                                        active_preview_animation.item_animation, 0, 10, 5);
            }
            if (primary_item_moby != 0) {
                set_moby_animation(primary_item_moby, 1, 0);
            }
        } else if (primary_item_moby != 0) {
            blend_moby_animation_ex(primary_item_moby, active_preview_animation.item_animation, 0,
                                    10, 5);
        }
        *attachment0 = delete_moby(*attachment0);
        *attachment1 = delete_moby(*attachment1);
        *attachment2 = delete_moby(*attachment2);
        if (active_preview_animation.attachment0_class != -1) {
            if ((*attachment0 =
                     create_menu_preview_moby(active_preview_animation.attachment0_class)) != 0) {
                Moby *moby;
                set_moby_animation(*attachment0, active_preview_animation.attachment0_animation, 0);
                blend_moby_animation(*attachment0, active_preview_animation.attachment0_animation,
                                     0, 10);
                (*attachment0)->flags = 0;
                moby = *attachment0;
                qcopy(&moby->position, &source_moby->position);
                qcopy(&moby->rotation, &source_moby->rotation);
                if (moby->oclass == 0x4A) {
                    moby->scale *= 3.0f;
                }
                refresh_moby_spatial_bounds(*attachment0);
                (*attachment0)->update = attachment_update_callback;
            }
        }
        if (active_preview_animation.attachment1_class != -1) {
            if ((*attachment1 =
                     create_menu_preview_moby(active_preview_animation.attachment1_class)) != 0) {
                Moby *moby;
                set_moby_animation(*attachment1, active_preview_animation.attachment1_animation, 0);
                blend_moby_animation(*attachment1, active_preview_animation.attachment1_animation,
                                     0, 10);
                (*attachment1)->flags = 0;
                moby = *attachment1;
                qcopy(&moby->position, &source_moby->position);
                qcopy(&moby->rotation, &source_moby->rotation);
                if (moby->oclass == 0x4A) {
                    moby->scale *= 3.0f;
                }
                refresh_moby_spatial_bounds(*attachment1);
                (*attachment1)->update = attachment_update_callback;
            }
        }
        if (active_preview_animation.attachment2_class != -1) {
            if ((*attachment2 =
                     create_menu_preview_moby(active_preview_animation.attachment2_class)) != 0) {
                Moby *moby;
                set_moby_animation(*attachment2, active_preview_animation.attachment2_animation, 0);
                blend_moby_animation(*attachment2, active_preview_animation.attachment2_animation,
                                     0, 10);
                (*attachment2)->flags = 0;
                moby = *attachment2;
                qcopy(&(*attachment2)->position, &source_moby->position);
                /* This expression preserves the retail reload of the third attachment. */
                qcopy(&(*attachment2)->rotation, &source_moby->rotation);
                if (moby->oclass == 0x4A) {
                    moby->scale *= 3.0f;
                }
                refresh_moby_spatial_bounds(*attachment2);
                (*attachment2)->update = attachment_update_callback;
            }
        }
    }
    return 0;
}
#endif /* NON_MATCHING */

PreviewAnimationRequest active_preview_animation = {0};
