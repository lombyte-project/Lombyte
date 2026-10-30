/* Level 03 moby class path_mover_spawner: spawns path_mover mobys along its path. */
#include "types.h"
#include "rnc/overlay/level_path.h"
#include "rnc/globals.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/mobys/moby_spawn.h"
#include "qcopy.h"

extern void update_entity(void *) __asm__("FUN_L00_00250df8");

extern void copy_hero_moby_field_38(struct Moby *) __asm__("FUN_L00_0025d1b8");

/* Spawns a path mover (class 0x382, moved by FUN_L03_002db020) on path, dist
 * units along it, facing the path's last point. */
struct Moby        *spawn_path_mover(LevelPath *path,
                                     float      initial_path_distance) __asm__("FUN_L03_002db198");
extern struct Moby *spawn_path_mover(LevelPath *, float) __asm__("FUN_L03_002db198");

struct Moby *spawn_path_mover(LevelPath *path, float initial_path_distance) {
    struct Moby *spawned_moby = create_moby(0x382);
    u8          *moby_vars;
    u8          *move;
    Vec4        *end;

    if (spawned_moby != 0) {
        spawned_moby->update_dist = 0x80;
        spawned_moby->draw_dist = 0x40;
        spawned_moby->visible = 1;
        spawned_moby->state = 0;
        moby_vars = spawned_moby->pvars;
        spawned_moby->flags |= 0x20;
        copy_hero_moby_field_38(spawned_moby);
        qcopy(&spawned_moby->pos, &path->points[0]);
        clear_u64_value(&spawned_moby->rot);
        end = (Vec4 *)((u8 *)path + (path->count - 1) * 16) + 1;
        spawned_moby->rot.z =
            fast_arc_tan(end->f[0] - spawned_moby->pos.x, end->f[1] - spawned_moby->pos.y);
        move = moby_vars + 0x20;
        *(float *)(moby_vars + 0x64) = initial_path_distance;
        *(LevelPath **)(moby_vars + 0x60) = path;
        *(u8 **)(moby_vars + 0x8) = move;
        update_entity(spawned_moby);
    }
    return spawned_moby;
}

#include "rnc/overlay/l03/mobys/path_mover_globals.h"

struct RailSpawnerVars {
    s32 path; /* index into D_L03_001B05B0, -1 for none */
    s32 timer;
    s32 voice;
};

struct RailSpawnerMoby {
    u8                      pad0[0x20];
    u8                      state;
    u8                      pad21[0xF];
    u8                      unk30;
    u8                      pad31[0x47];
    struct RailSpawnerVars *vars; /* 0x78 */
    u8                      pad7C[0x2C];
    u16                     uid; /* 0xA8 */
};

extern char path_mover_spawner_bad_path_format[] __asm__("D_L03_001E35F0");
extern void debug_print(char *, int) __asm__("FUN_001e93b0");
extern int  voice_is_playing(struct RailSpawnerMoby *, int) __asm__("FUN_L00_0028d8c0");
extern int  FUN_L00_0028dc90(int, int, struct RailSpawnerMoby *, int);

void update_path_mover_spawner(struct RailSpawnerMoby *moby) __asm__("FUN_L03_002db280");

void update_path_mover_spawner(struct RailSpawnerMoby *moby) {
    struct RailSpawnerVars *vars = moby->vars;
    LevelPath              *path;
    float                   spawn_distance;
    int                     i;

    if (moby->state == 0) {
        if (vars->path == -1) {
            debug_print(path_mover_spawner_bad_path_format, moby->uid);
            mark_moby_for_removal((char *)moby);
            return;
        }
        path = l03_paths[vars->path];
        path_mover_distance_step = frame_time * 3.0f;
        for (i = 0; i < path->count - 2; i++)
            path->points[i].f[3] = fast_vec_dist(&path->points[i], &path->points[i + 1]);
        vars->timer = 0;
        moby->state = 1;
        vars->voice = -1;
        for (spawn_distance = 2.9f; spawn_distance < path->count * path->points[0].f[3];
             spawn_distance += 2.9f)
            spawn_path_mover(path, spawn_distance);
        moby->unk30 = 0x60;
    }
    if (fast_dec_timer(&vars->timer)) {
        vars->timer = truncate_float_to_s32(2.9f / path_mover_distance_step);
        spawn_path_mover(l03_paths[vars->path], 0.0f);
    }
    if (!voice_is_playing(moby, vars->voice))
        vars->voice = FUN_L00_0028dc90(0, 4, moby, 0x382);
}
