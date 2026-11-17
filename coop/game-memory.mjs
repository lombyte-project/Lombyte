// EE addresses and field offsets taken from the recovered headers (US build,
// SCUS-97199). Each entry names the header it came from.

// include/rnc/hero/hero.h: `struct Hero hero` at D_0013F350, 0x2300 bytes.
export const HERO = 0x0013f350;
export const HERO_POS = HERO + 0x80; // motion.pos, Vec4 of f32

// include/rnc/overlay/shared/world/level_labels.h: `u8 *moby_table`.
// The label is shared-overlay L00; other levels are assumed, not verified.
export const MOBY_TABLE_PTR = 0x0015ffd8;

// include/rnc/mobys/moby.h: struct Moby, 0x100 bytes.
export const MOBY_SIZE = 0x100;
export const MOBY_POS = 0x10; // Vec4f
export const MOBY_STATE = 0x20; // u8; >= 0xFE means dead
export const MOBY_OCLASS = 0xa6; // s16

/** Main RAM is 32 MiB; a moby pointer outside it means no level is loaded. */
export function isRamAddress(address) {
    return address >= 0x00100000 && address < 0x02000000;
}

export async function readHeroPosition(pine) {
    const [x, y, z] = await pine.readFloats(HERO_POS, 3);
    return { x, y, z };
}

/** Address of moby `index` in the live table, or null when no level is loaded. */
export async function mobyAddress(pine, index) {
    const table = await pine.read32(MOBY_TABLE_PTR);
    return isRamAddress(table) ? table + index * MOBY_SIZE : null;
}

export async function readMobySummary(pine, address) {
    const [x, y, z] = await pine.readFloats(address + MOBY_POS, 3);
    const oclass = (await pine.read16(address + MOBY_OCLASS)) << 16 >> 16;
    return { address, oclass, x, y, z };
}

export async function writeMobyPosition(pine, address, { x, y, z }) {
    await pine.writeFloats(address + MOBY_POS, [x, y, z]);
}
