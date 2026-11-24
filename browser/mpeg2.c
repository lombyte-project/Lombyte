/* MPEG-2 video decoder for the game's movies (the PSS files play_mpeg_movie plays), written from
   ISO/IEC 13818-2. On the console the IPU decodes macroblocks for the SDK's libmpeg; in the browser
   play_mpeg_movie is a service (services.c -> spike_play_movie), and this decoder turns the
   movie's video elementary stream into RGBA frames in display order.

   Coverage: what the game's movies use (checked on the intro movie): Main Profile 4:2:0,
   progressive frame pictures with frame_pred_frame_dct = 1, I/P/B pictures. Anything else (field
   pictures, field prediction, dual prime, 4:2:2) stops with an error code instead of drawing
   something wrong.

   Interface: copy the elementary stream to mpeg2_input() (mpeg2_input_capacity bytes), call
   mpeg2_start(length) (returns width << 16 | height, or < 0), then mpeg2_next() until it returns
   0 (end) or < 0 (error); each 1 leaves the next display frame as RGBA at mpeg2_output(). */
#include <math.h>
#include <stdint.h>
#include <string.h>

#define INPUT_CAPACITY (64 * 1024 * 1024)
#define MAX_W 720
#define MAX_H 576

static uint8_t input[INPUT_CAPACITY];
static uint8_t output[MAX_W * MAX_H * 4];
static uint32_t input_length;

uint8_t *mpeg2_input(void) { return input; }
uint32_t mpeg2_input_capacity(void) { return INPUT_CAPACITY; }
uint8_t *mpeg2_output(void) { return output; }

/* ---- Bits ---- */
static uint32_t pos; /* bit position */
static inline uint32_t peek(int n) {
    const uint32_t byte = pos >> 3;
    uint64_t v = 0;
    for (int i = 0; i < 8; i++) v = (v << 8) | (byte + i < input_length ? input[byte + i] : 0);
    return (uint32_t)((v << (pos & 7)) >> (64 - n));
}
static inline void skip(int n) { pos += n; }
static inline uint32_t get(int n) { const uint32_t v = peek(n); pos += n; return v; }
static void align(void) { pos = (pos + 7) & ~7u; }
/* Next start code (00 00 01 xx) at or after the current byte; returns xx or -1. */
static int next_start_code(void) {
    align();
    uint32_t byte = pos >> 3;
    while (byte + 3 < input_length) {
        if (input[byte] == 0 && input[byte + 1] == 0 && input[byte + 2] == 1) { pos = byte * 8; return input[byte + 3]; }
        byte++;
    }
    pos = input_length * 8;
    return -1;
}

/* ---- VLC tables (13818-2 Annex B), as code strings, decoded with a binary tree ---- */
typedef struct { const char *code; int value; } Code;
typedef struct { int16_t child[2]; int32_t value; int32_t leaf; } Node;
#define MAX_NODES 1024
typedef struct { Node node[MAX_NODES]; int count; } Tree;
static int vlc_error;

static void tree_build(Tree *t, const Code *codes, int n) {
    /* Codes must be prefix-free: a code running through an earlier code's leaf, or ending on a
       node an earlier code continues past, sets vlc_error (a mistyped table entry). */
    t->count = 1;
    t->node[0].child[0] = t->node[0].child[1] = -1;
    t->node[0].leaf = 0;
    for (int i = 0; i < n; i++) {
        int at = 0;
        for (const char *c = codes[i].code; *c; c++) {
            const int bit = *c - '0';
            if (t->node[at].leaf) vlc_error = 1;
            if (t->node[at].child[bit] < 0) {
                const int k = t->count++;
                t->node[k].child[0] = t->node[k].child[1] = -1;
                t->node[k].leaf = 0;
                t->node[at].child[bit] = (int16_t)k;
            }
            at = t->node[at].child[bit];
        }
        if (t->node[at].leaf || t->node[at].child[0] >= 0 || t->node[at].child[1] >= 0) vlc_error = 1;
        t->node[at].leaf = 1;
        t->node[at].value = codes[i].value;
    }
}
static int tree_read(const Tree *t) {
    int at = 0;
    for (int length = 0; length < 18; length++) {
        const int k = t->node[at].child[get(1)];
        if (k < 0) return -100000; /* no such code */
        at = k;
        if (t->node[at].leaf) return t->node[at].value;
    }
    return -100000;
}

/* B-1 macroblock_address_increment (34 = escape, adds 33). */
static const Code MBA[] = {
    {"1", 1}, {"011", 2}, {"010", 3}, {"0011", 4}, {"0010", 5}, {"00011", 6}, {"00010", 7}, {"0000111", 8}, {"0000110", 9},
    {"00001011", 10}, {"00001010", 11}, {"00001001", 12}, {"00001000", 13}, {"00000111", 14}, {"00000110", 15},
    {"0000010111", 16}, {"0000010110", 17}, {"0000010101", 18}, {"0000010100", 19}, {"0000010011", 20}, {"0000010010", 21},
    {"00000100011", 22}, {"00000100010", 23}, {"00000100001", 24}, {"00000100000", 25}, {"00000011111", 26}, {"00000011110", 27},
    {"00000011101", 28}, {"00000011100", 29}, {"00000011011", 30}, {"00000011010", 31}, {"00000011001", 32}, {"00000011000", 33},
    {"00000001000", 34}, {"00000001111", 35} /* stuffing (MPEG-1), skipped */ };
/* Macroblock type flags. */
enum { QUANT = 1, FWD = 2, BWD = 4, PATTERN = 8, INTRA = 16 };
static const Code MBT_I[] = { {"1", INTRA}, {"01", INTRA | QUANT} };
static const Code MBT_P[] = { {"1", FWD | PATTERN}, {"01", PATTERN}, {"001", FWD}, {"00011", INTRA}, {"00010", QUANT | FWD | PATTERN},
    {"00001", QUANT | PATTERN}, {"000001", INTRA | QUANT} };
static const Code MBT_B[] = { {"10", FWD | BWD}, {"11", FWD | BWD | PATTERN}, {"010", BWD}, {"011", BWD | PATTERN}, {"0010", FWD},
    {"0011", FWD | PATTERN}, {"00011", INTRA}, {"00010", QUANT | FWD | BWD | PATTERN}, {"000011", QUANT | FWD | PATTERN},
    {"000010", QUANT | BWD | PATTERN}, {"000001", INTRA | QUANT} };
/* B-9 coded_block_pattern (4:2:0). */
static const Code CBP[] = {
    {"111", 60}, {"1101", 4}, {"1100", 8}, {"1011", 16}, {"1010", 32}, {"10011", 12}, {"10010", 48}, {"10001", 20}, {"10000", 40},
    {"01111", 28}, {"01110", 44}, {"01101", 52}, {"01100", 56}, {"01011", 1}, {"01010", 61}, {"01001", 2}, {"01000", 62},
    {"001111", 24}, {"001110", 36}, {"001101", 3}, {"001100", 63}, {"0010111", 5}, {"0010110", 9}, {"0010101", 17}, {"0010100", 33},
    {"0010011", 6}, {"0010010", 10}, {"0010001", 18}, {"0010000", 34}, {"00011111", 7}, {"00011110", 11}, {"00011101", 19},
    {"00011100", 35}, {"00011011", 13}, {"00011010", 49}, {"00011001", 21}, {"00011000", 41}, {"00010111", 14}, {"00010110", 50},
    {"00010101", 22}, {"00010100", 42}, {"00010011", 15}, {"00010010", 51}, {"00010001", 23}, {"00010000", 43}, {"00001111", 25},
    {"00001110", 37}, {"00001101", 26}, {"00001100", 38}, {"00001011", 29}, {"00001010", 45}, {"00001001", 53}, {"00001000", 57},
    {"00000111", 30}, {"00000110", 46}, {"00000101", 54}, {"00000100", 58}, {"000000111", 31}, {"000000110", 47}, {"000000101", 55},
    {"000000100", 59}, {"000000011", 27}, {"000000010", 39}, {"000000001", 0} };
/* B-10 motion_code magnitude (a sign bit follows a nonzero code). */
static const Code MOTION[] = { {"1", 0}, {"01", 1}, {"001", 2}, {"0001", 3}, {"000011", 4}, {"0000101", 5}, {"0000100", 6}, {"0000011", 7},
    {"000001011", 8}, {"000001010", 9}, {"000001001", 10}, {"0000010001", 11}, {"0000010000", 12}, {"0000001111", 13},
    {"0000001110", 14}, {"0000001101", 15}, {"0000001100", 16} };
/* B-12 / B-13 dct_dc_size. */
static const Code DC_LUMA[] = { {"100", 0}, {"00", 1}, {"01", 2}, {"101", 3}, {"110", 4}, {"1110", 5}, {"11110", 6}, {"111110", 7},
    {"1111110", 8}, {"11111110", 9}, {"111111110", 10}, {"111111111", 11} };
static const Code DC_CHROMA[] = { {"00", 0}, {"01", 1}, {"10", 2}, {"110", 3}, {"1110", 4}, {"11110", 5}, {"111110", 6}, {"1111110", 7},
    {"11111110", 8}, {"111111110", 9}, {"1111111110", 10}, {"1111111111", 11} };

/* B-14 and B-15 dct_coefficients: 111 (run, level) entries in one order, plus escape and EOB.
   Codes as (value, length) pairs; values are packed run << 8 | level; ESCAPE / EOB below. */
#define DCT_ESCAPE 0x10000
#define DCT_EOB 0x20000
static const uint8_t RL_RUN[111] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8,
    9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
static const uint8_t RL_LEVEL[111] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 1, 2, 3, 4, 5, 1, 2, 3, 4, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 1, 2,
    1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
static const uint16_t B14[113][2] = {
    {0x3, 2}, {0x4, 4}, {0x5, 5}, {0x6, 7}, {0x26, 8}, {0x21, 8}, {0xa, 10}, {0x1d, 12}, {0x18, 12}, {0x13, 12}, {0x10, 12},
    {0x1a, 13}, {0x19, 13}, {0x18, 13}, {0x17, 13}, {0x1f, 14}, {0x1e, 14}, {0x1d, 14}, {0x1c, 14}, {0x1b, 14}, {0x1a, 14},
    {0x19, 14}, {0x18, 14}, {0x17, 14}, {0x16, 14}, {0x15, 14}, {0x14, 14}, {0x13, 14}, {0x12, 14}, {0x11, 14}, {0x10, 14},
    {0x18, 15}, {0x17, 15}, {0x16, 15}, {0x15, 15}, {0x14, 15}, {0x13, 15}, {0x12, 15}, {0x11, 15}, {0x10, 15},
    {0x3, 3}, {0x6, 6}, {0x25, 8}, {0xc, 10}, {0x1b, 12}, {0x16, 13}, {0x15, 13}, {0x1f, 15}, {0x1e, 15}, {0x1d, 15},
    {0x1c, 15}, {0x1b, 15}, {0x1a, 15}, {0x19, 15}, {0x13, 16}, {0x12, 16}, {0x11, 16}, {0x10, 16},
    {0x5, 4}, {0x4, 7}, {0xb, 10}, {0x14, 12}, {0x14, 13}, {0x7, 5}, {0x24, 8}, {0x1c, 12}, {0x13, 13}, {0x6, 5}, {0xf, 10},
    {0x12, 12}, {0x7, 6}, {0x9, 10}, {0x12, 13}, {0x5, 6}, {0x1e, 12}, {0x14, 16}, {0x4, 6}, {0x15, 12}, {0x7, 7}, {0x11, 12},
    {0x5, 7}, {0x11, 13}, {0x27, 8}, {0x10, 13}, {0x23, 8}, {0x1a, 16}, {0x22, 8}, {0x19, 16}, {0x20, 8}, {0x18, 16},
    {0xe, 10}, {0x17, 16}, {0xd, 10}, {0x16, 16}, {0x8, 10}, {0x15, 16}, {0x1f, 12}, {0x1a, 12}, {0x19, 12}, {0x17, 12},
    {0x16, 12}, {0x1f, 13}, {0x1e, 13}, {0x1d, 13}, {0x1c, 13}, {0x1b, 13}, {0x1f, 16}, {0x1e, 16}, {0x1d, 16}, {0x1c, 16},
    {0x1b, 16}, {0x1, 6}, {0x2, 2} };
static const uint16_t B15[113][2] = {
    {0x02, 2}, {0x06, 3}, {0x07, 4}, {0x1c, 5}, {0x1d, 5}, {0x05, 6}, {0x04, 6}, {0x7b, 7}, {0x7c, 7}, {0x23, 8}, {0x22, 8},
    {0xfa, 8}, {0xfb, 8}, {0xfe, 8}, {0xff, 8}, {0x1f, 14}, {0x1e, 14}, {0x1d, 14}, {0x1c, 14}, {0x1b, 14}, {0x1a, 14},
    {0x19, 14}, {0x18, 14}, {0x17, 14}, {0x16, 14}, {0x15, 14}, {0x14, 14}, {0x13, 14}, {0x12, 14}, {0x11, 14}, {0x10, 14},
    {0x18, 15}, {0x17, 15}, {0x16, 15}, {0x15, 15}, {0x14, 15}, {0x13, 15}, {0x12, 15}, {0x11, 15}, {0x10, 15},
    {0x02, 3}, {0x06, 5}, {0x79, 7}, {0x27, 8}, {0x20, 8}, {0x16, 13}, {0x15, 13}, {0x1f, 15}, {0x1e, 15}, {0x1d, 15},
    {0x1c, 15}, {0x1b, 15}, {0x1a, 15}, {0x19, 15}, {0x13, 16}, {0x12, 16}, {0x11, 16}, {0x10, 16},
    {0x05, 5}, {0x07, 7}, {0xfc, 8}, {0x0c, 10}, {0x14, 13}, {0x07, 5}, {0x26, 8}, {0x1c, 12}, {0x13, 13}, {0x06, 6}, {0xfd, 8},
    {0x12, 12}, {0x07, 6}, {0x04, 9}, {0x12, 13}, {0x06, 7}, {0x1e, 12}, {0x14, 16}, {0x04, 7}, {0x15, 12}, {0x05, 7}, {0x11, 12},
    {0x78, 7}, {0x11, 13}, {0x7a, 7}, {0x10, 13}, {0x21, 8}, {0x1a, 16}, {0x25, 8}, {0x19, 16}, {0x24, 8}, {0x18, 16},
    {0x05, 9}, {0x17, 16}, {0x07, 9}, {0x16, 16}, {0x0d, 10}, {0x15, 16}, {0x1f, 12}, {0x1a, 12}, {0x19, 12}, {0x17, 12},
    {0x16, 12}, {0x1f, 13}, {0x1e, 13}, {0x1d, 13}, {0x1c, 13}, {0x1b, 13}, {0x1f, 16}, {0x1e, 16}, {0x1d, 16}, {0x1c, 16},
    {0x1b, 16}, {0x01, 6}, {0x06, 4} };

static Tree t_mba, t_mbt[3], t_cbp, t_motion, t_dc_luma, t_dc_chroma, t_b14, t_b15;

static void build_dct(Tree *t, const uint16_t table[113][2]) {
    static Code codes[113];
    static char text[113][20];
    for (int i = 0; i < 113; i++) {
        const int length = table[i][1];
        for (int b = 0; b < length; b++) text[i][b] = (char)('0' + ((table[i][0] >> (length - 1 - b)) & 1));
        text[i][length] = 0;
        codes[i].code = text[i];
        codes[i].value = i < 111 ? (RL_RUN[i] << 8 | RL_LEVEL[i]) : i == 111 ? DCT_ESCAPE : DCT_EOB;
    }
    tree_build(t, codes, 113);
}

static const uint8_t ZIGZAG[64] = { 0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6,
    7, 14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51, 58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54,
    47, 55, 62, 63 };
static const uint8_t ALTERNATE[64] = { 0, 8, 16, 24, 1, 9, 2, 10, 17, 25, 32, 40, 48, 56, 57, 49, 41, 33, 26, 18, 3, 11, 4, 12, 19, 27,
    34, 42, 50, 58, 35, 43, 51, 59, 20, 28, 5, 13, 6, 14, 21, 29, 36, 44, 52, 60, 37, 45, 53, 61, 22, 30, 7, 15, 23, 31, 38, 46,
    54, 62, 39, 47, 55, 63 };
static const uint8_t DEFAULT_INTRA[64] = { /* raster order */
    8, 16, 19, 22, 26, 27, 29, 34, 16, 16, 22, 24, 27, 29, 34, 37, 19, 22, 26, 27, 29, 34, 34, 38, 22, 22, 26, 27, 29, 34, 37, 40,
    22, 26, 27, 29, 32, 35, 40, 48, 26, 27, 29, 32, 35, 40, 48, 58, 26, 27, 29, 34, 38, 46, 56, 69, 27, 29, 35, 38, 46, 56, 69, 83 };
static const uint8_t NONLINEAR_Q[32] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 18, 20, 22, 24, 28, 32, 36, 40, 44, 48, 52, 56, 64,
    72, 80, 88, 96, 104, 112 };

/* ---- Decoder state ---- */
static int width, height, mb_width, mb_height;
static uint8_t intra_q[64], non_intra_q[64]; /* raster order */
static int picture_type, f_code[2][2], intra_dc_precision, picture_structure, frame_pred_frame_dct, concealment_mv, q_scale_type,
    intra_vlc_format, alternate_scan;
static int quantiser_scale;
static int dc_pred[3];
static int pmv[2][2]; /* [forward/backward][h/v] */
static int last_type, last_mv[2][2];

typedef struct { uint8_t y[MAX_W * MAX_H], cb[MAX_W * MAX_H / 4], cr[MAX_W * MAX_H / 4]; } Frame;
static Frame frames[3];
static int ref_forward = -1, ref_backward = -1; /* frames[] indices */
static int pending_display = -1;                /* a reference waiting to be shown */
static int error_code;

static double idct_c[8][8];

int32_t mpeg2_error(void) { return error_code; }

static void tables_init(void) {
    vlc_error = 0;
    tree_build(&t_mba, MBA, sizeof MBA / sizeof MBA[0]);
    tree_build(&t_mbt[0], MBT_I, 2);
    tree_build(&t_mbt[1], MBT_P, 7);
    tree_build(&t_mbt[2], MBT_B, 11);
    tree_build(&t_cbp, CBP, sizeof CBP / sizeof CBP[0]);
    tree_build(&t_motion, MOTION, sizeof MOTION / sizeof MOTION[0]);
    tree_build(&t_dc_luma, DC_LUMA, 12);
    tree_build(&t_dc_chroma, DC_CHROMA, 12);
    build_dct(&t_b14, B14);
    build_dct(&t_b15, B15);
    for (int k = 0; k < 8; k++)
        for (int n = 0; n < 8; n++)
            idct_c[k][n] = (k == 0 ? sqrt(0.125) : 0.5) * cos((2 * n + 1) * k * M_PI / 16.0);
}

/* Reference 2-D inverse DCT (separable, double precision), rounded and clamped to [-256, 255]. */
static void idct(const int32_t in[64], int32_t out[64]) {
    double tmp[64];
    for (int y = 0; y < 8; y++)
        for (int x = 0; x < 8; x++) {
            double s = 0;
            for (int u = 0; u < 8; u++) s += idct_c[u][x] * in[y * 8 + u];
            tmp[y * 8 + x] = s;
        }
    for (int x = 0; x < 8; x++)
        for (int y = 0; y < 8; y++) {
            double s = 0;
            for (int v = 0; v < 8; v++) s += idct_c[v][y] * tmp[v * 8 + x];
            int r = (int)floor(s + 0.5);
            out[y * 8 + x] = r < -256 ? -256 : r > 255 ? 255 : r;
        }
}

/* One block's coefficients (raster order, dequantized, mismatch-controlled). Returns 0 or an error. */
static int read_block(int index, int intra, int32_t f[64]) {
    int32_t qf[64];
    memset(qf, 0, sizeof qf);
    memset(f, 0, 64 * sizeof(int32_t));
    const uint8_t *scan = alternate_scan ? ALTERNATE : ZIGZAG;
    int n = 0;
    if (intra) {
        const int cc = index < 4 ? 0 : index == 4 ? 1 : 2;
        const int size = tree_read(cc == 0 ? &t_dc_luma : &t_dc_chroma);
        if (size < 0) return 10;
        int diff = 0;
        if (size) {
            diff = (int)get(size);
            if (!(diff & (1 << (size - 1)))) diff = diff - (1 << size) + 1;
        }
        dc_pred[cc] += diff;
        qf[0] = dc_pred[cc];
        n = 1;
    }
    const Tree *table = intra && intra_vlc_format ? &t_b15 : &t_b14;
    for (;;) {
        int run, level;
        if (!intra && n == 0 && peek(1) == 1) { /* first coefficient of a non-intra block: '1s' is run 0 level 1 */
            skip(1);
            run = 0; level = get(1) ? -1 : 1;
        } else {
            const int v = tree_read(table);
            if (v < 0) return 11;
            if (v == DCT_EOB) break;
            if (v == DCT_ESCAPE) {
                run = (int)get(6);
                level = (int)get(12);
                if (level & 0x800) level -= 0x1000;
                if (level == 0 || level == -2048) return 12;
            } else {
                run = v >> 8; level = v & 0xFF;
                if (get(1)) level = -level;
            }
        }
        n += run;
        if (n > 63) return 13;
        qf[scan[n]] = level;
        n++;
    }
    /* Inverse quantization (7.4). */
    int32_t sum = 0;
    for (int i = 0; i < 64; i++) {
        int32_t v;
        if (intra && i == 0) v = qf[0] * (8 >> intra_dc_precision);
        else if (intra) v = (qf[i] * intra_q[i] * quantiser_scale * 2) / 32;
        else v = qf[i] == 0 ? 0 : ((2 * qf[i] + (qf[i] > 0 ? 1 : -1)) * non_intra_q[i] * quantiser_scale) / 32;
        if (v > 2047) v = 2047;
        if (v < -2048) v = -2048;
        f[i] = v;
        sum += v;
    }
    if ((sum & 1) == 0) f[63] += (f[63] & 1) ? -1 : 1;
    return 0;
}

/* Motion vector component (7.6.3.1). */
static int motion_component(int s, int t) {
    const int code_abs = tree_read(&t_motion);
    if (code_abs < 0) { error_code = 20; return 0; }
    int code = code_abs;
    if (code_abs && get(1)) code = -code_abs;
    const int r_size = f_code[s][t] - 1, f = 1 << r_size;
    int delta;
    if (f == 1 || code == 0) delta = code;
    else {
        const int residual = (int)get(r_size);
        delta = ((code_abs - 1) * f) + residual + 1;
        if (code < 0) delta = -delta;
    }
    const int high = 16 * f - 1, low = -16 * f, range = 32 * f;
    int v = pmv[s][t] + delta;
    if (v < low) v += range;
    if (v > high) v -= range;
    pmv[s][t] = v;
    return v;
}

/* Prediction of one plane block from a reference (half-pel). */
static void predict(const uint8_t *ref, int stride, int plane_w, int plane_h, int x, int y, int w, int h, int mvx, int mvy,
                    uint8_t *dst, int average) {
    const int ix = x + (mvx >> 1), iy = y + (mvy >> 1), hx = mvx & 1, hy = mvy & 1;
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            int x0 = ix + i, y0 = iy + j;
            int x1 = x0 + hx, y1 = y0 + hy;
            x0 = x0 < 0 ? 0 : x0 >= plane_w ? plane_w - 1 : x0; x1 = x1 < 0 ? 0 : x1 >= plane_w ? plane_w - 1 : x1;
            y0 = y0 < 0 ? 0 : y0 >= plane_h ? plane_h - 1 : y0; y1 = y1 < 0 ? 0 : y1 >= plane_h ? plane_h - 1 : y1;
            int v;
            if (hx && hy) v = (ref[y0 * stride + x0] + ref[y0 * stride + x1] + ref[y1 * stride + x0] + ref[y1 * stride + x1] + 2) >> 2;
            else if (hx) v = (ref[y0 * stride + x0] + ref[y0 * stride + x1] + 1) >> 1;
            else if (hy) v = (ref[y0 * stride + x0] + ref[y1 * stride + x0] + 1) >> 1;
            else v = ref[y0 * stride + x0];
            uint8_t *d = dst + j * 16 + i;
            *d = average ? (uint8_t)((*d + v + 1) >> 1) : (uint8_t)v;
        }
    }
}

/* The motion-compensated prediction of a macroblock into pred (16x16 luma, two 8x8 chroma). */
static void mc_macroblock(int mbx, int mby, int type, int mv[2][2], uint8_t pred_y[256], uint8_t pred_cb[64], uint8_t pred_cr[64]) {
    int done = 0;
    for (int s = 0; s < 2; s++) {
        if (!(type & (s == 0 ? FWD : BWD))) continue;
        /* P pictures predict from the newest reference; B from the older (forward) and newer (backward). */
        const int ref = s == 0 ? (picture_type == 2 ? ref_backward : ref_forward) : ref_backward;
        if (ref < 0) continue;
        const Frame *r = &frames[ref];
        const int mvx = mv[s][0], mvy = mv[s][1];
        predict(r->y, width, width, height, mbx * 16, mby * 16, 16, 16, mvx, mvy, pred_y, done);
        const int cmx = mvx / 2, cmy = mvy / 2; /* 4:2:0 chroma vectors: halved, toward zero */
        predict(r->cb, width / 2, width / 2, height / 2, mbx * 8, mby * 8, 8, 8, cmx, cmy, pred_cb, done);
        predict(r->cr, width / 2, width / 2, height / 2, mbx * 8, mby * 8, 8, 8, cmx, cmy, pred_cr, done);
        done = 1;
    }
    if (!done) { memset(pred_y, 128, 256); memset(pred_cb, 128, 64); memset(pred_cr, 128, 64); }
}

/* The predicted chroma rows are 16 bytes apart in pred buffers too (predict writes j * 16). */
static uint8_t pred_y[256], pred_cb[128], pred_cr[128];

static void store_macroblock(Frame *cur, int mbx, int mby, int intra, int cbp, int32_t blocks[6][64]) {
    for (int b = 0; b < 6; b++) {
        uint8_t *plane; int stride, x0, y0; const uint8_t *pred;
        if (b < 4) { plane = cur->y; stride = width; x0 = mbx * 16 + (b & 1) * 8; y0 = mby * 16 + (b >> 1) * 8; pred = pred_y + (b >> 1) * 8 * 16 + (b & 1) * 8; }
        else { plane = b == 4 ? cur->cb : cur->cr; stride = width / 2; x0 = mbx * 8; y0 = mby * 8; pred = b == 4 ? pred_cb : pred_cr; }
        const int coded = intra || (cbp & (32 >> b));
        int32_t res[64];
        if (coded) idct(blocks[b], res);
        for (int y = 0; y < 8; y++)
            for (int x = 0; x < 8; x++) {
                int v = intra ? res[y * 8 + x] : pred[y * 16 + x] + (coded ? res[y * 8 + x] : 0);
                v = v < 0 ? 0 : v > 255 ? 255 : v;
                plane[(y0 + y) * stride + x0 + x] = (uint8_t)v;
            }
    }
}

static void reset_dc(void) { dc_pred[0] = dc_pred[1] = dc_pred[2] = 1 << (intra_dc_precision + 7); }

static int decode_slice(Frame *cur, int vertical) {
    quantiser_scale = q_scale_type ? NONLINEAR_Q[get(5)] : (int)get(5) * 2;
    if (peek(1)) { skip(9); while (get(1)) skip(8); } else skip(1);
    reset_dc();
    pmv[0][0] = pmv[0][1] = pmv[1][0] = pmv[1][1] = 0;
    int address = (vertical - 1) * mb_width - 1;
    int first = 1;
    while (peek(23) != 0) {
        int increment = 0;
        for (;;) {
            const int v = tree_read(&t_mba);
            if (v < 0) return 30;
            if (v == 35) continue;
            if (v == 34) { increment += 33; continue; }
            increment += v;
            break;
        }
        /* Skipped macroblocks (not before the first of a slice). */
        if (!first && increment > 1) {
            for (int k = 1; k < increment; k++) {
                const int a = address + k, mbx = a % mb_width, mby = a / mb_width;
                int mv[2][2] = { {0, 0}, {0, 0} };
                int type = FWD;
                if (picture_type == 3) { type = last_type; memcpy(mv, last_mv, sizeof mv); }
                else { pmv[0][0] = pmv[0][1] = 0; }
                mc_macroblock(mbx, mby, type, mv, pred_y, pred_cb, pred_cr);
                int32_t none[6][64];
                store_macroblock(cur, mbx, mby, 0, 0, none);
            }
            reset_dc();
        }
        address += increment;
        first = 0;
        const int mbx = address % mb_width, mby = address / mb_width;
        if (mby >= mb_height) return 31;
        const int type = tree_read(&t_mbt[picture_type - 1]);
        if (type < 0) return 32;
        if (!frame_pred_frame_dct) return 33; /* frame_motion_type / dct_type would follow */
        if (type & QUANT) { const int code = (int)get(5); quantiser_scale = q_scale_type ? NONLINEAR_Q[code] : code * 2; }
        int mv[2][2] = { {0, 0}, {0, 0} };
        if ((type & FWD) || ((type & INTRA) && concealment_mv)) { mv[0][0] = motion_component(0, 0); mv[0][1] = motion_component(0, 1); }
        if (type & BWD) { mv[1][0] = motion_component(1, 0); mv[1][1] = motion_component(1, 1); }
        if (error_code) return error_code;
        if ((type & INTRA) && concealment_mv) skip(1);
        int cbp = 0;
        if (type & PATTERN) { cbp = tree_read(&t_cbp); if (cbp < 0) return 34; }
        if (type & INTRA) cbp = 63;
        else reset_dc();
        if (type & INTRA) { if (!concealment_mv) { pmv[0][0] = pmv[0][1] = pmv[1][0] = pmv[1][1] = 0; } }
        else if (picture_type == 2 && !(type & FWD)) pmv[0][0] = pmv[0][1] = 0; /* P, no motion compensation: zero vector */
        static int32_t blocks[6][64];
        for (int b = 0; b < 6; b++) {
            if (cbp & (32 >> b)) { const int e = read_block(b, type & INTRA, blocks[b]); if (e) return e; }
        }
        if (!(type & INTRA)) {
            int t = type;
            if (picture_type == 2) t |= FWD; /* a P macroblock without vectors predicts with a zero vector */
            mc_macroblock(mbx, mby, t, mv, pred_y, pred_cb, pred_cr);
        }
        store_macroblock(cur, mbx, mby, type & INTRA, cbp, blocks);
        if (picture_type == 3) { last_type = type & (FWD | BWD); memcpy(last_mv, mv, sizeof mv); }
    }
    return 0;
}

static void to_rgba(const Frame *f) {
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            const double yy = 1.164 * (f->y[y * width + x] - 16);
            const double cb = f->cb[(y >> 1) * (width / 2) + (x >> 1)] - 128, cr = f->cr[(y >> 1) * (width / 2) + (x >> 1)] - 128;
            int r = (int)(yy + 1.596 * cr + 0.5), g = (int)(yy - 0.813 * cr - 0.391 * cb + 0.5), b = (int)(yy + 2.018 * cb + 0.5);
            uint8_t *o = output + (y * width + x) * 4;
            o[0] = (uint8_t)(r < 0 ? 0 : r > 255 ? 255 : r); o[1] = (uint8_t)(g < 0 ? 0 : g > 255 ? 255 : g);
            o[2] = (uint8_t)(b < 0 ? 0 : b > 255 ? 255 : b); o[3] = 255;
        }
}

static void read_quant_matrix(uint8_t m[64]) { for (int i = 0; i < 64; i++) m[ZIGZAG[i]] = (uint8_t)get(8); }

static int parse_sequence_header(void) {
    width = (int)get(12); height = (int)get(12);
    skip(4 + 4 + 18 + 1 + 10 + 1);
    if (get(1)) read_quant_matrix(intra_q); else memcpy(intra_q, DEFAULT_INTRA, 64);
    if (get(1)) read_quant_matrix(non_intra_q); else memset(non_intra_q, 16, 64);
    if (width <= 0 || height <= 0 || width > MAX_W || height > MAX_H) return 40;
    mb_width = (width + 15) / 16; mb_height = (height + 15) / 16;
    return 0;
}

int32_t mpeg2_start(uint32_t length) {
    input_length = length > INPUT_CAPACITY ? INPUT_CAPACITY : length;
    pos = 0; error_code = 0;
    ref_forward = ref_backward = pending_display = -1;
    tables_init();
    if (vlc_error) return -1;
    for (;;) {
        const int code = next_start_code();
        if (code < 0) return -2;
        skip(32);
        if (code == 0xB3) { const int e = parse_sequence_header(); if (e) return -e; return width << 16 | height; }
    }
}

/* Decodes one picture (after its picture start code) into a free frame; returns the frame index or < 0. */
static int decode_picture(void) {
    skip(10); /* temporal_reference */
    picture_type = (int)get(3);
    skip(16);
    if (picture_type == 2 || picture_type == 3) skip(4);
    if (picture_type == 3) skip(4);
    while (get(1)) skip(8);
    if (picture_type < 1 || picture_type > 3) return -50;
    /* Extensions and user data until the first slice. */
    int code;
    for (;;) {
        code = next_start_code();
        if (code < 0) return -51;
        if (code >= 0x01 && code <= 0xAF) break;
        skip(32);
        if (code == 0xB5) {
            const int id = (int)get(4);
            if (id == 8) {
                f_code[0][0] = (int)get(4); f_code[0][1] = (int)get(4); f_code[1][0] = (int)get(4); f_code[1][1] = (int)get(4);
                intra_dc_precision = (int)get(2); picture_structure = (int)get(2); skip(1);
                frame_pred_frame_dct = (int)get(1); concealment_mv = (int)get(1); q_scale_type = (int)get(1);
                intra_vlc_format = (int)get(1); alternate_scan = (int)get(1);
                if (picture_structure != 3) return -52;
            } else if (id == 3) {
                if (get(1)) read_quant_matrix(intra_q);
                if (get(1)) read_quant_matrix(non_intra_q);
            }
        }
    }
    /* Pick a frame that is neither reference. */
    int target = 0;
    while (target == ref_forward || target == ref_backward) target++;
    Frame *cur = &frames[target];
    last_type = 0; memset(last_mv, 0, sizeof last_mv);
    while (code >= 0x01 && code <= 0xAF) {
        skip(32);
        const int e = decode_slice(cur, code);
        if (e) return -100 - e;
        code = next_start_code();
    }
    return target;
}

/* Decodes until the next frame to display; 1 = frame in mpeg2_output, 0 = end, < 0 = error. */
int32_t mpeg2_next(void) {
    for (;;) {
        const int code = next_start_code();
        if (code < 0 || code == 0xB7) { /* end: show the last reference */
            if (pending_display >= 0) { to_rgba(&frames[pending_display]); pending_display = -1; return 1; }
            return 0;
        }
        if (code == 0xB3) { skip(32); const int e = parse_sequence_header(); if (e) return -e; continue; }
        if (code != 0x00) { skip(32); continue; }
        skip(32);
        const int index = decode_picture();
        if (index < 0) { error_code = -index; return index; }
        if (picture_type == 3) { to_rgba(&frames[index]); return 1; } /* B pictures show at once */
        /* A new reference: the older one moves back, and the previous reference is displayed now. */
        ref_forward = ref_backward;
        ref_backward = index;
        const int show = pending_display;
        pending_display = index;
        if (show >= 0) { to_rgba(&frames[show]); return 1; }
    }
}
