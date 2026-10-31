/* Level 14 moby class unfolding_column (0x573): once its trigger moby reaches state 2, unfolds its six pieces (class 0x575) one by one, then swings the column on its pivot (class 0x574, created here) and releases the pieces. */
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/overlay/shared/mobys/evaluate_moby_animation_pose.h"
#include "types.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"

u8 *create_unfolding_column_pivot(u8 *self) __asm__("FUN_L14_00306158");

typedef struct UnfoldingColumnMoby {
    char                        pad0[0x10];
    float                       pos[4]; /* 0x10 */
    unsigned char               state;  /* 0x20 */
    char                        pad21[0x23];
    float                       f44; /* 0x44 */
    char                        pad48[0x30];
    struct UnfoldingColumnVars *data; /* 0x78 */
    char                        pad7C[0x44];
    float                       rot[4]; /* 0xC0 */
    char                        padD0[0x10];
    float                       fE0[4]; /* 0xE0 */
    char                        padF0[0x10];
} UnfoldingColumnMoby;

typedef struct UnfoldingColumnPieces {
    UnfoldingColumnMoby *m[6];
} UnfoldingColumnPieces;

typedef struct UnfoldingColumnVars {
    UnfoldingColumnMoby  *pivot;   /* 0x00 */
    UnfoldingColumnPieces pieces;  /* 0x04 */
    int                   trigger; /* 0x1C: index of the moby that starts it */
    short                 timer;   /* 0x20 */
    short                 cur;     /* 0x22 */
    float                 rate;    /* 0x24 */
    int                   idx[6];  /* 0x28 */
    int                   voice;   /* 0x40 */
} UnfoldingColumnVars;

typedef struct Snd {
    char                 pad0[0x74];
    unsigned char        f74;
    char                 pad75[0x13];
    UnfoldingColumnMoby *f88;
} Snd;

extern float interpolate_angle(float, float, float) __asm__("FUN_L00_0025b750");

extern int D_L14_0015FFD8_c __asm__("D_L14_0015FFD8");

extern float unfolding_column_initial_position_offset_scale __asm__("D_L14_001621F4")
    __attribute__((sda));

float unfolding_column_piece_spacing __asm__("D_L14_001621F8") = 1.05f;

float unfolding_column_first_piece_offset __asm__("D_L14_001621FC") = 0;

extern float unfolding_column_pivot_final_angle_degrees __asm__("D_L14_00162200")
    __attribute__((sda));

extern int unfolding_column_piece_unfold_frames __asm__("D_L14_00162204") __attribute__((sda));

extern int unfolding_column_pivot_swing_frames __asm__("D_L14_0016220C") __attribute__((sda));

f32 interpolate_scalar_linear(f32 a, f32 b, f32 t) __asm__("FUN_L00_0025b6a8");

int voice_is_playing(int a, int i) __asm__("FUN_L00_0028d8c0");

int trooper_play_sound(int i, int a1, int v, int k) __asm__("FUN_L00_0028dc90");

extern float interpolate_scalar_linear(float, float, float) __asm__("FUN_L00_0025b6a8");

void unfolding_column_update(UnfoldingColumnMoby *moby) __asm__("FUN_L14_00305d28");

/* Moby class 1395 */
void unfolding_column_update(UnfoldingColumnMoby *moby) {
    enum {
        UNFOLDING_COLUMN_INITIALIZE = 0,
        UNFOLDING_COLUMN_WAIT_FOR_TRIGGER = 1,
        UNFOLDING_COLUMN_UNFOLD_PIECES = 2,
        UNFOLDING_COLUMN_SWING_PIVOT = 3
    };
    UnfoldingColumnVars *data = moby->data;
    float                position_offset[4];

    switch (moby->state) {
    case UNFOLDING_COLUMN_INITIALIZE: {
        int i;
        if (data->pivot == 0) {
            data->pivot = create_unfolding_column_pivot(moby);
            fast_vec_scale(
                position_offset, moby->rot, unfolding_column_initial_position_offset_scale);
            fast_vec_add(data->pivot->pos, moby->pos, position_offset);
        }
        for (i = 0; i < 6; i++) {
            if (data->pieces.m[i] == 0) {
                data->pieces.m[i] = (UnfoldingColumnMoby *)(D_L14_0015FFD8_c + (data->idx[i] << 8));
                fast_vec_scale(
                    position_offset, moby->rot, unfolding_column_initial_position_offset_scale);
                fast_vec_add(data->pieces.m[i]->pos, moby->pos, position_offset);
                data->pieces.m[i]->pos[2] =
                    data->pieces.m[i]->pos[2] + (unfolding_column_piece_spacing * (float)i +
                                                 unfolding_column_first_piece_offset);
            }
        }
        data->voice = -1;
        if (data->pivot != 0) {
            moby->state = 1;
        }
        break;
    }
    case UNFOLDING_COLUMN_WAIT_FOR_TRIGGER:
        if (((UnfoldingColumnMoby *)(D_L14_0015FFD8_c + (data->trigger << 8)))->state == 2) {
            moby->state = 2;
            data->cur = 5;
            data->timer = scale_game_frames(unfolding_column_piece_unfold_frames);
            data->rate = 1.0f / convert_integer_to_float(data->timer);
            trooper_play_sound(0, 0, data->pieces.m[data->cur], 0x575);
        }
        break;
    case UNFOLDING_COLUMN_UNFOLD_PIECES: {
        UnfoldingColumnPieces *pieces = &data->pieces;
        UnfoldingColumnMoby   *current_piece = pieces->m[data->cur];
        fast_dec_timer_s16(&data->timer);
        current_piece->f44 =
            interpolate_angle(1.5707964f, 0.0f, convert_integer_to_float(data->timer) * data->rate);
        if (data->timer == 0) {
            data->timer = scale_game_frames(unfolding_column_piece_unfold_frames);
            data->cur--;
            if (data->cur < 0) {
                moby->state = 3;
                data->timer = scale_game_frames(unfolding_column_pivot_swing_frames);
                data->rate = 1.0f / convert_integer_to_float(data->timer);
            } else {
                trooper_play_sound(0, 0, pieces->m[data->cur], 0x575);
            }
        }
        break;
    }
    case UNFOLDING_COLUMN_SWING_PIVOT: {
        UnfoldingColumnMoby  *pivot = data->pivot;
        UnfoldingColumnMoby **pieces;
        int                   i;
        float                 remaining_timer_fraction;
        if (!voice_is_playing(moby, data->voice)) {
            data->voice = allocate_voice_for_target_entry(0, 4, moby);
        }
        fast_dec_timer_s16(&data->timer);
        pieces = data->pieces.m;
        remaining_timer_fraction = convert_integer_to_float(data->timer) * data->rate;
        pivot->f44 =
            interpolate_scalar_linear(unfolding_column_pivot_final_angle_degrees * 0.017453292f,
                                      0.0f,
                                      remaining_timer_fraction);
        for (i = 0; i < 6; i++) {
            fast_vec_scale(position_offset,
                           pivot->fE0,
                           unfolding_column_piece_spacing * (float)i +
                               unfolding_column_first_piece_offset);
            fast_vec_add(pieces[i]->pos, pivot->pos, position_offset);
        }
        if (data->timer == 0) {
            int j;
            if (data->voice != -1) {
                Snd *voice_entry = (Snd *)((char *)&voice_pool + data->voice * 0x70);
                if (voice_entry->f88 == moby && voice_entry->f74) {
                    release_voice_slot(data->voice);
                }
            }
            data->voice = -1;
            moby->state = 4;
            for (j = 0; j < 6; j++) {
                pieces[j]->state = 2;
            }
        }
        break;
    }
    }
}

u8 *create_unfolding_column_pivot(u8 *source_moby) {
    u8 *created_moby = create_moby(0x574);

    if (created_moby != 0) {
        qcopy(created_moby + 0x10, source_moby + 0x10);
        qcopy(created_moby + 0x40, source_moby + 0x40);
        created_moby[0x30] = 0x40;
        created_moby[0x20] = 0;
        MOBY(created_moby)->draw_dist = 0x40;
        created_moby[0x31] = 1;
        update_entity(created_moby);
    }
    return created_moby;
}

int unfolding_column_piece_unfold_frames __asm__("D_L14_00162204") = 0xf;

int unfolding_column_pivot_swing_frames __asm__("D_L14_0016220C") = 0x78;

float unfolding_column_initial_position_offset_scale __asm__("D_L14_001621F4") = 0.48f;

float unfolding_column_pivot_final_angle_degrees __asm__("D_L14_00162200") = 182.0f;
