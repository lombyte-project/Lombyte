/* Level 10 moby class jitter_strand_grid (class 1073): four groups of jittering strands. */
#include "types.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "asm.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/shared/math/random/random_vector.h"
#include "rnc/mobys/moby_spawn.h"

#include "rnc/math/vector.h"

extern unsigned char D_001413F4[];
extern s32           D_L10_00161D64;
extern s32           D_L10_00161D68;

extern int   D_L10_00161D64;
extern int   D_L10_00161D68;
extern float D_L10_00161D54 __attribute__((sda));
float        jitter_strand_row_y_span_gp __asm__("D_L10_00161D58") = 2.5f;
float        jitter_strand_phase_advance_rate_gp __asm__("D_L10_00161D5C") = 2.0f;

typedef struct {
    Vec4 pos[4][30][20];
    Vec4 vel[4][30][20];
    int  timers[4][30];
    f32  phase;
} PlatformWaveVars;

void jitter_strand_grid_update(struct Moby *moby) __asm__("FUN_L10_002dcc58");

/* Moby class 1073 */
void jitter_strand_grid_update(struct Moby *moby) {
    enum {
        JITTER_STRAND_GRID_STATE_INITIALIZE = 0,
        JITTER_STRAND_GRID_STATE_UPDATE = 1
    };
    PlatformWaveVars *jitter_strand_grid_vars = (PlatformWaveVars *)moby->pvars;
    Vec4              jitter[20][10];

    switch (moby->state) {
    case JITTER_STRAND_GRID_STATE_INITIALIZE:
        if (current_level_index == 10 && D_001413F4[0] == 1) {
            mark_moby_for_removal(moby);
            break;
        }
        D_L10_00161D68 = 22;
        D_L10_00161D64 = 45;
        {
            int i;
            for (i = 0; i < 10; i++) {
                int start = i * D_L10_00161D64 / 10;
                jitter_strand_grid_vars->timers[0][i] = start;
                jitter_strand_grid_vars->timers[1][i] = start + 11;
                jitter_strand_grid_vars->timers[2][i] = start + 23;
                jitter_strand_grid_vars->timers[3][i] = start + 34;
            }
        }
        moby->state = 1;
        break;
    case JITTER_STRAND_GRID_STATE_UPDATE: {
        int group, strand_column, row;

        jitter_strand_grid_vars->phase += jitter_strand_phase_advance_rate_gp * frame_time;
        if (jitter_strand_grid_vars->phase > 1.0f)
            jitter_strand_grid_vars->phase -= 1.0f;
        for (group = 0; group < 4; group++) {
            for (strand_column = 0; strand_column < 10; strand_column++) {
                if (fast_dec_timer(&jitter_strand_grid_vars->timers[group][strand_column])) {
                    for (row = 0; row < 20; row++) {
                        clear_u64_value(&jitter_strand_grid_vars->pos[group][row][strand_column]);
                        jitter_strand_grid_vars->pos[group][row][strand_column].f[1] =
                            row * (jitter_strand_row_y_span_gp / 20.0f) -
                            jitter_strand_row_y_span_gp * 0.5f;
                        jitter_strand_grid_vars->pos[group][row][strand_column].f[3] = 1.0f;
                        random_vector(
                            &jitter[row][strand_column], 0.0f, D_L10_00161D54 * frame_time);
                    }
                    for (row = 1; row < 19; row++) {
                        fast_vec_add(&jitter_strand_grid_vars->vel[group][row][strand_column],
                                     &jitter[row - 1][strand_column],
                                     &jitter[row][strand_column]);
                        fast_vec_add(&jitter_strand_grid_vars->vel[group][row][strand_column],
                                     &jitter_strand_grid_vars->vel[group][row][strand_column],
                                     &jitter[row + 1][strand_column]);
                        fast_vec_scale(&jitter_strand_grid_vars->vel[group][row][strand_column],
                                       &jitter_strand_grid_vars->vel[group][row][strand_column],
                                       0.333f);
                    }
                    jitter_strand_grid_vars->timers[group][strand_column] = D_L10_00161D64;
                } else {
                    int k;
                    for (k = 1; k < 19; k++) {
                        int row = k;
                        fast_vec_add(&jitter_strand_grid_vars->pos[group][row][strand_column],
                                     &jitter_strand_grid_vars->pos[group][row][strand_column],
                                     &jitter_strand_grid_vars->vel[group][row][strand_column]);
                    }
                }
            }
        }
        break;
    }
    }
}
