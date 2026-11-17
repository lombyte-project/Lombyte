import { readMobyClass } from './moby-geometry.mjs';

export function sampleHeroAnimationFrame(frames, time) {
  let start = 0;
  for (let index = 0; index < frames.length - 1; index++) {
    const duration = 1 / (60 * frames[index].rate);
    if (time < start + duration) {
      const blend = Math.max(0, (time - start) / duration);
      return { sourceFrame: frames[index].sourceFrame * (1 - blend) + frames[index + 1].sourceFrame * blend,
        rate: frames[index].rate };
    }
    start += duration;
  }
  return { sourceFrame: frames.at(-1).sourceFrame, rate: frames.at(-1).rate };
}

export class HeroSwingState {
  sequence;
  queued = false;
  finished = false;

  request(grounded, crouched = false) {
    if (this.sequence === 43 || this.sequence === 26) return false;
    if (!grounded) {
      this.sequence = 43;
      this.queued = false;
      this.finished = false;
      return true;
    }
    if (crouched) {
      this.sequence = 26;
      this.queued = false;
      return true;
    }
    if (this.sequence === undefined) this.sequence = 23;
    else this.queued = true;
    return true;
  }

  update(grounded) {
    if (this.sequence === 43) {
      if (grounded && this.finished) this.reset();
    } else if (!grounded) this.reset();
  }

  finish(sequence, grounded) {
    if (sequence !== this.sequence) return;
    if (sequence === 43 && !grounded) {
      this.finished = true;
      return;
    }
    if (this.queued && grounded) {
      this.sequence = 23 + (this.sequence - 23 + 1) % 3;
      this.queued = false;
    } else this.reset();
  }

  reset() {
    this.sequence = undefined;
    this.queued = false;
    this.finished = false;
  }
}

export function readHeroSequences(planet) {
  const model = readMobyClass(planet);
  const { base, view, range } = model;
  const header = new DataView(planet.header.buffer, planet.header.byteOffset, planet.header.byteLength);
  if (header.byteLength < 124) throw new Error('Missing hero animation table.');
  const table = header.getUint32(120, true), count = view.getUint8(base + 12);
  if (table < 124 || table > header.byteLength || count * 4 > header.byteLength - table) throw new Error('Invalid hero animation table.');
  const sequences = Array.from({ length: count }, (_, id) => {
    const offset = header.getUint32(table + id * 4, true);
    if (!offset) return null;
    range(offset, 28);
    const frames = view.getUint8(offset + 16), triggers = view.getUint8(offset + 18);
    if (!frames) throw new Error('Hero animation has no frames.');
    range(offset + 28, (frames + triggers) * 4);
    const entries = Array.from({ length: frames }, (_, frame) => {
      const packed = view.getUint32(offset + 28 + frame * 4, true);
      return { offset: range(offset + (packed & 0x0FFFFFFF), 4), compressed: Boolean(packed >>> 28) };
    });
    return { id, offset, frames: entries, metadata: offset + 28 + (frames + triggers) * 4 };
  });
  return { ...model, sequences };
}

// The skeleton of any moby class: joint parents and bind translations (class header + 24).
function readRig(model, what) {
  const { base, view, range, jointCount } = model;
  const translations = range(base + view.getUint32(base + 24, true), jointCount * 16);
  const unit = model.scale / 1024;
  const parents = [], bindPositions = new Float32Array(jointCount * 3);
  for (let joint = 0; joint < jointCount; joint++) {
    const record = translations + joint * 16;
    const parent = view.getUint16(record + 12, true) / 64;
    if (!Number.isInteger(parent) || joint > 0 && parent >= joint) throw new Error(`Invalid ${what} joint parent.`);
    parents.push(joint === 0 ? -1 : parent);
    for (let axis = 0; axis < 3; axis++) {
      const position = view.getFloat32(record + axis * 4, true) * unit;
      if (!Number.isFinite(position) || Math.abs(position) > 10) throw new Error(`Invalid ${what} bind translation.`);
      bindPositions[joint * 3 + axis] = position;
    }
  }
  return { unit, parents, bindPositions };
}

// One uncompressed pose frame at a planet-core offset: joint quaternions, then scale and
// translation records (the layout the hero's sequences use; every moby class shares it).
function decodePoseFrame(model, rig, frameOffset, what, staticAllowed = false) {
  const { view, range, jointCount } = model;
  const offset = range(frameOffset, 16);
  const bytes = view.getUint16(offset + 6, true) * 16;
  const rotationBytes = view.getUint16(offset + 8, true);
  const scaleCount = view.getUint16(offset + 10, true);
  const translationCount = view.getUint16(offset + 14, true);
  if (rotationBytes !== jointCount * 8 || scaleCount > jointCount || translationCount > jointCount ||
      rotationBytes + (scaleCount + translationCount) * 8 > bytes) throw new Error(`Invalid ${what} pose layout.`);
  range(offset + 16, bytes);
  const rotations = new Float32Array(jointCount * 4);
  const positions = rig.bindPositions.slice(), scales = new Float32Array(jointCount * 3).fill(1);
  for (let joint = 0; joint < jointCount; joint++) {
    let length = 0;
    for (let axis = 0; axis < 4; axis++) {
      const value = view.getInt16(offset + 16 + joint * 8 + axis * 2, true) / 32767;
      rotations[joint * 4 + axis] = axis < 3 ? -value : value;
      length += value * value;
    }
    if (length < 0.9 || length > 1.1) throw new Error(`Invalid ${what} pose quaternion.`);
    for (let axis = 0; axis < 4; axis++) rotations[joint * 4 + axis] /= Math.sqrt(length);
  }
  for (let record = 0; record < scaleCount + translationCount; record++) {
    const start = offset + 16 + rotationBytes + record * 8;
    const flags = view.getUint16(start + 6, true);
    const joint = flags & 0x7FFF;
    const scale = record < scaleCount;
    if (joint >= jointCount || Boolean(flags & 0x8000) !== scale) throw new Error(`Invalid ${what} pose transform index.`);
    for (let axis = 0; axis < 3; axis++) {
      (scale ? scales : positions)[joint * 3 + axis] = view.getInt16(start + axis * 2, true) * (scale ? 1 / 4096 : rig.unit);
    }
  }
  const rate = view.getFloat32(offset, true);
  // A rate of 0 marks a frame that never advances (a one-frame sequence); advance_moby_animation
  // returns early on it. Placed mobys may rest on one; the hero's played clips may not.
  if (!Number.isFinite(rate) || rate < 0 || rate === 0 && !staticAllowed || rate > 4) {
    throw new Error(`Invalid ${what} pose frame rate.`);
  }
  const sourceFrame = view.getInt16(offset + 4, true) / 16;
  return { rotations, positions, scales, rate, sourceFrame };
}

// A placed moby class's skeleton and a pose reader. The frame to show is chosen by the original
// code: advance_moby_animation leaves the current and next frame addresses at moby + 0x68 and
// + 0x6C (planet core offsets once the core's load address is subtracted) and the position
// between them in anim_time (+ 0x54). Compressed frames are not decoded; poseAt returns null.
export function readMobyAnimationRig(planet, classId) {
  const model = readMobyClass(planet, classId);
  if (!model.jointCount) return null;
  const rig = readRig(model, `class ${classId}`);
  const poses = new Map();
  function poseAt(coreOffset) {
    if (!poses.has(coreOffset)) {
      let pose = null;
      if (coreOffset >= 0 && coreOffset < model.view.byteLength) {
        try { pose = decodePoseFrame(model, rig, coreOffset, `class ${classId}`, true); } catch { pose = null; }
      }
      poses.set(coreOffset, pose);
    }
    return poses.get(coreOffset);
  }
  return { jointCount: model.jointCount, parents: rig.parents, bindPositions: rig.bindPositions, poseAt };
}

export function readHeroAnimationRig(planet) {
  const model = readHeroSequences(planet);
  const { jointCount } = model;
  const rig = readRig(model, 'hero');
  const { parents, bindPositions } = rig;
  function readFrame(entry) {
    if (entry.compressed) throw new Error('Compressed hero animation frames are not supported.');
    return decodePoseFrame(model, rig, entry.offset, 'hero');
  }
  const clips = new Map();
  for (const id of [0, 4, 7, 13, 22, 23, 24, 25, 26, 43]) {
    const sequence = model.sequences[id];
    if (!sequence) throw new Error(`Hero animation ${id} is missing.`);
    clips.set(id, sequence.frames.map(readFrame));
  }
  return { jointCount, parents, bindPositions, clips };
}