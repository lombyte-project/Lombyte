#include "types.h"
#include "rnc/ui/menus/menu_system.h"
#include "rnc/weapons/hand_gadget.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/ui/menus/menu_screen.h"
#include "rnc/mobys/moby.h"
#include "rnc/ui/menus/item_preview/clear_preview_animation_queue.h"

extern u8           D_00186310[];
extern int          active_class_resource_index __asm__("D_0015FF4C");
extern void         gadget_moby_update_noop() __asm__("FUN_00224b60");
extern int          noop_callback_e() __asm__("func_001E9410");
extern int          update_menu_preview_animation_pose() __asm__("FUN_00224fc0");
extern int          fill_transfer_words() __asm__("func_001F97E8");
extern struct Moby *create_menu_preview_moby() __asm__("func_00225490");
extern int          initialize_menu_stream_buffers() __asm__("FUN_00225ac0");
extern s32          acquire_menu_stream_buffer() __asm__("FUN_00225c18");

int initialize_menu_preview_objects(struct MenuScreen *preview) __asm__("FUN_002240c8");

int initialize_menu_preview_objects(struct MenuScreen *preview) {
    struct Moby *preview_moby;
    int         *resource_buffer_slot;
    int         *object_variables;
    char        *held_flag;
    int          i, j;

    initialize_menu_stream_buffers(1);
    clear_preview_animation_queue();
    {
        struct MenuSystem *g = &menu_system;

        active_class_resource_index = -1;
        g->preview_requested_class = -1;
        g->preview_loaded_class = -1;
        g->stream_buffer[0] = acquire_menu_stream_buffer(1);
        g->stream_buffer[1] = acquire_menu_stream_buffer(1);
        g->loaded_animation[0] = 0xFF;
        g->loaded_animation[1] = 0xFF;
        g->read_buffer_index = 0;
        resource_buffer_slot = g->preview_resource_buffers;
        for (i = 2; i >= 0; i--) {
            *resource_buffer_slot = acquire_menu_stream_buffer(0);
            resource_buffer_slot++;
        }
    }
    {
        struct MenuSystem *g = &menu_system;

        class_pose_manipulator.active = 0;
        g->current_gadget = -1;
        first_attachment_manipulator.active = 0;
        second_attachment_manipulator.active = 0;
    }
    preview_moby = create_menu_preview_moby(0);
    held_flag = (char *)preview + 0xBB;
    for (j = 23; j >= 0; j--) {
        *held_flag = 0;
        held_flag--;
    }
    if (preview_moby != 0) {
        struct GameCamera *camera = &game_camera;
        struct MenuSystem *g;

        preview->data.preview.moby = preview_moby;
        preview_moby->flags = 0;
        preview_moby->pos.x = camera->position.f[0] + 4.0f;
        preview_moby->pos.y = camera->position.f[1];
        preview_moby->pos.z = camera->position.f[2] - 0.6f;
        preview_moby->rot.z = 3.1415927f;
        preview_moby->update = gadget_moby_update_noop;
        object_variables = (int *)preview_moby->pvars;
        object_variables[0] = (int)preview;
        object_variables[1] = 0;
        object_variables[2] = 0;
        g = &menu_system;
        g->unkC0 = -1;
        fill_transfer_words(D_00186310, 0, 0x40);
        noop_callback_e(preview_moby);
    }
    preview_moby = create_menu_preview_moby(0x259);
    if (preview_moby != 0) {
        *(int *)preview_moby->pvars = (int)preview;
        preview_moby->update = (void (*)(struct Moby *))update_menu_preview_animation_pose;
        preview_moby->flags = 4;
    }
    preview->data.preview.second_moby = preview_moby;
    return 0;
}

extern __typeof__(initialize_menu_preview_objects) func_002240C8
    __attribute__((alias("FUN_002240c8")));
