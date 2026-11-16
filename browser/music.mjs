// Level music from the local disc, played through Web Audio.
//
// What follows the original: the track table is the level header's music_tracks[] (disc_table +
// 0x2AA8), read through the native runtime; src/audio/music/music_start_track.c starts
// music_tracks[track], and music_start_track_body.c then replaces it with music_tracks[track + 1].
// run_level_main_loop starts track 0. On Veldin those are a short intro and a long body.
//
// What is a browser adapter, not original logic: the IOP sound driver's stream scheduling is
// replaced by decoding both tracks up front and playing them back to back; the body is looped,
// which is an assumption (the original music state machine in music_update.c is not run, so
// its pause/unpause, fades and transitions do not happen).
import { SECTOR_BYTES } from './iso-reader.mjs';

export const MUSIC_TRACK_SLOTS = 21;
const MAX_TRACK_BYTES = 64 * 1024 * 1024;

// Reads and decodes one `VAGp` stream starting at `sector`. Returns { sampleRate, samples }.
export async function decodeVagStream(reader, audio, sector, signal) {
  if (!Number.isSafeInteger(sector) || sector <= 0) throw new Error('Invalid music track location.');
  const start = sector * SECTOR_BYTES;
  const headerBytes = audio.audio_header_bytes();
  const capacity = audio.audio_input_capacity();
  audio.audio_reset();
  const stage = (bytes) => new Uint8Array(audio.memory.buffer, audio.audio_input_address(), bytes.length).set(bytes);
  stage(await reader.read(start, headerBytes, signal));
  const begun = audio.audio_begin(headerBytes);
  if (begun !== 1) throw new Error(`Music track at sector ${sector} is not a VAG stream (${begun}).`);
  const dataBytes = audio.audio_data_bytes();
  if (dataBytes > MAX_TRACK_BYTES) throw new Error('Music track is larger than the decoder allows.');
  const samples = new Float32Array(dataBytes / 16 * 28);
  let written = 0;
  for (let offset = 0; offset < dataBytes && audio.audio_ended() !== 1; offset += capacity) {
    signal?.throwIfAborted();
    const length = Math.min(capacity, dataBytes - offset);
    stage(await reader.read(start + headerBytes + offset, length, signal));
    const produced = audio.audio_decode(length);
    if (produced < 0) throw new Error(`Music decoding failed (${produced}).`);
    const pcm = new Int16Array(audio.memory.buffer, audio.audio_output_address(), produced);
    for (let index = 0; index < produced; index += 1) samples[written + index] = pcm[index] / 32768;
    written += produced;
  }
  if (audio.audio_guards_valid() !== 1) throw new Error('Music decoder buffers were overrun.');
  const sampleRate = audio.audio_sample_rate();
  audio.audio_reset();
  return { sampleRate, samples: samples.subarray(0, written) };
}

// The intro and body for the track the level starts. `api` is the native runtime with the level
// header loaded. Returns null when the level has no music at that slot.
export async function loadLevelMusic(api, reader, audio, track, signal) {
  if (!Number.isInteger(track) || track < 0 || track + 1 >= MUSIC_TRACK_SLOTS) throw new Error('Invalid music track.');
  const introSector = api.runtime_disc_music_track_sector(track);
  if (introSector <= 0) return null;
  const intro = await decodeVagStream(reader, audio, introSector, signal);
  const bodySector = api.runtime_disc_music_track_sector(track + 1);
  const body = bodySector > 0 ? await decodeVagStream(reader, audio, bodySector, signal) : null;
  return { track, introSector, bodySector, intro, body };
}

// Plays an intro once and then a looping body. The context starts suspended until a user
// gesture; resume() is safe to call from any input handler.
export class MusicPlayer {
  constructor() {
    this.context = null;
    this.gain = null;
    this.sources = [];
    this.analyser = null;
    this.muted = false;
    this.volume = 0.6;
  }

  get playing() { return this.sources.length > 0; }

  play(music) {
    this.stop();
    if (!music) return;
    this.context ??= new AudioContext();
    if (!this.gain) {
      this.gain = this.context.createGain();
      this.analyser = this.context.createAnalyser();
      this.analyser.fftSize = 2048;
      this.gain.connect(this.analyser);
      this.analyser.connect(this.context.destination);
    }
    this.gain.gain.value = this.muted ? 0 : this.volume;
    const buffer = ({ sampleRate, samples }) => {
      const audioBuffer = this.context.createBuffer(1, samples.length, sampleRate);
      audioBuffer.copyToChannel(samples, 0);
      return audioBuffer;
    };
    const startAt = this.context.currentTime + 0.05;
    const intro = this.context.createBufferSource();
    intro.buffer = buffer(music.intro);
    intro.connect(this.gain);
    intro.start(startAt);
    this.sources.push(intro);
    if (music.body) {
      const body = this.context.createBufferSource();
      body.buffer = buffer(music.body);
      body.loop = true;
      body.connect(this.gain);
      body.start(startAt + intro.buffer.duration);
      this.sources.push(body);
    } else {
      intro.loop = true;
    }
    this.resume();
  }

  // Context state and the RMS of what is reaching the output right now. For verification: a
  // level of 0 while "running" and unmuted means nothing audible is being produced.
  probe() {
    if (!this.context || !this.analyser) return { state: 'none', level: 0, time: 0 };
    const window = new Float32Array(this.analyser.fftSize);
    this.analyser.getFloatTimeDomainData(window);
    let sum = 0;
    for (const value of window) sum += value * value;
    return { state: this.context.state, level: Math.sqrt(sum / window.length), time: this.context.currentTime };
  }

  resume() {
    if (this.context?.state === 'suspended') this.context.resume().catch(() => {});
  }

  setMuted(muted) {
    this.muted = muted;
    if (this.gain) this.gain.gain.value = muted ? 0 : this.volume;
  }

  stop() {
    for (const source of this.sources) {
      try { source.stop(); } catch { /* not started yet */ }
      source.disconnect();
    }
    this.sources = [];
  }
}
