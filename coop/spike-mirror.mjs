#!/usr/bin/env node
// First co-op spike: read Ratchet's live position from one PCSX2 and, optionally,
// place a moby at that position in a second PCSX2.
//
//   node coop/spike-mirror.mjs                       print hero position
//   node coop/spike-mirror.mjs --mobys 20            list the first 20 mobys
//   node coop/spike-mirror.mjs --to 28012 --moby 5   mirror into slot 28012's moby 5
//
// The puppet moby keeps running its own update, which may fight these writes.

import { parseArgs } from 'node:util';
import { DEFAULT_SLOT, PineClient } from './pine.mjs';
import { mobyAddress, readHeroPosition, readMobySummary, writeMobyPosition } from './game-memory.mjs';

const { values } = parseArgs({
    options: {
        from: { type: 'string', default: String(DEFAULT_SLOT) },
        to: { type: 'string' },
        moby: { type: 'string' },
        mobys: { type: 'string' },
        hz: { type: 'string', default: '30' },
        offset: { type: 'string', default: '2' },
    },
});

function integer(name, text, min, max) {
    const value = Number(text);
    if (!Number.isInteger(value) || value < min || value > max) {
        throw new RangeError(`--${name} must be an integer from ${min} to ${max}`);
    }
    return value;
}

async function connect(slot) {
    try {
        return await PineClient.connect(slot);
    } catch (error) {
        throw new Error(`cannot reach PCSX2 on PINE slot ${slot} (${error.code ?? error.message}). ` +
            'Start the game and enable PINE with that slot in PCSX2 advanced settings.', { cause: error });
    }
}

async function main() {
    const source = await connect(integer('from', values.from, 1, 65535));

    if (values.mobys !== undefined) {
        const count = integer('mobys', values.mobys, 1, 1024);
        for (let i = 0; i < count; i++) {
            const address = await mobyAddress(source, i);
            if (address === null) {
                throw new Error('no level loaded: moby table pointer is not in RAM');
            }
            const m = await readMobySummary(source, address);
            console.log(`${String(i).padStart(4)}  0x${address.toString(16)}  class ${String(m.oclass).padStart(5)}  ` +
                `${m.x.toFixed(2)} ${m.y.toFixed(2)} ${m.z.toFixed(2)}`);
        }
        source.close();
        return;
    }

    let target = null;
    let mobyIndex = 0;
    if (values.to !== undefined) {
        if (values.moby === undefined) {
            throw new RangeError('--to needs --moby <index>; list candidates with --mobys');
        }
        mobyIndex = integer('moby', values.moby, 0, 4095);
        target = await connect(integer('to', values.to, 1, 65535));
    }
    const offset = Number(values.offset);
    const period = 1000 / integer('hz', values.hz, 1, 120);

    for (;;) {
        const started = Date.now();
        const pos = await readHeroPosition(source);
        let note = '';
        if (target) {
            const address = await mobyAddress(target, mobyIndex);
            if (address === null) {
                note = '  (target has no level loaded)';
            } else {
                await writeMobyPosition(target, address, { x: pos.x + offset, y: pos.y, z: pos.z });
                note = `  -> moby ${mobyIndex}`;
            }
        }
        process.stdout.write(`\rhero ${pos.x.toFixed(2)} ${pos.y.toFixed(2)} ${pos.z.toFixed(2)}${note}   `);
        await new Promise((resolve) => setTimeout(resolve, Math.max(0, period - (Date.now() - started))));
    }
}

main().catch((error) => {
    console.error(`\n${error.message}`);
    process.exit(1);
});
