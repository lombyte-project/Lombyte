import assert from 'node:assert/strict';
import { openAsBlob } from 'node:fs';
import { readFile } from 'node:fs/promises';
import { test } from 'node:test';
import { IsoReader } from './iso-reader.mjs';
import { initializeDiscTable } from './disc-loader.mjs';
import { loadLevelGameplay, VELDIN_LEVEL } from './gameplay-loader.mjs';
import { loadPlanetCore, VELDIN_ARCHIVE } from './level-loader.mjs';
import { initializeLevelActors } from './level-actors.mjs';
import { InterimGame, MAX_HEALTH, OBJECTIVE_REACH } from './interim-game.mjs';

const isoPath = process.env.RNC_ISO_PATH;
const moby = (index, classId, position) => ({ index, classId, position });

test('interim rules: bolts, crates, objective, falls', () => {
  const game = new InterimGame([
    moby(0, 0, [0, 0, 0]), moby(1, 13, [10, 0, 0]), moby(2, 500, [20, 0, 0]),
    moby(3, 511, [30, 0, 0]), moby(4, 834, [100, 0, 0]), moby(5, 1564, [5, 5, 50]),
  ]);
  assert.equal(game.crateTotal, 2);
  assert.deepEqual(game.update([0, 0, 0], true, 1 / 60), []);
  assert.deepEqual(game.update([9, 0, 0], false, 1 / 60), [{ type: 'bolt', index: 1 }]);
  assert.equal(game.bolts, 1);
  assert.deepEqual(game.update([9, 0, 0], false, 1 / 60), [], 'a bolt is collected once');
  assert.deepEqual(game.update([19, 0, 0], false, 1 / 60), [], 'a crate needs a swing');
  assert.deepEqual(game.update([19, 0, 0], true, 1 / 60), [{ type: 'crate', index: 2 }]);
  assert.equal(game.bolts, 6);
  assert.deepEqual(game.update([29, 0, 0], true, 1 / 60), [{ type: 'crate', index: 3 }]);
  assert.equal(game.bolts, 6, 'only class-500 crates pay bolts');
  assert.equal(game.cratesBroken, 2);
  assert.equal(game.state, 'playing');
  assert.deepEqual(game.update([100 - OBJECTIVE_REACH - 0.1, 0, 0], false, 1 / 60), []);
  assert.deepEqual(game.update([100 - OBJECTIVE_REACH + 0.1, 0, 0], false, 1 / 60), [{ type: 'complete' }]);
  assert.equal(game.state, 'complete');
  assert.deepEqual(game.update([10, 0, 0], true, 1 / 60), [], 'nothing happens after completion');

  game.reset();
  assert.equal(game.bolts, 0);
  assert.equal(game.health, MAX_HEALTH);
  assert.deepEqual(game.update([9, 0, 0], false, 1 / 60), [{ type: 'bolt', index: 1 }], 'reset restores pickups');
  for (let fall = 1; fall < MAX_HEALTH; fall += 1) {
    game.fall();
    assert.equal(game.state, 'playing');
  }
  game.fall();
  assert.equal(game.state, 'dead');
  assert.equal(game.health, 0);
  assert.throws(() => new InterimGame([moby(0, 0, [0, 0, 0])]), /objective/);
});

test('Veldin: the crash-site objective and pickups come from the real moby table', {
  skip: isoPath ? false : 'Set RNC_ISO_PATH for the local-disc check',
}, async () => {
  const load = async name => {
    const { instance } = await WebAssembly.instantiate(await readFile(new URL(`../build/browser/${name}.wasm`, import.meta.url)), {});
    instance.exports._initialize?.();
    return instance.exports;
  };
  const [api, decoder, planetDecoder, level] = await Promise.all(
    ['runtime', 'wad', 'planet-wad', 'level'].map(load));
  api.runtime_init();
  const reader = new IsoReader(await openAsBlob(isoPath));
  await reader.open();
  await initializeDiscTable(api, reader);
  const gameplay = await loadLevelGameplay(api, reader, decoder, VELDIN_LEVEL);
  const planet = await loadPlanetCore(reader, planetDecoder, VELDIN_ARCHIVE);
  const actors = initializeLevelActors(level, planet, gameplay);
  const game = new InterimGame(actors.mobys);
  const start = actors.mobys.find(entry => entry.classId === 0).position;
  assert.equal(game.crateTotal, 111);
  assert.equal(game.targets.filter(target => target.classId === 13).length, 29);
  assert.ok(game.objectiveDistance(start) > 100, 'the objective is not at the start');

  // Whether a walking route exists is NOT checked: a straight-line walker fell out of the
  // level near (135, 145), so reaching the objective has only been shown by distance, not by play.
});
