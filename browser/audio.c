/* Browser replacement for the part of the PS2 sound hardware that music needs: decoding the
   SPU2's 4-bit ADPCM ("VAG") blocks to 16-bit PCM. On the console the 989snd driver on the IOP
   feeds these blocks to the SPU2, which decodes them itself; here the page hands the same disc
   bytes to this module and plays the PCM through Web Audio.

   The game's music files are `VAGp` streams: a 0x30-byte big-endian header (data size at 0x0C,
   sample rate at 0x10) followed by 16-byte blocks. Each block is: predictor/shift byte, flag
   byte, then 14 bytes holding 28 samples, low nibble first.

   Input is staged by the caller in bounded chunks; decoder history carries across chunks so a
   stream can be decoded piecewise. Nothing here reads the disc or keeps game state. */
#include <stdint.h>

#define VAG_BLOCK_BYTES 16u
#define VAG_BLOCK_SAMPLES 28u
#define INPUT_LIMIT (64u * 1024u)
#define OUTPUT_LIMIT (INPUT_LIMIT / VAG_BLOCK_BYTES * VAG_BLOCK_SAMPLES)
#define GUARD_BYTES 16u
#define VAG_HEADER_BYTES 0x30u

static uint8_t input[INPUT_LIMIT + GUARD_BYTES];
static int16_t output[OUTPUT_LIMIT + GUARD_BYTES / 2u];
static int32_t history1;
static int32_t history2;
static int ended;
static uint32_t header_data_bytes;
static uint32_t header_sample_rate;
static uint32_t odd_shift_blocks;

/* Prediction filters as fixed-point /64 pairs: the five the SPU2 defines. */
static const int32_t filters[5][2] = {
    { 0, 0 }, { 60, 0 }, { 115, -52 }, { 98, -55 }, { 122, -60 },
};

static void fill_guards(void) {
    uint32_t i;
    for (i = 0; i < GUARD_BYTES; i++) input[INPUT_LIMIT + i] = 0xA5;
    for (i = 0; i < GUARD_BYTES / 2u; i++) output[OUTPUT_LIMIT + i] = (int16_t)0xA5A5;
}

void audio_reset(void) {
    uint32_t i;
    for (i = 0; i < INPUT_LIMIT; i++) input[i] = 0;
    for (i = 0; i < OUTPUT_LIMIT; i++) output[i] = 0;
    history1 = 0;
    history2 = 0;
    ended = 0;
    header_data_bytes = 0;
    header_sample_rate = 0;
    odd_shift_blocks = 0;
    fill_guards();
}

int audio_guards_valid(void) {
    uint32_t i;
    for (i = 0; i < GUARD_BYTES; i++) {
        if (input[INPUT_LIMIT + i] != 0xA5) return 0;
    }
    for (i = 0; i < GUARD_BYTES / 2u; i++) {
        if (output[OUTPUT_LIMIT + i] != (int16_t)0xA5A5) return 0;
    }
    return 1;
}

uint32_t audio_input_address(void) { return (uint32_t)(uintptr_t)input; }
uint32_t audio_input_capacity(void) { return INPUT_LIMIT; }
uint32_t audio_output_address(void) { return (uint32_t)(uintptr_t)output; }
uint32_t audio_header_bytes(void) { return VAG_HEADER_BYTES; }
uint32_t audio_data_bytes(void) { return header_data_bytes; }
uint32_t audio_sample_rate(void) { return header_sample_rate; }
int audio_ended(void) { return ended; }
uint32_t audio_odd_shift_blocks(void) { return odd_shift_blocks; }

static uint32_t big_endian_32(const uint8_t *at) {
    return ((uint32_t)at[0] << 24) | ((uint32_t)at[1] << 16) | ((uint32_t)at[2] << 8) | (uint32_t)at[3];
}

/* Validates a staged `VAGp` header and starts a new stream. Returns 1, or a negative reason:
   -1 too few bytes, -2 not a VAGp header, -3 data size is zero or not whole blocks,
   -4 implausible sample rate. */
int audio_begin(uint32_t staged_bytes) {
    uint32_t data_bytes;
    uint32_t rate;
    if (staged_bytes < VAG_HEADER_BYTES || staged_bytes > INPUT_LIMIT) return -1;
    if (input[0] != 'V' || input[1] != 'A' || input[2] != 'G' || input[3] != 'p') return -2;
    data_bytes = big_endian_32(input + 0x0C);
    rate = big_endian_32(input + 0x10);
    if (data_bytes == 0 || data_bytes % VAG_BLOCK_BYTES != 0) return -3;
    if (rate < 4000u || rate > 96000u) return -4;
    history1 = 0;
    history2 = 0;
    ended = 0;
    odd_shift_blocks = 0;
    header_data_bytes = data_bytes;
    header_sample_rate = rate;
    return 1;
}

/* Decodes `staged_bytes` of whole blocks from the input buffer. Returns the number of samples
   written to the output buffer, or a negative reason: -1 no stream begun, -2 size is not whole
   blocks or exceeds the buffer, -3 a block names a filter the SPU2 does not have.
   Stops at the stream's end block: flag 7 is a terminator carrying no samples; a block with
   flag bit 0 set is the last one that does. */
int audio_decode(uint32_t staged_bytes) {
    uint32_t at;
    uint32_t written = 0;
    if (header_data_bytes == 0) return -1;
    if (staged_bytes == 0 || staged_bytes > INPUT_LIMIT || staged_bytes % VAG_BLOCK_BYTES != 0) return -2;
    for (at = 0; at < staged_bytes && !ended; at += VAG_BLOCK_BYTES) {
        const uint8_t *block = input + at;
        uint32_t shift = block[0] & 0x0Fu;
        const uint32_t filter = block[0] >> 4;
        const uint8_t flags = block[1];
        uint32_t k;
        if (flags == 7u) {
            ended = 1;
            break;
        }
        if (filter > 4u) return -3;
        if (shift > 12u) {
            /* Not used by any block this decoder has been run on. The PlayStation SPU is
               documented to treat shifts 13-15 as 9; applied here and counted so that a
               stream relying on it is visible instead of silently guessed. */
            shift = 9u;
            odd_shift_blocks++;
        }
        for (k = 0; k < VAG_BLOCK_SAMPLES; k++) {
            int32_t nibble = (block[2u + k / 2u] >> (4u * (k & 1u))) & 0x0F;
            int32_t sample;
            if (nibble >= 8) nibble -= 16;
            sample = (nibble * 4096) >> shift;
            sample += (history1 * filters[filter][0] + history2 * filters[filter][1] + 32) >> 6;
            if (sample > 32767) sample = 32767;
            if (sample < -32768) sample = -32768;
            history2 = history1;
            history1 = sample;
            output[written++] = (int16_t)sample;
        }
        if (flags & 1u) ended = 1;
    }
    return (int)written;
}
