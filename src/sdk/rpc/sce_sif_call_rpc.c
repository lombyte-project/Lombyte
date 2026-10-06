#include "asm.h"

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
struct SifRpcClient
{
  volatile s32 packet;
  volatile s32 request_id;
    s32 semaphore_id;
    u8 pad_C[0x8];
    s32 server_buffer;
    u8 pad_18[0x4];
    s32 end_callback;
  volatile s32 end_argument;
    s32 server;
};
struct SifRpcPacket {
    u8 pad_0[0x14];
    s32 packet_address;
    s32 request_id;
    s32 client;
    s32 rpc_number;
    s32 send_size;
    s32 receive_address;
    s32 receive_size;
    s32 completion_mode;
    s32 server;
};
extern u8 D_00156800[];
extern s32 CreateSema();
extern s32 DeleteSema();
extern s32 WaitSema();
extern s32 get_available_rpc_packet() __asm__("func_0011ACE8");
extern s32 func_0011AD90();
extern s32 sceSifSendCmd();
extern s32 sceSifWriteBackDCache();
s32 sceSifCallRpc(struct SifRpcClient *client, s32 rpc_number, s32 mode, s32 send_buf, s32 send_size, s32 recv_buf,
                  s32 recv_size, s32 end_func, s32 end_data) {
    s32 semaphore_parameters[6];
    struct SifRpcPacket *packet;
    s32 semaphore_id;
    s32 skip_cache_writeback;
    s32 request_id;
    s32 allocation_failure_result;
    packet = (struct SifRpcPacket *)get_available_rpc_packet(D_00156800);
    allocation_failure_result = -1;
    if (packet == 0) {
        return allocation_failure_result;
    }
    request_id = packet->request_id;
    skip_cache_writeback = mode & 2;
    client->end_argument = end_data;
    client->packet = (s32)packet;
    client->request_id = request_id;
    client->end_callback = end_func;
    packet->rpc_number = rpc_number;
    packet->send_size = send_size;
    packet->receive_address = recv_buf;
    packet->receive_size = recv_size;
    packet->packet_address = (s32)packet;
    packet->server = client->server;
    packet->client = (s32)client;
    if (!skip_cache_writeback) {
        if (send_buf == recv_buf) {
            sceSifWriteBackDCache(send_buf, (send_size >= recv_size) ? (send_size) : (recv_size));
        } else {
            if (send_size > 0) {
                sceSifWriteBackDCache(send_buf, send_size);
            }
            if (recv_size > 0) {
                sceSifWriteBackDCache(recv_buf, recv_size);
            }
        }
    }
    if (mode & 1) {
        if (end_func == 0) {
            packet->completion_mode = 0;
        } else {
            packet->completion_mode = 1;
        }
        client->semaphore_id = -1;
        if (sceSifSendCmd(0x8000000A, packet, 0x40, send_buf, client->server_buffer, send_size) != 0) {
            return 0;
        } else {
            func_0011AD90(packet);
            return -2;
        }
    } else {
        semaphore_parameters[1] = 1;
        semaphore_parameters[2] = 0;
        semaphore_id = CreateSema(semaphore_parameters);
        client->semaphore_id = semaphore_id;
        if (semaphore_id < 0) {
            func_0011AD90(packet);
            return -3;
        }
        packet->completion_mode = 1;
        if (sceSifSendCmd(0x8000000A, packet, 0x40, send_buf, client->server_buffer, send_size) == 0) {
            DeleteSema(client->semaphore_id);
            func_0011AD90(packet);
            return -2;
        }
        WaitSema(client->semaphore_id);
        DeleteSema(client->semaphore_id);
        return 0;
    }
}
