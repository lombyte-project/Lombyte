/* Hero: primary item input, item slot selection, water and ground transitions, and the hoverboard tricks, race progress and respawn. */
#include "types.h"
#include "rnc/overlay/hero_tables.h"
#include "qcopy.h"
#include "rnc/overlay/collision.h"
#include "rnc/hero/hero.h"
#include "asm.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/fast_math.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/world/collision/ground_probe.h"
#include "rnc/audio/voices/allocate_voice_for_bank_entry.h"
#include "rnc/overlay/shared/audio/voices/voice_is_playing.h"
#include "rnc/overlay/shared/mobys/select_auto_aim_target_moby.h"
#include "rnc/overlay/shared/mobys/moby_point_position.h"
#include "rnc/overlay/shared/hero/hero_switch_item.h"
#include "rnc/overlay/shared/hero/hero_move_state_velocity.h"
#include "rnc/overlay/shared/hero/set_hero_animation.h"
#include "rnc/overlay/shared/math/random/random_int_between.h"
#include "rnc/overlay/shared/rendering/effects/spawn_explosion.h"
#include "rnc/overlay/shared/ui/help/help_request.h"
#include "rnc/mobys/animation/blend_moby_animation.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/overlay/shared/input/pad_pressed_within.h"
#include "rnc/overlay/shared/math/interpolation/spring_turn.h"
#include "rnc/overlay/shared/rendering/effects/spawn_type_10_particle.h"
#include "rnc/world/collision/collision_hit_material.h"
#include "rnc/overlay/l05/hero/hero_set_state.h"
#include "rnc/overlay/shared/vendor/vendor_menu.h"
#include "rnc/overlay/shared/math/vu0_vector_ops.h"
#include "rnc/game/mode_freeze_init.h"

/* Steps two floats of arg toward a target and clamps them to [-1, 1]. */
/* Ported from rac1-decomp (src/overlays/shared/help_00237B00.c: func_L05_00237B00), where it is exact; names translated to the US level program. */

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");

#include "rnc/data_labels.h"

int integrate_hero_motion_with_collisions() __asm__("FUN_L05_002413e0");

void smooth_and_clamp_value_pair(float *out,
                                 float *smoothed_values,
                                 float  step) __asm__("FUN_L05_00237488");

void smooth_and_clamp_value_pair(float *out, float *smoothed_values, float step) {
    char *target_values_base = (char *)((unsigned char *)&D_0013E533) + 0xE1D;
    float target_values[2];
    target_values[0] = -*(float *)(target_values_base + 0x1D20);
    target_values[1] = *(float *)(target_values_base + 0x1D24);
    approach_value(smoothed_values + 2, target_values[0], step);
    if (smoothed_values[2] > 1.0f)
        smoothed_values[2] = 1.0f;
    if (smoothed_values[2] < -1.0f)
        smoothed_values[2] = -1.0f;
    out[0] = smoothed_values[2];
    approach_value(smoothed_values + 1, target_values[1], step);
    if (smoothed_values[1] > 1.0f)
        smoothed_values[1] = 1.0f;
    if (smoothed_values[1] < -1.0f)
        smoothed_values[1] = -1.0f;
    out[1] = smoothed_values[1];
}

#include "sda.h"
#include "rnc/overlay/level_path.h"

int hero_anim_tilt_lookup(int id, float *out) __asm__("FUN_L05_0023b378");

int hero_anim_tilt_lookup(int id, float *out) {
    *out = 0.0f;
    if (hero.state.control_mode == 0x16 && hero.race_ammo &&
        (unsigned)(*(unsigned char *)(((char *)hero.moby) + 0x53) - 0x69) < 4)
        return 1;
    switch (id) {
    case 0x37:
        *out = -0.95993108f; /* -55 degrees */
        return 1;
    case 0x1C:
    case 0x1D:
    case 0x1F:
        if (25.0f < hero.anim_frame)
            *out = -1.30899692f; /* -75 degrees */
        return 1;
    case 0x31:
    case 0x32:
    case 0x4A:
    case 0x4B:
    case 0x50:
    case 0x60:
    case 0x6D:
    case 0x6E:
        return 1;
    }
    return 0;
}

/* Applies the wrench/item selection for slot i: resolves pending, queued and requested items and refreshes the slot when it changes. */
/* Ported from rac1-decomp (src/overlays/shared/help_0020CDF0.c: func_L00_00210558), where it is exact; names translated to the US level program. */

/* L05 adds to hero_switch_item the state.control_mode / unk8CE test that guards the second
   unk20A6 set, and a new unguarded statement (the prologue's i!=0 branch lands
   on it, so it runs for every slot) that sets unk20A6 for items[0].item_id == 0x24 with
   state.control_mode != 0x16. The second condition is negated: retail's `beq` skips the
   store when state.control_mode == 0x16, which is what makes this spelling the only one
   that lands `beq` there. */

typedef struct {
    char pad[0x20];
    int  unk20;
    char pad2[0x28];
} ItemDef;

extern char    D_0013C940[];
extern char    D_00141660[];
extern u8      item_available_lui[35] __asm__("D_0013D4C0") NOT_SDA;
extern int     D_0015ED8C;
extern int     D_0015ED90;
extern int     D_0015ED94;
extern ItemDef D_L05_00179F20[];
extern int     hero_is_in_special_state() __asm__("FUN_L00_0020d568");
extern void    clear_hero_idle_rotation_targets(void) __asm__("FUN_L00_0020a500");
extern void    replenish_hero_drones(void) __asm__("FUN_L00_002cdcc0");
s32            can_select_hero_item_31(void) __asm__("FUN_L00_0020fe30");
void           set_hero_condition_animation_timer_for_item(int id) __asm__("FUN_L00_0020fd80");
void           set_hero_item_switch_timer_for_item(int id) __asm__("FUN_L00_0020fde0");

void apply_hero_item_slot_selection(int i) __asm__("FUN_L05_0023dc88");

void apply_hero_item_slot_selection(int i) {
    int           changed;
    int           flag6;
    int           flag4;
    unsigned char item_switch_gate_state;

    if (i == 0) {
        int          v;
        struct Hero *g = &hero;
        v = g->items[0].item_id;
        if (v != 8 && v != 0x17 && v != 9 && g->items[0].switch_lock == 2) {
            g->items[0].switch_lock = 0;
        }
    }
    if (i == 0) {
        struct Hero *g = &hero;
        if (g->item_select_blocked != 0 && *(int *)((char *)g + 0x10B8) != 8) {
            if (g->state.control_mode != 0x16 || g->race_ammo == 0) {
                g->force_slot0_item8 = 1;
            }
        }
    }
    {
        struct Hero *g = &hero;
        if (g->items[0].item_id == 0x24 && g->state.control_mode != 0x16) {
            g->force_slot0_item8 = 1;
        }
    }
    if (i == 0) {
        struct Hero *g = &hero;
        if (g->item_select_blocked != 0 && *(int *)((char *)g + 0x10B8) == 8) {
            if (g->pending_item[0] == 0x1F) {
                g->pending_item[0] = 0;
            }
            if (g->pending_item[0] != 0x18 && g->pending_item[0] != 0x24) {
                return;
            }
        }
    }
    {
        struct Hero *g = &hero;
        if (g->item_use_locked != 0 && i == 0) {
            if (g->force_slot0_item8 == 0 || *(int *)((char *)g + 0x10B8) == 8) {
                g->force_slot0_item8 = 0;
                return;
            }
        }
    }
    changed = 0;
    {
        char *pad = D_0013C940;
        if ((*(int *)(pad + 0x1A4) & 0x80) || (&hero)->force_slot0_item8 != 0) {
            if (i == 0) {
                int          v;
                struct Hero *g = &hero;
                v = *(int *)((char *)g + 0x10B8);
                if (v != 8) {
                    if (g->force_slot0_item8 != 0 && v != 0 && v != 0x1F) {
                        g->saved_item[0] = v;
                    }
                    (&hero)->selected_item[i] = 8;
                    changed = 1;
                }
            }
        }
    }
    if (i == 0) {
        char *pad = D_0013C940;
        if (*(int *)(pad + 0x1A4) & 0x20) {
            struct Hero *g = &hero;
            if (g->item_select_blocked == 0) {
                if (g->items[0].item_id != *(int *)(D_00141660) && *(int *)(D_00141660) != 0) {
                    g->selected_item[0] = *(int *)(D_00141660);
                    changed = 1;
                }
            }
        }
    }
    if (i == 1) {
        struct Hero *g;
        if (hero_is_in_special_state() || (&hero)->state.current == 0x12) {
            int v;
            g = &hero;
            v = *(int *)((char *)g + 0x1108);
            if (v != 0) {
                g->saved_item[1] = v;
                g->pending_item[1] = 0x26;
            }
        }
        g = &hero;
        if (g->state.control_mode == 0xF && item_available_lui[0x1D] != 0) {
            if (*(int *)(D_00141660 + 4) != 0x1D || *(int *)((char *)g + i * 0x50 + 0x10B8) == 0) {
                int v;
                g->pending_item[1] = 0x1D;
                v = *(int *)((char *)g + i * 0x50 + 0x10B8);
                if (v != 0) {
                    g->saved_item[i] = v;
                } else {
                    g->saved_item[i] = 0x26;
                }
            }
        }
        g = &hero;
        if (*(unsigned char *)((char *)g + 0x12E7) != 0 && item_available_lui[0x1C] != 0) {
            if (*(int *)(D_00141660 + 4) != 0x1C || *(int *)((char *)g + i * 0x50 + 0x10B8) == 0) {
                int v;
                g->pending_item[1] = 0x1C;
                v = *(int *)((char *)g + i * 0x50 + 0x10B8);
                if (v != 0) {
                    g->saved_item[i] = v;
                } else {
                    g->saved_item[i] = 0x26;
                }
            }
        }
    }
    flag6 = 0;
    if (i == 2) {
        struct Hero *g = &hero;
        struct Hero *h;
        struct Hero *equipped_item_check_hero;
        if (g->state.control_mode == 0x11 || g->state.current == 0x76 || g->state.current == 0x6A ||
            g->state.current == 0x82 || *(unsigned char *)((char *)g + 0x22CB) != 0) {
            if (((int *)(D_00141660))[i] != 6 ||
                (equipped_item_check_hero = &hero,
                 *(int *)((char *)equipped_item_check_hero + i * 0x50 + 0x10B8) == 0)) {
                h = &hero;
                if (*(int *)((char *)h + i * 0x50 + 0x10B8) != 6 && h->selected_item[i] != 6 &&
                    item_available_lui[6] != 0) {
                    int v;
                    h->pending_item[i] = 6;
                    flag6 = 1;
                    v = *(int *)((char *)h + i * 0x50 + 0x10B8);
                    if (v != 0) {
                        h->saved_item[i] = v;
                    } else {
                        h->saved_item[i] = 0x26;
                    }
                }
            }
        } else if (*(int *)((char *)g + 0x1158) == 6 && g->saved_item[2] != 0) {
            g->restore_item[2] = 1;
        }
    }
    flag4 = 0;
    if (i == 3) {
        struct Hero *g;
        if (hero_is_in_special_state() || (&hero)->state.current == 0x12) {
            if (*(int *)(D_00141660 + 0xC) != 4) {
                int v;
                g = &hero;
                v = *(int *)((char *)g + 0x11A8);
                if (v != 4 && item_available_lui[4] != 0) {
                    g->saved_item[3] = v;
                    flag4 = 1;
                    g->pending_item[3] = 4;
                }
            }
        } else if ((&hero)->items[3].item_id == 4 && (&hero)->saved_item[3] != 0) {
            (&hero)->restore_item[3] = 1;
        }
        if ((&hero)->state.current == 8 && ((int *)(D_00141660))[i] == 4) {
            (&hero)->pending_item[i] = 2;
        }
    }
    {
        struct Hero *g = &hero;
        int         *qb = g->restore_item;
        int         *restore_item_flag = qb + i;
        if (*restore_item_flag != 0) {
            int *r;
            int *rb;
            int  restored_item_id;
            *restore_item_flag = 0;
            rb = g->saved_item;
            r = rb + i;
            restored_item_id = *r;
            if (restored_item_id != 0 &&
                restored_item_id != *(int *)((char *)g + i * 0x50 + 0x10B8)) {
                if (restored_item_id == 0x26) {
                    restored_item_id = 0;
                }
                *r = 0;
                ((int *)(D_00141660))[i] = restored_item_id;
                g->selected_item[i] = restored_item_id;
                if (i == 0) {
                    D_L05_00179F20[g->selected_item[0]].unk20 = 0x20;
                }
                changed = 1;
            }
        }
    }
    {
        struct Hero *g = &hero;
        int         *s;
        int         *sb;
        if (*(unsigned char *)((char *)g + 0x1FF5) != 0) {
            replenish_hero_drones();
            g->pending_item[i] = 0;
        }
        sb = g->pending_item;
        s = sb + i;
        if (*s == 0x1F) {
            if (can_select_hero_item_31()) {
                int v;
                if (*(short *)((char *)g + 0x22DE) == 0) {
                    allocate_hero_voice_for_entry(0x18, 0);
                    *(short *)((char *)g + 0x22DE) = 0x12;
                    *(short *)((char *)g + 0x22DC) = 1;
                }
                changed = 1;
                g->selected_item[i] = *s;
                v = ((int *)(D_00141660))[i];
                ((int *)(D_00141660))[i] = *s;
                D_0015ED8C = v;
            }
            (&hero)->pending_item[i] = 0;
        }
    }
    {
        int          v;
        struct Hero *g = &hero;
        int         *ab = g->pending_item;
        int         *pending_item_entry = ab + i;
        v = *pending_item_entry;
        if (v != 0) {
            int *bb = g->selected_item;
            int *b = bb + i;
            if (v != *b) {
                if (v == 0x26) {
                    *b = 0;
                    ((int *)(D_00141660))[i] = 0;
                } else {
                    struct Hero *h;
                    int          u;
                    if (i == 0 && g->pending_item[0] != 0x24 && *(int *)((char *)g + 0x10B8) != 8) {
                        D_0015ED8C = *(int *)(D_00141660);
                    }
                    h = &hero;
                    u = h->pending_item[i];
                    h->selected_item[i] = u;
                    if (u != 0x24 && (flag6 == 0 || u != 6) && (flag4 == 0 || u != 4)) {
                        ((int *)(D_00141660))[i] = u;
                    }
                    if (i == 0) {
                        struct Hero *k = &hero;
                        D_L05_00179F20[k->selected_item[0]].unk20 = 0x20;
                    }
                }
                changed = 1;
            } else {
                *pending_item_entry = 0;
            }
        }
    }
    {
        struct Hero *s = (struct Hero *)(((char *)&hero) + i * sizeof(struct HeroItemSlot));
        item_switch_gate_state = s->items[0].switch_lock;
        if (item_switch_gate_state == 2) {
            changed = 0;
        }
        if (item_switch_gate_state == 1 && s->items[0].timer == 0) {
            s->items[0].timer = s->items[0].timer_reload;
        }
    }
    if (fast_dec_timer_s16(&hero.items[i].timer) == 0) {
        changed = 0;
    }
    if (changed) {
        struct Hero *g = &hero;
        *(int *)((char *)g + i * 0x50 + 0x10B0) = 3;
        clear_hero_idle_rotation_targets();
        *(int *)((char *)g + 0x1010) =
            random_int_between(scale_game_frames(0x32), scale_game_frames(0x5A));
        set_hero_item_switch_timer_for_item(*(hero.selected_item + i));
        set_hero_condition_animation_timer_for_item(*(hero.selected_item + i));
        if (g->item_anim_active != 0) {
            cancel_hero_item_animation();
        }
        if (i == 0) {
            if (*hero.selected_item == 8) {
                D_0015ED90 = 1;
            } else {
                D_0015ED90 = 0;
            }
        }
        if (i == 3) {
            if ((&hero)->items[3].item_id == 3) {
                D_0015ED94 = 1;
            } else {
                D_0015ED94 = 0;
            }
        }
        {
            struct Hero *k = &hero;
            struct Hero *s = (struct Hero *)((char *)k + i * sizeof(struct HeroItemSlot));
            s->items[0].thrown = 0;
            k->pending_item[i] = 0;
            k->force_slot0_item8 = 0;
            if (*(char **)((char *)k + 0xD00) != 0) {
                *(int *)(*(char **)((char *)k + 0xD00) + 0x34) = 1;
            }
            if (*(char **)((char *)k + 0xD04) != 0) {
                *(int *)(*(char **)((char *)k + 0xD04) + 0x34) = 1;
            }
            if (s->items[0].moby != 0) {
                blend_moby_animation(s->items[0].moby, 2, 0, 2);
            }
            if (s->items[0].moby2 != 0 && i != 3) {
                blend_moby_animation(s->items[0].moby2, 2, 0, 2);
            }
        }
        {
            struct Hero *slot_adjusted_hero =
                (struct Hero *)(((char *)&hero) + i * sizeof(struct HeroItemSlot));
            slot_adjusted_hero->items[0].state = 3;
        }
    }
}

extern int                         frame_number __asm__("D_L05_0015F5CC");
extern CollisionHit D_L05_00174240 NOT_SDA;
extern u8                          D_L05_00174260[] NOT_SDA;
extern u8                          D_L05_0017AB20[] NOT_SDA;
extern int probe_hero_forward_collision(float, float, float *) __asm__("FUN_L00_0020a240");
extern void
transform_hero_local_offset_to_world(float *, float, float, float) __asm__("FUN_L00_00233660");
extern int  collision_line(void *, void *, int, void *, int) __asm__("FUN_001efa68");
extern int  coll_sphere(float, void *, int, int) __asm__("FUN_L00_001f0d60");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");

void update_hero_movement_and_collision_probes(void) __asm__("FUN_L05_0024bdc0");

void update_hero_movement_and_collision_probes(void) {
    float start[4], end[4], probe[4];
    u128  moby_rotation[4], rotation[4], combined[4];
    float distance;
    float offset;
    int   hit;
    if (hero.tilt_disabled == 0) {
        integrate_hero_motion_with_collisions();
    }
    if (frame_number % 3 == 0) {
        hero.wall_probe_distance = 4.0f;
        hero.wall_probe_moby_in_class_range = 0;
        hero.wall_probe_hit_moby = 0;
        if (probe_hero_forward_collision(0.7f, 4.0f, &hero.wall_probe_distance)) {
            CollisionHit *wall = &D_L05_00174240;
            float         normal_xy = vector_length_xy(&wall->normal_x);
            hero.wall_normal_pitch = fast_arc_tan(wall->normal_z, normal_xy);
            if (wall->moby != 0) {
                hero.wall_probe_hit_moby = 1;
                if (is_moby_in_class_range_500_to_540(wall->moby))
                    hero.wall_probe_moby_in_class_range = 1;
            }
        }
    }
    if (frame_number % 5 == 0) {
        hero.edge_ahead = 0;
        hero.edge_drop_depth = 0.0f;
        if (hero.grounded_frames) {
            transform_hero_local_offset_to_world(start, 1.1f, 0.0f, 1.0f);
            transform_hero_local_offset_to_world(end, 1.1f, 0.0f, -20.0f);
            if (end[2] < 0.5f)
                end[2] = 0.5f;
            distance = 20.0f;
            hit = collision_line(start, end, 2, hero.moby, 0);
            if (!hit || (distance = fast_vec_dist(start, D_L05_00174260), distance > 3.0f)) {
                for (offset = -0.4f; offset < 0.5f; offset += 0.1f) {
                    transform_hero_local_offset_to_world(probe, 1.1f + offset, 0.0f, 0.0f);
                    if (!coll_sphere(hero.capsule_radius, probe, 2, 0)) {
                        hero.edge_ahead = 1;
                        if (!hit)
                            hero.edge_drop_depth = 20.0f;
                        else
                            hero.edge_drop_depth = distance;
                        hero.edge_probe_offset = hero.capsule_radius + offset;
                        if (hero.edge_probe_offset < 0.0f)
                            hero.edge_probe_offset = 0.0f;
                        break;
                    }
                }
            }
        }
    }
    if ((unsigned)(hero.state.control_mode - 0x15) < 2 && hero.hoverboard_moby) {
        qcopy(hero.hoverboard_moby + 0x10, (char *)&hero + 0x80);
        *(u128 *)start = 0;
        start[2] = 0.21f;
        transform_vector_by_basis(start, start, (char *)hero.moby + 0xc0);
        euler_rotation_matrix(rotation, D_L05_0017AB20);
        transform_vector(start, start, rotation);
        fast_vec_add(hero.hoverboard_moby + 0x10, hero.hoverboard_moby + 0x10, start);
        copy_matrix_rotation(moby_rotation, (char *)hero.moby + 0xc0);
        multiply_matrix(combined, moby_rotation, rotation);
        copy_matrix3x4(hero.hoverboard_moby + 0xc0, combined);
    }
}
/* Adapted from the exact FUN_L00_002279b0 (src/overlays/shared/hero/hero_set_state.c): L05 has a wider head guard and its own level data. */

extern float D_L05_00174268 NOT_SDA;
extern int func_L00_001EFFF0(void *, void *, int, int, int) __asm__("FUN_001efa68");
extern void
hero_get_collision_ray_endpoints(void *, void *, int, float, float) __asm__("FUN_L00_002126b8");

int try_apply_hero_water_and_ground_transitions(void) __asm__("FUN_L05_002515d0");

int try_apply_hero_water_and_ground_transitions(void) {
    struct Hero *hero_ptr = &hero;
    struct Hero *depth_check_hero;
    struct Hero *collision_check_hero;
    int          water_state_requested;
    float        ray_start[4];
    float        ray_end[4];
    float        water_surface_tolerance;
    float        scale;
    if ((unsigned)(hero_ptr->state.control_mode - 0x15) < 2) {
        return 0;
    }
    if (hero_ptr->state.current == 0x6F || hero_ptr->state.current == 0x6D ||
        hero_ptr->state.current == 0x6E || hero_ptr->state.current == 0x6A) {
        return 0;
    }
    water_state_requested = 0;
    if (hero_ptr->base_condition != 0) {
        if (hero_ptr->state.control_mode == 0x11) {
            if (hero_ptr->state.step != 1) {
                if (0.0f < hero_ptr->motion.displacement.f[2]) {
                    if (hero_ptr->water_surface_z - 0.4f < hero_ptr->motion.pos.f[2]) {
                        water_state_requested = 1;
                    }
                }
            }
        } else if (hero_ptr->state.control_mode != 0x12 && hero_ptr->state.control_mode != 3) {
            water_surface_tolerance = 0.2f;
            if (water_surface_tolerance < absolute_float(hero_ptr->motion.velocity.f[2]) + 0.07f) {
                water_surface_tolerance = absolute_float(hero_ptr->motion.velocity.f[2]) + 0.07f;
            }
            if (absolute_float(hero_ptr->water_surface_z - (hero_ptr->motion.pos.f[2] + 0.45f)) <
                water_surface_tolerance) {
                if (hero_ptr->state.control_mode != 4 || hero_ptr->past_jump_apex != 0) {
                    if (0.8f < hero_ptr->water_depth && hero_ptr->motion.displacement.f[2] < 0.0f) {
                        water_state_requested = 1;
                    }
                }
            }
            depth_check_hero = &hero;
            if (depth_check_hero->motion.pos.f[2] < depth_check_hero->water_surface_z - 0.8f) {
                if (depth_check_hero->state.control_mode != 4 ||
                    depth_check_hero->past_jump_apex != 0) {
                    water_state_requested = 1;
                }
            }
        }
    }
    hero_ptr = &hero;
    water_surface_tolerance = 0.27f;
    if (water_surface_tolerance < absolute_float(hero_ptr->motion.velocity.f[2]) + 0.07f) {
        water_surface_tolerance = absolute_float(hero_ptr->motion.velocity.f[2]);
    }
    if (hero_ptr->state.current == 0x12) {
        if (hero_ptr->past_jump_apex != 0) {
            if (hero_ptr->motion.pos.f[2] < hero_ptr->water_surface_z - 0.7f) {
                water_state_requested = 1;
            }
        }
    }
    collision_check_hero = &hero;
    if (collision_check_hero->state.control_mode - 0x11 >= 2u &&
        collision_check_hero->state.control_mode != 7 &&
        collision_check_hero->state.control_mode != 0x14 &&
        collision_check_hero->state.current != 0x12 && (frame_number & 0xF) == 0) {
        scale = 4.0f;
        if ((frame_number & 0x3F) == 0) {
            scale = 16.0f;
        }
        hero_get_collision_ray_endpoints(ray_start, ray_end, 0, scale, 1.3f);
        if (func_L00_001EFFF0(ray_start, ray_end, 2, *(int *)&collision_check_hero->moby, 0) != 0) {
            int hit_material = collision_hit_material();
            collision_check_hero->ground_type = hit_material;
            if ((short)hit_material == 0) {
                ray_start[2] = D_L05_00174268 - 0.01f;
                if (func_L00_001EFFF0(
                        ray_start, ray_end, 2, *(int *)&collision_check_hero->moby, 0) == 0) {
                    hero_set_state(0x34, 1);
                    return 1;
                }
            }
        }
    }
    if (water_state_requested != 0) {
        hero_set_state(0x37, 1);
        allocate_hero_voice_for_entry(3, 0);
    }
    return water_state_requested;
}

#include "rnc/input/pad_state.h"
#include "rnc/mobys/moby.h"

/* Gadget and weapon input for the current hero state: starts the matching move and returns 1 when the state changed. */
/* Ported from rac1-decomp (src/overlays/shared/help_00237B00.c: func_L05_002523C8), where it is exact; names translated to the US level program. */

typedef struct Hero Hero;

typedef struct Moby Moby;

typedef struct PadState PadState;

typedef struct {
    char p0[0xC];
    int  fC;
    char p10[0x1C];
    int  f2C[5];
} PVar;

extern int            D_0015ED84; /* no foreign declaration */
extern int            FUN_L00_001fefc8(void *);
extern int            FUN_L00_0020d498(int);
extern int            can_select_hero_item_31(void) __asm__("FUN_L00_0020fe30");
extern int            is_wrench_moby(void *) __asm__("FUN_L00_00211360");
extern int            select_hero_grapple_target(void) __asm__("FUN_L00_00222158");
extern int            get_ammo(int) __asm__("FUN_L00_00233e98");
extern unsigned short D_00141920[] NOT_SDA;
extern unsigned short D_00141D80[] NOT_SDA;
extern void           start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern void           launch_hero_wrench(void) __asm__("FUN_L00_00217658");
extern void           select_hero_grapple_swing_target(void) __asm__("FUN_L00_00221ee0");
extern void           FUN_L05_0024cee8(int, int);
extern void create_class_5c3_moby(void *, void *, void *, float, float) __asm__("FUN_L05_002d4168");
extern PadState       D_0013CA40_p2523C8 __asm__("D_0013C940");
extern int            D_00141660_lui __asm__("D_00141660") NOT_SDA;
extern unsigned char  D_0013D5C8_2523C8[] __asm__("D_0013D4DC");
extern HeroTableRow2C D_L05_0017C088[];
extern int            D_L05_0015F674_gp __asm__("D_L05_0015F674") __attribute__((sda));
extern Moby          *D_L05_0015FFD8_macro __asm__("D_L05_0015FFD8") MACRO_ADDR;
extern int            D_0015EEA4_macro __asm__("D_0015EEA4") MACRO_ADDR;
extern int            D_0015ED84_gp __asm__("D_0015ED84") MACRO_ADDR;

int process_hero_primary_item_input(void) __asm__("FUN_L05_00251be8");

int process_hero_primary_item_input(void) {
    enum {
        ITEM_WRENCH = 8,
        ITEM_BUFFERED_SOUND_DEVICE = 10,
        ITEM_TARGET_INTERACTION_DEVICE = 12,
        ITEM_HELD_SOUND_DEVICE = 15,
        ITEM_STATE_CHANGING_SOUND_DEVICE = 18,
        ITEM_PRESS_ACTIVATED_DEVICE = 21,
        ITEM_HERO_SOUND_REQUEST_DEVICE = 31,
        ITEM_AMMO_MENU_DEVICE = 32,
        ITEM_RACE_PROJECTILE = 36
    };
    float joint_pos[4];
    int   press_time0, press_time1, press_time2;
    int   start_timer;
    int   mask;

    if ((*(Hero *)((char *)&hero)).item_use_locked != 0 &&
        (*(Hero *)((char *)&hero)).items[0].item_id != 0x24 &&
        ((*(Hero *)((char *)&hero)).items[0].item_id != 8 ||
         (*(Hero *)((char *)&hero)).unk308 != 1)) {
        return 0;
    }
    if ((*(Hero *)((char *)&hero)).vehicle_mode != 0) {
        return 0;
    }
    if (((Moby *)(*(Hero *)((char *)&hero)).items[0].moby) == 0) {
        return 0;
    }
    if ((*(Hero *)((char *)&hero)).items[0].state != 2) {
        return 0;
    }
    start_timer = (*(Hero *)((char *)&hero)).state_timer;
    mask = (*(Hero *)((char *)&hero)).items[0].button_mask;
    switch ((*(Hero *)((char *)&hero)).items[0].item_id) {
    case ITEM_WRENCH:
        if ((*(Hero *)((char *)&hero)).state.control_mode < 2U ||
            ((*(Hero *)((char *)&hero)).state.control_mode == 4 &&
             (*(Hero *)((char *)&hero)).rail_land_frames != 0)) {
            Moby *item;

            if ((*(Hero *)((char *)&hero)).on_slip_ground != 0) {
                break;
            }
            if (!pad_pressed_within(mask, scale_game_frames(7), &press_time0)) {
                break;
            }
            item = (Moby *)(*(Hero *)((char *)&hero)).items[0].moby;
            if (item == 0) {
                break;
            }
            if (!is_wrench_moby(item)) {
                break;
            }
            if (item->state != 0) {
                break;
            }
            if ((D_0013CA40_p2523C8.held & 0xA) ||
                (((*(Hero *)((char *)&hero)).state.current == 1 ||
                  (*(Hero *)((char *)&hero)).state.current == 0x1E) &&
                 (*(Hero *)((char *)&hero)).state_timer > scale_game_frames(0x14))) {
                if (((*(Hero *)((char *)&hero)).state.current == 1 ||
                     (*(Hero *)((char *)&hero)).state.current == 0x1E) &&
                    (*(Hero *)((char *)&hero)).state_timer > scale_game_frames(0x14)) {
                    launch_hero_wrench();
                    break;
                }
                hero_set_state(0x15, 1);
                break;
            }
            if ((*(Hero *)((char *)&hero)).unk308 == 1) {
                hero_set_state(0x70, 1);
            } else {
                hero_set_state(0x13, 1);
            }
            break;
        }
        if ((*(Hero *)((char *)&hero)).state.current == 7 ||
            (*(Hero *)((char *)&hero)).state.current == 9 ||
            (*(Hero *)((char *)&hero)).state.current == 8 ||
            (*(Hero *)((char *)&hero)).state.current == 6 ||
            ((*(Hero *)((char *)&hero)).state.current == 0xE &&
             (*(Hero *)((char *)&hero)).anim_frame > 32.0f) ||
            ((*(Hero *)((char *)&hero)).state.current == 0xB &&
             (*(Hero *)((char *)&hero)).unk450 != 3 &&
             (*(Hero *)((char *)&hero)).anim_frame > 18.0f) ||
            ((*(Hero *)((char *)&hero)).state.current == 0xB &&
             (*(Hero *)((char *)&hero)).unk450 == 3 &&
             (*(Hero *)((char *)&hero)).anim_frame > 20.0f) ||
            (*(Hero *)((char *)&hero)).state.current == 0x2D ||
            (*(Hero *)((char *)&hero)).state.current == 0x11) {
            if ((*(Hero *)((char *)&hero)).state_timer > scale_game_frames(0xA)) {
                int window = scale_game_frames(0xA);

                if ((*(Hero *)((char *)&hero)).state.current == 0xB ||
                    (*(Hero *)((char *)&hero)).state.current == 0xE) {
                    window = scale_game_frames(0x12);
                }
                if (!pad_pressed_within(mask, window, 0)) {
                    break;
                }
                if (((*(Hero *)((char *)&hero)).past_jump_apex == 0 &&
                     (*(Hero *)((char *)&hero)).ground_distance > 0.1f) ||
                    ((*(Hero *)((char *)&hero)).past_jump_apex != 0 &&
                     (*(Hero *)((char *)&hero)).ground_distance > 0.57f) ||
                    ((*(Hero *)((char *)&hero)).state.current == 8 &&
                     (*(Hero *)((char *)&hero)).ground_distance > 0.57f)) {
                    hero_set_state(0x14, 1);
                    break;
                }
                if ((*(Hero *)((char *)&hero)).on_slip_ground != 0) {
                    break;
                }
                hero_set_state(0x13, 1);
                break;
            }
        }
        if ((*(Hero *)((char *)&hero)).state.control_mode == 6) {
            HeroTableRow2C *rec = &D_L05_0017C088[(*(Hero *)((char *)&hero)).attack_record];
            int             window;

            if ((*(Hero *)((char *)&hero)).state.current == 0x15) {
                if ((*(Hero *)((char *)&hero)).state_timer > scale_game_frames(0x1E)) {
                    if ((*(Hero *)((char *)&hero)).anim_changing != 0) {
                        break;
                    }
                    if ((*(Hero *)((char *)&hero)).anim_frame >= (float)rec->f10 &&
                        (D_0013CA40_p2523C8.held & 0xA) &&
                        pad_pressed_within(mask, scale_game_frames(0xF), 0)) {
                        hero_set_state(0x15, 1);
                        break;
                    }
                }
            }
            if ((*(Hero *)((char *)&hero)).anim_changing != 0) {
                break;
            }
            if (!((*(Hero *)((char *)&hero)).anim_frame >= (float)rec->f10)) {
                break;
            }
            window = (truncate_float_to_s32((*(Hero *)((char *)&hero)).anim_frame) - rec->fC) * 2;
            if (window <= 0) {
                window = 1;
            }
            if (window > scale_game_frames(0xF)) {
                window = scale_game_frames(0xF);
            }
            if ((*(Hero *)((char *)&hero)).on_slip_ground != 0) {
                break;
            }
            if (!pad_pressed_within(mask, window, 0)) {
                break;
            }
            if (D_0013CA40_p2523C8.held & 0xA) {
                hero_set_state(0x15, 1);
                break;
            }
            if ((*(Hero *)((char *)&hero)).unk308 == 1) {
                hero_set_state(0x70, 1);
            } else {
                hero_set_state(0x13, 1);
            }
        }
        break;
    case ITEM_TARGET_INTERACTION_DEVICE: {
        int window;

        select_hero_grapple_swing_target();
        select_hero_grapple_target();
        window = scale_game_frames(5);
        if ((*(Hero *)((char *)&hero)).air_frames.s != 0) {
            window = scale_game_frames(8);
        }
        if ((*(Hero *)((char *)&hero)).state.control_mode < 2U ||
            (*(Hero *)((char *)&hero)).state.control_mode == 4 ||
            (*(Hero *)((char *)&hero)).state.control_mode == 2 ||
            (*(Hero *)((char *)&hero)).state.control_mode == 0xF ||
            (*(Hero *)((char *)&hero)).state.control_mode == 5 ||
            (*(Hero *)((char *)&hero)).state.current == 0x1E) {
            if (pad_pressed_within(mask, window, 0) ||
                ((D_0013CA40_p2523C8.held & mask) &&
                 (*(Hero *)((char *)&hero)).items[0].ready_frames > scale_game_frames(0xF))) {
                if ((*(Hero *)((char *)&hero)).swing_target_in_sight != 0) {
                    if ((*(Hero *)((char *)&hero)).grapple_target_in_sight != 0) {
                        if ((*(Hero *)((char *)&hero)).swing_target_dist <
                            (*(Hero *)((char *)&hero)).unk984) {
                            (*(Hero *)((char *)&hero)).grapple_target_in_sight = 0;
                        } else {
                            (*(Hero *)((char *)&hero)).swing_target_in_sight = 0;
                        }
                    }
                    if ((*(Hero *)((char *)&hero)).swing_target_in_sight != 0 &&
                        (*(Hero *)((char *)&hero)).grapple_lockout_timer == 0 &&
                        (*(Hero *)((char *)&hero)).state.current != 0x2C) {
                        hero_set_state(0x2C, 1);
                        break;
                    }
                }
                if ((*(Hero *)((char *)&hero)).grapple_target_in_sight != 0 &&
                    (*(Hero *)((char *)&hero)).grapple_lockout_timer == 0) {
                    hero_set_state(0x24, 1);
                }
            }
        }
    } break;
    case ITEM_HERO_SOUND_REQUEST_DEVICE:
        if ((*(Hero *)((char *)&hero)).vehicle_mode != 0) {
            break;
        }
        if (!can_select_hero_item_31()) {
            break;
        }
        if (!pad_pressed_within(mask, scale_game_frames(7), 0)) {
            break;
        }
        if (D_00141660_lui != 0x1F) {
            break;
        }
        if (((Moby *)(*(Hero *)((char *)&hero)).items[0].moby)->oclass != 0x1E3) {
            break;
        }
        if ((*(Hero *)((char *)&hero)).vehicle_toggle_timer != 0) {
            break;
        }
        allocate_hero_voice_for_entry(0x18, 0);
        (*(Hero *)((char *)&hero)).vehicle_toggle_timer = 0x12;
        (*(Hero *)((char *)&hero)).unk22DC = 1;
        break;
    case ITEM_AMMO_MENU_DEVICE:
        if ((*(Hero *)((char *)&hero)).state.control_mode < 3U ||
            (*(Hero *)((char *)&hero)).state.control_mode == 4 ||
            (*(Hero *)((char *)&hero)).state.control_mode == 5) {
            if (pad_pressed_within(mask, scale_game_frames(8), 0)) {
                open_vendor_menu(0);
            }
        }
        break;
    case ITEM_HELD_SOUND_DEVICE:
        if ((*(Hero *)((char *)&hero)).state.control_mode < 2U ||
            (*(Hero *)((char *)&hero)).state.control_mode == 0xC ||
            (*(Hero *)((char *)&hero)).state.control_mode == 2 ||
            (*(Hero *)((char *)&hero)).state.control_mode == 4 ||
            (*(Hero *)((char *)&hero)).state.control_mode == 5 ||
            (*(Hero *)((char *)&hero)).state.control_mode == 0xF ||
            (*(Hero *)((char *)&hero)).state.current == 0x1E) {
            PVar *pv;

            if (((Moby *)(*(Hero *)((char *)&hero)).items[0].moby) == 0) {
                break;
            }
            pv = (PVar *)((Moby *)(*(Hero *)((char *)&hero)).items[0].moby)->pvars;
            if (((D_0013CA40_p2523C8.held & mask) &&
                 (*(Hero *)((char *)&hero)).items[0].ready_frames > scale_game_frames(5)) ||
                (D_0013CA40_p2523C8.pressed & mask)) {
                if (get_ammo(-1)) {
                    if ((*(Hero *)((char *)&hero)).item_anim_active == 0) {
                        start_equipped_item_animation();
                    }
                    break;
                }
            }
            if (D_0013CA40_p2523C8.held & mask) {
                if (!get_ammo(-1)) {
                    if (!voice_is_playing((Moby *)(*(Hero *)((char *)&hero)).items[0].moby,
                                          pv->fC)) {
                        pv->fC = allocate_voice_for_target_entry(
                            0, 0, (int)((Moby *)(*(Hero *)((char *)&hero)).items[0].moby));
                    }
                }
            }
        }
        break;
    case ITEM_BUFFERED_SOUND_DEVICE:
    case 17:
    case 20:
    case 24:
    case 25:
        if (((*(Hero *)((char *)&hero)).state.control_mode == 0 &&
             (*(Hero *)((char *)&hero)).state.current != 0x1E) ||
            (*(Hero *)((char *)&hero)).state.control_mode == 1 ||
            (*(Hero *)((char *)&hero)).state.control_mode == 0xC ||
            (*(Hero *)((char *)&hero)).state.control_mode == 5 ||
            (*(Hero *)((char *)&hero)).state.control_mode == 0xF ||
            ((*(Hero *)((char *)&hero)).state.control_mode == 4 &&
             (*(Hero *)((char *)&hero)).unk4AC != 0) ||
            ((*(Hero *)((char *)&hero)).state.current == 0x23 &&
             (*(Hero *)((char *)&hero)).anim_frame > 25.0f)) {
            int window;
            int held_frames;

            if ((*(Hero *)((char *)&hero)).item_anim_active != 0) {
                break;
            }
            window = scale_game_frames(7);
            held_frames = (*(Hero *)((char *)&hero)).items[0].ready_frames;
            if (pad_pressed_within(
                    mask, window <= held_frames ? window : held_frames, &press_time1) ||
                ((D_0013CA40_p2523C8.held & 0x20) && held_frames > scale_game_frames(8) &&
                 held_frames < scale_game_frames(0x11))) {
                if (get_ammo(-1)) {
                    if ((*(Hero *)((char *)&hero)).state.control_mode == 0xC ||
                        (*(Hero *)((char *)&hero)).state.control_mode == 0 ||
                        (*(Hero *)((char *)&hero)).state.current == 3 ||
                        ((*(Hero *)((char *)&hero)).state.control_mode == 1 &&
                         (*(Hero *)((char *)&hero)).stick_magnitude < 0.7f)) {
                        hero_set_state(0x23, 1);
                    } else {
                        start_equipped_item_animation();
                    }
                    break;
                }
                if (((Moby *)(*(Hero *)((char *)&hero)).items[0].moby) != 0) {
                    allocate_voice_for_target_entry(
                        0, 0, (int)((Moby *)(*(Hero *)((char *)&hero)).items[0].moby));
                }
            }
        }
        break;
    case ITEM_STATE_CHANGING_SOUND_DEVICE:
        if ((*(Hero *)((char *)&hero)).state.control_mode < 2U ||
            ((*(Hero *)((char *)&hero)).state.control_mode == 4 &&
             (*(Hero *)((char *)&hero)).rail_land_frames != 0)) {
            if ((*(Hero *)((char *)&hero)).state.current != 1) {
                int window = scale_game_frames(0xA);
                int held_frames = (*(Hero *)((char *)&hero)).items[0].ready_frames;

                if (pad_pressed_within(
                        mask, window <= held_frames ? window : held_frames, &press_time2)) {
                    if (((Moby *)(*(Hero *)((char *)&hero)).items[0].moby) != 0) {
                        hero_set_state(0x20, 1);
                        if (((Moby *)(*(Hero *)((char *)&hero)).items[0].moby) != 0) {
                            allocate_voice_for_target_entry(
                                0, 0, (int)((Moby *)(*(Hero *)((char *)&hero)).items[0].moby));
                        }
                    }
                }
            }
        }
        break;
    case ITEM_PRESS_ACTIVATED_DEVICE:
        if (((Moby *)(*(Hero *)((char *)&hero)).items[0].moby) != 0) {
            if (D_0013CA40_p2523C8.pressed & mask) {
                start_equipped_item_animation();
            }
        }
        break;
    case ITEM_RACE_PROJECTILE: {
        Moby              *item;
        Moby              *best;
        struct UsageStats *stats;
        int                play_time, unit;

        if ((*(Hero *)((char *)&hero)).race_ammo != 0) {
            if (D_00141D80[0] == 0 && D_00141920[0] == 0) {
                if (++(*(Hero *)((char *)&hero)).help_hint_frames >
                    (int)((float)scale_game_frames(0x14) * 60.0f)) {
                    help_request(0x3E81, 0x83);
                }
            }
        }
        item = (Moby *)(*(Hero *)((char *)&hero)).items[0].moby;
        if (item == 0) {
            break;
        }
        if (FUN_L00_001fefc8(&(*(Hero *)((char *)&hero)).shot_cooldown) == 2 &&
            (*(Hero *)((char *)&hero)).race_ammo != 0) {
            (*(Hero *)((char *)&hero)).race_ammo--;
        }
        if ((*(Hero *)((char *)&hero)).race_ammo == 0) {
            break;
        }
        if ((*(Hero *)((char *)&hero)).shot_cooldown != 0) {
            break;
        }
        if (!pad_pressed_within(mask, scale_game_frames(7), 0)) {
            break;
        }
        best = 0;
        if (((Moby *)(*(Hero *)((char *)&hero)).hoverboard_moby) != 0) {
            PVar *pv = (PVar *)((Moby *)(*(Hero *)((char *)&hero)).hoverboard_moby)->pvars;
            float best_score = 9999999.0f;
            int   i;

            i = 0;
            while (i < 5) {
                int   id = pv->f2C[i];
                Moby *candidate;
                float yaw_error, pitch_error, distance, score;

                if (id == -1) {
                    i++;
                    continue;
                }
                candidate = (Moby *)((char *)D_L05_0015FFD8_macro + (id << 8));
                yaw_error = fast_difference_between_rotations(
                    (*(Hero *)((char *)&hero)).motion.rot.f[2],
                    fast_arc_tan(candidate->pos.x - (*(Hero *)((char *)&hero)).motion.pos.f[0],
                                 candidate->pos.y - (*(Hero *)((char *)&hero)).motion.pos.f[1]));
                if (!(yaw_error > 0.7853982f)) {
                    pitch_error = fast_difference_between_rotations(
                        fast_arc_tan(vector_distance_xy((*(Hero *)((char *)&hero)).motion.pos.f,
                                                        &candidate->pos),
                                     candidate->pos.z - (*(Hero *)((char *)&hero)).motion.pos.f[2]),
                        0.0f);
                    if (!(pitch_error > 0.7853982f)) {
                        distance = vector_distance_xy(&candidate->pos,
                                                      (*(Hero *)((char *)&hero)).motion.pos.f);
                        score = distance;
                        score += distance * yaw_error * 2.0f;
                        score += distance * pitch_error * 2.0f;
                        if (best == 0 || score < best_score) {
                            best = candidate;
                            best_score = score;
                        }
                    }
                }
                i++;
            }
        }
        stats = &usage_stats;
        item->command = scale_game_frames(7);
        (*(Hero *)((char *)&hero)).shot_cooldown = scale_game_frames(0x32);
        if (stats->stat[27].count < 0xFFFF) {
            stats->stat[27].count++;
        }
        play_time = scale_game_frames(D_0015EEA4_macro);
        unit = 600;
        if (play_time / unit > stats->stat[27].unk2) {
            stats->stat[27].unk2 = scale_game_frames(D_0015EEA4_macro) / 600;
        }
        stats->stat[27].level_mask = stats->stat[27].level_mask | (1 << D_0015ED84_gp) | 0x80000000;
        allocate_voice_for_target_entry(0, 0, (int)item);
        moby_point_position(item, 0, joint_pos);
        create_class_5c3_moby(item,
                              joint_pos,
                              best,
                              (*(Hero *)((char *)&hero)).motion.rot.f[2],
                              (*(Hero *)((char *)&hero)).motion.rot.f[1]);
    } break;
    }
    if ((*(Hero *)((char *)&hero)).state_timer == start_timer) {
        if (FUN_L00_0020d498(3) == 3 && (*(Hero *)((char *)&hero)).state.current != 0x81 &&
            ((*(Hero *)((char *)&hero)).unk308 == 0 || D_0013D5C8_2523C8[0] == 0) &&
            (*(Hero *)((char *)&hero)).pack_disabled == 0) {
            fast_dec_timer(&D_L05_0015F674_gp);
            if (D_L05_0015F674_gp != 0) {
                if ((*(Hero *)((char *)&hero)).state.control_mode < 2U ||
                    (*(Hero *)((char *)&hero)).state.control_mode == 0xC ||
                    (((*(Hero *)((char *)&hero)).state.control_mode == 2 ||
                      (*(Hero *)((char *)&hero)).state.control_mode == 4) &&
                     (*(Hero *)((char *)&hero)).grounded_frames != 0)) {
                    if ((D_0013CA40_p2523C8.held & 8) && (D_0013CA40_p2523C8.pressed & 8)) {
                        hero_set_state(0x81, 1);
                    }
                }
            }
            if ((D_0013CA40_p2523C8.pressed & 8) &&
                (*(Hero *)((char *)&hero)).state.current != 0x81) {
                D_L05_0015F674_gp = scale_game_frames(0x1E);
            }
        } else if ((*(Hero *)((char *)&hero)).state.current != 0x13 &&
                   (*(Hero *)((char *)&hero)).state.current != 0x81 &&
                   (*(Hero *)((char *)&hero)).state.current != 0x15 &&
                   (*(Hero *)((char *)&hero)).state.current != 0x16) {
            (*(Hero *)((char *)&hero)).unk22CA = 0;
        }
    }
    return (*(Hero *)((char *)&hero)).state_timer < start_timer;
}
/* Hero idle: picks the idle animation from the stick direction, and occasionally plays a fidget or the
 * look-around when standing still. */
/* Ported from rac1-decomp (src/overlays/shared/help_00237B00.c: func_L05_00254030), where it is exact; names translated to the US level program. */

typedef struct {
    int bank;
    int seq;
} SndPair;

extern SndPair l05_hoverboard_trick_animation_frame_bounds[] __asm__("D_L05_00179BC0");

#include "rnc/input/pad_globals.h"

extern float
predict_hero_trajectory_intercept_time(float a, float b, float c) __asm__("FUN_L00_00232b90");
extern int  select_hero_hoverboard_animation_variant(void) __asm__("FUN_L05_002551b8");
extern void hero_queue_sequence(int bank, int seq) __asm__("FUN_L00_002325e0");

void update_hero_hoverboard_trick_animations(void) __asm__("FUN_L05_00253850");

void update_hero_hoverboard_trick_animations(void) {
    struct Hero *hero_ptr = &hero;
    int          pad;
    hero_ptr->board_jump_land_frames = truncate_float_to_s32(
        predict_hero_trajectory_intercept_time(60.0f, hero_ptr->board_gravity, -1.0f));
    if (hero_ptr->trick_input != 0 && ((pad = pad_held[0]) & 0xF) != 0) {
        hero_ptr->air_pose_index = 0;
        if (pad & 8)
            hero_ptr->air_pose_index = 1;
        else if (pad & 1)
            hero_ptr->air_pose_index = 2;
        else if (pad & 2)
            hero_ptr->air_pose_index = 3;
        {
            struct Hero *h = &hero;
            int          a = h->air_pose_index + 0x69;
            if (((unsigned char *)*(char **)&h->moby)[0x53] != a) {
                set_hero_animation(a, 2, (float)scale_game_frames(5));
                hero_queue_sequence(
                    l05_hoverboard_trick_animation_frame_bounds[h->air_pose_index].bank,
                    l05_hoverboard_trick_animation_frame_bounds[h->air_pose_index].seq);
            } else if ((float)h->anim_loop_frame - 2.0f < h->anim_frame) {
                h->tricks_landed[h->air_pose_index] = 1;
                h->unk6F8 += 1;
            }
        }
    }
    {
        struct Hero *k = &hero;
        if ((k->air_frames.s == 0 || (pad_held[0] & 0xF) == 0) &&
            (unsigned int)(((unsigned char *)((char *)k->moby))[0x53] - 0x69) < 4 &&
            k->anim_changing == 0 &&
            k->anim_loop_frame < ((unsigned char *)((char *)k->moby))[0x51] &&
            ((unsigned char *)((char *)k->moby))[0x51] < k->queued_anim_frame) {
            int animation_transition_frames = k->board_jump_land_frames;
            if (animation_transition_frames > scale_game_frames(0x1E)) {
                int x = select_hero_hoverboard_animation_variant();
                set_hero_animation(x, 9, (float)scale_game_frames(0x11));
            } else {
                animation_transition_frames = k->board_jump_land_frames;
                if (animation_transition_frames < scale_game_frames(0xA))
                    animation_transition_frames = scale_game_frames(0xA);
                set_hero_animation(select_hero_hoverboard_animation_variant(),
                                   0xD,
                                   (float)animation_transition_frames);
            }
        }
    }
    {
        struct Hero *h = &hero;
        if (h->hoverboard_airborne != 0) {
            int a = ((unsigned char *)((char *)h->moby))[0x53];
            if (a == 0x52 || a == 0x7D || a == 0x68 || a == 0x7E) {
                int x = select_hero_hoverboard_animation_variant();
                set_hero_animation(x, 0xB, (float)scale_game_frames(0xA));
            }
        }
    }
    {
        struct Hero *h = &hero;
        if ((h->anim_flags & 2) && h->frames_off_ground.s != 0) {
            int x = select_hero_hoverboard_animation_variant();
            set_hero_animation(x, 9, (float)scale_game_frames(7));
        }
    }
}

extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern f32  fast_sin(f32) __asm__("FUN_001f9de0");
extern s32  D_L00_0015F5CC __asm__("D_L00_0015F5CC");
extern s32  D_0013D4B8;
extern s32  weapon_ammo_counts[37] __asm__("D_0013D428");

void process_hero_race_object_interactions(void) __asm__("FUN_L05_00253b48");

void process_hero_race_object_interactions(void) {
    u8 *state;
    u8 *moby;
    u8 *first;
    u8 *second;
    u8 *third;
    f32 delta[4];
    f32 matrix[16];
    s32 count, red, green, blue;
    f32 period, wave, ratio;

    if (hero.hoverboard_moby == 0)
        return;
    state = *(u8 **)(hero.hoverboard_moby + 0x78);
    if (*(s32 *)(state + 0x44) != -1) {
        get_first_moby_in_group((void **)&first, *(s32 *)(state + 0x44), 0, 0);
        if (first != 0) {
            do {
                if (*(s16 *)(first + 0xA6) == 0x473 &&
                    fast_subtract_rotations_cf(first + 0xBC) != 0) {
                    fast_vec_sub(delta, first + 0x10, &hero.motion.body_center);
                    set_identity_matrix(matrix);
                    transpose_rotation_matrix(matrix, first + 0xC0);
                    transform_vector(delta, delta, matrix);
                    if (absolute_float(delta[0]) < 0.5f && absolute_float(delta[1]) < 2.9f &&
                        absolute_float(delta[2]) < 2.9f) {
                        hero.boost_pad_timer += **(s32 **)(first + 0x78);
                        first[0xBC] = scale_game_frames(0x3C);
                        if (hero.hoverboard_moby != 0)
                            allocate_voice_for_target_entry(1, 0, hero.hoverboard_moby);
                    }
                }
                get_next_moby_in_group((void **)&first, first, 0, 0);
            } while (first != 0);
        }
    }
    count = scale_game_frames(0x32);
    period = convert_integer_to_float(count);
    ratio = (f32)(D_L00_0015F5CC % count) / period;
    ratio += ratio;
    wave = fast_sin(ratio * 3.1415927f + -3.1415927f);
    red = truncate_float_to_s32(wave * 48.0f) + 0x68;
    green = truncate_float_to_s32(wave * 64.0f) + 0xD0;
    blue = truncate_float_to_s32(wave * 24.0f) + 0x3C;
    if (green > 0xFF)
        green = 0xFF;
    if (*(s32 *)(state + 0x48) != -1) {
        if (hero.hoverboard_airborne == 0) {
            get_first_moby_in_group((void **)&second, *(s32 *)(state + 0x48), 0, 0);
            if (second != 0) {
                u32 color = (blue << 16) | (0x80000000U | (green << 8)) | red;
                do {
                    if (*(s16 *)(second + 0xA6) == 0x474) {
                        *(u16 *)(second + 0x34) |= 0x10;
                        *(u32 *)(second + 0x90) = color;
                        if (fast_subtract_rotations_cf(second + 0xBC) != 0) {
                            fast_vec_sub(delta, second + 0x10, &hero.motion.pos);
                            set_identity_matrix(matrix);
                            transpose_rotation_matrix(matrix, second + 0xC0);
                            transform_vector(delta, delta, matrix);
                            if (absolute_float(delta[0]) < 2.8f &&
                                absolute_float(delta[1]) < 1.25f &&
                                absolute_float(delta[2]) < 1.5f) {
                                hero.boost_pad_timer += **(s32 **)(second + 0x78);
                                second[0xBC] = scale_game_frames(0x3C);
                                if (hero.hoverboard_moby != 0)
                                    allocate_voice_for_target_entry(1, 0, hero.hoverboard_moby);
                            }
                        }
                    }
                    get_next_moby_in_group((void **)&second, second, 0, 0);
                } while (second != 0);
            }
        }
    }
    if (*(s32 *)(state + 0x4C) != -1) {
        get_first_moby_in_group((void **)&third, *(s32 *)(state + 0x4C), 0, 0);
        while (third != 0) {
            if (*(s16 *)(third + 0xA6) == 0x1D6 && fast_subtract_rotations_cf(third + 0xBC) != 0 &&
                fast_vec_dist(&hero.motion.body_center, third + 0x10) < 1.5f) {
                struct Hero *hero_ptr = &hero;
                third[0xBC] = scale_game_frames(300);
                if (*(s32 *)((u8 *)hero_ptr + 0x10B8) != 0x24) {
                    *(s32 *)((u8 *)hero_ptr + 0x20B8) = 0x24;
                    hero_ptr->item_use_locked = 0;
                    apply_hero_item_slot_selection(0);
                    hero_ptr->item_use_locked = 1;
                }
                hero_ptr->race_ammo++;
                if (hero_ptr->race_ammo > 3)
                    hero_ptr->race_ammo = 3;
                *(s32 *)((u8 *)hero_ptr + 0x20B8) = 0x24;
                weapon_ammo_counts[36]++;
            }
            get_next_moby_in_group((void **)&third, third, 0, 0);
        }
    }
}
/* Ported from rac1-decomp (src/overlays/shared/help_00237B00.c: func_L05_00254838), where it is exact; names translated to the US level program. */

struct ScoreTable {
    f32 values[5];
};

struct HeroTally {
    u8  pad0[0x6D0];
    f32 spin[3]; /* 0x6D0 */
    u8  pad6DC[4];
    s32 flags[4];        /* 0x6E0 */
    s32 flag_count;      /* 0x6F0 */
    f32 rank_scale;      /* 0x6F4 */
    s32 time_score_base; /* 0x6F8 */
    s32 score;           /* 0x6FC */
    s32 spin_count[3];   /* 0x700 */
    s32 max_spins;       /* 0x70C */
    s32 spin_score;      /* 0x710 */
    s32 total_score;     /* 0x714 */
    u8  pad718[0x190];
    s32 active; /* 0x8A8 */
};

extern struct ScoreTable l05_hoverboard_score_multipliers_by_trick_count __asm__("D_L05_002169F8");
extern float             scale_time(float) __asm__("FUN_001f96b0");
extern int               D_0015ED84;
extern int               D_0015EEA4;
extern u8                alternate_item_available[128] __asm__("D_0013D388");

#define TALLY ((struct HeroTally *)&hero)

void update_hero_hoverboard_trick_score(void) __asm__("FUN_L05_00254058");

void update_hero_hoverboard_trick_score(void) {
    int i;
    if (alternate_item_available[0] == 0)
        return;
    if (TALLY->active != 0) {
        struct UsageStats *stats = &usage_stats;
        if (stats->stat[30].count < 0xFFFF)
            stats->stat[30].count++;
        if (scale_game_frames(D_0015EEA4) / 600 > stats->stat[30].unk2)
            stats->stat[30].unk2 = scale_game_frames(D_0015EEA4) / 600;
        stats->stat[30].level_mask = stats->stat[30].level_mask | (1 << D_0015ED84) | 0x80000000;
    }
    for (i = 0; i < 3; i++) {
        float *spin = &TALLY->spin[i];
        if (absolute_float(*spin) > 3.3161256f) {
            float value = *spin;
            if (value > 0.0f) {
                *spin = value - 6.2831855f;
                TALLY->spin_count[i]++;
            } else {
                *spin = value + 6.2831855f;
                TALLY->spin_count[i]++;
            }
        }
    }
    TALLY->max_spins = 0;
    for (i = 0; i < 3; i++) {
        if (TALLY->max_spins < TALLY->spin_count[i])
            TALLY->max_spins = TALLY->spin_count[i];
    }
    {
        int step = 100;
        TALLY->spin_score = 0;
        for (i = 0; i < TALLY->max_spins; i++) {
            TALLY->spin_score += step;
            step += 50;
        }
    }
    TALLY->flag_count = 0;
    for (i = 0; i < 4; i++) {
        if (TALLY->flags[i] != 0)
            TALLY->flag_count++;
    }
    {
        struct ScoreTable table = l05_hoverboard_score_multipliers_by_trick_count;
        TALLY->rank_scale = table.values[TALLY->flag_count];
        TALLY->score = truncate_float_to_s32(
            (float)((TALLY->time_score_base * 100) / (int)(scale_time(1.0f) * 60.0f)));
        TALLY->score += TALLY->flag_count * 25;
        TALLY->score = truncate_float_to_s32((float)TALLY->score * TALLY->rank_scale);
        TALLY->total_score = TALLY->spin_score + TALLY->score;
    }
}
/* Finds the nearest path node ahead of the hero (behind the facing test) and returns its position and heading. */
/* Ported from rac1-decomp (src/overlays/shared/help_00237B00.c: func_L05_00254B38), where it is exact; names translated to the US level program. */

extern char D_0013F3D0[];

void compute_hero_hoverboard_respawn_transform(float *out,
                                               float *out_rotation) __asm__("FUN_L05_00254358");

void compute_hero_hoverboard_respawn_transform(float *out, float *out_rotation) {
    char        *hero_base = (char *)((char *)((char *)&hero));
    struct Moby *path_owner_moby = *(char **)(hero_base + 0x86C);
    char        *path;
    float        distance;
    int          i, best;
    float        ground_probe_position[4] __attribute__((aligned(16)));

    if (path_owner_moby == 0) {
        qcopy(out, hero_base + 0x80);
        qcopy(out_rotation, hero_base + 0x90);
        return;
    }
    path = ((char *)l05_paths[*(int *)(((char *)path_owner_moby->pvars) + 0x24)]);
    distance = 9999999.0f;
    best = 0;
    for (i = 0; i < *(int *)path; i += 2) {
        char *hero_bytes = (char *)((char *)((char *)&hero));
        char *candidate_path_record = path + i * 16;
        float limit = 4.0f;
        float angle =
            fast_arc_tan(*(float *)(candidate_path_record + 0x10) - *(float *)(hero_bytes + 0x80),
                         *(float *)(candidate_path_record + 0x14) - *(float *)(hero_bytes + 0x84));
        int   path_count_plus_two = *(int *)path + 2;
        int   next = (i + path_count_plus_two) % *(int *)path;
        char *lookahead_path_record = path + next * 16;
        if (fast_difference_between_rotations(
                angle,
                fast_arc_tan(*(float *)(lookahead_path_record + 0x10) -
                                 *(float *)(candidate_path_record + 0x10),
                             *(float *)(lookahead_path_record + 0x14) -
                                 *(float *)(candidate_path_record + 0x14))) > 1.5707964f) {
            float candidate_distance_xy;
            if (D_0015ED84_gp == 0x10 && *(char **)(hero_bytes + 0x8B4) != 0) {
                char *verts = *(char **)(hero_bytes + 0x8B4);
                char *first_height_sample_point = verts + *(short *)(hero_bytes + 0x8C8) * 16;
                char *second_height_sample_point = verts + *(short *)(hero_bytes + 0x898) * 16;
                float z1 = *(float *)(first_height_sample_point + 0x18);
                float z2 = *(float *)(second_height_sample_point + 0x18);
                if (z1 < z2) {
                    if (absolute_float(z2 - *(float *)(candidate_path_record + 0x18)) > limit)
                        goto next_point;
                } else {
                    if (absolute_float(z1 - *(float *)(candidate_path_record + 0x18)) > limit)
                        goto next_point;
                }
            }
            candidate_distance_xy = vector_distance_xy(((char *)&D_0013F3D0), path + i * 16 + 0x10);
            if (candidate_distance_xy < distance) {
                distance = candidate_distance_xy;
                best = i;
            }
        }
    next_point:;
    }
    qcopy(out, path + best * 16 + 0x10);
    qcopy(ground_probe_position, out);
    ground_probe_position[2] += 2.0f;
    out[2] = probe_ground_height(ground_probe_position, 0, 0.5f);
    clear_u64_value(out_rotation);
    {
        float *selected_path_record = (float *)(path + best * 16);
        float *heading_target_path_record = (float *)(path + (best + 1) * 16);
        out_rotation[2] = fast_arc_tan(heading_target_path_record[4] - selected_path_record[4],
                                       heading_target_path_record[5] - selected_path_record[5]);
    }
}

extern int  D_0015ED84_lui __asm__("D_0015ED84") MACRO_ADDR;
extern int  FUN_001f96f8_254608(int) __asm__("FUN_001f96f8");
typedef int u128_254608 __attribute__((mode(TI)));

typedef struct {
    float x, y, z, w;
} __attribute__((aligned(16))) Node_254608;

typedef struct {
    int         n;
    int         pad[3];
    Node_254608 p[1];
} Path;

typedef struct {
    char pad0[0x20];
    int  unk20;
    int  unk24;
    int  f28;
    int  f2C[5];
    int  f40;
} Data;

typedef struct {
    char          pad0[9];
    unsigned char f9;
} Flags;

extern unsigned char alternate_item_available_lui[] __asm__("D_0013D388") NOT_SDA;
extern unsigned char item_unlocked_lui[] __asm__("D_0013D408") NOT_SDA;
extern unsigned char D_0013D4DF_lui __asm__("D_0013D4DF") NOT_SDA;
extern int           D_0015EE38_macro __asm__("D_0015EE38") MACRO_ADDR;
extern int           D_0015EE3C_macro __asm__("D_0015EE3C") MACRO_ADDR;
extern int           freeze_stats_time_ref_macro[2] __asm__("D_0015EE58") MACRO_ADDR;
extern int           freeze_stats_score_ref_a_macro[2] __asm__("D_0015EE68") MACRO_ADDR;
extern void          set_race_camera(void *, void *, int, int) __asm__("FUN_L00_00216f90");
extern void          decrement_resource_counter(void) __asm__("FUN_001ff768");

#include "rnc/overlay/shared/hero/process_hero_primary_item_input_data/update_hero_hoverboard_race_progress.h"

extern float D_L05_0015F67C __attribute__((sda));
extern int   D_L05_0015F6A8 __attribute__((sda));
extern f32   frame_scale __asm__("D_0015ED60") MACRO_ADDR;
extern s32   pal_mode __asm__("D_0015ED80") MACRO_ADDR;
extern s32   current_bolt_count_macro __asm__("D_0015ED98") MACRO_ADDR;
/* Hoverboard race tick: advances the hero's and the opponents' path nodes and laps, handles wrong-way and fall resets, ranks everyone and finishes the race after three laps. */
void update_hero_hoverboard_race_progress(void) __asm__("FUN_L05_00254608");

void update_hero_hoverboard_race_progress(void) {
    float world_position_scratch[4] __attribute__((aligned(16)));
    float course_point_or_respawn_rotation[4] __attribute__((aligned(16)));
    Data *race_pvars;
    Path *path;
    int   near;
    int   j;
    int   k;
    int   comparison_opponent_index;
    float path_segment_heading;
    float wrong_way_reference_heading;

    hero.race_time_frames++;
    if (((struct Moby *)hero.hoverboard_moby) == 0) {
        return;
    }
    race_pvars = (Data *)((struct Moby *)hero.hoverboard_moby)->pvars;
    if (race_pvars->unk20 == -1) {
        return;
    }
    path = ((Path **)((char **)l05_paths))[race_pvars->unk20];
    hero.race_path = (void *)path;
    if (D_0015ED84_lui == 0x10) {
        *(u128_254608 *)world_position_scratch = 0;
        world_position_scratch[0] = 163.0f;
        world_position_scratch[1] = 119.0f;
        world_position_scratch[2] = 80.0f;
        if (fast_vec_dist(hero.motion.pos.f, world_position_scratch) < 11.0f) {
            if (hero.race_path_node < 3 || hero.race_path_node > 0x46) {
                if (hero.race_path_node >= 3) {
                    hero.race_lap++;
                }
                hero.race_path_node = 0xD;
            }
        }
        *(u128_254608 *)course_point_or_respawn_rotation = 0;
        course_point_or_respawn_rotation[0] = 223.0f;
        course_point_or_respawn_rotation[1] = 281.0f;
        course_point_or_respawn_rotation[2] = 93.0f;
        if (fast_vec_dist(hero.motion.pos.f, course_point_or_respawn_rotation) < 7.0f) {
            if (hero.race_path_node < 0x3B) {
                hero.race_path_node = 0x3B;
            }
        }
    }
    {
        int i;
        int n1;
        int next;

        i = hero.race_path_node;
        n1 = path->n + 1;
        next = (i + n1) % path->n;
        path_segment_heading =
            fast_arc_tan(path->p[next].x - path->p[i].x, path->p[next].y - path->p[i].y);
        if (fast_difference_between_rotations(path_segment_heading,
                                              fast_arc_tan(hero.motion.pos.f[0] - path->p[i].x,
                                                           hero.motion.pos.f[1] - path->p[i].y)) <
            1.5707964f) {
            hero.race_path_node = next;
            if (next == 0) {
                hero.race_lap++;
            }
        }
    }
    near = 0;
    if (D_0015ED84_lui == 0x10) {
        if (hero.motion.pos.f[2] < path->p[hero.race_path_node].z - 2.0f) {
            near = 1;
            if (hero.off_course_frames < FUN_001f96f8_254608(0x46)) {
                hero.off_course_frames = FUN_001f96f8_254608(0x46);
            }
        }
    }
    {
        int i;
        int n1;
        int next;

        i = hero.wrong_way_path_point;
        n1 = path->n + 1;
        next = (i + n1) % path->n;
        wrong_way_reference_heading =
            fast_arc_tan(path->p[next].x - path->p[i].x, path->p[next].y - path->p[i].y);
    }
    if (near != 0 || (fast_difference_between_rotations(hero.motion.rot.f[2],
                                                        wrong_way_reference_heading) > 1.5707964f &&
                      hero.trick_input == 0)) {
        hero.off_course_frames++;
        if (hero.off_course_frames > FUN_001f96f8_254608(0xB4)) {
            if (race_respawn_request_active == 0) {
                race_respawn_request_active = 1;
                race_respawn_gate_state = 0;
                D_L05_0015F67C = frame_scale * 0.08f;
            }
            hero.off_course_reset_pending = 1;
            if (race_respawn_gate_state == 1) {
                hero.off_course_reset_pending = 0;
                compute_hero_hoverboard_respawn_transform(world_position_scratch,
                                                          course_point_or_respawn_rotation);
                hero.off_course_frames = 0;
                hero.board_air_yaw = course_point_or_respawn_rotation[2];
                set_race_camera(world_position_scratch, course_point_or_respawn_rotation, 0x6B, 1);
                if (((Path *)hero.race_path) != 0) {
                    int r = select_point_index_by_distance(
                        hero.motion.pos.f, (Path *)hero.race_path, 0.0f);

                    if (r < hero.race_path_node) {
                        hero.race_path_node = r;
                    }
                }
            }
        }
    } else if (hero.off_course_reset_pending == 0) {
        hero.off_course_frames = 0;
    }
    for (j = 0; j < race_pvars->f40; j++) {
        int i;
        int n1;
        int next;

        i = hero.racer_path_point[j];
        n1 = path->n + 1;
        next = (i + n1) % path->n;
        path_segment_heading =
            fast_arc_tan(path->p[next].x - path->p[i].x, path->p[next].y - path->p[i].y);
        qcopy(world_position_scratch, &D_L05_0015FFD8_macro[race_pvars->f2C[j]].pos);
        if (fast_difference_between_rotations(
                path_segment_heading,
                fast_arc_tan(world_position_scratch[0] - path->p[i].x,
                             world_position_scratch[1] - path->p[i].y)) < 1.5707964f) {
            hero.racer_path_point[j] = next;
            if (next == 0) {
                hero.racer_laps[j]++;
            }
        }
    }
    hero.race_position = 1;
    for (j = 0; j < race_pvars->f40; j++) {
        if (hero.race_lap < hero.racer_laps[j] ||
            (hero.racer_laps[j] == hero.race_lap &&
             hero.racer_path_point[j] >= hero.race_path_node)) {
            hero.race_position++;
        }
    }
    for (k = 0; k < race_pvars->f40; k++) {
        hero.race_rank[k] = 1;
        if (hero.race_lap > hero.racer_laps[k] ||
            (hero.racer_laps[k] == hero.race_lap &&
             hero.race_path_node > hero.racer_path_point[k])) {
            hero.race_rank[k] = 2;
        }
        for (comparison_opponent_index = 0; comparison_opponent_index < race_pvars->f40;
             comparison_opponent_index++) {
            if (hero.racer_laps[k] < hero.racer_laps[comparison_opponent_index] ||
                (hero.racer_laps[k] == hero.racer_laps[comparison_opponent_index] &&
                 hero.racer_path_point[k] < hero.racer_path_point[comparison_opponent_index])) {
                hero.race_rank[k]++;
            }
        }
    }
    if (hero.race_lap < 3) {
        return;
    }
    if (*(alternate_item_available_lui) != 0) {
        D_L05_0015F6A8 = 1;
    }
    if (D_0015ED84_lui == 5) {
        D_0015EE38_macro++;
    } else if (D_0015ED84_lui == 0x10) {
        D_0015EE3C_macro++;
    }
    if (D_0015ED84_lui == 5) {
        int fast_finish_threshold_frames;

        if (pal_mode != 0) {
            fast_finish_threshold_frames = FUN_001f96f8_254608(0x1680);
        } else {
            fast_finish_threshold_frames = FUN_001f96f8_254608(0x1644);
        }
        if (fast_finish_threshold_frames >= hero.race_time_frames) {
            if (((Flags *)item_unlocked_lui)->f9 == 0) {
                ((Flags *)item_unlocked_lui)->f9 = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        if (race_pvars->f28 == -1) {
            return;
        }
        if (*(alternate_item_available_lui) == 0 && hero.race_position == 1) {
            D_L05_0015FFD8_macro[race_pvars->f28].command = 1;
        } else {
            hero.unk890 = (u8 *)(&D_L05_0015FFD8_macro[race_pvars->f28]);
            mode_freeze_init(0, 0);
        }
        if (freeze_stats_time_ref_macro[0] == 0) {
            freeze_stats_time_ref_macro[0] = hero.race_time_frames;
            current_bolt_count_macro += 0x3E8;
        } else if (hero.race_time_frames < freeze_stats_time_ref_macro[0]) {
            freeze_stats_time_ref_macro[0] = hero.race_time_frames;
            current_bolt_count_macro += 0x3E8;
        }
        if (freeze_stats_score_ref_a_macro[0] < hero.race_score) {
            freeze_stats_score_ref_a_macro[0] = hero.race_score;
            current_bolt_count_macro += 0x3E8;
        }
        D_L05_0015FFD8_macro[race_pvars->f28].update_dist = 0xFF;
        if (hero.trick_hud_handle != -1) {
            set_animation_parameter(hero.trick_hud_handle, 0);
            hero.trick_hud_handle = -1;
        }
        decrement_resource_counter();
    } else if (D_0015ED84_lui == 0x10) {
        if (race_pvars->f28 == -1) {
            return;
        }
        if (D_0013D4DF_lui == 0 && hero.race_position == 1) {
            D_L05_0015FFD8_macro[race_pvars->f28].command = 1;
        } else {
            hero.unk890 = (u8 *)(&D_L05_0015FFD8_macro[race_pvars->f28]);
            mode_freeze_init(0, 0);
        }
        if (freeze_stats_time_ref_macro[1] == 0) {
            freeze_stats_time_ref_macro[1] = hero.race_time_frames;
            current_bolt_count_macro += 0x3E8;
        } else if (hero.race_time_frames < freeze_stats_time_ref_macro[1]) {
            freeze_stats_time_ref_macro[1] = hero.race_time_frames;
            current_bolt_count_macro += 0x3E8;
        }
        if (freeze_stats_score_ref_a_macro[1] < hero.race_score) {
            freeze_stats_score_ref_a_macro[1] = hero.race_score;
            current_bolt_count_macro += 0x3E8;
        }
        D_L05_0015FFD8_macro[race_pvars->f28].update_dist = 0xFF;
        if (hero.trick_hud_handle != -1) {
            set_animation_parameter(hero.trick_hud_handle, 0);
            hero.trick_hud_handle = -1;
        }
        decrement_resource_counter();
    }
}

/* Ported from rac1-decomp (src/overlays/shared/help_00237B00.c: func_L05_002559A0), where it is exact; names translated to the US level program. */

int select_hero_hoverboard_animation_variant(void) {
    unsigned char animation_variant_selector = hero.race_ammo; // selected help mode
    int           animation_id = 0x7F;
    if (animation_variant_selector == 0)
        animation_id = 0x55;
    return animation_id;
}

int hero_select_anim_id_by_flags(void) __asm__("FUN_L05_002551d0");

int hero_select_anim_id_by_flags(void) {
    struct Hero *hero_ptr = &hero;
    int          hero_variant_selector = hero_ptr->race_ammo;
    if (hero_ptr->board_boost_active == 0) {
        int return_value = 0x7D;
        if (hero_variant_selector == 0)
            return_value = 0x52;
        return return_value;
    }
    { return hero_variant_selector == 0 ? 0x68 : 0x7E; }
}

extern s32   current_level_index __asm__("D_0015ED84") __attribute__((sda));
extern float frame_time __asm__("D_0015ED6C") MACRO_ADDR;
extern void  fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
extern int   offset_path_index(void *, int, int, int) __asm__("FUN_L00_0025d7a0");
extern int   D_L05_0015F698 __attribute__((sda));

int hero_pick_help_mode(void) __asm__("FUN_L05_00255208");

int hero_pick_help_mode(void) {
    struct Hero *player = &hero;
    float        start[4] __attribute__((aligned(16)));
    float        end[4] __attribute__((aligned(16)));
    float        velocity[4] __attribute__((aligned(16)));
    int          level;
    int          state;
    int          can_fall;
    if (frame_time * 12.0f < player->motion.flat_speed) {
        transform_hero_local_offset_to_world(start, 0.0f, 0.0f, 0.75f);
        qcopy(end, start);
        qcopy(velocity, &player->motion.velocity);
        fast_vec_normalize(velocity, velocity, 0.55f);
        fast_vec_add(velocity, velocity, &player->motion.velocity);
        velocity[2] = 0.0f;
        fast_vec_add(end, start, velocity);
        if (collision_line(start, end, 2, 0, 0) && D_L05_00174240.moby == 0 &&
            D_L05_00174240.unk1C != 0) {
            CollisionHit *wall = &D_L05_00174240;
            if (1.2217305f < fast_arc_tan(wall->normal_z, vector_length_xy(&wall->normal_x))) {
                player->unk8A4 =
                    fast_add_rotations(fast_arc_tan(wall->normal_x, wall->normal_y), 3.1415927f);
                if (fast_difference_between_rotations(
                        fast_arc_tan(player->motion.velocity.f[0], player->motion.velocity.f[1]),
                        player->unk8A4) < 0.7853982f) {
                    state = 0x6F;
                    if (!(player->ground_distance < 1.0f))
                        state = 0x6D;
                    goto change_state;
                }
            }
        }
    }
    level = current_level_index;
    if ((&hero)->wall_hit) {
        float distance = vector_length_xy(&(&hero)->motion.displacement);
        if (distance < vector_length_xy(&(&hero)->motion.velocity) * 0.25f) {
            state = 0x6D;
            goto change_state;
        }
        level = *(int *)0x0015ED84;
    }
    if (level == 5) {
        if ((&hero)->motion.pos.f[2] < 61.4f) {
            (&hero)->board_floor_z = 61.4f;
            state = 0x6E;
            goto change_state;
        }
    }
    if (level == 0x10) {
        if ((&hero)->motion.pos.f[2] < 76.0f) {
            (&hero)->board_floor_z = 76.0f;
            state = 0x6E;
            goto change_state;
        }
        if ((&hero)->on_slip_ground && (&hero)->ground_distance < 0.1f) {
            state = 0x6D;
            goto change_state;
        }
    }
    if ((&hero)->frames_off_ground.s == 0) {
        if ((&hero)->race_path != 0) {
            int index =
                select_point_index_by_distance(&(&hero)->motion.pos, (&hero)->race_path, 0.0f);
            int next = offset_path_index((&hero)->race_path, index, -1, 1);
            (&hero)->wrong_way_path_point = index;
            if (*(int *)0x0015ED84 == 5) {
                Path *path = (Path *)(&hero)->race_path;
                float height = path->p[index].z;
                float other = path->p[next].z;
                float distance;
                float minimum;
                if (height < other)
                    minimum = height;
                else
                    minimum = other;
                distance = vector_distance_xy(&(&hero)->motion.pos, &path->p[index]);
                if (0.5f < minimum - (&hero)->motion.pos.f[2] && 4.0f < distance) {
                    state = 0x6D;
                    goto change_state;
                }
            }
        }
        if ((&hero)->frames_off_ground.s == 0 && (&hero)->hoverboard_airborne &&
            (&hero)->board_jump_frames == 0 &&
            (*((unsigned char *)(&hero)->moby + 0x53) == 0x55 ||
             *((unsigned char *)(&hero)->moby + 0x53) == 0x7F) &&
            *((unsigned char *)(&hero)->moby + 0x51) < 13) {
            int animation = select_hero_hoverboard_animation_variant();
            int frames = scale_game_frames(7);
            set_hero_animation(animation, 14, (float)frames);
        }
    }
    can_fall = (&hero)->frames_off_ground.s == 0;
    if ((&hero)->wall_hit &&
        absolute_float((&hero)->wall_contact_point.f[2] - (&hero)->ground_z.f) < 0.05f)
        can_fall = 1;
    if (!can_fall)
        return 0;
    if ((&hero)->board_jump_frames == 0)
        return 0;
    if (!((&hero)->motion.velocity.f[2] < 0.0f))
        return 0;
    if (1.0471976f < absolute_float((&hero)->motion.rot.f[1])) {
        state = 0x6D;
        goto change_state;
    }
    if (1.0471976f < absolute_float((&hero)->motion.rot.f[0])) {
        state = 0x6D;
        goto change_state;
    }
    if (1.3962634f <
        fast_difference_between_rotations((&hero)->motion.rot.f[2], (&hero)->board_air_yaw)) {
        state = 0x6D;
        goto change_state;
    }
    if ((unsigned)(*((unsigned char *)(&hero)->moby + 0x53) - 0x69) < 4 &&
        (&hero)->anim_frame < (float)(&D_L05_0015F698)[(&hero)->air_pose_index]) {
        state = 0x6D;
        goto change_state;
    }
    goto continue_fall;
change_state:
    hero_set_state(state, 1);
    return 1;
continue_fall:
    if (alternate_item_available_lui[0]) {
        if (current_level_index == 5 && (&hero)->tricks_landed[4] == 4 && (&hero)->unk70C > 3 &&
            item_unlocked_lui[7] == 0) {
            item_unlocked_lui[7] = 1;
            allocate_voice_for_bank_entry(1, 0, 0);
            queue_dialogue_message(0x53D6, -1);
        }
        {
            struct Hero *player = &hero;
            *(int *)((char *)player + 0x714) = 0;
        }
    }
    if ((&hero)->state.current == 0x6C) {
        hero_set_state(0x6B, 0);
        if (*((unsigned char *)(&hero)->moby + 0x53) != 0x55 &&
            *((unsigned char *)(&hero)->moby + 0x53) != 0x7F) {
            int animation = select_hero_hoverboard_animation_variant();
            int frames = scale_game_frames(7);
            set_hero_animation(animation, 14, (float)frames);
        }
        return 1;
    }
    if (*((unsigned char *)(&hero)->moby + 0x53) != 0x55 &&
        *((unsigned char *)(&hero)->moby + 0x53) != 0x7F) {
        int animation = select_hero_hoverboard_animation_variant();
        int frames = scale_game_frames(7);
        set_hero_animation(animation, 14, (float)frames);
    }
    return 0;
}
/* Ported from rac1-decomp (src/overlays/shared/help_00237B00.c: func_L05_0025BDA8), where it is exact; names translated to the US level program. */

extern char hero_hoverboard_animation_link_table[] __asm__("D_L05_0017C068");

void hero_update_linked_target(void) __asm__("FUN_L05_0025b538");

void hero_update_linked_target(void) {
    char *hero_base = (char *)((unsigned char *)&D_0013E533) + 0xE1D;
    int   path_owner_moby = *(int *)(hero_base + 0x86C);

    *(int *)(hero_base + 0x2278) = 0;
    *(int *)(hero_base + 0x227C) = 0;
    if (path_owner_moby != 0) {
        if ((unsigned)(*(int *)(hero_base + 0x208C) - 0x15) < 2) {
            *(int *)(hero_base + 0x2278) = path_owner_moby;
            *(int *)(hero_base + 0x227C) = (int)hero_hoverboard_animation_link_table;
        }
    }
}

int D_L16_0015F674 = 0;

struct ScoreTable D_L16_001E65B8 = {
    {1.0f, 1.0f, 1.5f, 2.25f, 4.0f}
};

int D_L16_0015F698 = 0x19;

struct ScoreTable l05_hoverboard_score_multipliers_by_trick_count __asm__("D_L05_002169F8") = {
    {1.0f, 1.0f, 1.5f, 2.25f, 4.0f}
};
