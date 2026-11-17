// Client for PCSX2's PINE memory-access socket.
//
// Wire format (little-endian): a request is `u32 total_size` followed by one or
// more commands, each `u8 opcode` plus arguments. A reply is `u32 total_size`,
// `u8 result` (0 = ok, 0xFF = fail), then each command's result back to back.
// Addresses are PS2 EE addresses, so recovered symbols are used unchanged.

import net from 'node:net';
import os from 'node:os';
import path from 'node:path';

export const DEFAULT_SLOT = 28011;

const OP = { read8: 0, read16: 1, read32: 2, read64: 3, write8: 4, write16: 5, write32: 6, write64: 7 };
const WIDTH = { read8: 1, read16: 2, read32: 4, read64: 8, write8: 1, write16: 2, write32: 4, write64: 8 };

/** Where PCSX2 listens for a slot: a unix socket, or localhost TCP on Windows. */
export function pineEndpoint(slot = DEFAULT_SLOT, platform = process.platform, env = process.env) {
    if (platform === 'win32') {
        return { host: '127.0.0.1', port: slot };
    }
    const dir = platform === 'darwin' ? (env.TMPDIR ?? os.tmpdir()) : (env.XDG_RUNTIME_DIR ?? '/tmp');
    const name = slot === DEFAULT_SLOT ? 'pcsx2.sock' : `pcsx2.sock.${slot}`;
    return { path: path.join(dir, name) };
}

function checkAddress(address) {
    if (!Number.isInteger(address) || address < 0 || address > 0xffffffff) {
        throw new RangeError(`bad EE address: ${address}`);
    }
}

/** Encode a batch of `{ op, address, value? }` commands into one request. */
export function encodeBatch(commands) {
    if (commands.length === 0) {
        throw new RangeError('empty PINE batch');
    }
    let size = 4;
    for (const { op } of commands) {
        if (!(op in OP)) {
            throw new RangeError(`unknown PINE op: ${op}`);
        }
        size += 5 + (op.startsWith('write') ? WIDTH[op] : 0);
    }
    const out = Buffer.alloc(size);
    let at = out.writeUInt32LE(size, 0);
    for (const { op, address, value } of commands) {
        checkAddress(address);
        at = out.writeUInt8(OP[op], at);
        at = out.writeUInt32LE(address, at);
        if (op.startsWith('write')) {
            const width = WIDTH[op];
            if (width === 8) {
                at = out.writeBigUInt64LE(BigInt.asUintN(64, BigInt(value)), at);
            } else {
                at = out.writeUIntLE(Number(value) >>> 0 & (width === 4 ? 0xffffffff : (1 << (width * 8)) - 1), at, width);
            }
        }
    }
    return out;
}

/** Decode a reply to `commands`; returns one value per read, undefined per write. */
export function decodeBatch(commands, reply) {
    if (reply.length < 5 || reply.readUInt32LE(0) !== reply.length) {
        throw new Error('malformed PINE reply');
    }
    if (reply.readUInt8(4) !== 0) {
        throw new Error('PINE request failed (is a game running?)');
    }
    let at = 5;
    return commands.map(({ op }) => {
        if (op.startsWith('write')) {
            return undefined;
        }
        const width = WIDTH[op];
        if (at + width > reply.length) {
            throw new Error('short PINE reply');
        }
        const value = width === 8 ? reply.readBigUInt64LE(at) : reply.readUIntLE(at, width);
        at += width;
        return value;
    });
}

const f32 = new Float32Array(1);
const u32 = new Uint32Array(f32.buffer);
export function bitsToFloat(bits) {
    u32[0] = bits;
    return f32[0];
}
export function floatToBits(value) {
    f32[0] = value;
    return u32[0];
}

export class PineClient {
    #socket;
    #pending = [];
    #buffer = Buffer.alloc(0);

    /** Connect to a running PCSX2 with PINE enabled on `slot`. */
    static connect(slot = DEFAULT_SLOT, endpoint = pineEndpoint(slot)) {
        return new Promise((resolve, reject) => {
            const socket = net.createConnection(endpoint);
            socket.once('error', reject);
            socket.once('connect', () => {
                socket.off('error', reject);
                resolve(new PineClient(socket));
            });
        });
    }

    constructor(socket) {
        this.#socket = socket;
        socket.on('data', (chunk) => this.#onData(chunk));
        socket.on('error', (error) => this.#fail(error));
        socket.on('close', () => this.#fail(new Error('PINE socket closed')));
    }

    #onData(chunk) {
        this.#buffer = Buffer.concat([this.#buffer, chunk]);
        while (this.#buffer.length >= 4) {
            const size = this.#buffer.readUInt32LE(0);
            if (size < 5) {
                this.#fail(new Error('malformed PINE reply'));
                return;
            }
            if (this.#buffer.length < size) {
                return;
            }
            const reply = this.#buffer.subarray(0, size);
            this.#buffer = this.#buffer.subarray(size);
            const waiter = this.#pending.shift();
            if (!waiter) {
                continue;
            }
            try {
                waiter.resolve(decodeBatch(waiter.commands, reply));
            } catch (error) {
                waiter.reject(error);
            }
        }
    }

    #fail(error) {
        for (const waiter of this.#pending.splice(0)) {
            waiter.reject(error);
        }
    }

    /** Send several commands in one round trip. */
    batch(commands) {
        return new Promise((resolve, reject) => {
            const request = encodeBatch(commands);
            this.#pending.push({ commands, resolve, reject });
            this.#socket.write(request);
        });
    }

    async read32(address) {
        return (await this.batch([{ op: 'read32', address }]))[0];
    }

    async read16(address) {
        return (await this.batch([{ op: 'read16', address }]))[0];
    }

    /** Read `count` consecutive floats (for example a Vec4 position). */
    async readFloats(address, count) {
        const commands = Array.from({ length: count }, (_, i) => ({ op: 'read32', address: address + i * 4 }));
        return (await this.batch(commands)).map(bitsToFloat);
    }

    async writeFloats(address, values) {
        await this.batch(values.map((value, i) => ({ op: 'write32', address: address + i * 4, value: floatToBits(value) })));
    }

    close() {
        this.#socket.destroy();
    }
}
