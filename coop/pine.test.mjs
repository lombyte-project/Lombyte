import assert from 'node:assert/strict';
import fs from 'node:fs';
import net from 'node:net';
import os from 'node:os';
import path from 'node:path';
import test from 'node:test';

import { DEFAULT_SLOT, PineClient, bitsToFloat, decodeBatch, encodeBatch, floatToBits, pineEndpoint } from './pine.mjs';
import { HERO_POS, MOBY_TABLE_PTR, mobyAddress, readHeroPosition, writeMobyPosition } from './game-memory.mjs';

// A stand-in for PCSX2: answers read/write commands against a sparse memory map.
function fakePcsx2(memory) {
    const socketPath = path.join(fs.mkdtempSync(path.join(os.tmpdir(), 'pine-')), 's');
    const server = net.createServer((socket) => {
        let buffer = Buffer.alloc(0);
        socket.on('data', (chunk) => {
            buffer = Buffer.concat([buffer, chunk]);
            while (buffer.length >= 4 && buffer.length >= buffer.readUInt32LE(0)) {
                const size = buffer.readUInt32LE(0);
                const request = buffer.subarray(4, size);
                buffer = buffer.subarray(size);
                const parts = [];
                for (let at = 0; at < request.length;) {
                    const op = request.readUInt8(at);
                    const address = request.readUInt32LE(at + 1);
                    at += 5;
                    const width = 1 << (op & 3);
                    if (op < 4) {
                        const out = Buffer.alloc(width);
                        out.writeUIntLE(memory.get(address) ?? 0, 0, Math.min(width, 6));
                        parts.push(out);
                    } else {
                        memory.set(address, request.readUIntLE(at, Math.min(width, 6)));
                        at += width;
                    }
                }
                const body = Buffer.concat([Buffer.from([0]), ...parts]);
                const header = Buffer.alloc(4);
                header.writeUInt32LE(body.length + 4);
                // Split the reply to exercise reassembly.
                socket.write(header);
                socket.write(body);
            }
        });
    });
    return new Promise((resolve) => server.listen(socketPath, () => resolve({ server, endpoint: { path: socketPath } })));
}

test('endpoint follows PCSX2 socket naming', () => {
    assert.deepEqual(pineEndpoint(DEFAULT_SLOT, 'darwin', { TMPDIR: '/t' }), { path: '/t/pcsx2.sock' });
    assert.deepEqual(pineEndpoint(28012, 'linux', { XDG_RUNTIME_DIR: '/run/u' }), { path: '/run/u/pcsx2.sock.28012' });
    assert.deepEqual(pineEndpoint(28012, 'win32', {}), { host: '127.0.0.1', port: 28012 });
});

test('batch encoding matches the wire format', () => {
    const request = encodeBatch([
        { op: 'read32', address: 0x0013f3d0 },
        { op: 'write32', address: 0x00200010, value: 0x3f800000 },
    ]);
    assert.equal(request.toString('hex'), '12000000' + '02d0f31300' + '06100020000000803f');
    assert.throws(() => encodeBatch([]), RangeError);
    assert.throws(() => encodeBatch([{ op: 'read32', address: -1 }]), RangeError);
    assert.throws(() => encodeBatch([{ op: 'peek', address: 0 }]), RangeError);
});

test('reply decoding rejects failures and truncation', () => {
    const commands = [{ op: 'read32', address: 0 }];
    assert.deepEqual(decodeBatch(commands, Buffer.from('090000000078563412', 'hex')), [0x12345678]);
    assert.throws(() => decodeBatch(commands, Buffer.from('05000000ff', 'hex')), /failed/);
    assert.throws(() => decodeBatch(commands, Buffer.from('0700000000aabb', 'hex')), /short/);
    assert.throws(() => decodeBatch(commands, Buffer.from('0900000000', 'hex')), /malformed/);
});

test('float bit conversion round-trips', () => {
    assert.equal(bitsToFloat(0x3f800000), 1);
    assert.equal(floatToBits(-2.5), 0xc0200000);
});

test('hero read and moby write go through a socket', async () => {
    const memory = new Map([
        [HERO_POS, floatToBits(10.5)],
        [HERO_POS + 4, floatToBits(-3)],
        [HERO_POS + 8, floatToBits(42)],
        [MOBY_TABLE_PTR, 0x00400000],
    ]);
    const { server, endpoint } = await fakePcsx2(memory);
    const pine = await PineClient.connect(DEFAULT_SLOT, endpoint);
    try {
        assert.deepEqual(await readHeroPosition(pine), { x: 10.5, y: -3, z: 42 });
        const address = await mobyAddress(pine, 3);
        assert.equal(address, 0x00400300);
        await writeMobyPosition(pine, address, { x: 1, y: 2, z: 3 });
        assert.equal(memory.get(0x00400310), floatToBits(1));
        assert.equal(memory.get(0x00400318), floatToBits(3));

        memory.set(MOBY_TABLE_PTR, 0);
        assert.equal(await mobyAddress(pine, 0), null);
    } finally {
        pine.close();
        server.close();
    }
});

test('connect reports a missing emulator', async () => {
    await assert.rejects(PineClient.connect(DEFAULT_SLOT, { path: '/nonexistent/pcsx2.sock' }));
});
