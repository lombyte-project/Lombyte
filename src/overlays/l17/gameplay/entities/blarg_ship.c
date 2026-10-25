/* Level 17 moby classes: blarg_ship. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "rnc/gameplay/entities/moby.h"
#include "rnc/math_consts.h"
#include "rnc/globals.h"
#include "asm.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/level_labels.h"

/* Update for moby class 1470: watches camera boxes around the hero, gives skill points and help messages, and turns the camera. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F4210), where it is exact; names translated to the US level program. */

extern char *D_L17_0015FFDC;
extern char D_0013E52B[];
extern char D_L17_00167580[];
extern char D_L17_001E6F60[];
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float fast_difference_between_rotations(float, float) __asm__("FUN_001fa688");
extern int D_00141DC4 __attribute__((section(".data")));
extern int D_L17_00179D10[];
extern int help_request(int a, int b) __asm__("FUN_L00_00203908");
extern int hero_is_in_special_state(void) __asm__("FUN_L00_0020d568");
extern int allocate_voice_for_bank_entry(int, int, int) __asm__("FUN_0022db10");
extern int is_point_inside_clip_volume(void *arg0, int arg1) __asm__("FUN_00214720");
extern int scale_game_frames(int);
extern int tick_countdown_32();
extern unsigned char D_0013D408[];
extern unsigned char D_0013D408_b[] __asm__("D_0013D408");
extern unsigned char D_0013D4DF __attribute__((section(".data")));
extern int D_0015ED80_e[] __asm__("D_0015ED80") __attribute__((section(".sdata")));
extern void DebugPrint();
extern void clear_active_help_record(void) __asm__("FUN_L00_002039a0");
extern void queue_dialogue_message(int arg0, int arg1) __asm__("FUN_L00_00263d40");
extern void set_follow_camera_blend_output_flags(void) __asm__("FUN_L00_002e8680");
extern void set_follow_camera_angle_blend_from_position(int a, float x, float y) __asm__("FUN_L00_002e8970");
extern void enter_hero_idle_state_for_current_vehicle_mode(void) __asm__("FUN_L06_00239528");
extern int hero_set_state(int, int) __asm__("FUN_L17_0021e530");
extern char D_00141D54[];
extern char D_00141D7C[];
#include "rnc/gameplay/state/game_globals.h"
extern int func_001F9908_i(int *) __asm__("FUN_001f9740");

void update_fleet_camera_box_watcher(struct Moby *moby) __asm__("FUN_L17_002f26d0");

void update_fleet_camera_box_watcher(struct Moby *moby) {
    enum {
        CAMERA_BOX_WATCHER_INITIALIZE = 0,
        CAMERA_BOX_WATCHER_MONITOR = 1,
        CAMERA_BOX_WATCHER_ALIGN_FIRST_TARGET = 2,
        CAMERA_BOX_WATCHER_WAIT_AND_ALIGN_SECOND_TARGET = 3
    };
    enum {
        CLIP_VOLUME_SEQUENCE_WAIT_FOR_ENTRY = 0,
        CLIP_VOLUME_SEQUENCE_MONITOR_FOLLOWUP = 1
    };
    int clip_volume_entry_byte_offset;
    char *pvars = (char *)moby->pvars;
    int clip_volume_sequence_state;
    int i;
    int j;
    int found;
    int pending_help_clip_volume_index;
    int checked_moby_index;
    char *checked_moby;
    int *moby_indices;
    int *clip_volume_index_ptr;
    float target_heading;
    int next_moby_index;
    int mask;
    unsigned char *sk;
    int wrap;

    clip_volume_sequence_state = *(int *)(pvars + 0x6C);
    switch (clip_volume_sequence_state) {
    case CLIP_VOLUME_SEQUENCE_WAIT_FOR_ENTRY: {
        char *h = (char *)&hero.motion.pos;
        if (is_point_inside_clip_volume(h, *(int *)(pvars + 0x60))) {
            if (*(int *)(h + 0x200C) == 0x11) {
                *(int *)(pvars + 0x6C) = 1;
            }
        }
        break;
    }
    case CLIP_VOLUME_SEQUENCE_MONITOR_FOLLOWUP: {
        struct Hero *h = &hero;
        int hero_current_state = h->state.current;
        if (hero_current_state == 0x76 || hero_current_state == 0x32) {
            *(int *)(pvars + 0x6C) = 0;
        } else if (is_point_inside_clip_volume(&h->motion.pos, *(int *)(pvars + 0x64)) ||
                   is_point_inside_clip_volume(&h->motion.pos, *(int *)(pvars + 0x68))) {
            if (D_0013D408[0x1C] == 0) {
                D_0013D408[0x1C] = 1;
                allocate_voice_for_bank_entry(1, 0, 0);
                queue_dialogue_message(0x53D6, -1);
            }
        }
        break;
    }
    }

    if (D_0013D408[0x1B] == 0) {
        for (j = 0; j < 30; j++) {
            char *mb;
            sk = D_0013D408;
            mb = D_L17_0015FFD8 + (*(int *)(pvars + 0x70) << 8);
            if ((*(short *)(mb + 0xA6) == 0x2C || *(short *)(mb + 0xA6) == 0x566) &&
                *(signed char *)(mb + 0x20) >= 0 &&
                !is_point_inside_clip_volume(mb + 0x10, *(int *)(pvars + 0x78))) {
                *(int *)(pvars + 0x74) += 1;
            }
            next_moby_index = *(int *)(pvars + 0x70) + 1;
            wrap = D_L17_0015FFD8 + (next_moby_index << 8) < D_L17_0015FFDC;
            *(int *)(pvars + 0x70) = next_moby_index;
            if (!wrap) {
                if (*(int *)(pvars + 0x74) == 0 && sk[0x1B] == 0) {
                    sk[0x1B] = 1;
                    allocate_voice_for_bank_entry(1, 0, 0);
                    queue_dialogue_message(0x53D6, -1);
                }
                *(int *)(pvars + 0x70) = 0;
                *(int *)(pvars + 0x74) = 0;
            }
        }
    }

    switch (moby->state) {
    case CAMERA_BOX_WATCHER_INITIALIZE:
        moby->update_dist = 0xFF;
        *(int *)(pvars + 0x38) = scale_game_frames(600);
        *(unsigned char *)(pvars + 0x4C) = 0;
        if (*(int *)(pvars + 0x44) == -1 || *(int *)(pvars + 0x40) == -1 || *(int *)(pvars + 0x3C) == -1 ||
            *(int *)(pvars + 0x48) == -1) {
            DebugPrint(D_L17_001E6F60, *(int *)(pvars + 0x3C), *(int *)(pvars + 0x48), *(int *)(pvars + 0x40),
                       *(int *)(pvars + 0x44));
            *(unsigned char *)(pvars + 0x4C) = 1;
        }
        moby->state = 1;
        break;
    case CAMERA_BOX_WATCHER_MONITOR:
        pending_help_clip_volume_index = -1;
        checked_moby_index = -1;
        found = 0;
        for (i = 0; i < 1; i++) {
            clip_volume_entry_byte_offset = i * 4;
            if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(pvars + clip_volume_entry_byte_offset))) {
                found = 1;
                if (*(int *)(pvars + 0x30) != *(int *)(pvars + clip_volume_entry_byte_offset)) {
                    pending_help_clip_volume_index = *(int *)(pvars + clip_volume_entry_byte_offset);
                    moby_indices = (int *)(pvars + 0x10);
                    checked_moby_index = moby_indices[i];
                }
                break;
            }
        }
        if (found) {
            *(int *)(pvars + 0xC) += 1;
        } else {
            *(int *)(pvars + 0xC) = 0;
            *(int *)(pvars + 0x30) = -1;
        }
        if (checked_moby_index != -1) {
            checked_moby = D_L17_0015FFD8 + (checked_moby_index << 8);
            if (checked_moby == 0)
                goto skip1;
            if (*(short *)(checked_moby + 0xA6) != 0x4E)
                goto skip1;
            if (*(unsigned char *)(checked_moby + 0x20) == 0xFE)
                goto skip1;
            if (*(unsigned char *)(checked_moby + 0x20) == 0xFD)
                goto skip1;
            if (*(unsigned char *)(checked_moby + 0x20) == 3)
                goto skip1;
        }
        if (*(int *)(pvars + 0xC) > *(int *)(pvars + 0x38) && pending_help_clip_volume_index != -1 &&
            ((unsigned int)hero.state.control_mode < 2 ||
             hero.state.control_mode == 9) &&
            D_L17_00179D10[0] == 0 && D_L17_00179D10[9] == -1 && D_0013D4DF != 0) {
            *(int *)(pvars + 0x38) = scale_game_frames(0x708);
            if (*(int *)(((char *)&D_00141D54)) >= 0) {
                help_request(0x3A9A, 0x7D);
            }
            *(int *)(pvars + 0x30) = pending_help_clip_volume_index;
            *(int *)(pvars + 0xC) = 0;
        }
    skip1:
        if (is_point_inside_clip_volume(&hero.motion.pos, *(int *)(pvars + 0x20)) && D_L17_00179D10[0] == 0 &&
            D_L17_00179D10[9] == -1 && D_0013D408_b[0xBC] == 0) {
            int help_gate_flags = *(int *)(((char *)&D_00141D7C));
            mask = 1 << D_0015ED80_e[1];
            if (!(help_gate_flags & mask)) {
                help_request(0x3A9D, 0x82);
            }
        }
        pending_help_clip_volume_index = -1;
        found = 0;
        for (i = 0; i < 3; i++) {
            clip_volume_entry_byte_offset = i * 4;
            clip_volume_index_ptr = (int *)(pvars + 0x20 + clip_volume_entry_byte_offset);
            if (is_point_inside_clip_volume(&hero.motion.pos, *clip_volume_index_ptr)) {
                found = 1;
                if (*(int *)(pvars + 0x30) != *clip_volume_index_ptr) {
                    pending_help_clip_volume_index = *clip_volume_index_ptr;
                }
                break;
            }
        }
        if (!found || (*(int *)(pvars + 0x2C) += 1, hero.state.current == 0x35)) {
            *(int *)(pvars + 0x2C) = 0;
            *(int *)(pvars + 0x30) = -1;
        }
        if (scale_game_frames(0xE10) < *(int *)(pvars + 0x2C) && pending_help_clip_volume_index != -1 && hero_is_in_special_state() &&
            D_L17_00179D10[0] == 0 && D_L17_00179D10[9] == -1 && D_0013D408_b[0xBC] != 0) {
            help_request(0x4E2E, 0x78);
            *(int *)(pvars + 0x30) = pending_help_clip_volume_index;
            *(int *)(pvars + 0x2C) = 0;
        }
        if (*(int *)(pvars + 0x58) != 0xFF &&
            visited52[*(int *)(pvars + 0x58) + (current_level_index << 4)] == 0xFF) {
            *(unsigned char *)(pvars + 0x4C) = 1;
        }
        if (*(unsigned char *)(pvars + 0x4C) == 0 &&
            is_point_inside_clip_volume(&hero.motion.pos, *(int *)(pvars + 0x3C))) {
            clear_active_help_record();
            if (*(int *)(((char *)&D_00141DC4)) >= 0) {
                help_request(0x4268, 0x8B);
            }
            if (D_L17_00179D10[0] != 0 || D_L17_00179D10[9] != -1) {
                *(int *)(pvars + 0x50) = 0;
                moby->state = 2;
                *(int *)(pvars + 0x5C) = scale_game_frames(0xB4);
                hero_set_state(0x72, 0);
            }
        }
        break;
    case CAMERA_BOX_WATCHER_ALIGN_FIRST_TARGET: {
        char *cam;
        set_follow_camera_blend_output_flags();
        set_follow_camera_angle_blend_from_position((int)(D_L17_001600EC + (*(int *)(pvars + 0x44) << 7) + 0x30), 0.02617994f,
                         0.0f);
        cam = D_L17_00167580;
        target_heading = fast_arc_tan(*(float *)(D_L17_001600EC + ((*(int *)(pvars + 0x44) << 7) + 0x30)) -
                             *(float *)(cam + 0x140),
                         *(float *)(D_L17_001600EC + ((*(int *)(pvars + 0x44) << 7) + 0x34)) -
                             *(float *)(cam + 0x144));
        if (fast_difference_between_rotations(*(float *)(cam + 0x158), target_heading) < 0.034906585f) {
            moby->state = 3;
            *(int *)(pvars + 0x5C) = scale_game_frames(0x3C);
        }
        break;
    }
    case CAMERA_BOX_WATCHER_WAIT_AND_ALIGN_SECOND_TARGET:
        set_follow_camera_blend_output_flags();
        if (func_001F9908_i((int *)(pvars + 0x5C))) {
            char *cam;
            set_follow_camera_angle_blend_from_position((int)(D_L17_001600EC + (*(int *)(pvars + 0x40) << 7) + 0x30), 0.02617994f,
                             0.0f);
            cam = D_L17_00167580;
            target_heading = fast_arc_tan(*(float *)(D_L17_001600EC + ((*(int *)(pvars + 0x40) << 7) + 0x30)) -
                                 *(float *)(cam + 0x140),
                             *(float *)(D_L17_001600EC + ((*(int *)(pvars + 0x40) << 7) + 0x34)) -
                                 *(float *)(cam + 0x144));
            if (fast_difference_between_rotations(*(float *)(cam + 0x158), target_heading) < 0.034906585f) {
                enter_hero_idle_state_for_current_vehicle_mode();
                *(unsigned char *)(pvars + 0x4C) = 1;
                moby->state = 1;
            }
        }
        break;
    }
}
#include "sda.h"
#include "rnc/overlay/quad.h"
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F49B8), where it is exact; names translated to the US level program. */
extern void FUN_L00_002637f8_p() __asm__("FUN_L00_002637f8");

extern struct {
    int pad0[12];
    int v;
    int pad1[81];
    int tbl[3];
} D_L17_0016D160;
void update_fleet_guarded_moby_effects(struct Moby *moby) __asm__("FUN_L17_002f2e78");

void update_fleet_guarded_moby_effects(struct Moby *moby) {
    enum {
        MOBY_UPDATE_DISTANCE_INITIALIZE = 0,
        MOBY_GUARDED_TABLE_CALL = 1
    };
    switch (moby->state) {
    case MOBY_UPDATE_DISTANCE_INITIALIZE:
        moby->update_dist = 0xFF;
        moby->state = 1;
        break;
    case MOBY_GUARDED_TABLE_CALL:
        if (D_L17_0015F5C4 == 2) {
            int v = D_L17_0016D160.v;
            int i;
            if (v != 0 && v != D_L17_0015F5C4)
                return;
            i = 0;
            if (v == 0 || v == D_L17_0015F5C4)
                i = 2;
            FUN_L00_002637f8_p(D_L17_0016D160.tbl[i]);
        }
        break;
    }
}

/* Draws the Drek's Fleet reflection/backdrop quads around a moby. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F4E30), where it is exact; names translated to the US level program. */


typedef struct {
    OvlQuad m[4];
    int col[4];
    float uv[8];
    int q[8];
} Pkt;

extern char D_L17_001676C0[];
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern float frame_delta_time __asm__("D_0015ED7C");
extern float fast_cos_c(float) __asm__("FUN_001f9dc8");
extern float fast_sin_c(float) __asm__("FUN_001f9de0");
extern int fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern int get_effect_texture_alt(int) __asm__("FUN_001f44b8");
float D_L17_001623C0[4] __attribute__((section(".sdata"))) = {-0.5f, -1.0f, 1.0f, 1.0f};
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void cross_vectors_xyz(void *, void *, void *) __asm__("FUN_001f9ad8");
extern void draw_geometry_quad(void *, int, int) __asm__("FUN_001f7d30");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
void vu1_add_g_sregister_c(s32 register_id, s64 value) __asm__("FUN_00233980");

extern float D_L17_0016D3E4 __attribute__((section(".data")));
extern float D_L17_001DDFF8[];
extern float D_L17_001DE040[];
extern float D_L17_001DE0A0[];
extern float D_L17_001DE0B8[];
extern float D_L17_001DE0D0[];
extern float D_L17_001E6FA0[];
extern int D_L17_001DE010[];
extern int D_L17_001DE028[];
extern int D_L17_001DE088[];
int D_L17_001623B8 = 0x807e4931;
void draw_fleet_backdrop_quads(struct Moby *moby) __asm__("FUN_L17_002f32f0");

void draw_fleet_backdrop_quads(struct Moby *moby) {
    Pkt pkt;
    OvlQuad vv[4];
    char *data = (char *)moby->pvars;
    char *backdrop_endpoint_position;
    int i;
    int k;
    int j;
    OvlQuad *vb;
    float *pu;
    float *quad_vertex_v_coordinates;
    vu1_add_g_sregister_c(0x47, 0x513F1);
    vb = &vv[2];
    backdrop_endpoint_position = data + 0x10;
    pu = &pkt.uv[0];
    quad_vertex_v_coordinates = &pkt.uv[1];
    D_L17_0016D3E4 = 8192000.0f;
    *(float *)(data + 0x14) = -8000.0f;
    for (i = 0; i < 2; i++) {
        subtract_vector_xyz(vb, data, backdrop_endpoint_position);
        subtract_vector_xyz(&vv[0], data, D_L17_001676C0);
        subtract_vector_xyz(&vv[1], backdrop_endpoint_position, D_L17_001676C0);
        cross_vectors_xyz(&vv[0], &vv[0], vb);
        normalize_vector_xyz(&vv[0], &vv[0], D_L17_001DE0B8[i]);
        cross_vectors_xyz(&vv[1], &vv[1], vb);
        normalize_vector_xyz(&vv[1], &vv[1], *(float *)(data + 0x28));
        *(long *)&pkt.q[4] = 0x0000FF9000000260LL;
        *(long *)&pkt.q[6] = 0x0000008000000048LL;
        *(long *)&pkt.q[0] = 0;
        pkt.col[3] = D_L17_001623B8;
        pkt.col[2] = D_L17_001623B8;
        pkt.col[1] = D_L17_001623B8;
        pkt.col[0] = D_L17_001623B8;
        {
            float *qu = &pkt.uv[0];
            float *qv = &pkt.uv[1];
            int m;
            for (m = 0; m < 4; m++) {
                qu[m * 2] = D_L17_001E6FA0[m * 2];
                qv[m * 2] = D_L17_001E6FA0[m * 2 + 1];
            }
        }
        subtract_vector_xyz(&vv[3], data, &vv[0]);
        pkt.m[0] = vv[3];
        add_vector_xyz(&vv[3], data, &vv[0]);
        pkt.m[1] = vv[3];
        subtract_vector_xyz(&vv[3], backdrop_endpoint_position, &vv[1]);
        pkt.m[2] = vv[3];
        add_vector_xyz(&vv[3], backdrop_endpoint_position, &vv[1]);
        pkt.m[3] = vv[3];
        *(long *)&pkt.q[2] = get_effect_texture_alt(D_L17_001DE088[i]);
        for (j = 0; j < 4; j++) {
            pu[j * 2] = D_L17_001E6FA0[j * 2] + D_L17_001DE0A0[i];
            quad_vertex_v_coordinates[j * 2] = D_L17_001E6FA0[j * 2 + 1];
        }
        D_L17_001DE0A0[i] += D_L17_001623C0[i] * frame_delta_time;
        draw_geometry_quad(&pkt, 0, 0);
    }
    *(long *)&pkt.q[2] = get_effect_texture_alt(0x2F);
    for (k = 0; k < 5; k++) {
        float quad_side_length = D_L17_001DDFF8[k];
        float uv_rotation_angle = D_L17_001DE0D0[k];
        float x = *(float *)(data + 0x10) - quad_side_length * 0.5f;
        float z;
        ((float *)&pkt)[0] = x;
        ((float *)&pkt)[1] = *(float *)(data + 0x14) + 1000.0f;
        z = *(float *)(data + 0x18) + quad_side_length * 0.5f;
        ((float *)&pkt)[3] = 1.0f;
        ((float *)&pkt)[2] = z;
        pkt.m[3] = pkt.m[0];
        pkt.m[2] = pkt.m[0];
        pkt.m[1] = pkt.m[0];
        ((float *)&pkt)[0] = x + 0.0f;
        ((float *)&pkt)[2] = z + 0.0f;
        ((float *)&pkt)[4] = ((float *)&pkt)[4] + 0.0f;
        ((float *)&pkt)[6] = ((float *)&pkt)[6] - quad_side_length;
        ((float *)&pkt)[8] = ((float *)&pkt)[8] + quad_side_length;
        ((float *)&pkt)[10] = ((float *)&pkt)[10] + 0.0f;
        ((float *)&pkt)[12] = ((float *)&pkt)[12] + quad_side_length;
        ((float *)&pkt)[14] = ((float *)&pkt)[14] - quad_side_length;
        pkt.uv[0] = fast_sin_c(fast_add_rotations(uv_rotation_angle, 4.712389f)) * 0.5f + 0.5f;
        pkt.uv[1] = fast_cos_c(fast_add_rotations(uv_rotation_angle, 4.712389f)) * 0.5f + 0.5f;
        pkt.uv[2] = fast_sin_c(fast_add_rotations(uv_rotation_angle, 3.1415927f)) * 0.5f + 0.5f;
        pkt.uv[3] = fast_cos_c(fast_add_rotations(uv_rotation_angle, 3.1415927f)) * 0.5f + 0.5f;
        pkt.uv[4] = fast_sin_c(uv_rotation_angle) * 0.5f + 0.5f;
        pkt.uv[5] = fast_cos_c(uv_rotation_angle) * 0.5f + 0.5f;
        pkt.uv[6] = fast_sin_c(fast_add_rotations(uv_rotation_angle, 1.5707964f)) * 0.5f + 0.5f;
        pkt.uv[7] = fast_cos_c(fast_add_rotations(uv_rotation_angle, 1.5707964f)) * 0.5f + 0.5f;
        {
            int interpolated_vertex_color = fast_tween_color(D_L17_001DE010[k], D_L17_001DE028[k], D_L17_001DE040[k]);
            pkt.col[3] = interpolated_vertex_color;
            pkt.col[2] = interpolated_vertex_color;
            pkt.col[1] = interpolated_vertex_color;
            pkt.col[0] = interpolated_vertex_color;
        }
        draw_geometry_quad(&pkt, 0, 0);
    }
    vu1_add_g_sregister_c(0x47, 0x5360B);
}
#include "qcopy.h"

/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F5388), where it is exact; names translated to the US level program. */


#include "rnc/overlay/l17/gameplay/entities/blarg_ship_globals.h"
extern float backdrop_shared_radius_numerator_gp __asm__("D_L17_001623D0") __attribute__((sda));
extern float backdrop_offset_radius_divisor_gp __asm__("D_L17_001623D8") __attribute__((sda));
extern void FUN_L17_002f32f0_c(char *) __asm__("FUN_L17_002f32f0");
extern void enqueue_callback_list_1_alt(void (*)(void), void *) __asm__("FUN_001f4600");
s32 is_point_inside_clip_volume_u(s32 arg0, s32 arg1) __asm__("FUN_00214720");
void build_spherical_offset(f32 *out, f32 scale, f32 a, f32 b) __asm__("FUN_00214db0");

extern float D_L17_001DDFE0[];
extern float D_L17_001DE058[];
extern float D_L17_001DE070[];
extern float backdrop_heading_radius_numerator_addend_gp __asm__("D_L17_001623D4") __attribute__((sda));
void update_fleet_backdrop_animation(struct Moby *moby) __asm__("FUN_L17_002f3848");

void update_fleet_backdrop_animation(struct Moby *moby) {
    char *moby_pvars = (char *)moby->pvars;
    float offset_accumulator[4];
    float pvar_angle_offset[4];
    float vector_sum[4];
    int i;
    if (*(int *)(moby_pvars + 0x4C) != -1) {
        struct Hero *hero_data = &hero;
        if (hero_data->state.current != 0x32 || ((char *)hero_data->ship_moby) == 0 ||
            *(short *)(((char *)hero_data->ship_moby) + 0xA6) != 0x563) {
            if (is_point_inside_clip_volume_u(&hero.motion.pos, *(int *)(moby_pvars + 0x4C))) {
                D_L17_0015F638 = 150.0f;
                goto done;
            }
        }
        D_L17_0015F638 = 10.0f;
    }
done:
    if (moby->state == 0) {
        moby->state = 1;
        for (i = 0; i < 5; i++) {
            D_L17_001DE0D0[i] = (float)i * 25.0f;
        }
        qcopy(moby_pvars, &moby->pos);
        build_spherical_offset(
            offset_accumulator, (backdrop_heading_radius_numerator_addend_gp + backdrop_shared_radius_numerator_gp) / backdrop_offset_radius_divisor_gp,
            moby->rot.z, 0.0f);
        build_spherical_offset(pvar_angle_offset, backdrop_shared_radius_numerator_gp / backdrop_offset_radius_divisor_gp,
                               *(float *)(moby_pvars + 0x44), 0.0f);
        add_vector_xyz(vector_sum, offset_accumulator, pvar_angle_offset);
        *(OvlQuad *)offset_accumulator = *(OvlQuad *)vector_sum;
        add_vector_xyz(vector_sum, moby_pvars, offset_accumulator);
        *(OvlQuad *)(moby_pvars + 0x10) = *(OvlQuad *)vector_sum;
        moby->update_dist = 0xFF;
        *(float *)(moby_pvars + 0x10) -= 300.0f;
    } else {
        for (i = 0; i < 5; i++) {
            D_L17_001DE040[i] += D_L17_001DE070[i] * D_L17_001DE058[i] * frame_delta_time;
            if (1.0f < D_L17_001DE040[i]) {
                D_L17_001DE040[i] = 1.0f;
                D_L17_001DE058[i] = -1.0f;
            }
            if (D_L17_001DE040[i] < 0.0f) {
                D_L17_001DE040[i] = 0.0f;
                D_L17_001DE058[i] = 1.0f;
            }
        }
        for (i = 0; i < 5; i++) {
            D_L17_001DE0D0[i] =
                fast_add_rotations(D_L17_001DE0D0[i], D_L17_001DDFE0[i] * 0.1f * frame_delta_time);
        }
        for (i = 0; i < 2; i++) {
            D_L17_001DE0A0[i] += D_L17_001623C0[i] * frame_delta_time;
            if (1.0f <= D_L17_001DE0A0[i]) {
                D_L17_001DE0A0[i] -= 1.0f;
            }
            if (D_L17_001DE0A0[i] < 0.0f) {
                D_L17_001DE0A0[i] += 1.0f;
            }
        }
        enqueue_callback_list_1_alt(FUN_L17_002f32f0_c, moby);
    }
}
#include "sda.h"

#include "qcopy.h"

/* UpdateMoby 1843: a flying fleet craft that circles, follows a path, strafes the player and fires. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F5C18), where it is exact; names translated to the US level program. */

extern char *spawn_class_3f9_moby(int owner, float *dir, float *pos, float size, float len, float z) __asm__("FUN_L11_00308f48");
extern char D_L17_001676C0[];
extern char D_L17_001DE110[];
extern char D_L17_001DE120[];
extern f32 advance_accelerated_scalar(f32 *, f32 *, f32, f32, f32, f32) __asm__("func_00213F38");
extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern f32 fast_add_rotations(f32 a, f32 b) __asm__("FUN_001fa580");
extern f32 vector_length_xy(void *) __asm__("FUN_001f9b20");
extern f32 vector_length_xyz(void *a) __asm__("FUN_001f9af0");
extern float FUN_001f9b48(void *, void *);
extern float fast_arc_tan(float, float) __asm__("FUN_001f9e90");
extern float randf_sym(float lo, float hi) __asm__("FUN_L00_00257c48");
extern float approach_angle(float *p, float *v, float a, float b, float c, float d) __asm__("FUN_L00_0025be00");
extern float fast_cos(float) __asm__("func_001F9DC8");
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float random_angle_radians(void) __asm__("FUN_00213308");
extern float random_float_between_alt(float, float) __asm__("FUN_002132a8");
extern int fast_bsphere_check(char *, float) __asm__("FUN_001fa728");
extern int FUN_0022da68(int, int, int);
extern int random_integer_below_c(int) __asm__("FUN_00213260");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern unsigned char *spawn_ship_pickup(char *position, int moby_class) __asm__("FUN_L13_002e1c50");
extern void cart_burst(void *, void *, int, float, float) __asm__("FUN_L00_0025f090");
extern void FUN_L00_0025f730(void *a, float x);
extern void update_moby_trail_points(void *) __asm__("FUN_L11_00319670");
extern void update_moby_hit_stats_and_state(void *) __asm__("FUN_L11_0031a1c0");
extern void update_moby_path_transform(void *) __asm__("FUN_L11_0031a438");
extern void add_vector_xyz(void *, void *, void *) __asm__("FUN_001f9a10");
extern void normalize_vector_xyz(void *out, void *a, f32 len) __asm__("FUN_001f9bf8");
extern void subtract_vector_xyz(void *out, void *a, void *b) __asm__("FUN_001f9a28");
extern void transform_vector_by_basis(void *, void *, void *) __asm__("func_001F9CF8");
extern void vu_euler_rotation_basis(void *, void *) __asm__("func_001FA030");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

extern float blarg_ship_firing_speed_gp __asm__("D_L17_001623E8") __attribute__((sda));
void blarg_ship_update(struct Moby *moby) __asm__("FUN_L17_002f40d8");
void blarg_ship_update(struct Moby *moby) {
    enum {
        BLARG_SHIP_INITIALIZE = 0,
        BLARG_SHIP_FOLLOW_PATH = 1,
        BLARG_SHIP_BURST_AND_HIDE = 2,
        BLARG_SHIP_WAIT_TO_REACTIVATE = 3,
        BLARG_SHIP_INACTIVE = 4,
        BLARG_SHIP_TRACK_HERO_AND_FIRE = 5,
        BLARG_SHIP_BURST_AND_DEACTIVATE = 6
    };
    float scratch_vector[4];
    float movement_target_position[4];
    float hero_position_delta[4];
    float clamped_euler_angles[4];
    float rotation_basis_row0[4];
    float rotation_basis_row1[4];
    float rotation_basis_row2[4];
    float hero_relative_position[4];
    float firing_target_delta[4];
    float firing_direction[4];
    float sA0[4];
    float sB0[4];
    float movement_distance_accumulator;
    char *moby_pvars = (char *)moby->pvars;
    float offset_angle;
    float movement_target_distance;
    char *firing_moby;

    update_moby_hit_stats_and_state(moby);
    switch (moby->state) {
    case BLARG_SHIP_INITIALIZE:
        moby->draw_dist = 0xFF;
        moby->update_dist = 0xFF;
        if (*(int *)(moby_pvars + 0x88) == 0) {
            moby->state = 1;
            *(float *)(moby_pvars + 0x6C) = randf_sym(0.3f, 0.5f);
            *(float *)(moby_pvars + 0x70) = random_angle_radians();
            *(float *)(moby_pvars + 0x74) = randf_sym(60.0f, 120.0f) * DEG_TO_RAD * frame_time;
            *(float *)(moby_pvars + 0x78) = random_float_between_alt(8.0f, 12.0f);
        } else {
            moby->state = 4;
            moby->collision = 0;
            moby->flags |= 1;
        }
        break;
    case BLARG_SHIP_FOLLOW_PATH:
    case BLARG_SHIP_BURST_AND_HIDE:
        update_moby_trail_points(moby);
        *(float *)(moby_pvars + 0x68) += *(float *)(moby_pvars + 0x6C);
        if (*(float *)(moby_pvars + 0x68) > 1.0f) {
            *(float *)(moby_pvars + 0x68) -= 1.0f;
            *(int *)(moby_pvars + 0x64) =
                (*(int *)(moby_pvars + 0x64) + 1) % **(int **)(D_L17_001B0DB0 + *(int *)(moby_pvars + 0x60) * 4);
        } else if (*(float *)(moby_pvars + 0x68) < 0.0f) {
            *(float *)(moby_pvars + 0x68) += 1.0f;
            *(int *)(moby_pvars + 0x64) =
                (*(int *)(moby_pvars + 0x64) + **(int **)(D_L17_001B0DB0 + *(int *)(moby_pvars + 0x60) * 4) - 1) %
                **(int **)(D_L17_001B0DB0 + *(int *)(moby_pvars + 0x60) * 4);
        }
        fast_add_rotations(*(float *)(moby_pvars + 0x70), *(float *)(moby_pvars + 0x74));
        update_moby_path_transform(moby);
        if (moby->state == 2) {
            cart_burst(moby, &moby->pos, -1, 2.0f, 13.0f);
            if (random_integer_below_c(2)) {
                spawn_ship_pickup((char *)&moby->pos, 0xE0);
            } else {
                spawn_ship_pickup((char *)&moby->pos, 0xE4);
            }
            FUN_0022da68(1, 0, (int)moby);
            moby->state = 3;
            moby->collision = 0;
            moby->flags |= 1;
        } else if (moby->visible == 0) {
            if (FUN_001f9b48(&moby->pos, &hero.motion.pos) < 32.0f) {
                moby->state = 3;
                moby->collision = 0;
                moby->flags |= 1;
            }
        }
        break;
    case BLARG_SHIP_WAIT_TO_REACTIVATE:
        qcopy(scratch_vector, &moby->pos);
        scratch_vector[3] = 3.0f;
        if (fast_bsphere_check((char *)&moby->pos, 512.0f) == -1) {
            if (FUN_001f9b48(&moby->pos, &hero.motion.pos) > 32.0f) {
                *(int *)(moby_pvars + 0x84) += 1;
                if (scale_game_frames(60) < *(int *)(moby_pvars + 0x84)) {
                    moby->state = 1;
                    if (moby->prev_seq) {
                        blend_moby_animation(moby, 0, 0, scale_game_frames(10));
                    }
                    moby->flags &= 0xFFFE;
                    moby->collision = moby->pclass->collision;
                }
            } else {
                *(int *)(moby_pvars + 0x84) = 0;
            }
        } else {
            *(int *)(moby_pvars + 0x84) = 0;
        }
        break;
    case BLARG_SHIP_INACTIVE:
        break;
    case BLARG_SHIP_TRACK_HERO_AND_FIRE:
        movement_distance_accumulator = 0.0f;
        qcopy(clamped_euler_angles, D_L17_001676D0);
        clamped_euler_angles[0] = 0.0f;
        if (clamped_euler_angles[1] > 0.2617994f) {
            clamped_euler_angles[1] = 0.2617994f;
        } else if (clamped_euler_angles[1] < -0.2617994f) {
            clamped_euler_angles[1] = -0.2617994f;
        }
        vu_euler_rotation_basis(rotation_basis_row0, clamped_euler_angles);
        subtract_vector_xyz(hero_position_delta, &hero.motion.pos, moby_pvars + 0xB0);
        qcopy(moby_pvars + 0xB0, &hero.motion.pos);
        add_vector_xyz(&moby->pos, &moby->pos, hero_position_delta);
        offset_angle = fast_add_rotations(0.7853982f, (float)*(int *)(moby_pvars + 0x88) * 1.5707964f);
        normalize_vector_xyz(scratch_vector, rotation_basis_row1, fast_sin(offset_angle) * 8.0f);
        add_vector_xyz(movement_target_position, D_L17_001676C0, scratch_vector);
        normalize_vector_xyz(scratch_vector, rotation_basis_row2, fast_cos(offset_angle) * 8.0f);
        add_vector_xyz(movement_target_position, movement_target_position, scratch_vector);
        normalize_vector_xyz(scratch_vector, rotation_basis_row0, 20.0f);
        add_vector_xyz(movement_target_position, movement_target_position, scratch_vector);
        subtract_vector_xyz(scratch_vector, movement_target_position, &moby->pos);
        movement_target_distance = vector_length_xyz(scratch_vector);
        if (*(char *)&hero.ship_flags & 2) {
            advance_accelerated_scalar(&movement_distance_accumulator, (float *)(moby_pvars + 0x6C), movement_target_distance, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, frame_time);
        } else if (moby->command) {
            advance_accelerated_scalar(&movement_distance_accumulator, (float *)(moby_pvars + 0x6C), movement_target_distance, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, frame_time * 15.0f);
        } else {
            advance_accelerated_scalar(&movement_distance_accumulator, (float *)(moby_pvars + 0x6C), movement_target_distance, frame_time_sq + frame_time_sq,
                                       frame_time_sq * 20.0f, frame_time * 5.0f);
        }
        FUN_L00_0025f730(scratch_vector, *(float *)(moby_pvars + 0x6C));
        add_vector_xyz(&moby->pos.x, &moby->pos.x, scratch_vector);
        if (moby->command) {
            if (moby->visible) {
                *(int *)(moby_pvars + 0x98) += 1;
            }
            if (FUN_001f9b48(movement_target_position, &moby->pos.x) < 2.0f) {
                moby->command = 0;
            } else if (scale_game_frames(120) < *(int *)(moby_pvars + 0x98)) {
                moby->command = 0;
            }
        } else {
            float *hero_position = (float *)(&hero.motion.pos);
            float *yaw = &moby->rot.z;
            float *pitch = &moby->rot.y;
            float target_rotation_angle;
            subtract_vector_xyz(hero_relative_position, hero_position, &moby->pos.x);
            target_rotation_angle = fast_arc_tan(hero_relative_position[0], hero_relative_position[1]);
            approach_angle(yaw, (float *)(moby_pvars + 0x8C), target_rotation_angle, frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f, frame_time * 12.566371f);
            target_rotation_angle = -fast_arc_tan(vector_length_xy(hero_relative_position), hero_relative_position[2]);
            approach_angle(pitch, (float *)(moby_pvars + 0x90), target_rotation_angle, frame_time_sq * 12.566371f,
                             frame_time_sq * 12.566371f, frame_time * 12.566371f);
            if (D_L17_0015F5CC % 10 == 0) {
                FUN_0022da68(2, 0, (int)moby);
                approach_value((float *)(moby_pvars + 0x94), 0.0f, 0.25f);
                normalize_vector_xyz(sB0, hero_position_delta, *(float *)(moby_pvars + 0x94));
                add_vector_xyz(sB0, sB0, hero_position);
                if (D_L17_0015F5CC % 20 == 0) {
                    transform_vector_by_basis(sA0, D_L17_001DE110, &moby->rot_row0);
                } else {
                    transform_vector_by_basis(sA0, D_L17_001DE120, &moby->rot_row0);
                }
                add_vector_xyz(sA0, sA0, &moby->pos.x);
                subtract_vector_xyz(firing_target_delta, sB0, sA0);
                normalize_vector_xyz(firing_direction, firing_target_delta, blarg_ship_firing_speed_gp * frame_time);
                add_vector_xyz(firing_direction, firing_direction, hero_position_delta);
                firing_moby = spawn_class_3f9_moby((int)moby, firing_direction, sA0, 128.0f, -1.0f, 10.0f);
                if (firing_moby) {
                    subtract_vector_xyz(firing_direction, firing_direction, hero_position_delta);
                    *(float *)(firing_moby + 0x44) = -fast_arc_tan(vector_length_xy(firing_direction), firing_direction[2]);
                    *(float *)(firing_moby + 0x48) = fast_arc_tan(firing_direction[0], firing_direction[1]);
                }
            }
            if (moby->visible == 0) {
                moby->state = 4;
                moby->collision = 0;
                moby->flags |= 1;
            }
        }
        break;
    case BLARG_SHIP_BURST_AND_DEACTIVATE:
        subtract_vector_xyz(scratch_vector, &hero.motion.pos, moby_pvars + 0xB0);
        qcopy(moby_pvars + 0xB0, &hero.motion.pos);
        add_vector_xyz(&moby->pos, &moby->pos, scratch_vector);
        cart_burst(moby, &moby->pos, -1, 2.0f, 13.0f);
        if (random_integer_below_c(2)) {
            spawn_ship_pickup((char *)&moby->pos, 0xE0);
        } else {
            spawn_ship_pickup((char *)&moby->pos, 0xE4);
        }
        FUN_0022da68(1, 0, (int)moby);
        moby->state = 4;
        moby->collision = 0;
        moby->flags |= 1;
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

#include "qcopy.h"

/* Finds the listed moby of type 0x733 in state 4 and launches it along a rotated vector. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6AF8), where it is exact; names translated to the US level program. */

extern char D_0013E533[];
extern char D_L17_001676C0[];
extern float fast_cos(float) __asm__("FUN_001f9dc8");
extern float fast_sin(float) __asm__("FUN_001f9de0");
extern float fast_add_rotations(float, float) __asm__("FUN_001fa580");
extern int scale_game_frames(int) __asm__("FUN_001f96f8");
extern void fast_vec_add(void *, void *, void *) __asm__("FUN_001f9a10");
extern void euler_rotation_basis(void *, void *) __asm__("FUN_001fa030");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");
s32 random_integer_below(s32 arg0) __asm__("FUN_00213260");
void blend_moby_animation(MobyAnim *arg0, int arg1, int arg2, int arg3) __asm__("FUN_00212f90");

char *try_activate_blarg_ship_from_list(int list) __asm__("FUN_L17_002f4fb8");

char *try_activate_blarg_ship_from_list(int list) {
    short *moby_index_ptr = D_L17_001AC140[list];
    float A[4], B[4], C[4], D[4], E[4], F[4];
    char *selected_moby;
    char *moby_pvars;
    int moby_byte_offset;
    do {
        moby_byte_offset = (*moby_index_ptr & 0x7FFF) << 8;
        if (*(short *)(0xA6 + moby_byte_offset + D_L17_0015FFD8) != 0x733)
            continue;
        if (*(unsigned char *)(0x20 + moby_byte_offset + D_L17_0015FFD8) != 4)
            continue;
        if (random_integer_below(10))
            continue;
        selected_moby = D_L17_0015FFD8 + moby_byte_offset;
        moby_pvars = *(char **)(selected_moby + 0x78);
        qcopy(C, D_L17_001676D0);
        C[0] = 0;
        if (C[1] > 0.17453292f)
            C[1] = 0.17453292f;
        else if (C[1] < -0.17453292f)
            C[1] = -0.17453292f;
        euler_rotation_basis(D, C);
        {
            float offset_angle = fast_add_rotations(0.7853982f, (float)*(int *)(moby_pvars + 0x88) * 1.5707964f);
            fast_vec_normalize(A, E, fast_sin(offset_angle) * 12.0f);
            fast_vec_add(B, D_L17_001676C0, A);
            fast_vec_normalize(A, F, fast_cos(offset_angle) * 12.0f);
        }
        fast_vec_add(B, B, A);
        fast_vec_normalize(A, D, 6.0f);
        fast_vec_add(B, B, A);
        qcopy(moby_pvars + 0xB0, D_0013E533 + 0xE9D);
        qcopy(selected_moby + 0x10, B);
        selected_moby[0x20] = 5;
        selected_moby[0xBC] = 1;
        *(float *)(moby_pvars + 0x6C) = frame_time * 20.0f;
        *(float *)(moby_pvars + 0x94) = 5.0f;
        if (*(unsigned char *)(selected_moby + 0x53))
            blend_moby_animation(selected_moby, 0, 0, scale_game_frames(10));
        *(unsigned short *)(selected_moby + 0x34) &= 0xFFFE;
        *(int *)(selected_moby + 0x94) = *(int *)(*(char **)(selected_moby + 0x24) + 0x10);
        return selected_moby;
    } while (*moby_index_ptr++ >= 0);
    return 0;
}
/* UpdateMoby 1873: wait for a target, then play two effects and delete itself. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6DA0), where it is exact; names translated to the US level program. */

extern char *find_moby_hit(void *, int, int) __asm__("FUN_L00_0025a420");
extern int FUN_0022da68(int, int, int);
extern void FUN_L00_00257470(void *, int, int);
extern void spawn_moby_break_fragments(void *, int, int, int, int, int, int) __asm__("FUN_L00_00263e30");
extern void spawn_proximity_gated_bolts(void *) __asm__("FUN_L01_002787a0");
extern void spawn_moby_from_placement(void *, int) __asm__("FUN_L01_00278e20");
void mark_moby_for_removal(struct Obj *obj) __asm__("FUN_0020c828");

void update_hit_triggered_fragment_spawner(struct Moby *moby) __asm__("FUN_L17_002f5260");

void update_hit_triggered_fragment_spawner(struct Moby *moby) {
    enum {
        HIT_EFFECT_INITIALIZE = 0,
        HIT_EFFECT_WAIT_FOR_HIT = 1,
        HIT_EFFECT_EMIT_AND_REMOVE = 2
    };
    int flag = 0;
    char *hit_result = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case HIT_EFFECT_INITIALIZE:
        moby->state = 1;
        break;
    case HIT_EFFECT_WAIT_FOR_HIT:
        if (hit_result && *(float *)(hit_result + 0x2C) > 0.0f)
            flag = 1;
        if (flag)
            moby->state = 2;
        break;
    case HIT_EFFECT_EMIT_AND_REMOVE:
        FUN_0022da68(0, 0, (int)moby);
        FUN_L00_00257470(moby, 0, -1);
        spawn_proximity_gated_bolts(moby);
        spawn_moby_break_fragments(moby, 0x753, 1, 0x753, 1, 11, 2);
        spawn_moby_break_fragments(moby, 0x756, 1, 0x756, 1, 1, 2);
        spawn_moby_from_placement(moby, 0x752);
        mark_moby_for_removal(moby);
        break;
    }
}
/* Update moby 1876: waits for a trigger, then plays an effect and finishes. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F6ED0), where it is exact; names translated to the US level program. */

extern s32 allocate_voice_for_target_entry(s32, s32, void *) __asm__("func_0022DA68");
extern void *FUN_L00_00263fd8(char *src, int cls, float *pos, void *mat, int a8, int a9, float *v10,
                              float *v11, float scale, float *v12);
void mark_moby_for_removal_c(void *obj) __asm__("FUN_0020c828");

extern float D_L17_0015F580_macro[] __asm__("D_L17_0015F580") __attribute__((section(".sdata")));
void update_hit_triggered_fragment_and_placement_spawner(struct Moby *moby) __asm__("FUN_L17_002f5390");

void update_hit_triggered_fragment_and_placement_spawner(struct Moby *moby) {
    enum {
        HIT_EFFECT_INITIALIZE = 0,
        HIT_EFFECT_WAIT_FOR_HIT = 1,
        HIT_EFFECT_EMIT_AND_REMOVE = 2
    };
    int flag = 0;
    char *hit_result = find_moby_hit(moby, 0x10000, 0);
    switch (moby->state) {
    case HIT_EFFECT_INITIALIZE:
        moby->state = 1;
        break;
    case HIT_EFFECT_WAIT_FOR_HIT:
        if (hit_result) {
            if (*(float *)(hit_result + 0x2C) > 0.0f)
                flag = 1;
        }
        if (flag)
            moby->state = 2;
        break;
    case HIT_EFFECT_EMIT_AND_REMOVE:
        allocate_voice_for_target_entry(0, 0, (int)moby);
        FUN_L00_00257470(moby, 0, -1);
        spawn_proximity_gated_bolts(moby);
        FUN_L00_00263fd8((char *)moby, 0x753, &moby->pos.x, &moby->rot, 0, 0, D_L17_0015F580_macro,
                         D_L17_0015F580_macro, 0.0f, D_L17_0015F580_macro);
        spawn_moby_break_fragments(moby, 0x756, 1, 0x756, 1, 1, 2);
        spawn_moby_from_placement(moby, 0x755);
        mark_moby_for_removal_c(moby);
        break;
    }
}
#define NOT_SDA

#define MACRO_ADDR

/* Projects each direction pair into 2D texture coordinates (reflection mapping). */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F7018), where it is exact; names translated to the US level program. */

typedef struct {
    float x, y, z;
} V3;

typedef struct {
    V3 v;
    float w;
} V4;

extern char D_L17_001676C0[];
extern float fast_vec_dot(void *, void *) __asm__("FUN_001f9ab0");
extern float fast_vec_length(void *) __asm__("FUN_001f9af0");
extern void fast_vec_sub(void *, void *, void *) __asm__("FUN_001f9a28");
extern void fast_vec_scale(void *, void *, float) __asm__("FUN_001f9a68");
extern void fast_vec_normalize(void *, void *, float) __asm__("FUN_001f9bf8");

void compute_reflection_sphere_map_uvs(int vertex_count, V3 *input_directions, V3 *vertex_positions, int unused, float *out) __asm__("FUN_L17_002f54d8");

void compute_reflection_sphere_map_uvs(int vertex_count, V3 *input_directions, V3 *vertex_positions, int unused, float *out) {
    V4 normalized_position_delta;
    V4 vertex_position;
    V4 reflection_mapping_vector;
    V4 normalized_input_direction;
    int i;
    for (i = 0; i < vertex_count; i++) {
        float direction_dot_product;
        float sphere_map_denominator;
        vertex_position.w = 1.0f;
        vertex_position.v.x = vertex_positions[i].x;
        vertex_position.v.y = vertex_positions[i].y;
        vertex_position.v.z = vertex_positions[i].z;
        fast_vec_sub(&normalized_position_delta, &vertex_position, D_L17_001676C0);
        fast_vec_normalize(&normalized_position_delta, &normalized_position_delta, 1.0f);
        normalized_input_direction.w = 1.0f;
        normalized_input_direction.v.x = input_directions[i].x;
        normalized_input_direction.v.y = input_directions[i].y;
        normalized_input_direction.v.z = input_directions[i].z;
        fast_vec_normalize(&normalized_input_direction, &normalized_input_direction, 1.0f);
        direction_dot_product = fast_vec_dot(&normalized_input_direction, &normalized_position_delta);
        fast_vec_scale(&reflection_mapping_vector, &normalized_input_direction, direction_dot_product + direction_dot_product);
        fast_vec_sub(&reflection_mapping_vector, &normalized_position_delta, &reflection_mapping_vector);
        fast_vec_normalize(&reflection_mapping_vector, &reflection_mapping_vector, 1.0f);
        reflection_mapping_vector.v.z = reflection_mapping_vector.v.z + 1.0f;
        sphere_map_denominator = fast_vec_length(&reflection_mapping_vector);
        sphere_map_denominator = sphere_map_denominator + sphere_map_denominator;
        out[i * 2] = (reflection_mapping_vector.v.x / sphere_map_denominator + 0.5f) * 4.0f;
        out[i * 2 + 1] = (reflection_mapping_vector.v.y / sphere_map_denominator + 0.5f) * 4.0f;
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F71D0), where it is exact; names translated to the US level program. */

extern char D_L17_001DE8A0[];
extern int get_effect_texture_alt(int) __asm__("FUN_001f44b8");
extern void font_queue_vu_state(void) __asm__("FUN_001f76a0");
extern void FUN_L00_001fde98(int, int, int, void *, int);
void compute_reflection_sphere_map_uvs(int n, V3 *b, V3 *a, int unused, float *out) __asm__("FUN_L17_002f54d8");
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

int D_L17_001623F0[2] = {26, 26};
extern float D_L17_001DE150[];
extern float D_L17_001DE288[];
float *D_L17_001623F8[2] = {D_L17_001DE150, D_L17_001DE288};
extern float D_L17_001DE3C0[];
extern float D_L17_001DE490[];
float *D_L17_00162400[2] = {D_L17_001DE3C0, D_L17_001DE490};
extern float D_L17_001DE560[];
extern float D_L17_001DE698[];
float *D_L17_00162408[2] = {D_L17_001DE560, D_L17_001DE698};
extern float D_L17_001DE7D0[];
extern float D_L17_001DE838[];
float *D_L17_00162410[2] = {D_L17_001DE7D0, D_L17_001DE838};
void draw_reflection_strips(void) __asm__("FUN_L17_002f5690");
void draw_reflection_strips(void) {
    int i;
    float *reflection_uv_coordinates;
    vu1_add_g_sregister(6, get_effect_texture_alt(0x30));
    vu1_add_g_sregister(0x42, 0x2000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    reflection_uv_coordinates = (float *)D_L17_001DE8A0;
    for (i = 0; i < 2; i++) {
        compute_reflection_sphere_map_uvs(D_L17_001623F0[i], ((int *)D_L17_00162408)[i],
                         ((int *)D_L17_001623F8)[i], ((int *)D_L17_00162400)[i], reflection_uv_coordinates);
        FUN_L00_001fde98(D_L17_001623F0[i], ((int *)D_L17_001623F8)[i],
                         ((int *)D_L17_00162410)[i], reflection_uv_coordinates, 1);
    }
}
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F74E8), where it is exact; names translated to the US level program. */

extern char D_L17_001DF6A0[];
extern float D_L17_001DED50[];
float *D_L17_00162440[2] = {D_L17_001DED50, 0};
extern float D_L17_001DF280[];
float *D_L17_00162450[2] = {D_L17_001DF280, 0};
extern float D_L17_001DF598[];
float *D_L17_00162458[2] = {D_L17_001DF598, 0};
void compute_reflection_sphere_map_uvs(int n, V3 *b, V3 *a, int unused, float *out) __asm__("FUN_L17_002f54d8");
void vu1_add_g_sregister(s32 a0, s64 a1) __asm__("FUN_00233980");

int D_L17_00162420[2] = {66, 0};
extern float D_L17_001DF070[];
float *D_L17_00162448[2] = {D_L17_001DF070, 0};
void draw_reflection_strip(void) __asm__("FUN_L17_002f59a8");
void draw_reflection_strip(void) {
    int i;
    float *sphere_map_uv_coordinates;
    vu1_add_g_sregister(6, get_effect_texture_alt(0x30));
    vu1_add_g_sregister(0x42, 0x2000000064L);
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x14, 0xFF9000000260L);
    font_queue_vu_state();
    sphere_map_uv_coordinates = (float *)D_L17_001DF6A0;
    for (i = 0; i < 1; i++) {
        compute_reflection_sphere_map_uvs(D_L17_00162420[i], (V3 *)((int *)D_L17_00162450)[i],
                         (V3 *)((int *)D_L17_00162440)[i], ((int *)D_L17_00162448)[i], sphere_map_uv_coordinates);
        FUN_L00_001fde98(D_L17_00162420[i], ((int *)D_L17_00162440)[i],
                         ((int *)D_L17_00162458)[i], sphere_map_uv_coordinates, 1);
    }
}
/* Computes a per-vertex 2D sphere-map texture coordinate from position and normal arrays. */
/* Ported from rac1-decomp (src/overlays/l17_fleet/vendor_002F1558.c: func_L17_002F7C78), where it is exact; names translated to the US level program. */

void compute_vertex_to_reference_sphere_map_uvs(int vertex_count, char *nrm, char *vertex_positions, int unused, float *out) __asm__("FUN_L17_002f6138");

void compute_vertex_to_reference_sphere_map_uvs(int vertex_count, char *nrm, char *vertex_positions, int unused, float *out) {
    float normalized_position_delta[4];
    float vertex_position[4];
    float reflection_mapping_vector[4];
    float normalized_normal[4];
    int i;
    for (i = 0; i < vertex_count; i++) {
        float *position_element = (float *)(i * 12 + (int)vertex_positions);
        float *normal_element;
        float sphere_map_denominator;
        vertex_position[3] = 1.0f;
        vertex_position[0] = position_element[0];
        vertex_position[1] = position_element[1];
        vertex_position[2] = position_element[2];
        fast_vec_sub(normalized_position_delta, D_L17_001676C0, vertex_position);
        fast_vec_normalize(normalized_position_delta, normalized_position_delta, 1.0f);
        normalized_normal[3] = 1.0f;
        normal_element = (float *)(i * 12 + (int)nrm);
        normalized_normal[0] = normal_element[0];
        normalized_normal[1] = normal_element[1];
        normalized_normal[2] = normal_element[2];
        fast_vec_normalize(normalized_normal, normalized_normal, 1.0f);
        fast_vec_scale(reflection_mapping_vector, normalized_normal, fast_vec_dot(normalized_normal, normalized_position_delta) * 2.0f);
        fast_vec_sub(reflection_mapping_vector, normalized_position_delta, reflection_mapping_vector);
        fast_vec_normalize(reflection_mapping_vector, reflection_mapping_vector, 1.0f);
        reflection_mapping_vector[2] += 1.0f;
        sphere_map_denominator = fast_vec_length(reflection_mapping_vector) * 2.0f;
        out[i * 2] = (reflection_mapping_vector[0] / sphere_map_denominator + 0.5f) * 4.0f;
        out[i * 2 + 1] = (reflection_mapping_vector[1] / sphere_map_denominator + 0.5f) * 4.0f;
    }
}
