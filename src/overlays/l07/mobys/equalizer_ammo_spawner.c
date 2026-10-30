/* Level 07 moby class 0x476: equalizer ammo spawner; spawns batches of the listed ammo pickups along its path while their stock is low. */
#include "types.h"
#include "asm.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/mobys/moby_spawn.h"
#include "rnc/rendering/clip_volume.h"
#include "rnc/mobys/moby.h"

extern void  debug_print(const char *, ...) __asm__("FUN_001e93b0");
extern int   sample_path(int *, float *, int *, float *, float, int) __asm__("FUN_L00_0025d808");
extern void  spawn_on_path(float *, int, int, int) __asm__("FUN_L00_002c52a8");
extern int   fptodp(float) __asm__("FUN_00120478");
extern char *D_L07_001B0530[];
extern char  equalizer_ammo_spawner_missing_trigger_message[] __asm__("D_L07_00211AF0");
extern char  D_L07_00211B30[];
extern char  equalizer_ammo_spawn_counts_format[] __asm__("D_L07_00211B50");
extern char  D_L07_00211B78[];
extern char  equalizer_ammo_batch_reduced_format[] __asm__("D_L07_00211BA8");
/* Item id list ended by -1; sized so Ps2EeAs reaches it through $gp like retail. */
int D_L07_00161CA0[2] = {0xa, 0xf};

extern char D_0013F3D0[];

typedef struct {
    float pos[3];
    float length;
} L07PathNode;

typedef struct {
    char           pad[0xe];
    unsigned short scale;
    char           tail[8];
} L07ScaleRow;

extern L07ScaleRow l07_ammo_spawner_scaling_rows[] __asm__("D_L07_001C4130");

/* Ported from rac1-decomp nonmatching/l07_umbris/func_L07_0031E6B0.c (func_L07_0031E6B0) */
/* Umbris item spawner: measures its path once, then for each listed item whose stock ratio is in
 * range spawns batches of pickups along the path, away from the target moby. */
void update_equalizer_ammo_spawner(struct Moby *moby) __asm__("FUN_L07_0031d2e0");

void update_equalizer_ammo_spawner(struct Moby *moby) {
    int *data = ((int *)moby->pvars);
    int  state = moby->state;
    if (state == 1)
        goto state_one;
    if (state >= 2)
        return;
    if (state != 0)
        return;
    {
        int bad = 0;
        int i;
        if (data[0] == -1) {
            debug_print(equalizer_ammo_spawner_missing_trigger_message);
            bad = 1;
        }
        if (data[1] == -1) {
            debug_print(equalizer_ammo_spawner_missing_trigger_message);
            bad = 1;
        }
        if (bad) {
            debug_print(D_L07_00211B30, ((short)moby->save_id));
            mark_moby_for_removal(moby);
            return;
        }
        *(float *)(data + 8) = 0.0f;
        for (i = 0; i < *(int *)D_L07_001B0530[data[1]]; i++) {
            L07PathNode *nodes = (L07PathNode *)D_L07_001B0530[data[1]];
            ((L07PathNode *)(D_L07_001B0530[data[1]] + (i << 4)))[1].length =
                fast_vec_dist(&nodes[i + 1], &nodes[(i + 1) % *(int *)nodes + 1]);
            *(float *)(data + 8) += ((L07PathNode *)(D_L07_001B0530[data[1]] + (i << 4)))[1].length;
        }
        moby->state = 1;
    }
    return;
state_one: {
    int i;
    if (is_point_inside_clip_volume(D_0013F3D0, data[0]) == 0)
        return;
    for (i = 0; D_L07_00161CA0[i] != -1; i++) {
        int   id = D_L07_00161CA0[i];
        float value = convert_integer_to_float(weapon_ammo_counts[id]);
        float scale = convert_integer_to_float(
            ((L07ScaleRow *)((char *)l07_ammo_spawner_scaling_rows + id * 0x18))->scale);
        float ratio = value / scale;
        float step = random_float_between(0.0f, *(float *)(data + 8));
        float min_ratio = *(float *)(data + 2) / 100.0f;
        if (min_ratio <= ratio && ratio < *(float *)(data + 3) / 100.0f) {
            float amount;
            float lower = scale * (*(float *)(data + 4) / 100.0f) - value;
            float upper = scale * (*(float *)(data + 5) / 100.0f);
            int   total;
            int   batch;
            int   remaining;
            int   quotient;
            float position[4];
            float fraction;
            int   segment;
            char *path;
            if (data[6] == 0)
                amount = lower < upper ? lower : upper;
            else
                amount = upper < lower ? lower : upper;
            total = truncate_float_to_s32(amount + 0.5f);
            quotient = total / data[7];
            batch = total;
            fraction = 0.0f;
            segment = 0;
            path = D_L07_001B0530[data[1]];
            if (quotient != 0)
                batch = quotient;
            debug_print(equalizer_ammo_spawn_counts_format, total, batch);
            if (total > 0) {
                float near = 12.0f;
                float retry_step = 10.0f;
                do {
                    int accepted;
                    remaining = total - batch;
                    accepted = 0;
                    do {
                        sample_path((int *)path, position, &segment, &fraction, step, 1);
                        if (data[9] == -1)
                            accepted = 1;
                        else if (near <
                                 vector_distance_xy(level_mobys + data[9] * 0x100 + 0x10, position))
                            accepted = 1;
                        else
                            step += retry_step;
                    } while (accepted == 0);
                    total = remaining;
                    debug_print(D_L07_00211B78,
                                id,
                                batch,
                                fptodp(position[0]),
                                fptodp(position[1]),
                                fptodp(position[2]),
                                fptodp(step));
                    spawn_on_path(position, id, batch, 0);
                    step += random_float_between(*(float *)(data + 8) / retry_step, 2.0f);
                    if (remaining < batch) {
                        batch = remaining;
                        debug_print(equalizer_ammo_batch_reduced_format, remaining);
                    }
                } while (remaining > 0);
            }
        }
    }
    moby->state = 2;
}
}

char D_L07_00211B30[] = "ERRORS, killing moby %d \n";

char D_L07_00211B78[] = " spawn %d of %d at %3.3f,%3.3f,%3.3f dist:%f \n";
