// PS2 SPU ADPCM ("VAG" sample format): 16-byte blocks, each 28 samples. Byte 0: predictor (high
// nibble) and shift (low nibble); byte 1: flags; 14 bytes of 4-bit samples, low nibble first.
// sample = (nibble << 12 as a signed 16-bit value) >> shift, plus (s1 * F0 + s2 * F1) >> 6 with
// the SPU's filter pairs, clamped to 16 bits.
const F0 = [0, 60, 115, 98, 122], F1 = [0, 0, -52, -55, -60];

// Decodes interleaved channel data (each channel `interleave` bytes in turn) to Float32 channels.
export function decodePs2Adpcm(data, channels, interleave) {
  const blocksPerChannel = Math.floor(data.length / channels / 16);
  const out = Array.from({ length: channels }, () => new Float32Array(blocksPerChannel * 28));
  const history = Array.from({ length: channels }, () => [0, 0]);
  const written = new Array(channels).fill(0);
  for (let at = 0, turn = 0; at + 16 <= data.length; at += interleave, turn++) {
    const ch = turn % channels;
    for (let block = at; block < at + interleave && block + 16 <= data.length; block += 16) {
      const predictor = Math.min(data[block] >> 4, 4), shift = data[block] & 15;
      const h = history[ch], dst = out[ch];
      for (let i = 0; i < 28; i++) {
        if (written[ch] >= dst.length) break;
        const byte = data[block + 2 + (i >> 1)];
        let nibble = (i & 1 ? byte >> 4 : byte & 15) << 12;
        if (nibble & 0x8000) nibble -= 0x10000;
        let s = (nibble >> shift) + ((h[0] * F0[predictor] + h[1] * F1[predictor]) >> 6);
        s = s > 32767 ? 32767 : s < -32768 ? -32768 : s;
        h[1] = h[0]; h[0] = s;
        dst[written[ch]++] = s / 32768;
      }
    }
  }
  return out;
}
