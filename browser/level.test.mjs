import assert from 'node:assert/strict';
import { openAsBlob } from 'node:fs';
import { readFile } from 'node:fs/promises';
import { test } from 'node:test';
import { IsoReader } from './iso-reader.mjs';
import { initializeDiscTable } from './disc-loader.mjs';
import { loadLevelGameplay, VELDIN_LEVEL } from './gameplay-loader.mjs';
import { loadPlanetCore, VELDIN_ARCHIVE } from './level-loader.mjs';
import { initializeLevelActors, levelTickStats, tickLevelActors } from './level-actors.mjs';
import { readMobyGeometry } from './moby-geometry.mjs';

const load = name => readFile(new URL(`../build/browser/${name}.wasm`, import.meta.url)).then(WebAssembly.compile);
const [levelModule, runtimeModule, wadModule, planetModule] = await Promise.all(
  ['level', 'runtime', 'wad', 'planet-wad'].map(load));
const isoPath = process.env.RNC_ISO_PATH;

async function instantiate(module) {
  const instance = await WebAssembly.instantiate(module, {});
  instance.exports._initialize?.();
  return instance.exports;
}

function header(classCount, table = 0x80) {
  const bytes = new Uint8Array(0x200);
  const view = new DataView(bytes.buffer);
  view.setUint32(0x18, classCount, true);
  view.setUint32(0x1C, table, true);
  view.setUint32(0x3C, 0x100, true);
  return { bytes, view };
}

test('actor setup refuses out-of-order calls and out-of-range class tables', async () => {
  const level = await instantiate(levelModule);
  level.runtime_level_reset();
  assert.equal(level.runtime_level_spawn_mobys(0x1000), -1);
  assert.equal(level.runtime_level_register_classes(0x40, 0x1000), -2);
  assert.equal(level.runtime_level_register_classes(0x200, level.runtime_level_core_capacity() + 1), -2);
  const tooFar = header(20, 0x1F0);
  new Uint8Array(level.memory.buffer, level.runtime_level_header_address(), 0x200).set(tooFar.bytes);
  assert.equal(level.runtime_level_register_classes(0x200, 0x1000), -3);
  const duplicate = header(2);
  duplicate.view.setUint32(0x84, 7, true);
  duplicate.view.setUint32(0xA4, 7, true);
  new Uint8Array(level.memory.buffer, level.runtime_level_header_address(), 0x200).set(duplicate.bytes);
  assert.equal(level.runtime_level_register_classes(0x200, 0x1000), -4);
  const outside = header(1);
  outside.view.setUint32(0x80, 0x1000 - 8, true);
  new Uint8Array(level.memory.buffer, level.runtime_level_header_address(), 0x200).set(outside.bytes);
  assert.equal(level.runtime_level_register_classes(0x200, 0x1000), -5);
  assert.equal(level.runtime_level_guards_valid(), 1);
});

test('logic-only classes register through the original chain without class data', async () => {
  const level = await instantiate(levelModule);
  level.runtime_level_reset();
  const only = header(1);
  only.view.setUint32(0x84, 834, true);
  new Uint8Array(level.memory.buffer, level.runtime_level_header_address(), 0x200).set(only.bytes);
  assert.equal(level.runtime_level_register_classes(0x200, 0x1000), 1);
  assert.equal(level.runtime_level_class_slot(834), 0);
  assert.equal(level.runtime_level_class_id(0), 834);
  assert.equal(level.runtime_level_class_has_data(0), 0);
  // assign_moby_class_data found class 834 in the L00 table copied to D_001E8B80.
  assert.equal(level.runtime_level_class_has_update(0), 1);
  assert.equal(level.runtime_level_class_slot(835), 0xFF);
});

test('original class registration and moby initialization build the Veldin moby table', {
  skip: isoPath ? false : 'Set RNC_ISO_PATH for the local-disc check',
}, async () => {
  const [api, decoder, planetDecoder, level] = await Promise.all(
    [runtimeModule, wadModule, planetModule, levelModule].map(instantiate));
  api.runtime_init();
  const reader = new IsoReader(await openAsBlob(isoPath));
  await reader.open();
  await initializeDiscTable(api, reader);
  const gameplay = await loadLevelGameplay(api, reader, decoder, VELDIN_LEVEL);
  const planet = await loadPlanetCore(reader, planetDecoder, VELDIN_ARCHIVE);
  const actors = initializeLevelActors(level, planet, gameplay);

  assert.equal(actors.classes, 125);
  assert.equal(actors.placed, 296);
  assert.equal(actors.count + actors.skipped, 296);
  assert.equal(level.runtime_level_guards_valid(), 1);
  // Each placed record keeps its class, uid, and transform through the original init.
  const records = gameplay.mobys.mobys;
  const byUid = new Map(records.map(record => [record.uid, record]));
  for (const moby of actors.mobys) {
    const record = byUid.get(moby.uid);
    assert.ok(record, `uid ${moby.uid} came from a placed record`);
    assert.equal(moby.classId, record.classId);
    assert.deepEqual(moby.position, record.position.map(Math.fround));
    assert.equal(level.runtime_level_class_id(moby.classSlot), moby.classId);
    assert.equal(moby.poolSlot, moby.index);
  }
  // The page draws geometry that already carries the class scale, so the native
  // instance scale (class scale x record scale) divided by it is the record's.
  const geometry = new Map();
  for (const moby of actors.mobys.filter(moby => moby.hasClassData)) {
    if (!geometry.has(moby.classId)) {
      try { geometry.set(moby.classId, readMobyGeometry(planet, moby.classId)); } catch { geometry.set(moby.classId, null); }
    }
    const model = geometry.get(moby.classId);
    if (!model) continue; // the page skips classes its geometry reader cannot decode
    assert.ok(Math.abs(moby.scale / model.scale - byUid.get(moby.uid).scale) < 1e-5, `class ${moby.classId} scale`);
  }
  const clank = actors.mobys.filter(moby => moby.classId === 834);
  assert.equal(clank.length, 1);
  assert.equal(clank[0].hasUpdate, true);
  const hero = actors.mobys.find(moby => moby.classId === 0);
  assert.deepEqual(hero.position.map(value => Math.round(value * 10) / 10), [132.1, 115.5, 31.4]);
  console.log(`Veldin actors: ${actors.count} initialized, ${actors.skipped} filtered, ` +
    `${actors.mobys.filter(moby => moby.hasUpdate).length} with an update, ` +
    `${actors.pendingEntityUpdates} update_entity calls pending, ` +
    `${level.runtime_level_table_value_stores()} table value stored`);
});

test('the original pterodactyl update flies each pterodactyl along its path', {
  skip: isoPath ? false : 'Set RNC_ISO_PATH for the local-disc check',
}, async () => {
  const [api, decoder, planetDecoder, level] = await Promise.all(
    [runtimeModule, wadModule, planetModule, levelModule].map(instantiate));
  api.runtime_init();
  const reader = new IsoReader(await openAsBlob(isoPath));
  await reader.open();
  await initializeDiscTable(api, reader);
  const gameplay = await loadLevelGameplay(api, reader, decoder, VELDIN_LEVEL);
  const planet = await loadPlanetCore(reader, planetDecoder, VELDIN_ARCHIVE);
  const actors = initializeLevelActors(level, planet, gameplay);

  // Every record with a pvar index got its own copied block.
  const records = new Map(gameplay.mobys.mobys.map(record => [record.uid, record]));
  const withPvars = actors.mobys.filter(moby => records.get(moby.uid).pvarIndex >= 0);
  assert.equal(actors.pvarUsers, withPvars.length);
  assert.ok(withPvars.every(moby => moby.pvars !== 0));
  assert.equal(new Set(withPvars.map(moby => moby.pvars)).size, withPvars.length);
  assert.ok(actors.mobys.filter(moby => records.get(moby.uid).pvarIndex < 0).every(moby => moby.pvars === 0));
  assert.equal(actors.paths, 42);

  const memory = () => new DataView(level.memory.buffer);
  const vec = (at) => [0, 4, 8, 12].map(offset => memory().getFloat32(at + offset, true));
  const pterodactyls = actors.mobys.filter(moby => moby.classId === 1564);
  assert.equal(pterodactyls.length, 25);
  const before = pterodactyls.map(moby => {
    const view = memory();
    const path = view.getInt32(moby.pvars + 0x60, true);
    assert.ok(path >= 0 && path < actors.paths, `uid ${moby.uid} path ${path}`);
    assert.equal(level.runtime_level_moby_fixup_pending(moby.index), 0);
    const at = level.runtime_level_path_address(path);
    const count = view.getInt32(at, true);
    const points = Array.from({ length: count }, (_, index) => vec(at + 16 + index * 16));
    return { moby, count, points, height: view.getFloat32(moby.pvars + 0x64, true), start: view.getFloat32(moby.pvars + 0x68, true) };
  });

  // Frame 1: state 0 sets up, then falls through to state 1 at progress start * count.
  const first = tickLevelActors(level, actors.count);
  assert.equal(first.called, 25);
  for (const { moby, count, points, height, start } of before) {
    const now = first.mobys[moby.index];
    const view = memory();
    assert.equal(now.state, 1);
    assert.equal(now.scale, Math.fround(moby.scale * 2));
    const progress = Math.fround(Math.fround(start) * count);
    const index = Math.trunc(progress);
    const fraction = Math.fround(progress - index);
    const [a, b] = [points[index], points[(index + 1) % count]];
    const expected = [0, 1, 2].map(axis => a[axis] + (b[axis] - a[axis]) * fraction);
    expected[2] += height;
    now.position.forEach((value, axis) => assert.ok(Math.abs(value - expected[axis]) < 1e-3,
      `uid ${moby.uid} axis ${axis}: ${value} vs ${expected[axis]}`));
    const increment = view.getFloat32(moby.pvars + 0x74, true);
    const segment = Math.hypot(...[0, 1, 2].map(axis => points[1][axis] - points[0][axis]));
    assert.ok(Math.abs(increment - 6 / 60 / segment) < 1e-6, `uid ${moby.uid} increment`);
    assert.ok(Math.abs(view.getFloat32(moby.pvars + 0x6C, true) - (progress + increment)) < 1e-4);
    if (fraction === 0) {
      // Heading and pitch at a path point: the fast_arc_tan port against atan2.
      const d = [0, 1, 2].map(axis => b[axis] - a[axis]);
      assert.ok(Math.abs(now.rotation[2] - Math.atan2(d[1], d[0])) < 1e-5, `uid ${moby.uid} heading`);
      assert.ok(Math.abs(now.rotation[1] - -Math.atan2(d[2], Math.hypot(d[0], d[1])) * 0.5) < 1e-5,
        `uid ${moby.uid} pitch`);
    }
  }

  // Ten more seconds: no trap, nothing written outside storage, and every
  // pterodactyl stays near its path.
  let frame;
  const framesSeen = new Map();
  for (let tick = 0; tick < 600; tick++) {
    frame = tickLevelActors(level, actors.count);
    for (const moby of frame.mobys) {
      if (!framesSeen.has(moby.index)) framesSeen.set(moby.index, new Set());
      framesSeen.get(moby.index).add(moby.frame);
    }
  }
  assert.equal(level.runtime_level_guards_valid(), 1);

  // Animation: advance_moby_animation runs for every linked moby without flags 0x40. Each one
  // whose sequence has two or more frames and a non-zero speed must show more than one frame.
  const animated = frame.mobys.filter(moby => {
    if (!moby.hasClassData || moby.state & 0x80 || moby.flags & 0x42 || moby.animSpeed === 0) return false;
    const view = memory();
    const pclass = view.getUint32(level.runtime_level_moby_address(moby.index) + 0x24, true);
    const sequence = view.getUint32(pclass + 0x48 + 4 * moby.targetSequence, true);
    return sequence !== 0 && view.getUint8(sequence + 0x10) >= 2;
  });
  assert.ok(animated.length > 0, 'no animated moby on Veldin');
  const frozen = animated.filter(moby => framesSeen.get(moby.index).size < 2);
  assert.deepEqual(frozen.map(moby => `class ${moby.classId} uid ${moby.uid}`), [], 'animated mobys that never changed frame');
  console.log(`Veldin animation: ${animated.length} animated mobys, all changed frame over 600 ticks ` +
    `(classes ${[...new Set(animated.map(moby => moby.classId))].sort((x, y) => x - y).join(', ')})`);
  for (const { moby, points, height } of before) {
    const now = frame.mobys[moby.index];
    const nearest = Math.min(...points.map(point => Math.hypot(...[0, 1, 2].map(axis =>
      point[axis] + (axis === 2 ? height : 0) - now.position[axis]))));
    const longest = Math.max(...points.map((point, index) => Math.hypot(...[0, 1, 2].map(axis =>
      points[(index + 1) % points.length][axis] - point[axis]))));
    assert.ok(nearest <= longest, `uid ${moby.uid} left its path`);
    assert.notDeepEqual(now.position, first.mobys[moby.index].position);
  }
  const stats = levelTickStats(level);
  assert.equal(stats.ticks, 601);
  assert.equal(stats.updates, 25 * 601);
  console.log(`Veldin tick: ${stats.updates} pterodactyl updates over ${stats.ticks} frames; ` +
    `${stats.animationAdvances} animation advances (${stats.animationSoundCalls} sound calls not made); ` +
    `${stats.unportedUpdates} unported class updates and ` +
    `${stats.pendingBoundsRefreshes} bounds refreshes not run; ${actors.relocatedPvarPointers} pvar pointers ` +
    `relocated, ${actors.pendingPvarFixups} pvar fixups pending`);
});
