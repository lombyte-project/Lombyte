import assert from 'node:assert/strict';
import { openAsBlob } from 'node:fs';
import { readFile } from 'node:fs/promises';
import { test } from 'node:test';
import { IsoReader } from './iso-reader.mjs';
import { initializeDiscTable } from './disc-loader.mjs';
import { loadLevelGameplay, readLevelFog, VELDIN_LEVEL } from './gameplay-loader.mjs';
import { loadPlanetCore, VELDIN_ARCHIVE } from './level-loader.mjs';
import { readTieTextures } from './terrain-geometry.mjs';
import { readTieClasses, readTieInstances } from './tie-geometry.mjs';

const isoPath = process.env.RNC_ISO_PATH;

// One class with one packet: a single 4-vertex strip using material record 0.
function syntheticPlanet({ slots = [7, 10, 13, 16], q = 4096, stripCount = 4 } = {}) {
  const header = new Uint8Array(0x100);
  const h = new DataView(header.buffer);
  h.setUint32(8 * 4, 1, true);
  h.setUint32(9 * 4, 0x80, true);
  h.setUint32(0x80, 0x100, true); // class data offset in the core
  h.setInt32(0x84, 4242, true);
  header.fill(0xFF, 0x90, 0xA0);
  header[0x90] = 5; // record 0 -> texture 5
  const core = new Uint8Array(0x400);
  const c = new DataView(core.buffer);
  const data = 0x100;
  c.setUint32(data, 0x60, true); // stream at data + 0x60
  core[data + 0x20] = 1;
  core[data + 0x23] = 1;
  c.setFloat32(data + 0x40, 2, true);
  const stream = data + 0x60;
  c.setUint32(stream, 0x20, true); // packet at stream + 0x20
  core[stream + 4] = 1; // one material
  core[stream + 8] = 4; // vertices at packet + 0x40
  core[stream + 9] = 4; // four 16-byte vertices
  const packet = stream + 0x20;
  core[packet + 0x23] = 1; // one strip
  core[packet + 0x2C] = stripCount;
  core[packet + 0x2E] = 6; // tag offset: slots are 7, 10, 13, 16
  const corners = [[0, 0, 0], [1024, 0, 0], [0, 1024, 0], [1024, 1024, 0]];
  corners.forEach((corner, index) => {
    const at = packet + 0x40 + index * 16;
    corner.forEach((value, axis) => c.setInt16(at + axis * 2, value, true));
    c.setUint16(at + 6, slots[index], true);
    c.setInt16(at + 8, index % 2 ? 4096 : 0, true);
    c.setInt16(at + 10, index > 1 ? 4096 : 0, true);
    c.setInt16(at + 12, q, true);
  });
  return { header, core };
}

test('a strip decodes to triangles in class units with its texture', () => {
  const classes = readTieClasses(syntheticPlanet());
  const tie = classes.get(4242);
  assert.equal(tie.scale, 2);
  assert.equal(tie.triangles, 2);
  assert.deepEqual(tie.packets, { decoded: 1, total: 1 });
  assert.equal(tie.batches.length, 1);
  assert.equal(tie.batches[0].texture, 5);
  assert.deepEqual([...tie.batches[0].positions], [0, 0, 0, 1, 0, 0, 0, 1, 0, /* second, flipped */ 0, 1, 0, 1, 0, 0, 1, 1, 0]);
  assert.deepEqual([...tie.batches[0].uvs.subarray(0, 6)], [0, 0, 1, 0, 0, 1]);
});

test('packets that cannot be verified are refused, or collected when asked', () => {
  assert.throws(() => readTieClasses(syntheticPlanet({ slots: [7, 10, 13, 13] })), /slot/, 'a slot filled twice');
  assert.throws(() => readTieClasses(syntheticPlanet({ slots: [7, 10, 13, 17] })), /slot/, 'a slot off the strip');
  assert.throws(() => readTieClasses(syntheticPlanet({ q: 2048 })), /slot/, 'a vertex that is not a vertex record');
  assert.throws(() => readTieClasses(syntheticPlanet({ stripCount: 2 })), /fewer than three/);
  const failures = [];
  const classes = readTieClasses(syntheticPlanet({ slots: [7, 10, 13, 13] }), failures);
  assert.equal(failures.length, 1);
  assert.deepEqual(classes.get(4242).packets, { decoded: 0, total: 1 });
  assert.equal(classes.get(4242).triangles, 0);
});

test('Veldin scenery, placements, textures and fog from the local disc', {
  skip: isoPath ? false : 'Set RNC_ISO_PATH for the local-disc check',
}, async () => {
  const load = async name => {
    const { instance } = await WebAssembly.instantiate(await readFile(new URL(`../build/browser/${name}.wasm`, import.meta.url)), {});
    instance.exports._initialize?.();
    return instance.exports;
  };
  const [api, decoder, planetDecoder] = await Promise.all(['runtime', 'wad', 'planet-wad'].map(load));
  api.runtime_init();
  const reader = new IsoReader(await openAsBlob(isoPath));
  await reader.open();
  await initializeDiscTable(api, reader);
  const gameplay = await loadLevelGameplay(api, reader, decoder, VELDIN_LEVEL);
  const planet = await loadPlanetCore(reader, planetDecoder, VELDIN_ARCHIVE);

  const failures = [];
  const classes = readTieClasses(planet, failures);
  assert.equal(classes.size, 63);
  let decoded = 0;
  let total = 0;
  let triangles = 0;
  for (const tie of classes.values()) {
    decoded += tie.packets.decoded;
    total += tie.packets.total;
    triangles += tie.triangles;
    for (const batch of tie.batches) {
      assert.equal(batch.positions.length % 9, 0);
      assert.equal(batch.uvs.length, batch.positions.length / 3 * 2);
      for (const value of batch.positions) assert.ok(Number.isFinite(value) && Math.abs(value) <= 32);
    }
  }
  assert.equal(total, 965);
  assert.equal(decoded, 964, 'one packet (class 2215) is known not to verify');
  assert.equal(failures.length, 1);
  assert.match(failures[0], /Tie class 2215/);
  assert.equal(triangles, 40668);

  const instances = readTieInstances(gameplay.bytes);
  assert.equal(instances.length, 1114);
  const images = readTieTextures(planet);
  assert.equal(images.length, 131);
  let placedTriangles = 0;
  for (const instance of instances) {
    const tie = classes.get(instance.classId);
    assert.ok(tie, `class ${instance.classId} has geometry`);
    placedTriangles += tie.triangles;
    for (const batch of tie.batches) assert.ok(images[batch.texture], `texture ${batch.texture} exists`);
    for (let element = 0; element < 15; element += 1) assert.ok(Number.isFinite(instance.matrix[element]));
    // Rows 0-2 are a basis: no degenerate placements.
    assert.ok(Math.hypot(instance.matrix[0], instance.matrix[1], instance.matrix[2]) > 0.01);
  }
  assert.equal(placedTriangles, 396700);

  const fog = readLevelFog(gameplay.bytes);
  assert.deepEqual(fog.background, [71, 66, 58]);
  assert.deepEqual(fog.fog, [47, 26, 15]);
  assert.equal(fog.nearDistance, 0);
  assert.equal(fog.farDistance, 235);
  assert.equal(fog.nearIntensity, 255);
  assert.ok(Math.abs(fog.farIntensity - 53.55) < 0.01);
});
