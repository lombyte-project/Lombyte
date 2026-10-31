/* Level 15 Quartu cutscene trigger moby class and its message drawing. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/game/game_globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/runtime/callbacks/add_draw_callback.h"
#include "rnc/overlay/shared/mobys/apply_nearest_environment_sample.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/overlay/shared/game/queue_planet_unlock_message.h"

extern int D_L15_0015F5C4;

extern void start_scene(int) __asm__("FUN_L00_00298840");

extern int D_L15_00161AC0 __asm__("D_L15_00161AC0");

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");

extern char *D_L15_001600EC;

extern int is_point_inside_clip_volume(void *, int) __asm__("FUN_00214720");

extern void FUN_L00_00232fe8_v(void) __asm__("FUN_L00_00232fe8");

extern void queue_scene_movie_with_game_language(int) __asm__("FUN_L00_00299460");

extern void FUN_L00_002330d0_v(void) __asm__("FUN_L00_002330d0");

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");

extern void unlock_planet(int) __asm__("FUN_L00_002607d0");

extern void jet_reset_camera(void *, void *) __asm__("FUN_L00_00284e50");

void draw_quartu_cutscene_messages(void) __asm__("FUN_L15_002eb6b8");

void quartu_cutscene_trigger_update(struct Moby *moby) __asm__("FUN_L15_002eb4c0");

/* Moby class 1419 */
void quartu_cutscene_trigger_update(struct Moby *moby) {
    enum {
        CUTSCENE_TRIGGER_WAIT_FOR_CONDITIONS = 0,
        CUTSCENE_TRIGGER_START_SECOND_SCENE = 1,
        CUTSCENE_TRIGGER_WAIT_FOR_FIRST_SCENE = 2,
        CUTSCENE_TRIGGER_FINALIZE_AND_SAVE = 3,
        CUTSCENE_TRIGGER_COMPLETED = 4
    };
    char *trigger_data = (char *)moby->pvars;
    moby->update_dist = 0x80;
    switch (moby->state) {
    case CUTSCENE_TRIGGER_WAIT_FOR_CONDITIONS:
        if (visited52[moby->mission + current_level_index * 16] == 0xFF)
            break;
        if (D_L15_00161AC0 != 0)
            break;
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)trigger_data) == 0)
            break;
        if (((unsigned char *)l15_moby_array)[(*(int *)(trigger_data + 8) << 8) + 0x20] != 3)
            break;
        start_scene(3);
        moby->state = 2;
        jet_clear_persistent(moby->mission);
        FUN_L00_00232fe8_v();
        break;
    case CUTSCENE_TRIGGER_WAIT_FOR_FIRST_SCENE:
        if (D_L15_0015F5C4 != 2) {
            queue_scene_movie_with_game_language(0x11);
            moby->state = 1;
        }
        break;
    case CUTSCENE_TRIGGER_START_SECOND_SCENE:
        if (D_L15_0015F5C4 != 2) {
            start_scene(4);
            moby->state = 3;
            FUN_L00_002330d0_v();
            if (*(int *)(trigger_data + 4) != -1) {
                char *o = D_L15_001600EC + (*(int *)(trigger_data + 4) << 7);
                set_race_camera(o + 0x30, o + 0x70, 0, 1);
            }
        }
        break;
    case CUTSCENE_TRIGGER_FINALIZE_AND_SAVE:
        if (D_L15_0015F5C4 != 2) {
            unlock_planet(0x11);
            if (*(int *)(trigger_data + 4) != -1) {
                char *o = D_L15_001600EC + (*(int *)(trigger_data + 4) << 7);
                jet_reset_camera(o + 0x30, o + 0x70);
            }
            queue_planet_unlock_message(0x11);
            memcard_save_data(0, -1);
            moby->state = 4;
        }
        break;
    case CUTSCENE_TRIGGER_COMPLETED:
        break;
    }
    if (D_L15_0015F5C4 == 2)
        add_draw_callback(draw_quartu_cutscene_messages, moby);
}

extern char *get_help_message_text(s32) __asm__("func_001FDD10");

extern char quartu_cutscene_message_format[] __asm__("D_L15_00162190");

extern char D_L15_0016CE60[];

extern int sprintf(char *str, const char *fmt, ...) __asm__("FUN_00116248");

extern int strlen_alt(char *) __asm__("FUN_001166cc");

extern void print_text_with_texture_3(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_L00_001fb470");

int D_L15_00162180 = 150;

int D_L15_00162184 = 150;

int D_L15_00162188 = 0x3800ffff;

int D_L15_0016218C = 0x38800000;

void draw_quartu_cutscene_messages(void) {
    char  formatted_message_text[0x50];
    int   flag = 0;
    int   val = 0;
    char *message_timing_state = D_L15_0016CE60;
    int   print_character_count;
    char *second_message_timing_state;
    char *third_message_timing_state;
    if (*(int *)(message_timing_state + 0x30) == 3) {
        if (scale_game_frames(0xB5) < *(int *)(message_timing_state + 0x34)) {
            if (*(int *)(message_timing_state + 0x34) < scale_game_frames(0x104)) {
                flag = 1;
                sprintf(formatted_message_text,
                        quartu_cutscene_message_format,
                        get_help_message_text(0x526A));
                val = (*(int *)(message_timing_state + 0x34) - scale_game_frames(0xB5)) >> 2;
            }
        }
        second_message_timing_state = D_L15_0016CE60;
        if (scale_game_frames(0x1C2) < *(int *)(second_message_timing_state + 0x34)) {
            if (*(int *)(second_message_timing_state + 0x34) < scale_game_frames(0x280)) {
                flag = 1;
                sprintf(formatted_message_text,
                        quartu_cutscene_message_format,
                        get_help_message_text(0x526B));
                val =
                    (*(int *)(second_message_timing_state + 0x34) - scale_game_frames(0x1C2)) >> 2;
            }
        }
        third_message_timing_state = D_L15_0016CE60;
        if (scale_game_frames(0x2EE) < *(int *)(third_message_timing_state + 0x34)) {
            if (*(int *)(third_message_timing_state + 0x34) < scale_game_frames(0x3A2)) {
                flag = 1;
                sprintf(formatted_message_text,
                        quartu_cutscene_message_format,
                        get_help_message_text(0x526C));
                val = (*(int *)(third_message_timing_state + 0x34) - scale_game_frames(0x2EE)) >> 2;
            }
        }
    } else if (*(int *)(message_timing_state + 0x30) == 4) {
        if (scale_game_frames(0x8FC) < *(int *)(message_timing_state + 0x34)) {
            if (*(int *)(message_timing_state + 0x34) < scale_game_frames(0x960)) {
                flag = 1;
                sprintf(formatted_message_text,
                        quartu_cutscene_message_format,
                        get_help_message_text(0x526E));
                val = (*(int *)(message_timing_state + 0x34) - scale_game_frames(0x8FC)) >> 2;
            }
        }
    }
    if (flag == 0)
        return;
    print_character_count = strlen_alt(formatted_message_text);
    if (val < print_character_count) {
        print_character_count = val;
    } else if (print_character_count < val) {
        if ((print_character_count - val) & 4)
            print_character_count = print_character_count - 1;
    }
    print_text_with_texture_3((void *)(D_L15_00162180 + 1),
                              (void *)(D_L15_00162184 + 1),
                              (void *)D_L15_0016218C,
                              formatted_message_text,
                              (void *)print_character_count);
    print_text_with_texture_3((void *)D_L15_00162180,
                              (void *)D_L15_00162184,
                              (void *)D_L15_00162188,
                              formatted_message_text,
                              (void *)print_character_count);
}

extern struct Moby *l15_moby_array __asm__("D_L15_0015FFD8");
