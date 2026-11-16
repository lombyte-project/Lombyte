import assert from 'node:assert/strict';
import { openAsBlob } from 'node:fs';
import { readFile } from 'node:fs/promises';
import { test } from 'node:test';
import { IsoReader, SECTOR_BYTES } from './iso-reader.mjs';
import { initializeDiscTable } from './disc-loader.mjs';
import { loadLevelGameplay, VELDIN_LEVEL } from './gameplay-loader.mjs';
import { decodeVagStream, loadLevelMusic } from './music.mjs';

const load = name => readFile(new URL(`../build/browser/${name}.wasm`, import.meta.url)).then(WebAssembly.compile);
const [audioModule, runtimeModule, wadModule] = await Promise.all(['audio', 'runtime', 'wad'].map(load));
const isoPath = process.env.RNC_ISO_PATH;

async function instantiate(module) {
  const instance = await WebAssembly.instantiate(module, {});
  instance.exports._initialize?.();
  return instance.exports;
}

// Written independently of browser/audio.c: floating-point prediction with rounding to nearest
// and the same end-block rules.
function referenceDecode(blocks) {
  const filters = [[0, 0], [60 / 64, 0], [115 / 64, -52 / 64], [98 / 64, -55 / 64], [122 / 64, -60 / 64]];
  const out = [];
  let h1 = 0;
  let h2 = 0;
  for (let at = 0; at < blocks.length; at += 16) {
    if (blocks[at + 1] === 7) break;
    const shift = blocks[at] & 15;
    const [a, b] = filters[blocks[at] >> 4];
    for (let k = 0; k < 28; k += 1) {
      let nibble = (blocks[at + 2 + (k >> 1)] >> (4 * (k & 1))) & 15;
      if (nibble >= 8) nibble -= 16;
      const predicted = Math.floor((h1 * a * 64 + h2 * b * 64 + 32) / 64);
      const sample = Math.max(-32768, Math.min(32767, Math.floor(nibble * 4096 / 2 ** shift) + predicted));
      h2 = h1;
      h1 = sample;
      out.push(sample);
    }
    if (blocks[at + 1] & 1) break;
  }
  return out;
}

function header(dataBytes, rate = 44100) {
  const bytes = new Uint8Array(0x30);
  bytes.set([0x56, 0x41, 0x47, 0x70]);
  new DataView(bytes.buffer).setUint32(0x0C, dataBytes);
  new DataView(bytes.buffer).setUint32(0x10, rate);
  return bytes;
}

function stage(audio, bytes) {
  new Uint8Array(audio.memory.buffer, audio.audio_input_address(), bytes.length).set(bytes);
}

function decodeAll(audio, blocks) {
  stage(audio, blocks);
  const produced = audio.audio_decode(blocks.length);
  return produced < 0 ? produced : [...new Int16Array(audio.memory.buffer, audio.audio_output_address(), produced)];
}

test('headers are validated before a stream starts', async () => {
  const audio = await instantiate(audioModule);
  audio.audio_reset();
  assert.equal(audio.audio_decode(16), -1, 'decoding needs a begun stream');
  stage(audio, header(32));
  assert.equal(audio.audio_begin(0x2F), -1);
  const wrongMagic = header(32);
  wrongMagic[3] = 0x20;
  stage(audio, wrongMagic);
  assert.equal(audio.audio_begin(0x30), -2);
  stage(audio, header(0));
  assert.equal(audio.audio_begin(0x30), -3);
  stage(audio, header(40));
  assert.equal(audio.audio_begin(0x30), -3, 'data must be whole 16-byte blocks');
  stage(audio, header(32, 100));
  assert.equal(audio.audio_begin(0x30), -4);
  stage(audio, header(32, 44100));
  assert.equal(audio.audio_begin(0x30), 1);
  assert.equal(audio.audio_data_bytes(), 32);
  assert.equal(audio.audio_sample_rate(), 44100);
  assert.equal(audio.audio_guards_valid(), 1);
});

test('blocks decode to the reference samples for every filter and shift', async () => {
  const audio = await instantiate(audioModule);
  let seed = 12345;
  const random = () => { seed = (seed * 1103515245 + 12345) >>> 0; return (seed >>> 16) & 0xFF; };
  const blocks = new Uint8Array(16 * 65);
  for (let block = 0; block < 65; block += 1) {
    blocks[block * 16] = ((block % 5) << 4) | (block % 13);
    blocks[block * 16 + 1] = 0;
    for (let k = 2; k < 16; k += 1) blocks[block * 16 + k] = random();
  }
  audio.audio_reset();
  stage(audio, header(blocks.length));
  assert.equal(audio.audio_begin(0x30), 1);
  const decoded = decodeAll(audio, blocks);
  assert.equal(decoded.length, 65 * 28);
  assert.deepEqual(decoded, referenceDecode(blocks));
  assert.ok(decoded.some(sample => sample === 32767 || sample === -32768) || Math.max(...decoded.map(Math.abs)) > 1000);
  assert.equal(audio.audio_odd_shift_blocks(), 0);
  assert.equal(audio.audio_guards_valid(), 1);
});

test('history carries across chunks and end blocks stop the stream', async () => {
  const audio = await instantiate(audioModule);
  const blocks = new Uint8Array(16 * 4);
  for (let block = 0; block < 4; block += 1) {
    blocks[block * 16] = 0x24;
    for (let k = 2; k < 16; k += 1) blocks[block * 16 + k] = 0x31 + block * 7 + k;
  }
  audio.audio_reset();
  stage(audio, header(blocks.length));
  audio.audio_begin(0x30);
  const whole = decodeAll(audio, blocks);
  audio.audio_reset();
  stage(audio, header(blocks.length));
  audio.audio_begin(0x30);
  const split = [...decodeAll(audio, blocks.subarray(0, 32)), ...decodeAll(audio, blocks.subarray(32))];
  assert.deepEqual(split, whole, 'two chunks equal one');

  const ending = blocks.slice();
  ending[16 + 1] = 1; // last block that carries samples
  audio.audio_reset();
  stage(audio, header(ending.length));
  audio.audio_begin(0x30);
  assert.equal(decodeAll(audio, ending).length, 2 * 28);
  assert.equal(audio.audio_ended(), 1);
  assert.equal(audio.audio_decode(16), 0, 'nothing after the end');

  const terminated = blocks.slice();
  terminated[32 + 1] = 7; // terminator: no samples
  audio.audio_reset();
  stage(audio, header(terminated.length));
  audio.audio_begin(0x30);
  assert.equal(decodeAll(audio, terminated).length, 2 * 28);
  assert.equal(audio.audio_ended(), 1);
});

test('malformed blocks and sizes are rejected', async () => {
  const audio = await instantiate(audioModule);
  audio.audio_reset();
  stage(audio, header(32));
  audio.audio_begin(0x30);
  assert.equal(audio.audio_decode(0), -2);
  assert.equal(audio.audio_decode(24), -2);
  assert.equal(audio.audio_decode(audio.audio_input_capacity() + 16), -2);
  const badFilter = new Uint8Array(16);
  badFilter[0] = 0x50;
  assert.equal(decodeAll(audio, badFilter), -3);
  assert.equal(audio.audio_guards_valid(), 1);
});

test('Veldin music: track table, both streams, and decoded audio from the local disc', {
  skip: isoPath ? false : 'Set RNC_ISO_PATH for the local-disc check',
}, async () => {
  const [audio, api, decoder] = await Promise.all([audioModule, runtimeModule, wadModule].map(instantiate));
  api.runtime_init();
  const reader = new IsoReader(await openAsBlob(isoPath));
  await reader.open();
  await initializeDiscTable(api, reader);
  assert.equal(api.runtime_disc_music_track_sector(0), -1, 'no level header loaded yet');
  await loadLevelGameplay(api, reader, decoder, VELDIN_LEVEL);
  assert.equal(api.runtime_disc_music_track_sector(21), -1);
  const intro = api.runtime_disc_music_track_sector(0);
  const body = api.runtime_disc_music_track_sector(1);
  assert.ok(intro > 0 && body > intro, 'Veldin has an intro track followed by a body track');
  for (let track = 2; track < 21; track += 1) assert.equal(api.runtime_disc_music_track_sector(track), 0);

  const music = await loadLevelMusic(api, reader, audio, 0);
  assert.equal(music.intro.sampleRate, 44100);
  assert.equal(music.body.sampleRate, 44100);
  const seconds = track => track.samples.length / track.sampleRate;
  assert.ok(seconds(music.intro) > 5 && seconds(music.intro) < 15, `intro is ${seconds(music.intro)} s`);
  assert.ok(seconds(music.body) > 60 && seconds(music.body) < 300, `body is ${seconds(music.body)} s`);
  for (const track of [music.intro, music.body]) {
    let sum = 0;
    let peak = 0;
    let jumps = 0;
    for (let index = 1; index < track.samples.length; index += 1) {
      const value = track.samples[index];
      assert.ok(Number.isFinite(value) && Math.abs(value) <= 1);
      sum += value * value;
      peak = Math.max(peak, Math.abs(value));
      if (Math.abs(value - track.samples[index - 1]) > 1.2) jumps += 1;
    }
    const rms = Math.sqrt(sum / track.samples.length);
    assert.ok(rms > 0.01 && rms < 0.5, `rms ${rms}`);
    assert.ok(peak > 0.2, `peak ${peak}`);
    // A wrong block layout or interleave would put a discontinuity at every block edge. The body
    // has one genuine jump: a filter-0, shift-0 block of raw full-scale nibbles at block 51415.
    assert.ok(jumps <= track.samples.length / 1e6, `${jumps} full-scale discontinuities`);
  }
  // The first block of the intro, decoded independently of the native module.
  const raw = await reader.read(intro * SECTOR_BYTES + 0x30, 16 * 64);
  const expected = referenceDecode(raw);
  const again = await decodeVagStream(reader, audio, intro);
  for (let index = 0; index < expected.length; index += 1) {
    assert.equal(Math.round(again.samples[index] * 32768), expected[index]);
  }
  assert.equal(audio.audio_guards_valid(), 1);
});
