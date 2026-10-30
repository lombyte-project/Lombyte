/* Level 06 moby class 0x454: countdown sequence; once its group is cleared it runs the red button camera path, voice lines and the timed countdown. */
#include "types.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "asm.h"
#include "qcopy.h"
#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/overlay/shared/audio/music/request_music_track.h"
#include "rnc/overlay/l06/scripted_camera_sequence.h"
#include "rnc/overlay/l06/mobys/countdown_sequence_globals.h"

extern int D_L06_001ABFC0_u[] __asm__("D_L06_001ABFC0");

/* 0x002ffc38, 116 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
void set_moby_list_state_17(int moby_list_index) __asm__("FUN_L06_002ffc38");

#include "rnc/overlay/cuboid.h"

extern void set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void jet_mount(void *, void *, int, int, int) __asm__("FUN_L00_002eaaa0");
extern void FUN_L06_00312b40(float, float, float, float, float, float);
extern void FUN_L06_00312a78(void *);
extern void FUN_L06_00312ad0(void *);
extern void FUN_L06_00312ce8(int);

extern float approach_value(float *, float, float) __asm__("FUN_00213ed8");

#include "rnc/game/game_globals.h"
#include "rnc/overlay/l06/mobys/alien_queen.h"

extern char *D_L06_0015FFD8_301b90 __asm__("D_L06_0015FFD8");
extern int  *D_L06_001B0C30_301b90[] __asm__("D_L06_001B0C30");
extern char  dialogue_playback_phase[] __asm__("D_0015172A");
extern int   D_L06_0015F5C4 __asm__("D_L06_0015F5C4");
extern float D_L06_00162098 __asm__("D_L06_00162098") __attribute__((sda));
extern float D_L06_0016209C __asm__("D_L06_0016209C") __attribute__((sda));
extern float D_L06_001620A0 __asm__("D_L06_001620A0") __attribute__((sda));
extern float D_L06_001620A4 __asm__("D_L06_001620A4") __attribute__((sda));
extern float D_L06_001620A8 __asm__("D_L06_001620A8") __attribute__((sda));
extern float D_L06_001620B0 __asm__("D_L06_001620B0") __attribute__((sda));
extern float l06_linked_sequence_post_path_countdown_duration __asm__("D_L06_001620C4")
    __attribute__((sda));
extern void set_moby_list_state_4(int) __asm__("FUN_L06_002f45f0");
extern int  count_active_moby_group_members(int, int) __asm__("FUN_L01_0026e008");
void        begin_red_button_rotation(char *moby) __asm__("FUN_L06_003047b8");
extern void begin_red_button_rotation(char *) __asm__("FUN_L06_003047b8");
s32         get_red_button_sequence_status(const u8 *moby) __asm__("FUN_L06_003047f8");
extern int  get_red_button_sequence_status(const u8 *) __asm__("FUN_L06_003047f8");
void select_localized_moby_voice_line(struct Moby *moby, s32 kind) __asm__("FUN_L06_00302400");
void set_moby_state_5(unsigned char *moby) __asm__("FUN_L06_00304088");
extern void  select_localized_moby_voice_line(struct Moby *, int) __asm__("FUN_L06_00302400");
extern void  set_moby_list_state_17(int) __asm__("FUN_L06_002ffc38");
extern void  set_moby_list_state_15(int) __asm__("FUN_L06_002e9e30");
extern void  clear_moby_list_state_4(int) __asm__("FUN_L06_002f45a0");
extern void  activate_listed_spark_emitters(int) __asm__("FUN_L06_00309240");
extern void  FUN_L06_002356a0(int, int);
extern float scale_recovery_time(float) __asm__("FUN_L06_00216fa8");
extern float FUN_002132a8(float, float);
extern void  continue_audio_stream_if_ready(void) __asm__("FUN_00215b10");
extern int   hero_respawn(void) __asm__("FUN_L00_00211250");
extern void  FUN_001f4600(int, char *) __asm__("FUN_L06_00210fc8");
extern void  draw_pulsing_timer_display(char *) __asm__("FUN_L06_003021d8");
extern void  allocate_voice_for_group_entry(int, int, int) __asm__("FUN_0022dba0");
void         update_countdown_sequence(unsigned char *moby) __asm__("FUN_L06_00301b90");

struct TauntVoiceVars {
    u8  pad0[0x38];
    s32 greet[6];      /* 0x38: one per language */
    s32 leave[6];      /* 0x50 */
    s32 chatter[3][6]; /* 0x68 */
};

extern s32 chatter_variant_used_flags[1] __asm__("D_L06_001620B8") __attribute__((sda));
extern s32 queued_dialogue_id[] __asm__("D_001516EC");
extern s32 FUN_00213260(s32);
extern int D_L06_0015F5C4;

extern float approach_value(float *p, float target, float maxstep) __asm__("FUN_00213ed8");

/* Ported from rac1-decomp (src/overlays/l06_blarg/vendor_002FE5D0.c: func_L06_00305BF8), where it is exact; names translated to the US level program. */
extern int allocate_voice_for_target_entry(int, int, void *) __asm__("FUN_0022da68");

#include "rnc/game/item_state.h"

extern int D_L06_001ABFC0[];
extern int D_L06_0015FFD8;

/* 0x002e9e30, 108 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
/* Switches every moby listed in a table into state 15 until a negative entry. */
void set_moby_list_state_15(int index) __asm__("FUN_L06_002e9e30");

/* 0x002f45a0, 80 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
/* Clears state 4 from every moby in the selected table. */
void clear_moby_list_state_4(int moby_list_index) __asm__("FUN_L06_002f45a0");

/* 0x002f45f0, 72 bytes.
 * Ported from rac1-decomp, where it is exact; names translated to
 * the US level program (data still reached through the MACRO_ADDR form). */
/* Sets each moby in the selected table to state 4 until its terminal entry. */
void set_moby_list_state_4(int moby_list_index) __asm__("FUN_L06_002f45f0");

void set_moby_list_state_15(int index) {
    short *moby_index_cursor = D_L06_001ABFC0[index];
    do {
        char *moby = D_L06_0015FFD8 + ((*(unsigned short *)moby_index_cursor & 0x7FFF) << 8);
        if ((unsigned char)moby[0x20] < 0x7F) {
            unsigned short flags = MOBY(moby)->flags;
            moby[0x20] = 15;
            flags |= 0x41;
            MOBY(moby)->collision = 0;
            flags &= 0xEFFF;
            MOBY(moby)->flags = flags;
        }
    } while (*moby_index_cursor++ >= 0);
}

void clear_moby_list_state_4(int moby_list_index) {
    short *moby_index_cursor = D_L06_001ABFC0[moby_list_index];
    do {
        char *moby = (char *)(((*moby_index_cursor & 0x7FFF) << 8) + (int)D_L06_0015FFD8);
        if ((unsigned char)moby[0x20] == 4) {
            moby[0x20] = 0;
        }
    } while (*moby_index_cursor++ >= 0);
}

void set_moby_list_state_4(int moby_list_index) {
    short *moby_index_cursor = D_L06_001ABFC0[moby_list_index];
    do {
        char *moby = (char *)(((*moby_index_cursor & 0x7FFF) << 8) + (int)D_L06_0015FFD8);
        moby[0x20] = 4;
    } while (*moby_index_cursor++ >= 0);
}

void set_moby_state_3(unsigned char *moby) __asm__("FUN_L06_002fc630");

void set_moby_state_3(unsigned char *moby) {
    moby[0x20] = 0x3;
}

void set_moby_list_state_17(int moby_list_index) {
    short *moby_list_entry = D_L06_001ABFC0_u[moby_list_index];
    if (moby_list_entry != 0) {
        do {
            char *moby = D_L06_0015FFD8 + ((*moby_list_entry & 0x7FFF) << 8);
            if ((unsigned char)moby[0x20] < 0x7F) {
                unsigned short flags = MOBY(moby)->flags;
                moby[0x20] = 0x11;
                flags |= 1;
                MOBY(moby)->collision = 0;
                flags &= 0xEFFF;
                MOBY(moby)->flags = flags;
            }
        } while (*moby_list_entry++ >= 0);
    }
}

void update_countdown_sequence(unsigned char *moby) {
    enum {
        INITIALIZE_ENTITY = 0,
        TRIGGER_LINKED_SEQUENCE_WHEN_GROUP_INACTIVE = 1,
        WAIT_FOR_LINKED_SEQUENCE_COMPLETION = 2,
        ADVANCE_CAMERA_ALONG_PATH = 3,
        UPDATE_POST_PATH_TIMED_EVENTS = 4,
        RAMP_SHARED_VALUE_AFTER_DELAY = 5
    };
    char *moby_vars = ((char *)MOBY(moby)->pvars);
    float va[4] __attribute__((aligned(16)));
    float vc[4] __attribute__((aligned(16)));
    float vb[4] __attribute__((aligned(16)));
    switch (moby[0x20]) {
    case INITIALIZE_ENTITY:
        if (visited52[moby[0xB0] + (current_level_index << 4)] != 0) {
            mark_moby_for_removal((struct Obj *)moby);
            return;
        }
        set_moby_list_state_4(*(int *)(moby_vars + 0));
        set_moby_list_state_18(*(int *)(moby_vars + 4));
        set_moby_list_state_18(*(int *)(moby_vars + 8));
        moby[0x20] = 1;
        moby[0x30] = 0xFF;
        return;
    case TRIGGER_LINKED_SEQUENCE_WHEN_GROUP_INACTIVE:
        if (count_active_moby_group_members(*(int *)(moby_vars + 0x1C), -1) != 0)
            return;
        begin_red_button_rotation(D_L06_0015FFD8_301b90 + (*(int *)(moby_vars + 0x2C) << 8));
        if (get_red_button_sequence_status((const u8 *)D_L06_0015FFD8_301b90 +
                                           (*(int *)(moby_vars + 0x2C) << 8)) != 1)
            return;
        moby[0x20] = 2;
        select_localized_moby_voice_line((struct Moby *)moby, 0);
        return;
    case WAIT_FOR_LINKED_SEQUENCE_COMPLETION: {
        int  *p;
        float r;
        if (get_red_button_sequence_status((const u8 *)D_L06_0015FFD8_301b90 +
                                           (*(int *)(moby_vars + 0x2C) << 8)) != 2)
            return;
        moby[0x20] = 3;
        *(int *)(moby_vars + 0xB0) = 1;
        request_music_track(6, 9);
        if (*(short *)dialogue_playback_phase == 3)
            continue_audio_stream_if_ready();
        set_moby_list_state_17(*(int *)(moby_vars + 0x10));
        set_moby_list_state_15(*(int *)(moby_vars + 0x14));
        set_moby_list_state_15(*(int *)(moby_vars + 0x18));
        clear_moby_list_state_4(*(int *)(moby_vars + 0));
        clear_moby_list_state_18(*(int *)(moby_vars + 4));
        clear_moby_list_state_18(*(int *)(moby_vars + 8));
        set_moby_state_3(D_L06_0015FFD8_301b90 + (*(int *)(moby_vars + 0x20) << 8));
        {
            ((LevelCuboid *)((*(int *)(moby_vars + 0xC) << 7) + (int)l06_cuboids))->pos.z += 10.0f;
            p = D_L06_001B0C30_301b90[*(int *)(moby_vars + 0x24)];
            qcopy(va, (char *)p + 0x20);
            qcopy(vb, (char *)p + 0x10);
            clear_u64_value(vc);
            vc[2] = fast_arc_tan(vb[0] - va[0], vb[1] - va[1]);
            r = vector_distance_xy(va, vb);
            vc[1] = -fast_arc_tan(r, vb[2] - va[2]);
            jet_mount(va, vc, 2, scale_game_frames(300), 0);
            FUN_L06_00312b40(D_L06_00162098,
                             D_L06_0016209C,
                             D_L06_001620A0,
                             D_L06_001620A4,
                             D_L06_001620A8,
                             D_L06_001620B0);
            FUN_L06_00312a78(va);
            FUN_L06_00312ad0(vc);
            {
                LevelCuboid *q = &l06_cuboids[*(int *)(moby_vars + 0x28)];
                set_race_camera(&q->pos, &q->rot, 0x72, 0);
            }
        }
        return;
    }
    case ADVANCE_CAMERA_ALONG_PATH: {
        int **p;
        int   x, nb;
        float r;
        p = &D_L06_001B0C30_301b90[*(int *)(moby_vars + 0x24)];
        nb = *(int *)(moby_vars + 0xB0);
        qcopy(vb, (char *)((nb << 4) + (int)*p) + 0x10);
        nb++;
        *(int *)(moby_vars + 0xB0) = nb;
        if (nb == **p - 1) {
            moby[0x20] = 4;
            select_localized_moby_voice_line((struct Moby *)moby, 1);
            *(int *)(moby_vars + 0xB4) = truncate_float_to_s32(
                scale_recovery_time(l06_linked_sequence_post_path_countdown_duration));
            FUN_L06_00312ce8(2);
            FUN_L06_002356a0(0, 1);
            scripted_camera_sequence_active = 0;
            return;
        }
        activate_listed_spark_emitters(*(int *)(moby_vars + 0x34));
        x = *(int *)(moby_vars + 0xB0) - 30;
        if (x < 0)
            x = 0;
        qcopy(va, (char *)D_L06_001B0C30_301b90[*(int *)(moby_vars + 0x24)] + 0x10 + (x << 4));
        clear_u64_value(vc);
        vc[2] = fast_arc_tan(va[0] - vb[0], va[1] - vb[1]);
        r = vector_distance_xy(vb, va);
        vc[1] = -fast_arc_tan(r, va[2] - vb[2]);
        FUN_L06_00312a78(vb);
        FUN_L06_00312ad0(vc);
        return;
    }
    case UPDATE_POST_PATH_TIMED_EVENTS: {
        int   x;
        char *q;
        x = scale_game_frames(0x708);
        if (*(int *)(moby_vars + 0xB4) == x) {
            if (*(short *)dialogue_playback_phase == 3)
                continue_audio_stream_if_ready();
        } else {
            x = scale_game_frames(900);
            if (*(int *)(moby_vars + 0xB4) == x)
                select_localized_moby_voice_line((struct Moby *)moby, 2);
            else {
                x = scale_game_frames(300);
                if (*(int *)(moby_vars + 0xB4) == x && *(short *)dialogue_playback_phase == 3)
                    continue_audio_stream_if_ready();
            }
        }
        if (fast_dec_timer(moby_vars + 0xC0) != 0) {
            allocate_voice_for_group_entry(0, 0, (int)hero.moby);
            *(int *)(moby_vars + 0xC0) =
                truncate_float_to_s32(scale_recovery_time(FUN_002132a8(480.0f, 600.0f)));
            l06_camera.shake_amount = 0.4f;
            l06_camera.shake_frames = scale_game_frames(60);
        }
        if (D_L06_0015F5C4 == 0)
            FUN_001f4600((int)draw_pulsing_timer_display, (char *)moby);
        if (hero.state.current == 0x72) {
            moby[0x20] = 6;
            return;
        }
        if (D_L06_0015F5C4 != 0)
            return;
        if (fast_dec_timer(moby_vars + 0xB4) == 0)
            return;
        moby[0x20] = 5;
        x = scale_game_frames(15);
        q = D_L06_0015FFD8_301b90 + (*(int *)(moby_vars + 0x30) << 8);
        *(float *)(moby_vars + 0xB8) = 0.0f;
        *(int *)(moby_vars + 0xBC) = x;
        set_moby_state_5(q);
        allocate_voice_for_group_entry(1, 0, (int)hero.moby);
        return;
    }
    case RAMP_SHARED_VALUE_AFTER_DELAY:
        if (*(float *)(moby_vars + 0xB8) == 1.0f) {
            hero_respawn();
            linked_sequence_exit_ramp_progress = 0.0f;
            return;
        }
        if (fast_dec_timer(moby_vars + 0xBC) == 0)
            return;
        approach_value((float *)(moby_vars + 0xB8), 1.0f, frame_time * 6.0f);
        linked_sequence_exit_ramp_progress = *(float *)(moby_vars + 0xB8);
        return;
    }
}

/* plays one of the vendor's greet, chatter or leave lines in the current language */
void select_localized_moby_voice_line(struct Moby *moby, s32 kind) {
    enum {
        VOICE_KIND_GREETING = 0,
        VOICE_KIND_CHATTER = 1,
        VOICE_KIND_LEAVING = 2
    };
    struct TauntVoiceVars *voice_vars = (struct TauntVoiceVars *)moby->pvars;
    s32                    id = -1;
    s32                    lang;
    s32                    chatter_index;
    s32                    i;

    switch (kind) {
    case VOICE_KIND_GREETING:
        lang = game_language;
        if (lang != 0)
            lang--;
        id = voice_vars->greet[lang];
        break;
    case VOICE_KIND_CHATTER:
        chatter_index = FUN_00213260(3);
        for (i = 0; i < 4; i++) {
            if (chatter_variant_used_flags[chatter_index] == 0) {
                chatter_variant_used_flags[chatter_index] = 1;
                break;
            }
            chatter_index = (chatter_index + 1) % 3;
            if (i == 2) {
                chatter_variant_used_flags[0] = 0;
                chatter_second_variant_used = 0;
                chatter_third_variant_used = 0;
            }
        }
        lang = game_language;
        if (lang != 0)
            lang--;
        id = voice_vars->chatter[chatter_index][lang];
        break;
    case VOICE_KIND_LEAVING:
        lang = game_language;
        if (lang != 0)
            lang--;
        id = voice_vars->leave[lang];
        break;
    }
    if (id != -1) {
        id += 20000;
        queued_dialogue_id[0] = id;
    }
}

void set_moby_state_5(unsigned char *moby) {
    moby[0x20] = 0x5;
}

/* Moves the moby from state 1 to 2 and gives it a voice. */
void begin_red_button_rotation(char *moby) {
    if ((unsigned char)moby[0x20] == 1) {
        moby[0x20] = 2;
        allocate_voice_for_target_entry(0, 0, moby);
    }
}

s32 get_red_button_sequence_status(const u8 *moby) {
    u32 state = moby[0x20];

    if (state == 7) {
        return 2;
    }
    if (state > 3) {
        return 1;
    }
    return 0;
}
