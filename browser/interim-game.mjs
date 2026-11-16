// INTERIM gameplay rules for the game page. Temporary, and approved as such by the project owner
// on 2026-11-14 as an exception to AGENTS.md's "no substitute gameplay" rule.
//
// Nothing in this file is recovered game logic. It exists so the page is playable while the
// original code path (see docs/build/native-browser-pipeline.md, "Whole-Tree Spike") is brought
// up; each rule here is to be deleted when the original update for that class runs in the page.
//
// What is real: every position comes from the native moby table built by the original
// init_moby_instance from the disc's placed-moby records, the class ids are the game's, the
// objective actor is the class the original code uses (834, clank_cutscene_trigger_update), and
// the objective text is the game's own mission label.
// What is invented, and marked INTERIM below: reach distances, bolt values, health and its loss.

// Classes, as named by their recovered update functions.
export const CRATE_CLASSES = new Set([500, 501, 511]); // crate_update
export const BOLT_CLASS = 13; // bolt_update
export const OBJECTIVE_CLASS = 834; // clank_cutscene_trigger_update

// Mission-menu label 0x53B2 from the USA disc's help text (docs/build/browser-mission-catalog.md).
export const OBJECTIVE_TEXT = 'Investigate crash site';

// The original trigger polls with radius 3 and update_npc_dialogue reaches at twice the radius.
export const OBJECTIVE_REACH = 6;

// INTERIM values with no source in the recovered code yet.
export const MAX_HEALTH = 4;
const CRATE_REACH = 2.4; // how close a wrench swing must be to break a crate
const BOLT_REACH = 1.6; // how close the hero must be to pick up a placed bolt
const CRATE_BOLTS = 5; // bolts from a class-500 crate
const BOLT_VALUE = 1;

const distance = (a, b) => Math.hypot(a[0] - b[0], a[1] - b[1], a[2] - b[2]);

export class InterimGame {
  // `mobys` is the native moby table as readMobyTable returns it.
  constructor(mobys) {
    this.targets = mobys
      .filter(moby => CRATE_CLASSES.has(moby.classId) || moby.classId === BOLT_CLASS)
      .map(moby => ({ index: moby.index, classId: moby.classId, position: moby.position.slice(), taken: false }));
    const objective = mobys.find(moby => moby.classId === OBJECTIVE_CLASS);
    if (!objective) throw new Error('The level has no class-834 objective actor.');
    this.objective = objective.position.slice();
    this.crateTotal = this.targets.filter(target => CRATE_CLASSES.has(target.classId)).length;
    this.reset();
  }

  reset() {
    for (const target of this.targets) target.taken = false;
    this.bolts = 0;
    this.health = MAX_HEALTH;
    this.cratesBroken = 0;
    this.state = 'playing'; // playing | complete | dead
    this.seconds = 0;
  }

  // Distance from the hero to the objective actor, for the HUD.
  objectiveDistance(hero) { return distance(hero, this.objective); }

  // One fixed tick. `swinging` is true while a wrench swing or slam animation is active.
  // Returns the events of this tick: [{ type: 'crate' | 'bolt' | 'complete', index? }].
  update(hero, swinging, seconds) {
    const events = [];
    if (this.state !== 'playing') return events;
    this.seconds += seconds;
    for (const target of this.targets) {
      if (target.taken) continue;
      const reach = distance(hero, target.position);
      if (target.classId === BOLT_CLASS) {
        if (reach <= BOLT_REACH) {
          target.taken = true;
          this.bolts += BOLT_VALUE;
          events.push({ type: 'bolt', index: target.index });
        }
      } else if (swinging && reach <= CRATE_REACH) {
        target.taken = true;
        this.cratesBroken += 1;
        if (target.classId === 500) this.bolts += CRATE_BOLTS;
        events.push({ type: 'crate', index: target.index });
      }
    }
    if (this.objectiveDistance(hero) <= OBJECTIVE_REACH) {
      this.state = 'complete';
      events.push({ type: 'complete' });
    }
    return events;
  }

  // INTERIM: falling out of the level costs one unit of health.
  fall() {
    if (this.state !== 'playing') return;
    this.health -= 1;
    if (this.health <= 0) {
      this.health = 0;
      this.state = 'dead';
    }
  }
}
