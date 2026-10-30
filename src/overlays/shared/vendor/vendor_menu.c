/* Vendor menu: loads the vendor moby assets, builds the item rows, opens, updates and closes the menu, handles purchases and plays the exit cutscene. */
#include "types.h"
#include "sda.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/vendor_menu.h"
#include "rnc/ui/vendor/vendor_item_info.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/globals.h"
#include "rnc/input/pad_state.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/rendering/effects/fade_to_black.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/mobys/attach_manipulator.h"
#include "rnc/overlay/shared/camera/scripted_camera.h"
#include "rnc/overlay/shared/game/unlock_planet.h"
#include "rnc/overlay/shared/camera/follow_camera_set_up_spring.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "rnc/overlay/shared/ui/hud/draw_hud_item_selector_panel.h"
#include "rnc/rendering/view/update_view_context.h"
#include "rnc/audio/streaming/continue_audio_stream_if_ready.h"
#include "rnc/audio/voices/snd_continue_all_sounds_in_group.h"
#include "rnc/audio/voices/snd_pause_all_sounds_in_group.h"
#include "rnc/mobys/animation/update_moby_animation_state.h"
#include "rnc/mobys/detach_manipulator.h"
#include "rnc/mobys/init_moby_instance.h"
#include "rnc/mobys/update_visible_resident_objects.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/rendering/buffers/vu1_send_chain_render_buffer.h"
#include "rnc/world/loaders/relocate_asset_entry_pointers.h"
#include "rnc/world/select_world_object_resource_tables.h"
#include "rnc/overlay/shared/vendor/vendor_menu_globals.h"
#include "rnc/game/item_state.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "qcopy.h"
#include "rnc/overlay/level_camera.h"

/* Clears D_L00_0015F424..0015F434, D_L00_0017E504[0] and D_L00_0015F2F0/2F4. */
void reset_frame_counters(void) __asm__("FUN_L00_00297e30");

void update_game_frame_and_idle_counters(void) __asm__("FUN_L00_00297e70");

extern s32 D_L00_001B9DE0 NOT_SDA;
extern s32 D_0013D384     NOT_SDA;
extern s32 D_L00_00184214 NOT_SDA;
extern s32
queue_animation_update(s32, s32, void *, void *, void *, void *, s32) __asm__("FUN_001ff308");
extern void noop_main_callback(void *, s32) __asm__("FUN_001e93e8");
extern void coordinate_hero_frame_update(void) __asm__("FUN_L00_002070d0");
extern void update_parts(void) __asm__("FUN_00217b88");
extern void sound_update(void) __asm__("FUN_0022ca50");
extern void update_hud_elements(s32) __asm__("FUN_L00_0023ac78");

typedef struct {
    char           pad[0x20];
    unsigned char  st;
    char           pad21[0x13];
    unsigned short flags;
    char           pad36[0x70];
    short          type;
    char           pada8[0x58];
} O_298840;

typedef struct {
    char           pad[0x34];
    unsigned short flags;
} F_298840;

typedef struct {
    char  pad[0x30];
    int   x30;
    int   x34;
    int   x38;
    int   x3c;
    char  pad40[8];
    short x48;
    char  pad4a[0xE];
    int   x58;
    int   x5c;
} C_298840;

typedef struct {
    char          pad[0x20A5];
    unsigned char x20a5;
    char          pad20a6[0x202];
    int           x22a8;
} P_298840;

extern C_298840 D_L00_0016C860_lui __asm__("D_L00_0016C860") NOT_SDA;
extern int      D_L00_0015F5D8 __asm__("D_L00_0015F5D8");
extern char     D_L00_0017C340_lui[] __asm__("D_L00_0017C340") NOT_SDA;

extern s32 D_L00_00173E00[];

extern int D_L00_0015F400 __asm__("D_L00_0015F400");
void       fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");
void       clear_active_help_record(void) __asm__("FUN_L00_002039a0");

extern float D_L00_0016CAF0_lui __asm__("D_L00_0016CAF0") NOT_SDA;
void         calculate_dma_transfer_address(void) __asm__("FUN_002335a0");
void         set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

extern void sce_vu0_unit_matrix(void *) __asm__("FUN_00125218");
extern void sce_vu0_rot_matrix_x(void *, void *, float) __asm__("FUN_00125360");
extern void sce_vu0_rot_matrix_y(void *, void *, float) __asm__("FUN_00125408");
extern void sceVu0RotMatrixZ(void *, void *, float) __asm__("FUN_001252b8");

/* Moby-sized (0x100) records used by the vendor menu. */
typedef struct {
    u8  pad0[0xC];
    u8  count; /* 0x0C */
    u8  pad0D[0x3B];
    s32 slots[1]; /* 0x48 */
} VendorMobyClass;

typedef struct VendorMoby {
    u8               pad0[0x10];
    Vec4             pos; /* 0x10 */
    u8               pad20[2];
    u8               class_id; /* 0x22 */
    u8               pad23;
    VendorMobyClass *pclass; /* 0x24 */
    u8               pad28[0xA];
    u16              h32; /* 0x32 */
    u8               pad34[0xC];
    Vec4             rot;        /* 0x40 */
    u8               frame;      /* 0x50 */
    u8               next_frame; /* 0x51 */
    u8               pad52;
    u8               b53;   /* 0x53 */
    f32              blend; /* 0x54 */
    f32              f58;   /* 0x58 */
    u8               pad5C[0x14];
    u8               b70; /* 0x70 */
    u8               b71; /* 0x71 */
    u8               pad72[6];
    Vec4            *keys; /* 0x78 */
    u8               pad7C[3];
    u8               b7F; /* 0x7F */
    u8               pad80[0x26];
    s16              oclass; /* 0xA6 */
    u8               padA8[0x18];
    Vec4             mtx[4]; /* 0xC0 */
} VendorMoby;

typedef struct {
    f32 pos[3];
    u8  flag;
    u8  padD[3];
    f32 rot[4];
} CutsceneCameraKey;

typedef struct {
    char               pad0[0x34];
    s32                f34;
    s32                f38;
    s32                f3C;
    s16                h40;
    s16                pad42;
    s16                h44;
    s16                pad46;
    s16                h48; /* 0x48: frame at which continue_audio_stream_if_ready runs */
    char               pad4A[0xA];
    CutsceneCameraKey *f54;
    char               pad58[0x120];
    VendorMoby        *f178[1];
} VendorCutscene;

extern VendorCutscene D_L00_0016C860;
extern void           update_moby_shadow_range(void *) __asm__("FUN_00213700");
extern void           apply_hero_moby_scale(void *) __asm__("FUN_L00_00234b38");

extern s32 D_0015ED80;

extern int  D_L00_00161048;
extern int  D_L00_0016104C;
extern int  D_L00_00161050;
extern void FlushCache(s32);
/* starts the slideshow: resets state, rebases the level data pointers, and starts the first two streams */
/* Ported from rac1-decomp (src/overlays/shared/tieproc_00299108.c: func_L00_0029ADD8), where it is exact; names translated to the US level program. */

typedef struct {
    int a;
    int b;
} LL9;

typedef struct {
    char pad[0x16A8];
    LL9  normal[20];
    LL9  alt[20];
} G9;

extern int D_L00_0015F5C4 MACRO_ADDR;

extern int D_L00_001611CC MACRO_ADDR;

extern s32 D_L00_0015F3FC;

extern void FUN_0022ca50(void);

/* Ported from rac1-decomp (src/overlays/shared/update_0029B6A0.c: func_L00_0029B6A0), where it is exact; names translated to the US level program. */

extern char      *D_L00_00197300_lui[] __asm__("D_L00_00197300") NOT_SDA;
extern VendorMenu vendor_menu_lui __asm__("D_L00_001CA4C0") NOT_SDA;
extern void       fast_decompress(int, int) __asm__("FUN_L00_0024d8d0");

extern unsigned char D_L00_00197BCC NOT_SDA;
void decompress_and_relocate_vendor_moby_assets(void) __asm__("FUN_L00_0029a330");

void decompress_and_relocate_vendor_moby_assets(void) {
    int   i;
    int  *entry_offset_cursor;
    char *base;
    int   compressed_source_address;
    compressed_source_address = vendor_menu_lui.unk18 + 0x80000;
    FlushCache(0);
    fast_decompress(compressed_source_address, vendor_menu_lui.unk18);
    FlushCache(0);
    entry_offset_cursor = (int *)vendor_menu_lui.unk18;
    base = D_L00_00197300_lui[D_L00_00197BCC];
    for (i = 0; i < 16; i++) {
        int entry_offset = *entry_offset_cursor++;
        if (entry_offset) {
            *(int *)(base + 0x48 + i * 4) = vendor_menu_lui.unk18 + entry_offset;
            relocate_asset_entry_pointers(base, i);
        }
    }
}

#include "rnc/overlay/shared/vendor/vendor_menu.h"

extern VendorSound vendor_sound_lui[] __asm__("D_L00_001C2108") NOT_SDA;
/* Stocked item ids (low 6 bits; bit 0x40 = listed as selected), 0xFF-terminated. */

extern u8 D_0015EDD0_macro[] __asm__("D_0015EDD0") MACRO_ADDR;
/* Builds the vendor list: the stocked items (up to 12, 0xFF-terminated), then
   every other available item (item_available set, h8 non-zero) not already listed. */
void build_vendor_menu_rows(s32 skip_unflagged) __asm__("FUN_L00_0029a400");

void build_vendor_menu_rows(s32 skip_unflagged) {
    s32 count, i, j, value, item, k;

    count = 0;
    if (D_0015EDD0_macro[0] != 0xFF) {
        do
            count++;
        while (count < 12 && D_0015EDD0_macro[count] != 0xFF);
    }
    vendor_menu_lui.row_count = 0;
    for (i = 0; i < count; i++) {
        value = D_0015EDD0_macro[i];
        if (value == 0xFF)
            continue;
        item = value & 0x3F;
        if (value & 0x40) {
            s32 sound;
            if (D_L00_001C40B0[item].h8 == 0)
                continue;
            sound = vendor_sound_lui[item].sound;
            vendor_menu_lui.rows[vendor_menu_lui.row_count].item = item;
            vendor_menu_lui.rows[vendor_menu_lui.row_count].is_ammo = 1;
            vendor_menu_lui.rows[vendor_menu_lui.row_count].state = 0;
            vendor_menu_lui.rows[vendor_menu_lui.row_count].sound = sound;
            vendor_menu_lui.row_count++;
        } else {
            s32 sound;
            if (skip_unflagged)
                continue;
            sound = vendor_sound_lui[item].sound;
            vendor_menu_lui.rows[vendor_menu_lui.row_count].item = item;
            vendor_menu_lui.rows[vendor_menu_lui.row_count].is_ammo = 0;
            vendor_menu_lui.rows[vendor_menu_lui.row_count].state = 0;
            vendor_menu_lui.rows[vendor_menu_lui.row_count].sound = sound;
            vendor_menu_lui.row_count++;
        }
    }
    for (k = 0; k < 37; k++) {
        s32 sound;
        if (!item_available[k])
            continue;
        if (!D_L00_001C40B0[k].h8)
            continue;
        for (j = 0; j < vendor_menu_lui.row_count; j++) {
            if (vendor_menu_lui.rows[j].item == k)
                break;
        }
        if (j < vendor_menu_lui.row_count)
            continue;
        sound = vendor_sound_lui[k].sound;
        vendor_menu_lui.rows[vendor_menu_lui.row_count].item = k;
        vendor_menu_lui.rows[vendor_menu_lui.row_count].is_ammo = 1;
        vendor_menu_lui.rows[vendor_menu_lui.row_count].state = 0;
        vendor_menu_lui.rows[vendor_menu_lui.row_count].sound = sound;
        vendor_menu_lui.row_count++;
    }
}

#include "qzero.h"

typedef struct {
    u8  pad[0xE];
    u16 text;
    u8  tail[0x18 - 0x10];
} VendorItem_29a6b0;

typedef struct {
    u8   pad0[0x10];
    Vec4 pos;
    u8   state;
    u8   pad21[0x11];
    s16  h32;
    u8   pad34[0xC];
    Vec4 rot;
    u8   pad50[8];
    f32  f58;
} VendorMoby_29a6b0;

extern VendorMenu vendor_menu __asm__("D_L00_001CA4C0");
extern VendorSlot vendor_slots[4] __asm__("D_L00_00165E80");

extern s32           D_L00_00161E18;
extern s32           D_L00_00161E20;
extern s32           D_L00_00161E24_29a6b0 __asm__("D_L00_00161E24");
extern s32           D_L00_00161E30;
extern s32           D_L00_00161E34;
extern unsigned char D_0013F350[];
extern Vec4          D_L00_00180260_29a6b0 __asm__("D_L00_00180260");
extern s32           D_L00_00161188;
extern s32           D_L00_0016118C;
extern void          FUN_00216050_29a6b0(s32) __asm__("FUN_00216050");
extern void          snd_flush_sound_commands(void) __asm__("FUN_0012dc80");
extern void          reset_hud_and_suspend_weapon_updates(void) __asm__("FUN_L00_0023aef8");
extern void initialize_numeric_hud_timer_and_base_dimensions(void) __asm__("FUN_L00_0023a760");
extern void draw_weapon_ammo_hud_panel(void) __asm__("FUN_L00_0023a7a8");
extern void FUN_L00_00232fe8_29a6b0(void) __asm__("FUN_L00_00232fe8");

typedef unsigned int Q_29a6b0 __attribute__((mode(TI), aligned(16)));
extern u8            vendor_extra[] __asm__("D_L00_001CA6E0");
extern f32  vendor_camera_eye_local_offset_x_gp __asm__("D_L00_00161060") __attribute__((sda));
extern Vec4 D_L00_00180250;

#include "rnc/overlay/shared/vendor/vendor_menu_data/open_vendor_menu.h"

extern f32 D_L00_001610E0 __attribute__((sda));
extern s32 game_language_macro __asm__("D_0015ED88") MACRO_ADDR;
extern f32 frame_scale_macro __asm__("D_0015ED60") MACRO_ADDR;

/* Opens the vendor: clears its state, spawns the vendor moby when none is given, builds the item list and its texts, then sets up the camera and the four slot offsets. */
void open_vendor_menu(void *) __asm__("FUN_L00_0029a6b0");

void open_vendor_menu(void *vendor_moby_arg) {
    VendorMoby_29a6b0 *vendor_moby = vendor_moby_arg;
    s32                i;
    Vec4              *vector_destination_cursor;
    s32                half;
    s32                sel;
    s32                k;
    VendorSlot        *slot;

    fill_transfer_words(&vendor_menu, 0, 0x220);
    fill_transfer_words(vendor_extra, 0, 0x40);
    if (vendor_moby == 0) {
        vendor_moby = create_moby(0xB);
        *(s16 *)((char *)vendor_moby + 0x32) = 0x40;
        *(f32 *)((char *)vendor_moby + 0x10) =
            l00_camera.pos.f[0] + vendor_camera_eye_local_offset_x_gp;
        *(f32 *)((char *)vendor_moby + 0x14) = l00_camera.pos.f[1];
        *(f32 *)((char *)vendor_moby + 0x18) =
            l00_camera.pos.f[2] - vendor_camera_eye_local_offset_z;
        clear_u64_value(&vendor_moby->rot);
        vendor_moby->rot.f[2] = 3.1415927f;
        update_entity(vendor_moby);
        vendor_moby->state = 3;
        vendor_menu.mode = 1;
    }
    if ((vendor_menu.timer = game_language_macro - 1) < 0) {
        vendor_menu.timer = 0;
    }
    vendor_menu.unk44 = 0;
    vendor_menu.purchased_item = vendor_menu.title_text = vendor_menu.item_text = -1;
    vendor_menu.cursor = 0;
    D_L00_00161E18 = 0;
    D_L00_00161E20 = 0;
    D_L00_00161E30 = 0;
    vendor_menu.purchase_state = 0;
    D_L00_00161E34 = 0;
    D_L00_00161E24_29a6b0 = 0;
    snd_pause_all_sounds_in_group(0x1D);
    FUN_00216050_29a6b0(0);
    snd_flush_sound_commands();
    build_vendor_menu_rows(vendor_menu.mode != 0);
    half = vendor_menu.row_count;
    sel = half / 2;
    if (half < 3)
        sel = 1;
    vendor_menu.cursor = half / 2;
    vendor_menu.scroll = (sel - half / 2) * 0x28;
    reset_hud_and_suspend_weapon_updates();
    vendor_menu.title_text =
        queue_animation_update(0x12,
                               0x754E,
                               initialize_bolt_counter_hud,
                               update_bolt_counter_hud_animation_and_visibility,
                               draw_bolt_counter_hud_panel,
                               &current_bolt_count,
                               0x98967F);
    if (vendor_menu.rows[vendor_menu.cursor].is_ammo == 1) {
        s32 item = vendor_menu.rows[vendor_menu.cursor].item;

        vendor_menu.item_text =
            queue_animation_update(0x30,
                                   item + 0xEA60,
                                   initialize_numeric_hud_timer_and_base_dimensions,
                                   hud_update_counter,
                                   draw_weapon_ammo_hud_panel,
                                   &weapon_ammo_counts[item],
                                   (D_L00_001C40B0 + item)->hE);
    }
    D_L00_0015F5D8 = 1;
    vendor_menu.state = 0;
    vendor_menu.unk10 = D_L00_00173E00[2] + 0x60000;
    vendor_menu.unk14 = (struct VendorMoby *)(D_L00_00173E00[1] + 0x60000);
    vendor_menu.unk08 = 0;
    vendor_menu.vendor_moby = (struct VendorMoby *)vendor_moby;
    allocate_voice_for_target_entry(3, 0, vendor_moby);
    if (vendor_menu.mode == 0) {
        fade_to_black(4);
    }
    clear_active_help_record();
    fade_level = 1.0f;
    D_L00_0015F5C4 = 5;
    D_L00_0015F400 = 0;
    hero_set_state(100, 1);
    D_0013F350[0x20AC] = 1;
    D_0013F350[0x20A5] = 1;
    FUN_L00_00232fe8_29a6b0();
    if (vendor_menu.mode != 0) {
        set_moby_animation(vendor_moby, 3, 0);
    } else {
        set_moby_animation(vendor_moby, 2, 0);
    }
    vendor_moby->f58 = frame_scale_macro * 0.5f;
    if (vendor_menu.mode == 0) {
        transform_vector_by_basis(&vendor_menu.eye,
                                  &vendor_camera_eye_local_offset_x_gp,
                                  ((char *)vendor_menu.vendor_moby) + 0xC0);
        fast_vec_add(&vendor_menu.eye, &vendor_menu.eye, ((char *)vendor_menu.vendor_moby) + 0x10);
        vendor_menu.look.f[2] =
            fast_add_rotations(MOBY(((char *)vendor_menu.vendor_moby))->rot.z, 3.1415927f);
        jet_mount(&vendor_menu.eye, &vendor_menu.look, 1, 0, 0);
        set_camera_position(&vendor_menu.eye);
        set_camera_rotation(&vendor_menu.look);
    }
    vector_destination_cursor = &D_L00_00180250;
    transform_vector_by_basis(vector_destination_cursor,
                              &vendor_basis_local_vector_x_gp,
                              ((char *)vendor_menu.vendor_moby) + 0xC0);
    qcopy(vector_destination_cursor - 1, &D_L00_001610E0);
    vector_destination_cursor += 2;
    qzero(vector_destination_cursor);
    qzero(&D_L00_00180260_29a6b0);
    for (i = 0; i < 4; i++) {
        slot = &vendor_slots[i];
        if (slot->hidden == 0) {
            attach_manipulator(((char *)vendor_menu.vendor_moby), i + 0x14, slot);
        }
    }
    if (vendor_menu.row_count < 8) {
        k = 7 - vendor_menu.row_count;
        vendor_slots[0].offset = k * 0x681;
        vendor_slots[1].offset = k * -0x681;
        vendor_slots[2].offset = k * 0x681;
        vendor_slots[3].offset = k * -0x681;
    } else {
        vendor_slots[0].offset = 0;
        vendor_slots[1].offset = 0;
        vendor_slots[2].offset = 0;
        vendor_slots[3].offset = 0;
    }
    D_L00_00161188 = 0;
    D_L00_0016118C = 0;
}

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Vec4_29ab70;

extern void f2330d0_29ab70(void) __asm__("FUN_L00_002330d0");
extern void update_resource_counter(void) __asm__("FUN_001ff768");
extern void music_unpause(void) __asm__("FUN_00216088");
extern int  vendor_close_hero_local_offset_gp __asm__("D_L00_00161070") __attribute__((sda));
void        close_vendor_menu(int exit_cutscene_completed) __asm__("FUN_L00_0029ab70");

void close_vendor_menu(int exit_cutscene_completed) {
    Vec4_29ab70 world_position;
    Vec4_29ab70 euler_rotation;
    char       *vendor_state_base = (char *)&vendor_menu;
    if (*(int *)(vendor_state_base + 0x54) != -1)
        set_animation_parameter(*(int *)(vendor_state_base + 0x54), 0);
    if (*(int *)(vendor_state_base + 0x50) != -1)
        set_animation_parameter(*(int *)(vendor_state_base + 0x50), 0);
    *(int *)(vendor_state_base + 0x54) = -1;
    *(int *)(vendor_state_base + 0x50) = -1;
    update_resource_counter();
    D_L00_0015F5D8 = 1;
    D_L00_0015F5C4 = 0;
    hero_set_state(HERO_STATE_IDLE, 1);
    hero.unk20A5 = 0;
    f2330d0_29ab70();
    if (*(int *)(vendor_state_base + 0x40) == 0) {
        transform_vector_by_basis(&world_position,
                                  &vendor_close_hero_local_offset_gp,
                                  *(char **)(vendor_state_base + 0x1C) + 0xC0);
        fast_vec_add(&world_position, &world_position, *(char **)(vendor_state_base + 0x1C) + 0x10);
        euler_rotation.x = 0.0f;
        euler_rotation.y = 0.0f;
        euler_rotation.z =
            fast_add_rotations(*(float *)(*(char **)(vendor_state_base + 0x1C) + 0x48), 3.1415927f);
        if (exit_cutscene_completed == 0) {
            set_race_camera(&world_position, &euler_rotation, 0, 0);
            jet_end_mount(2);
        } else {
            set_race_camera(&world_position, &euler_rotation, 0, 1);
        }
    }
    {
        char *vendor_state = (char *)&vendor_menu;
        *(*(char **)(vendor_state + 0x1C) + 0x20) = 1;
        if ((unsigned short)(music_stream_state.secondary.state - 6) >= 2)
            music_stream_state.secondary.state = 5;
        if (*(int *)(vendor_state + 0x40))
            mark_moby_for_removal(*(void **)(vendor_state + 0x1C));
    }
    snd_continue_all_sounds_in_group(0x1D);
    music_unpause();
    snd_flush_sound_commands();
}

extern s32 vendor_exit_cutscene_ids_by_item[] __asm__("D_L00_001CA020");
extern s32 vendor_exit_hero_local_offset_gp __asm__("D_L00_00161100") __attribute__((sda));
extern s32 vendor_exit_cutscene_local_offset_gp __asm__("D_L00_001610F0") __attribute__((sda));

#include "rnc/overlay/item_defs.h"

extern float probe_ground_height(float, void *, s32) __asm__("FUN_00213508");
extern void  clear_hero_item_slot(s32, s32) __asm__("FUN_L00_0020fca8");
extern void  load_scene_chunks_with_fade(s32) __asm__("FUN_L00_002454c8");
extern void  FUN_L00_002455c0(s32);
extern void  sce_gs_sync_v(s32) __asm__("FUN_00122298");

void start_vendor_exit_cutscene(void) __asm__("FUN_L00_0029ad08");

void start_vendor_exit_cutscene(void) {
    VendorMenu              *vendor_state;
    VendorMoby_29a6b0       *moby;
    s32                      index, item, offset;
    f32                     *cutscene_world_origin;
    struct MusicStreamState *state;
    s32                     *selected;
    s16                      mode;

    jet_end_mount(2);
    if ((u16)(music_stream_state.secondary.state - 6) >= 2)
        music_stream_state.secondary.state = 5;
    vendor_state = &vendor_menu;
    D_L00_0015F5D8 = 1;
    ((VendorMoby_29a6b0 *)vendor_state->vendor_moby)->state = 1;
    vendor_state->state = 3;
    vendor_state->frames = 0;
    cutscene_world_origin = (f32 *)((char *)vendor_state + 0xC0);
    transform_vector_by_basis(cutscene_world_origin,
                              &vendor_exit_cutscene_local_offset_gp,
                              (char *)vendor_state->vendor_moby + 0xC0);
    moby = (VendorMoby_29a6b0 *)vendor_state->vendor_moby;
    fast_vec_add(cutscene_world_origin, cutscene_world_origin, (char *)moby + 0x10);
    *(f32 *)((char *)vendor_state + 0xC8) += 2.0f;
    *(f32 *)((char *)vendor_state + 0xC8) = probe_ground_height(0.5f, cutscene_world_origin, 0);
    clear_u64_value((char *)vendor_state + 0xB0);
    *(f32 *)((char *)vendor_state + 0xB8) =
        fast_add_rotations(((VendorMoby_29a6b0 *)vendor_state->vendor_moby)->rot.f[2], 3.1415927f);
    euler_rotation_basis((char *)vendor_state + 0x80, (char *)vendor_state + 0xB0);
    fill_transfer_words(&D_L00_0016C860_lui, 0, 0x1C0);
    fill_transfer_words(D_L00_0017C340_lui, 0, 0x40);
    vu1_sync_chain(1);
    calculate_dma_transfer_address();
    selected = (s32 *)((char *)vendor_state + 0x48);
    offset = D_L00_001611CC - 0x40000;
    D_L00_001611CC = offset;
    index = vendor_menu.purchased_item;
    D_L00_0016C860_lui.x58 = D_L00_00173E00[1] + offset;
    D_L00_0016C860_lui.x5c = D_L00_00173E00[2] + offset;
    item = vendor_exit_cutscene_ids_by_item[index];
    D_L00_0015F3FC = 0;
    D_L00_0015F400 = 0;
    if (hero.items[0].item_id != 0 && index != hero.items[0].item_id)
        clear_hero_item_slot(0, 0);
    hero.pending_item[0] = vendor_menu.purchased_item;
    if (hero.pending_item[0] == 0x18)
        hero.pending_item[0] = 0;
    select_world_object_resource_tables(item_defs[vendor_menu.purchased_item].cls, -1);
    load_scene_chunks_with_fade(item);
    state = &music_stream_state;
    state->queued_secondary_track = 0x2734;
    FUN_L00_002455c0(0);
    mode = music_stream_state.secondary.state;
    while (mode != 3) {
        sound_update();
        sce_gs_sync_v(0);
        mode = music_stream_state.secondary.state;
    }
    continue_audio_stream_if_ready();
}

extern int D_L00_0015F5D8;
void       FUN_L00_002330d0(void);
void       restore_hero_after_vendor_exit_cutscene(void) __asm__("FUN_L00_0029af80");

void restore_hero_after_vendor_exit_cutscene(void) {
    Vec4  world_position;
    Vec4  euler_rotation;
    char *vendor_state_base;
    D_L00_0015F5D8 = 1;
    D_L00_0015F5C4 = 0;
    hero_set_state(HERO_STATE_IDLE, 1);
    hero.unk20A5 = 0;
    FUN_L00_002330d0();
    vendor_state_base = (char *)&vendor_menu;
    transform_vector_by_basis(&world_position,
                              &vendor_exit_hero_local_offset_gp,
                              *(char **)(vendor_state_base + 0x28) + 0xC0);
    fast_vec_add(&world_position, &world_position, *(char **)(vendor_state_base + 0x28) + 0x10);
    euler_rotation.f[0] = 0.0f;
    euler_rotation.f[1] = 0.0f;
    euler_rotation.f[2] =
        fast_add_rotations(*(float *)(*(char **)(vendor_state_base + 0x28) + 0x48), -1.5707964f);
    set_race_camera(&world_position, &euler_rotation, 0, 1);
    update_resource_counter();
    if ((unsigned short)(music_stream_state.secondary.state - 6) >= 2)
        music_stream_state.secondary.state = 5;
    snd_continue_all_sounds_in_group(0x1D);
    music_unpause();
    snd_flush_sound_commands();
}

/* steps the pickup-spawn state machine for the given mode */
/* Ported from rac1-decomp (src/overlays/shared/update_0029B6A0.c: func_L00_0029C840), where it is exact; names translated to the US level program. */

extern int           D_L00_0015F3F8;
extern int           D_L00_00161188;
extern int           D_L00_00161194;
extern unsigned char queued_dialogue_id[] __asm__("D_001516EC");

void handle_vendor_auxiliary_moby_event(int vendor_event) __asm__("FUN_L00_0029b4d8");

void handle_vendor_auxiliary_moby_event(int vendor_event) {
    enum {
        VENDOR_SELECTION_CHANGED = 0,
        VENDOR_AUXILIARY_MOBY_READY = 1,
        VENDOR_ITEM_PURCHASE = 2,
        VENDOR_AMMO_PURCHASE = 3
    };
    char        *vendor_state_base;
    struct Moby *auxiliary_moby;
    int          auxiliary_moby_state;
    int          moby_state;
    int          selection_variant_index;
    int          ready_variant_index;
    if (D_L00_00161188 == 0)
        return;
    vendor_state_base = (char *)&vendor_menu;
    auxiliary_moby = *(unsigned char **)(vendor_state_base + 0x24);
    switch (vendor_event) {
    case VENDOR_SELECTION_CHANGED:
        auxiliary_moby_state = auxiliary_moby->state;
        if (auxiliary_moby_state == 2 || auxiliary_moby_state == 0) {
            if (random_integer_below(4) == 0) {
                if (*(int *)(vendor_state_base + 0x30) + scale_game_frames(0x168) <
                    D_L00_0015F3F8) {
                    *(int *)(vendor_state_base + 0x30) = D_L00_0015F3F8;
                    selection_variant_index = random_integer_below(2);
                    *(int *)(vendor_state_base + 0x3C) = selection_variant_index;
                    vendor_auxiliary_reaction_started = 1;
                    D_L00_00161194 = 0;
                    *(int *)(queued_dialogue_id) = (selection_variant_index * 3 + 2) * 6 +
                                                   *(int *)(vendor_state_base + 0x38) + 10000;
                    auxiliary_moby->state = 0xB;
                }
            }
        }
        break;
    case VENDOR_AUXILIARY_MOBY_READY:
        ready_variant_index = random_integer_below(2);
        *(int *)(vendor_state_base + 0x3C) = ready_variant_index;
        vendor_auxiliary_reaction_started = vendor_event;
        D_L00_00161194 = 0;
        *(int *)(queued_dialogue_id) =
            ready_variant_index * 0x12 + *(int *)(vendor_state_base + 0x38) + 10000;
        if (auxiliary_moby->prev_seq != vendor_event)
            blend_moby_animation(auxiliary_moby, 1, 0, 10);
        auxiliary_moby->state = 10;
        break;
    case VENDOR_ITEM_PURCHASE:
    case VENDOR_AMMO_PURCHASE:
        moby_state = auxiliary_moby->state;
        if (moby_state == 2 || moby_state == 0) {
            char *vendor_state = (char *)&vendor_menu;
            *(int *)(vendor_state + 0x34) = 0;
        }
        break;
    }
}

typedef float  V_29b680[4] __attribute__((aligned(16)));
extern ItemDef item_defs_lui[] __asm__("D_L00_00179AC0") NOT_SDA;
extern int     D_L00_001C8FF0_lui[][16] __asm__("D_L00_001C8FF0") NOT_SDA;
extern int     D_L00_00161E38;
extern void    FUN_0020d4f0_29b680(void *, int, int, int, int) __asm__("FUN_0020d4f0");

extern float D_L00_001C8920[][12] NOT_SDA;
extern float D_L00_001C8900[][12] NOT_SDA;
extern int   vendor_selected_item_local_offset_gp __asm__("D_L00_00161090") __attribute__((sda));
void         initialize_vendor_selected_item_mobys(void) __asm__("FUN_L00_0029b680");

void initialize_vendor_selected_item_mobys(void) {
    V_29b680 item_local_position;
    int      id;
    int      k;
    int      x;

    k = vendor_menu_lui.rows[vendor_menu_lui.cursor].item;
    id = item_defs_lui[k].cls;
    if (k == 0x18) {
        id = 0x1DF;
    } else {
        select_world_object_resource_tables(id, -1);
    }
    init_moby_instance(((u8 *)vendor_menu_lui.menu_mobys) + 0x100, id);
    *(short *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x132) = 0x40;
    *(int *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x194) = 0;
    fast_vec_add(item_local_position,
                 &vendor_selected_item_local_offset_gp,
                 D_L00_001C8920[vendor_menu_lui.rows[vendor_menu_lui.cursor].item]);
    if (vendor_menu_lui.rows[vendor_menu_lui.cursor].is_ammo == 0) {
        float item_z_offset = D_L00_001C8920[vendor_menu_lui.rows[vendor_menu_lui.cursor].item][3];
        item_local_position[2] += item_z_offset;
        if (discount_purchase_pricing[0]) {
            item_local_position[2] += item_z_offset;
        }
    }
    transform_vector_by_basis(((u8 *)vendor_menu_lui.menu_mobys) + 0x110,
                              item_local_position,
                              ((u8 *)vendor_menu_lui.vendor_moby) + 0xC0);
    fast_vec_add(((u8 *)vendor_menu_lui.menu_mobys) + 0x110,
                 ((u8 *)vendor_menu_lui.menu_mobys) + 0x110,
                 ((u8 *)vendor_menu_lui.vendor_moby) + 0x10);
    {
        float(*t)[12] = D_L00_001C8900;
        *(float *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x140) =
            t[vendor_menu_lui.rows[vendor_menu_lui.cursor].item][4];
    }
    *(float *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x144) =
        D_L00_001C8900[vendor_menu_lui.rows[vendor_menu_lui.cursor].item][5];
    *(float *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x148) =
        D_L00_001C8900[vendor_menu_lui.rows[vendor_menu_lui.cursor].item][6] +
        *(float *)(((u8 *)vendor_menu_lui.vendor_moby) + 0x48);
    update_entity(((u8 *)vendor_menu_lui.menu_mobys) + 0x100);
    set_moby_animation(((u8 *)vendor_menu_lui.menu_mobys) + 0x100, 0, 0);
    if ((*(unsigned char **)(((u8 *)vendor_menu_lui.menu_mobys) + 0x124))[0xC] >= 2) {
        set_moby_animation(((u8 *)vendor_menu_lui.menu_mobys) + 0x100, 1, 0);
    }
    *(int *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x158) = 0;
    ((u8 *)vendor_menu_lui.menu_mobys)[0x151] = 0;
    update_moby_animation_state(((u8 *)vendor_menu_lui.menu_mobys) + 0x100);
    FUN_0020d4f0_29b680(((u8 *)vendor_menu_lui.menu_mobys) + 0x100, 0x202020, 0xE, 0xE, 0);
    {
        int(*t)[16] = D_L00_001C8FF0_lui;
        x = t[vendor_menu_lui.rows[vendor_menu_lui.cursor].item][12];
    }
    if (x) {
        init_moby_instance(((u8 *)vendor_menu_lui.menu_mobys) + 0x200, x);
        if ((*(unsigned char **)(((u8 *)vendor_menu_lui.menu_mobys) + 0x224))[6]) {
            ((u8 *)vendor_menu_lui.menu_mobys)[0x273] = 0x18;
        }
        *(short *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x232) = 0x40;
        update_entity(((u8 *)vendor_menu_lui.menu_mobys) + 0x200);
        FUN_0020d4f0_29b680(((u8 *)vendor_menu_lui.menu_mobys) + 0x200, 0x202020, 0xE, 0xE, 0);
    }
    init_moby_instance(((u8 *)vendor_menu_lui.menu_mobys) + 0x300, id);
    if ((*(unsigned char **)(((u8 *)vendor_menu_lui.menu_mobys) + 0x224))[6]) {
        ((u8 *)vendor_menu_lui.menu_mobys)[0x273] = 0x18;
    }
    *(short *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x332) = 0x40;
    *(int *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x394) = 0;
    update_entity(((u8 *)vendor_menu_lui.menu_mobys) + 0x300);
    set_moby_animation(((u8 *)vendor_menu_lui.menu_mobys) + 0x300, 0, 0);
    if ((*(unsigned char **)(((u8 *)vendor_menu_lui.menu_mobys) + 0x324))[0xC] >= 2) {
        set_moby_animation(((u8 *)vendor_menu_lui.menu_mobys) + 0x300, 1, 0);
    }
    *(int *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x358) = 0;
    ((u8 *)vendor_menu_lui.menu_mobys)[0x351] = 0;
    update_moby_animation_state(((u8 *)vendor_menu_lui.menu_mobys) + 0x300);
    *(unsigned short *)(((u8 *)vendor_menu_lui.menu_mobys) + 0x334) |= 0x100;
    FUN_0020d4f0_29b680(((u8 *)vendor_menu_lui.menu_mobys) + 0x300, 0x202020, 0xE, 0xE, 0);
    D_L00_00161E38 = 0;
}

/* FUN_L00_0029c648 - Level-00 shared: vendor menu update. Dispatches on the
 * menu state vendor_menu.state (0 = opening, 1 = open, 2 = closing, 3 = exit
 * cutscene). The item list itself (D_L00_001CA4C0) is shared with the
 * buy/consume half FUN_L00_0029ba78, which this function calls every frame. */

typedef struct {
    char pad0[0x10];
    f32  f10;
    f32  f14;
    char pad18[8];
    Vec4 v20;
    char pad30[0x10];
} VendorItemSlot40;

typedef struct {
    f32  f0;
    f32  f4;
    f32  f8;
    char padC[4];
    f32  f10;
    f32  f14;
    char pad18[8];
    Vec4 v20;
} VendorItemSlot30;

extern VendorSound      vendor_sound[] __asm__("D_L00_001C2108");
extern VendorItemSlot40 D_L00_001C8FF0[];
extern VendorItemSlot30 l00_vendor_item_display_transform_parameters[] __asm__("D_L00_001C9930");
extern u8               vendor_moby_update_label[] __asm__("D_L00_00161128");
extern u8               vendor_hero_update_label[] __asm__("D_L00_00161138");
extern u8               vendor_part_update_label[] __asm__("D_L00_00161148");

#include "rnc/overlay/shared/vendor/vendor_menu_data/update_vendor_purchase_2.h"
#include "rnc/overlay/shared/vendor/vendor_menu_data/update_vendor_purchase.h"

extern f32  D_L00_00161E24;
extern s32  D_L00_00161E38;
extern Vec4 D_L00_00165F80[3];

extern void FUN_0022dc50(void);
extern void FUN_00212f90(void *, s32, s32, s32);
extern void draw_vendor_inset_quads(void) __asm__("FUN_L00_0029f990");
extern void FUN_00201a28(void);
extern void pack_render_command_fields(void *, s32, s32, s32, s32) __asm__("FUN_0020d4f0");
extern void advance_moby_animation(void *) __asm__("FUN_0020d580");
extern void update_resident_dialogue_animation_state(void *) __asm__("FUN_00237ed0");
extern f32  scale_time(f32) __asm__("FUN_001f96b0");
extern s32  FUN_001fdd10(s32);
extern void FUN_00237e90(s32);
void        update_vendor_purchase(void) __asm__("FUN_L00_0029ba78");
extern void FUN_002049f0(s32);

#define CUR_ITEM (vendor_menu.rows[vendor_menu.cursor])

/* FUN_L00_0029ba78 - Level-00 shared: per-frame state machine (states 0..4) that
 * drives advance_moby_animation/FUN_L00_00250df8 on g->f20+0x400 and updates the
 * progress tables D_L00_001C40B0 / weapon_ammo_counts / D_0013DD70 / D_0015ED98. */

extern s32 ammo_bought_stats[] __asm__("D_0013DD70");
extern u8  D_0013D4E3[];
extern s32 D_0015ED98;
extern u8  vendor_item_0x18_purchased[] __asm__("D_00141345");
extern s32 vendor_purchase_confirmed_gp __asm__("D_L00_00161E0C") __attribute__((sda));
extern s32 vendor_ammo_quantity_adjustment_enabled_gp __asm__("D_L00_00161E10")
    __attribute__((sda));
extern void FUN_0020d4f0_2ba78(void *, int, int, int, int) __asm__("FUN_0020d4f0");
extern s32  add_ammo(u32, int) __asm__("FUN_L00_00233e40");
extern int  get_help_message_text(int) __asm__("FUN_001fdd10");
extern void set_scrolling_status_message(int) __asm__("FUN_00237e90");

#define CUR_TB (D_L00_001C40B0[CUR_ITEM.item])
void update_vendor_purchase(void) {
    u128 lv;
    s32  purchase_state;
    s32  purchase_cost, k, last_row_index;

    purchase_state = vendor_menu.purchase_state;
    if (purchase_state == 0) {
        return;
    }
    if (purchase_state == 1) {
        D_L00_00161E34 = 1;
        vendor_purchase_confirmed_gp = 0;
        init_moby_instance(&vendor_menu.menu_mobys[4], 0x471);
        vendor_menu.menu_mobys[4].h32 = 0x40;
        lv = 0;
        fast_vec_add(&vendor_menu.menu_mobys[4].pos, &lv, &vendor_menu.vendor_moby->pos);
        {
            VendorMoby *menu_mobys = vendor_menu.menu_mobys;
            qcopy(&menu_mobys[4].rot, &vendor_menu.vendor_moby->rot);
            set_moby_animation(&menu_mobys[4], 0, 0);
        }
        if (((u8 *)vendor_menu.menu_mobys[4].pclass->slots[0])[0x10] < 2) {
            vendor_menu.menu_mobys[4].f58 = 0.0f;
            vendor_menu.menu_mobys[4].next_frame = 0;
            update_moby_animation_state(&vendor_menu.menu_mobys[4]);
        }
        update_entity(&vendor_menu.menu_mobys[4]);
        FUN_0020d4f0_2ba78(&vendor_menu.menu_mobys[4], 0x101010, 0xE, 0xE, 0);
        vendor_ammo_quantity_adjustment_enabled_gp = 0;
        if (CUR_ITEM.is_ammo == 1 && !(weapon_ammo_counts[CUR_ITEM.item] < CUR_TB.hE)) {
            allocate_voice_for_target_entry(2, 0, vendor_menu.vendor_moby);
        } else if (CUR_ITEM.is_ammo == 1) {
            s32 nn = vendor_menu.mode ? CUR_TB.hA : CUR_TB.h8;
            if (D_0015ED98 < nn) {
                allocate_voice_for_target_entry(2, 0, vendor_menu.vendor_moby);
            } else {
                vendor_ammo_quantity_adjustment_enabled_gp = 1;
            }
        } else {
            s32 nn = D_0013D4E3[0] ? CUR_TB.f4 : CUR_TB.f0;
            if (D_0015ED98 < nn) {
                allocate_voice_for_target_entry(2, 0, vendor_menu.vendor_moby);
            }
        }
        vendor_menu.purchase_state = 2;
        return;
    }
    if (purchase_state == 2) {
        if (vendor_ammo_quantity_adjustment_enabled_gp == 1) {
            if (controller_state.held_unmasked & 0x8000) {
                vendor_ammo_decrease_held_frames++;
            } else {
                vendor_ammo_decrease_held_frames = 0;
            }
            if (controller_state.held_unmasked & 0x2000) {
                vendor_ammo_increase_held_frames++;
            } else {
                vendor_ammo_increase_held_frames = 0;
            }
            if ((controller_state.pressed_unmasked & 0x8000) ||
                (vendor_ammo_decrease_held_frames >= 0x10 &&
                 (vendor_ammo_decrease_held_frames & 0x7) == 0) ||
                vendor_ammo_decrease_held_frames >= 0x30) {
                if (D_L00_00161E34 >= 2) {
                    D_L00_00161E34 = D_L00_00161E34 - 1;
                    if ((vendor_ammo_decrease_held_frames & 0x3) == 0 ||
                        vendor_ammo_decrease_held_frames < 0x10) {
                        allocate_voice_for_target_entry(1, 0, vendor_menu.vendor_moby);
                    }
                }
            }
            if ((controller_state.pressed_unmasked & 0x2000) ||
                (vendor_ammo_increase_held_frames >= 0x10 &&
                 (vendor_ammo_increase_held_frames & 0x7) == 0) ||
                vendor_ammo_increase_held_frames >= 0x30) {
                if ((vendor_menu.mode ? CUR_TB.hA : CUR_TB.h8) > 0) {
                    s32 max_ammo_purchase_quantity =
                        D_0015ED98 / (vendor_menu.mode ? CUR_TB.hA : CUR_TB.h8);
                    s32 i = CUR_ITEM.item;
                    s32 left = D_L00_001C40B0[i].hE - weapon_ammo_counts[i];
                    if (left < max_ammo_purchase_quantity)
                        max_ammo_purchase_quantity = left;
                    if (D_L00_00161E34 < max_ammo_purchase_quantity) {
                        D_L00_00161E34++;
                        if ((vendor_ammo_increase_held_frames & 0x3) == 0 ||
                            vendor_ammo_increase_held_frames < 0x10) {
                            allocate_voice_for_target_entry(1, 0, vendor_menu.vendor_moby);
                        }
                    }
                }
            }
        }
        if (controller_state.pressed_unmasked & 0x10) {
            if (!(D_L00_00161E30 & 0x10)) {
                vendor_menu.purchase_state = 3;
                vendor_purchase_confirmed_gp = 0;
                blend_moby_animation(&vendor_menu.menu_mobys[4],
                                     0,
                                     ((u8 *)vendor_menu.menu_mobys[4].pclass->slots[0])[0x10] - 1,
                                     1);
                update_entity(&vendor_menu.menu_mobys[4]);
                return;
            }
        }
        if (controller_state.pressed_unmasked & 0x40) {
            if (!(D_L00_00161E30 & 0x40)) {
                vendor_menu.purchase_state = 3;
                vendor_purchase_confirmed_gp = 1;
                blend_moby_animation(&vendor_menu.menu_mobys[4],
                                     0,
                                     ((u8 *)vendor_menu.menu_mobys[4].pclass->slots[0])[0x10] - 1,
                                     8);
                update_entity(&vendor_menu.menu_mobys[4]);
                return;
            }
        }
        advance_moby_animation(&vendor_menu.menu_mobys[4]);
        if (!vendor_menu.menu_mobys[4].b53 && (vendor_menu.menu_mobys[4].b70 & 2)) {
            blend_moby_animation(&vendor_menu.menu_mobys[4], 1, 0, 1);
        }
        update_entity(&vendor_menu.menu_mobys[4]);
        return;
    }
    if (purchase_state == 3) {
        advance_moby_animation(&vendor_menu.menu_mobys[4]);
        if (!vendor_menu.menu_mobys[4].b53 && (vendor_menu.menu_mobys[4].b70 & 2)) {
            vendor_menu.menu_mobys[4].f58 = -1.0f;
            vendor_menu.purchase_state = 4;
        }
        update_entity(&vendor_menu.menu_mobys[4]);
        return;
    }
    if (purchase_state != 4) {
        return;
    }
    advance_moby_animation(&vendor_menu.menu_mobys[4]);
    if (vendor_menu.menu_mobys[4].b70 & 2) {
        if (vendor_purchase_confirmed_gp == 1) {
            s32 need;

            if (CUR_ITEM.is_ammo == vendor_purchase_confirmed_gp) {
                need = vendor_menu.mode ? CUR_TB.hA * D_L00_00161E34 : CUR_TB.h8 * D_L00_00161E34;
            } else if (D_0013D4E3[0]) {
                need = CUR_TB.f4;
            } else {
                need = CUR_TB.f0;
            }
            if (D_0015ED98 < need && !(weapon_ammo_counts[CUR_ITEM.item] < CUR_TB.hE)) {
                allocate_voice_for_target_entry(0, 0, vendor_menu.vendor_moby);
            } else if (CUR_ITEM.is_ammo == 1) {
                s32             i;
                VendorItemInfo *selected_item_info;

                if (weapon_ammo_counts[CUR_ITEM.item] < (selected_item_info = &CUR_TB)->hE) {
                    purchase_cost =
                        (vendor_menu.mode ? selected_item_info->hA : selected_item_info->h8) *
                        D_L00_00161E34;
                    if (D_0015ED98 >= purchase_cost) {
                        i = CUR_ITEM.item;
                        if (weapon_ammo_counts[i] > D_L00_001C40B0[i].hE) {
                            weapon_ammo_counts[i] = D_L00_001C40B0[i].hE;
                        }
                        k = D_L00_00161E34;
                        k -= add_ammo(CUR_ITEM.item, k);
                        purchase_cost = (vendor_menu.mode ? CUR_TB.hA : CUR_TB.h8) * k;
                        ammo_bought_stats[CUR_ITEM.item] += k;
                        handle_vendor_auxiliary_moby_event(3);
                        D_0015ED98 = D_0015ED98 - purchase_cost;
                        build_vendor_menu_rows(vendor_menu.mode != 0);
                        set_scrolling_status_message(get_help_message_text(0x4F5F));
                        allocate_voice_for_target_entry(7, 0, vendor_menu.vendor_moby);
                    } else {
                        set_scrolling_status_message(get_help_message_text(0x4F60));
                        allocate_voice_for_target_entry(0, 0, vendor_menu.vendor_moby);
                    }
                } else {
                    allocate_voice_for_target_entry(0, 0, vendor_menu.vendor_moby);
                }
            } else {
                purchase_cost = D_0013D4E3[0] ? CUR_TB.f4 : CUR_TB.f0;
                if (D_0015ED98 >= purchase_cost) {
                    handle_vendor_auxiliary_moby_event(2);
                    grant_inventory_item(CUR_ITEM.item, 1);
                    vendor_menu.purchased_item = CUR_ITEM.item;
                    if (CUR_ITEM.item == 0x18) {
                        vendor_item_0x18_purchased[0] = 1;
                    }
                    D_0015ED98 = D_0015ED98 - purchase_cost;
                    build_vendor_menu_rows(vendor_menu.mode != 0);
                    if (vendor_menu.item_text != -1) {
                        set_animation_parameter(vendor_menu.item_text, 0);
                    }
                    vendor_menu.item_text = -1;
                    if (CUR_ITEM.is_ammo == 1) {
                        s32 i = CUR_ITEM.item;

                        vendor_menu.item_text =
                            queue_animation_update(0x30,
                                                   0xEA60 + i,
                                                   initialize_numeric_hud_timer_and_base_dimensions,
                                                   hud_update_counter,
                                                   draw_weapon_ammo_hud_panel,
                                                   &weapon_ammo_counts[i],
                                                   D_L00_001C40B0[i].hE);
                    } else {
                        last_row_index = vendor_menu.row_count - 1;
                        if (last_row_index < vendor_menu.cursor) {
                            vendor_menu.cursor = last_row_index;
                        }
                        initialize_vendor_selected_item_mobys();
                    }
                    set_scrolling_status_message(get_help_message_text(0x4F5F));
                    allocate_voice_for_target_entry(7, 0, vendor_menu.vendor_moby);
                } else {
                    set_scrolling_status_message(get_help_message_text(0x4F60));
                    allocate_voice_for_target_entry(0, 0, vendor_menu.vendor_moby);
                }
            }
        } else {
            allocate_voice_for_target_entry(0, 0, vendor_menu.vendor_moby);
        }
        vendor_menu.purchase_state = 0;
    }
    update_entity(&vendor_menu.menu_mobys[4]);
}

void update_vendor_menu(void) __asm__("FUN_L00_0029c648");

void update_vendor_menu(void) {
    enum {
        VENDOR_MENU_OPENING = 0,
        VENDOR_MENU_OPEN = 1,
        VENDOR_MENU_CLOSING = 2,
        VENDOR_MENU_EXIT_CUTSCENE = 3
    };
    Vec4               position_scratch;
    Vec4               position_contribution_scratch;
    f32                camera_rotation_matrix[4][4];
    s32                vendor_menu_state;
    s32                ok;
    s32                i;
    s32                j;
    s32                k;
    s32                id;
    f32                k1;
    f32                k2;
    f32                k3;
    f32                camera_world_z_rotation;
    f32                one;
    VendorSlot        *row;
    VendorCutscene    *cs;
    CutsceneCameraKey *current_camera_key;
    f32               *rot;
    VendorMoby        *mb;
    VendorMoby        *cutscene_moby_to_remove;
    Vec4              *keys;
    s32                cutscene_frame_threshold;
    u8                 flag;

    switch (vendor_menu.state) {
    case VENDOR_MENU_OPENING:
    case VENDOR_MENU_CLOSING:
        if (vendor_menu.mode == 0 || vendor_menu.state == 2) {
            if ((g_debug_menu.update_flags & 2) ||
                ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
                reset_frame_counters();
            }
            if ((g_debug_menu.update_flags & 2) ||
                ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
                update_visible_resident_objects();
                FUN_0022dc50();
            }
            noop_main_callback(vendor_moby_update_label, 3);
            if ((g_debug_menu.update_flags & 1) ||
                ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
                coordinate_hero_frame_update();
            }
            noop_main_callback(vendor_hero_update_label, 7);
            if ((g_debug_menu.update_flags & 4) ||
                ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
                update_parts();
            }
            noop_main_callback(vendor_part_update_label, 5);
        }
        vendor_menu_state = vendor_menu.state;
        if (vendor_menu_state == 0) {
            fade_level -= 0.34f;
            if (fade_level < 0.0f) {
                fade_level = 0.0f;
            }
            vendor_menu.frames++;
            if (vendor_menu.frames >= scale_game_frames(40) || vendor_menu.mode != 0) {
                fade_level = 0.0f;
                vendor_menu.unk0C = 1;
                vendor_menu.state = 1;
                vendor_menu.frames = 0;
                FUN_00212f90(vendor_menu.vendor_moby, 3, 0, 8);
                vendor_menu.vendor_moby->f58 = 1.0f;
                D_L00_00161E20 = 1;
                vendor_opening_countdown_frames = 8;
                allocate_voice_for_target_entry(4, 0, vendor_menu.vendor_moby);
            }
        } else if (vendor_menu_state == 2) {
            ok = 0;
            if (vendor_menu.mode == 0 && vendor_menu.purchased_item >= 0) {
                if (vendor_exit_cutscene_ids_by_item[vendor_menu.purchased_item] >= 0) {
                    ok = 1;
                }
            }
            vendor_menu.frames++;
            if (vendor_menu.frames >= scale_game_frames(40) || vendor_menu.mode != 0) {
                FUN_00212f90(vendor_menu.vendor_moby, 1, 0, 8);
                row = vendor_slots;
                for (k = 0; k < 4; k++) {
                    if (row->hidden == 1) {
                        detach_manipulator(vendor_menu.vendor_moby, row);
                    }
                    row++;
                }
                if (ok) {
                    start_vendor_exit_cutscene();
                } else {
                    close_vendor_menu(0);
                    allocate_voice_for_target_entry(6, 0, vendor_menu.vendor_moby);
                }
            }
            update_hud_elements(1);
        }
        if (vendor_menu.frames <= scale_game_frames(36)) {
            add_draw_callback(draw_vendor_inset_quads, vendor_menu.vendor_moby);
        }
        if (vendor_menu.mode == 0) {
            set_camera_position(&vendor_menu.eye);
            set_camera_rotation(&vendor_menu.look);
        }
        FUN_00201a28();
        if ((g_debug_menu.update_flags & 2) ||
            ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
            update_game_frame_and_idle_counters();
        }
        break;
    case VENDOR_MENU_OPEN:
        if (D_L00_00161188 == 0 && cd_read_active[0] == 0 && D_L00_0016118C != 0) {
            decompress_and_relocate_vendor_moby_assets();
            vendor_menu.unk24 = vendor_menu.unk14;
            init_moby_instance(vendor_menu.unk24, 0xC);
            vendor_menu.unk24->h32 = 0x40;
            vendor_menu.unk24->rot.f[2] = vendor_menu.vendor_moby->rot.f[2];
            update_entity(vendor_menu.unk24);
            pack_render_command_fields(vendor_menu.unk24, 0x202020, 0xE, 0xE, 0);
            D_L00_00161188 = 1;
            D_L00_0016118C = 0;
            handle_vendor_auxiliary_moby_event(1);
        }
        advance_moby_animation(vendor_menu.vendor_moby);
        update_entity(vendor_menu.vendor_moby);
        if (D_L00_00161188 == 1) {
            transform_vector_by_basis(&vendor_menu.unk24->pos,
                                      &vendor_auxiliary_moby_local_offset_gp,
                                      vendor_menu.vendor_moby->mtx);
            fast_vec_add(
                &vendor_menu.unk24->pos, &vendor_menu.unk24->pos, &vendor_menu.vendor_moby->pos);
            advance_moby_animation(vendor_menu.unk24);
            update_entity(vendor_menu.unk24);
            update_resident_dialogue_animation_state(vendor_menu.unk24);
        }
        k1 = -0.05f;
        k2 = -0.36f;
        transform_vector_by_basis(&vendor_menu.menu_mobys[0].pos,
                                  &vendor_rotating_display_local_offset_gp,
                                  vendor_menu.vendor_moby->mtx);
        k3 = 0.05f;
        fast_vec_add(&vendor_menu.menu_mobys[0].pos,
                     &vendor_menu.menu_mobys[0].pos,
                     &vendor_menu.vendor_moby->pos);
        vendor_menu.menu_mobys[0].rot.f[0] =
            fast_add_rotations(vendor_menu.menu_mobys[0].rot.f[0], scale_time(k1));
        vendor_menu.menu_mobys[0].rot.f[1] = k2;
        vendor_menu.menu_mobys[0].rot.f[2] =
            fast_add_rotations(vendor_menu.menu_mobys[0].rot.f[2], scale_time(k3));
        update_entity(&vendor_menu.menu_mobys[0]);
        transform_vector_by_basis(&vendor_menu.menu_mobys[5].pos,
                                  &vendor_menu_rotating_display_local_offset_gp,
                                  vendor_menu.vendor_moby->mtx);
        fast_vec_add(&vendor_menu.menu_mobys[5].pos,
                     &vendor_menu.menu_mobys[5].pos,
                     &vendor_menu.vendor_moby->pos);
        vendor_menu.menu_mobys[5].rot.f[0] =
            fast_add_rotations(vendor_menu.menu_mobys[5].rot.f[0], scale_time(k1));
        vendor_menu.menu_mobys[5].rot.f[1] = k2;
        vendor_menu.menu_mobys[5].rot.f[2] =
            fast_add_rotations(vendor_menu.menu_mobys[5].rot.f[2], scale_time(k3));
        update_entity(&vendor_menu.menu_mobys[5]);
        D_L00_00161E24 = fast_add_rotations(D_L00_00161E24, scale_time(k3));
        advance_moby_animation(&vendor_menu.menu_mobys[1]);
        if (!vendor_menu.menu_mobys[1].b53 && (vendor_menu.menu_mobys[1].b70 & 2)) {
            FUN_00212f90(&vendor_menu.menu_mobys[1], 1, 0, 8);
        }
        update_entity(&vendor_menu.menu_mobys[1]);
        position_contribution_scratch.q = 0;
        position_contribution_scratch.f[2] = fast_sin(D_L00_00161E24) / 20.0f;
        position_scratch.q = position_contribution_scratch.q;
        fast_vec_add(
            &position_scratch, &vendor_item_display_common_local_offset_gp, &position_scratch);
        fast_vec_add(&position_scratch, &position_scratch, &D_L00_001C8FF0[CUR_ITEM.item].v20);
        transform_vector_by_basis(
            &vendor_menu.menu_mobys[2].pos, &position_scratch, vendor_menu.vendor_moby->mtx);
        fast_vec_add(&vendor_menu.menu_mobys[2].pos,
                     &vendor_menu.menu_mobys[2].pos,
                     &vendor_menu.vendor_moby->pos);
        vendor_menu.menu_mobys[2].rot.f[0] = D_L00_001C8FF0[CUR_ITEM.item].f10;
        vendor_menu.menu_mobys[2].rot.f[1] = D_L00_001C8FF0[CUR_ITEM.item].f14;
        vendor_menu.menu_mobys[2].rot.f[2] = D_L00_00161E24;
        update_entity(&vendor_menu.menu_mobys[2]);
        position_contribution_scratch.q = 0;
        position_contribution_scratch.f[2] = fast_sin(D_L00_00161E24) / 20.0f;
        position_scratch.q = position_contribution_scratch.q;
        fast_vec_add(
            &position_scratch, &vendor_item_display_common_local_offset_gp, &position_scratch);
        fast_vec_add(&position_scratch,
                     &position_scratch,
                     &l00_vendor_item_display_transform_parameters[CUR_ITEM.item].v20);
        transform_vector_by_basis(
            &vendor_menu.menu_mobys[3].pos, &position_scratch, vendor_menu.vendor_moby->mtx);
        fast_vec_add(&vendor_menu.menu_mobys[3].pos,
                     &vendor_menu.menu_mobys[3].pos,
                     &vendor_menu.vendor_moby->pos);
        vendor_menu.menu_mobys[3].rot.f[0] =
            l00_vendor_item_display_transform_parameters[CUR_ITEM.item].f10;
        vendor_menu.menu_mobys[3].rot.f[1] =
            l00_vendor_item_display_transform_parameters[CUR_ITEM.item].f14;
        vendor_menu.menu_mobys[3].rot.f[2] = D_L00_00161E24;
        advance_moby_animation(&vendor_menu.menu_mobys[3]);
        if (!vendor_menu.menu_mobys[3].b53 && (vendor_menu.menu_mobys[3].b70 & 2)) {
            FUN_00212f90(&vendor_menu.menu_mobys[3], 1, 0, 8);
        }
        update_entity(&vendor_menu.menu_mobys[3]);
        position_contribution_scratch.f[0] =
            l00_vendor_item_display_transform_parameters[CUR_ITEM.item].f0;
        position_contribution_scratch.f[1] =
            l00_vendor_item_display_transform_parameters[CUR_ITEM.item].f4;
        position_contribution_scratch.f[2] =
            l00_vendor_item_display_transform_parameters[CUR_ITEM.item].f8;
        euler_rotation_basis(vendor_menu.menu_mobys[3].mtx, &vendor_menu.menu_mobys[3].rot);
        transform_vector_by_basis(&position_contribution_scratch,
                                  &position_contribution_scratch,
                                  vendor_menu.menu_mobys[3].mtx);
        fast_vec_add(&vendor_menu.menu_mobys[3].pos,
                     &vendor_menu.menu_mobys[3].pos,
                     &position_contribution_scratch);
        vendor_menu.frames++;
        if (vendor_menu.purchase_state == 0 && D_L00_00161E18 == 0) {
            if (vendor_menu.row_count < 8) {
                if ((controller_state.pressed_unmasked & 0x2000) && !(D_L00_00161E30 & 0x2000)) {
                    if (vendor_menu.cursor < vendor_menu.row_count - 1) {
                        D_L00_00161E38 = 1;
                        handle_vendor_auxiliary_moby_event(0);
                        vendor_menu.cursor++;
                        allocate_voice_for_target_entry(1, 0, vendor_menu.vendor_moby);
                        FUN_00237e90(FUN_001fdd10(vendor_sound[CUR_ITEM.item].message));
                        if (vendor_menu.item_text != -1) {
                            set_animation_parameter(vendor_menu.item_text, 0);
                        }
                        vendor_menu.item_text = -1;
                        if (CUR_ITEM.is_ammo == 1) {
                            vendor_menu.item_text = queue_animation_update(
                                0x30,
                                CUR_ITEM.item + 0xEA60,
                                initialize_numeric_hud_timer_and_base_dimensions,
                                hud_update_counter,
                                draw_weapon_ammo_hud_panel,
                                &weapon_ammo_counts[CUR_ITEM.item],
                                D_L00_001C40B0[CUR_ITEM.item].hE);
                        }
                    }
                }
                if ((controller_state.pressed_unmasked & 0x8000) && !(D_L00_00161E30 & 0x8000)) {
                    if (vendor_menu.cursor > 0) {
                        D_L00_00161E38 = 1;
                        handle_vendor_auxiliary_moby_event(0);
                        vendor_menu.cursor--;
                        allocate_voice_for_target_entry(1, 0, vendor_menu.vendor_moby);
                        FUN_00237e90(FUN_001fdd10(vendor_sound[CUR_ITEM.item].message));
                        if (vendor_menu.item_text != -1) {
                            set_animation_parameter(vendor_menu.item_text, 0);
                        }
                        vendor_menu.item_text = -1;
                        if (CUR_ITEM.is_ammo == 1) {
                            vendor_menu.item_text = queue_animation_update(
                                0x30,
                                CUR_ITEM.item + 0xEA60,
                                initialize_numeric_hud_timer_and_base_dimensions,
                                hud_update_counter,
                                draw_weapon_ammo_hud_panel,
                                &weapon_ammo_counts[CUR_ITEM.item],
                                D_L00_001C40B0[CUR_ITEM.item].hE);
                        }
                    }
                }
            } else {
                if ((controller_state.pressed_unmasked & 0x2000) && !(D_L00_00161E30 & 0x2000)) {
                    if (vendor_menu.scroll <= 0x38) {
                        D_L00_00161E38 = 1;
                        vendor_menu.scroll += 0x38;
                        allocate_voice_for_target_entry(1, 0, vendor_menu.vendor_moby);
                        handle_vendor_auxiliary_moby_event(0);
                        if (vendor_menu.cursor < vendor_menu.row_count - 1) {
                            vendor_menu.cursor++;
                        } else {
                            vendor_menu.cursor = 0;
                        }
                        FUN_00237e90(FUN_001fdd10(vendor_sound[CUR_ITEM.item].message));
                        if (vendor_menu.item_text != -1) {
                            set_animation_parameter(vendor_menu.item_text, 0);
                        }
                        vendor_menu.item_text = -1;
                        if (CUR_ITEM.is_ammo == 1) {
                            vendor_menu.item_text = queue_animation_update(
                                0x30,
                                CUR_ITEM.item + 0xEA60,
                                initialize_numeric_hud_timer_and_base_dimensions,
                                hud_update_counter,
                                draw_weapon_ammo_hud_panel,
                                &weapon_ammo_counts[CUR_ITEM.item],
                                D_L00_001C40B0[CUR_ITEM.item].hE);
                        }
                    }
                }
                if ((controller_state.pressed_unmasked & 0x8000) && !(D_L00_00161E30 & 0x8000)) {
                    if (vendor_menu.scroll >= -0x38) {
                        D_L00_00161E38 = 1;
                        vendor_menu.scroll -= 0x38;
                        allocate_voice_for_target_entry(1, 0, vendor_menu.vendor_moby);
                        handle_vendor_auxiliary_moby_event(0);
                        if (vendor_menu.cursor > 0) {
                            vendor_menu.cursor--;
                        } else {
                            vendor_menu.cursor = vendor_menu.row_count - 1;
                        }
                        FUN_00237e90(FUN_001fdd10(vendor_sound[CUR_ITEM.item].message));
                        if (vendor_menu.item_text != -1) {
                            set_animation_parameter(vendor_menu.item_text, 0);
                        }
                        vendor_menu.item_text = -1;
                        if (CUR_ITEM.is_ammo == 1) {
                            vendor_menu.item_text = queue_animation_update(
                                0x30,
                                CUR_ITEM.item + 0xEA60,
                                initialize_numeric_hud_timer_and_base_dimensions,
                                hud_update_counter,
                                draw_weapon_ammo_hud_panel,
                                &weapon_ammo_counts[CUR_ITEM.item],
                                D_L00_001C40B0[CUR_ITEM.item].hE);
                        }
                    }
                }
            }
            if ((controller_state.pressed_unmasked & 0x40) && !(D_L00_00161E30 & 0x40) &&
                CUR_ITEM.state == 0) {
                vendor_menu.purchase_state = 1;
                allocate_voice_for_target_entry(0, 0, vendor_menu.vendor_moby);
            }
            if ((controller_state.pressed_unmasked & 0x10) && D_L00_0016118C == 0) {
                vendor_close_delay_frames_remaining = 8;
                D_L00_00161E18 = 1;
                allocate_voice_for_target_entry(5, 0, vendor_menu.vendor_moby);
            }
        }
        update_vendor_purchase();
        D_L00_00161E30 = controller_state.pressed_unmasked;
        if (D_L00_00161E20 == 1) {
            if (vendor_opening_countdown_frames == 0) {
                D_L00_00161E20 = 0;
            } else {
                vendor_opening_countdown_frames--;
            }
        }
        if (D_L00_00161E18 == 1) {
            if (--vendor_close_delay_frames_remaining == 0) {
                D_L00_0015F5D8 = 1;
                calculate_dma_transfer_address();
                FUN_00212f90(vendor_menu.vendor_moby, 4, 9, 8);
                vendor_menu.vendor_moby->f58 = frame_scale * -0.5f;
                if (vendor_menu.mode != 0) {
                    qcopy(g_debug_camera.angles, &vendor_menu.look);
                }
                ((Vec4 *)g_debug_camera.basis)[0].q = D_L00_00165F80[0].q;
                ((Vec4 *)g_debug_camera.basis)[1].q = D_L00_00165F80[1].q;
                ((Vec4 *)g_debug_camera.basis)[2].q = D_L00_00165F80[2].q;
                vendor_menu.state = 2;
                vendor_menu.unk08 = 0;
                vendor_menu.frames = 0;
                if (hero.items[0].moby != 0) {
                    select_world_object_resource_tables(((VendorMoby *)hero.items[0].moby)->oclass,
                                                        -1);
                    ((VendorMoby *)hero.items[0].moby)->pclass =
                        D_L00_00197300[((VendorMoby *)hero.items[0].moby)->class_id];
                    update_moby_animation_state(hero.items[0].moby);
                }
            }
        } else if (D_L00_00161E38 != 0) {
            initialize_vendor_selected_item_mobys();
        }
        update_hud_elements(1);
        break;
    case VENDOR_MENU_EXIT_CUTSCENE:
        reset_frame_counters();
        update_visible_resident_objects();
        FUN_0022dc50();
        coordinate_hero_frame_update();
        update_parts();
        D_L00_0016C860.f38++;
        D_L00_0016C860.f34++;
        if (D_L00_0016C860.h40 - 12 < D_L00_0016C860.f34) {
            fade_level += 0.1f;
            if (fade_level > 1.0f) {
                fade_level = 1.0f;
            }
        } else {
            fade_level -= 0.125f;
            if (fade_level < 0.0f) {
                fade_level = 0.0f;
            }
        }
        cs = &D_L00_0016C860;
        if (cs->f34 >= cs->h40) {
            fade_level = 0.0f;
            calculate_dma_transfer_address();
            D_L00_0016CAF0_lui = 0.63f;
            update_view_context();
            for (j = 0; j < D_L00_0016C860.h44; j++) {
                cutscene_moby_to_remove = D_L00_0016C860.f178[j];
                if (cutscene_moby_to_remove != 0) {
                    cutscene_moby_to_remove->pclass->count--;
                    cutscene_moby_to_remove->pclass->slots[cutscene_moby_to_remove->pclass->count] =
                        0;
                    mark_moby_for_removal(cutscene_moby_to_remove);
                }
            }
            if (vendor_menu.unk28 != 0) {
                restore_hero_after_vendor_exit_cutscene();
            } else {
                close_vendor_menu(1);
            }
            break;
        }
        cutscene_frame_threshold = D_0015ED80 ? 0x50 : 0x60;
        if (cs->f38 >= cutscene_frame_threshold) {
            cs->f3C++;
            FUN_002049f0(cs->f3C);
        }
        current_camera_key = &cs->f54[cs->f38];
        rot = current_camera_key->rot;
        flag = current_camera_key->flag;
        D_L00_0016CAF0_lui = rot[3];
        update_view_context();
        qcopy(l00_camera.pos.f, current_camera_key);
        transform_vector_by_basis(l00_camera.pos.f, l00_camera.pos.f, vendor_menu.basis);
        fast_vec_add(l00_camera.pos.f, l00_camera.pos.f, &vendor_menu.unkC0);
        camera_world_z_rotation = fast_add_rotations(rot[2], vendor_menu.basis[3].f[2]);
        sce_vu0_unit_matrix(camera_rotation_matrix);
        sce_vu0_rot_matrix_x(camera_rotation_matrix, camera_rotation_matrix, rot[0]);
        sce_vu0_rot_matrix_y(camera_rotation_matrix, camera_rotation_matrix, rot[1]);
        sceVu0RotMatrixZ(camera_rotation_matrix, camera_rotation_matrix, camera_world_z_rotation);
        l00_camera.unk350[0] = -camera_rotation_matrix[2][0];
        l00_camera.unk350[4] = -camera_rotation_matrix[0][0];
        l00_camera.unk350[8] = camera_rotation_matrix[1][0];
        l00_camera.unk350[1] = -camera_rotation_matrix[2][1];
        l00_camera.unk350[5] = -camera_rotation_matrix[0][1];
        l00_camera.unk350[9] = camera_rotation_matrix[1][1];
        l00_camera.unk350[2] = -camera_rotation_matrix[2][2];
        l00_camera.unk350[6] = -camera_rotation_matrix[0][2];
        l00_camera.unk350[10] = camera_rotation_matrix[1][2];
        for (i = 0; i < D_L00_0016C860.h44; i++) {
            mb = D_L00_0016C860.f178[i];
            id = D_L00_0016C860.f38 >> 1;
            mb->next_frame = id + 1;
            mb->frame = id;
            update_moby_animation_state(mb);
            mb->blend = convert_integer_to_float(D_L00_0016C860.f38 & 1) * 0.5f;
            if (flag && (D_L00_0016C860.f38 & 1)) {
                mb->blend = 1.0f;
            }
            keys = mb->keys;
            fast_vec_scale(&position_scratch, &keys[mb->frame], 1.0f - mb->blend);
            fast_vec_scale(&position_contribution_scratch, &keys[mb->next_frame], mb->blend);
            fast_vec_add(&mb->pos, &position_scratch, &position_contribution_scratch);
            transform_vector_by_basis(&mb->pos, &mb->pos, vendor_menu.basis);
            fast_vec_add(&mb->pos, &mb->pos, &vendor_menu.unkC0);
            mb->rot.f[2] = vendor_menu.basis[3].f[2];
            mb->b71 = 0xFF;
            update_entity(mb);
            if (mb->b7F) {
                update_moby_shadow_range(mb);
            }
            if (mb->oclass == 0) {
                apply_hero_moby_scale(mb);
            }
        }
        FUN_00201a28();
        update_game_frame_and_idle_counters();
        break;
    }
    FUN_0022ca50();
}
