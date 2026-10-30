/* Level overlay code: stubs become C as functions are matched. */
#include "types.h"
#include "rnc/gameplay/hero.h"
#include "asm.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"

typedef struct {
    char pad09[0x86];
    s16 id;
    char pad09b[0x18];
} Ent09;
extern Ent09 D_L00_00167150_u[] __asm__("D_L00_00167150");
/* Finds the 0xA0-byte entry in D_L00_00167150 whose short at 0x86 equals id; 0 when none. */
s32 find_camera_record_by_id(s32 id) __asm__("FUN_L00_001eb1f8");

s32 find_camera_record_by_id(s32 id) {
    s32 camera_record_address;
    s32 camera_table_end_address;
    s16 camera_record_id;
    camera_record_address = (s32)&D_L00_00167150_u[0];
    camera_table_end_address = camera_record_address + 0x1E00;
    for (;;) {
        camera_record_id = *(s16 *)(camera_record_address + 0x86);
        if (camera_record_id != id) {
            camera_record_address += 0xA0;
            if (camera_record_address < camera_table_end_address)
                continue;
            return 0;
        }
        return camera_record_address;
    }
}
float absolute_float(float) __asm__("FUN_001f99c0");
float rope_spring_turn(float *p, float a, float b, float c, float d, float lim) __asm__("FUN_L00_001eb328");
float rope_spring_turn(float *spring_state, float current_angle, float target_angle, float response_gain, float damping_factor, float spring_state_limit) {
    float angle_difference = fast_subtract_rotations(target_angle, current_angle);
    *spring_state = *spring_state + (response_gain * angle_difference - damping_factor * *spring_state);
    if (spring_state_limit != 0.0f) {
        if (*spring_state > spring_state_limit)
            *spring_state = spring_state_limit;
        else if (*spring_state < -spring_state_limit)
            *spring_state = -spring_state_limit;
    }
    if (*spring_state > absolute_float(angle_difference))
        *spring_state = absolute_float(angle_difference);
    else if (*spring_state < -absolute_float(angle_difference))
        *spring_state = -absolute_float(angle_difference);
    return fast_add_rotations(current_angle, *spring_state);
}
typedef struct {
    char pad[0x7D];
    u8 b;
    s16 h;
} T001eb448;
typedef struct {
    char pad[0x180];
    T001eb448 *p;
} S001eb448;
extern S001eb448 D_L00_00166C80_u __asm__("D_L00_00166C80");
/* Sets h and clears b in the block D_L00_00166C80 points to. */
void initialize_camera_record_handoff_defaults(void) __asm__("FUN_L00_001eb448");

void initialize_camera_record_handoff_defaults(void) {
    D_L00_00166C80_u.p->h = 1;
    D_L00_00166C80_u.p->b = 0;
}
typedef struct {
    s32 id;
    s32 pad[4];
} E5;
extern E5 l00_camera_id_lookup_table[] __asm__("D_L00_001EA880");
/* Index of id in the table that ends with -1 (the end index when it is missing). */
s32 find_camera_table_index_by_id(s32 id) __asm__("FUN_L00_001ed230");

s32 find_camera_table_index_by_id(s32 id) {
    s32 table_index = 0;
    while (l00_camera_id_lookup_table[table_index].id != -1 && l00_camera_id_lookup_table[table_index].id != id)
        table_index++;
    return table_index;
}

#include "qcopy.h"

/* resets the camera behind the hero */
/* Ported from rac1-decomp (src/overlays/shared/camera_001EB508.c: func_L00_001ED600), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x254];
    int p;
    char pad2[0x18];
    short s;
    char pad3[1];
    char c;
    char pad4[0x14];
    float f0;
    char pad5[8];
    float f1;
} S_u;

extern S_u D_L00_00166C80_c __asm__("D_L00_00166C80");
extern char *FUN_L00_001eb1f8_u(int) __asm__("FUN_L00_001eb1f8");
extern char active_camera_saved_state_storage[] __asm__("D_L00_00169110");
extern void update_camera_tracking_state(void) __asm__("FUN_001ed470");
void backup_current_cam(void) __asm__("FUN_001ebc90");
void camera_run_setup_to_new_cam(struct Moby *cam) __asm__("FUN_001ebec8");

void init_camera_system(void) __asm__("FUN_L00_001ed280");
void init_camera_system(void) {
    char *camera_system_state = (char *)&D_L00_00166C80_c;
    float *camera_vector_snapshot, *camera_record_vector_destination, *vector_copy_source, *vector_copy_destination;
    char *current_camera_record;
    if (*(char **)(camera_system_state + 0x180) != 0) {
        *(short *)(*(char **)(camera_system_state + 0x180) + 0x8E) = 0;
        *(*(char **)(camera_system_state + 0x180) + 0x7D) = 0;
        *(short *)(*(char **)(camera_system_state + 0x180) + 0x7E) = 0;
    }
    *(char **)(camera_system_state + 0x180) = FUN_L00_001eb1f8_u(0);
    *(short *)(*(char **)(camera_system_state + 0x180) + 0x8E) = 1;
    *(char **)(*(char **)(camera_system_state + 0x180) + 0x70) = active_camera_saved_state_storage;
    *(short *)(camera_system_state + 0x270) = 0;
    update_camera_tracking_state();
    camera_run_setup_to_new_cam(*(char **)(camera_system_state + 0x180));
    backup_current_cam();
    current_camera_record = *(char **)(camera_system_state + 0x180);
    camera_vector_snapshot = (float *)(camera_system_state + 0x140);
    qcopy(camera_vector_snapshot, current_camera_record + 0x30);
    camera_record_vector_destination = (float *)(current_camera_record + 0x64);
    camera_record_vector_destination[0] = camera_vector_snapshot[0];
    camera_record_vector_destination[1] = camera_vector_snapshot[1];
    camera_record_vector_destination[2] = camera_vector_snapshot[2];
    vector_copy_source = (float *)(D_0013E533 + 0xE9D);
    vector_copy_destination = (float *)(camera_system_state + 0x190);
    vector_copy_destination[0] = vector_copy_source[0];
    vector_copy_destination[1] = vector_copy_source[1];
    vector_copy_destination[2] = vector_copy_source[2];
}

#include "qcopy.h"

/* Exact match: needs -fno-force-mem (config/func_cflags.txt). */
/* Ported from rac1-decomp (src/overlays/shared/camera_001EB508.c: func_L00_001ED6D8), where it is exact; names translated to the US level program. */

typedef struct {
    char pad[0x254];
    int p;
    char pad2[0x18];
    short s;
    char pad3[1];
    char c;
    char pad4[0x14];
    float f0;
    char pad5[8];
    float f1;
} S;

typedef struct {
    char pad[0x74];
    int i74;
    float f78;
    char c7C, c7D;
    short s7E, s80, s82, s84, s86;
    char pad2;
    char c89;
    short s8A, s8C, s8E;
    char pad3[0x10];
} Elem;

extern Elem D_L00_00167150[];
extern S D_L00_00166C80;
extern int D_L00_0015EF4C;
extern int level_camera_present_flags[] __asm__("D_L00_00169890");
s32 find_camera_table_index_by_id(s32 id) __asm__("FUN_L00_001ed230");
void init_camera_system(void) __asm__("FUN_L00_001ed280");

void setup_level_cameras(void) __asm__("FUN_L00_001ed358");
extern f32 D_L00_0015EF40_gp __asm__("D_L00_0015EF40") __attribute__((sda));
void setup_level_cameras(void) {
    char *camera_system_state = (char *)&D_L00_00166C80;
    char *camera_system_state_alias;
    char *q;
    char *normalized_hero_vector;
    struct Hero *base;
    int i;
    int k;

    *(int *)(camera_system_state + 0x394) = 0;
    *(int *)(camera_system_state + 0x398) = 0;
    D_L00_0015EF40_gp = 0.75f;
    D_L00_0015EF4C = 0;
    for (k = 0; k < 48; k++) {
        D_L00_00167150[k].s8E = 0;
        level_camera_present_flags[k] = 0;
    }
    camera_system_state_alias = (char *)&D_L00_00166C80;
    base = &hero;
    q = camera_system_state_alias + 0x190;
    normalized_hero_vector = camera_system_state_alias + 0x1B0;
    *(int *)(camera_system_state_alias + 0x184) = 0;
    *(short *)(camera_system_state_alias + 0x270) = 0;
    fast_vec_normalize(normalized_hero_vector, (char *)(((int)base->moby) + 0xE0), 1.0f);
    qcopy(camera_system_state_alias + 0x1C0, normalized_hero_vector);
    qcopy(camera_system_state_alias + 0x1D0, normalized_hero_vector);
    clear_u64_value(camera_system_state_alias + 0x1E0);
    qcopy(camera_system_state_alias + 0x1F0, &base->motion.pos);
    {
        float hero_rotation_z = base->motion.rot.f[2];
        float hero_position_z = base->motion.pos.f[2];
        *(int *)(q + 0xC4) = 0;
        *(int *)(q + 0xCC) = 0;
        *(int *)(q + 0xC0) = 0;
        *(int *)(q + 0xC8) = 0;
        *(int *)(q + 0xD0) = 0;
        *(float *)(q + 0xBC) = hero_rotation_z;
        *(float *)(q + 0xB8) = hero_rotation_z;
        *(float *)(q + 0xB4) = hero_rotation_z;
        *(float *)(q + 0xB0) = hero_rotation_z;
        *(int *)(q + 0xD4) = 0;
        *(int *)(q + 0xDC) = 0;
        *(float *)(q + 0xAC) = hero_rotation_z;
        *(float *)(q + 0xD8) = hero_position_z;
    }
    i = 0;
    if (D_L00_0015EF54 > 0) {
        do {
            char *level_camera_entry;
            char *level_camera_data;
            Elem *el;
            int camera_id;
            level_camera_entry = (char *)(i * 32);
            level_camera_entry = D_L00_0015EF50 + (int)level_camera_entry;
            level_camera_data = *(char **)(level_camera_entry + 0x1C);
            el = &D_L00_00167150[i];
            el->s84 = i;
            *(Elem **)(level_camera_data + 4) = el;
            el->s8E = 0;
            el->c89 = 0;
            el->s8A = 0;
            el->s82 = -1;
            el->s80 = -1;
            el->c7D = 0;
            el->s7E = 0;
            el->f78 = -1.0f;
            el->c7C = level_camera_data[0x1C];
            el->i74 = (unsigned char)level_camera_data[0x1F];
            camera_id = *(unsigned short *)(level_camera_entry + 0xC);
            level_camera_present_flags[i] = 1;
            el->s86 = camera_id;
            el->s8C = find_camera_table_index_by_id((short)camera_id);
            i++;
        } while (i < D_L00_0015EF54);
    }
    init_camera_system();
    *(short *)D_L00_00166EF0 = 0;
    {
        int *q = (int *)(D_L00_00166EF0 + 0x110);
        q[0] = 0;
        q[3] = 0;
        q[1] = 0;
        q[4] = 0;
    }
}

/* Ported from rac1-decomp (src/overlays/shared/camera_001EB508.c: func_L00_001ED900), where it is exact; names translated to the US level program. */

void compute_position_from_spherical_offset(float *output_position, float *angles_and_distance, void *origin_position) __asm__("FUN_L00_001ed580");

void compute_position_from_spherical_offset(float *output_position, float *angles_and_distance, void *origin_position) {
    output_position[0] = angles_and_distance[2] * fast_cos(angles_and_distance[1]) * fast_cos(angles_and_distance[0]);
    output_position[1] = angles_and_distance[2] * fast_cos(angles_and_distance[1]) * fast_sin(angles_and_distance[0]);
    output_position[2] = angles_and_distance[2] * fast_sin(angles_and_distance[1]);
    *(int *)(output_position + 3) = 0;
    fast_vec_add(output_position, output_position, origin_position);
}

/* Ported from rac1-decomp (src/overlays/shared/camera_001EB508.c: func_L00_001ED9B0), where it is exact; names translated to the US level program. */

typedef float W[4] __attribute__((aligned(16)));

extern void subtract_vector_xyz(float *) __asm__("FUN_001f9a28");

/* Fills a from a subtract_vector_xyz result: two fast_arc_tan angles and its length. */
int compute_spherical_offset_from_positions(float *angles_and_distance) __asm__("FUN_L00_001ed630");

int compute_spherical_offset_from_positions(float *angles_and_distance) {
    W difference_vector;
    float xy_angle;
    subtract_vector_xyz(difference_vector);
    angles_and_distance[1] = fast_arc_tan(vector_length_xy(difference_vector), difference_vector[2]);
    xy_angle = fast_arc_tan(difference_vector[0], difference_vector[1]);
    fast_subtract_rotations(xy_angle, angles_and_distance[0]);
    angles_and_distance[0] = xy_angle;
    angles_and_distance[2] = fast_vec_length(difference_vector);
    return 0;
}
#include "qcopy.h"
#include "rnc/overlay/quad.h"
typedef struct {
    char pad[0x140];
    OvlQuad q;
    char pad2[0x30];
    char *o;
} C_1ed6a8;
extern C_1ed6a8 D_L00_00166C80_lui __asm__("D_L00_00166C80") NOT_SDA;
extern OvlQuad D_L00_00173E70_lui __asm__("D_L00_00173E70") NOT_SDA;
extern OvlQuad g_debug_camera_lui __asm__("D_L00_00166DC0") NOT_SDA;
extern int g_debug_menu_lui[] __asm__("D_L00_0016C058") NOT_SDA;
extern float f9af0_1ed6a8(void *) __asm__("FUN_001f9af0");
extern void divide_vector_xyz(void *, void *, float) __asm__("FUN_L00_001ff378");
extern int coll_sphere(void *, int, int, float) __asm__("FUN_L00_001f0d60");

extern int D_L00_0016C06C NOT_SDA;
extern int D_L00_0015EF5C_gp __asm__("D_L00_0015EF5C") __attribute__((sda));
void resolve_camera_movement_collisions(OvlQuad *camera_position, float collision_radius) __asm__("FUN_L00_001ed6a8");

void resolve_camera_movement_collisions(OvlQuad *camera_position, float collision_radius) {
    OvlQuad step_delta[1];
    float camera_movement_origin_snapshot[4] __attribute__((aligned(16)));
    char *o = D_L00_00166C80_lui.o;
    float *camera_movement_origin = (float *)(o + 0x64);
    int step_count, i, j;

    camera_movement_origin_snapshot[0] = camera_movement_origin[0];
    camera_movement_origin_snapshot[1] = camera_movement_origin[1];
    camera_movement_origin_snapshot[2] = camera_movement_origin[2];
    ((int *)camera_movement_origin_snapshot)[3] = 0;
    if (*(short *)(o + 0x8A) != 0) {
        if (D_L00_0016C06C == 0) {
            qcopy(&D_L00_00166C80_lui.q, camera_position);
        }
        *(short *)(o + 0x8A) = 0;
        return;
    }
    fast_vec_sub(step_delta, camera_position, camera_movement_origin_snapshot);
    step_count = truncate_float_to_s32(f9af0_1ed6a8(step_delta) / (collision_radius * 0.9f)) + 1;
    divide_vector_xyz(step_delta, step_delta, convert_integer_to_float(step_count));
    qcopy(camera_position, camera_movement_origin_snapshot);
    for (i = 0; i < step_count; i++) {
        fast_vec_add(camera_position, camera_position, step_delta);
        for (j = 0; j < 6 && coll_sphere(camera_position, D_L00_0015EF5C_gp, ((int)hero.moby), collision_radius); j++) {
            qcopy(camera_position, &D_L00_00173E70_lui);
            {
                char *current_camera_record = D_L00_00166C80_lui.o;
                if (*(unsigned char *)(current_camera_record + 0x89) == 0) {
                    *(unsigned char *)(current_camera_record + 0x89) = 1;
                }
            }
        }
    }
    if (g_debug_menu_lui[5] == 0) {
        qcopy(&g_debug_camera_lui, camera_position);
    }
}

Elem D_L01_001675D0 = {0};
int D_L01_0015EF4C = 0;
int D_L01_0016C4EC NOT_SDA = 0;
Elem D_L02_00167750 = {0};
int D_L02_0016C66C NOT_SDA = 0;
Elem D_L03_00167250 = {0};
int D_L03_0015EF4C = 0;
int D_L03_0016C16C NOT_SDA = 0;
Elem D_L04_001672D0 = {0};
int D_L04_0015EF4C = 0;
int D_L04_0016C1EC NOT_SDA = 0;
Elem D_L05_00167550 = {0};
int D_L05_0015EF4C = 0;
int D_L05_0016C46C NOT_SDA = 0;
int D_L06_0015EF4C = 0;
int D_L06_0016C7EC NOT_SDA = 0;
Elem D_L07_001671D0 = {0};
int D_L07_0015EF4C = 0;
int D_L07_0016C0EC NOT_SDA = 0;
Elem D_L08_00167950 = {0};
int D_L08_0015EF4C = 0;
int D_L08_0016C86C NOT_SDA = 0;
Elem D_L09_001672D0 = {0};
int D_L09_0015EF4C = 0;
int D_L09_0016C1EC NOT_SDA = 0;
Elem D_L10_001675D0 = {0};
int D_L10_0015EF4C = 0;
int D_L10_0016C4EC NOT_SDA = 0;
Elem D_L11_00167B50 = {0};
int D_L11_0015EF4C = 0;
int D_L11_0016CA6C NOT_SDA = 0;
Elem D_L12_001675D0 = {0};
int D_L12_0015EF4C = 0;
int D_L12_0016C4EC NOT_SDA = 0;
Elem D_L13_00167450 = {0};
int D_L13_0015EF4C = 0;
int D_L13_0016C36C NOT_SDA = 0;
Elem D_L14_00167850 = {0};
int D_L14_0015EF4C = 0;
int D_L14_0016C76C NOT_SDA = 0;
Elem D_L15_00167750 = {0};
int D_L15_0015EF4C = 0;
int D_L15_0016C66C NOT_SDA = 0;
Elem D_L16_00167550 = {0};
int D_L16_0015EF4C = 0;
int D_L16_0016C46C NOT_SDA = 0;
Elem D_L17_00167A50 = {0};
int D_L17_0015EF4C = 0;
int D_L17_0016C96C NOT_SDA = 0;
int D_L18_0015EF4C = 0;
int D_L18_0016CA6C NOT_SDA = 0;
