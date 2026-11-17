// Stages a planet's core header, decoded core, and decoded gameplay into
// level.wasm, then runs the original class registration and placed-moby
// initialization (browser/level.c). The moby table it returns is read from the
// 0x100-byte instances the original init_moby_instance filled.
const MOBY_BYTES = 0x100;
const END_STATE = 0xFF;

const ERRORS = new Map([
  [-1, 'actor setup called out of order'], [-2, 'level data exceeds native capacity'],
  [-3, 'invalid moby class table'], [-4, 'invalid or duplicate moby class id'],
  [-5, 'moby class data is outside the core'], [-6, 'moby class slots overflowed'],
  [-7, 'placed moby table is outside the gameplay data'], [-8, 'invalid placed moby record'],
  [-9, 'too many placed mobys'], [-10, 'placed moby needs an unported original function'],
  [-11, 'level storage guard was overwritten'], [-12, 'invalid moby pvar table or block'],
  [-13, 'invalid path table'],
]);

function stage(level, address, capacity, bytes, what) {
  if (bytes.length > capacity) throw new Error(`${what} (${bytes.length} bytes) exceeds native capacity ${capacity}.`);
  new Uint8Array(level.memory.buffer, address, bytes.length).set(bytes);
}

function unportedName(level) {
  const name = level.runtime_level_unported_name();
  if (!name) return '';
  const bytes = new Uint8Array(level.memory.buffer, name);
  return new TextDecoder().decode(bytes.subarray(0, bytes.indexOf(0)));
}

// level_unported() traps so no original code runs past a missing function;
// the name it recorded survives in linear memory.
function call(level, step, run) {
  try {
    return run();
  } catch (error) {
    const name = error instanceof WebAssembly.RuntimeError && unportedName(level);
    if (name) throw new Error(`Native ${step} reached unported original function ${name}.`, { cause: error });
    throw error;
  }
}

function fail(level, step, result) {
  const name = unportedName(level);
  let detail = ERRORS.get(result) ?? `error ${result}`;
  if (name) detail += `: ${name}`;
  throw new Error(`Native ${step} failed (${detail}).`);
}

export function initializeLevelActors(level, planet, gameplay) {
  level.runtime_level_reset();
  stage(level, level.runtime_level_header_address(), level.runtime_level_header_capacity(), planet.header, 'Planet header');
  stage(level, level.runtime_level_core_address(), level.runtime_level_core_capacity(), planet.core, 'Planet core');
  stage(level, level.runtime_level_gameplay_address(), level.runtime_level_gameplay_capacity(), gameplay.bytes, 'Gameplay');
  const classes = call(level, 'class registration',
    () => level.runtime_level_register_classes(planet.header.length, planet.core.length));
  if (classes < 0) fail(level, 'class registration', classes);
  const count = call(level, 'placed moby initialization', () => level.runtime_level_spawn_mobys(gameplay.bytes.length));
  if (count < 0) fail(level, 'placed moby initialization', count);
  const pvars = call(level, 'pvar and path setup', () => level.runtime_level_attach_pvars_and_paths(gameplay.bytes.length));
  if (pvars < 0) fail(level, 'pvar and path setup', pvars);
  if (level.runtime_level_guards_valid() !== 1) throw new Error('Level storage guard was overwritten.');
  return {
    classes, count, mobys: readMobyTable(level, count),
    placed: level.runtime_level_placed_count(), skipped: level.runtime_level_skipped_count(),
    extraSlots: level.runtime_level_extra_slots(),
    pendingEntityUpdates: level.runtime_level_pending_entity_updates(),
    pvarUsers: pvars, paths: level.runtime_level_path_count(),
    pendingPvarFixups: level.runtime_level_pending_pvar_fixups(),
    relocatedPvarPointers: level.runtime_level_relocated_pvar_pointers(),
  };
}

// One frame of the native moby update loop (runtime_level_tick in
// browser/level.c). Only ported class updates run; the rest are counted.
export function tickLevelActors(level, count) {
  const called = call(level, 'moby update', () => level.runtime_level_tick());
  if (called < 0) fail(level, 'moby update', called);
  return { called, mobys: readMobyTable(level, count) };
}

export function levelTickStats(level) {
  return {
    ticks: level.runtime_level_tick_count(), updates: level.runtime_level_ticked_updates(),
    unportedUpdates: level.runtime_level_unported_update_calls(),
    animationAdvances: level.runtime_level_animation_advances(),
    animationSoundCalls: level.runtime_level_animation_sound_calls(),
    pendingBoundsRefreshes: level.runtime_level_pending_bounds_refreshes(),
  };
}

// Field offsets follow struct Moby (include/rnc/mobys/moby.h) and the
// MobyInst view in load_level_gameplay.c.
export function readMobyTable(level, count) {
  if (level.runtime_level_moby_stride() !== MOBY_BYTES) throw new Error('Unexpected native moby size.');
  const base = level.runtime_level_moby_address(0);
  const view = new DataView(level.memory.buffer, base, (count + 1) * MOBY_BYTES);
  if (view.getUint8(count * MOBY_BYTES + 0x20) !== END_STATE) throw new Error('Moby table has no end marker.');
  const mobys = [];
  for (let index = 0; index < count; index++) {
    const at = index * MOBY_BYTES;
    const f32 = offset => view.getFloat32(at + offset, true);
    mobys.push({
      index, classId: view.getInt16(at + 0xA6, true), classSlot: view.getUint8(at + 0x22),
      hasClassData: view.getUint32(at + 0x24, true) !== 0, hasUpdate: view.getUint32(at + 0x74, true) !== 0,
      state: view.getUint8(at + 0x20), group: view.getUint8(at + 0x21), flags: view.getUint16(at + 0x34, true),
      scale: f32(0x2C), position: [f32(0x10), f32(0x14), f32(0x18)], rotation: [f32(0x40), f32(0x44), f32(0x48)],
      uid: view.getInt16(at + 0xB2, true), mission: view.getUint8(at + 0xB0), poolSlot: view.getInt32(at + 0xAC, true),
      pvars: view.getUint32(at + 0x78, true), animFlags: view.getUint8(at + 0x70),
      // Animation state (struct Moby 0x50-0x6C): what advance_moby_animation reads and writes.
      frame: view.getUint8(at + 0x50), nextFrame: view.getUint8(at + 0x51), sequence: view.getUint8(at + 0x52),
      targetSequence: view.getUint8(at + 0x53), animTime: f32(0x54), animSpeed: f32(0x58),
      frameData: view.getUint32(at + 0x68, true), nextFrameData: view.getUint32(at + 0x6C, true),
    });
  }
  return mobys;
}
