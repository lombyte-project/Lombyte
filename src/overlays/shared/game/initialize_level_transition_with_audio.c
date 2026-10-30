/* Level transitions: records and deactivates moby states, and prepares the transition to the target level, with its audio and help cleanup. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/audio/music/music_stream_state.h"
#include "asm.h"
#include "rnc/overlay/level_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/random.h"
#include "rnc/overlay/shared/hero/hero_set_state.h"
#include "rnc/save/memcard_save_data.h"
#include "rnc/mobys/animation/set_moby_animation.h"
#include "rnc/audio/streaming/continue_audio_stream_if_ready.h"
#include "rnc/world/loaders/parse_space_scene_chunk.h"

/* Start the moby's camera-follow state: stash it in the global record, point its update at func_L00_0028F458. */
/* Ported from rac1-decomp (src/overlays/shared/space_0028FB78.c: func_L00_0028FB78), where it is exact; names translated to the US level program. */

extern char D_0013E030_c[] __asm__("D_0013E030");
extern void deactivate_recorded_moby(void) __asm__("FUN_L00_0028e990");
extern void update_moby_proximity_interaction(void) __asm__("FUN_L00_0028e180");

extern s32 D_L00_00160540_gp __asm__("D_L00_00160540") __attribute__((sda));
void       initialize_moby_state_record(char *moby) __asm__("FUN_L00_0028e8a0");

void initialize_moby_state_record(char *moby) {
    char *moby_state_record;
    int  *moby_state_copy_source;
    D_L00_00160540_gp = 1;
    deactivate_recorded_moby();
    if (current_level_index == 0xD) {
        char *global_record = D_0013E030_c;
        *(float *)(global_record + 0x60) = 464.66f;
        *(float *)(global_record + 0x64) = 580.68f;
        *(float *)(global_record + 0x68) = 316.72f;
        *(float *)(global_record + 0x78) = 2.77f;
    }
    moby_state_record = D_0013E030_c;
    *(short *)(moby_state_record + 0x26) = *(unsigned short *)(moby + 0xA6) - 0x213;
    *(char **)moby_state_record = moby;
    *(void **)(moby + 0x74) = update_moby_proximity_interaction;
    *(unsigned short *)(moby + 0x34) &= 0xFFFC;
    *(int *)(moby + 0x94) = *(int *)(*(int *)(moby + 0x24) + 0x10);
    set_moby_animation(moby, 1, 0);
    moby_state_copy_source = *(int **)(moby + 0x78);
    *(int *)(moby_state_record + 0x30) = moby_state_copy_source[0];
    *(int *)(moby_state_record + 0x34) = moby_state_copy_source[1];
    *(int *)(moby_state_record + 0x38) = moby_state_copy_source[2];
}

#include "sda.h"
#include "rnc/math/fast_math.h"

extern u8 *D_0013E030 NOT_SDA;

/* Flags the moby in D_0013E030 (bits 0 and 1) and clears its field 0x94. */
void deactivate_recorded_moby(void) {
    if (D_0013E030 != 0) {
        *(u16 *)(D_0013E030 + 0x34) |= 3;
        *(s32 *)(D_0013E030 + 0x94) = 0;
    }
}

typedef struct {
    char           pad[0x20];
    unsigned char  st;
    char           pad21[0x13];
    unsigned short flags;
    char           pad36[0x70];
    short          type;
    char           pada8[0x58];
} O_28e9c8;

typedef struct {
    char           pad[0x34];
    unsigned short flags;
} F_28e9c8;

typedef struct {
    F_28e9c8 *x0;
    int       x4;
    int       x8;
    char      padc[0x14];
    int       x20;
    short     x24;
    short     x26;
    char      pad28[4];
    short     x2c;
} E_28e9c8;

typedef struct {
    char pad[0x58];
    int  x58;
    int  x5c;
} C_28e9c8;

extern E_28e9c8 D_0013E030_28e9c8 __asm__("D_0013E030");
extern C_28e9c8 D_L00_0016C860_lui __asm__("D_L00_0016C860") NOT_SDA;
extern int      D_L00_0015F5D8;

extern struct {
    int x0, x4, x8;
} D_L00_00173E00_28e9c8 __asm__("D_L00_00173E00") __attribute__((section(".data")));

extern unsigned char D_001413F5_lui __asm__("D_001413F5") NOT_SDA;
extern O_28e9c8     *D_L00_0015FFDC_28e9c8 __asm__("D_L00_0015FFDC");
extern unsigned char D_0013D4C6_lui __asm__("D_0013D4C6") NOT_SDA;
void                 fill_transfer_words(void *, int, int) __asm__("FUN_001f97e8");
void                 select_level_vu_chain(void) __asm__("FUN_002335a0");
void                 f232fe8_28e9c8(void) __asm__("FUN_L00_00232fe8");
void                 load_chunks_with_optional_fade(int, int) __asm__("FUN_L00_00245610");
void                 sound_update(void) __asm__("FUN_0022ca50");
int                  sce_gs_sync_v(int) __asm__("FUN_00122298");
void initialize_level_transition_with_audio(int audio_selection_mode) __asm__("FUN_L00_0028e9c8");

void initialize_level_transition_with_audio(int audio_selection_mode) {
    O_28e9c8 *moby;
    int       secondary_track_index, base_audio_selection_index, audio_stream_table_index,
        alternate_audio_stream_table_index, shared_table_offset;
    D_0013E030_28e9c8.x24 = -1;
    D_L00_0015F5D8 = 1;
    D_0013E030_28e9c8.x20 = audio_selection_mode;
    D_0013E030_28e9c8.x8 = 0;
    fill_transfer_words(&D_L00_0016C860_lui, 0, 0x1C0);
    fill_transfer_words(D_L00_0017C340, 0, 0x40);
    fill_transfer_words(D_L00_0017C380, 0, 0x40);
    select_level_vu_chain();
    shared_table_offset = D_L00_001611CC - 0x60000;
    D_L00_0016C860_lui.x58 = D_L00_00173E00_28e9c8.x4 + shared_table_offset;
    D_L00_0016C860_lui.x5c = D_L00_00173E00_28e9c8.x8 + shared_table_offset;
    fade_level = 1.0f;
    D_L00_0015F5C4 = 6;
    D_L00_001611CC = shared_table_offset;
    D_L00_0015F400 = 0;
    hero_set_state(100, 2);
    D_001413F5_lui = 1;
    f232fe8_28e9c8();
    D_0013E030_28e9c8.x0->flags |= 1;
    for (moby = D_L00_0015FFDC_28e9c8; moby->st != 0xFF; moby++) {
        if (!(moby->st & 0x80) && (moby->type == 0x4A || moby->type == 0xCB))
            moby->flags |= 0x80;
    }
    if (audio_selection_mode == 0) {
        base_audio_selection_index = D_0013E030_28e9c8.x26;
        audio_stream_table_index = base_audio_selection_index + 1;
        secondary_track_index = base_audio_selection_index + 6;
        if (current_level_index == 10 && base_audio_selection_index == 1 && D_0013D4C6_lui == 0) {
            audio_stream_table_index = 0;
            secondary_track_index = 11;
        } else if (current_level_index == 14 && D_0013E030_28e9c8.x26 == 2) {
            audio_stream_table_index = 8;
            secondary_track_index = 14;
        }
        load_chunks_with_optional_fade(audio_stream_table_index, scale_game_frames(6));
    } else {
        int k, fade_frame_count;
        base_audio_selection_index = D_0013E030_28e9c8.x26;
        k = D_0013E030_28e9c8.x2c;
        secondary_track_index = base_audio_selection_index + 3;
        if (k)
            secondary_track_index = base_audio_selection_index;
        alternate_audio_stream_table_index = base_audio_selection_index + 5;
        if (current_level_index == 10 && base_audio_selection_index == 1 && D_0013D4C6_lui == 0) {
            secondary_track_index = 10;
            if (k)
                secondary_track_index = 9;
            alternate_audio_stream_table_index = 4;
        } else if (current_level_index == 14 && D_0013E030_28e9c8.x26 == 2) {
            secondary_track_index = 13;
            if (D_0013E030_28e9c8.x2c)
                secondary_track_index = 12;
            alternate_audio_stream_table_index = 9;
        }
        fade_frame_count = 0;
        if (!D_0013E030_28e9c8.x2c)
            fade_frame_count = scale_game_frames(6);
        load_chunks_with_optional_fade(alternate_audio_stream_table_index, fade_frame_count);
    }
    music_stream_state.queued_secondary_track = secondary_track_index + 40000;
    parse_space_scene_chunk(0);
    while (music_stream_state.secondary.state != 3) {
        sound_update();
        sce_gs_sync_v(0);
    }
    continue_audio_stream_if_ready();
}

extern short D_0013E05C NOT_SDA;
void                    music_pause(int) __asm__("FUN_00216050");
void                    clear_active_help_record(void) __asm__("FUN_L00_002039a0");
void                    initialize_level_transition_with_audio(int) __asm__("FUN_L00_0028e9c8");
/* Sets D_L00_0015F5D8, keeps +0x22A8 >= 1, calls 00216050, sound_update, 002039a0, 0028e9c8. */
void prepare_level_transition_with_help_cleanup(void) __asm__("FUN_L00_0028ecd8");

void prepare_level_transition_with_help_cleanup(void) {
    struct Hero             *hero_state = &hero;
    struct MusicStreamState *music_state;
    if (hero_state->health.hp <= 0)
        hero_state->health.hp = 1;
    D_L00_0015F5D8 = 1;
    music_pause(0);
    music_state = &music_stream_state;
    if ((unsigned)((unsigned short)music_state->secondary.state - 6) >= 2)
        music_state->secondary.state = 5;
    sound_update();
    clear_active_help_record();
    initialize_level_transition_with_audio(0);
    D_0013E05C = 0;
}

typedef struct {
    u8  p[0x20];
    s32 i20;
    s16 h24;
    s16 h26;
} A_0028ed58;

extern A_0028ed58 level_render_state_lui __asm__("D_0013E030") NOT_SDA;

typedef struct {
    s32 i0;
    s32 i4;
    s32 i8;
} B_0028ed58;

extern B_0028ed58 D_L00_00173E00_0028ed58 __asm__("D_L00_00173E00")
    __attribute__((section(".data")));

typedef struct {
    u8  p[0x34];
    s32 i34;
    s32 i38;
    s32 i3C;
    u8  p2[0x18];
    s32 i58;
    s32 i5C;
} C_0028ed58;

extern C_0028ed58 D_L00_0016C860_0028ed58 __asm__("D_L00_0016C860")
    __attribute__((section(".data")));
void       initialize_level_transition_with_audio(s32) __asm__("FUN_L00_0028e9c8");
extern s32 transition_sequence_positions[1] __asm__("D_L00_00160570") __attribute__((sda));
void       prepare_target_level_transition(s32 target_level_index) __asm__("FUN_L00_0028ed58");

void prepare_target_level_transition(s32 target_level_index) {
    A_0028ed58 *shared_state_record = &level_render_state_lui;
    C_0028ed58 *shared_result_record;
    s32         shared_table_offset;
    (&D_L00_0015F5C4)[5] = 1; /* D_L00_0015F5D8: the unit declares it sda */
    D_L00_0015F5C4 = 6;
    fade_level = 1.0f;
    D_L00_0015F400 = 0;
    shared_state_record->i20 = 0;
    shared_state_record->h24 = -1;
    select_level_vu_chain();
    shared_table_offset = D_L00_001611CC - 0x60000;
    shared_result_record = &D_L00_0016C860_0028ed58;
    shared_result_record->i58 = D_L00_00173E00_0028ed58.i4 + shared_table_offset;
    shared_result_record->i5C = D_L00_00173E00_0028ed58.i8 + shared_table_offset;
    D_L00_001611CC = shared_table_offset;
    if (current_level_index == target_level_index) {
        s32 divisor;
        initialize_level_transition_with_audio(8);
        divisor = pal_mode ? 0x50 : 0x60;
        shared_result_record->i34 =
            scale_game_frames(transition_sequence_positions[shared_state_record->h26]);
        shared_result_record->i3C = shared_result_record->i34 / divisor;
        parse_space_scene_chunk(shared_result_record->i3C);
        shared_result_record->i38 = shared_result_record->i34 % divisor;
    } else {
        memcard_save_data(0, target_level_index);
        D_L00_0015F5C0 = target_level_index;
        shared_state_record->i20 = 3;
        shared_state_record->h24 = -1;
    }
}

typedef struct {
    char p0[0x20];
    s32  x20;
    s16  x24;
    s16  p26;
    s16  p28;
    s16  x2a;
    s16  x2c;
} E_eed0;

extern E_eed0 D_0013E030_eed0 __asm__("D_0013E030") NOT_SDA;

extern struct {
    int x0, x4, x8_c;
} D_L00_00173E00_eed0 __asm__("D_L00_00173E00") __attribute__((section(".data")));

extern struct {
    char p[0x58];
    int  x58, x5c;
} D_L00_0016C860_eed0 __asm__("D_L00_0016C860") __attribute__((section(".data")));

void                 FUN_002335a0_eed0(void) __asm__("FUN_002335a0");
extern u8 D_0013DD43 NOT_SDA;
extern u8            level_14_entry_transition_gate[] __asm__("D_0013D3F0");
void                 prepare_level_entry_transition(void) __asm__("FUN_L00_0028eed0");

void prepare_level_entry_transition(void) {
    int level_index, shared_table_offset;
    D_0013E030_eed0.x2c = 1;
    if (D_0013E030_eed0.x2a != 0)
        D_0013E030_eed0.x2a = 0;
    level_index = current_level_index;
    if (level_index == 1 && D_0013DD43 == 0) {
        D_L00_0015F5C4 = 0;
        return;
    }
    if (level_index != 0) {
        if (level_index == 0xE) {
            if (level_14_entry_transition_gate[0] == 0)
                goto out;
        }
        if (level_index < 0x14)
            goto in;
    }
out:
    D_L00_0015F5C4 = 0;
    return;
in:
    D_0013E030_eed0.x24 = -1;
    D_L00_0015F5C4 = 6;
    fade_level = 1.0f;
    D_0013E030_eed0.x20 = 0;
    FUN_002335a0_eed0();
    shared_table_offset = D_L00_001611CC - 0x60000;
    D_L00_0016C860_eed0.x58 = D_L00_00173E00_eed0.x4 + shared_table_offset;
    D_L00_0016C860_eed0.x5c = D_L00_00173E00_eed0.x8_c + shared_table_offset;
    D_L00_001611CC = shared_table_offset;
    initialize_level_transition_with_audio(8);
}

typedef struct {
    float f[4];
} __attribute__((aligned(16))) V_28efc8;

typedef struct {
    V_28efc8     corner[4];
    unsigned int color[4];
    struct {
        float u, v;
    } uv[4];
    long unk70, tex, unk80, unk88;
} Q_28efc8;

typedef struct {
    char *p0;
    char  pad[0x22];
    short h26;
} G_28efc8;

long get_effect_texture(int) __asm__("FUN_001f44b8");
void draw_geometry_quad(void *, void *, int) __asm__("FUN_001f7d30");
/* FUN_L00_0028efc8 is the executable function of the same bytes (src/gameplay). */

s32 D_L01_00160540 = 0;
s32 D_L01_00160570[1] = {0xf0};

s32 D_L02_00160540 = 0;
s32 D_L02_00160570[1] = {0xf0};

s32 D_L03_00160540 = 0;
s32 D_L03_00160570[1] = {0xf0};

s32 D_L04_00160540 = 0;
s32 D_L04_00160570[1] = {0xf0};

s32 D_L05_00160540 = 0;
s32 D_L05_00160570[1] = {0xf0};

s32 D_L06_00160540 = 0;
s32 D_L06_00160570[1] = {0xf0};

s32 D_L07_00160540 = 0;
s32 D_L07_00160570[1] = {0xf0};

s32 D_L08_00160540 = 0;
s32 D_L08_00160570[1] = {0xf0};

s32 D_L09_00160540 = 0;
s32 D_L09_00160570[1] = {0xf0};

s32 D_L10_00160540 = 0;
s32 D_L10_00160570[1] = {0xf0};

s32 D_L11_00160540 = 0;
s32 D_L11_00160570[1] = {0xf0};

s32 D_L12_00160540 = 0;
s32 D_L12_00160570[1] = {0xf0};

s32 D_L13_00160540 = 0;
s32 D_L13_00160570[1] = {0xf0};

s32 D_L14_00160540 = 0;
s32 D_L14_00160570[1] = {0xf0};

s32 D_L15_00160540 = 0;
s32 D_L15_00160570[1] = {0xf0};

s32 D_L16_00160540 = 0;
s32 D_L16_00160570[1] = {0xf0};

s32 D_L17_00160540 = 0;
s32 D_L17_00160570[1] = {0xf0};

s32 D_L18_00160540 = 0;
s32 D_L18_00160570[1] = {0xf0};

s32 transition_sequence_positions[1] __asm__("D_L00_00160570") = {0xf0};
