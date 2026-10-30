/* Cutscene, movie and slideshow playback, the game frame and idle counters and pause input; also defines the vendor menu data (vendor/vendor_menu.c holds its code). */
#include "types.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/game/scene_trigger.h"
#include "rnc/game/overlay_game_state.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/mobys/moby.h"
#include "rnc/audio/music/music_stream_state.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/streaming/disc_table.h"
#include "rnc/math/vector.h"
#include "rnc/input/pad_state.h"
#include "rnc/rendering/debug/debug_menu.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
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
#include "rnc/audio/music/music_stop.h"
#include "rnc/audio/streaming/continue_audio_stream_if_ready.h"
#include "rnc/audio/streaming/safe_cd/snd_stream_safe_cd_sync.h"
#include "rnc/audio/voices/snd_continue_all_sounds_in_group.h"
#include "rnc/audio/voices/snd_pause_all_sounds_in_group.h"
#include "rnc/audio/voices/sound_stop_all_sounds.h"
#include "rnc/mobys/animation/update_moby_animation_state.h"
#include "rnc/mobys/detach_manipulator.h"
#include "rnc/mobys/init_moby_instance.h"
#include "rnc/mobys/update_visible_mobys.h"
#include "rnc/overlay/shared/ui/hud/hud_update_counter.h"
#include "rnc/overlay/shared/rendering/buffers/vu1_send_chain_render_buffer.h"
#include "rnc/world/loaders/relocate_asset_entry_pointers.h"
#include "rnc/save/memcard_update_state.h"
#include "rnc/world/select_world_object_resource_tables.h"
#include "rnc/overlay/shared/vendor/vendor_menu.h"

/* Ported from rac1-decomp (src/overlays/shared/tieproc_00299108.c: func_L00_00299108), where it is exact; names translated to the US level program. */

extern int D_L00_0015F2F0;
extern int D_L00_0015F2F4;
extern int D_L00_0015F424;
extern int D_L00_0015F428;
extern int D_L00_0015F42C;
extern int D_L00_0015F430;
extern int D_L00_0015F434;
extern int D_L00_0017E504[];

/* Clears D_L00_0015F424..0015F434, D_L00_0017E504[0] and D_L00_0015F2F0/2F4. */
void reset_frame_counters(void) __asm__("FUN_L00_00297e30");

void reset_frame_counters(void) {
    D_L00_0015F424 = 0;
    D_L00_0017E504[0] = 0;
    D_L00_0015F42C = 0;
    D_L00_0015F430 = 0;
    D_L00_0015F428 = 0;
    D_L00_0015F434 = 0;
    D_L00_0015F2F0 = 0;
    D_L00_0015F2F4 = 0;
}

#include "rnc/game/usage_stats.h"

extern struct PadStateWords D_0013C940;

int        controller_idle_frames_gp __asm__("D_L00_00160FF0") = 0;
extern int D_L00_0015F5CC;
extern int D_0015EEA4;
extern int D_L00_0015F598;
extern int D_0015EE28;
void       update_game_frame_and_idle_counters(void) __asm__("FUN_L00_00297e70");

void update_game_frame_and_idle_counters(void) {
    if (D_0013C940.buttons.w.held != 0 || D_0013C940.analog[2] != 0.0f ||
        D_0013C940.analog[3] != 0.0f || D_0013C940.analog[0] != 0.0f ||
        D_0013C940.analog[1] != 0.0f) {
        controller_idle_frames_gp = 0;
    } else {
        controller_idle_frames_gp++;
    }
    D_L00_0015F5CC++;
    D_0015EEA4++;
    if (D_L00_0015F598) {
        if (controller_idle_frames_gp < (int)((float)scale_game_frames(15) * 60.0f)) {
            D_0015EE28++;
        }
    }
}

typedef struct {
    u8  pad0[8];
    s32 w8;
    u8  padc[0x14 - 0xC];
    s32 w14;
    s32 w18;
    u8  pad1c[0xA0 - 0x1C];
    s32 wA0;
    u8  padA4[0xEC - 0xA4];
    s32 wEC;
} S_297f78;

typedef struct {
    u8  pad0[0x19C];
    s32 w19C;
    s32 w1A0;
    s32 w1A4;
    s32 w1A8;
} C_297f78;

typedef struct {
    u8  pad0[0xA6];
    s16 hA6;
} A_297f78;

typedef struct {
    u8        pad0[0x10AC];
    u8        b10AC;
    u8        pad10ad[0x15F0 - 0x10AD];
    A_297f78 *p15F0;
    u8        pad15f4[0x2084 - 0x15F4];
    s32       w2084;
    u8        pad2088[4];
    s32       w208C;
    u8        pad2090[0x20A4 - 0x2090];
    u8        b20A4;
    u8        pad20a5[0x20B1 - 0x20A5];
    u8        b20B1;
    u8        pad20b2[0x22A8 - 0x20B2];
    s32       w22A8;
} G_297f78;

typedef struct {
    u8  pad0[0x86];
    s16 h86;
} P_297f78;

extern S_297f78                 D_L00_0016C058;
extern C_297f78                 D_0013C940_c __asm__("D_0013C940");
extern s32                      D_L00_0015F5C0_macro[] __asm__("D_L00_0015F5C0") MACRO_ADDR;
extern s32                      level_timer __asm__("D_L00_0015F5C8");
extern s32                      game_frame __asm__("D_L00_0015F5CC");
extern s32 D_L00_001B9DE0       NOT_SDA;
extern P_297f78 *D_L00_00166E00 NOT_SDA;
extern s32 D_0013D384           NOT_SDA;

#include "rnc/game/item_state.h"

extern s32                alternate_animation_sequence __asm__("D_0015EE20");
extern s32 D_L00_00184214 NOT_SDA;
extern s32                D_0015EEA4;
extern s32                D_L00_0015F630;
extern u8 D_L00_00160FF8[], D_L00_00161008[], D_L00_00161018[], D_L00_00161028[], D_L00_00161038[];
extern void mode_freeze_init(s32, s32) __asm__("FUN_001fbab8");
extern void pause_all_sounds(s32) __asm__("FUN_00218d78");
extern void update_timed_hud_visibility(void) __asm__("FUN_L00_00264328");
extern s32  show_health_hud(void) __asm__("FUN_L00_00234e00");
extern void hud_keep_visible(s32, s32) __asm__("FUN_L00_00235e18");
extern s32
queue_animation_update(s32, s32, void *, void *, void *, void *, s32) __asm__("FUN_001ff308");
extern void prepare_level_transition_with_help_cleanup(void) __asm__("FUN_L00_0028ecd8");
extern void reset_frame_counters(void) __asm__("FUN_L00_00297e30");
extern void count_expired_mobys(void) __asm__("FUN_L00_0024e7a8");
extern void update_all_mobys_profiled(void) __asm__("FUN_L00_002648e0");
extern void enqueue_voice_request(void) __asm__("FUN_0022dc50");
extern void update_ambience_target_cycle(void) __asm__("FUN_L00_002a4890");
extern void noop_main_callback(void *, s32) __asm__("FUN_001e93e8");
extern void no_op_main_callback_alt(void) __asm__("FUN_001e93e8");
extern void coordinate_hero_frame_update(void) __asm__("FUN_L00_002070d0");
extern void update_parts(void) __asm__("FUN_00217b88");
extern void update_glints(void) __asm__("FUN_L00_001fea18");
extern void update_camera(void) __asm__("FUN_001edaa8");
extern void update_debug_camera(void) __asm__("FUN_L00_001f3aa0");
extern void update_hero_from_debug_camera(void) __asm__("FUN_L00_001f4490");
extern void sound_update(void) __asm__("FUN_0022ca50");
extern void update_all_point_lights(void) __asm__("FUN_00201a28");
extern void update_resident_packed_direction(void) __asm__("FUN_0020cfd0");
extern void update_hud_elements(s32) __asm__("FUN_L00_0023ac78");
extern void update_help_state(void) __asm__("FUN_001fde90");
extern void update_game_frame_and_idle_counters(void) __asm__("FUN_L00_00297e70");

#define DBG(b) \
    ((D_L00_0016C058.w8 & (b)) || ((D_L00_0016C058.w8 & 0x10) && (D_0013C940_c.w1A4 & 0x200)))
#define HELD ((D_0013C940_c.w1A4 & 0x800) || noctl)

void handle_pause_input(void) __asm__("FUN_L00_00297f78");

void handle_pause_input(void) {
    s32 lvl, area, noctl, special;
    if (D_L00_0016C058.wEC) {
        if (--D_L00_0016C058.wEC == 0)
            D_L00_0016C058.w8 = 0x10;
    }
    if ((D_L00_0016C058.w8 & 0x10) && (D_0013C940_c.w1A4 & 0x200)) {
        D_0013C940_c.w1A4 = ~D_L00_0016C058.wA0 & D_0013C940_c.w1A0;
        D_0013C940_c.w1A8 = ~D_0013C940_c.w1A0 & D_L00_0016C058.wA0;
        D_L00_0016C058.wA0 = D_0013C940_c.w1A0 & ~0x200;
    }
    lvl = D_L00_0015F5C0_macro[1];
    if (lvl == 0 && (mode_freeze_flags & 1) && level_timer >= 8) {
        mode_freeze_init(3, 0);
        return;
    }
    noctl = D_0013C940_c.w19C == 0;
    if (D_L00_001B9DE0 && lvl == 0) {
        pause_all_sounds(0);
        return;
    }
    area = current_level_index;
    if (area == 15 && level_timer >= 8 && hero.vehicle_mode == 2 && HELD &&
        hero.state.current != 0x72) {
        if (D_L00_00166E00->h86 == 0x13)
            return;
        mode_freeze_init(4, 0);
        return;
    }
    special = 0;
    if (hero.state.control_mode == 9 && hero.state.current == 0x32 && hero.ship_moby) {
        s32 k = ((A_297f78 *)hero.ship_moby)->hA6;
        if (k == 0x45 || k == 0x563 || k == 0x4DA)
            special = 1;
    }
    if (special && level_timer >= 8 && HELD && hero.state.current != 0x72 &&
        D_L00_00166E00->h86 != 0x13) {
        mode_freeze_init(1, 0);
        return;
    }
    if (lvl == 0 && (area == 8 || area == 0xC) && HELD && hero.state.current == 0x32) {
        mode_freeze_init(1, 0);
        return;
    }
    if (HELD && level_timer >= 8 && hero.state.current != 0x72 && hero.state.control_mode == 0x16) {
        mode_freeze_init(0, 0);
        return;
    }
    if (HELD && level_timer >= 8 && hero.state.current != 0x72 && hero.items[0].switch_lock != 2 &&
        hero.state.current != 0x32 && hero.state.current != 0x1D && !hero.fall_fade_pending &&
        hero.health.hp && lvl == 0) {
        pause_all_sounds(0);
        return;
    }
    if (!(D_L00_0016C058.w8 & 0x10) && level_timer >= 8 && (D_0013C940_c.w1A4 & 0x500) &&
        hero.state.control_mode != 0x16 && hero.state.current != 0x72 &&
        hero.state.current != 0x32 && hero.state.current != 0x1D && !hero.fall_fade_pending &&
        hero.health.hp && lvl == 0 && !special && !D_L00_0016C058.wEC) {
        pause_all_sounds(10);
        return;
    }
    if (D_0013D384 && !alternate_item_available[0x10] && level_timer >= 8 &&
        !alternate_animation_sequence && !hero.fall_fade_pending && hero.health.hp &&
        scale_game_frames(0x1E) < game_frame && current_level_index) {
        mode_freeze_init(5, 0);
        alternate_item_available[0x10] = 1;
        return;
    }
    if (hero.state.current != 0x1D && hero.state.current != 0x32)
        update_timed_hud_visibility();
    if ((D_0013C940_c.w1A4 & 0x200) && hero.state.current != 0x1D && hero.state.current != 0x32) {
        s32 x;
        x = show_health_hud();
        hud_keep_visible(x, scale_game_frames(0xB4));
        x = queue_animation_update(2,
                                   0x754E,
                                   initialize_bolt_counter_hud,
                                   update_bolt_counter_hud_animation_and_visibility,
                                   draw_bolt_counter_hud_panel,
                                   &current_bolt_count,
                                   9999999);
        hud_keep_visible(x, scale_game_frames(0xB4));
    }
    if (D_L00_00184214) {
        struct UsageStats *usage_stats_ptr = &usage_stats;
        if (usage_stats_ptr->stat[9].count < 0xFFFF)
            usage_stats_ptr->stat[9].count++;
        if (scale_game_frames(D_0015EEA4) / 600 > usage_stats_ptr->stat[9].unk2)
            usage_stats_ptr->stat[9].unk2 = scale_game_frames(D_0015EEA4) / 600;
        usage_stats_ptr->stat[9].level_mask =
            usage_stats_ptr->stat[9].level_mask | (1 << current_level_index) | 0x80000000;
    }
    if (D_L00_0015F630) {
        D_L00_0015F630 = 0;
        if (hero.health.hp) {
            prepare_level_transition_with_help_cleanup();
            return;
        }
    }
    if (DBG(2))
        reset_frame_counters();
    if (DBG(2)) {
        count_expired_mobys();
        update_all_mobys_profiled();
        enqueue_voice_request();
        update_ambience_target_cycle();
    }
    noop_main_callback(D_L00_00160FF8, 3);
    if (DBG(1))
        coordinate_hero_frame_update();
    noop_main_callback(D_L00_00161008, 7);
    if (DBG(4)) {
        update_parts();
        update_glints();
    }
    noop_main_callback(D_L00_00161018, 5);
    if (DBG(8)) {
        update_camera();
    } else if (D_L00_0016C058.w14) {
        update_debug_camera();
        if (D_L00_0016C058.w14 == 2)
            update_hero_from_debug_camera();
    }
    noop_main_callback(D_L00_00161028, 8);
    sound_update();
    noop_main_callback(D_L00_00161038, 6);
    update_all_point_lights();
    update_resident_packed_direction();
    update_hud_elements(1);
    update_help_state();
    if (DBG(2))
        update_game_frame_and_idle_counters();
    if (D_L00_0016C058.w18 == 2)
        no_op_main_callback_alt();
    if (D_L00_0016C058.w18 == 3)
        no_op_main_callback_alt();
    if (D_L00_0016C058.w18 == 4)
        no_op_main_callback_alt();
    if (D_L00_0016C058.w18 == 5)
        no_op_main_callback_alt();
    if (D_L00_0016C058.w18 == 6)
        no_op_main_callback_alt();
}

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
extern char     D_L00_0017C380_lui[] __asm__("D_L00_0017C380") NOT_SDA;
extern char     D_L00_0017C400_lui[] __asm__("D_L00_0017C400") NOT_SDA;
extern char     D_L00_0017C3C0_lui[] __asm__("D_L00_0017C3C0") NOT_SDA;

extern struct {
    int x0, x4, x8;
} D_L00_00173E00_298840 __asm__("D_L00_00173E00") __attribute__((section(".data")));

extern float     D_L00_0015F5A0 __asm__("D_L00_0015F5A0");
extern float     fade_level __asm__("D_L00_0015F3FC");
extern int       D_L00_0015F400 __asm__("D_L00_0015F400");
extern O_298840 *D_L00_0015FFDC_298840 __asm__("D_L00_0015FFDC");
void             music_pause(int) __asm__("FUN_00216050");
extern int       update_audio_stream_until_idle(int) __asm__("FUN_002168a8");
void             fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");
void             clear_active_help_record(void) __asm__("FUN_L00_002039a0");
void             f232fe8_298840(void) __asm__("FUN_L00_00232fe8");
int              start_cutscene_chunk_read(int) __asm__("FUN_L00_002451b8");
void             update_world_object_animation(void) __asm__("FUN_00204790");
void             sound_update(void) __asm__("FUN_0022ca50");
int              f122298_298840(int) __asm__("FUN_00122298");
extern char      D_L00_0017C440[] NOT_SDA;
extern float     D_L00_0015F59C;
void             start_scene(int a) __asm__("FUN_L00_00298840");

void start_scene(int scene_id) {
    O_298840 *moby;
    int       scene_buffer_base, i;
    if (scene_id < 0)
        return;
    if (hero.health.hp == 0) {
        hero_respawn();
        return;
    }
    music_pause(0);
    update_audio_stream_until_idle(0);
    D_L00_0015F5D8 = 1;
    fill_transfer_words(&D_L00_0016C860_lui, 0, 0x1C0);
    fill_transfer_words(D_L00_0017C440, 0, 0x40);
    fill_transfer_words(D_L00_0017C340_lui, 0, 0x40);
    fill_transfer_words(D_L00_0017C380_lui, 0, 0x40);
    fill_transfer_words(D_L00_0017C400_lui, 0, 0x40);
    fill_transfer_words(D_L00_0017C3C0_lui, 0, 0x40);
    scene_buffer_base = D_L00_001611CC - 0x3C000;
    D_L00_0016C860_lui.x30 = scene_id;
    D_L00_0015F59C = 1.0f;
    D_L00_0015F5A0 = 0;
    D_L00_0016C860_lui.x34 = 0;
    D_L00_0016C860_lui.x3c = 0;
    D_L00_001611CC = scene_buffer_base;
    D_L00_0016C860_lui.x58 = D_L00_00173E00_298840.x4 + scene_buffer_base;
    D_L00_0016C860_lui.x5c = D_L00_00173E00_298840.x8 + scene_buffer_base;
    clear_active_help_record();
    fade_level = 1.0f;
    D_L00_0015F5C0_macro[1] = 2;
    D_L00_0015F400 = 0;
    hero_set_state(100, 2);
    hero.unk20A5 = 1;
    f232fe8_298840();
    if (active_scene_trigger.moby)
        active_scene_trigger.moby->flags |= 1;
    if (D_L00_001B9CF0.unkEC)
        ((struct Moby *)D_L00_001B9CF0.unkEC)->flags |= 1;
    start_cutscene_chunk_read(0);
    fade_to_black(scale_game_frames(6));
    update_audio_stream_until_idle(1);
    update_world_object_animation();
    for (moby = D_L00_0015FFDC_298840; moby->st != 0xFF; moby++) {
        if (!(moby->st & 0x80) && (moby->type == 0x4A || moby->type == 0xCB))
            moby->flags |= 0x80;
    }
    music_stream_state.queued_secondary_track = D_L00_0016C860_lui.x30;
    while (music_stream_state.secondary.state != 3) {
        sound_update();
        f122298_298840(0);
    }
    D_L00_0016C860_lui.x48 = -3;
    continue_audio_stream_if_ready();
    start_cutscene_chunk_read(1);
    for (i = 0; D_L00_0016C860_lui.x48 < i; i--)
        f122298_298840(0);
}

typedef struct {
    char pad[0x24];
    struct {
        char          pad[0xC];
        unsigned char n;
        char          pad0d[0x3B];
        int           x48[1];
    } *cls;
} M_298b18;

typedef struct {
    char           pad[0x20];
    unsigned char  st;
    char           pad21[0x13];
    unsigned short flags;
    char           pad36[0x70];
    short          type;
    char           pada8[0x58];
} O_298b18;

typedef struct {
    char           pad[0x34];
    unsigned short flags;
} F_298b18;

typedef struct {
    char      pad[0x10];
    char      v[8];
    float     x18;
    char      pad1c[0x28];
    short     n;
    short     x46;
    char      pad48[0x130];
    M_298b18 *m[1];
} C_298b18;

extern float     D_L00_0016CAF0_lui __asm__("D_L00_0016CAF0") NOT_SDA;
extern O_298b18 *D_L00_0015FFDC_298b18 __asm__("D_L00_0015FFDC");
extern C_298b18  D_L00_0016C860_298b18 __asm__("D_L00_0016C860") NOT_SDA;
void             select_level_vu_chain(void) __asm__("FUN_002335a0");
void             f2330d0_298b18(void) __asm__("FUN_L00_002330d0");
void             set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
void             process_menu_entry(F_298b18 *, void *, int) __asm__("FUN_00216c48");
void             music_set_active_track_value(int) __asm__("FUN_L00_00265880");
void             finish_cutscene(void) __asm__("FUN_L00_00298b18");

void finish_cutscene(void) {
    O_298b18 *moby;
    float     f, hero_candidate_z;
    int       i;
    if ((unsigned short)(music_stream_state.secondary.state - 6) >= 2)
        music_stream_state.secondary.state = 5;
    D_L00_0015F5D8 = 1;
    select_level_vu_chain();
    fade_to_black(0xC);
    D_L00_0015F5C0_macro[1] = 0;
    D_L00_0016CAF0_lui = 0.63f;
    (*(int *)&fade_level) = 0;
    update_view_context();
    for (i = 0; i < D_L00_0016C860_298b18.n; i++) {
        M_298b18 *m = D_L00_0016C860_298b18.m[i];
        if (m) {
            m->cls->n--;
            m->cls->x48[m->cls->n] = 0;
            mark_moby_for_removal(m);
        }
    }
    for (moby = D_L00_0015FFDC_298b18; moby->st != 0xFF; moby++) {
        if (!(moby->st & 0x80) && (moby->type == 0x4A || moby->type == 0xCB))
            moby->flags &= ~0x80;
    }
    hero_set_state(HERO_STATE_IDLE, 1);
    hero_candidate_z = probe_ground_height(&hero.motion.pos, 0, 0.5f);
    if (2.0f < hero_candidate_z && absolute_float(hero.motion.pos.f[2] - hero_candidate_z) < 4.5f)
        hero.motion.pos.f[2] = hero_candidate_z;
    hero.unk20A5 = 0;
    f2330d0_298b18();
    if (D_L00_0016C860_298b18.x46) {
        f = probe_ground_height(D_L00_0016C860_298b18.v, 0, 0.5f);
        if (2.0f < f && absolute_float(D_L00_0016C860_298b18.x18 - f) < 2.0f)
            D_L00_0016C860_298b18.x18 = f;
        set_race_camera(D_L00_0016C860_298b18.v, D_L00_0016C860_298b18.v + 0x10, 0, 1);
    }
    if (active_scene_trigger.moby)
        active_scene_trigger.moby->flags &= ~1;
    if (D_L00_001B9CF0.unkEC) {
        ((struct Moby *)D_L00_001B9CF0.unkEC)->flags &= ~1;
        D_L00_001B9CF0.unkEC = 0;
    }
    if (active_scene_trigger.moby) {
        F_298b18 *scene_trigger_moby = active_scene_trigger.moby;
        void     *scene_trigger = active_scene_trigger.trigger;
        active_scene_trigger.moby = 0;
        active_scene_trigger.trigger = 0;
        process_menu_entry(scene_trigger_moby, scene_trigger, 1);
    }
    music_set_active_track_value(scale_game_frames(0x1E));
}

#include "qcopy.h"
#include "rnc/overlay/quad.h"

typedef struct {
    char  pad[0x38];
    int   i38;
    char  pad2[0x54 - 0x3C];
    char *p54;
} H_298de8;

extern H_298de8      D_L00_0016C860_298de8 __asm__("D_L00_0016C860") NOT_SDA;
extern char          D_L00_0016CA40_lui[] __asm__("D_L00_0016CA40") NOT_SDA;
extern unsigned char D_0015EDB0_298de8 __asm__("D_0015EDB0");
extern unsigned char D_0015EDB0_298de8b __asm__("D_0015EDB0");
extern unsigned char D_0015EDB4_298de8 __asm__("D_0015EDB4");

#include "rnc/overlay/level_camera.h"

extern LevelCamera l00_camera_lui __asm__("D_L00_00166C80") NOT_SDA;
extern void        sce_vu0_unit_matrix(void *) __asm__("FUN_00125218");
extern void        sce_vu0_rot_matrix_x(void *, void *, float) __asm__("FUN_00125360");
extern void        sce_vu0_rot_matrix_y(void *, void *, float) __asm__("FUN_00125408");
extern void        sceVu0RotMatrixZ(void *, void *, float) __asm__("FUN_001252b8");
int                apply_cutscene_camera_key(void) __asm__("FUN_L00_00298de8");

int apply_cutscene_camera_key(void) {
    float   camera_key_rotation_matrix[16] __attribute__((aligned(16)));
    OvlQuad camera_offset[1];
    char   *camera_key = D_L00_0016C860_298de8.p54 + D_L00_0016C860_298de8.i38 * 32;
    float  *camera_key_values = (float *)(camera_key + 0x10);
    int     camera_key_flag = *(unsigned char *)(camera_key + 0xC);
    char   *x = D_L00_0016CA40_lui;
    char   *camera_position;
    char   *camera_state;
    float   k;
    float   unscaled_camera_value = camera_key_values[3];
    *(float *)(x + 0xB0) = unscaled_camera_value;
    if (D_0015EDB0_298de8)
        *(float *)(x + 0xB0) = unscaled_camera_value * D_L00_0015F59C;
    update_view_context();
    camera_position = (char *)&l00_camera_lui.pos;
    qcopy(camera_position, camera_key);
    sce_vu0_unit_matrix(camera_key_rotation_matrix);
    sce_vu0_rot_matrix_x(
        camera_key_rotation_matrix, camera_key_rotation_matrix, camera_key_values[0]);
    sce_vu0_rot_matrix_y(
        camera_key_rotation_matrix, camera_key_rotation_matrix, camera_key_values[1]);
    sceVu0RotMatrixZ(camera_key_rotation_matrix, camera_key_rotation_matrix, camera_key_values[2]);
    camera_state = (char *)&l00_camera_lui;
    *(float *)(camera_state + 0x350) = -camera_key_rotation_matrix[8];
    *(float *)(camera_state + 0x360) = -camera_key_rotation_matrix[0];
    *(float *)(camera_state + 0x370) = camera_key_rotation_matrix[4];
    *(float *)(camera_state + 0x354) = -camera_key_rotation_matrix[9];
    *(float *)(camera_state + 0x364) = -camera_key_rotation_matrix[1];
    *(float *)(camera_state + 0x374) = camera_key_rotation_matrix[5];
    *(float *)(camera_state + 0x358) = -camera_key_rotation_matrix[10];
    *(float *)(camera_state + 0x368) = -camera_key_rotation_matrix[2];
    *(float *)(camera_state + 0x378) = camera_key_rotation_matrix[6];
    if (D_0015EDB4_298de8)
        fast_vec_cross(camera_position + 0x220, camera_position + 0x230, camera_position + 0x210);
    k = D_L00_0015F5A0;
    if (D_0015EDB0_298de8b && k != 0.0f) {
        qcopy(camera_offset, camera_position + 0x210);
        fast_vec_normalize(camera_offset, camera_offset, k);
        fast_vec_sub(camera_position, camera_position, camera_offset);
    }
    return camera_key_flag;
}

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

extern f32            fade_level __asm__("D_L00_0015F3FC");
extern VendorCutscene D_L00_0016C860;
extern s32            D_0015EEA0;
extern s32            decode_mode __asm__("D_0015EED8");
extern void           apply_level_specific_cutscene_moby_effects(void) __asm__("FUN_L00_0023e268");
extern s32            FUN_002168a8(s32);
extern void           FUN_00204790(void);
extern void           update_moby_shadow_range(void *) __asm__("FUN_00213700");
extern void           apply_hero_moby_scale(void *) __asm__("FUN_L00_00234b38");
extern void           update_moby_pulsing_glow_effects(void *) __asm__("FUN_L00_00207430");

/* Per-frame update while the cutscene in D_L00_0016C860 plays: runs the debug-gated subsystem
   updates, fades, advances the frame counters and either finishes the cutscene (end frame
   reached or skipped) or steps each of its mobys between two keyframes. */
void update_cutscene_playback(void) __asm__("FUN_L00_00298f90");

void update_cutscene_playback(void) {
    VendorCutscene *cs;
    VendorMoby     *cutscene_moby;
    s32             done;
    s32             last;
    s32             i;
    s32             id;
    s32             audio_chunk_frame_threshold;
    Vec4           *keys;
    f32             fade;
    f32             weighted_current_key_position[4] __attribute__((aligned(16)));
    f32             weighted_next_key_position[4] __attribute__((aligned(16)));

    if ((g_debug_menu.update_flags & 2) ||
        ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
        reset_frame_counters();
    }
    if (!(g_debug_menu.update_flags & 0x10) || (D_0013CAE4 & 0x220)) {
        apply_level_specific_cutscene_moby_effects();
    }
    if ((g_debug_menu.update_flags & 2) ||
        ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
        update_visible_mobys();
        enqueue_voice_request();
    }
    noop_main_callback(D_L00_00160FF8, 3);
    if ((g_debug_menu.update_flags & 1) ||
        ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
        coordinate_hero_frame_update();
    }
    noop_main_callback(D_L00_00161008, 7);
    if ((g_debug_menu.update_flags & 4) ||
        ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
        update_parts();
    }
    noop_main_callback(D_L00_00161018, 5);
    cs = &D_L00_0016C860;
    fade = fade_level - 0.34f;
    cs->f34++;
    cs->f38++;
    fade_level = fade;
    if (fade_level < 0.0f) {
        fade_level = 0.0f;
    }
    if (cs->f34 >= cs->h48) {
        continue_audio_stream_if_ready();
    }
    done = cs->f34 >= cs->h40;
    if (cs->f34 >= scale_game_frames(0x12) && fade_level == 0.0f) {
        if ((D_0015EEA0 || alternate_animation_sequence || decode_mode ||
             current_level_index <= 0) &&
            (D_0013C940.buttons.w.pressed & 0x800)) {
            done = 1;
        } else if ((D_0013C940.buttons.held_pressed & 0x8000000000FUL) == 0x8000000000FUL) {
            done = 1;
        }
    }
    if (done) {
        finish_cutscene();
    } else {
        audio_chunk_frame_threshold = pal_mode ? 0x50 : 0x60;
        if (D_L00_0016C860.f38 >= audio_chunk_frame_threshold) {
            FUN_002168a8(1);
            D_L00_0016C860.f3C++;
            FUN_00204790();
            start_cutscene_chunk_read(D_L00_0016C860.f3C + 1);
        }
        last = apply_cutscene_camera_key();
        for (i = 0; i < D_L00_0016C860.h44; i++) {
            cutscene_moby = D_L00_0016C860.f178[i];
            id = D_L00_0016C860.f38 >> 1;
            cutscene_moby->frame = id;
            cutscene_moby->next_frame = id + 1;
            update_moby_animation_state(cutscene_moby);
            cutscene_moby->blend = convert_integer_to_float(D_L00_0016C860.f38 & 1) * 0.5f;
            if (last && (D_L00_0016C860.f38 & 1)) {
                cutscene_moby->blend = 1.0f;
            }
            keys = cutscene_moby->keys;
            fast_vec_scale(weighted_current_key_position,
                           &keys[cutscene_moby->frame],
                           1.0f - cutscene_moby->blend);
            fast_vec_scale(
                weighted_next_key_position, &keys[cutscene_moby->next_frame], cutscene_moby->blend);
            fast_vec_add(
                &cutscene_moby->pos, weighted_current_key_position, weighted_next_key_position);
            cutscene_moby->b71 = 0xFF;
            update_entity(cutscene_moby);
            if (cutscene_moby->b7F) {
                update_moby_shadow_range(cutscene_moby);
            }
            if (cutscene_moby->oclass == 0) {
                apply_hero_moby_scale(cutscene_moby);
            }
            if (cutscene_moby->oclass == 10 || cutscene_moby->oclass == 0x1A3 ||
                cutscene_moby->oclass == 0x555) {
                update_moby_pulsing_glow_effects(cutscene_moby);
            }
        }
    }
    noop_main_callback(D_L00_00161028, 8);
    sound_update();
    noop_main_callback(D_L00_00161038, 6);
    update_all_point_lights();
    update_resident_packed_direction();
    if ((g_debug_menu.update_flags & 2) ||
        ((g_debug_menu.update_flags & 0x10) && (D_0013CAE4 & 0x200))) {
        update_game_frame_and_idle_counters();
    }
}

extern s32 D_0015ED80;
void       prepare_movie_playback(s32 sector, s32 size, s32 language) __asm__("FUN_L00_002995d0");

void prepare_movie_playback(s32, s32, s32) __asm__("FUN_L00_002995d0");
void hero_respawn() __asm__("FUN_L00_00211250");
void queue_scene_movie_with_game_language(s32 i) __asm__("FUN_L00_00299460");

void queue_scene_movie_with_game_language(s32 i) {
    s32          a, b;
    struct Hero *hero_ptr;
    if (i < 0)
        return;
    if (D_0015ED80) {
        a = disc_table.unk18A0[i].sector;
        b = disc_table.unk18A0[i].size;
    } else {
        a = disc_table.unk1808[i].sector;
        b = disc_table.unk1808[i].size;
    }
    hero_ptr = &hero;
    if (hero_ptr->health.hp == 0) {
        hero_respawn();
    } else if (hero_ptr->fall_fade_pending == 0) {
        prepare_movie_playback(a, b, game_language);
    }
}

/* Passes disc table entry i at 0x1A28 (0x1A50 while D_0015ED80 is set) to FUN_L00_002995d0. */
void queue_language_zero_movie(s32 i) __asm__("FUN_L00_002994f8");

void queue_language_zero_movie(s32 i) {
    s32 a, b;
    if (i < 0)
        return;
    if (D_0015ED80) {
        a = disc_table.unk1A50[i].sector;
        b = disc_table.unk1A50[i].size;
    } else {
        a = disc_table.unk1A28[i].sector;
        b = disc_table.unk1A28[i].size;
    }
    prepare_movie_playback(a, b, 0);
}

/* Passes movie i (the alt table while D_0015ED80 is set) and the language to FUN_L00_002995d0. */
void queue_movie_with_game_language(s32 i) __asm__("FUN_L00_00299560");

void queue_movie_with_game_language(s32 i) {
    s32 movie_sector, movie_size;
    if (i < 0)
        return;
    if (D_0015ED80) {
        movie_sector = disc_table.movies_alt[i].sector;
        movie_size = disc_table.movies_alt[i].size;
    } else {
        movie_sector = disc_table.movies[i].sector;
        movie_size = disc_table.movies[i].size;
    }
    prepare_movie_playback(movie_sector, movie_size, game_language);
}

extern int D_L00_00161048_macro __asm__("D_L00_00161048") MACRO_ADDR;
extern int D_L00_0016104C_macro __asm__("D_L00_0016104C") MACRO_ADDR;
extern s32 D_L00_00161050_macro __asm__("D_L00_00161050") MACRO_ADDR;
extern s32 D_L00_00161054_macro __asm__("D_L00_00161054") MACRO_ADDR;
extern s32 D_L00_00161058_macro __asm__("D_L00_00161058") MACRO_ADDR;
extern s32 D_L00_0015F40C_macro __asm__("D_L00_0015F40C") MACRO_ADDR;
extern s32 D_L00_0015F5C4_macro __asm__("D_L00_0015F5C4") MACRO_ADDR;
extern s32 D_L00_00173E04_lui __asm__("D_L00_00173E04") NOT_SDA;
/* [0] is gs_texture_allocation_start, [4] depth_buffer_address. */
extern s32  D_0015EE78_macro[6] __asm__("D_0015EE78") MACRO_ADDR;
extern void flush_cache(s32) __asm__("FUN_00118a80");
extern void clear_active_help_record(void) __asm__("FUN_L00_002039a0");
extern s32  sceGsSetDefLoadImage(void *, s16, s16, s16, s16, s16, s16, s16);
extern s32  sceGsExecLoadImage(void *, u8 *);
extern void sceGsSyncPath(s32, s32) __asm__("FUN_00120558");
extern void debug_print(char *, s32) __asm__("FUN_001e93b0");
extern void raise_kernel_trap(void) __asm__("FUN_001f97b0");
extern char mpeg_memory_alloc_error_format[] __asm__("D_L00_001E9480");

/* Sets up a movie load, then uploads image data from the level buffer to GS memory in two runs of strips. */
void prepare_movie_playback(s32 sector, s32 size, s32 language) {
    s32 image[24];
    s32 large_count, small_count, gs_address, source, i, j;

    voice_pool.reverb_commands |= 8;
    flush_cache(0);
    sound_stop_all_sounds();
    music_stop();
    D_L00_00161048_macro = sector;
    D_L00_0016104C_macro = size;
    D_L00_00161050_macro = language;
    D_L00_0015F5D8 = 1;
    fade_to_black(4);
    clear_active_help_record();
    D_L00_0015F5C4_macro = 1;
    D_L00_00161054_macro = D_L00_00173E04_lui;
    D_L00_00161058_macro = D_L00_0015F40C_macro;
    large_count = D_0015ED80 != 0 ? 0xE : 0xD;
    small_count = D_0015ED80 != 0 ? 0xE : 0x11;
    source = D_L00_00161058_macro;
    gs_address = D_0015EE78_macro[4];
    i = 0;
    while (i < large_count) {
        sceGsSetDefLoadImage(image, gs_address >> 8, 2, 1, 0, 0, 0x80, 0x80);
        i++;
        gs_address += 0x10000;
        flush_cache(0);
        sceGsExecLoadImage(image, (u8 *)source);
        source += 0xC000;
        sceGsSyncPath(0, 0);
    }
    gs_address = D_0015EE78_macro[0];
    j = 0;
    while (j < small_count) {
        sceGsSetDefLoadImage(image, gs_address >> 8, 1, 0, 0, 0, 0x40, 0x40);
        j++;
        gs_address += 0x4000;
        flush_cache(0);
        sceGsExecLoadImage(image, (u8 *)source);
        source += 0x4000;
        sceGsSyncPath(0, 0);
    }
    if (gs_address > 0x400000) {
        debug_print(mpeg_memory_alloc_error_format, gs_address - 0x400000);
        raise_kernel_trap();
    }
}

typedef struct {
    u8 pad[0x6B];
    u8 f;
} M_2997c8;

extern struct MusicStreamState   music_stream_state_lui __asm__("D_001516D0") NOT_SDA;
extern s32                       D_L00_0015F5C4_2997c8 __asm__("D_L00_0015F5C4");
extern struct ActiveSceneTrigger active_scene_trigger_lui __asm__("D_L00_00179100") NOT_SDA;
extern char                      voice_pool_lui[] __asm__("D_0013E550") NOT_SDA;
void                             sceGsSyncPath(s32, s32) __asm__("FUN_00120558");
void                             FUN_00122e68_2997c8(void *) __asm__("FUN_00122e68");
void FUN_00122518_2997c8(void *, short, int, int, int, int, int, int) __asm__("FUN_00122518");
void FUN_00118a80_2997c8(int) __asm__("FUN_00118a80");
void FUN_001227d8_2997c8(void *, int) __asm__("FUN_001227d8");
void music_start_track(s32 track, s32 track_flags, s32 volume) __asm__("FUN_00215c40");
void count_vsync(void) __asm__("FUN_0012f1c8");
void movie_exit_to_gameplay(void) __asm__("FUN_L00_002997c8");

void movie_exit_to_gameplay(void) {
    char                       buf[0x70] __attribute__((aligned(16)));
    s32                        large_count, small_count, gs_address, source, i, j;
    struct ActiveSceneTrigger *active;
    D_L00_0015F5D8 = 1;
    f122298_298840(0);
    sceGsSyncPath(0, 0);
    FUN_00122e68_2997c8(count_vsync);
    source = D_L00_00161058_macro;
    gs_address = D_0015EE78_macro[4];
    large_count = D_0015ED80 != 0 ? 0xE : 0xD;
    small_count = D_0015ED80 != 0 ? 0xE : 0x11;
    i = 0;
    while (i < large_count) {
        FUN_00122518_2997c8(buf, gs_address >> 8, 2, 1, 0, 0, 0x80, 0x80);
        i++;
        gs_address += 0x10000;
        flush_cache(0);
        FUN_001227d8_2997c8(buf, source);
        source += 0xC000;
        sceGsSyncPath(0, 0);
    }
    gs_address = D_0015EE78_macro[0];
    j = 0;
    while (j < small_count) {
        FUN_00122518_2997c8(buf, gs_address >> 8, 1, 0, 0, 0, 0x40, 0x40);
        j++;
        gs_address += 0x4000;
        flush_cache(0);
        FUN_001227d8_2997c8(buf, source);
        source += 0x4000;
        sceGsSyncPath(0, 0);
    }
    if (decode_mode == 2) {
        decode_mode = 0;
    }
    fade_to_black(4);
    music_start_track(music_stream_state_lui.primary.track, 1, 0x400);
    D_L00_0015F5C4_2997c8 = 0;
    active = &active_scene_trigger_lui;
    if (active->moby != 0) {
        struct SceneTrigger *trigger = active->trigger;
        struct Moby         *moby = active->moby;
        active->moby = 0;
        active->trigger = 0;
        process_menu_entry(moby, trigger, 1);
    }
    (*(M_2997c8 *)voice_pool_lui).f |= 0x10;
}
/* Stop sound, wait for the loader to go idle, then start the movie load. */
/* Ported from rac1-decomp (src/overlays/shared/tieproc_00299108.c: func_L00_0029AD18), where it is exact; names translated to the US level program. */

extern char D_0013D290[];
extern int  D_L00_00161048;
extern int  D_L00_0016104C;
extern int  D_L00_00161050;
extern int  sceCdSync_alt(int) __asm__("FUN_00120c30");
extern void play_mpeg_movie(int, int, int, int, int) __asm__("FUN_0023a3b8");
extern void movie_exit_to_gameplay(void) __asm__("FUN_L00_002997c8");
extern void FlushCache(s32);

void play_queued_movie(void) __asm__("FUN_L00_002999a8");

void play_queued_movie(void) {
    FlushCache(0);
    sound_stop_all_sounds();
    music_stop();
    snd_stream_safe_cd_sync(0);
    for (;;) {
        char *ld = D_0013D290;
        if (*(int *)(ld + 0xD4) < 3 && *(int *)(ld + 0xDC) < 0) {
            break;
        }
        memcard_update_state();
    }
    play_mpeg_movie(D_L00_00161048,
                    D_L00_0016104C,
                    (D_L00_00161054 + 0x3F) & ~0x3F,
                    (D_L00_00161058 + 0x3F) & ~0x3F,
                    D_L00_00161050);
    sceCdSync_alt(0);
    movie_exit_to_gameplay();
}
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

extern float              fade_level_gp __asm__("D_L00_0015F3FC") MACRO_ADDR;
extern int D_L00_0015F5C4 MACRO_ADDR;
extern int                D_L00_0015F5D8_gp __asm__("D_L00_0015F5D8") MACRO_ADDR;

typedef struct {
    s16  state;
    s16  slide;
    s16  timer;
    s16  frame;
    s32 *dst;
    s32 *src;
} Slideshow;

extern Slideshow          slideshow __asm__("D_L00_0015F620") MACRO_ADDR;
extern int                D_L00_00173E00[];
extern int D_L00_001611CC MACRO_ADDR;
extern int                D_0015ED80_gp __asm__("D_0015ED80") MACRO_ADDR;
extern s32  start_audio_stream_read(s32 dst, s32 sector, s32 sector_count) __asm__("FUN_00216788");
extern void FillTransferWords(void *, s32, s32);

void start_slideshow(void) __asm__("FUN_L00_00299a68");

void start_slideshow(void) {
    int slideshow_buffer_base;
    update_audio_stream_until_idle(0);
    fade_to_black(8);
    D_L00_0015F5D8_gp = 1;
    fade_level_gp = 1.0f;
    D_L00_0015F5C4 = 7;
    FillTransferWords(&slideshow, 0, 16);
    slideshow_buffer_base = D_L00_001611CC - 0xE0000;
    slideshow.dst = (s32 *)(D_L00_00173E00[1] + slideshow_buffer_base);
    slideshow.src = (s32 *)(D_L00_00173E00[2] + slideshow_buffer_base);
    D_L00_001611CC = slideshow_buffer_base;
    if (D_0015ED80_gp != 0) {
        start_audio_stream_read((s32)slideshow.dst,
                                (*(G9 *)&disc_table).alt[slideshow.slide].a,
                                (*(G9 *)&disc_table).alt[slideshow.slide].b);
    } else {
        start_audio_stream_read((s32)slideshow.dst,
                                (*(G9 *)&disc_table).normal[slideshow.slide].a,
                                (*(G9 *)&disc_table).normal[slideshow.slide].b);
    }
    update_audio_stream_until_idle(1);
    slideshow.slide++;
    if (D_0015ED80_gp != 0) {
        start_audio_stream_read((s32)slideshow.src,
                                (*(G9 *)&disc_table).alt[slideshow.slide].a,
                                (*(G9 *)&disc_table).alt[slideshow.slide].b);
    } else {
        start_audio_stream_read((s32)slideshow.src,
                                (*(G9 *)&disc_table).normal[slideshow.slide].a,
                                (*(G9 *)&disc_table).normal[slideshow.slide].b);
    }
}

extern s32 D_L00_0015F3FC;
void       request_audio_stream_break(void) __asm__("FUN_002166e8");
/* Calls 002a09d8(1), fades to black, breaks the audio stream, clears globals, calls 002335a0. */
void end_slideshow(void) __asm__("FUN_L00_00299c00");

void end_slideshow(void) {
    vu1_sync_chain(1);
    fade_to_black(0xC);
    request_audio_stream_break();
    D_L00_0015F5C0_macro[1] = 0;
    D_L00_0015F5D8 = 1;
    D_L00_0015F3FC = 0;
    select_level_vu_chain();
}

extern s16  cd_read_active[] __asm__("D_001516D8");
extern char D_0013D1D0[];
extern s32  slideshow_frame_thresholds[] __asm__("D_L00_00173BF0");
extern void FUN_0022ca50(void);

void update_slideshow(void) __asm__("FUN_L00_00299c48");

void update_slideshow(void) {
    s32  skip_check;
    s32  i;
    s32 *src;
    s32 *dst;
    s32  slide;

    update_audio_stream_until_idle(0);
    fade_level_gp -= 0.05f;
    if (fade_level_gp < 0.0f) {
        fade_level_gp = 0.0f;
    }
    skip_check = 1;
    if ((u8)D_0013D1D0[2] != 'A') {
        skip_check = 0;
    }
    if (slideshow.state == 0) {
        if (slideshow.frame >= slideshow_frame_thresholds[slideshow.slide] - 0x24) {
            if (cd_read_active[0] != 0) {
                update_audio_stream_until_idle(1);
            }
            if (!skip_check && slideshow.slide == 0x27) {
            end_show:
                end_slideshow();
                return;
            }
            if (slideshow.slide >= 0x2B) {
                goto end_show;
            }
            slideshow.timer = -1;
            slideshow.state = 1;
        }
    } else if (slideshow.state == 1) {
        vu1_sync_chain(1);
        if (slideshow.timer < 0x20) {
            src = slideshow.src;
            dst = slideshow.dst;
            src += slideshow.timer;
            dst += slideshow.timer;
            for (i = 0; i < 0x38000; i += 0x20) {
                *dst = *src;
                src += 0x20;
                dst += 0x20;
            }
        } else if (slideshow.timer >= 0x24) {
            if (!skip_check) {
                if (slideshow.slide == 8) {
                    slideshow.slide = 0x11;
                    slideshow.frame = 0x1590;
                }
            } else {
                if (slideshow.slide == 0x11) {
                    slideshow.slide = 0x1B;
                    slideshow.frame = 0x2238;
                }
                if (slideshow.slide == 0x25) {
                    slideshow.slide = 0x27;
                    slideshow.frame = 0x32B4;
                }
            }
            slideshow.state = 0;
            slideshow.timer = -1;
            slideshow.slide++;
            slide = slideshow.slide % 20;
            if (D_0015ED80_gp != 0) {
                start_audio_stream_read((s32)slideshow.src,
                                        (*(G9 *)&disc_table).alt[slide].a,
                                        (*(G9 *)&disc_table).alt[slide].b);
            } else {
                start_audio_stream_read((s32)slideshow.src,
                                        (*(G9 *)&disc_table).normal[slide].a,
                                        (*(G9 *)&disc_table).normal[slide].b);
            }
        }
    }
    slideshow.timer++;
    slideshow.frame++;
    if (D_0013CAE4 & 0x800) {
        end_slideshow();
    }
    FUN_0022ca50();
}

s32 D_L00_001B9DDC[1] __attribute__((section(".data"))) = {0};

s32 D_L00_001CA514[1] __attribute__((section(".data"))) = {0};

s32 D_L01_0017E984[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001BA25C[1] __attribute__((section(".data"))) = {0};

s32 D_L01_001CA994[1] __attribute__((section(".data"))) = {0};

s32 D_L02_0016CE9C[1] __attribute__((section(".data"))) = {0};

s32 D_L02_0017EB04[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001BA3DC[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001CAAD8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001CAAE4[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001CAAE8[1] __attribute__((section(".data"))) = {0};

s32 D_L02_001CAB14[1] __attribute__((section(".data"))) = {0};

s32 D_L03_0017E604[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001B9EDC[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001CA5D8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001CA5E4[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001CA5E8[1] __attribute__((section(".data"))) = {0};

s32 D_L03_001CA614[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001B9F5C[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001CA658[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001CA664[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001CA668[1] __attribute__((section(".data"))) = {0};

s32 D_L04_001CA694[1] __attribute__((section(".data"))) = {0};

s32 D_L05_0017E984[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001BA25C[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CA958[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CA964[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CA968[1] __attribute__((section(".data"))) = {0};

s32 D_L05_001CA994[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001BA55C[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001CAC58[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001CAC64[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001CAC68[1] __attribute__((section(".data"))) = {0};

s32 D_L06_001CAC94[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001B9E5C[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CA558[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CA564[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CA568[1] __attribute__((section(".data"))) = {0};

s32 D_L07_001CA594[1] __attribute__((section(".data"))) = {0};

s32 D_L08_0016D09C[1] __attribute__((section(".data"))) = {0};

s32 D_L08_0017ED04[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001BA5DC[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CACD8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CACE4[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CACE8[1] __attribute__((section(".data"))) = {0};

s32 D_L08_001CAD14[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001B9F5C[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CA658[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CA664[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CA668[1] __attribute__((section(".data"))) = {0};

s32 D_L09_001CA694[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001BA25C[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001CA958[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001CA964[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001CA968[1] __attribute__((section(".data"))) = {0};

s32 D_L10_001CA994[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001CAED8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001CAEE4[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001CAEE8[1] __attribute__((section(".data"))) = {0};

s32 D_L11_001CAF14[1] __attribute__((section(".data"))) = {0};

s32 D_L12_0017E984[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001BA25C[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001CA958[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001CA964[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001CA968[1] __attribute__((section(".data"))) = {0};

s32 D_L12_001CA994[1] __attribute__((section(".data"))) = {0};

s32 D_L13_0016CB9C[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001BA0DC[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001CA7D8[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001CA7E4[1] __attribute__((section(".data"))) = {0};

s32 D_L13_001CA814[1] __attribute__((section(".data"))) = {0};

s32 D_L14_0016CF9C[1] __attribute__((section(".data"))) = {0};

s32 D_L14_0017EC04[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001BA4DC[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001CABD8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001CABE4[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001CABE8[1] __attribute__((section(".data"))) = {0};

s32 D_L14_001CAC14[1] __attribute__((section(".data"))) = {0};

s32 D_L15_0017EB04[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001BA3DC[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001CAAD8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001CAAE4[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001CAAE8[1] __attribute__((section(".data"))) = {0};

s32 D_L15_001CAB14[1] __attribute__((section(".data"))) = {0};

s32 D_L16_0017E984[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001BA25C[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001CA958[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001CA964[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001CA968[1] __attribute__((section(".data"))) = {0};

s32 D_L16_001CA994[1] __attribute__((section(".data"))) = {0};

s32 D_L17_0016D19C[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001CADD8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001CADE4[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001CADE8[1] __attribute__((section(".data"))) = {0};

s32 D_L17_001CAE14[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001CAED8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001CAEE4[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001CAEE8[1] __attribute__((section(".data"))) = {0};

s32 D_L18_001CAF14[1] __attribute__((section(".data"))) = {0};

int      D_L01_0015F2F0 = 0;
int      D_L01_0015F2F4 = 0;
int      D_L01_0015F424 = 0;
int      D_L01_0015F428 = 0;
int      D_L01_0015F42C = 0;
int      D_L01_0015F430 = 0;
int      D_L01_0015F434 = 0;
int      D_L01_00160FF0 = 0;
int      D_L01_0015F598 = 0;
S_297f78 D_L01_0016C4D8 = {0};

s32                       D_L01_0015F5C8 = 0;
s32 D_L01_001BA260        NOT_SDA = 0;
s32 D_L01_00184694        NOT_SDA = 0;
u8                        D_L01_00160FF8 = 0x6d;
int                       D_L01_0015F5A0 = 0;
O_298840                 *D_L01_0015FFDC = 0;
float                     D_L01_0015F59C = 1.0f;
float D_L01_0016CF70      NOT_SDA = 0;
int                       D_L01_00161048 = 0;
int                       D_L01_0016104C = 0;
s32                       D_L01_00161050 = 0;
s32                       D_L01_00161054 = 0;
s32                       D_L01_00161058 = 0;
s32                       D_L01_0015F40C = 0;
s32 D_L01_00174284        NOT_SDA = 0;
struct ActiveSceneTrigger D_L01_00179580 = {0};

Slideshow D_L01_0015F620 = {0};

unsigned char D_L01_0019804C NOT_SDA = 0;

VendorSlot D_L01_00166300[4] __attribute__((section(".bss")));

s32 D_L01_00162298 __attribute__((section(".bss")));

s32 D_L01_001622A0 __attribute__((section(".bss")));

s32 D_L01_001622A4 __attribute__((section(".bss")));

s32 D_L01_001622B0 __attribute__((section(".bss")));

s32 D_L01_001622B4 __attribute__((section(".bss")));

Vec4 D_L01_001806E0 = {0};

s32  D_L01_00161188 = 0;
s32  D_L01_0016118C = 0;
f32  D_L01_00161060 = 3.8f;
Vec4 D_L01_001806D0 = {0};

f32 D_L01_001610E0 = 0.9f;
int D_L01_00161070 = 0x40600000;
s32 D_L01_001610F0 = 0x40800000;
int D_L01_0015F3F8 = 0;
int D_L01_00161194 = 0;

int D_L01_001622B8 __attribute__((section(".bss")));

float D_L01_001C8DA0 NOT_SDA = 0;
float D_L01_001C8D80 NOT_SDA = 0;
int                  D_L01_00161090 = 0;
VendorSound          D_L01_001C2588 = {0};

Vec4 D_L01_00166400[3] __attribute__((section(".bss")));

void *D_L01_00197780 NOT_SDA = 0;

s32 D_L01_0016228C __attribute__((section(".bss")));

s32 D_L01_00162290 __attribute__((section(".bss")));

int      D_L02_0015F2F0 = 0;
int      D_L02_0015F2F4 = 0;
int      D_L02_0015F424 = 0;
int      D_L02_0015F428 = 0;
int      D_L02_0015F42C = 0;
int      D_L02_0015F430 = 0;
int      D_L02_0015F434 = 0;
int      D_L02_00160FF0 = 0;
int      D_L02_0015F598 = 0;
S_297f78 D_L02_0016C658 = {0};

s32                       D_L02_0015F5C8 = 0;
s32 D_L02_001BA3E0        NOT_SDA = 0;
s32 D_L02_00184814        NOT_SDA = 0;
u8                        D_L02_00160FF8 = 0x6d;
int                       D_L02_0015F5D8 = 0;
int                       D_L02_0015F5A0 = 0;
int                       D_L02_0015F400 = 0;
float                     D_L02_0015F59C = 1.0f;
float D_L02_0016D0F0      NOT_SDA = 0;
int                       D_L02_00161048 = 0;
int                       D_L02_0016104C = 0;
s32                       D_L02_00161050 = 0;
s32                       D_L02_00161054 = 0;
s32                       D_L02_00161058 = 0;
s32                       D_L02_0015F40C = 0;
s32 D_L02_00174404        NOT_SDA = 0;
struct ActiveSceneTrigger D_L02_00179700 = {0};

Slideshow D_L02_0015F620 = {0};

int D_L02_00174400           NOT_SDA = 0;
int                          D_L02_001611CC = 0;
unsigned char D_L02_001981CC NOT_SDA = 0;
VendorList                   D_L02_001CAAC0 = {0};

VendorSlot D_L02_00166480[4] __attribute__((section(".bss")));

s32 D_L02_00162418 __attribute__((section(".bss")));

s32 D_L02_00162420 __attribute__((section(".bss")));

s32 D_L02_00162424 __attribute__((section(".bss")));

s32 D_L02_00162430 __attribute__((section(".bss")));

s32 D_L02_00162434 __attribute__((section(".bss")));

Vec4 D_L02_00180860 = {0};

s32  D_L02_00161188 = 0;
s32  D_L02_0016118C = 0;
f32  D_L02_00161060 = 3.8f;
Vec4 D_L02_00180850 = {0};

f32 D_L02_001610E0 = 0.9f;
int D_L02_00161070 = 0x40600000;
s32 D_L02_00161100 = 0;
s32 D_L02_001610F0 = 0x40800000;
int D_L02_0015F3F8 = 0;
int D_L02_00161194 = 0;

int D_L02_00162438 __attribute__((section(".bss")));

float D_L02_001C8F20 NOT_SDA = 0;
float D_L02_001C8F00 NOT_SDA = 0;
int                  D_L02_00161090 = 0;
VendorSound          D_L02_001C2708 = {0};

Vec4 D_L02_00166580[3] __attribute__((section(".bss")));

void *D_L02_00197900 NOT_SDA = 0;

s32 D_L02_0016240C __attribute__((section(".bss")));

s32 D_L02_00162410 __attribute__((section(".bss")));

int      D_L03_0015F2F0 = 0;
int      D_L03_0015F2F4 = 0;
int      D_L03_0015F424 = 0;
int      D_L03_0015F428 = 0;
int      D_L03_0015F42C = 0;
int      D_L03_0015F430 = 0;
int      D_L03_0015F434 = 0;
int      D_L03_00160FF0 = 0;
int      D_L03_0015F598 = 0;
S_297f78 D_L03_0016C158 = {0};

s32                       D_L03_0015F5C8 = 0;
s32 D_L03_001B9EE0        NOT_SDA = 0;
s32 D_L03_00184314        NOT_SDA = 0;
u8                        D_L03_00160FF8 = 0x6d;
int                       D_L03_0015F5D8 = 0;
float                     D_L03_0015F3FC = 0;
int                       D_L03_0015F400 = 0;
O_298840                 *D_L03_0015FFDC = 0;
float                     D_L03_0015F59C = 1.0f;
float D_L03_0016CBF0      NOT_SDA = 0;
int                       D_L03_00161048 = 0;
int                       D_L03_0016104C = 0;
s32                       D_L03_00161050 = 0;
s32                       D_L03_00161054 = 0;
s32                       D_L03_00161058 = 0;
s32                       D_L03_0015F40C = 0;
s32 D_L03_00173F04        NOT_SDA = 0;
struct ActiveSceneTrigger D_L03_00179200 = {0};

Slideshow D_L03_0015F620 = {0};

int D_L03_00173F00           NOT_SDA = 0;
int                          D_L03_001611CC = 0;
unsigned char D_L03_00197CCC NOT_SDA = 0;
VendorList                   D_L03_001CA5C0 = {0};

VendorSlot D_L03_00165F80[4] __attribute__((section(".bss")));

s32 D_L03_00161F18 __attribute__((section(".bss")));

s32 D_L03_00161F20 __attribute__((section(".bss")));

s32 D_L03_00161F24 __attribute__((section(".bss")));

s32 D_L03_00161F30 __attribute__((section(".bss")));

s32 D_L03_00161F34 __attribute__((section(".bss")));

Vec4 D_L03_00180360 = {0};

s32  D_L03_00161188 = 0;
s32  D_L03_0016118C = 0;
f32  D_L03_00161060 = 3.8f;
Vec4 D_L03_00180350 = {0};

f32 D_L03_001610E0 = 0.9f;
int D_L03_00161070 = 0x40600000;
s32 D_L03_00161100 = 0;
s32 D_L03_001610F0 = 0x40800000;
int D_L03_0015F3F8 = 0;
int D_L03_00161194 = 0;

int D_L03_00161F38 __attribute__((section(".bss")));

float D_L03_001C8A20 NOT_SDA = 0;
float D_L03_001C8A00 NOT_SDA = 0;
int                  D_L03_00161090 = 0;
VendorSound          D_L03_001C2208 = {0};

Vec4 D_L03_00166080[3] __attribute__((section(".bss")));

void *D_L03_00197400 NOT_SDA = 0;

s32 D_L03_00161F0C __attribute__((section(".bss")));

s32 D_L03_00161F10 __attribute__((section(".bss")));

int                D_L04_0015F2F0 = 0;
int                D_L04_0015F2F4 = 0;
int                D_L04_0015F424 = 0;
int                D_L04_0015F428 = 0;
int                D_L04_0015F42C = 0;
int                D_L04_0015F430 = 0;
int                D_L04_0015F434 = 0;
int D_L04_0017E684 NOT_SDA = 0;
int                D_L04_00160FF0 = 0;
int                D_L04_0015F598 = 0;
S_297f78           D_L04_0016C1D8 = {0};

s32                       D_L04_0015F5C8 = 0;
s32 D_L04_001B9F60        NOT_SDA = 0;
s32 D_L04_00184394        NOT_SDA = 0;
u8                        D_L04_00160FF8 = 0x6d;
int                       D_L04_0015F5D8 = 0;
int                       D_L04_0015F5A0 = 0;
float                     D_L04_0015F3FC = 0;
int                       D_L04_0015F400 = 0;
O_298840                 *D_L04_0015FFDC = 0;
char D_L04_0017C5C0       NOT_SDA = 0;
float                     D_L04_0015F59C = 1.0f;
float D_L04_0016CC70      NOT_SDA = 0;
int                       D_L04_00161048 = 0;
int                       D_L04_0016104C = 0;
s32                       D_L04_00161050 = 0;
s32                       D_L04_00161054 = 0;
s32                       D_L04_00161058 = 0;
s32                       D_L04_0015F40C = 0;
s32 D_L04_00173F84        NOT_SDA = 0;
struct ActiveSceneTrigger D_L04_00179280 = {0};

Slideshow D_L04_0015F620 = {0};

int D_L04_00173F80           NOT_SDA = 0;
int                          D_L04_001611CC = 0;
unsigned char D_L04_00197D4C NOT_SDA = 0;
VendorList                   D_L04_001CA640 = {0};

VendorSlot D_L04_00166000[4] __attribute__((section(".bss")));

s32 D_L04_00161F98 __attribute__((section(".bss")));

s32 D_L04_00161FA0 __attribute__((section(".bss")));

s32 D_L04_00161FA4 __attribute__((section(".bss")));

s32 D_L04_00161FB0 __attribute__((section(".bss")));

s32 D_L04_00161FB4 __attribute__((section(".bss")));

Vec4 D_L04_001803E0 = {0};

s32  D_L04_00161188 = 0;
s32  D_L04_0016118C = 0;
f32  D_L04_00161060 = 3.8f;
Vec4 D_L04_001803D0 = {0};

f32 D_L04_001610E0 = 0.9f;
int D_L04_00161070 = 0x40600000;
s32 D_L04_00161100 = 0;
s32 D_L04_001610F0 = 0x40800000;
int D_L04_0015F3F8 = 0;
int D_L04_00161194 = 0;

int D_L04_00161FB8 __attribute__((section(".bss")));

float D_L04_001C8AA0 NOT_SDA = 0;
float D_L04_001C8A80 NOT_SDA = 0;
int                  D_L04_00161090 = 0;
VendorSound          D_L04_001C2288 = {0};

Vec4 D_L04_00166100[3] __attribute__((section(".bss")));

s32 D_L04_00161F8C __attribute__((section(".bss")));

s32 D_L04_00161F90 __attribute__((section(".bss")));

int      D_L05_0015F2F0 = 0;
int      D_L05_0015F2F4 = 0;
int      D_L05_0015F424 = 0;
int      D_L05_0015F428 = 0;
int      D_L05_0015F42C = 0;
int      D_L05_0015F430 = 0;
int      D_L05_0015F434 = 0;
int      D_L05_00160FF0 = 0;
int      D_L05_0015F598 = 0;
S_297f78 D_L05_0016C458 = {0};

s32                       D_L05_0015F5C8 = 0;
s32 D_L05_001BA260        NOT_SDA = 0;
s32 D_L05_00184694        NOT_SDA = 0;
u8                        D_L05_00160FF8 = 0x6d;
int                       D_L05_0015F5D8 = 0;
int                       D_L05_0015F5A0 = 0;
int                       D_L05_0015F400 = 0;
O_298840                 *D_L05_0015FFDC = 0;
float                     D_L05_0015F59C = 1.0f;
float D_L05_0016CEF0      NOT_SDA = 0;
int                       D_L05_00161048 = 0;
int                       D_L05_0016104C = 0;
s32                       D_L05_00161050 = 0;
s32                       D_L05_00161054 = 0;
s32                       D_L05_00161058 = 0;
s32                       D_L05_0015F40C = 0;
s32 D_L05_00174204        NOT_SDA = 0;
struct ActiveSceneTrigger D_L05_00179500 = {0};

Slideshow D_L05_0015F620 = {0};

int D_L05_00174200           NOT_SDA = 0;
int                          D_L05_001611CC = 0;
unsigned char D_L05_0019804C NOT_SDA = 0;
VendorList                   D_L05_001CA940 = {0};

VendorSlot D_L05_00166280[4] __attribute__((section(".bss")));

s32 D_L05_00162218 __attribute__((section(".bss")));

s32 D_L05_00162220 __attribute__((section(".bss")));

s32 D_L05_00162224 __attribute__((section(".bss")));

s32 D_L05_00162230 __attribute__((section(".bss")));

s32 D_L05_00162234 __attribute__((section(".bss")));

Vec4 D_L05_001806E0 = {0};

s32  D_L05_00161188 = 0;
s32  D_L05_0016118C = 0;
f32  D_L05_00161060 = 3.8f;
Vec4 D_L05_001806D0 = {0};

f32 D_L05_001610E0 = 0.9f;
int D_L05_00161070 = 0x40600000;
s32 D_L05_00161100 = 0;
s32 D_L05_001610F0 = 0x40800000;
int D_L05_0015F3F8 = 0;
int D_L05_00161194 = 0;

int D_L05_00162238 __attribute__((section(".bss")));

float D_L05_001C8DA0 NOT_SDA = 0;
float D_L05_001C8D80 NOT_SDA = 0;
int                  D_L05_00161090 = 0;
VendorSound          D_L05_001C2588 = {0};

Vec4 D_L05_00166380[3] __attribute__((section(".bss")));

void *D_L05_00197780 NOT_SDA = 0;

s32 D_L05_0016220C __attribute__((section(".bss")));

s32 D_L05_00162210 __attribute__((section(".bss")));

int                D_L06_0015F2F0 = 0;
int                D_L06_0015F2F4 = 0;
int                D_L06_0015F424 = 0;
int                D_L06_0015F428 = 0;
int                D_L06_0015F42C = 0;
int                D_L06_0015F430 = 0;
int                D_L06_0015F434 = 0;
int D_L06_0017EC84 NOT_SDA = 0;
int                D_L06_00160FF0 = 0;
int                D_L06_0015F598 = 0;
S_297f78           D_L06_0016C7D8 = {0};

s32                  D_L06_0015F5C8 = 0;
s32 D_L06_001BA560   NOT_SDA = 0;
s32 D_L06_00184994   NOT_SDA = 0;
u8                   D_L06_00160FF8 = 0x6d;
int                  D_L06_0015F5D8 = 0;
int                  D_L06_0015F5A0 = 0;
O_298840            *D_L06_0015FFDC = 0;
char D_L06_0017CBC0  NOT_SDA = 0;
float                D_L06_0015F59C = 1.0f;
float D_L06_0016D270 NOT_SDA = 0;
int                  D_L06_00161048 = 0;
int                  D_L06_0016104C = 0;
s32                  D_L06_00161050 = 0;
s32                  D_L06_00161054 = 0;
s32                  D_L06_00161058 = 0;
s32 D_L06_00174584   NOT_SDA = 0;
Slideshow            D_L06_0015F620 = {0};

int                          D_L06_001611CC = 0;
unsigned char D_L06_0019834C NOT_SDA = 0;
VendorList                   D_L06_001CAC40 = {0};

VendorSlot D_L06_00166600[4] __attribute__((section(".bss")));

s32 D_L06_00162598 __attribute__((section(".bss")));

s32 D_L06_001625A0 __attribute__((section(".bss")));

s32 D_L06_001625A4 __attribute__((section(".bss")));

s32 D_L06_001625B0 __attribute__((section(".bss")));

s32 D_L06_001625B4 __attribute__((section(".bss")));

Vec4 D_L06_001809E0 = {0};

s32  D_L06_00161188 = 0;
s32  D_L06_0016118C = 0;
f32  D_L06_00161060 = 3.8f;
Vec4 D_L06_001809D0 = {0};

f32 D_L06_001610E0 = 0.9f;
int D_L06_00161070 = 0x40600000;
s32 D_L06_00161100 = 0;
s32 D_L06_001610F0 = 0x40800000;
int D_L06_0015F3F8 = 0;
int D_L06_00161194 = 0;

int D_L06_001625B8 __attribute__((section(".bss")));

float D_L06_001C90A0 NOT_SDA = 0;
float D_L06_001C9080 NOT_SDA = 0;
int                  D_L06_00161090 = 0;
VendorSound          D_L06_001C2888 = {0};

Vec4 D_L06_00166700[3] __attribute__((section(".bss")));

s32 D_L06_0016258C __attribute__((section(".bss")));

s32 D_L06_00162590 __attribute__((section(".bss")));

int                D_L07_0015F2F0 = 0;
int                D_L07_0015F2F4 = 0;
int                D_L07_0015F424 = 0;
int                D_L07_0015F428 = 0;
int                D_L07_0015F42C = 0;
int                D_L07_0015F430 = 0;
int                D_L07_0015F434 = 0;
int D_L07_0017E584 NOT_SDA = 0;
int                D_L07_00160FF0 = 0;
int                D_L07_0015F598 = 0;
S_297f78           D_L07_0016C0D8 = {0};

s32                  D_L07_0015F5C8 = 0;
s32 D_L07_001B9E60   NOT_SDA = 0;
s32 D_L07_00184294   NOT_SDA = 0;
u8                   D_L07_00160FF8 = 0x6d;
int                  D_L07_0015F5D8 = 0;
int                  D_L07_0015F5A0 = 0;
float                D_L07_0015F3FC = 0;
int                  D_L07_0015F400 = 0;
O_298840            *D_L07_0015FFDC = 0;
char D_L07_0017C4C0  NOT_SDA = 0;
float                D_L07_0015F59C = 1.0f;
float D_L07_0016CB70 NOT_SDA = 0;
int                  D_L07_00161048 = 0;
int                  D_L07_0016104C = 0;
s32                  D_L07_00161050 = 0;
s32                  D_L07_00161054 = 0;
s32                  D_L07_00161058 = 0;
s32 D_L07_00173E84   NOT_SDA = 0;
Slideshow            D_L07_0015F620 = {0};

int                          D_L07_001611CC = 0;
unsigned char D_L07_00197C4C NOT_SDA = 0;
VendorList                   D_L07_001CA540 = {0};

VendorSlot D_L07_00165F00[4] __attribute__((section(".bss")));

s32 D_L07_00161E98 __attribute__((section(".bss")));

s32 D_L07_00161EA0 __attribute__((section(".bss")));

s32 D_L07_00161EA4 __attribute__((section(".bss")));

s32 D_L07_00161EB0 __attribute__((section(".bss")));

s32 D_L07_00161EB4 __attribute__((section(".bss")));

Vec4 D_L07_001802E0 = {0};

s32  D_L07_00161188 = 0;
s32  D_L07_0016118C = 0;
f32  D_L07_00161060 = 3.8f;
Vec4 D_L07_001802D0 = {0};

f32 D_L07_001610E0 = 0.9f;
int D_L07_00161070 = 0x40600000;
s32 D_L07_00161100 = 0;
s32 D_L07_001610F0 = 0x40800000;
int D_L07_0015F3F8 = 0;
int D_L07_00161194 = 0;

int D_L07_00161EB8 __attribute__((section(".bss")));

float D_L07_001C89A0 NOT_SDA = 0;
float D_L07_001C8980 NOT_SDA = 0;
int                  D_L07_00161090 = 0;
VendorSound          D_L07_001C2188 = {0};

Vec4 D_L07_00166000[3] __attribute__((section(".bss")));

s32 D_L07_00161E8C __attribute__((section(".bss")));

s32 D_L07_00161E90 __attribute__((section(".bss")));

int      D_L08_0015F2F0 = 0;
int      D_L08_0015F2F4 = 0;
int      D_L08_0015F424 = 0;
int      D_L08_0015F428 = 0;
int      D_L08_0015F42C = 0;
int      D_L08_0015F430 = 0;
int      D_L08_0015F434 = 0;
int      D_L08_00160FF0 = 0;
int      D_L08_0015F598 = 0;
S_297f78 D_L08_0016C858 = {0};

s32                       D_L08_0015F5C8 = 0;
s32 D_L08_001BA5E0        NOT_SDA = 0;
s32 D_L08_00184A14        NOT_SDA = 0;
u8                        D_L08_00160FF8 = 0x6d;
int                       D_L08_0015F5D8 = 0;
float                     D_L08_0015F3FC = 0;
int                       D_L08_0015F400 = 0;
float D_L08_0016D2F0      NOT_SDA = 0;
int                       D_L08_00161048 = 0;
int                       D_L08_0016104C = 0;
s32                       D_L08_00161050 = 0;
s32                       D_L08_00161054 = 0;
s32                       D_L08_00161058 = 0;
s32                       D_L08_0015F40C = 0;
s32 D_L08_00174604        NOT_SDA = 0;
struct ActiveSceneTrigger D_L08_00179900 = {0};

Slideshow D_L08_0015F620 = {0};

int D_L08_00174600           NOT_SDA = 0;
int                          D_L08_001611CC = 0;
unsigned char D_L08_001983CC NOT_SDA = 0;
VendorList                   D_L08_001CACC0 = {0};

VendorSlot D_L08_00166680[4] __attribute__((section(".bss")));

s32 D_L08_00162618 __attribute__((section(".bss")));

s32 D_L08_00162620 __attribute__((section(".bss")));

s32 D_L08_00162624 __attribute__((section(".bss")));

s32 D_L08_00162630 __attribute__((section(".bss")));

s32 D_L08_00162634 __attribute__((section(".bss")));

Vec4 D_L08_00180A60 = {0};

s32  D_L08_00161188 = 0;
s32  D_L08_0016118C = 0;
f32  D_L08_00161060 = 3.8f;
Vec4 D_L08_00180A50 = {0};

f32 D_L08_001610E0 = 0.9f;
int D_L08_00161070 = 0x40600000;
s32 D_L08_00161100 = 0;
s32 D_L08_001610F0 = 0x40800000;
int D_L08_00161194 = 0;

int D_L08_00162638 __attribute__((section(".bss")));

float D_L08_001C9120 NOT_SDA = 0;
float D_L08_001C9100 NOT_SDA = 0;
int                  D_L08_00161090 = 0;
VendorSound          D_L08_001C2908 = {0};

Vec4 D_L08_00166780[3] __attribute__((section(".bss")));

void *D_L08_00197B00 NOT_SDA = 0;

s32 D_L08_0016260C __attribute__((section(".bss")));

s32 D_L08_00162610 __attribute__((section(".bss")));

int                D_L09_0015F2F0 = 0;
int                D_L09_0015F2F4 = 0;
int                D_L09_0015F424 = 0;
int                D_L09_0015F428 = 0;
int                D_L09_0015F42C = 0;
int                D_L09_0015F430 = 0;
int                D_L09_0015F434 = 0;
int D_L09_0017E684 NOT_SDA = 0;
int                D_L09_00160FF0 = 0;
int                D_L09_0015F598 = 0;
S_297f78           D_L09_0016C1D8 = {0};

s32                D_L09_0015F5C8 = 0;
s32 D_L09_001B9F60 NOT_SDA = 0;
s32 D_L09_00184394 NOT_SDA = 0;
u8                 D_L09_00160FF8 = 0x6d;
C_298840           D_L09_0016C9E0 = {0};

int                       D_L09_0015F5D8 = 0;
int                       D_L09_0015F5A0 = 0;
float                     D_L09_0015F3FC = 0;
int                       D_L09_0015F400 = 0;
O_298840                 *D_L09_0015FFDC = 0;
char D_L09_0017C5C0       NOT_SDA = 0;
float                     D_L09_0015F59C = 1.0f;
float D_L09_0016CC70      NOT_SDA = 0;
int                       D_L09_00161048 = 0;
int                       D_L09_0016104C = 0;
s32                       D_L09_00161050 = 0;
s32                       D_L09_00161054 = 0;
s32                       D_L09_00161058 = 0;
s32                       D_L09_0015F40C = 0;
s32 D_L09_00173F84        NOT_SDA = 0;
struct ActiveSceneTrigger D_L09_00179280 = {0};

Slideshow D_L09_0015F620 = {0};

int D_L09_00173F80           NOT_SDA = 0;
int                          D_L09_001611CC = 0;
unsigned char D_L09_00197D4C NOT_SDA = 0;
VendorList                   D_L09_001CA640 = {0};

VendorSlot D_L09_00166000[4] __attribute__((section(".bss")));

s32 D_L09_00161F98 __attribute__((section(".bss")));

s32 D_L09_00161FA0 __attribute__((section(".bss")));

s32 D_L09_00161FA4 __attribute__((section(".bss")));

s32 D_L09_00161FB0 __attribute__((section(".bss")));

s32 D_L09_00161FB4 __attribute__((section(".bss")));

Vec4 D_L09_001803E0 = {0};

s32  D_L09_00161188 = 0;
s32  D_L09_0016118C = 0;
f32  D_L09_00161060 = 3.8f;
Vec4 D_L09_001803D0 = {0};

f32 D_L09_001610E0 = 0.9f;
int D_L09_00161070 = 0x40600000;
s32 D_L09_00161100 = 0;
s32 D_L09_001610F0 = 0x40800000;
int D_L09_0015F3F8 = 0;
int D_L09_00161194 = 0;

int D_L09_00161FB8 __attribute__((section(".bss")));

float D_L09_001C8AA0 NOT_SDA = 0;
float D_L09_001C8A80 NOT_SDA = 0;
int                  D_L09_00161090 = 0;
VendorSound          D_L09_001C2288 = {0};

Vec4 D_L09_00166100[3] __attribute__((section(".bss")));

void *D_L09_00197480 NOT_SDA = 0;

s32 D_L09_00161F8C __attribute__((section(".bss")));

s32 D_L09_00161F90 __attribute__((section(".bss")));

int                D_L10_0015F2F0 = 0;
int                D_L10_0015F2F4 = 0;
int                D_L10_0015F424 = 0;
int                D_L10_0015F428 = 0;
int                D_L10_0015F42C = 0;
int                D_L10_0015F430 = 0;
int                D_L10_0015F434 = 0;
int D_L10_0017E984 NOT_SDA = 0;
int                D_L10_00160FF0 = 0;
int                D_L10_0015F598 = 0;
S_297f78           D_L10_0016C4D8 = {0};

s32                       D_L10_0015F5C8 = 0;
s32 D_L10_001BA260        NOT_SDA = 0;
s32 D_L10_00184694        NOT_SDA = 0;
u8                        D_L10_00160FF8 = 0x6d;
int                       D_L10_0015F5D8 = 0;
int                       D_L10_0015F5A0 = 0;
int                       D_L10_0015F400 = 0;
O_298840                 *D_L10_0015FFDC = 0;
char D_L10_0017C8C0       NOT_SDA = 0;
float                     D_L10_0015F59C = 1.0f;
float D_L10_0016CF70      NOT_SDA = 0;
int                       D_L10_00161048 = 0;
int                       D_L10_0016104C = 0;
s32                       D_L10_00161050 = 0;
s32                       D_L10_00161054 = 0;
s32                       D_L10_00161058 = 0;
s32                       D_L10_0015F40C = 0;
s32 D_L10_00174284        NOT_SDA = 0;
struct ActiveSceneTrigger D_L10_00179580 = {0};

Slideshow D_L10_0015F620 = {0};

int D_L10_00174280           NOT_SDA = 0;
int                          D_L10_001611CC = 0;
unsigned char D_L10_0019804C NOT_SDA = 0;
VendorList                   D_L10_001CA940 = {0};

VendorSlot D_L10_00166300[4] __attribute__((section(".bss")));

s32 D_L10_00162298 __attribute__((section(".bss")));

s32 D_L10_001622A0 __attribute__((section(".bss")));

s32 D_L10_001622A4 __attribute__((section(".bss")));

s32 D_L10_001622B0 __attribute__((section(".bss")));

s32 D_L10_001622B4 __attribute__((section(".bss")));

Vec4 D_L10_001806E0 = {0};

s32  D_L10_00161188 = 0;
s32  D_L10_0016118C = 0;
f32  D_L10_00161060 = 3.8f;
Vec4 D_L10_001806D0 = {0};

f32 D_L10_001610E0 = 0.9f;
int D_L10_00161070 = 0x40600000;
s32 D_L10_00161100 = 0;
s32 D_L10_001610F0 = 0x40800000;
int D_L10_00161194 = 0;

int D_L10_001622B8 __attribute__((section(".bss")));

float D_L10_001C8DA0 NOT_SDA = 0;
float D_L10_001C8D80 NOT_SDA = 0;
int                  D_L10_00161090 = 0;
VendorSound          D_L10_001C2588 = {0};

Vec4 D_L10_00166400[3] __attribute__((section(".bss")));

void *D_L10_00197780 NOT_SDA = 0;

s32 D_L10_0016228C __attribute__((section(".bss")));

s32 D_L10_00162290 __attribute__((section(".bss")));

int                D_L11_0015F2F0 = 0;
int                D_L11_0015F2F4 = 0;
int                D_L11_0015F424 = 0;
int                D_L11_0015F428 = 0;
int                D_L11_0015F42C = 0;
int                D_L11_0015F430 = 0;
int                D_L11_0015F434 = 0;
int D_L11_0017EF04 NOT_SDA = 0;
int                D_L11_00160FF0 = 0;
int                D_L11_0015F598 = 0;
S_297f78           D_L11_0016CA58 = {0};

s32                       D_L11_0015F5C8 = 0;
s32 D_L11_001BA7E0        NOT_SDA = 0;
s32 D_L11_00184C14        NOT_SDA = 0;
u8                        D_L11_00160FF8 = 0x6d;
int                       D_L11_0015F5D8 = 0;
int                       D_L11_0015F5A0 = 0;
int                       D_L11_0015F400 = 0;
O_298840                 *D_L11_0015FFDC = 0;
char D_L11_0017CE40       NOT_SDA = 0;
float                     D_L11_0015F59C = 1.0f;
int                       D_L11_00161048 = 0;
int                       D_L11_0016104C = 0;
s32                       D_L11_00161050 = 0;
s32                       D_L11_00161054 = 0;
s32                       D_L11_00161058 = 0;
s32                       D_L11_0015F40C = 0;
s32 D_L11_00174804        NOT_SDA = 0;
struct ActiveSceneTrigger D_L11_00179B00 = {0};

Slideshow D_L11_0015F620 = {0};

int D_L11_00174800           NOT_SDA = 0;
int                          D_L11_001611CC = 0;
unsigned char D_L11_001985CC NOT_SDA = 0;
VendorList                   D_L11_001CAEC0 = {0};

VendorSlot D_L11_001668C0[4] __attribute__((section(".bss")));

s32 D_L11_00162818 __attribute__((section(".bss")));

s32 D_L11_00162820 __attribute__((section(".bss")));

s32 D_L11_00162824 __attribute__((section(".bss")));

s32 D_L11_00162830 __attribute__((section(".bss")));

s32 D_L11_00162834 __attribute__((section(".bss")));

Vec4 D_L11_00180C60 = {0};

s32  D_L11_00161188 = 0;
s32  D_L11_0016118C = 0;
f32  D_L11_00161060 = 3.8f;
Vec4 D_L11_00180C50 = {0};

f32 D_L11_001610E0 = 0.9f;
int D_L11_00161070 = 0x40600000;
s32 D_L11_00161100 = 0;
s32 D_L11_001610F0 = 0x40800000;
int D_L11_0015F3F8 = 0;
int D_L11_00161194 = 0;

int D_L11_00162838 __attribute__((section(".bss")));

float D_L11_001C9320 NOT_SDA = 0;
float D_L11_001C9300 NOT_SDA = 0;
int                  D_L11_00161090 = 0;
VendorSound          D_L11_001C2B08 = {0};

Vec4 D_L11_001669C0[3] __attribute__((section(".bss")));

void *D_L11_00197D00 NOT_SDA = 0;

s32 D_L11_0016280C __attribute__((section(".bss")));

s32 D_L11_00162810 __attribute__((section(".bss")));

int      D_L12_0015F2F0 = 0;
int      D_L12_0015F2F4 = 0;
int      D_L12_0015F424 = 0;
int      D_L12_0015F428 = 0;
int      D_L12_0015F42C = 0;
int      D_L12_0015F430 = 0;
int      D_L12_0015F434 = 0;
int      D_L12_00160FF0 = 0;
int      D_L12_0015F598 = 0;
S_297f78 D_L12_0016C4D8 = {0};

s32                       D_L12_0015F5C8 = 0;
s32 D_L12_001BA260        NOT_SDA = 0;
s32 D_L12_00184694        NOT_SDA = 0;
u8                        D_L12_00160FF8 = 0x6d;
int                       D_L12_0015F5D8 = 0;
int                       D_L12_0015F5A0 = 0;
int                       D_L12_0015F400 = 0;
O_298840                 *D_L12_0015FFDC = 0;
float                     D_L12_0015F59C = 1.0f;
float D_L12_0016CF70      NOT_SDA = 0;
int                       D_L12_00161048 = 0;
int                       D_L12_0016104C = 0;
s32                       D_L12_00161050 = 0;
s32                       D_L12_00161054 = 0;
s32                       D_L12_00161058 = 0;
s32                       D_L12_0015F40C = 0;
s32 D_L12_00174284        NOT_SDA = 0;
struct ActiveSceneTrigger D_L12_00179580 = {0};

Slideshow D_L12_0015F620 = {0};

int D_L12_00174280           NOT_SDA = 0;
int                          D_L12_001611CC = 0;
unsigned char D_L12_0019804C NOT_SDA = 0;
VendorList                   D_L12_001CA940 = {0};

VendorSlot D_L12_00166300[4] __attribute__((section(".bss")));

s32 D_L12_00162298 __attribute__((section(".bss")));

s32 D_L12_001622A0 __attribute__((section(".bss")));

s32 D_L12_001622A4 __attribute__((section(".bss")));

s32 D_L12_001622B0 __attribute__((section(".bss")));

s32 D_L12_001622B4 __attribute__((section(".bss")));

Vec4 D_L12_001806E0 = {0};

s32  D_L12_00161188 = 0;
s32  D_L12_0016118C = 0;
f32  D_L12_00161060 = 3.8f;
Vec4 D_L12_001806D0 = {0};

f32 D_L12_001610E0 = 0.9f;
int D_L12_00161070 = 0x40600000;
s32 D_L12_00161100 = 0;
s32 D_L12_001610F0 = 0x40800000;
int D_L12_0015F3F8 = 0;
int D_L12_00161194 = 0;

int D_L12_001622B8 __attribute__((section(".bss")));

float D_L12_001C8DA0 NOT_SDA = 0;
float D_L12_001C8D80 NOT_SDA = 0;
int                  D_L12_00161090 = 0;
VendorSound          D_L12_001C2588 = {0};

Vec4 D_L12_00166400[3] __attribute__((section(".bss")));

void *D_L12_00197780 NOT_SDA = 0;

s32 D_L12_0016228C __attribute__((section(".bss")));

s32 D_L12_00162290 __attribute__((section(".bss")));

int                D_L13_0015F2F0 = 0;
int                D_L13_0015F2F4 = 0;
int                D_L13_0015F424 = 0;
int                D_L13_0015F428 = 0;
int                D_L13_0015F42C = 0;
int                D_L13_0015F430 = 0;
int                D_L13_0015F434 = 0;
int D_L13_0017E804 NOT_SDA = 0;
int                D_L13_00160FF0 = 0;
int                D_L13_0015F598 = 0;
S_297f78           D_L13_0016C358 = {0};

s32                       D_L13_0015F5C8 = 0;
s32 D_L13_001BA0E0        NOT_SDA = 0;
s32 D_L13_00184514        NOT_SDA = 0;
u8                        D_L13_00160FF8 = 0x6d;
int                       D_L13_0015F5D8 = 0;
int                       D_L13_0015F5A0 = 0;
int                       D_L13_0015F400 = 0;
O_298840                 *D_L13_0015FFDC = 0;
char D_L13_0017C740       NOT_SDA = 0;
float                     D_L13_0015F59C = 1.0f;
int                       D_L13_00161048 = 0;
int                       D_L13_0016104C = 0;
s32                       D_L13_00161050 = 0;
s32                       D_L13_00161054 = 0;
s32                       D_L13_00161058 = 0;
s32                       D_L13_0015F40C = 0;
s32 D_L13_00174104        NOT_SDA = 0;
struct ActiveSceneTrigger D_L13_00179400 = {0};

Slideshow D_L13_0015F620 = {0};

int D_L13_00174100           NOT_SDA = 0;
int                          D_L13_001611CC = 0;
unsigned char D_L13_00197ECC NOT_SDA = 0;
VendorList                   D_L13_001CA7C0 = {0};

VendorSlot D_L13_00166180[4] __attribute__((section(".bss")));

s32 D_L13_00162118 __attribute__((section(".bss")));

s32 D_L13_00162120 __attribute__((section(".bss")));

s32 D_L13_00162124 __attribute__((section(".bss")));

s32 D_L13_00162130 __attribute__((section(".bss")));

s32 D_L13_00162134 __attribute__((section(".bss")));

Vec4 D_L13_00180560 = {0};

s32  D_L13_00161188 = 0;
s32  D_L13_0016118C = 0;
f32  D_L13_00161060 = 3.8f;
Vec4 D_L13_00180550 = {0};

f32 D_L13_001610E0 = 0.9f;
int D_L13_00161070 = 0x40600000;
s32 D_L13_00161100 = 0;
s32 D_L13_001610F0 = 0x40800000;
int D_L13_0015F3F8 = 0;
int D_L13_00161194 = 0;

int D_L13_00162138 __attribute__((section(".bss")));

float D_L13_001C8C20 NOT_SDA = 0;
float D_L13_001C8C00 NOT_SDA = 0;
int                  D_L13_00161090 = 0;
VendorSound          D_L13_001C2408 = {0};

Vec4 D_L13_00166280[3] __attribute__((section(".bss")));

void *D_L13_00197600 NOT_SDA = 0;

s32 D_L13_0016210C __attribute__((section(".bss")));

s32 D_L13_00162110 __attribute__((section(".bss")));

int      D_L14_0015F2F0 = 0;
int      D_L14_0015F2F4 = 0;
int      D_L14_0015F424 = 0;
int      D_L14_0015F428 = 0;
int      D_L14_0015F42C = 0;
int      D_L14_0015F430 = 0;
int      D_L14_0015F434 = 0;
int      D_L14_00160FF0 = 0;
int      D_L14_0015F598 = 0;
S_297f78 D_L14_0016C758 = {0};

s32                       D_L14_0015F5C8 = 0;
s32 D_L14_001BA4E0        NOT_SDA = 0;
s32 D_L14_00184914        NOT_SDA = 0;
u8                        D_L14_00160FF8 = 0x6d;
int                       D_L14_0015F5D8 = 0;
int                       D_L14_0015F5A0 = 0;
float                     D_L14_0015F3FC = 0;
int                       D_L14_0015F400 = 0;
char D_L14_0017CB40       NOT_SDA = 0;
float                     D_L14_0015F59C = 1.0f;
float D_L14_0016D1F0      NOT_SDA = 0;
int                       D_L14_00161048 = 0;
int                       D_L14_0016104C = 0;
s32                       D_L14_00161050 = 0;
s32                       D_L14_00161054 = 0;
s32                       D_L14_00161058 = 0;
s32                       D_L14_0015F40C = 0;
s32 D_L14_00174504        NOT_SDA = 0;
struct ActiveSceneTrigger D_L14_00179800 = {0};

Slideshow D_L14_0015F620 = {0};

int D_L14_00174500           NOT_SDA = 0;
int                          D_L14_001611CC = 0;
unsigned char D_L14_001982CC NOT_SDA = 0;
VendorList                   D_L14_001CABC0 = {0};

VendorSlot D_L14_00166580[4] __attribute__((section(".bss")));

s32 D_L14_00162518 __attribute__((section(".bss")));

s32 D_L14_00162520 __attribute__((section(".bss")));

s32 D_L14_00162524 __attribute__((section(".bss")));

s32 D_L14_00162530 __attribute__((section(".bss")));

s32 D_L14_00162534 __attribute__((section(".bss")));

Vec4 D_L14_00180960 = {0};

s32  D_L14_00161188 = 0;
s32  D_L14_0016118C = 0;
f32  D_L14_00161060 = 3.8f;
Vec4 D_L14_00180950 = {0};

f32 D_L14_001610E0 = 0.9f;
int D_L14_00161070 = 0x40600000;
s32 D_L14_00161100 = 0;
s32 D_L14_001610F0 = 0x40800000;
int D_L14_0015F3F8 = 0;
int D_L14_00161194 = 0;

int D_L14_00162538 __attribute__((section(".bss")));

float D_L14_001C9020 NOT_SDA = 0;
float D_L14_001C9000 NOT_SDA = 0;
int                  D_L14_00161090 = 0;
VendorSound          D_L14_001C2808 = {0};

Vec4 D_L14_00166680[3] __attribute__((section(".bss")));

void *D_L14_00197A00 NOT_SDA = 0;

s32 D_L14_0016250C __attribute__((section(".bss")));

s32 D_L14_00162510 __attribute__((section(".bss")));

int      D_L15_0015F2F0 = 0;
int      D_L15_0015F2F4 = 0;
int      D_L15_0015F424 = 0;
int      D_L15_0015F428 = 0;
int      D_L15_0015F42C = 0;
int      D_L15_0015F430 = 0;
int      D_L15_0015F434 = 0;
int      D_L15_00160FF0 = 0;
int      D_L15_0015F598 = 0;
S_297f78 D_L15_0016C658 = {0};

s32                       D_L15_0015F5C8 = 0;
s32 D_L15_001BA3E0        NOT_SDA = 0;
s32 D_L15_00184814        NOT_SDA = 0;
u8                        D_L15_00160FF8 = 0x6d;
int                       D_L15_0015F5D8 = 0;
int                       D_L15_0015F5A0 = 0;
int                       D_L15_0015F400 = 0;
O_298840                 *D_L15_0015FFDC = 0;
char D_L15_0017CA40       NOT_SDA = 0;
float                     D_L15_0015F59C = 1.0f;
float D_L15_0016D0F0      NOT_SDA = 0;
int                       D_L15_00161048 = 0;
int                       D_L15_0016104C = 0;
s32                       D_L15_00161050 = 0;
s32                       D_L15_00161054 = 0;
s32                       D_L15_00161058 = 0;
s32                       D_L15_0015F40C = 0;
s32 D_L15_00174404        NOT_SDA = 0;
struct ActiveSceneTrigger D_L15_00179700 = {0};

Slideshow D_L15_0015F620 = {0};

int D_L15_00174400           NOT_SDA = 0;
int                          D_L15_001611CC = 0;
unsigned char D_L15_001981CC NOT_SDA = 0;
VendorList                   D_L15_001CAAC0 = {0};

VendorSlot D_L15_00166480[4] __attribute__((section(".bss")));

s32 D_L15_00162418 __attribute__((section(".bss")));

s32 D_L15_00162420 __attribute__((section(".bss")));

s32 D_L15_00162424 __attribute__((section(".bss")));

s32 D_L15_00162430 __attribute__((section(".bss")));

s32 D_L15_00162434 __attribute__((section(".bss")));

Vec4 D_L15_00180860 = {0};

s32  D_L15_00161188 = 0;
s32  D_L15_0016118C = 0;
f32  D_L15_00161060 = 3.8f;
Vec4 D_L15_00180850 = {0};

f32 D_L15_001610E0 = 0.9f;
int D_L15_00161070 = 0x40600000;
s32 D_L15_00161100 = 0;
s32 D_L15_001610F0 = 0x40800000;
int D_L15_00161194 = 0;

int D_L15_00162438 __attribute__((section(".bss")));

float D_L15_001C8F20 NOT_SDA = 0;
float D_L15_001C8F00 NOT_SDA = 0;
int                  D_L15_00161090 = 0;
VendorSound          D_L15_001C2708 = {0};

Vec4 D_L15_00166580[3] __attribute__((section(".bss")));

void *D_L15_00197900 NOT_SDA = 0;

s32 D_L15_0016240C __attribute__((section(".bss")));

s32 D_L15_00162410 __attribute__((section(".bss")));

int      D_L16_0015F2F0 = 0;
int      D_L16_0015F2F4 = 0;
int      D_L16_0015F424 = 0;
int      D_L16_0015F428 = 0;
int      D_L16_0015F42C = 0;
int      D_L16_0015F430 = 0;
int      D_L16_0015F434 = 0;
int      D_L16_00160FF0 = 0;
int      D_L16_0015F598 = 0;
S_297f78 D_L16_0016C458 = {0};

s32                       D_L16_0015F5C8 = 0;
s32 D_L16_001BA260        NOT_SDA = 0;
s32 D_L16_00184694        NOT_SDA = 0;
u8                        D_L16_00160FF8 = 0x6d;
int                       D_L16_0015F5D8 = 0;
int                       D_L16_0015F5A0 = 0;
float                     D_L16_0015F3FC = 0;
int                       D_L16_0015F400 = 0;
O_298840                 *D_L16_0015FFDC = 0;
char D_L16_0017C8A0       NOT_SDA = 0;
float                     D_L16_0015F59C = 1.0f;
float D_L16_0016CEF0      NOT_SDA = 0;
int                       D_L16_00161048 = 0;
int                       D_L16_0016104C = 0;
s32                       D_L16_00161050 = 0;
s32                       D_L16_00161054 = 0;
s32                       D_L16_00161058 = 0;
s32                       D_L16_0015F40C = 0;
s32 D_L16_00174204        NOT_SDA = 0;
struct ActiveSceneTrigger D_L16_00179500 = {0};

Slideshow D_L16_0015F620 = {0};

int D_L16_00174200           NOT_SDA = 0;
int                          D_L16_001611CC = 0;
unsigned char D_L16_0019804C NOT_SDA = 0;
VendorList                   D_L16_001CA940 = {0};

VendorSlot D_L16_001662C0[4] __attribute__((section(".bss")));

s32 D_L16_00162218 __attribute__((section(".bss")));

s32 D_L16_00162220 __attribute__((section(".bss")));

s32 D_L16_00162224 __attribute__((section(".bss")));

s32 D_L16_00162230 __attribute__((section(".bss")));

s32 D_L16_00162234 __attribute__((section(".bss")));

Vec4 D_L16_001806E0 = {0};

s32  D_L16_00161188 = 0;
s32  D_L16_0016118C = 0;
f32  D_L16_00161060 = 3.8f;
Vec4 D_L16_001806D0 = {0};

f32 D_L16_001610E0 = 0.9f;
int D_L16_00161070 = 0x40600000;
s32 D_L16_00161100 = 0;
s32 D_L16_001610F0 = 0x40800000;
int D_L16_0015F3F8 = 0;
int D_L16_00161194 = 0;

int D_L16_00162238 __attribute__((section(".bss")));

float D_L16_001C8DA0 NOT_SDA = 0;
float D_L16_001C8D80 NOT_SDA = 0;
int                  D_L16_00161090 = 0;
VendorSound          D_L16_001C2588 = {0};

Vec4 D_L16_001663C0[3] __attribute__((section(".bss")));

void *D_L16_00197780 NOT_SDA = 0;

s32 D_L16_0016220C __attribute__((section(".bss")));

s32 D_L16_00162210 __attribute__((section(".bss")));

int                D_L17_0015F2F0 = 0;
int                D_L17_0015F2F4 = 0;
int                D_L17_0015F424 = 0;
int                D_L17_0015F428 = 0;
int                D_L17_0015F42C = 0;
int                D_L17_0015F430 = 0;
int                D_L17_0015F434 = 0;
int D_L17_0017EE04 NOT_SDA = 0;
int                D_L17_00160FF0 = 0;
int                D_L17_0015F598 = 0;
S_297f78           D_L17_0016C958 = {0};

s32                       D_L17_0015F5C8 = 0;
s32 D_L17_001BA6E0        NOT_SDA = 0;
s32 D_L17_00184B14        NOT_SDA = 0;
u8                        D_L17_00160FF8 = 0x6d;
int                       D_L17_0015F5D8 = 0;
int                       D_L17_0015F5A0 = 0;
int                       D_L17_0015F400 = 0;
char D_L17_0017CD40       NOT_SDA = 0;
float                     D_L17_0015F59C = 1.0f;
int                       D_L17_00161048 = 0;
int                       D_L17_0016104C = 0;
s32                       D_L17_00161050 = 0;
s32                       D_L17_00161054 = 0;
s32                       D_L17_00161058 = 0;
s32                       D_L17_0015F40C = 0;
s32 D_L17_00174704        NOT_SDA = 0;
struct ActiveSceneTrigger D_L17_00179A00 = {0};

Slideshow D_L17_0015F620 = {0};

int D_L17_00174700           NOT_SDA = 0;
int                          D_L17_001611CC = 0;
unsigned char D_L17_001984CC NOT_SDA = 0;
VendorList                   D_L17_001CADC0 = {0};

VendorSlot D_L17_00166780[4] __attribute__((section(".bss")));

s32 D_L17_00162718 __attribute__((section(".bss")));

s32 D_L17_00162720 __attribute__((section(".bss")));

s32 D_L17_00162724 __attribute__((section(".bss")));

s32 D_L17_00162730 __attribute__((section(".bss")));

s32 D_L17_00162734 __attribute__((section(".bss")));

Vec4 D_L17_00180B60 = {0};

s32  D_L17_00161188 = 0;
s32  D_L17_0016118C = 0;
f32  D_L17_00161060 = 3.8f;
Vec4 D_L17_00180B50 = {0};

f32 D_L17_001610E0 = 0.9f;
int D_L17_00161070 = 0x40600000;
s32 D_L17_00161100 = 0;
s32 D_L17_001610F0 = 0x40800000;
int D_L17_0015F3F8 = 0;
int D_L17_00161194 = 0;

int D_L17_00162738 __attribute__((section(".bss")));

float D_L17_001C9220 NOT_SDA = 0;
float D_L17_001C9200 NOT_SDA = 0;
int                  D_L17_00161090 = 0;
VendorSound          D_L17_001C2A08 = {0};

Vec4 D_L17_00166880[3] __attribute__((section(".bss")));

void *D_L17_00197C00 NOT_SDA = 0;

s32 D_L17_0016270C __attribute__((section(".bss")));

s32 D_L17_00162710 __attribute__((section(".bss")));

int                D_L18_0015F2F0 = 0;
int                D_L18_0015F2F4 = 0;
int                D_L18_0015F424 = 0;
int                D_L18_0015F428 = 0;
int                D_L18_0015F42C = 0;
int                D_L18_0015F430 = 0;
int                D_L18_0015F434 = 0;
int D_L18_0017EF04 NOT_SDA = 0;
int                D_L18_00160FF0 = 0;
int                D_L18_0015F598 = 0;
S_297f78           D_L18_0016CA58 = {0};

s32                  D_L18_0015F5C8 = 0;
s32 D_L18_001BA7E0   NOT_SDA = 0;
s32 D_L18_00184C14   NOT_SDA = 0;
u8                   D_L18_00160FF8 = 0x6d;
int                  D_L18_0015F5D8 = 0;
int                  D_L18_0015F5A0 = 0;
int                  D_L18_0015F400 = 0;
O_298840            *D_L18_0015FFDC = 0;
float                D_L18_0015F59C = 1.0f;
float D_L18_0016D4F0 NOT_SDA = 0;
int                  D_L18_00161048 = 0;
int                  D_L18_0016104C = 0;
s32                  D_L18_00161050 = 0;
s32                  D_L18_00161054 = 0;
s32                  D_L18_00161058 = 0;
s32 D_L18_00174804   NOT_SDA = 0;
Slideshow            D_L18_0015F620 = {0};

int                          D_L18_001611CC = 0;
unsigned char D_L18_001985CC NOT_SDA = 0;
VendorList                   D_L18_001CAEC0 = {0};

VendorSlot D_L18_00166880[4] __attribute__((section(".bss")));

s32 D_L18_00162818 __attribute__((section(".bss")));

s32 D_L18_00162820 __attribute__((section(".bss")));

s32 D_L18_00162824 __attribute__((section(".bss")));

s32 D_L18_00162830 __attribute__((section(".bss")));

s32 D_L18_00162834 __attribute__((section(".bss")));

Vec4 D_L18_00180C60 = {0};

s32  D_L18_00161188 = 0;
s32  D_L18_0016118C = 0;
f32  D_L18_00161060 = 3.8f;
Vec4 D_L18_00180C50 = {0};

f32 D_L18_001610E0 = 0.9f;
int D_L18_00161070 = 0x40600000;
s32 D_L18_00161100 = 0;
s32 D_L18_001610F0 = 0x40800000;
int D_L18_0015F3F8 = 0;
int D_L18_00161194 = 0;

int D_L18_00162838 __attribute__((section(".bss")));

float D_L18_001C9320 NOT_SDA = 0;
float D_L18_001C9300 NOT_SDA = 0;
int                  D_L18_00161090 = 0;
VendorSound          D_L18_001C2B08 = {0};

Vec4 D_L18_00166980[3] __attribute__((section(".bss")));

s32 D_L18_0016280C __attribute__((section(".bss")));

s32 D_L18_00162810 __attribute__((section(".bss")));

float D_L00_0015F59C = 1.0f;

float D_L00_0015F5A0 __asm__("D_L00_0015F5A0") = 0;

int D_L00_00161048_macro __asm__("D_L00_00161048") = 0;

int D_L00_0016104C_macro __asm__("D_L00_0016104C") = 0;

s32 D_L00_00161050_macro __asm__("D_L00_00161050") = 0;

s32 D_L00_00161054_macro __asm__("D_L00_00161054") = 0;

s32 D_L00_00161058_macro __asm__("D_L00_00161058") = 0;

Slideshow slideshow __asm__("D_L00_0015F620") = {0};

unsigned char D_L00_00197BCC NOT_SDA = 0;
