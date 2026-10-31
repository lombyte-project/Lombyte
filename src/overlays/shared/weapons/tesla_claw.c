/* Shared moby class tesla_claw: the lightning gun held by the hero, and the beam and arcs it draws. */

#include "types.h"
#include "rnc/overlay/point_light.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/data_labels.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/audio/voice_alloc.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/input/pad_state.h"
#include "rnc/mobys/get_moby_target_info.h"
#include "rnc/mobys/moby.h"
#include "qcopy.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/game/item_state.h"
#include "rnc/overlay/shared/weapons/tesla_claw_globals.h"
#include "qzero.h"
#include "rnc/overlay/beam_vector.h"

typedef struct {
    u8  p0[0x2084];
    s32 i2084;
} G_B4F40;

typedef unsigned int Q_2B9AD8 __attribute__((mode(TI)));

typedef union {
    struct {
        float x, y, z, w;
    } f;
    Q_2B9AD8 q;
} V_2B9AD8 __attribute__((aligned(16)));

typedef struct Data {
    V_2B9AD8     pos;
    V_2B9AD8     tip;
    struct Moby *target;
    float        reach;
    short        hold;
    short        hold2;
    int          timer;
    int          light;
    int          glow;
    struct Moby *target2;
    short        tick;
    short        f3E;
    short        lock;
    short        lock2;
    short        f44;
    short        f46;
    int          sound;
} Data;

typedef struct Stats {
    char           pad0[0x98];
    unsigned short uses;
    unsigned short longest;
    int            levels;
} Stats;

typedef struct Stats2 {
    char           pad0[0xD0];
    unsigned short uses;
    unsigned short longest;
    int            levels;
} Stats2;

typedef struct Aim {
    char  pad0[4];
    short kind;
} Aim;

extern int  get_ammo(int) __asm__("FUN_L00_00233e98");
extern void moby_point_position(void *, int, void *) __asm__("FUN_L00_0024f7c8");
extern void start_equipped_item_animation(void) __asm__("FUN_L00_0020e4c8");
extern void release_point_light_slot(int) __asm__("FUN_L00_0023e838");
void        cancel_hero_item_animation(void) __asm__("FUN_L00_0020e698");
extern void add_draw_callback(void (*)(void), void *) __asm__("FUN_001f4600");
void        help_request(int, int) __asm__("FUN_L00_00203908");
extern void draw_beam_and_arc_effects(char *) __asm__("FUN_L00_002ba970");
extern int  FUN_L00_00233db8_2b2100a(int, int) __asm__("FUN_L00_00233db8");
extern int  FUN_L00_0023e5e0_2b2100(void *, float, float, int) __asm__("FUN_L00_0023e5e0");
extern void FUN_L00_002b9310_2b2100(void *) __asm__("FUN_L00_002b9310");
extern void FUN_L00_002b94d0_2b2100(void *) __asm__("FUN_L00_002b94d0");
extern int  FUN_L00_0028d8c0_2b2100(void *, int) __asm__("FUN_L00_0028d8c0");

extern int            D_0015EEA4;
extern s32            D_0015EEA4;
extern unsigned char  D_0013E533_2b2100[] __asm__("D_0013E533");
extern unsigned short D_00141D70_2b2100[] __asm__("D_00141D70");
extern unsigned short D_00141918_2b2100[] __asm__("D_00141918");
extern int            tesla_beam_restart_pending __asm__("D_L00_00161590");
extern float          D_L00_001615AC_gp __asm__("D_L00_001615AC") __attribute__((sda));
extern float          D_L00_001615B4_gp __asm__("D_L00_001615B4") __attribute__((sda));
extern float          D_L00_001615C8_gp __asm__("D_L00_001615C8") __attribute__((sda));
int                   tesla_beam_restart_pending_gp __asm__("D_L00_00161590") = 1;
extern int            D_L00_0016167C __attribute__((sda));
int                   tesla_beam_initial_frame_count_gp __asm__("D_L00_00161624") = 90;
extern float          D_L00_00161594 __attribute__((sda));
extern short          D_L00_00161658[] MACRO_ADDR;
extern short          D_L00_00161668[] MACRO_ADDR;
extern short          D_L00_0016166A;
extern short          D_L00_0016166C;
extern short          D_L00_0016166E;
extern short          D_L00_00161670[] MACRO_ADDR;
extern short          D_L00_00161672;
extern short          D_L00_00161674;
extern short          D_L00_00161676;
extern V2b9310        l00_tesla_beam_previous_point_window[] __asm__("D_L00_001DB5B0");
extern V2b9310        D_L00_001DB5C0[];

int     D_L01_001615C0 = 1;
float   D_L01_001615DC = 0;
float   D_L01_001615E4 = 0;
float   D_L01_001615F8 = 0;
int     D_L01_001616AC = 0;
int     D_L01_00161654 = 0x5a;
float   D_L01_001615C4 = 15.0f;
short   D_L01_00161688 = 0;
short   D_L01_00161698 = 0;
short   D_L01_0016169A = 0;
short   D_L01_0016169C = 0;
short   D_L01_0016169E = 0;
short   D_L01_001616A0 = 0;
short   D_L01_001616A2 = 0;
short   D_L01_001616A4 = 0;
short   D_L01_001616A6 = 0;
V2b9310 D_L01_001DBA40 = {0};

int     D_L02_00161560 = 1;
float   D_L02_0016157C = 0;
float   D_L02_00161584 = 0;
float   D_L02_00161598 = 0;
int     D_L02_0016164C = 0;
int     D_L02_001615F4 = 0x5a;
float   D_L02_00161564 = 15.0f;
short   D_L02_00161628 = 0;
short   D_L02_00161638 = 0;
short   D_L02_0016163A = 0;
short   D_L02_0016163C = 0;
short   D_L02_0016163E = 0;
short   D_L02_00161640 = 0;
short   D_L02_00161642 = 0;
short   D_L02_00161644 = 0;
short   D_L02_00161646 = 0;
V2b9310 D_L02_001D0040 = {0};

int     D_L03_00161570 = 1;
float   D_L03_0016158C = 0;
float   D_L03_00161594 = 0;
float   D_L03_001615A8 = 0;
int     D_L03_0016165C = 0;
int     D_L03_00161604 = 0x5a;
float   D_L03_00161574 = 15.0f;
short   D_L03_00161638 = 0;
short   D_L03_00161648 = 0;
short   D_L03_0016164A = 0;
short   D_L03_0016164C = 0;
short   D_L03_0016164E = 0;
short   D_L03_00161650 = 0;
short   D_L03_00161652 = 0;
short   D_L03_00161654 = 0;
short   D_L03_00161656 = 0;
V2b9310 D_L03_001CFB40 = {0};

int     D_L04_00161560 = 1;
float   D_L04_0016157C = 0;
float   D_L04_00161584 = 0;
float   D_L04_00161598 = 0;
int     D_L04_0016164C = 0;
int     D_L04_001615F4 = 0x5a;
float   D_L04_00161564 = 15.0f;
short   D_L04_00161628 = 0;
short   D_L04_00161638 = 0;
short   D_L04_0016163A = 0;
short   D_L04_0016163C = 0;
short   D_L04_0016163E = 0;
short   D_L04_00161640 = 0;
short   D_L04_00161642 = 0;
short   D_L04_00161644 = 0;
short   D_L04_00161646 = 0;
V2b9310 D_L04_001CFBC0 = {0};

int     D_L05_001615B0 = 1;
float   D_L05_001615CC = 0;
float   D_L05_001615D4 = 0;
float   D_L05_001615E8 = 0;
int     D_L05_0016169C = 0;
int     D_L05_00161644 = 0x5a;
float   D_L05_001615B4 = 15.0f;
short   D_L05_00161678 = 0;
short   D_L05_00161688 = 0;
short   D_L05_0016168A = 0;
short   D_L05_0016168C = 0;
short   D_L05_0016168E = 0;
short   D_L05_00161690 = 0;
short   D_L05_00161692 = 0;
short   D_L05_00161694 = 0;
short   D_L05_00161696 = 0;
V2b9310 D_L05_001CFEE0 = {0};

int     D_L06_00161590 = 1;
float   D_L06_001615AC = 0;
float   D_L06_001615B4 = 0;
float   D_L06_001615C8 = 0;
int     D_L06_0016167C = 0;
int     D_L06_00161624 = 0x5a;
float   D_L06_00161594 = 15.0f;
short   D_L06_00161658 = 0;
short   D_L06_00161668 = 0;
short   D_L06_0016166A = 0;
short   D_L06_0016166C = 0;
short   D_L06_0016166E = 0;
short   D_L06_00161670 = 0;
short   D_L06_00161672 = 0;
short   D_L06_00161674 = 0;
short   D_L06_00161676 = 0;
V2b9310 D_L06_001D01C0 = {0};

int     D_L07_00161560 = 1;
float   D_L07_0016157C = 0;
float   D_L07_00161584 = 0;
float   D_L07_00161598 = 0;
int     D_L07_0016164C = 0;
int     D_L07_001615F4 = 0x5a;
float   D_L07_00161564 = 15.0f;
short   D_L07_00161628 = 0;
short   D_L07_00161638 = 0;
short   D_L07_0016163A = 0;
short   D_L07_0016163C = 0;
short   D_L07_0016163E = 0;
short   D_L07_00161640 = 0;
short   D_L07_00161642 = 0;
short   D_L07_00161644 = 0;
short   D_L07_00161646 = 0;
V2b9310 D_L07_001CFAC0 = {0};

int     D_L08_001615B0 = 1;
float   D_L08_001615CC = 0;
float   D_L08_001615D4 = 0;
float   D_L08_001615E8 = 0;
int     D_L08_0016169C = 0;
int     D_L08_00161644 = 0x5a;
float   D_L08_001615B4 = 15.0f;
short   D_L08_00161678 = 0;
short   D_L08_00161688 = 0;
short   D_L08_0016168A = 0;
short   D_L08_0016168C = 0;
short   D_L08_0016168E = 0;
short   D_L08_00161690 = 0;
short   D_L08_00161692 = 0;
short   D_L08_00161694 = 0;
short   D_L08_00161696 = 0;
V2b9310 D_L08_001D0240 = {0};

int     D_L09_00161620 = 1;
float   D_L09_0016163C = 0;
float   D_L09_00161644 = 0;
float   D_L09_00161658 = 0;
int     D_L09_0016170C = 0;
int     D_L09_001616B4 = 0x5a;
float   D_L09_00161624 = 15.0f;
short   D_L09_001616E8 = 0;
short   D_L09_001616F8 = 0;
short   D_L09_001616FA = 0;
short   D_L09_001616FC = 0;
short   D_L09_001616FE = 0;
short   D_L09_00161700 = 0;
short   D_L09_00161702 = 0;
short   D_L09_00161704 = 0;
short   D_L09_00161706 = 0;
V2b9310 D_L09_001CFBC0 = {0};

int     D_L10_00161570 = 1;
float   D_L10_0016158C = 0;
float   D_L10_00161594 = 0;
float   D_L10_001615A8 = 0;
int     D_L10_0016165C = 0;
int     D_L10_00161604 = 0x5a;
float   D_L10_00161574 = 15.0f;
short   D_L10_00161638 = 0;
short   D_L10_00161648 = 0;
short   D_L10_0016164A = 0;
short   D_L10_0016164C = 0;
short   D_L10_0016164E = 0;
short   D_L10_00161650 = 0;
short   D_L10_00161652 = 0;
short   D_L10_00161654 = 0;
short   D_L10_00161656 = 0;
V2b9310 D_L10_001CFEC0 = {0};

int     D_L11_001615F0 = 1;
float   D_L11_0016160C = 0;
float   D_L11_00161614 = 0;
float   D_L11_00161628 = 0;
int     D_L11_001616DC = 0;
int     D_L11_00161684 = 0x5a;
float   D_L11_001615F4 = 15.0f;
short   D_L11_001616B8 = 0;
short   D_L11_001616C8 = 0;
short   D_L11_001616CA = 0;
short   D_L11_001616CC = 0;
short   D_L11_001616CE = 0;
short   D_L11_001616D0 = 0;
short   D_L11_001616D2 = 0;
short   D_L11_001616D4 = 0;
short   D_L11_001616D6 = 0;
V2b9310 D_L11_001D0440 = {0};

int     D_L12_00161570 = 1;
float   D_L12_0016158C = 0;
float   D_L12_00161594 = 0;
float   D_L12_001615A8 = 0;
int     D_L12_0016165C = 0;
int     D_L12_00161604 = 0x5a;
float   D_L12_00161574 = 15.0f;
short   D_L12_00161638 = 0;
short   D_L12_00161648 = 0;
short   D_L12_0016164A = 0;
short   D_L12_0016164C = 0;
short   D_L12_0016164E = 0;
short   D_L12_00161650 = 0;
short   D_L12_00161652 = 0;
short   D_L12_00161654 = 0;
short   D_L12_00161656 = 0;
V2b9310 D_L12_001E2AE0 = {0};

int     D_L13_00161640 = 1;
float   D_L13_0016165C = 0;
float   D_L13_00161664 = 0;
float   D_L13_00161678 = 0;
int     D_L13_0016172C = 0;
int     D_L13_001616D4 = 0x5a;
float   D_L13_00161644 = 15.0f;
short   D_L13_00161708 = 0;
short   D_L13_00161718 = 0;
short   D_L13_0016171A = 0;
short   D_L13_0016171C = 0;
short   D_L13_0016171E = 0;
short   D_L13_00161720 = 0;
short   D_L13_00161722 = 0;
short   D_L13_00161724 = 0;
short   D_L13_00161726 = 0;
V2b9310 D_L13_001D0350 = {0};

int     D_L14_00161780 = 1;
float   D_L14_0016179C = 0;
float   D_L14_001617A4 = 0;
float   D_L14_001617B8 = 0;
int     D_L14_0016186C = 0;
int     D_L14_00161814 = 0x5a;
float   D_L14_00161784 = 15.0f;
short   D_L14_00161848 = 0;
short   D_L14_00161858 = 0;
short   D_L14_0016185A = 0;
short   D_L14_0016185C = 0;
short   D_L14_0016185E = 0;
short   D_L14_00161860 = 0;
short   D_L14_00161862 = 0;
short   D_L14_00161864 = 0;
short   D_L14_00161866 = 0;
V2b9310 D_L14_001DCA00 = {0};

int     D_L15_00161720 = 1;
float   D_L15_0016173C = 0;
float   D_L15_00161744 = 0;
float   D_L15_00161758 = 0;
int     D_L15_0016180C = 0;
int     D_L15_001617B4 = 0x5a;
float   D_L15_00161724 = 15.0f;
short   D_L15_001617E8 = 0;
short   D_L15_001617F8 = 0;
short   D_L15_001617FA = 0;
short   D_L15_001617FC = 0;
short   D_L15_001617FE = 0;
short   D_L15_00161800 = 0;
short   D_L15_00161802 = 0;
short   D_L15_00161804 = 0;
short   D_L15_00161806 = 0;
V2b9310 D_L15_001D04C0 = {0};

int     D_L16_00161620 = 1;
float   D_L16_0016163C = 0;
float   D_L16_00161644 = 0;
float   D_L16_00161658 = 0;
int     D_L16_0016170C = 0;
int     D_L16_001616B4 = 0x5a;
float   D_L16_00161624 = 15.0f;
short   D_L16_001616E8 = 0;
short   D_L16_001616F8 = 0;
short   D_L16_001616FA = 0;
short   D_L16_001616FC = 0;
short   D_L16_001616FE = 0;
short   D_L16_00161700 = 0;
short   D_L16_00161702 = 0;
short   D_L16_00161704 = 0;
short   D_L16_00161706 = 0;
V2b9310 D_L16_001CFEC0 = {0};

int     D_L17_00161670 = 1;
float   D_L17_0016168C = 0;
float   D_L17_00161694 = 0;
float   D_L17_001616A8 = 0;
int     D_L17_0016175C = 0;
int     D_L17_00161704 = 0x5a;
float   D_L17_00161674 = 15.0f;
short   D_L17_00161738 = 0;
short   D_L17_00161748 = 0;
short   D_L17_0016174A = 0;
short   D_L17_0016174C = 0;
short   D_L17_0016174E = 0;
short   D_L17_00161750 = 0;
short   D_L17_00161752 = 0;
short   D_L17_00161754 = 0;
short   D_L17_00161756 = 0;
V2b9310 D_L17_001D0740 = {0};

int     D_L18_00161600 = 1;
float   D_L18_0016161C = 0;
float   D_L18_00161624 = 0;
float   D_L18_00161638 = 0;
int     D_L18_001616EC = 0;
int     D_L18_00161694 = 0x5a;
float   D_L18_00161604 = 15.0f;
short   D_L18_001616C8 = 0;
short   D_L18_001616D8 = 0;
short   D_L18_001616DA = 0;
short   D_L18_001616DC = 0;
short   D_L18_001616DE = 0;
short   D_L18_001616E0 = 0;
short   D_L18_001616E2 = 0;
short   D_L18_001616E4 = 0;
short   D_L18_001616E6 = 0;
V2b9310 D_L18_001D0440 = {0};

int  D_L01_00161628 = 0x7f2020;
Vec4 D_L01_001DBE90[4][5] = {0};

int  D_L02_001615C8 = 0x7f2020;
Vec4 D_L02_001D0490[4][5] = {0};

int  D_L03_001615D8 = 0x7f2020;
Vec4 D_L03_001CFF90[4][5] = {0};

int  D_L04_001615C8 = 0x7f2020;
Vec4 D_L04_001D0010[4][5] = {0};

int  D_L05_00161618 = 0x7f2020;
Vec4 D_L05_001D0330[4][5] = {0};

int  D_L06_001615F8 = 0x7f2020;
Vec4 D_L06_001D0610[4][5] = {0};

int  D_L07_001615C8 = 0x7f2020;
Vec4 D_L07_001CFF10[4][5] = {0};

int  D_L08_00161618 = 0x7f2020;
Vec4 D_L08_001D0690[4][5] = {0};

int  D_L09_00161688 = 0x7f2020;
Vec4 D_L09_001D0010[4][5] = {0};

int  D_L10_001615D8 = 0x7f2020;
Vec4 D_L10_001D0310[4][5] = {0};

int  D_L11_00161658 = 0x7f2020;
Vec4 D_L11_001D0890[4][5] = {0};

int  D_L12_001615D8 = 0x7f2020;
Vec4 D_L12_001E2F30[4][5] = {0};

int  D_L13_001616A8 = 0x7f2020;
Vec4 D_L13_001D07A0[4][5] = {0};

int  D_L14_001617E8 = 0x7f2020;
Vec4 D_L14_001DCE50[4][5] = {0};

Vec4 D_L15_001D0910[4][5] = {0};

int  D_L16_00161688 = 0x7f2020;
Vec4 D_L16_001D0310[4][5] = {0};

Vec4 D_L17_001D0B90[4][5] = {0};

Vec4 D_L18_001D0890[4][5] = {0};

Vec4 D_L00_001DBA10[4][5] = {0};

/* Tesla claw (class 177) update: tracks up to two targets while fire is held, extends the claw tip, runs its
   firing states and statistics, and moves its light. */
/* Ported from rac1-decomp (src/overlays/shared/vendor_002B33E8.c: func_L00_002B9AD8), where it is exact; names translated to the US level program. */
void tesla_claw_update(struct Moby *m) __asm__("FUN_L00_002b87e0");

/* Moby class 177 */
void tesla_claw_update(struct Moby *tesla_claw_moby) {
    enum {
        TESLA_CLAW_STATE_INITIALIZE = 0,
        TESLA_CLAW_STATE_WAIT_FOR_FIRE = 1,
        TESLA_CLAW_STATE_FIRING = 3,
        TESLA_CLAW_STATE_FINISHED = 4
    };
    V_2B9AD8     transformed_offset;
    Data        *tesla_claw_vars = (Data *)tesla_claw_moby->pvars;
    int          active;
    int          done = hero.items[0].state == 3;
    int          ready;
    struct Hero *player_hero;

    if ((controller_state.pressed & hero.items[0].button_mask) && !hero.item_use_locked) {
        tesla_claw_vars->reach = 0.8f;
    }
    D_L00_0017AEBC = 0.9f;
    active = 0;
    ready = get_ammo(-1);
    if (D_00141D70_2b2100[0] == 0 && D_00141918_2b2100[0] == 0 && hero.wrench_combo_step >= 3) {
        if ((unsigned int)hero.state.control_mode < 2 || hero.state.control_mode == 9)
            help_request(0x4E30, 0x81);
    }
    if ((controller_state.held & hero.items[0].button_mask) && tesla_claw_moby->state != 4 &&
        tesla_claw_moby->state != 0 && !hero.item_use_locked && hero.state.current != 0x72) {
        if (tesla_claw_vars->timer == 0) {
            if (ready > 0) {
                if (fast_dec_timer_s16(&tesla_claw_vars->tick)) {
                    fast_dec_timer_s16(&tesla_claw_vars->f44);
                    fast_dec_timer_s16(&tesla_claw_vars->f46);
                    FUN_L00_00233db8_2b2100a(-1, 1);
                    tesla_claw_vars->tick = scale_game_frames(10);
                }
                if (tesla_claw_vars->light == -1) {
                    tesla_claw_vars->light =
                        FUN_L00_0023e5e0_2b2100(&tesla_claw_moby->pos, 7.5f, 0.0f, 0);
                } else if (tesla_claw_vars->glow < scale_game_frames(10)) {
                    tesla_claw_vars->glow++;
                }
                active = 1;
                if (tesla_claw_vars->target) {
                    if (2.0943f < fast_difference_between_rotations(
                                      hero.motion.rot.f[2],
                                      fast_arc_tan(
                                          tesla_claw_vars->target->pos.x - hero.motion.pos.f[0],
                                          tesla_claw_vars->target->pos.y - hero.motion.pos.f[1])) ||
                        !tesla_claw_vars->target->visible ||
                        20.0f < fast_vec_dist(&tesla_claw_vars->target->pos, &hero.motion.pos)) {
                        tesla_claw_vars->target = 0;
                        tesla_claw_vars->lock = 0;
                    }
                }
                if (tesla_claw_vars->target2) {
                    if (2.0943f <
                            fast_difference_between_rotations(
                                hero.motion.rot.f[2],
                                fast_arc_tan(tesla_claw_vars->target2->pos.x - hero.motion.pos.f[0],
                                             tesla_claw_vars->target2->pos.y -
                                                 hero.motion.pos.f[1])) ||
                        !tesla_claw_vars->target2->visible ||
                        20.0f < fast_vec_dist(&tesla_claw_vars->target2->pos, &hero.motion.pos)) {
                        tesla_claw_vars->target2 = 0;
                        tesla_claw_vars->lock2 = 0;
                    }
                }
                qcopy(&tesla_claw_vars->pos, &tesla_claw_moby->pos);
                tesla_claw_vars->pos.f.z += 0.7f;
                qcopy(&tesla_claw_vars->tip, &tesla_claw_vars->pos);
                if (tesla_claw_vars->target && tesla_claw_vars->lock) {
                    int   lim = 50;
                    Aim  *a;
                    short n;
                    tesla_claw_vars->hold++;
                    a = get_moby_target_info(tesla_claw_vars->target);
                    if (a && a->kind == 1)
                        lim = 5;
                    n = tesla_claw_vars->hold;
                    if (!(n < scale_game_frames(lim))) {
                        tesla_claw_vars->target = 0;
                        tesla_claw_vars->lock = 0;
                    }
                } else {
                    tesla_claw_vars->hold = 0;
                }
                if (tesla_claw_vars->target2 && tesla_claw_vars->lock2) {
                    int   lim = 50;
                    Aim  *a;
                    short n;
                    tesla_claw_vars->hold2++;
                    a = get_moby_target_info(tesla_claw_vars->target2);
                    if (a && a->kind == 1)
                        lim = 5;
                    n = tesla_claw_vars->hold2;
                    if (!(n < scale_game_frames(lim))) {
                        tesla_claw_vars->target2 = 0;
                        tesla_claw_vars->lock2 = 0;
                    }
                } else {
                    tesla_claw_vars->hold2 = 0;
                }
                tesla_claw_vars->reach += (convert_integer_to_float(D_0013E533_2b2100[0]) * 6.0f +
                                           12.0f - tesla_claw_vars->reach) *
                                          (frame_scale * 0.07f);
                if (hero.state.current == 1) {
                    transformed_offset.f.x = 1.2f;
                    transformed_offset.f.y = 0.0f;
                    transformed_offset.f.z = 0.0f;
                    transform_vector_by_basis(
                        &transformed_offset, &transformed_offset, D_L00_00166FD0);
                    tesla_claw_vars->pos.f.z = transformed_offset.f.z + tesla_claw_moby->pos.z;
                    tesla_claw_vars->pos.f.x = transformed_offset.f.x + hero.motion.pos.f[0];
                    tesla_claw_vars->pos.f.y = transformed_offset.f.y + hero.motion.pos.f[1];
                    transformed_offset.f.x = tesla_claw_vars->reach;
                    transformed_offset.f.y = 0.0f;
                    transformed_offset.f.z = 0.0f;
                    transform_vector_by_basis(
                        &transformed_offset, &transformed_offset, D_L00_00166FD0);
                    fast_vec_add(&tesla_claw_vars->tip, &tesla_claw_vars->pos, &transformed_offset);
                } else if (hero.unk22C8 == 1) {
                    moby_point_position(tesla_claw_moby, 0, &tesla_claw_vars->pos);
                    transformed_offset.f.x = 0.0f;
                    transformed_offset.f.y = -tesla_claw_vars->reach;
                    transformed_offset.f.z = 0.0f;
                    transform_vector_by_basis(
                        &transformed_offset, &transformed_offset, &tesla_claw_moby->rot_row0);
                    fast_vec_add(&tesla_claw_vars->tip, &tesla_claw_vars->pos, &transformed_offset);
                } else {
                    moby_point_position(tesla_claw_moby, 0, &tesla_claw_vars->pos);
                    fast_vec_normalize(
                        &tesla_claw_vars->tip, (char *)&hero + 0x640, tesla_claw_vars->reach);
                    fast_vec_add(
                        &tesla_claw_vars->tip, &tesla_claw_vars->pos, &tesla_claw_vars->tip);
                }
                FUN_L00_002b9310_2b2100(tesla_claw_moby);
                FUN_L00_002b94d0_2b2100(tesla_claw_moby);
                add_draw_callback(draw_beam_and_arc_effects, tesla_claw_moby);
            } else {
                tesla_claw_vars->target = 0;
                tesla_claw_vars->target2 = 0;
                tesla_claw_vars->lock = 0;
                tesla_claw_vars->lock2 = 0;
                tesla_claw_vars->f44 = 0;
                tesla_claw_vars->f46 = 0;
            }
        }
    } else {
        tesla_claw_vars->target = 0;
        tesla_claw_vars->target2 = 0;
        tesla_claw_vars->lock = 0;
        tesla_claw_vars->lock2 = 0;
    }
    if (!active) {
        tesla_beam_restart_pending = 1;
        if (tesla_claw_vars->glow > 0)
            tesla_claw_vars->glow--;
        if (tesla_claw_vars->sound != -1) {
            VoiceSlot *firing_voice_slot_window = (VoiceSlot *)&voice_pool + tesla_claw_vars->sound;
            if ((struct Moby *)firing_voice_slot_window[1].owner == tesla_claw_moby &&
                firing_voice_slot_window[1].state)
                release_voice_slot(tesla_claw_vars->sound);
        }
        tesla_claw_vars->sound = -1;
    } else if (!FUN_L00_0028d8c0_2b2100(tesla_claw_moby, tesla_claw_vars->sound)) {
        tesla_claw_vars->sound = allocate_voice_for_target_entry(4, 4, tesla_claw_moby);
    }
    if (!tesla_claw_vars->target || tesla_claw_vars->target->state == 0xFE ||
        tesla_claw_vars->target->state == 0xFD || !(tesla_claw_vars->target->flags & 0x1000))
        tesla_claw_vars->lock = 1;
    if (!tesla_claw_vars->target2 || tesla_claw_vars->target2->state == 0xFE ||
        tesla_claw_vars->target2->state == 0xFD || !(tesla_claw_vars->target2->flags & 0x1000))
        tesla_claw_vars->lock2 = 1;
    switch (tesla_claw_moby->state) {
    case TESLA_CLAW_STATE_INITIALIZE:
        tesla_claw_vars->timer = scale_game_frames(16);
        tesla_claw_vars->light = FUN_L00_0023e5e0_2b2100(&tesla_claw_vars->pos, 0.0f, 0.0f, 0);
        tesla_beam_restart_pending = 1;
        tesla_claw_moby->state = 1;
        tesla_claw_vars->f3E = 20;
        tesla_claw_vars->sound = -1;
        tesla_claw_vars->target2 = 0;
        tesla_claw_vars->target = 0;
        tesla_claw_vars->lock = 0;
        tesla_claw_vars->lock2 = 0;
        tesla_claw_vars->f46 = 0;
        tesla_claw_vars->f44 = 0;
        break;
    case TESLA_CLAW_STATE_WAIT_FOR_FIRE:
        fast_dec_timer(&tesla_claw_vars->timer);
        if (active) {
            start_equipped_item_animation();
            tesla_claw_moby->state = 3;
        }
        if ((controller_state.pressed & (player_hero = &hero)->items[0].button_mask) &&
            !player_hero->item_use_locked) {
            if (active) {
                Stats *st = (Stats *)(D_0014161B + 0x65);
                if (st->uses < 65535)
                    st->uses++;
                if (scale_game_frames(D_0015EEA4) / 600 > st->longest)
                    st->longest = scale_game_frames(D_0015EEA4) / 600;
                st->levels = (st->levels | (1 << current_level_index)) | 0x80000000;
                start_equipped_item_animation();
                tesla_claw_moby->state = 3;
            } else {
                tesla_claw_vars->target = 0;
                tesla_claw_vars->target2 = 0;
                tesla_claw_vars->lock = 0;
                tesla_claw_vars->lock2 = 0;
                tesla_claw_moby->state = 1;
                allocate_voice_for_target_entry(0, 0, tesla_claw_moby);
                cancel_hero_item_animation();
            }
        }
        break;
    case 2:
        break;
    case TESLA_CLAW_STATE_FIRING:
        if ((controller_state.released & hero.items[0].button_mask) || !active ||
            hero.item_use_locked) {
            player_hero = &hero;
            if (scale_game_frames(70) < player_hero->item_anim_frames) {
                if (((Stats2 *)(D_0014161B + 0x22D))->uses < 65535)
                    ((Stats2 *)(D_0014161B + 0x22D))->uses++;
            }
            if (scale_game_frames(D_0015EEA4) / 600 > ((Stats2 *)(D_0014161B + 0x22D))->longest)
                ((Stats2 *)(D_0014161B + 0x22D))->longest = scale_game_frames(D_0015EEA4) / 600;
            ((Stats2 *)(D_0014161B + 0x22D))->levels =
                (((Stats2 *)(D_0014161B + 0x22D))->levels | (1 << current_level_index)) |
                0x80000000;
            if (hero.item_anim_frames < scale_game_frames(30))
                hero.wrench_combo_step++;
            else
                hero.wrench_combo_step = 0;
            tesla_claw_vars->target = 0;
            tesla_claw_vars->target2 = 0;
            tesla_claw_vars->lock = 0;
            tesla_claw_vars->lock2 = 0;
            tesla_claw_vars->reach = 0.0f;
            cancel_hero_item_animation();
            tesla_claw_moby->state = 1;
        }
        break;
    case TESLA_CLAW_STATE_FINISHED:
        break;
    case 5:
        break;
    }
    if (tesla_claw_vars->light != -1) {
        float k;
        qzero(&transformed_offset);
        transformed_offset.f.x = 2.0f;
        transformed_offset.f.z = 2.0f;
        transform_vector_by_basis(&transformed_offset, &transformed_offset, (char *)&hero + 0x640);
        point_lights[tesla_claw_vars->light].position.f[0] =
            hero.motion.pos.f[0] + transformed_offset.f.x;
        point_lights[tesla_claw_vars->light].position.f[1] =
            hero.motion.pos.f[1] + transformed_offset.f.y;
        point_lights[tesla_claw_vars->light].position.f[2] =
            hero.motion.pos.f[2] + transformed_offset.f.z;
        point_lights[tesla_claw_vars->light].position.f[3] = 7.5f;
        k = convert_integer_to_float(tesla_claw_vars->glow) / (float)scale_game_frames(10);
        point_lights[tesla_claw_vars->light].color[2] = k;
        k = k * 0.7f;
        point_lights[tesla_claw_vars->light].color[1] = k;
        point_lights[tesla_claw_vars->light].color[0] =
            point_lights[tesla_claw_vars->light].color[1];
        if (done && tesla_claw_vars->light != -1) {
            release_point_light_slot(tesla_claw_vars->light);
            tesla_claw_vars->light = -1;
        }
    }
    if (done && tesla_claw_vars->light == -1)
        tesla_claw_moby->state = 4;
}

/* Starts the beam when one is pending: lays 19 points from the moby toward its target and resets the segment state. */
void initialize_tesla_claw_beam(struct Moby *moby) __asm__("FUN_L00_002b9310");

void initialize_tesla_claw_beam(struct Moby *moby) {
    V2b9310 beam_endpoint_delta;
    V2b9310 step;
    char   *tesla_claw_beam_vars;
    int     i;
    int     j;

    tesla_claw_beam_vars = ((char *)moby->pvars);
    if (tesla_beam_restart_pending_gp == 0) {
        return;
    }
    tesla_beam_restart_pending_gp = 0;
    D_L00_0016167C = 0;
    tesla_beam_scaled_initial_frame_count = scale_game_frames(tesla_beam_initial_frame_count_gp);
    D_L00_001615F8 = 0x7F2020;
    fast_vec_sub(&beam_endpoint_delta, tesla_claw_beam_vars + 0x10, tesla_claw_beam_vars);
    fast_vec_normalize(&step, &beam_endpoint_delta, D_L00_00161594 / 20.0f);
    qcopy(D_L00_001DB5C0, tesla_claw_beam_vars);
    for (i = 1; i < 20; i++) {
        fast_vec_add(&D_L00_001DB5C0[i], &l00_tesla_beam_previous_point_window[i], &step);
        D_L00_001DB840[i] = 0;
        clear_u64_value(&D_L00_001DB890[i]);
        D_L00_001DB9D0[i] = 0;
    }
    for (j = 0; j < 4; j++) {
        D_L00_00161658[j] = -1;
    }
    D_L00_00161668[0] = 8;
    D_L00_0016166C = 8;
    D_L00_00161670[0] = 4;
    D_L00_0016166A = 4;
    D_L00_00161672 = 2;
    D_L00_00161674 = 4;
    D_L00_0016166E = 4;
    D_L00_00161676 = 2;
    D_L00_001615AC_gp = 0.0f;
    D_L00_001615B4_gp = 0.0f;
    D_L00_001615C8_gp = 0.0f;
}
