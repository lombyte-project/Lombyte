import * as THREE from 'three';
import { OrbitControls } from './node_modules/three/examples/jsm/controls/OrbitControls.js';
import { IsoReader } from './iso-reader.mjs';
import { initializeDiscTable } from './disc-loader.mjs';
import { readSkyGeometry, readSkyTextures } from './sky-geometry.mjs';
import { loadPlanetCore, VELDIN_ARCHIVE, initializePlanetCollision } from './level-loader.mjs';
import { readTerrainGeometry, readTerrainTextures, readMobyTextures, readShrubTextures, readTieTextures } from './terrain-geometry.mjs';
import { readShrubClasses, readShrubInstances } from './shrub-geometry.mjs';
import { readTieClasses, readTieInstances } from './tie-geometry.mjs';
import { readMobyGeometry } from './moby-geometry.mjs';
import { loadLevelGameplay, readLevelFog, VELDIN_LEVEL } from './gameplay-loader.mjs';
import { initializeLevelActors, tickLevelActors } from './level-actors.mjs';
import { loadLevelMusic, MusicPlayer } from './music.mjs';
import { InterimGame, MAX_HEALTH, OBJECTIVE_TEXT } from './interim-game.mjs';
import { HeroSwingState, readHeroAnimationRig, sampleHeroAnimationFrame } from './moby-animation.mjs';

const canvas = document.querySelector('canvas');
const status = document.querySelector('#status');
const disc = document.querySelector('#disc');
const scene = new THREE.Scene();
const skyScene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(70, 1, 0.001, 1000);
camera.up.set(0, 0, 1);
camera.position.set(0, -0.01, 0);
const renderer = new THREE.WebGLRenderer({ canvas, antialias: true, preserveDrawingBuffer: true });
renderer.setPixelRatio(Math.min(devicePixelRatio, 2));
renderer.setClearColor(0x070a0c);
renderer.autoClear = false;
const controls = new OrbitControls(camera, canvas);
controls.enablePan = false;
controls.enableZoom = false;
controls.enableDamping = true;
controls.rotateSpeed = 0.45;
const skyGroup = new THREE.Group();
const terrainGroup = new THREE.Group();
const characterGroup = new THREE.Group();
const actorGroup = new THREE.Group();
const tieGroup = new THREE.Group();
skyScene.add(skyGroup);
scene.add(terrainGroup, tieGroup, actorGroup, characterGroup);
let api;
let decoder;
let planetDecoder;
let collision;
let level;
let audio;
let musicLabel = '';
let musicTracks = null;
let interim = null; // InterimGame, once a level is loaded
let play = 'menu'; // menu | playing | paused | ended: the interim page flow, not game state
let currentJob;
let textures = [];
let terrainRadius = 0;
let heroActive = false;
let previousTime;
let tickRemainder = 0;
let jumpQueued = false;
let swingRequested = false;
let heroAnimation;
const music = new MusicPlayer();
const LEVEL_START_TRACK = 0; // run_level_main_loop starts music_stream_state.primary.track, which it sets to 0
const keys = new Set();
const touchKeys = new Map();
const spawn = new THREE.Vector3();
const movement = new THREE.Vector3();
const forward = new THREE.Vector3();
const followDelta = new THREE.Vector3();
const controlKeys = new Set(['KeyW', 'KeyA', 'KeyS', 'KeyD', 'ArrowUp', 'ArrowLeft',
  'ArrowDown', 'ArrowRight', 'Space', 'ShiftLeft', 'ShiftRight', 'KeyR', 'KeyF',
  'KeyC', 'ControlLeft', 'ControlRight']);

function releaseInput() {
  keys.clear();
  touchKeys.clear();
  heroAnimation?.swing.reset();
  collision?.runtime_hero_control_slam(0);
  collision?.runtime_hero_control_crouch(0);
  collision?.runtime_hero_control_jump_release();
  jumpQueued = false;
  swingRequested = false;
  tickRemainder = 0;
  previousTime = undefined;
}

// Browsers keep audio suspended until a user gesture; any key or pointer press resumes it.
addEventListener('keydown', event => {
  music.resume();
  if (event.code === 'KeyM' && !event.repeat &&
      !(event.target instanceof Element && event.target.closest('input, select, textarea'))) {
    toggleMusic();
  }
  if (event.code === 'Escape' && !event.repeat) {
    if (play === 'playing') pauseGame();
    else if (play === 'paused') resumeGame();
  }
});
addEventListener('pointerdown', () => music.resume());
addEventListener('keydown', event => {
  if (!heroActive || !controlKeys.has(event.code) || event.target !== canvas &&
      event.target instanceof Element && event.target.closest('input, button, select, textarea')) return;
  event.preventDefault();
  if (event.code === 'Space' && !event.repeat) jumpQueued = true;
  if (event.code === 'KeyF' && !event.repeat) requestSwing();
  if (event.code === 'KeyR' && !event.repeat) resetHero();
  keys.add(event.code);
});
addEventListener('keyup', event => {
  keys.delete(event.code);
  if (event.code === 'Space' && !pressed('Space')) collision?.runtime_hero_control_jump_release();
  if (heroActive && controlKeys.has(event.code)) event.preventDefault();
});
addEventListener('blur', releaseInput);
document.addEventListener('visibilitychange', releaseInput);
canvas.tabIndex = 0;
canvas.addEventListener('pointerdown', () => canvas.focus({ preventScroll: true }));
for (const button of document.querySelectorAll('[data-control]')) {
  button.addEventListener('pointerdown', event => {
    if (!heroActive) return;
    event.preventDefault();
    canvas.focus({ preventScroll: true });
    button.setPointerCapture(event.pointerId);
    const code = button.dataset.control;
    touchKeys.set(event.pointerId, code);
    if (code === 'Space') jumpQueued = true;
    if (code === 'KeyF') requestSwing();
    if (code === 'KeyR') resetHero();
  });
  for (const name of ['pointerup', 'pointercancel', 'lostpointercapture']) {
    button.addEventListener(name, event => {
      const code = touchKeys.get(event.pointerId);
      touchKeys.delete(event.pointerId);
      if (code === 'Space' && !pressed('Space')) collision?.runtime_hero_control_jump_release();
    });
  }
}

let shownAudioState = '';
setInterval(() => {
  const { state, level: output, time } = music.probe();
  document.body.dataset.audioState = state;
  document.body.dataset.audioLevel = output.toFixed(4);
  document.body.dataset.audioTime = time.toFixed(2);
  // The browser decides when audio may start; keep the status line in step with it.
  if (state !== shownAudioState && document.body.dataset.state === 'rendering' && music.playing) {
    shownAudioState = state;
    if (play === 'playing') status.textContent = `Veldin / Ratchet${musicStatus()}`;
  }
}, 250);

function musicStatus() {
  if (!music.playing) return musicLabel;
  if (music.probe().state !== 'running') return ' / click or press a key to start the music';
  return music.muted ? ' / music muted (M)' : ' / music playing (M mutes)';
}

// ---- Interim page flow: menu, HUD, pause, level result (see interim-game.mjs) -------------------
const ui = Object.fromEntries(['menu-main', 'menu-controls-panel', 'menu-pause', 'menu-end', 'menu-hint', 'menu-disc',
  'menu-start', 'menu-controls', 'menu-music', 'pause-objective', 'pause-resume', 'pause-restart', 'pause-controls',
  'pause-quit', 'end-title', 'end-text', 'end-again', 'end-quit', 'end-note', 'hud-objective', 'hud-distance',
  'hud-bolts', 'hud-health', 'toast', 'toast-text'].map(id => [id, document.getElementById(id)]));
let controlsReturn = 'menu-main';
let toastTimer;

function openPanel(id) {
  for (const panel of document.querySelectorAll('.panel')) panel.classList.toggle('open', panel.id === id);
  if (id) document.querySelector(`#${id} button:not(:disabled)`)?.focus({ preventScroll: true });
}

function setPlay(next) {
  play = next;
  document.body.dataset.play = next;
  if (next !== 'playing') releaseInput();
}

function setMenuReady(ready) {
  ui['menu-start'].disabled = !ready;
  ui['menu-hint'].textContent = ready ? 'Veldin is loaded.'
    : 'Choose your USA disc image to begin. It stays on this computer.';
}

function showToast(text) {
  ui['toast-text'].textContent = text;
  ui.toast.classList.add('open');
  clearTimeout(toastTimer);
  toastTimer = setTimeout(() => ui.toast.classList.remove('open'), 2200);
}

function toggleMusic() {
  music.setMuted(!music.muted);
  ui['menu-music'].textContent = `Music: ${music.muted ? 'off' : 'on'}`;
  if (play === 'playing') status.textContent = `Veldin / Ratchet${musicStatus()}`;
}

function restoreActors() {
  for (const actor of liveActors?.groups ?? []) actor.visible = true;
}

function startLevel() {
  if (!interim) return;
  interim.reset();
  restoreActors();
  resetHero();
  openPanel('');
  setPlay('playing');
  if (musicTracks) music.play(musicTracks);
  music.resume();
  status.textContent = `Veldin / Ratchet${musicStatus()}`;
  showToast(`Objective: ${OBJECTIVE_TEXT}`);
  updateHud();
  canvas.focus({ preventScroll: true });
}

function pauseGame() {
  setPlay('paused');
  ui['pause-objective'].textContent = `Objective: ${OBJECTIVE_TEXT}`;
  openPanel('menu-pause');
}

function resumeGame() {
  openPanel('');
  setPlay('playing');
  music.resume();
  canvas.focus({ preventScroll: true });
}

function quitToMenu() {
  music.stop();
  setPlay('menu');
  openPanel('menu-main');
  status.textContent = interim ? 'Veldin loaded. Choose New game.' : 'No disc selected';
}

function endLevel() {
  const won = interim.state === 'complete';
  setPlay('ended');
  ui['end-title'].textContent = won ? 'Crash site reached' : 'Ratchet is down';
  ui['end-text'].textContent = won
    ? `Time ${Math.floor(interim.seconds / 60)}:${String(Math.floor(interim.seconds % 60)).padStart(2, '0')} / ` +
      `${interim.bolts} bolts / ${interim.cratesBroken} of ${interim.crateTotal} crates`
    : 'You ran out of health.';
  ui['end-note'].textContent = won
    ? 'In the game, Clank\'s cutscene and the flight to Novalis follow here. Neither is ported yet.'
    : '';
  openPanel('menu-end');
}

function applyInterimEvents(events) {
  for (const event of events) {
    if (event.type === 'complete') { endLevel(); continue; }
    const actor = liveActors?.groups.find(group => group.userData.index === event.index);
    if (actor) actor.visible = false;
    if (event.type === 'crate') showToast('Crate smashed');
  }
}

function updateHud() {
  if (!interim || play !== 'playing') return;
  ui['hud-objective'].textContent = OBJECTIVE_TEXT;
  ui['hud-distance'].textContent = `${Math.round(interim.objectiveDistance(characterGroup.position.toArray()))} m away`;
  ui['hud-bolts'].textContent = String(interim.bolts);
  if (ui['hud-health'].childElementCount !== MAX_HEALTH) {
    ui['hud-health'].replaceChildren(...Array.from({ length: MAX_HEALTH }, () => document.createElement('span')));
  }
  [...ui['hud-health'].children].forEach((orb, index) => orb.classList.toggle('on', index < interim.health));
}

ui['menu-disc'].addEventListener('click', () => disc.click());
ui['menu-start'].addEventListener('click', startLevel);
ui['menu-music'].addEventListener('click', toggleMusic);
ui['menu-controls'].addEventListener('click', () => { controlsReturn = 'menu-main'; openPanel('menu-controls-panel'); });
ui['pause-controls'].addEventListener('click', () => { controlsReturn = 'menu-pause'; openPanel('menu-controls-panel'); });
document.querySelector('#menu-controls-panel [data-close]').addEventListener('click', () => openPanel(controlsReturn));
ui['pause-resume'].addEventListener('click', resumeGame);
ui['pause-restart'].addEventListener('click', startLevel);
ui['pause-quit'].addEventListener('click', quitToMenu);
ui['end-again'].addEventListener('click', startLevel);
ui['end-quit'].addEventListener('click', quitToMenu);
document.body.dataset.play = play;
ui['menu-disc'].disabled = true; // until the native modules are ready

function pressed(code) { return keys.has(code) || [...touchKeys.values()].includes(code); }

function requestSwing() {
  swingRequested = true;
}

function resetHero() {
  releaseInput();
  if (heroAnimation) {
    heroAnimation.mixer.stopAllAction();
    heroAnimation.sequence = undefined;
  }
  const result = collision.runtime_hero_control_reset(spawn.x, spawn.y, spawn.z);
  if (result !== 0) throw new Error(`Native hero reset failed (${result}).`);
  syncHero();
}

function syncHero() {
  movement.set(...[0, 1, 2].map(axis => collision.runtime_hero_collision_position(axis)));
  followDelta.copy(movement).sub(characterGroup.position);
  characterGroup.position.copy(movement);
  characterGroup.rotation.z = collision.runtime_hero_locomotion_yaw();
  camera.position.add(followDelta);
  controls.target.add(followDelta);
}

export function readHeroState() {
  return { active: heroActive, position: characterGroup.position.toArray(),
    yaw: characterGroup.rotation.z, grounded: Boolean(collision?.runtime_hero_control_grounded()),
    crouched: Boolean(collision?.runtime_hero_control_crouched()),
    jumps: collision?.runtime_hero_control_jump_count() ?? 0,
    verticalSpeed: collision?.runtime_hero_locomotion_velocity(2) ?? 0,
    animation: heroAnimation ? { sequence: heroAnimation.sequence,
      time: heroAnimation.actions.get(heroAnimation.sequence)?.time,
      sourceFrame: heroAnimation.frames.has(heroAnimation.sequence) ? sampleHeroAnimationFrame(
        heroAnimation.frames.get(heroAnimation.sequence), heroAnimation.actions.get(heroAnimation.sequence).time).sourceFrame : null,
      swing: heroAnimation.swing.sequence ?? null, queued: heroAnimation.swing.queued,
      pose: heroAnimation.bones[8].quaternion.toArray() } : null,
    camera: camera.position.toArray(), target: controls.target.toArray() };
}

function updateHero(time) {
  if (previousTime === undefined) previousTime = time;
  const elapsed = Math.min(Math.max((time - previousTime) / 1000, 0), 0.1);
  previousTime = time;
  if (!heroActive || document.hidden || play !== 'playing') { tickRemainder = 0; return; }
  tickRemainder += elapsed;
  camera.getWorldDirection(forward);
  forward.z = 0;
  if (forward.lengthSq() > 0.0001) forward.normalize();
  else forward.set(-1, 0, 0);
  const horizontal = Number(pressed('KeyD') || pressed('ArrowRight')) -
    Number(pressed('KeyA') || pressed('ArrowLeft'));
  const vertical = Number(pressed('KeyW') || pressed('ArrowUp')) -
    Number(pressed('KeyS') || pressed('ArrowDown'));
  const inputX = forward.x * vertical + forward.y * horizontal;
  const inputY = forward.y * vertical - forward.x * horizontal;
  const inputLength = Math.max(Math.hypot(inputX, inputY), 1);
  while (tickRemainder >= 1 / 60) {
    const jump = jumpQueued || pressed('Space');
    const crouch = pressed('KeyC') || pressed('ControlLeft') || pressed('ControlRight');
    const stance = collision.runtime_hero_control_crouch(Number(crouch && heroAnimation?.swing.sequence !== 43));
    if (stance !== 0) throw new Error(`Native crouch input failed (${stance}).`);
    if (jump && heroAnimation?.swing.sequence !== 43) heroAnimation?.swing.reset();
    heroAnimation?.swing.update(Boolean(collision.runtime_hero_control_grounded()));
    const movementScale = heroAnimation?.swing.sequence === undefined ? 1 : 0;
    const result = collision.runtime_hero_control_step(inputX / inputLength * movementScale, inputY / inputLength * movementScale,
      Number(jump), Number(keys.has('ShiftLeft') || keys.has('ShiftRight')));
    jumpQueued = false;
    if (result !== 0) throw new Error(`Native hero step failed (${result}).`);
    const grounded = Boolean(collision.runtime_hero_control_grounded());
    heroAnimation?.swing.update(grounded);
    if (swingRequested) {
      if (heroAnimation?.swing.request(grounded, Boolean(collision.runtime_hero_control_crouched())) && !grounded) {
        const slam = collision.runtime_hero_control_slam(1);
        if (slam !== 0) throw new Error(`Native hero slam failed (${slam}).`);
      }
      swingRequested = false;
    }
    tickRemainder -= 1 / 60;
    if (interim) {
      const hero = [0, 1, 2].map(axis => collision.runtime_hero_collision_position(axis));
      applyInterimEvents(interim.update(hero, heroAnimation?.swing.sequence !== undefined, 1 / 60));
    }
  }
  syncHero();
  updateHeroAnimation(elapsed);
  if (characterGroup.position.z < spawn.z - 40) {
    interim?.fall();
    resetHero();
    if (interim?.state === 'dead') endLevel();
    else if (interim) showToast('You fell. Health lost.');
  }
  updateHud();
}

function updateHeroAnimation(elapsed) {
  if (!heroAnimation) return;
  const speed = collision.runtime_hero_locomotion_speed() * 60;
  const grounded = collision.runtime_hero_control_grounded();
  const crouched = collision.runtime_hero_control_crouched();
  heroAnimation.swing.update(Boolean(grounded));
  let sequence;
  if (heroAnimation.swing.sequence !== undefined) sequence = heroAnimation.swing.sequence;
  else if (crouched) sequence = 13;
  else if (grounded) {
    if (speed > 0.1) sequence = 4;
    else sequence = 0;
  } else {
    if (collision.runtime_hero_control_jump_count() === 2) sequence = 22;
    else sequence = 7;
  }
  const { mixer, actions } = heroAnimation;
  const action = actions.get(sequence);
  if (heroAnimation.sequence !== sequence) {
    actions.get(heroAnimation.sequence)?.fadeOut(0.12);
    action.reset().fadeIn(0.12).play();
    heroAnimation.sequence = sequence;
  }
  action.timeScale = sequence === 4 ? Math.max(0.35, speed / 5.7) : 1;
  if (sequence === 7 || sequence === 22) {
    const pose = sampleHeroAnimationFrame(heroAnimation.frames.get(sequence), action.time);
    const rate = collision.runtime_hero_jump_animation_speed(pose.sourceFrame, pose.rate);
    if (!Number.isFinite(rate) || rate <= 0) throw new Error('Invalid native jump animation speed.');
    action.timeScale = rate;
  }
  if (sequence === 43) {
    const pose = sampleHeroAnimationFrame(heroAnimation.frames.get(sequence), action.time);
    const rate = collision.runtime_hero_slam_animation_speed(pose.sourceFrame, pose.rate);
    if (!Number.isFinite(rate) || rate <= 0) throw new Error('Invalid native slam animation speed.');
    action.timeScale = rate;
  }
  mixer.update(elapsed);
}

function createHeroAnimation(rig) {
  const bones = Array.from({ length: rig.jointCount }, (_, joint) => {
    const bone = new THREE.Bone();
    bone.name = `hero_joint_${joint}`;
    bone.position.fromArray(rig.bindPositions, joint * 3);
    return bone;
  });
  for (let joint = 1; joint < bones.length; joint++) bones[rig.parents[joint]].add(bones[joint]);
  bones[0].updateMatrixWorld(true);
  const skeleton = new THREE.Skeleton(bones);
  skeleton.calculateInverses();
  const mixer = new THREE.AnimationMixer(characterGroup);
  const actions = new Map();
  for (const [id, frames] of rig.clips) {
    const tracks = [], times = [0];
    for (let frame = 1; frame < frames.length; frame++) times.push(times[frame - 1] + 1 / (60 * frames[frame - 1].rate));
    if (id === 0 || id === 4 || id === 13) times.push(times.at(-1) + 1 / (60 * frames.at(-1).rate));
    for (let joint = 0; joint < bones.length; joint++) {
      for (const [property, field, width, Track] of [
        ['position', 'positions', 3, THREE.VectorKeyframeTrack],
        ['quaternion', 'rotations', 4, THREE.QuaternionKeyframeTrack],
        ['scale', 'scales', 3, THREE.VectorKeyframeTrack],
      ]) {
        const values = [];
        for (let frame = 0; frame < times.length; frame++) {
          values.push(...frames[frame % frames.length][field].subarray(joint * width, (joint + 1) * width));
        }
        tracks.push(new Track(`${bones[joint].name}.${property}`, times, values));
      }
    }
    const clip = new THREE.AnimationClip(`hero_${id}`, -1, tracks);
    actions.set(id, { clip });
  }
  const swing = new HeroSwingState();
  mixer.addEventListener('finished', event => {
    const sequence = Number(event.action.getClip().name.slice(5));
    if (sequence >= 23 && sequence <= 26 || sequence === 43) {
      swing.finish(sequence, Boolean(collision.runtime_hero_control_grounded()));
    }
  });
  return { bones, skeleton, mixer, actions, swing, frames: rig.clips, sequence: undefined };
}

function fitTerrainCamera() {
  if (!terrainRadius) return;
  const halfAngle = Math.atan(Math.tan(THREE.MathUtils.degToRad(camera.fov / 2)) * Math.min(camera.aspect, 1));
  const direction = camera.position.clone().sub(controls.target).normalize();
  camera.position.copy(controls.target).addScaledVector(direction, terrainRadius * 1.05 / Math.sin(halfAngle));
}

function resize() {
  const width = innerWidth;
  const height = innerHeight;
  renderer.setSize(width, height, false);
  camera.aspect = width / height;
  camera.updateProjectionMatrix();
  fitTerrainCamera();
}
addEventListener('resize', resize);
resize();

function clearSky() {
  for (const mesh of [...skyGroup.children]) {
    mesh.geometry.dispose();
    for (const material of Array.isArray(mesh.material) ? mesh.material : [mesh.material]) material.dispose();
    skyGroup.remove(mesh);
  }
  for (const texture of textures) texture.dispose();
  textures = [];
}

// Live actors: placed mobys whose transform the native moby update loop can
// change. Ticked at the game's 60 Hz frame rate, like the hero.
let liveActors;
let actorTime;
let actorRemainder = 0;

function updateActors(time) {
  if (actorTime === undefined) actorTime = time;
  const elapsed = Math.min(Math.max((time - actorTime) / 1000, 0), 0.1);
  actorTime = time;
  if (!liveActors || document.hidden) { actorRemainder = 0; return; }
  actorRemainder += elapsed;
  let frame;
  while (actorRemainder >= 1 / 60) {
    frame = tickLevelActors(level, liveActors.count);
    actorRemainder -= 1 / 60;
  }
  if (!frame) return;
  for (const actor of liveActors.groups) {
    if (!actor.visible) continue;
    const moby = frame.mobys[actor.userData.index];
    actor.position.fromArray(moby.position);
    actor.rotation.set(moby.rotation[0], moby.rotation[1], moby.rotation[2], 'ZYX');
    actor.scale.setScalar(moby.scale / actor.userData.modelScale);
  }
  document.body.dataset.actorTicks = String(level.runtime_level_tick_count());
}

function clearTerrain() {
  heroActive = false;
  liveActors = undefined;
  actorRemainder = 0;
  delete document.body.dataset.actorTicks;
  if (heroAnimation) {
    heroAnimation.mixer.stopAllAction();
    heroAnimation.mixer.uncacheRoot(characterGroup);
    heroAnimation.skeleton.dispose();
    heroAnimation = undefined;
  }
  releaseInput();
  terrainRadius = 0;
  collision?.runtime_collision_reset();
  delete document.body.dataset.collisionCells;
  delete document.body.dataset.collisionFaces;
  delete document.body.dataset.characterTriangles;
  delete document.body.dataset.placedMobys;
  delete document.body.dataset.renderedActors;
  for (const mesh of actorGroup.children.flatMap(actor => actor.children)) {
    mesh.geometry.dispose();
    mesh.material.map?.dispose();
    mesh.material.dispose();
  }
  actorGroup.clear();
  scene.fog = null;
  delete document.body.dataset.ties;
  delete document.body.dataset.tieTriangles;
  delete document.body.dataset.shrubs;
  delete document.body.dataset.shrubTriangles;
  for (const mesh of [...terrainGroup.children, ...characterGroup.children, ...tieGroup.children]) {
    mesh.geometry.dispose();
    mesh.material.map?.dispose();
    mesh.material.dispose();
    mesh.removeFromParent();
  }
  characterGroup.position.set(0, 0, 0);
  characterGroup.rotation.set(0, 0, 0);
}

function mobyMaterial(image) {
  const texture = new THREE.DataTexture(image.rgba, image.width, image.height, THREE.RGBAFormat);
  texture.colorSpace = THREE.SRGBColorSpace;
  texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
  texture.magFilter = THREE.LinearFilter;
  texture.minFilter = THREE.LinearMipmapLinearFilter;
  texture.generateMipmaps = true;
  texture.needsUpdate = true;
  return new THREE.MeshBasicMaterial({ map: texture, side: THREE.DoubleSide, alphaTest: 0.1 });
}

// Bind-pose display of the moby table built by the original class registration
// and init_moby_instance (level.wasm). updateActors moves them as ported class
// updates run; classes without model data are logic-only.
// The level's own fog, as a linear ramp: no fog at the near distance, and at the far distance
// the share of fog the far intensity implies. The PS2 computes fog per vertex in VU1; this uses
// the browser's per-pixel linear fog with the same end points.
function showFog(gameplay) {
  const fog = readLevelFog(gameplay.bytes);
  const farShare = 1 - Math.min(Math.max(fog.farIntensity, 0), 255) / 255;
  const colour = new THREE.Color().setRGB(fog.fog[0] / 255, fog.fog[1] / 255, fog.fog[2] / 255, THREE.SRGBColorSpace);
  renderer.setClearColor(new THREE.Color().setRGB(
    fog.background[0] / 255, fog.background[1] / 255, fog.background[2] / 255, THREE.SRGBColorSpace));
  if (farShare > 0 && fog.farDistance > fog.nearDistance) {
    scene.fog = new THREE.Fog(colour, fog.nearDistance, fog.nearDistance + (fog.farDistance - fog.nearDistance) / farShare);
  }
  document.body.dataset.fog = `${fog.fog.join(',')} to ${Math.round(fog.farDistance)} units`;
}

// Static scenery. One instanced mesh per class and texture; see tie-geometry.mjs for what is and
// is not decoded. Brightness is uniform because the baked lighting is not computed yet.
function showTies(planet, gameplay) {
  const failures = [];
  const drawn = showScenery(readTieClasses(planet, failures), readTieInstances(gameplay.bytes), readTieTextures(planet));
  if (failures.length) console.info('Scenery packets left out:', failures);
  document.body.dataset.ties = String(drawn.placed);
  document.body.dataset.tieTriangles = String(drawn.triangles);
}

// Plants, rocks and small props; see shrub-geometry.mjs.
function showShrubs(planet, gameplay) {
  const failures = [];
  const drawn = showScenery(readShrubClasses(planet, failures), readShrubInstances(gameplay.bytes), readShrubTextures(planet));
  if (failures.length) console.info('Shrub packets left out:', failures);
  document.body.dataset.shrubs = String(drawn.placed);
  document.body.dataset.shrubTriangles = String(drawn.triangles);
}

function showScenery(classes, instances, images) {
  const byClass = new Map();
  for (const instance of instances) {
    if (!byClass.has(instance.classId)) byClass.set(instance.classId, []);
    byClass.get(instance.classId).push(instance);
  }
  const materials = new Map();
  const matrix = new THREE.Matrix4();
  const scale = new THREE.Matrix4();
  let triangles = 0;
  let placed = 0;
  for (const [classId, placedInstances] of byClass) {
    const sceneryClass = classes.get(classId);
    if (!sceneryClass) continue;
    for (const batch of sceneryClass.batches) {
      const image = images[batch.texture];
      if (!image) continue;
      if (!materials.has(batch.texture)) materials.set(batch.texture, mobyMaterial(image));
      const geometry = new THREE.BufferGeometry();
      geometry.setAttribute('position', new THREE.BufferAttribute(batch.positions, 3));
      geometry.setAttribute('uv', new THREE.BufferAttribute(batch.uvs, 2));
      const mesh = new THREE.InstancedMesh(geometry, materials.get(batch.texture), placedInstances.length);
      placedInstances.forEach((instance, index) => {
        // The record's rows are the basis vectors and the position; the loader overwrites the
        // last element with the class scale, which the vertices are multiplied by.
        matrix.fromArray(instance.matrix);
        matrix.elements[3] = matrix.elements[7] = matrix.elements[11] = 0;
        matrix.elements[15] = 1;
        scale.makeScale(sceneryClass.scale, sceneryClass.scale, sceneryClass.scale);
        mesh.setMatrixAt(index, matrix.multiply(scale));
      });
      mesh.instanceMatrix.needsUpdate = true;
      mesh.frustumCulled = false;
      tieGroup.add(mesh);
      triangles += batch.positions.length / 9 * placedInstances.length;
    }
    placed += placedInstances.length;
  }
  return { placed, triangles };
}

function showPlacedMobys(planet, actors) {
  const images = readMobyTextures(planet);
  const models = new Map();
  const skipped = new Map();
  let rendered = 0;
  const groups = [];
  for (const moby of actors.mobys) {
    if (moby.classId === 0) continue;
    if (!models.has(moby.classId)) {
      try {
        models.set(moby.classId, readMobyGeometry(planet, moby.classId));
      } catch (error) {
        models.set(moby.classId, null);
        skipped.set(moby.classId, error.message);
      }
    }
    const model = models.get(moby.classId);
    // The native scale is class scale times record scale; the geometry already
    // carries the class scale.
    const scale = model && moby.scale / model.scale;
    if (!model || !(scale > 0 && scale < 100)) continue;
    const actor = new THREE.Group();
    for (const batch of model.batches) {
      const image = images[model.textures[batch.material]];
      if (!image) continue;
      const geometry = new THREE.BufferGeometry();
      geometry.setAttribute('position', new THREE.BufferAttribute(batch.positions, 3));
      geometry.setAttribute('uv', new THREE.BufferAttribute(batch.uvs, 2));
      actor.add(new THREE.Mesh(geometry, mobyMaterial(image)));
    }
    actor.position.fromArray(moby.position);
    actor.rotation.set(moby.rotation[0], moby.rotation[1], moby.rotation[2], 'ZYX');
    actor.scale.setScalar(scale);
    actor.userData = { classId: moby.classId, uid: moby.uid, mission: moby.mission, index: moby.index, modelScale: model.scale };
    actorGroup.add(actor);
    groups.push(actor);
    rendered++;
  }
  if (skipped.size) console.info('Placed classes without browser geometry:', Object.fromEntries(skipped));
  document.body.dataset.placedMobys = String(actors.placed);
  document.body.dataset.initializedMobys = String(actors.count);
  document.body.dataset.renderedActors = String(rendered);
  return groups;
}

function showRatchet(planet, start) {
  const model = readMobyGeometry(planet);
  heroAnimation = createHeroAnimation(readHeroAnimationRig(planet));
  const images = readMobyTextures(planet);
  for (const batch of model.batches) {
    const image = images[model.textures[batch.material]];
    if (!image) throw new Error('Ratchet material references an absent image.');
    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute('position', new THREE.BufferAttribute(batch.positions, 3));
    geometry.setAttribute('uv', new THREE.BufferAttribute(batch.uvs, 2));
    geometry.setAttribute('skinIndex', new THREE.BufferAttribute(batch.joints, 4));
    geometry.setAttribute('skinWeight', new THREE.BufferAttribute(batch.weights, 4));
    const mesh = new THREE.SkinnedMesh(geometry, mobyMaterial(image));
    if (characterGroup.children.length === 0) mesh.add(heroAnimation.bones[0]);
    mesh.bind(heroAnimation.skeleton, new THREE.Matrix4());
    mesh.frustumCulled = false;
    characterGroup.add(mesh);
  }
  for (const [id, { clip }] of heroAnimation.actions) {
    const action = heroAnimation.mixer.clipAction(clip);
    if (id !== 0 && id !== 4 && id !== 13) { action.setLoop(THREE.LoopOnce, 1); action.clampWhenFinished = true; }
    heroAnimation.actions.set(id, action);
  }
  // Original hero start: the class-0 placed moby (load_level_gameplay.c).
  spawn.fromArray(start.position);
  characterGroup.position.copy(spawn);
  controls.target.copy(spawn).add(new THREE.Vector3(0, 0, 0.8));
  terrainRadius = 1.15;
  camera.position.copy(controls.target).add(new THREE.Vector3(3, -1, 0.8));
  controls.minDistance = 1.8;
  controls.maxDistance = 25;
  controls.maxPolarAngle = Math.PI * 0.48;
  controls.enablePan = false;
  fitTerrainCamera();
  controls.update();
  resetHero();
  heroActive = true;
  document.body.dataset.characterTriangles = String(model.triangles);
}

function showTerrain(planet) {
  const terrain = readTerrainGeometry(planet);
  clearTerrain();
  clearSky();
  const images = readTerrainTextures(planet);
  for (const batch of terrain.batches) {
    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute('position', new THREE.BufferAttribute(batch.positions, 3));
    geometry.setAttribute('color', new THREE.BufferAttribute(batch.colours, 3));
    geometry.setAttribute('uv', new THREE.BufferAttribute(batch.uvs, 2));
    const image = images[batch.material];
    if (!image) throw new Error('Terrain material references an absent image.');
    const texture = new THREE.DataTexture(image.rgba, image.width, image.height, THREE.RGBAFormat);
    texture.colorSpace = THREE.SRGBColorSpace;
    texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
    texture.magFilter = THREE.LinearFilter;
    texture.minFilter = THREE.LinearMipmapLinearFilter;
    texture.generateMipmaps = true;
    texture.needsUpdate = true;
    const material = new THREE.MeshBasicMaterial({ map: texture, vertexColors: true, side: THREE.DoubleSide, alphaTest: 0.1 });
    terrainGroup.add(new THREE.Mesh(geometry, material));
  }
  const bounds = new THREE.Box3().setFromObject(terrainGroup);
  const centre = bounds.getCenter(new THREE.Vector3());
  const radius = bounds.getBoundingSphere(new THREE.Sphere()).radius;
  terrainRadius = radius;
  controls.enablePan = true;
  controls.enableZoom = true;
  controls.minDistance = 0.5;
  controls.maxDistance = radius * 5;
  controls.target.copy(centre);
  camera.position.copy(centre).add(new THREE.Vector3(0.7, -1.5, 1).normalize().multiplyScalar(radius * 1.8));
  camera.near = 0.05;
  camera.far = radius * 20;
  camera.updateProjectionMatrix();
  fitTerrainCamera();
  controls.update();
  const header = new DataView(planet.header.buffer, planet.header.byteOffset, planet.header.byteLength);
  const skyOffset = header.getUint32(16, true);
  const skyEnd = header.getUint32(20, true);
  const skyBytes = skyEnd - skyOffset;
  if (skyBytes <= 0 || skyBytes > api.runtime_sky_capacity() || skyEnd > planet.core.length) {
    throw new Error('Planet sky exceeds native staging capacity.');
  }
  api.runtime_sky_reset();
  new Uint8Array(api.memory.buffer, api.runtime_sky_stage_address(), skyBytes).set(planet.core.subarray(skyOffset, skyEnd));
  if (api.runtime_sky_load(skyBytes, 0) !== 1) throw new Error('Native planet sky relocation failed.');
  showSky({ bytes: skyBytes, header: { skyOffset: 0 } });
  skyGroup.scale.setScalar(radius * 4);
  document.body.dataset.triangles = String(terrain.triangles);
  document.body.dataset.fragments = String(terrain.fragments);
}

function showSky(archive) {
  clearSky();
  const base = api.runtime_sky_stage_address();
  const bytes = new Uint8Array(api.memory.buffer, base, archive.bytes);
  const shells = readSkyGeometry(bytes, archive.header.skyOffset, base);
  textures = readSkyTextures(bytes, archive.header.skyOffset, base).map(image => {
    const texture = new THREE.DataTexture(image.rgba, image.width, image.height, THREE.RGBAFormat);
    texture.colorSpace = THREE.SRGBColorSpace;
    texture.wrapS = texture.wrapT = THREE.RepeatWrapping;
    texture.magFilter = THREE.LinearFilter;
    texture.minFilter = THREE.LinearMipmapLinearFilter;
    texture.generateMipmaps = true;
    texture.needsUpdate = true;
    return texture;
  });
  for (const shell of shells) {
    const geometry = new THREE.BufferGeometry();
    geometry.setAttribute('position', new THREE.BufferAttribute(shell.positions, 3));
    geometry.setAttribute('color', new THREE.BufferAttribute(shell.colours, 3));
    geometry.setAttribute('uv', new THREE.BufferAttribute(shell.uvs, 2));
    const material = shell.coloured
      ? new THREE.MeshBasicMaterial({ vertexColors: true, side: THREE.DoubleSide, depthWrite: false })
      : textures.map(texture => new THREE.MeshBasicMaterial({ map: texture, side: THREE.DoubleSide,
        transparent: true, blending: THREE.AdditiveBlending, depthWrite: false }));
    if (!shell.coloured) {
      let start = 0;
      for (let triangle = 1; triangle <= shell.materials.length; triangle++) {
        if (triangle === shell.materials.length || shell.materials[triangle] !== shell.materials[start]) {
          const index = shell.materials[start];
          if (index >= textures.length) throw new Error('Sky triangle references an absent texture.');
          geometry.addGroup(start * 3, (triangle - start) * 3, index);
          start = triangle;
        }
      }
    }
    const mesh = new THREE.Mesh(geometry, material);
    const scales = [1, 3, 2.5, 2, 1.5, 1.25];
    mesh.scale.setScalar(scales[shell.index] ?? 1);
    mesh.rotation.z = shell.index ? shell.index * 1.256637 - Math.PI : 0;
    mesh.renderOrder = shell.index;
    skyGroup.add(mesh);
  }
  document.body.dataset.triangles = String(shells.reduce((sum, shell) => sum + shell.materials.length, 0));
}

async function loadWasm(name) {
  const response = await fetch(`./${name}.wasm`);
  if (!response.ok) throw new Error(`Native module unavailable: ${name}`);
  const module = await WebAssembly.compile(await response.arrayBuffer());
  if (WebAssembly.Module.imports(module).length) throw new Error('Unexpected native runtime imports.');
  const instance = await WebAssembly.instantiate(module, {});
  instance.exports._initialize();
  return instance.exports;
}

disc.addEventListener('change', async () => {
  const file = disc.files[0];
  if (!file) return;
  currentJob?.abort();
  const job = new AbortController();
  currentJob = job;
  music.stop();
  musicLabel = '';
  musicTracks = null;
  interim = null;
  setMenuReady(false);
  clearSky();
  clearTerrain();
  skyGroup.scale.setScalar(1);
  controls.enablePan = false;
  controls.enableZoom = false;
  controls.target.set(0, 0, 0);
  camera.position.set(0, -0.01, 0);
  camera.near = 0.001;
  camera.far = 1000;
  camera.updateProjectionMatrix();
  renderer.setClearColor(0x070a0c);
  document.body.dataset.state = 'loading';
  status.textContent = 'Loading disc...';
  try {
    const reader = new IsoReader(file);
    await reader.open(job.signal);
    const table = await initializeDiscTable(api, reader, job.signal, decoder);
    job.signal.throwIfAborted();
    if (currentJob !== job) return;
    showSky(table.residentArchive);
    status.textContent = 'Loading Veldin...';
    await new Promise(resolve => setTimeout(resolve, 0));
    const planet = await loadPlanetCore(reader, planetDecoder, VELDIN_ARCHIVE, job.signal);
    job.signal.throwIfAborted();
    if (currentJob !== job) return;
    showTerrain(planet);
    const world = initializePlanetCollision(planet, collision);
    status.textContent = 'Loading Veldin gameplay...';
    const gameplay = await loadLevelGameplay(api, reader, decoder, VELDIN_LEVEL, job.signal);
    job.signal.throwIfAborted();
    if (currentJob !== job) return;
    showFog(gameplay);
    showTies(planet, gameplay);
    showShrubs(planet, gameplay);
    status.textContent = 'Initializing Veldin actors...';
    const actors = initializeLevelActors(level, planet, gameplay);
    const groups = showPlacedMobys(planet, actors);
    const start = actors.mobys.find(moby => moby.classId === 0);
    if (!start) throw new Error('The moby table has no class-0 hero start.');
    showRatchet(planet, start);
    document.body.dataset.collisionCells = String(world.cells);
    document.body.dataset.collisionFaces = String(world.faces);
    liveActors = { count: actors.count, groups };
    document.body.dataset.state = 'rendering';
    interim = new InterimGame(actors.mobys);
    status.textContent = 'Veldin loaded / loading music...';
    setMenuReady(true);
    // Music loads after the level is ready; a failure here leaves the level playable.
    try {
      musicTracks = await loadLevelMusic(api, reader, audio, LEVEL_START_TRACK, job.signal);
      if (currentJob !== job) return;
      if (musicTracks) {
        document.body.dataset.musicSeconds = String(Math.round(
          musicTracks.intro.samples.length / musicTracks.intro.sampleRate +
          (musicTracks.body ? musicTracks.body.samples.length / musicTracks.body.sampleRate : 0)));
        if (play === 'playing') music.play(musicTracks);
      } else {
        musicLabel = ' / no music track';
      }
    } catch (error) {
      if (currentJob !== job || error.name === 'AbortError') return;
      musicLabel = ' / music unavailable';
      console.error(error);
    }
    status.textContent = play === 'playing' ? `Veldin / Ratchet${musicStatus()}` : 'Veldin loaded. Choose New game.';
  } catch (error) {
    if (currentJob !== job) return;
    music.stop();
    clearSky();
    clearTerrain();
    document.body.dataset.state = 'error';
    status.textContent = error.message;
    console.error(error);
  }
});

try {
  [api, decoder, planetDecoder, collision, level, audio] = await Promise.all([
    loadWasm('runtime'), loadWasm('wad'), loadWasm('planet-wad'), loadWasm('hero-locomotion'), loadWasm('level'),
    loadWasm('audio'),
  ]);
  if (api.runtime_init() !== 0) throw new Error('Native initialization failed.');
  disc.disabled = false;
  ui['menu-disc'].disabled = false;
  document.body.dataset.state = 'ready';
  status.textContent = 'No disc selected';
  renderer.setAnimationLoop(time => {
    api.runtime_step(Math.round(time * 1000));
    try {
      updateHero(time);
      updateActors(time);
    } catch (error) {
      clearTerrain();
      document.body.dataset.state = 'error';
      status.textContent = error.message;
      console.error(error);
    }
    controls.update();
    skyGroup.position.copy(camera.position);
    renderer.clear();
    renderer.render(skyScene, camera);
    renderer.clearDepth();
    renderer.render(scene, camera);
  });
} catch (error) {
  document.body.dataset.state = 'error';
  status.textContent = error.message;
  console.error(error);
}