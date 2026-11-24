// Splits a PlayStation 2 movie (.PSS: an MPEG-2 program stream) into its video elementary stream
// (stream 0xE0) and one audio track. Audio is private stream 1; each packet's payload starts with
// four bytes ff a1 00 NN, NN the track (the game's movies carry one per language: 0, 2, 3, 4, 5;
// play_mpeg_movie's fifth argument picks it). A track's first packet starts with Sony's "SShd"
// header (type, rate, channels, interleave), then "SSbd" (data size) and PS2 ADPCM data, the
// channels interleaved. Returns { video: Uint8Array, audio: { type, rate, channels, interleave,
// data: Uint8Array } | null, tracks: [NN...] }.
export function demuxPss(bytes, track = 0) {
  const parts = [], audioParts = [], tracks = new Set();
  let videoLength = 0, audioLength = 0;
  let i = 0;
  while (i + 4 <= bytes.length) {
    if (bytes[i] !== 0 || bytes[i + 1] !== 0 || bytes[i + 2] !== 1) { i++; continue; }
    const code = bytes[i + 3];
    if (code === 0xBA) { i += (bytes[i + 4] >> 6) === 1 ? 14 + (bytes[i + 13] & 7) : 12; continue; }
    if (code === 0xB9) break;
    if (code < 0xBB) { i++; continue; }
    const length = (bytes[i + 4] << 8) | bytes[i + 5];
    const end = i + 6 + length;
    if (code === 0xE0 || code === 0xBD) {
      const headerLength = bytes[i + 8];
      let payload = i + 9 + headerLength;
      if (code === 0xE0) { parts.push(bytes.subarray(payload, end)); videoLength += end - payload; }
      else {
        const id = bytes[payload + 3];
        tracks.add(id);
        if (bytes[payload] === 0xFF && id === track) { payload += 4; audioParts.push(bytes.subarray(payload, end)); audioLength += end - payload; }
      }
    }
    i = end;
  }
  const join = (list, total) => { const out = new Uint8Array(total); let at = 0; for (const p of list) { out.set(p, at); at += p.length; } return out; };
  const video = join(parts, videoLength);
  let audio = null;
  if (audioLength) {
    const raw = join(audioParts, audioLength);
    const v = new DataView(raw.buffer);
    if (raw[0] === 0x53 && raw[1] === 0x53 && raw[2] === 0x68 && raw[3] === 0x64) { // SShd
      const headerSize = v.getUint32(4, true);
      const type = v.getUint32(8, true), rate = v.getUint32(12, true), channels = v.getUint32(16, true), interleave = v.getUint32(20, true);
      const body = 8 + headerSize; // SSbd
      const dataSize = v.getUint32(body + 4, true);
      audio = { type, rate, channels, interleave, data: raw.subarray(body + 8, Math.min(raw.length, body + 8 + dataSize)) };
    }
  }
  return { video, audio, tracks: [...tracks] };
}
